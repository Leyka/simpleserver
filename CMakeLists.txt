cmake_minimum_required(VERSION 3.13)
project(webserver C)

set(CMAKE_C_STANDARD 99)

add_executable(webserver src/main.c src/network.c src/network.h)