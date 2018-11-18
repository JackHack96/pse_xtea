cmake_minimum_required(VERSION 3.10)
project(XTEA)

set(CMAKE_CXX_STANDARD 11)

set(SYSTEMC_HOME /usr/local)

include_directories(include)
include_directories(${SYSTEMC_HOME}/include)

find_library(systemc systemc ${SYSTEMC_HOME}/lib-linux64)
link_directories(${SYSTEMC_HOME}/lib-linux64)

add_executable(XTEA src/XTEA_testbench.cpp src/XTEA_RTL.cpp)
target_link_libraries(XTEA systemc)