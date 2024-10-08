#ifndef MACHINESTATE_H
#define MACHINESTATE_H

#include "Register.hpp"
#include <ostream>
#include <cstdint>
#include <vector>
#include <map>

struct MachineState
{
  std::vector<Register> registerFile;
  std::map<uint64_t, Register> memory;

  uint64_t pc;
  bool running;

  void halt();

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

  void setMem(uint64_t address, Register value, int num_bytes)
  {
    switch (num_bytes)
    {
    case 1:
      memory[address] = value & 0x0FF;
    case 2:
      memory[address] = value & 0x0FF;
      memory[address + 1] = (value >> 8) & 0x0FF;
    case 4:
    default:
      memory[address] = value & 0x0FF;
      memory[address + 1] = (value >> 8) & 0x0FF;
      memory[address + 2] = (value >> 16) & 0x0FF;
      memory[address + 3] = (value >> 24) & 0x0FF;
    }
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

  friend std::ostream& operator<<(std::ostream& OS, const MachineState& thing);
};

#endif
