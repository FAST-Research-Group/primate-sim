#include <stdint.h>
#include "ExtractUnit.hpp"
#include "Instruction.hpp"
#include "MachineState.hpp"

ExtractUnit::ExtractUnit(std::string fpath) : primateCFG(fpath) {}

ExtractUnit::~ExtractUnit() {}

void ExtractUnit::processInstruction(Instruction &I, MachineState &MS)
{
    int destination = I.get_rd();
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

    Register result = (source >> shift) & mask;
    MS.setRegister(destination, result);

    return;
}
