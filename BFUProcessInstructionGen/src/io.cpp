#include "../../BFUs/inc/io.hpp"
namespace io
{
    void processInstruction(Instruction &I,
                            MachineState &CMS,
                            MachineState &NMS,
                            int &index)
    {
        switch (I.get_opcode())
        {
        case IO_UNIT_INPUT:
            handleInput(I, CMS, NMS, index);
            break;
        case IO_UNIT_OUTPUT:
            handleOutputEmiti(I, CMS, NMS, index);
            break;
        default:
            throw std::string("Unknown custom instruction.");
            break;
        }
    }
    void (*getFunctionPTR())(Instruction &I,
                             MachineState &CMS,
                             MachineState &NMS,
                             int &index)
    {
        return &processInstruction;
    }
}
