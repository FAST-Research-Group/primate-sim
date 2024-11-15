#include <stdint.h>
#include "Insert.hpp"
#include "Instruction.hpp"
#include "MachineState.hpp"

InsertUnit::InsertUnit(PrimateConfig cfg, bool reg, unsigned slot) : primateCFG(cfg), FunctionalUnit(reg, slot) {}

InsertUnit::~InsertUnit() {}

void InsertUnit::processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    // std::cout << "Insert: " << std::hex << I.get_rawinstruction() << std::endl; // used for debugging
    if (I.get_opcode() == 19)
    {
        // std::cout << "Should be a blue functional unit; otherwise, code brokie. No actual assertion here" << std::endl;
        return;
    }

    int destination = I.get_rd();               // rd
    int source = I.get_rs1();                   // rs1
    int immediate = I.get_immediate();          // FieldSpec
    int mode_size = primateCFG.Src_Mode.size(); // number of modes
    int pos_size = primateCFG.Src_Pos.size();   // nmber of positions
    int clog_mode = 0;
    int clog_size = 0;

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
    // std::cout << "INDEX MODE is: " << INDEX_MODE << std::endl; // used for debugging

    int INDEX_POS = (immediate >> clog_mode) & src_pos_bits;
    // std::cout << "INDEX POS is: " << INDEX_POS << std::endl; // used for debugging

    int NUM_MASK_BITS = primateCFG.Src_Mode.at(INDEX_MODE);
    // std::cout << "Mask is this many bits: " << NUM_MASK_BITS << std::endl; // used for debugging

    int mask = (1 << NUM_MASK_BITS) - 1;

    int shift = primateCFG.Src_Pos.at(INDEX_POS);

    Register Interconnect_Value = CMS.getInterconnectValue(slotIdx - 1);
    // std::cout << "Value from GFU is: " << CMS.getInterconnectValue(slotIdx - 1) << std::endl; // used for debugging

    Register result_to_write_to_reg_file = Interconnect_Value & mask;

    int clear_mask = ~(mask << shift);

    Register destination_reg_value = CMS.getRegister(destination);
    destination_reg_value = destination_reg_value & clear_mask;
    destination_reg_value = destination_reg_value | (result_to_write_to_reg_file << shift);

    NMS.setRegister(destination, destination_reg_value);
}