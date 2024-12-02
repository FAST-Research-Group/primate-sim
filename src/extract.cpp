#include <stdint.h>
#include "extract.hpp"

ExtractUnit::ExtractUnit(PrimateConfig cfg, bool reg, unsigned slot) : primateCFG(cfg), FunctionalUnit(reg, slot) {}

ExtractUnit::~ExtractUnit() {}

void ExtractUnit::processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    // std::cout << "Extract: " << std::hex << I.get_rawinstruction() << " " << this->slotIdx << std::endl; // Used for debugging
    if (I.get_rawinstruction() == 19)
    {
        return;
    }

    int destination = I.get_rd(); // this line gets ignored since extract writes to the interconnect. This might be a bug, but idk
    Register src = CMS.getRegister(I.get_rs1());
    int source = I.get_rs1();
    int immediate = I.get_immediate();
    int mode_size = primateCFG.Src_Mode.size();
    int pos_size = primateCFG.Src_Pos.size();
    int clog_mode;
    int clog_size;

    while (mode_size > 0)
    {
        clog_mode++;
        mode_size = mode_size >> 1;
    }
    while (pos_size > 0)
    {
        clog_size++;
        pos_size = pos_size >> 1;
    }
    int src_mode_bits = (1 << clog_mode) - 1;
    int src_pos_bits = (1 << clog_size) - 1;

    int INDEX_MODE = immediate & src_mode_bits;
    int INDEX_POS = (immediate >> clog_mode) & src_pos_bits;

    int NUM_MASK_BITS = primateCFG.Src_Mode.at(INDEX_MODE);
    int mask = (1 << NUM_MASK_BITS) - 1;

    int shift = primateCFG.Src_Pos.at(INDEX_POS);

    // this is wrong. Should be the value at I.get_rs1() not the actual register number
    // std::cout << "writing to slot: " << this->slotIdx << std::endl;
    Register result = (src >> shift) & mask;
    // std::cout << result << std::endl;
    CMS.setInterconnectValue(this->slotIdx, result);
    // std::cout << "Writing: " << result << " to: " << this->slotIdx << std::endl;

    return;
}
