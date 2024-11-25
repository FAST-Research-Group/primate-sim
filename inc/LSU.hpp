#ifndef LSU_HPP
#define LSU_HPP

#include "Instruction.hpp"
#include "MachineState.hpp"

namespace LSU
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

#endif