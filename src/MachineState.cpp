#include "MachineState.hpp"
#include "Generated/consts.hpp"

void MachineState::halt() { running = false; }

std::ostream& operator<<(std::ostream& OS, const MachineState& thing) {
  OS << "PC: " << thing.pc << "\n";
  for(int i = 0; i < thing.registerFile.size(); i++) {
    OS << "X" << i << "\t" << thing.registerFile[i] << "\n";
  }
  
  return OS;
}
