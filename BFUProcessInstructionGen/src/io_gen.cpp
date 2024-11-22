#include "../BFUs/inc/io.hpp"
namespace io
{
    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)
    {
        // bool regFile = (index < 0) ? 1 : 0;
        // /*This is where the name of your struct goes. Set the name to output (ex: matrix result)*/
        // if (regFile) {
        //     result = Reg2struct(CMS.getRegister(I.get_rs1()));
        // } else {
        //     result = Reg2struct(CMS.getInterconnectValue(index));
        // }
        // /* PLACE YOUR FUNCTION HERE WITH THE PROPER PARAMETERS */
        // Register output = struct2Reg(result);
        // if (regFile) {
        //     NMS.setRegister(I.get_rd(), output);
        // } else {
        //     NMS.setInterconnectValue(index + 2, output);
        // }
    }
    void (*getFunctionPTR())(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)
    {
        return &processInstruction;
    }
}
