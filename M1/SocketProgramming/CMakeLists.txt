cmake_minimum_required(VERSION 3.17)
project(SocketProgramming C)

set(CMAKE_C_STANDARD 99)

add_executable(SocketProgramming server.c keyValueStore.c keyValueStore.h sub.c sub.h)