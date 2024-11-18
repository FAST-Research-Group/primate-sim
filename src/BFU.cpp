#include <stdint.h>
#include "../inc/BFU.hpp"
#include "../src/main.cpp"

BFU::BFU(std::string func, bool reg, unsigned slot) : name(func), FunctionalUnit(reg, slot) { pc = 0; }
BFU::~BFU() {};
std::string BFU::getName()
{
    return this->name;
}

void BFU::processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    int index;
    if (!(this->isConnectedToRegisterFile()))
    {
        index = this->slotIdx - 2;
    }
    else
    {
        index = -1;
    }
    indexToFunction[nameToIndex[getName()]](I, CMS, NMS, index);
}