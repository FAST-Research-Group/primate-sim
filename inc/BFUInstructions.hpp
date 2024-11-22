#pragma once
#include <map>
#include "../inc/Instruction.hpp"
#include "../inc/MachineState.hpp"
#include <string>

extern std::map<int, void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)> indexToFunction;
extern std::map<void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index), std::string> functionToName;
extern std::map<std::string, int> nameToIndex;

void generateMaps(std::map<int, void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)> &,
                  std::map<void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index), std::string> &,
                  std::map<std::string, int> &);