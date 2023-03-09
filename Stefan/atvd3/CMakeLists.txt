cmake_minimum_required(VERSION 3.1)
project( atvd3 )

set(CMAKE_GXX_FLAGS "-Wall -Wextra -WConversion -pedantic -std=c++17")

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( atvd3 hello.cpp )
target_link_libraries( atvd3 ${OpenCV_LIBS} )