#include <stdint.h>
#include "extract.hpp"

ExtractUnit::ExtractUnit(PrimateConfig cfg, bool reg, unsigned slot) : primateCFG(cfg), FunctionalUnit(reg, slot) {}

ExtractUnit::~ExtractUnit() {}

void ExtractUnit::processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    // std::cout << "We made it here, with Opcode:" << I.get_rawinstruction() << std::endl;
    if (I.get_rawinstruction() == 19)
    {
        return;
    }
    Register src;
    int destination = I.get_rd();
    // std::cout << "WE MADE IT HERE" << std::endl;
    if (this->isConnectedToRegisterFile())
    {
        // std::cout << "Reading from Reg File" << std::endl;
        src = CMS.getRegister(I.get_rs1());
        // std::cout << "Made it past this." << std::endl;
    }
    else
    {
        // std::cout << "Reading from Interconnect" << std::endl;
        src = CMS.getInterconnectValue(I.get_rs1());
    }

    int source = I.get_rs1();
    int immediate = I.get_immediate();
    int mode_size = primateCFG.Src_Mode.size();
    int src_size = primateCFG.Src_Pos.size();
    int clog_mode;
    int clog_size;

    while (mode_size > 0)
    {
        clog_mode++;
        mode_size = mode_size >> 1;
    }
    while (src_size > 0)
    {
        clog_size++;
        src_size = src_size >> 1;
    }

    int mask = immediate & ((1 << clog_size) - 1);
    immediate = immediate >> clog_size;

    int shift = immediate & ((1 << clog_mode) - 1);

    Register result = (src >> shift) & mask;
    // std::cout << "Destination Register is: " << destination << " and Result is: " << result << std::endl;
    // NMS.setRegister(destination, 0);
    // std::cout << "Instruction finished. Result is:  " << NMS.getRegister(destination) << std::endl;
    NMS.setInterconnectValue(destination, result); // was setRegister
    // std::cout << "Instruction finished: " << NMS.getInterconnectValue(destination) << std::endl;
    return;
}
