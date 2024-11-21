#pragma once
#include "MachineState.hpp"
#include "FunctionalUnit.hpp"

class BFU : public FunctionalUnit
{
private:
    uint64_t pc;
    std::string name;

public:
    // std::map<int, void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)> indexToFunction;
    // std::map<void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index), std::string> functionToName;
    // std::map<std::string, int> nameToIndex;
    BFU(std::string, bool, unsigned);
    virtual ~BFU();
    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS) override;
    std::string getName();
};