#Sets the minimum required version of cmake for a project
cmake_minimum_required( VERSION 3.1 )

#Sets the name of the project, and stores it in the variable "project" name
project( DeviceSoftware )

#This property specifies the C++ standard whose features are requested to build this target.
set (CMAKE_CXX_STANDARD 11)

#Adds an executable target called <name> to be built from the source files listed in the command invocation.
#add_executable(src main.cpp)

#link_directories(${PROJECT_SOURCE_DIR}/src)

#Add the given directories to those the compiler uses to search for include files.
#Relative paths are interpreted as relative to the current source directory.
#Top level source directory for the current project: "PROJECT_SOURCE_DIR"
#include directory only point to header files location
include_directories(${PROJECT_SOURCE_DIR}/include)

#Add a subdirectory to the build.
#The source_dir specifies the directory in which the source CMakeLists.txt and code files are located.
#The subdirectory only contrains source files code
add_subdirectory( src )
