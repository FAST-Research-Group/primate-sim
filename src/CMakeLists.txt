file(GLOB all_SRCS
        "${PrimSim_SOURCE_DIR}/src/*.cpp"
        )
include_directories(${PrimSim_SOURCE_DIR}/inc)
include_directories(${Boost_INCLUDE_DIRS})
add_executable(VLIWParsing ${all_SRCS})
target_link_libraries(VLIWParsing ${Boost_LIBRARIES})