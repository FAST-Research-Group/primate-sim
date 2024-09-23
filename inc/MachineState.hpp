#pragma once

#include "Register.hpp"
#include <cstdint>
#include <vector>
#include <map>

class MachineState
{
public:
  std::vector<Register> registerFile;
  std::map<uint64_t, Register> memory;

  uint64_t pc;
  bool running;

  uint64_t getPC()
  {
    return pc;
  }

  void setPC(uint64_t newPC)
  {
    pc = newPC;
  }

  Register getMem(uint64_t address)
  {
    return memory.at(address); // Assuming Register has a `value` field
  }

  std::vector<Register> getMem(uint64_t startAddress, uint64_t endAddress)
  {
    std::vector<Register> result;
    for (int i = startAddress; i <= endAddress; i++)
    {
      result.push_back(memory.at(i));
    }
    return result;
  }

  void setMem(uint64_t address, Register value)
  {
    memory[address] = value;
  }

  Register getRegister(int regNum)
  {
    return registerFile.at(regNum);
  }

  std::vector<Register> getRegister(int startReg, int endReg)
  {
    std::vector<Register> result;
    for (int i = startReg; i <= endReg; i++)
    {
      result.push_back(registerFile.at(i));
    }
    return result;
  }

  void setRegister(int regNum, Register value)
  {
    registerFile.at(regNum) = value;
  }

  MachineState(uint64_t starting_addr);
};