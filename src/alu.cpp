#include <stdint.h>
#include "alu.hpp"

BranchUnit::BranchUnit() { pc = 0; }

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

void BranchUnit::processInstruction(Instruction &I, MachineState &MS)
{
    switch (I.get_type())
    {
    case Instruction::type::J:
    {
        std::cout << "found a jump with target: " << std::hex << I.get_immediate() << "\n"
                  << std::dec;
        if (I.get_immediate() == -1)
        {
            MS.halt();
            break;
        }
        // FIXME: Might be wrong.
        MS.setPC(MS.getPC() + I.get_immediate());
        break;
    }
    case Instruction::type::B:
        switch (I.get_funct3())
        {
        case 0:
            (I.get_rs1() == I.get_rs2()) ? MS.setPC(MS.getPC() + I.get_immediate()) : MS.setPC(MS.getPC() + 4);
        case 1:
            (I.get_rs1() != I.get_rs2()) ? MS.setPC(MS.getPC() + I.get_immediate()) : MS.setPC(MS.getPC() + 4);
        case 4:
            (I.get_rs1() < I.get_rs2()) ? MS.setPC(MS.getPC() + I.get_immediate()) : MS.setPC(MS.getPC() + 4);
        case 6:
            (((unsigned)I.get_rs1()) < ((unsigned)I.get_rs2())) ? MS.setPC(MS.getPC() + I.get_immediate()) : MS.setPC(MS.getPC() + 4);
        case 5:
            (I.get_rs1() >= I.get_rs2()) ? MS.setPC(MS.getPC() + I.get_immediate()) : MS.setPC(MS.getPC() + 4);
        case 7:
            (((unsigned)I.get_rs1()) >= ((unsigned)I.get_rs2())) ? MS.setPC(MS.getPC() + I.get_immediate()) : MS.setPC(MS.getPC() + 4);
        }
    default:
        MS.setPC(MS.getPC() + 1);
    }
}

ALU::ALU() { pc = 0; }

ALU::~ALU() {}

Register ALU::add(Register a, Register b) { return a + b; }

Register ALU::addi(Register a, Register imm) { return a + imm; }

Register ALU::sub(Register a, Register b) { return a - b; }

Register ALU::lui(Register imm) { return imm << 12; }

Register ALU::slti(Register a, Register imm) { return a < imm ? 1 : 0; }

Register ALU::sltiu(Register a, Register imm) { return a < imm ? 1 : 0; }

Register ALU::andi(Register a, Register imm) { return a & imm; }

Register ALU::ori(Register a, Register imm) { return a | imm; }

Register ALU::xori(Register a, Register imm) { return a ^ imm; }

Register ALU::snez(Register a) { return a != 0 ? 1 : 0; }

Register ALU::and_op(Register a, Register b) { return a & b; }

Register ALU::or_op(Register a, Register b) { return a | b; }

Register ALU::xor_op(Register a, Register b) { return a ^ b; }

Register ALU::not_op(Register a) { return ~a; }

Register ALU::slli(Register a, Register shift) { return a << (int)shift; }

Register ALU::srli(Register a, Register shift) { return a >> (int)shift; }

Register ALU::srai(Register a, Register shift)
{
    Register mask = 1 << 31;
    return (a >> (int)shift) | (mask >> (int)shift);
}

Register ALU::sll(Register a, Register b) { return a << (int)b; }

Register ALU::srl(Register a, Register b) { return a >> (int)b; }

Register ALU::sra(Register a, Register b)
{
    Register mask = 1 << 31;
    return (a >> (int)b) | (mask >> (int)b);
}

void ALU::processRType(Instruction &I, MachineState &MS)
{
    switch (I.get_funct3())
    {
    case 0x0: // ADD or SUB
        if (I.get_funct7() == 0x00)
            MS.setRegister(I.get_rd(), add(MS.getRegister(I.get_rs1()),
                                           MS.getRegister(I.get_rs2())));
        else if (I.get_funct7() == 0x20)
            MS.setRegister(I.get_rd(), sub(MS.getRegister(I.get_rs1()),
                                           MS.getRegister(I.get_rs2())));
        break;
    case 0x7: // AND
        MS.setRegister(I.get_rd(), and_op(MS.getRegister(I.get_rs1()),
                                          MS.getRegister(I.get_rs2())));
        break;
    case 0x6: // OR
        MS.setRegister(I.get_rd(), or_op(MS.getRegister(I.get_rs1()),
                                         MS.getRegister(I.get_rs2())));
        break;
    case 0x4: // XOR
        MS.setRegister(I.get_rd(), xor_op(MS.getRegister(I.get_rs1()),
                                          MS.getRegister(I.get_rs2())));
        break;
    case 0x1: // SLL (Shift Left Logical)
        MS.setRegister(I.get_rd(), slli(MS.getRegister(I.get_rs1()),
                                        MS.getRegister(I.get_rs2()) & 0x1F));
        break;
    case 0x5: // SRL or SRA (Shift Right Logical or Arithmetic)
        if (I.get_funct7() == 0x00)
            MS.setRegister(I.get_rd(),
                           srli(MS.getRegister(I.get_rs1()),
                                MS.getRegister(I.get_rs2()) & 0x1F));
        else if (I.get_funct7() == 0x20)
            MS.setRegister(I.get_rd(),
                           srai(MS.getRegister(I.get_rs1()),
                                MS.getRegister(I.get_rs2()) & 0x1F));
        break;
    // Handle other R-type instructions...
    default:
        std::cerr << "Unknown funct3 for R-type: " << I.get_funct3()
                  << std::endl;
        break;
    }
}

// Process I-type instructions
void ALU::processIType(Instruction &I, MachineState &MS)
{
    switch (I.get_funct3())
    {
    case 0x0: // ADDI
        MS.setRegister(I.get_rd(),
                       addi(MS.getRegister(I.get_rs1()), I.get_immediate()));
        break;
    case 0x2: // SLTI
        MS.setRegister(I.get_rd(),
                       slti(MS.getRegister(I.get_rs1()), I.get_immediate()));
        break;
    case 0x3: // SLTIU
        MS.setRegister(
            I.get_rd(), sltiu(MS.getRegister(I.get_rs1()), I.get_immediate()));
        break;
    case 0x7: // ANDI
        MS.setRegister(I.get_rd(),
                       andi(MS.getRegister(I.get_rs1()), I.get_immediate()));
        break;
    case 0x6: // ORI
        MS.setRegister(I.get_rd(),
                       ori(MS.getRegister(I.get_rs1()), I.get_immediate()));
        break;
    case 0x4: // XORI
        MS.setRegister(I.get_rd(),
                       xori(MS.getRegister(I.get_rs1()), I.get_immediate()));
        break;
    case 0x1: // SLLI (Shift Left Logical Immediate)
        MS.setRegister(I.get_rd(),
                       slli(MS.getRegister(I.get_rs1()), I.get_immediate()));
        break;
    case 0x5: // SRLI or SRAI (Shift Right Logical Immediate or Arithmetic)
        if (I.get_funct7() == 0x00)
            MS.setRegister(I.get_rd(),
                           srli(MS.getRegister(I.get_rs1()), I.get_immediate()));
        else if (I.get_funct7() == 0x20)
            MS.setRegister(I.get_rd(),
                           srai(MS.getRegister(I.get_rs1()), I.get_immediate()));
        break;
    // Handle other I-type instructions...
    default:
        std::cerr << "Unknown funct3 for I-type: " << I.get_funct3()
                  << std::endl;
        break;
    }
}

// Process S-type (Store) instructions
void ALU::processSType(Instruction &I, MachineState &MS)
{
    Register address = MS.getRegister(I.get_rs1()) + I.get_immediate();
    switch (I.get_funct3())
    {
    case 0x0: // SB (Store Byte) - Invalid Instruction
        MS.setMem((int)address, MS.getRegister(I.get_rs2()) & 0xFF, 1);
        break;
    case 0x1: // SH (Store Halfword) - Invalid Instruction
        MS.setMem((int)address, MS.getRegister(I.get_rs2()) & 0xFFFF, 2);
        break;
    case 0x2: // SW (Store Word)
        std::cout << MS.getRegister(I.get_rs2()) << " " << I.get_immediate() << " " << address << std::endl;
        MS.setMem((int)address, MS.getRegister(I.get_rs2()), 4);
        break;
    // Handle other S-type instructions...
    default:
        std::cerr << "Unknown funct3 for S-type: " << I.get_funct3()
                  << std::endl;
        break;
    }
}

// Process U-type (Upper Immediate) instructions
void ALU::processUType(Instruction &I, MachineState &MS)
{
    switch (I.get_opcode())
    {
    case 0x37: // LUI (Load Upper Immediate)
        MS.setRegister(I.get_rd(), lui(I.get_immediate()));
        break;
    case 0x17: // AUIPC (Add Upper Immediate to PC)
        MS.setRegister(I.get_rd(), MS.getPC() + I.get_immediate());
        break;
    // Handle other U-type instructions...
    default:
        std::cerr << "Unknown opcode for U-type: " << I.get_opcode()
                  << std::endl;
        break;
    }
}

// Process the instruction based on its opcode
void ALU::processInstruction(Instruction &I, MachineState &MS)
{
    switch (I.get_opcode())
    {
    case 0x33: // R-type instructions
        processRType(I, MS);
        break;
    case 0x13: // I-type instructions
        processIType(I, MS);
        break;
    case 0x03: // I-type load instructions (for load, use the same function but
               // add a switch case for OPCODE_LOAD)
        // Handle load instruction here
        break;
    case 0x23: // S-type instructions (store)
        processSType(I, MS);
        break;
    case 0x63: // B-type instructions (branch)
        // processBType(I, MS); brokie
        break;
    case 0x37: // U-type instructions (LUI)
    case 0x17: // U-type instructions (AUIPC)
        processUType(I, MS);
        break;
    case 0x6F: // J-type instructions (JAL)
        // Add processing for JAL
        break;
    case 0x67: // I-type JALR
        // Add processing for JALR
        break;
    default:
        std::cerr << "Unknown opcode: " << I.get_opcode() << std::endl;
        break;
    }
}
