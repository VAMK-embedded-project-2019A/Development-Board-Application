cmake_minimum_required( VERSION 3.1 )
project( DeviceSoftware )
set (CMAKE_CXX_STANDARD 11)

set( Device_lib
	SOURCE_DIR
)

add_executable(main main.cpp )
target_link_libraries( 
	${Device_lib}
)

add_subdirectory( src )

include_directories( "${PROJECT_SOURCE_DIR}/include" )
