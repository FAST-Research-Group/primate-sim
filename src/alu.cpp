#include <stdint.h>
#include "alu.hpp"

BranchUnit::BranchUnit(bool reg, unsigned slot) : FunctionalUnit(reg, slot) { pc = 0; }

BranchUnit::~BranchUnit() {}

uint64_t BranchUnit::j(uint64_t imm) { return pc + imm; }

uint64_t BranchUnit::jr(uint64_t imm) { return imm; }

uint64_t BranchUnit::beq(uint64_t a, uint64_t b) { return a == b ? 1 : 0; }

uint64_t BranchUnit::bne(uint64_t a, uint64_t b) { return a != b ? 1 : 0; }

uint64_t BranchUnit::blt(uint64_t a, uint64_t b) { return a < b ? 1 : 0; }

uint64_t BranchUnit::bge(uint64_t a, uint64_t b) { return a >= b ? 1 : 0; }

uint64_t BranchUnit::bltu(uint64_t a, uint64_t b) { return a < b ? 1 : 0; }

uint64_t BranchUnit::bgeu(uint64_t a, uint64_t b) { return a >= b ? 1 : 0; }

uint64_t BranchUnit::end() { return 0; }

void BranchUnit::processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    // std::cout << "BranchUnit: " << std::hex << I.get_rawinstruction() << std::endl; // Used for debugging
    if (I.get_rawinstruction() == 19)
    {
        NMS.setPC(CMS.getPC() + 1);
        return;
    }
    Register rs1; // assumes that the registers hold the slot indexes
    Register rs2;
    switch (I.get_type())
    {
    case Instruction::type::J:
    {
        // std::cout << "found a jump with target: " << std::hex << I.get_immediate() << "\n"
        // << std::dec;
        if (I.get_immediate() == -1)
        {
            NMS.halt();
            break;
        }
        // FIXME: Might be wrong.
        NMS.setPC(CMS.getPC() + I.get_immediate());
        break;
    }
    case Instruction::type::B:
        rs1 = CMS.getInterconnectValue(I.get_rs1());
        rs2 = CMS.getInterconnectValue(I.get_rs2());
        switch (I.get_funct3())
        {
        case 0:
            (I.get_rs1() == I.get_rs2()) ? NMS.setPC(CMS.getPC() + I.get_immediate()) : NMS.setPC(CMS.getPC() + 1);
            break;
        case 1:
            (I.get_rs1() != I.get_rs2()) ? NMS.setPC(CMS.getPC() + I.get_immediate()) : NMS.setPC(CMS.getPC() + 1);
            break;
        case 4:
            (I.get_rs1() < I.get_rs2()) ? NMS.setPC(CMS.getPC() + I.get_immediate()) : NMS.setPC(CMS.getPC() + 1);
            break;
        case 6:
            (((unsigned)I.get_rs1()) < ((unsigned)I.get_rs2())) ? NMS.setPC(CMS.getPC() + I.get_immediate()) : NMS.setPC(CMS.getPC() + 1);
            break;
        case 5:
            (I.get_rs1() >= I.get_rs2()) ? NMS.setPC(CMS.getPC() + I.get_immediate()) : NMS.setPC(CMS.getPC() + 1);
            break;
        case 7:
            (((unsigned)I.get_rs1()) >= ((unsigned)I.get_rs2())) ? NMS.setPC(CMS.getPC() + I.get_immediate()) : NMS.setPC(CMS.getPC() + 1);
            break;
        }
    default:
        std::cerr << "No supported branch instruction for given Opcode" << std::endl;
        // NMS.setPC(CMS.getPC() + 1);
    }
}

ALU::ALU(bool reg, unsigned slot) : FunctionalUnit(reg, slot) { pc = 0; }

ALU::~ALU() {}

Register ALU::add(Register a, Register b) { return a + b; }

Register ALU::addi(Register a, Register imm) { return a + imm; }

Register ALU::sub(Register a, Register b) { return a - b; }

Register ALU::lui(Register imm) { return imm; }

Register ALU::slti(Register a, Register imm) { return a < imm ? 1 : 0; }

Register ALU::sltiu(Register a, Register imm)
{
    unsigned temp1 = (signed)a;
    unsigned temp2 = (signed)imm;
    return temp1 < temp2 ? 1 : 0;
    // scuffed logic but should work as intended
}

Register ALU::andi(Register a, Register imm) { return a & imm; }

Register ALU::ori(Register a, Register imm) { return a | imm; }

Register ALU::xori(Register a, Register imm) { return a ^ imm; }

Register ALU::snez(Register a) { return a != 0 ? 1 : 0; }

Register ALU::and_op(Register a, Register b)
{
    return a & b;
}

Register ALU::or_op(Register a, Register b) { return a | b; }

Register ALU::xor_op(Register a, Register b) { return a ^ b; }

Register ALU::not_op(Register a) { return ~a; }

Register ALU::slli(Register a, Register shift) { return a << (int)shift; }

Register ALU::srli(Register a, Register shift)
{
    unsigned temp3 = (signed)a;
    return temp3 >> (int)(shift);
}

Register ALU::srai(Register a, Register shift)
{
    // Register mask = 1 << 31; Not needed since auto sign extension; also, don't know size of ints
    Register shiftVal = shift & (0x1F);
    std::cout << "Shifting: " << a << "by " << (int)shiftVal << std::endl;
    return (a >> (int)shiftVal); // | (mask >> (int)shift);
}

Register ALU::sll(Register a, Register b) { return a << (int)b; }

Register ALU::srl(Register a, Register b) { return a >> (int)b; }

Register ALU::sra(Register a, Register b)
{
    return (a >> (int)b);
}

Register ALU::slt(Register a, Register b)
{
    return a < b ? 1 : 0;
}

Register ALU::sltu(Register a, Register b)
{
    unsigned temp1 = (signed)a;
    unsigned temp2 = (signed)b;
    return temp1 < temp2 ? 1 : 0;
}

void ALU::processRType(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    Register op1, op2;
    Register res;
    // if not connect to regfile we should
    // goto interconnect
    if (this->isConnectedToRegisterFile())
    {
        op1 = CMS.getRegister(I.get_rs1());
        op2 = CMS.getRegister(I.get_rs2());
    }
    else
    {
        op1 = CMS.getInterconnectValue(this->slotIdx - 2);
        op2 = CMS.getInterconnectValue(this->slotIdx - 1);
    }

    switch (I.get_funct3())
    {
    case 0x0: // ADD or SUB
        if (I.get_funct7() == 0x00)
        {
            res = add(op1, op2);
            // std::cout << "Adding: " << MS.getRegister(I.get_rs1()) << " + " << MS.getRegister(I.get_rs2()) << " to " << I.get_rd() << std::endl;
        }
        else if (I.get_funct7() == 0x20)
            res = sub(op1, op2);
        break;
    case 0x2: // SLT
        res = slt(op1, op2);
        break;
    case 0x3: // SLTU
        res = sltu(op1, op2);
        break;
    case 0x7: // AND
        res = and_op(op1, op2);
        break;
    case 0x6: // OR
        res = or_op(op1, op2);
        break;
    case 0x4: // XOR
        res = xor_op(op1, op2);
        break;
    case 0x1: // SLL (Shift Left Logical)
        res = slli(op1, op2);
        break;
    case 0x5: // SRL or SRA (Shift Right Logical or Arithmetic)
        if (I.get_funct7() == 0x00)
            res = srli(op1, op2 & 0x1F);
        else if (I.get_funct7() == 0x20)
            res = srai(op1, op2 & 0x1F);
        break;
    // Handle other R-type instructions...
    default:
        std::cerr << "Unknown funct3 for R-type: " << I.get_funct3()
                  << std::endl;
        break;
    }

    CMS.setInterconnectValue(slotIdx, res);
    if (this->isConnectedToRegisterFile())
    {
        NMS.setRegister(I.get_rd(), res);
    }
}

// Process I-type instructions
void ALU::processIType(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    Register op1;
    Register res;
    if (this->isConnectedToRegisterFile())
    {
        op1 = CMS.getRegister(I.get_rs1());
    }
    else
    {
        // std::cout << "Attempting to read: " << this->slotIdx - 2 << std::endl;
        op1 = CMS.getInterconnectValue(this->slotIdx - 2);
        // std::cout << "OP1 is read" << std::endl;
    }

    switch (I.get_funct3())
    {
    case 0x0: // ADDI
        res = addi(op1, I.get_immediate());
        break;
    case 0x2: // SLTI
        res = slti(op1, I.get_immediate());
        break;
    case 0x3: // SLTIU
        res = sltiu(op1, I.get_immediate());
        break;
    case 0x7: // ANDI
        res = andi(op1, I.get_immediate());
        break;
    case 0x6: // ORI
        res = ori(op1, I.get_immediate());
        break;
    case 0x4: // XORI
        res = xori(op1, I.get_immediate());
        break;
    case 0x1: // SLLI (Shift Left Logical Immediate)
        res = slli(op1, I.get_immediate());
        break;
    case 0x5: // SRLI or SRAI (Shift Right Logical Immediate or Arithmetic)
        if ((I.get_immediate() & 1 << 10) != 0)
        {
            res = srai(op1, I.get_immediate());

            // std::cout << "Executing srai" << std::endl;
        }
        else
        {
            res = srli(op1, I.get_immediate());
            // std::cout << "Executing srli" << std::endl;
        }
        break;
    // Handle other I-type instructions...
    default:
        std::cerr << "Unknown funct3 for I-type: " << I.get_funct3()
                  << std::endl;
        break;
    }

    CMS.setInterconnectValue(slotIdx, res);
    if (this->isConnectedToRegisterFile())
    {
        NMS.setRegister(I.get_rd(), res);
    }
}

// Process S-type (Store) instructions
void ALU::processSType(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    Register op1, op2;
    Register res;
    // if not connect to regfile we should
    // goto interconnect
    if (this->isConnectedToRegisterFile())
    {
        op1 = CMS.getRegister(I.get_rs1());
        op2 = CMS.getRegister(I.get_rs2());
    }
    else
    {
        op1 = CMS.getInterconnectValue(this->slotIdx - 2);
        op2 = CMS.getInterconnectValue(this->slotIdx - 1);
    }

    Register address = op1 + I.get_immediate();
    switch (I.get_funct3())
    {
    case 0x0: // SB (Store Byte) - Invalid Instruction
        // NMS.setMem((int)address, op2 & 0xFF, 1);
        break;
    case 0x1: // SH (Store Halfword) - Invalid Instruction
        // NMS.setMem((int)address, op2, 2);
        break;
    case 0x2: // SW (Store Word)
        std::cout << op2 << " " << I.get_immediate() << " " << address << std::endl;
        // NMS.setMem((int)address, op2, 4);
        break;
    // Handle other S-type instructions...
    default:
        std::cerr << "Unknown funct3 for S-type: " << I.get_funct3()
                  << std::endl;
        break;
    }
    CMS.setInterconnectValue(slotIdx, res);
    if (this->isConnectedToRegisterFile())
    {
        NMS.setRegister(I.get_rd(), res);
    }
}

// Process U-type (Upper Immediate) instructions
void ALU::processUType(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    Register res;
    switch (I.get_opcode())
    {
    case 0x37: // LUI (Load Upper Immediate)
        res = lui(I.get_immediate());
        break;
    case 0x17: // AUIPC (Add Upper Immediate to PC)
        res = CMS.getPC() + I.get_immediate();
        break;
    // Handle other U-type instructions...
    default:
        std::cerr << "Unknown opcode for U-type: " << I.get_opcode()
                  << std::endl;
        break;
    }
    CMS.setInterconnectValue(slotIdx, res);
    if (this->isConnectedToRegisterFile())
    {
        NMS.setRegister(I.get_rd(), res);
    }
}

// Process the instruction based on its opcode
void ALU::processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS)
{
    // std::cout << "ALU: " << std::hex << I.get_rawinstruction() << std::endl; // Used for debugging
    if (I.get_rawinstruction() == 19)
    {
        return;
    }
    switch (I.get_opcode())
    {
    case 0x33:                     // R-type instructions
        processRType(I, CMS, NMS); // reads from interconnect properly
        break;
    case 0x13:                     // I-type instructions
        processIType(I, CMS, NMS); // reads from interconnect properly
        break;
    case 0x03: // I-type load instructions (for load, use the same function but
               // add a switch case for OPCODE_LOAD)
        // Handle load instruction here
        break;
    case 0x23:                     // S-type instructions (store)
        processSType(I, CMS, NMS); // reads from interconnect properly
        break;
    case 0x63: // B-type instructions (branch)
        // processBType(I, MS); brokie
        break;
    case 0x37:                     // U-type instructions (LUI)
    case 0x17:                     // U-type instructions (AUIPC)
        processUType(I, CMS, NMS); // this doesn't read from interconnect, better have NOPS
        break;
    case 0x6F: // J-type instructions (JAL)
        // Add processing for JAL
        break;
    case 0x67: // I-type JALR
        // Add processing for JALR
        break;
    default:
        std::cerr << "Unknown opcode for GFU: " << I.get_opcode() << std::endl;
        break;
    }
}
