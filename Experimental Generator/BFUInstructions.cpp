
#include <map>
#include <string>
#include <iostream>
#include "../inc/BFUInstructions.hpp"
#include "../BFUs/inc/threebitadd.hpp"
#include "../BFUs/inc/lsu.hpp"
#include "../BFUs/inc/io.hpp"
std::map<int, void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)> indexToFunction;
std::map<void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index), std::string> functionToName;
std::map<std::string, int> nameToIndex;


void generateMaps(
    std::map<int, void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)> &indexToFunction,
    std::map<void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index), std::string> &functionToName,
    std::map<std::string, int> &nameToIndex
) {
    // Populate indexToFunction map
    indexToFunction[0] = threebitadd::getFunctionPTR();
    indexToFunction[1] = lsu::getFunctionPTR();
    indexToFunction[2] = io::getFunctionPTR();

    // Populate functionToName map
    functionToName[threebitadd::getFunctionPTR()] = "threebitadd";
    functionToName[lsu::getFunctionPTR()] = "lsu";
    functionToName[io::getFunctionPTR()] = "io";

    // Populate nameToIndex map
    nameToIndex["threebitadd"] = 0;
    nameToIndex["lsu"] = 1;
    nameToIndex["io"] = 2;

}
