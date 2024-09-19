#pragma once

#include "Register.hpp"
#include <cstdint>
#include <map>
#include <vector>

class MachineState {
  public:
    std::vector<Register> registerFile;
    std::map<uint64_t, Register> memory;

    uint64_t pc;
    bool running;

    MachineState(uint64_t starting_addr);
    uint64_t returnMemory();
    uint64_t readPc();
};

uint64_t MachineState::readPc() { return pc; }