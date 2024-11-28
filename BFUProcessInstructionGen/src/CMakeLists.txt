# Glob all source files in the current directory
file(GLOB BFUProcessInstruction_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Create a library target for BFUProcessInstruction
add_library(BFUProcessInstruction ${BFUProcessInstruction_SOURCES})

# Link Boost libraries to BFUProcessInstruction
target_link_libraries(BFUProcessInstruction PUBLIC ${Boost_LIBRARIES})

# Include headers for BFUProcessInstruction
target_include_directories(BFUProcessInstruction PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../inc)

# Ensure BFUProcessInstruction depends on BFUs library
target_link_libraries(BFUProcessInstruction PUBLIC BFUs)
