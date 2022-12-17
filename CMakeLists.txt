CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

project(CurveFit)

SET(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-g")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

find_package(Threads REQUIRED)
find_package(GSL REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)

include_directories(
    ${BoostINCLUDE_DIR}
    ./include
    ./bsplines
)

add_subdirectory(./GUI)
add_subdirectory(./src)
