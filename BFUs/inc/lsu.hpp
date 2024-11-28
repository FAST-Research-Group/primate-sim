#pragma once
#include "../inc/Instruction.hpp"
#include "../inc/MachineState.hpp"

// namespace lsu
// {
//     typedef struct loadstore
//     {
//         Register answer;
//     } outputs;

//     void processInstruction(Instruction &, MachineState &, MachineState &, int &);
//     void (*getFunctionPTR())(Instruction &, MachineState &, MachineState &, int &);
//     Register struct2Reg(const outputs &);
//     outputs Reg2struct(const Register &);
//     void lsu(Register data, MachineState NMS);
// }
namespace lsu
{
    struct MemOp
    {
        uint64_t address;
        Register value;
        uint8_t operation; // 0 for load, 1 for store
    };
    // Function declarations
    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS, int &index);
    void (*getFunctionPTR())(Instruction &, MachineState &, MachineState &, int &);
    Register struct2Reg(const MemOp &op);
    MemOp Reg2struct(const Register &packedData);
    void processMemOp(MemOp &op, MachineState &CMS, MachineState &NMS);
}