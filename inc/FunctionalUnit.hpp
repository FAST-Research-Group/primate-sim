#pragma once

#include "Instruction.h"
#include "MachineState.hpp"

class FunctionalUnit {
  public:
    virtual ~FunctionalUnit() = default;

    virtual void processInstruction(Instruction &I, MachineState &MS) = 0;
};
