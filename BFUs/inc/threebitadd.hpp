#pragma once
#include "../inc/Instruction.hpp"
#include "../../inc/MachineState.hpp"

// namespace threebitadd
// {
//     struct threebitadd
//     {
//         Register testVal;
//     };
//     // Function declarations
//     void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS, int &index);
//     void (*getFunctionPTR())(Instruction &, MachineState &, MachineState &, int &);
//     Register struct2Reg(struct threebitadd &op);
//     struct threebitadd Reg2struct(const Register &packedData);
//     struct threebitadd threebitadd(struct threebitadd &op);
// }
namespace threebitadd
{
    struct threebitadd
    {
        Register testVal;
    };

    // Function declarations
    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS, int &index);
    void (*getFunctionPTR())(Instruction &, MachineState &, MachineState &, int &);
    Register struct2Reg(const struct threebitadd &op);
    struct threebitadd Reg2struct(const Register &packedData);
    struct threebitadd processthreebitadd(struct threebitadd &op);
}