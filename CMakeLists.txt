cmake_minimum_required(VERSION 3.8)

project(raytracer) 

#IMPORTANT COMPILING DIRECTIVES

# 1
# Enable this when you start working on your solution and you add the file RayTracer.h 
# in the src folder; Without this file the code base will not compile therefore in your 
# code this define is disabled
# Also do not forget to create the src folder under code
add_compile_options(-DSTUDENT_SOLUTION)


# 2
# This define is used by the teaching team to compile and run the official solution
# Students: for your project this define should always be commented out
# If we commit it by mistake please disable it and let us know
#add_compile_options(-DCOURSE_SOLUTION)


# When testing large scenes the debug mode will be very slow
# so switch to release
set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_PREFIX_PATH
    /encs # For ENCS lab computers
    /opt/local # Macports
)

include_directories(/usr/include/eigen3)
# If you need too manually add the paths to your Eigen library, you can add it here
# do not delete the other 

# These folders include some important header files including the header files for your own solution
#do not remove
include_directories(src/)
include_directories(external/)

#internal includes
aux_source_directory(external SOURCE)
aux_source_directory(src SOURCE)

add_executable(raytracer main.cpp ${SOURCE}) #The name of the cpp file and its path can vary

