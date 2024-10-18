cmake_minimum_required(VERSION 3.10)  

project(AD9361 CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories("/usr/include/iio")

set(HEADER_FILES
    iio/iio.h
    ../ad9361/ad9361.hpp
    ../log_level/log.hpp
    ../stream/iiostream-common.hpp
    iio/iio-debug.h
    #../src/correlation/time_corr.hpp
    ../src/correlation/time_corr.hpp
    # ad9361.h 
    #device_api.h
)


set(RX
    ../log_level/log.cpp
    ../ad9361/ad9361.cpp
    ../src/correlation/time_corr.cpp
    #../stream/iiostream-common.cpp
    # device_api.c
    ../src/test/rx.cpp
)


set(TX
    ../log_level/log.cpp
    ../ad9361/ad9361.cpp
    ../src/correlation/time_corr.cpp
    # ../stream/iiostream-common.cpp
    # device_api.c
    ../src/test/tx.cpp

)


# set(TRX
#     ../log_level/log.cpp
#     ../ad9361/ad9361.cpp
#     ../stream/iiostream-common.cpp
#     # device_api.c
#     ../src/trx.cpp

# )

add_executable(rx ${RX})
add_executable(tx ${TX})
# add_executable(trx ${TRX})

target_link_libraries(rx iio)
target_link_libraries(tx iio)
# target_link_libraries(trx iio)