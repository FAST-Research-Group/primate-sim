#pragma once
#include "FunctionalUnit.hpp"
#include "PrimateConfig.hpp"

class ExtractUnit : public FunctionalUnit
{
private:
    PrimateConfig primateCFG;

public:
    ExtractUnit(PrimateConfig cfg, bool, unsigned);
    virtual ~ExtractUnit();
    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS) override;
};