#pragma once
#include "FunctionalUnit.hpp"
#include "PrimateConfig.hpp"

class ExtractUnit : public FunctionalUnit
{
private:
    PrimateConfig primateCFG;

public:
    ExtractUnit(std::string fpath);
    virtual ~ExtractUnit();
    void processInstruction(Instruction &I, MachineState &MS) override;
};
