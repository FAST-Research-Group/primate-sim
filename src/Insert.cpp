#include <stdint.h>
#include "Insert.hpp"
#include "Instruction.hpp"
#include "MachineState.hpp"

InsertUnit::InsertUnit(PrimateConfig cfg, bool reg, unsigned slot) : primateCFG(cfg), FunctionalUnit(reg, slot) {}

InsertUnit::~InsertUnit() {}

void InsertUnit::processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    std::cout << "Insert: " << std::hex << I.get_rawinstruction() << std::endl;
    if (I.get_opcode() == 19)
    {
        // std::cout << "Should be a blue functional unit; otherwise, code brokie. No actual assertion here" << std::endl;
        return;
    }
    int destination = I.get_rd();               // rd
    int source = I.get_rs1();                   // rs1
    int immediate = I.get_immediate();          // FieldSpec
    int mode_size = primateCFG.Src_Mode.size(); // number of modes
    int src_size = primateCFG.Src_Pos.size();   // nmber of positions
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

    // std::cout << std::endl;
    // std::cout << "Attempting to Read Interconnect val at: " << slotIdx - 1 << std::endl;
    Register dest_value = CMS.getInterconnectValue(slotIdx - 1);
    // std::cout << "Read the interconnect value" << std::endl;
    // std::cout << std::endl;
    // std::cout << std::endl;
    // std::cout << std::endl;

    Register value_to_insert = (dest_value & mask) << shift;

    Register clear_mask = ~(((1 << clog_size) - 1) << shift);
    dest_value &= clear_mask;

    dest_value |= value_to_insert;
    // std::cout << "Finished insert" << destination << std::endl; // Helps with debugging
    NMS.setRegister(destination, dest_value);
}