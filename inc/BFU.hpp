#pragma once

#include "FunctionalUnit.hpp"

class BFU : public FunctionalUnit
{
    BFU(bool, unsigned);
    virtual ~BFU();
    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS) override;
};