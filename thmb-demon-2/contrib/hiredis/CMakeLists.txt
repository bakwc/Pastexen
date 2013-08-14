
add_library(hiredis async.c dict.c fmacros.h hiredis.c net.c sds.c)
target_link_libraries(hiredis
                        ws2_32)
