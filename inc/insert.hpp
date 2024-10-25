#pragma once
#include "FunctionalUnit.hpp"
#include "PrimateConfig.hpp"

class InsertUnit : public FunctionalUnit
{
private:
    PrimateConfig primateCFG;

public:
    InsertUnit(std::string fpath);
    virtual ~InsertUnit();
    void processInstruction(Instruction &I, MachineState &MS) override;
};