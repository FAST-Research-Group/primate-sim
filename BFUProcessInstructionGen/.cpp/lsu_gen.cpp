#include "../inc/lsu.hpp"
namespace lsu
{
    void (*getFunctionPTR())(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)
    {
        return &processInstruction;
    }
    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)
    {
        bool regFile = (index < 0) ? 1 : 0;
        /*This is where the name of your struct goes. Set the name to output (ex: matrix result)*/
        outputs result;
        if (regFile)
        {
            result = Reg2struct(CMS.getRegister(I.get_rs1()));
        }
        else
        {
            result = Reg2struct(CMS.getInterconnectValue(index));
        }
        /* PLACE YOUR FUNCTION HERE WITH THE PROPER PARAMETERS */
        if (I.get_opcode() == 0x03)
        {
            // load and store only supports 32-bits?? therfore going for if - else, will update when more functionality exists
            Register total = CMS.getMem((unsigned)result.answer);
        }
        else
        {
            NMS.setMem((unsigned)I.get_rd(), result.answer);
        }
        // hoping that this is ok...
        Register output = struct2Reg(result);
        if (regFile)
        {
            NMS.setRegister(I.get_rd(), output);
        }
        else
        {
            NMS.setInterconnectValue(index + 2, output);
        }
    }

}
