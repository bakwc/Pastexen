
add_executable (thmb_demon main.cpp redis_task.cpp thumb_generator.cpp defines.h)
target_link_libraries(thmb_demon
                        hiredis
                        lodepng
                        jpeg
                        utils
                        ${Boost_LIBRARIES})
