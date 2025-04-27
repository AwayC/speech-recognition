cmake_minimum_required(VERSION 3.30)
project(Speech_recognition)

set(CMAKE_CXX_STANDARD 11)

add_library(dtw dtw/dtw.cpp dtw/dtw.h)
