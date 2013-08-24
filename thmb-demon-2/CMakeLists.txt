cmake_minimum_required(VERSION 2.6)
project(pastexen-thumb-demon)

set(CMAKE_CXX_FLAGS "-std=c++0x")

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost 1.48.0 COMPONENTS locale system filesystem thread chrono REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}
                    ${Boost_INCLUDE_DIRS})

link_directories (${PROJECT_BINARY_DIR}/thmb_demon
                  ${PROJECT_BINARY_DIR}/contrib/hiredis
                  ${PROJECT_BINARY_DIR}/contrib/lodepng
                  ${PROJECT_BINARY_DIR}/utils
                  ${Boost_LIBRARIES})

add_subdirectory(contrib)
add_subdirectory(utils)
add_subdirectory(thmb_demon)
