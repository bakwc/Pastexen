/* Extracted from anet.c to work properly with Hiredis error reporting.
 *
 * Copyright (c) 2006-2011, Salvatore Sanfilippo <antirez at gmail dot com>
 * Copyright (c) 2010-2011, Pieter Noordhuis <pcnoordhuis at gmail dot com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *   * Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of Redis nor the names of its contributors may be used
 *     to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "fmacros.h"
#include <sys/types.h>
#include <stdlib.h>

#ifdef _WIN32
  #ifndef FD_SETSIZE
    #define FD_SETSIZE 16000
  #endif
//  #include <winsock2.h>
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  #define socklen_t int
  #ifndef EINPROGRESS
    #define EINPROGRESS WSAEWOULDBLOCK
  #endif
#else
  #include <sys/socket.h>
  #include <sys/select.h>
  #include <sys/un.h>
  #include <netinet/in.h>
  #include <netinet/tcp.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <netdb.h>
  #include <poll.h>
#endif

#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>
#include <limits.h>

#include "net.h"
#include "sds.h"

/* Defined in hiredis.c */
void __redisSetError(redisContext *c, int type, const char *str);

static void __redisSetErrorFromErrno(redisContext *c, int type, const char *prefix) {
    char buf[128];
    size_t len = 0;

    if (prefix != NULL)
        len = snprintf(buf,sizeof(buf),"%s: ",prefix);
#ifdef _WIN32
    WSAGetLastError();
#else
    strerror_r(errno,buf+len,sizeof(buf)-len);
#endif
    __redisSetError(c,type,buf);
}

#ifdef _WIN32
void startWinsock__(void) {
	/* This is needed, because otherwise all of socket calls will fail under
     * Windows, as the WSA subsystem wasn't initialized for our process. */
    WSADATA t_wsa;
    WORD wVers = MAKEWORD(2, 2); // Set the version number to 2.2
    int iError = WSAStartup(wVers, &t_wsa);

    if(iError != NO_ERROR || LOBYTE(t_wsa.wVersion) != 2 || HIBYTE(t_wsa.wVersion) != 2 ) {
       printf("Winsock2 init error: %d\n", iError);
       exit(1);
    }

    atexit((void(*)(void)) WSACleanup);
}

void startWinsock(void)
{
    static bool done = false;

    if (done) {
        return;
    }

    startWinsock__();
    done = true;
}
#endif

#ifndef _WIN32
static int redisSetReuseAddr(redisContext *c, int fd) {
    int on = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
        close(fd);
        return REDIS_ERR;
    }
    return REDIS_OK;
}
#endif

#ifdef _WIN32
static int redisCreateSocket(redisContext *c, int type) {
    SOCKET s;
    int on=1;

    startWinsock();

    s = socket(type, SOCK_STREAM, IPPROTO_TCP);
    if (s == INVALID_SOCKET) {
        __redisSetError(c,REDIS_ERR_IO,sdscatprintf(sdsempty(), "socket error: %d\n", WSAGetLastError()));
        return REDIS_ERR;
    }
    if (type == AF_INET) {
        LINGER l;
        l.l_onoff = 1;
        l.l_linger = 2;
        setsockopt(s, SOL_SOCKET, SO_LINGER, (const char *) &l, sizeof(l));

        if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (const char *) &on, sizeof(on)) == -1) {
            __redisSetError(c,REDIS_ERR_IO,NULL);
            closesocket(s);
            return REDIS_ERR;
        }
    }
    return (int)s;
}
#else
static int redisCreateSocket(redisContext *c, int type) {
    int s;
    if ((s = socket(type, SOCK_STREAM, 0)) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
        return REDIS_ERR;
    }
    if (type == AF_INET) {
        if (redisSetReuseAddr(c,s) == REDIS_ERR) {
            return REDIS_ERR;
        }
    }
    return s;
}
#endif

#ifndef _WIN32
static int redisSetBlocking(redisContext *c, int fd, int blocking) {
    int flags;

    /* Set the socket nonblocking.
     * Note that fcntl(2) for F_GETFL and F_SETFL can't be
     * interrupted by a signal. */
    if ((flags = fcntl(fd, F_GETFL)) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,"fcntl(F_GETFL)");
        close(fd);
        return REDIS_ERR;
    }

    if (blocking)
        flags &= ~O_NONBLOCK;
    else
        flags |= O_NONBLOCK;

    if (fcntl(fd, F_SETFL, flags) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,"fcntl(F_SETFL)");
        close(fd);
        return REDIS_ERR;
    }
    return REDIS_OK;
}
#endif

static int redisSetTcpNoDelay(redisContext *c, int fd) {
    int yes = 1;
#ifdef _WIN32
    if (setsockopt((SOCKET)fd, IPPROTO_TCP, TCP_NODELAY, (const char *)&yes, sizeof(yes)) == -1) {
#else
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &yes, sizeof(yes)) == -1) {
#endif
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,"setsockopt(TCP_NODELAY)");
        close(fd);
        return REDIS_ERR;
    }
    return REDIS_OK;
}

#define __MAX_MSEC (((LONG_MAX) - 999) / 1000)

#ifdef _WIN32
static int redisContextWaitReady(redisContext *c, int fd, const struct timeval *timeout) {
    struct timeval to;
    struct timeval *toptr = NULL;
    fd_set wfd;
    int err;
    socklen_t errlen;

    /* Only use timeout when not NULL. */
    if (timeout != NULL) {
        to = *timeout;
        toptr = &to;
    }

    if (errno == EINPROGRESS) {
        FD_ZERO(&wfd);
        FD_SET((SOCKET)fd, &wfd);

        if (select(FD_SETSIZE, NULL, &wfd, NULL, toptr) == -1) {
            __redisSetError(c,REDIS_ERR_IO,
                sdscatprintf(sdsempty(), "select(2): %s", strerror(errno)));
            closesocket(fd);
            return REDIS_ERR;
        }

        if (!FD_ISSET(fd, &wfd)) {
            errno = WSAGetLastError();
            __redisSetError(c,REDIS_ERR_IO,NULL);
            closesocket(fd);
            return REDIS_ERR;
        }

        err = 0;
        errlen = sizeof(err);
        if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (char *)&err, &errlen) == SOCKET_ERROR) {
            __redisSetError(c,REDIS_ERR_IO,
                sdscatprintf(sdsempty(), "getsockopt(SO_ERROR): %d", WSAGetLastError()));
            closesocket(fd);
            return REDIS_ERR;
        }
        if (err) {
            errno = err;
            __redisSetError(c,REDIS_ERR_IO,NULL);
            closesocket(fd);
            return REDIS_ERR;
        }

        return REDIS_OK;
    }

    __redisSetError(c,REDIS_ERR_IO,NULL);
    closesocket(fd);
    return REDIS_ERR;
}
#else
static int redisContextWaitReady(redisContext *c, int fd, const struct timeval *timeout) {
    struct pollfd   wfd[1];
    long msec;

    msec          = -1;
    wfd[0].fd     = fd;
    wfd[0].events = POLLOUT;

    /* Only use timeout when not NULL. */
    if (timeout != NULL) {
        if (timeout->tv_usec > 1000000 || timeout->tv_sec > __MAX_MSEC) {
            close(fd);
            return REDIS_ERR;
        }

        msec = (timeout->tv_sec * 1000) + ((timeout->tv_usec + 999) / 1000);

        if (msec < 0 || msec > INT_MAX) {
            msec = INT_MAX;
        }
    }

    if (errno == EINPROGRESS) {
        int res;

        if ((res = poll(wfd, 1, msec)) == -1) {
            __redisSetErrorFromErrno(c, REDIS_ERR_IO, "poll(2)");
            close(fd);
            return REDIS_ERR;
        } else if (res == 0) {
            errno = ETIMEDOUT;
            __redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
            close(fd);
            return REDIS_ERR;
        }

        if (redisCheckSocketError(c, fd) != REDIS_OK)
            return REDIS_ERR;

        return REDIS_OK;
    }

    __redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
    close(fd);
    return REDIS_ERR;
}
#endif

#ifdef _WIN32
int redisCheckSocketError(redisContext *c, int fd) {
    int err = 0;
    socklen_t errlen = sizeof(err);

    if (getsockopt(fd, SOL_SOCKET, SO_ERROR, (char *)&err, &errlen) == SOCKET_ERROR) {
        __redisSetError(c,REDIS_ERR_IO,
            sdscatprintf(sdsempty(), "getsockopt(SO_ERROR): %d", WSAGetLastError()));
        closesocket(fd);
        return REDIS_ERR;
    }

    if (err) {
        errno = err;
        __redisSetError(c,REDIS_ERR_IO,NULL);
        closesocket(fd);
        return REDIS_ERR;
    }

    return REDIS_OK;
}
#else
int redisCheckSocketError(redisContext *c, int fd) {
    int err = 0;
    socklen_t errlen = sizeof(err);

    if (getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &errlen) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,"getsockopt(SO_ERROR)");
        close(fd);
        return REDIS_ERR;
    }

    if (err) {
        errno = err;
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,NULL);
        close(fd);
        return REDIS_ERR;
    }

    return REDIS_OK;
}
#endif

int redisContextSetTimeout(redisContext *c, struct timeval tv) {
#ifdef _WIN32
    if (setsockopt(c->fd,SOL_SOCKET,SO_RCVTIMEO,(const char *)&tv,sizeof(tv)) == SOCKET_ERROR ) {
        __redisSetError(c,REDIS_ERR_IO,
            sdscatprintf(sdsempty(), "setsockopt(SO_RCVTIMEO): %d",  WSAGetLastError()));
        return REDIS_ERR;
    }
    if (setsockopt(c->fd,SOL_SOCKET,SO_SNDTIMEO,(const char *)&tv,sizeof(tv)) == SOCKET_ERROR ) {
        __redisSetError(c,REDIS_ERR_IO,
            sdscatprintf(sdsempty(), "setsockopt(SO_SNDTIMEO): %d",  WSAGetLastError()));
        return REDIS_ERR;
    }
#else
    if (setsockopt(c->fd,SOL_SOCKET,SO_RCVTIMEO,&tv,sizeof(tv)) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,"setsockopt(SO_RCVTIMEO)");
        return REDIS_ERR;
    }
    if (setsockopt(c->fd,SOL_SOCKET,SO_SNDTIMEO,&tv,sizeof(tv)) == -1) {
        __redisSetErrorFromErrno(c,REDIS_ERR_IO,"setsockopt(SO_SNDTIMEO)");
        return REDIS_ERR;
    }
#endif
    return REDIS_OK;
}

#ifdef _WIN32
int redisContextConnectTcp(redisContext *c, const char *addr, int port, struct timeval *timeout) {
    int s;
    int blocking = (c->flags & REDIS_BLOCK);
    struct sockaddr_in sa;

    if ((s = redisCreateSocket(c,AF_INET)) < 0)
        return REDIS_ERR;

    sa.sin_family = AF_INET;
    sa.sin_port = htons(port);
    unsigned long inAddress;

    if (redisSetTcpNoDelay(c,s) != REDIS_OK)
        return REDIS_ERR;

    inAddress = inet_addr(addr);
    if (inAddress == INADDR_NONE || inAddress == INADDR_ANY) {
        struct hostent *he;

        he = gethostbyname(addr);
        if (he == NULL) {
            __redisSetError(c,REDIS_ERR_OTHER,
                sdscatprintf(sdsempty(),"Can't resolve: %s", addr));
            closesocket(s);
            return REDIS_ERR;;
        }
        memcpy(&sa.sin_addr, he->h_addr, sizeof(struct in_addr));
    }
    else {
      sa.sin_addr.s_addr = inAddress;
    }

    if (connect((SOCKET)s, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
        errno = WSAGetLastError();
        if ((errno == WSAEINVAL) || (errno == WSAEWOULDBLOCK))
            errno = EINPROGRESS;
        if (errno == EINPROGRESS && !blocking) {
            /* This is ok. */
        } else {
            if (redisContextWaitReady(c,s,timeout) != REDIS_OK)
                return REDIS_ERR;
        }
    }

    /* Reset socket to be blocking after connect(2). */
    c->fd = s;
    c->flags |= REDIS_CONNECTED;
    return REDIS_OK;
}
#else
int redisContextConnectTcp(redisContext *c, const char *addr, int port, struct timeval *timeout) {
    int s, rv;
    char _port[6];  /* strlen("65535"); */
    struct addrinfo hints, *servinfo, *p;
    int blocking = (c->flags & REDIS_BLOCK);

    snprintf(_port, 6, "%d", port);
    memset(&hints,0,sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(addr,_port,&hints,&servinfo)) != 0) {
        __redisSetError(c,REDIS_ERR_OTHER,gai_strerror(rv));
        return REDIS_ERR;
    }
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((s = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) == -1)
            continue;

        if (redisSetBlocking(c,s,0) != REDIS_OK)
            goto error;
        if (connect(s,p->ai_addr,p->ai_addrlen) == -1) {
            if (errno == EHOSTUNREACH) {
                close(s);
                continue;
            } else if (errno == EINPROGRESS && !blocking) {
                /* This is ok. */
            } else {
                if (redisContextWaitReady(c,s,timeout) != REDIS_OK)
                    goto error;
            }
        }
        if (blocking && redisSetBlocking(c,s,1) != REDIS_OK)
            goto error;
        if (redisSetTcpNoDelay(c,s) != REDIS_OK)
            goto error;

        c->fd = s;
        c->flags |= REDIS_CONNECTED;
        rv = REDIS_OK;
        goto end;
    }
    if (p == NULL) {
        char buf[128];
        snprintf(buf,sizeof(buf),"Can't create socket: %s",strerror(errno));
        __redisSetError(c,REDIS_ERR_OTHER,buf);
        goto error;
    }

error:
    rv = REDIS_ERR;
end:
    freeaddrinfo(servinfo);
    return rv;  // Need to return REDIS_OK if alright
}
#endif

int redisContextConnectUnix(redisContext *c, const char *path, struct timeval *timeout) {
#ifdef _WIN32
    (void) timeout;
    __redisSetError(c,REDIS_ERR_IO,
        sdscatprintf(sdsempty(),"Unix sockets are not suported on Windows platform. (%s)\n", path));

    return REDIS_ERR;
#else
    int s;
    int blocking = (c->flags & REDIS_BLOCK);
    struct sockaddr_un sa;

    if ((s = redisCreateSocket(c,AF_LOCAL)) < 0)
        return REDIS_ERR;
    if (redisSetBlocking(c,s,0) != REDIS_OK)
        return REDIS_ERR;

    sa.sun_family = AF_LOCAL;
    strncpy(sa.sun_path,path,sizeof(sa.sun_path)-1);
    if (connect(s, (struct sockaddr*)&sa, sizeof(sa)) == -1) {
        if (errno == EINPROGRESS && !blocking) {
            /* This is ok. */
        } else {
            if (redisContextWaitReady(c,s,timeout) != REDIS_OK)
                return REDIS_ERR;
        }
    }

    /* Reset socket to be blocking after connect(2). */
    if (blocking && redisSetBlocking(c,s,1) != REDIS_OK)
        return REDIS_ERR;

    c->fd = s;
    c->flags |= REDIS_CONNECTED;
    return REDIS_OK;
#endif
}
