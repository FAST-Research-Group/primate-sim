#include <stdint.h>
#include "extract.hpp"

InsertUnit::InsertUnit(PrimateConfig cfg, bool reg, unsigned slot) : primateCFG(cfg), FunctionalUnit(reg, slot) {}

InsertUnit::~InsertUnit() {}

void InsertUnit::processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    int destination = I.get_rd();
    Register src = CMS.getRegister(I.get_rs1());
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
    // this is wrong. Should be the value at I.get_rs1() not the actual register number
    int mask = immediate & ((1 << clog_size) - 1);
    immediate = immediate >> clog_size;

    int shift = immediate & ((1 << clog_mode) - 1);

    Register result = (src >> shift) & mask;
    NMS.setRegister(destination, result); // shouldn't it be the current state for extract?

    return;
}
