
cmake_minimum_required(VERSION 3.0.0) # 版本
project(CALC) # 名称
set(CMAKE_CXX_STANDARD 17) # c17
add_executable(server main.cpp redis.cpp socket.cpp)
