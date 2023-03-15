cmake_minimum_required(VERSION 3.7)
project(SmartPi VERSION 0.1.0 LANGUAGES CXX)


set(CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD 14)

file(GLOB_RECURSE ALLFILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
add_executable(SmartPi ${ALLFILES})
target_link_libraries(SmartPi 
    sfml-system
    sfml-graphics
    sfml-window
    boost_date_time)
