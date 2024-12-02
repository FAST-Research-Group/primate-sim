#include "../../BFUs/inc/threebitadd.hpp"
#include <iostream>

namespace threebitadd
{
    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)
    {
        std::cout << "rs1 index = " << index << std::endl;

        bool regFile = (index < 0) ? 1 : 0;
        struct threebitadd input;

        if (regFile)
        {
            input = Reg2struct(CMS.getRegister(I.get_rs1()));
        }
        else
        {
            input = Reg2struct(CMS.getInterconnectValue(index));
        }

        struct threebitadd output = processthreebitadd(input);
        Register result = struct2Reg(output);

        if (regFile)
        {
            NMS.setRegister(I.get_rd(), result);
        }
        else
        {

            CMS.setInterconnectValue(index + 2, result);
        }
    }

    void (*getFunctionPTR())(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)
    {
        return &processInstruction;
    }
}
