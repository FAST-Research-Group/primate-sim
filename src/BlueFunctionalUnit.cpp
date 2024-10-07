#include "BlueFunctionalUnit.hpp"
#include <iostream>

// Constructor
BlueFunctionalUnit::BlueFunctionalUnit() {}

// Destructor
BlueFunctionalUnit::~BlueFunctionalUnit() {}

void BlueFunctionalUnit::processInstruction(Instruction &I, MachineState &MS) {
    // Check the opcode or identifier for blue instructions in I
    // and call the respective handler method
    switch(I.get_opcode()) {
    case 1: // Opcode::BLUEMATCH:
        handleBluematch(I, MS);
        break;
    default:
        std::cerr << "Unknown blue instruction encountered" << std::endl;
        break;
    }
}

void BlueFunctionalUnit::handleBluematch(Instruction &I, MachineState &MS) {

    Register rd = I.get_rd();
    Register rs1 = I.get_rs1();
    int immediate = I.get_immediate();
}