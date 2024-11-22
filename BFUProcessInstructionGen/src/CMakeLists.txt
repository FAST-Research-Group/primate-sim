file(GLOB BFUProcessInstruction_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Add library for BFUProcessInstruction
add_library(BFUProcessInstruction ${BFUProcessInstruction_SOURCES})

# Link Boost libraries
target_link_libraries(BFUProcessInstruction ${Boost_LIBRARIES})

# Include headers if needed
target_include_directories(BFUProcessInstruction PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../inc)
