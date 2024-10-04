cmake_minimum_required(VERSION 3.10)  

project(AD9361 CXX)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("/usr/include/iio")

set(HEADER_FILES
    iio/iio.h
    ../ad9361/ad9361.hpp
    ..sream/iiostream-common.hpp
    iio/iio-debug.h
    # ad9361.h 
    #device_api.h
)

set(SOURCES
    ../ad9361/ad9361.cpp
    ../stream/iiostream-common.cpp
    # device_api.c
    ../src/main.cpp
)


set(TX
    ../ad9361/ad9361.cpp
    ../stream/iiostream-common.cpp
    # device_api.c
    ../src/tx.cpp
)

add_executable(rx ${SOURCES})
add_executable(tx ${TX})


target_link_libraries(rx iio)
target_link_libraries(tx iio)