#pragma once

#include <memory>

#include "FunctionalUnit.hpp"

class MergedUnit: public FunctionalUnit {

    std::unique_ptr<FunctionalUnit> aluInst;
    std::unique_ptr<FunctionalUnit> bfuInst;

public:
    MergedUnit(std::unique_ptr<FunctionalUnit> bfu, std::unique_ptr<FunctionalUnit> alu, bool reg, unsigned slotIdx): 
            aluInst(std::move(alu)), 
            bfuInst(std::move(bfu)), 
            FunctionalUnit(reg, slotIdx) {}

    void processInstruction(Instruction& I, MachineState& CMS, MachineState& NMS) {
        if(I.isGFUInst()) {
            aluInst->processInstruction(I, CMS, NMS);
        }
        else {
            bfuInst->processInstruction(I, CMS, NMS);
        }
    }
};