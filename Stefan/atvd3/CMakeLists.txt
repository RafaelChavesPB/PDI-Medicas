cmake_minimum_required(VERSION 3.1)
project( atvd3 )

set(CMAKE_GXX_FLAGS "-Wall -Wextra -WConversion -pedantic -std=c++17")

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( atvd3 main.cpp ../helpers/helpers.cpp)
set_target_properties(atvd3 PROPERTIES OUTPUT_NAME "atvd3.o")

target_link_libraries( atvd3 ${OpenCV_LIBS} )