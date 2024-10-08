#include "MachineState.hpp"
#include "consts.hpp"

MachineState::MachineState(uint64_t starting_addr)
    : registerFile(NUM_REGISTERS, 0), // num registers is the only const used?
      memory(), pc(starting_addr), running(true) {}

void MachineState::halt() { running = false; }

std::ostream& operator<<(std::ostream& OS, const MachineState& thing) {
  OS << "PC: " << thing.pc << "\n";
  for(int i = 0; i < thing.registerFile.size(); i++) {
    OS << "X" << i << "\t" << thing.registerFile[i] << "\n";
  }
  
  return OS;
}
