cmake_minimum_required(VERSION 3.30)
project(Speech_recognition)

set(CMAKE_CXX_STANDARD 11)

add_library(dtw dtw/dtw.cpp dtw/dtw.h)

add_executable(test_dtw test/dtw/test_dtw.cpp)

# 修改这一行，指定头文件所在的目录
target_include_directories(test_dtw PRIVATE dtw)
target_link_libraries(test_dtw dtw)