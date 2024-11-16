#pragma once
#include "MachineState.hpp"
#include "FunctionalUnit.hpp"

class BFU : public FunctionalUnit
{
private:
    uint64_t pc;
    std::string name;

public:
    BFU(std::string, bool, unsigned);
    virtual ~BFU();
    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS) override;
    std::string getName();
};