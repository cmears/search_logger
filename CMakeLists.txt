cmake_minimum_required(VERSION 3.0)
project(search_logger)

include(FindProtobuf)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


find_package(Protobuf REQUIRED)
find_package(ZeroMQ REQUIRED)

include_directories(${PROTOBUF_INCLUDE_DIR})
include_directories(${ZEROMQ_INCLUDE_DIRS})

set(SOURCE_FILES
    search_logger.cpp
    message.pb.cpp
    zmq.hpp)

add_executable(search_logger ${SOURCE_FILES})
target_link_libraries(search_logger ${ZEROMQ_LIBRARIES})
target_link_libraries(search_logger ${PROTOBUF_LIBRARY})


