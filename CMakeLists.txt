cmake_minimum_required(VERSION 2.8)
project(opencv_project)
find_package(OpenCV REQUIRED)
find_package(tesseract CONFIG REQUIRED)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

find_package( PkgConfig REQUIRED)
set(CMAKE_PREFIX_PATH "/home/arman/tesseract/cmake/templates")
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${TESSERACT_INCLUDE_DIRS})
include_directories(${LEPTONICA_INCLUDE_DIRS})
link_directories(${TESSERACT_LIBRARY_DIRS})
link_directories(${LEPTONICA_LIBRARY_DIRS})
add_executable(proj proj.cpp)
target_link_libraries(proj ${OpenCV_LIBS})
target_link_libraries(DisplayImage ${TESSERACT_LIBRARIES})
target_link_libraries(DisplayImage ${LEPTONICA_LIBRARIES})
