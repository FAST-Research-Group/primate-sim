// THIS FILE IS A TEST OF WHAT I THINK A USER'S CODE SHOULD LOOK LIKE WHEN FEEDING IT INTO PRIMATE

#pragma once
#include "FunctionalUnit.hpp"
#include "PrimateConfig.hpp"

class MatMul : public FunctionalUnit
{
private:
    PrimateConfig primateCFG;

public:
    MatMul(PrimateConfig cfg, bool, unsigned);
    virtual ~MatMul();
    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS) override;
};