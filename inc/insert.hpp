#pragma once
#include "FunctionalUnit.hpp"
#include "PrimateConfig.hpp"

class InsertUnit : public FunctionalUnit
{
private:
    PrimateConfig primateCFG;

public:
    InsertUnit(std::string fpath, bool, unsigned);
    virtual ~InsertUnit();
    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS) override;
};