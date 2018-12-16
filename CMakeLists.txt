cmake_minimum_required(VERSION 3.10)
project(XTEA)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)
include_directories($ENV{SYSTEMC_HOME}/include)

find_library(systemc systemc $ENV{SYSTEMC_HOME}/lib-linux64)
link_directories($ENV{SYSTEMC_HOME}/lib-linux64)

# Generate RTL binary
add_executable(XTEA_RTL src/RTL/XTEA_RTL.cpp src/RTL/XTEA_RTL_testbench.cpp src/RTL/main_XTEA_RTL.cpp)
target_link_libraries(XTEA_RTL systemc)

# Generate TLM UT binary
add_executable(XTEA_TLM_UT src/TLM_UT/XTEA_TLM_UT.cpp src/TLM_UT/XTEA_TLM_UT_testbench.cpp src/TLM_UT/main_XTEA_TLM_UT.cpp)
target_link_libraries(XTEA_TLM_UT systemc)