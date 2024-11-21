#include <stdint.h>
#include "../inc/BFU.hpp"
#include "../inc/BFUInstructions.hpp"

BFU::BFU(std::string func, bool reg, unsigned slot) : name(func), FunctionalUnit(reg, slot) { pc = 0; }
BFU::~BFU() {};
std::string BFU::getName()
{
    return this->name;
}

void BFU::processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    int num;
    if (!(this->isConnectedToRegisterFile()))
    {
        num = this->slotIdx - 2;
    }
    else
    {
        num = -1;
    }
    indexToFunction[nameToIndex[getName()]](I, CMS, NMS, num);
}