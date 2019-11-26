#Sets the minimum required version of cmake for a project
cmake_minimum_required( VERSION 3.1 )

#Sets the name of the project, and stores it in the variable "project" name
project( DeviceSoftware )

#This property specifies the C++ standard whose features are requested to build this target.
set (CMAKE_CXX_STANDARD 11)

#Adds an executable target called <name> to be built from the source files listed in the command invocation.
add_executable(main main.cpp )

#Specify libraries or flags to use when linking a given target and/or its dependents. 
#Usage requirements from linked library targets will be propagated. 
#Usage requirements of a target’s dependencies affect compilation of its own sources.
target_link_libraries(main
	SOURCE_DIR
)

#Add the given directories to those the compiler uses to search for include files. 
#Relative paths are interpreted as relative to the current source directory.
include_directories( "${PROJECT_SOURCE_DIR}/include" )

#Add a subdirectory to the build. 
#The source_dir specifies the directory in which the source CMakeLists.txt and code files are located.
add_subdirectory( src )


