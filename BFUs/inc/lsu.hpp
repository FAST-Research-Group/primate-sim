#pragma once
#include "../inc/Instruction.hpp"
#include "../inc/MachineState.hpp"

namespace lsu
{
    typedef struct loadstore
    {
        Register answer;
    } outputs;

    void processInstruction(Instruction &, MachineState &, MachineState &, int &);
    void (*getFunctionPTR())(Instruction &, MachineState &, MachineState &, int &);
    Register struct2Reg(const outputs &);
    outputs Reg2struct(const Register &);
}
