cmake_minimum_required( VERSION 3.1 )
project( DeviceSoftware )
set (CMAKE_CXX_STANDARD 11)

set( Device_lib
	SOURCE_DIR
	INCLUDE_DIR
)

add_executable( .cpp )
target_link_libraries( 
	${Device_lib}
)

include_directories( "${CURDIR}/src" )
add_subdirectory( src )

include_directories( "${CURDIR}/include" )
add_subdirectory( include )