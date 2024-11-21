
#include <map>
#include <string>
#include <iostream>
#include "MatMul.hpp"
#include "../inc/lsu.hpp"
#include "../inc/io.hpp"
#include "BFUInstructions.hpp"

std::map<int, void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)> indexToFunction;
std::map<void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index), std::string> functionToName;
std::map<std::string, int> nameToIndex;

void generateMaps(
    std::map<int, void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)> &indexToFunction,
    std::map<void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index), std::string> &functionToName,
    std::map<std::string, int> &nameToIndex)
{
    // Populate indexToFunction map
    indexToFunction[0] = MatMul::getFunctionPTR();
    indexToFunction[1] = lsu::getFunctionPTR();
    indexToFunction[2] = io::getFunctionPTR();

    // Populate functionToName map
    functionToName[MatMul::getFunctionPTR()] = "MatMul";
    functionToName[lsu::getFunctionPTR()] = "lsu";
    functionToName[io::getFunctionPTR()] = "io";

    // Populate nameToIndex map
    nameToIndex["MatMul"] = 0;
    nameToIndex["lsu"] = 1;
    nameToIndex["io"] = 2;
}
