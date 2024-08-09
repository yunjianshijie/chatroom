# 这是一个 CMakeLists.txt 文件
cmake_minimum_required(VERSION 3.5.0) # 版本
project(CALC) # 名称
set(CMAKE_CXX_STANDARD 17) # c17
# include_directories(/usr/include)
# include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(client main.cpp 
 ui/user_long.cpp  
 socket.cpp socket1.cpp 
 socket2.cpp 
 ../include/read.cpp
 group.cpp)
include(FetchContent)
