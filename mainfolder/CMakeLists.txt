cmake_minimum_required(VERSION 3.5)
project(mainfolder)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    include/camera.h
    include/color.h
    include/light.h
    include/object.h
        include/parser.h
    include/plane.h
    include/ray.h
    include/sphere.h
    include/vect.h
    main.cpp)

add_executable(mainfolder ${SOURCE_FILES})