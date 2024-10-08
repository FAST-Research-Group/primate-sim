#ifndef FUNCTIONALUNIT_H
#define FUNCTIONALUNIT_H

#include "Instruction.hpp"
#include "MachineState.hpp"

class FunctionalUnit
{
public:
  virtual ~FunctionalUnit() {};

  virtual void processInstruction(Instruction &I, MachineState &MS)= 0;
};

#endif
