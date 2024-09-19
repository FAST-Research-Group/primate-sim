#include "MachineState.hpp"
#include "Instruction.h"

class FunctionalUnit {
  virtual void processInstruction(Instruction& I, MachineState& MS) = 0;
};