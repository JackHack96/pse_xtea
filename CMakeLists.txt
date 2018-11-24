cmake_minimum_required(VERSION 3.10)
project(XTEA)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories($ENV{SYSTEMC_HOME}/include)

find_library(systemc systemc $ENV{SYSTEMC_HOME}/lib-linux64)
link_directories($ENV{SYSTEMC_HOME}/lib-linux64)

add_executable(XTEA src/XTEA_RTL.cpp src/XTEA_RTL_testbench.cpp src/main_XTEA_RTL.cpp)
target_link_libraries(XTEA systemc)