#include "IOUnit.hpp"
#include <iostream>

// Constructor
IOUnit::IOUnit(bool regFileCon, unsigned slot) : FunctionalUnit(regFileCon, slot), inputStream("input.txt")
{
}

// Destructor
IOUnit::~IOUnit() {}

void IOUnit::processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    if (I.get_opcode() == 19)
    {
        return;
    }
    switch (I.get_opcode())
    {
    case IO_UNIT_INPUT:
        handleInput(I, CMS, NMS);
        break;
    case IO_UNIT_OUTPUT:
        handleOutputEmiti(I, CMS, NMS);
        break;
    default:
        throw std::string("Unknown custom intruction.");
        break;
    }
}

void IOUnit::handleInput(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    if (I.get_opcode() == 19)
    {
        return;
    }
    switch (I.get_funct3())
    {
    case 1:
        handleInputRead(I, CMS, NMS);
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

void IOUnit::handleInputRead(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    int rd = I.get_rd();
    int imm = I.get_immediate();
    int holdInput = 0;
    char isLast, hasData;
    char seperatorTrash;
    std::string temp;
    inputStream >> isLast >> seperatorTrash >> hasData >> seperatorTrash >> temp;
    for (int i = 0; i < imm; ++i)
        holdInput += (temp[i] - '0') << i;
    NMS.setRegister(rd, holdInput);
}

void IOUnit::handleOutputEmiti(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    int rd = I.get_rd();
    int imm = I.get_immediate();
    int mask = 0;
    for (int i = 1; i < imm; i++)
    {
        mask += 1 * i;
    }
    std::cout << (CMS.getRegister(rd) & mask);
}