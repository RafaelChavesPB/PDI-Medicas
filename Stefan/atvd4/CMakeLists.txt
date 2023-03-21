cmake_minimum_required(VERSION 3.1)
project( atvd4 )

set(CMAKE_GXX_FLAGS "-Wall -Wextra -WConversion -pedantic -std=c++17")

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( atvd4 main.cpp ../helpers/helpers.cpp)
set_target_properties(atvd4 PROPERTIES OUTPUT_NAME "atvd4.o")

target_link_libraries( atvd4 ${OpenCV_LIBS} )