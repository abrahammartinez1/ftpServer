cmake_minimum_required(VERSION 3.27)
project(ftpServer C)

set(CMAKE_C_STANDARD 11)

add_executable(ftpServer main.c)
target_link_libraries(ftpServer ws2_32)
