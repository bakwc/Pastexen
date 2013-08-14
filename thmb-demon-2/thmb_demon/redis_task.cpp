#include "redis_task.h"

#include <assert.h>
#include <contrib/hiredis/hiredis.h>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

TTaskException::TTaskException(const string& message)
    : Message(message)
{
}

TTaskException::~TTaskException() throw() {
}

const char* TTaskException::what() const throw() {
    return Message.c_str();
}


class TRedisTaskManagerImpl {
public:
    TRedisTaskManagerImpl(const string& host, uint16_t port, size_t dbNum) {
        Context = NULL;
        Context = redisConnect(host.c_str(), port);
        if (!Context) {
            throw TConnectionError();
        }
        SelectDatabase(dbNum);
    }
    optional<TTask> GetTask() {
        redisReply *reply;
        string command = "SRANDMEMBER thmb_tasks";
        reply = (redisReply*)redisCommand(Context, command.c_str());
        if (!reply) {
            throw TConnectionError();
        }
        if (reply->type == REDIS_REPLY_NIL) {
            return optional<TTask>();
        }
        if (reply->type != REDIS_REPLY_STRING) {
            throw TUnexpectedResponseError();
        }
        TTask task;
        task.FileId = reply->str;
    }
private:
    void SelectDatabase(size_t dbNum) {
        redisReply *reply;
        string command = (boost::format("SELECT %1%") % dbNum).str();
        reply = (redisReply*)redisCommand(Context, command.c_str());
        if (!reply) {
            throw TConnectionError();
        }
        if (reply->type == REDIS_REPLY_ERROR) {
            throw TSelectDbError();
        }
        freeReplyObject(reply);
    }

private:
    redisContext* Context;
};

TRedisTaskManager::TRedisTaskManager(const string& host, uint16_t port, size_t dbNum)
    : Impl(new TRedisTaskManagerImpl(host, port, dbNum))
{
}

TRedisTaskManager::~TRedisTaskManager() {
}

optional<TTask> TRedisTaskManager::GetTask() {
    return Impl->GetTask();
}
