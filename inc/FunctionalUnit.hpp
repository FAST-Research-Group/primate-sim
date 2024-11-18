#pragma once

#include "Instruction.hpp"
#include "MachineState.hpp"

class FunctionalUnit
{
  bool consumeRegfile;

public:
  int slotIdx;
  FunctionalUnit(bool cRF, unsigned SI) : consumeRegfile(cRF), slotIdx(SI) {}
  virtual ~FunctionalUnit() {}

  bool isConnectedToRegisterFile() { return consumeRegfile; }

  virtual void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS) = 0;
};
