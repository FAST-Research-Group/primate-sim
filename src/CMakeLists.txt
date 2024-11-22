file(GLOB all_SRCS "${PrimSim_SOURCE_DIR}/src/*.cpp")
file(GLOB ExperimentalGenerator_SOURCE "${PrimSim_SOURCE_DIR}/Experimental Generator/BFUInstructions.cpp")
# Add executable
add_executable(VLIWParsing ${all_SRCS})
target_sources(VLIWParsing PRIVATE ${ExperimentalGenerator_SOURCE})

# Link with Boost and other libraries
target_link_libraries(VLIWParsing
    ${Boost_LIBRARIES}
    BFUs
    BFUProcessInstruction
)

# Include directories for headers
target_include_directories(VLIWParsing PUBLIC
    ${PrimSim_SOURCE_DIR}/inc
    ${PrimSim_SOURCE_DIR}/ExperimentalGenerator
    target_include_directories(VLIWParsing PUBLIC ${PrimSim_SOURCE_DIR}/ExperimentalGenerator)
)
