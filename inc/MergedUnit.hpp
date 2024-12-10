#pragma once

#include <memory>
#include "alu.hpp"
#include "BFU.hpp"
#include "FunctionalUnit.hpp"

class MergedUnit : public FunctionalUnit
{
    std::unique_ptr<ALU> aluInst;
    std::unique_ptr<BFU> bfuInst;

    // std::unique_ptr<FunctionalUnit> aluInst;
    // std::unique_ptr<FunctionalUnit> bfuInst;

public:
    MergedUnit(std::unique_ptr<BFU> bfu, std::unique_ptr<ALU> alu, bool reg, unsigned slotIdx) : aluInst(std::move(alu)),
                                                                                                 bfuInst(std::move(bfu)),
                                                                                                 FunctionalUnit(reg, slotIdx) {}

    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS)
    {
        if (I.isGFUInst())
        {
            aluInst->processInstruction(I, CMS, NMS);
        }
        else
        {
            std::cout << "Merged Unit. We got here" << std::endl;
            bfuInst->processInstruction(I, CMS, NMS);
        }
    }
};