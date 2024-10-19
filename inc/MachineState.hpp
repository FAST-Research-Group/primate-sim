#ifndef MACHINESTATE_H
#define MACHINESTATE_H

#include <sstream>
#include <ostream>
#include <cstdint>
#include <vector>
#include <map>
#include <utility>

#include "PrimateConfig.hpp"
#include "Register.hpp"

struct MachineState
{
  PrimateConfig& configFile;
  std::vector<Register> registerFile;
  std::vector<std::pair<bool, Register>> interconnect;
  std::map<uint64_t, Register> memory;

  uint64_t pc;
  bool running;

  void halt();

  MachineState(uint64_t starting_addr, PrimateConfig& cfg): interconnect(cfg.instruction_width, std::make_pair<bool, Register>(false, 0)), 
                                                            registerFile(0, cfg.Num_Regs), 
                                                            pc(starting_addr),
                                                            running(true),
                                                            configFile(cfg) {};
  MachineState(const MachineState& other): configFile(other.configFile),
                                           registerFile(other.registerFile), 
                                           memory(other.memory), 
                                           pc(other.pc), running(other.running),
                                           interconnect(configFile.instruction_width, std::make_pair<bool, Register>(false, 0)) {}
  MachineState& operator=(const MachineState& other) {
    registerFile = other.registerFile;
    configFile = other.configFile;
    memory = other.memory;
    pc = other.pc;
    running = other.running;
    interconnect = other.interconnect;

    return *this;
  }

  Register getInterconnectValue(uint64_t registerIdx) {
    auto& ret = interconnect[registerIdx];
    assert(ret.first && "Reading from an uninited interconnect slot!!!");
    return ret.second;
  }
  void setInterconnectValue(uint64_t registerIdx, Register val) {
    interconnect[registerIdx] = {true, val};
  }

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
    try
    {
      return memory.at(address); // Assuming Register has a `value` field
    }
    catch (const std::out_of_range &e)
    {
      throw std::runtime_error("Attempted to access uninitialized memory at address: " + std::to_string(address) + "\nIn Instruction at packet:/ " + std::to_string(pc));
    }
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

  friend std::ostream &operator<<(std::ostream &OS, const MachineState &thing);
};

#endif
