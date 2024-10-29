#pragma once
#include "FunctionalUnit.hpp"
#include "PrimateConfig.hpp"

class InsertUnit : public FunctionalUnit
{
private:
    PrimateConfig primateCFG;

public:
    InsertUnit(PrimateConfig cfg, bool, unsigned);
    virtual ~InsertUnit();
    void processInstruction(Instruction &I, MachineState &MS, MachineState &NMS) override;
};