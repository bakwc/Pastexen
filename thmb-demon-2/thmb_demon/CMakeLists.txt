
add_executable (thmb_demon main.cpp redis_task.cpp)
target_link_libraries(thmb_demon
                        hiredis
                        lodepng
                        ${Boost_LIBRARIES})
