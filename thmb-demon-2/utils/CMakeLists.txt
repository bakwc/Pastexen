add_library(utils ucast.cpp ucolonsep.cpp uexception.cpp usettings.cpp string.cpp)
target_link_libraries(utils iconv ${Boost_LIBRARIES})
