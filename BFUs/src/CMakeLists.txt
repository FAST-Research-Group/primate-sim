# Glob all source files in the current directory
file(GLOB BFUs_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Create a library target for BFUs
add_library(BFUs ${BFUs_SOURCES})

# Link Boost libraries to BFUs
target_link_libraries(BFUs PUBLIC ${Boost_LIBRARIES})

# Include headers for BFUs
target_include_directories(BFUs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../inc)
