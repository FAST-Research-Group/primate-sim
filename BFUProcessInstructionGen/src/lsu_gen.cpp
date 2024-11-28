#include "../BFUs/inc/lsu.hpp"
namespace lsu
{
    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)
    {

        bool regFile = (index < 0);
        MemOp memOp;
        // Extract the MemOp struct
        if (regFile)
        {
            memOp = Reg2struct(CMS.getRegister(I.get_rs1()));
        }
        else
        {
            memOp = Reg2struct(CMS.getInterconnectValue(index));
        }
        // Process the entire MemOp struct
        processMemOp(memOp, CMS, NMS);
        // Pack the updated struct back into a Register
        Register output = struct2Reg(memOp);
        // Write the output
        if (regFile)
        {
            NMS.setRegister(I.get_rd(), output);
        }
        else
        {
            NMS.setInterconnectValue(index + 2, output);
        }
    }
    void (*getFunctionPTR())(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)
    {
        return &processInstruction;
    }
}
