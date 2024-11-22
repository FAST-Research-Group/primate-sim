file(GLOB BFUs_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Add library for BFUs
add_library(BFUs ${BFUs_SOURCES})

# Link Boost libraries
target_link_libraries(BFUs ${Boost_LIBRARIES})

# Include headers in BFUs/inc
target_include_directories(BFUs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../inc)
