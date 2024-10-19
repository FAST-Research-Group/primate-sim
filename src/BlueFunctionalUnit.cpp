#include "BlueFunctionalUnit.hpp"
#include <iostream>

// Constructor
BFU::BFU() {}

// Destructor
BFU::~BFU() {}

void BFU::processInstruction(Instruction &I, MachineState &MS) {
    switch(I.get_opcode()) {
    case BFU_INPUT:
        handleInput(I, MS);
        break;
    case BFU_OUTPUT:
        handleOutputEmiti(I, MS);
        break;
    default:
        throw std::string("Unknown custom intruction.");
        break;
    }
}

void BFU::handleInput(Instruction &I, MachineState &MS) {
    switch(I.get_funct3()) {
    case 1:
        handleInputRead(I, MS);
        break;
    case 2:
        // handleInputSeek not implemented
        throw std::string("inputSeek is not an implemented instruction.");
        break;
    case 3:
        throw std::string("inputExtracti is not an implemented instruction.");
        break;
    case 4:
        throw std::string("inputDone is not an implemented instruction.");
        break;
    case 7:
        throw std::string(
            "inputExtractiDone is not an implemented instruction.");
        break;
    case 5:
    case 6:
    default:
        throw std::string("Not a custom instruction.");
        break;
    }
}

void BFU::handleInputRead(Instruction &I, MachineState &MS) {
    int rd = I.get_rd();
    int imm = I.get_immediate();
    int holdInput = 0;
    std::string temp = MS.inputStream.str();
    for(int i = 0; i < imm; ++i)
        holdInput += (temp[i] - '0') << i;
    MS.setRegister(rd, holdInput);
    temp.erase(0, imm);
    MS.inputStream.str("");
    MS.inputStream.clear();
    MS.inputStream << temp;
}

void handleOutputEmiti(Instruction &I, MachineState &MS) {}