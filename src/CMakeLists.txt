file(GLOB_RECURSE ALLFILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*)
add_executable(SmartPi ${ALLFILES})
target_link_libraries(SmartPi 
    sfml-system
    sfml-graphics
    sfml-window
    boost_date_time
    curl
)
