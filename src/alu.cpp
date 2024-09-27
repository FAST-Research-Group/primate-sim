#include <stdint.h>
#include "alu.hpp"

BranchUnit::BranchUnit() { pc = 0; }

BranchUnit::~BranchUnit() {}

int32_t BranchUnit::j(int32_t imm) { return pc + imm; }

int32_t BranchUnit::jr(int32_t imm) { return imm; }

int32_t BranchUnit::beq(int32_t a, int32_t b) { return a == b ? 1 : 0; }

int32_t BranchUnit::bne(int32_t a, int32_t b) { return a != b ? 1 : 0; }

int32_t BranchUnit::blt(int32_t a, int32_t b) { return a < b ? 1 : 0; }

int32_t BranchUnit::bge(int32_t a, int32_t b) { return a >= b ? 1 : 0; }

int32_t BranchUnit::bltu(int32_t a, uint32_t b) { return a < b ? 1 : 0; }

int32_t BranchUnit::bgeu(int32_t a, uint32_t b) { return a >= b ? 1 : 0; }

int32_t BranchUnit::end() { return 0; }

void BranchUnit::processInstruction(Instruction &I, MachineState &MS)
{
  switch (I.get_type())
  {
  case 'J':
    MS.setPC(MS.getPC() + I.get_immediate()); // not sure how to interpret spreadsheet for this instruction. Debugging might be needed here
    break;
  case 'B':
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
    MS.setPC(MS.getPC() + 4);
  }
}

void ALU::processInstruction(Instruction &I, MachineState &MS) {

}

ALU::ALU() { pc = 0; }

ALU::~ALU() {}

int32_t ALU::add(int32_t a, int32_t b) { return a + b; }

int32_t ALU::addi(int32_t a, int32_t imm) { return a + imm; }

int32_t ALU::sub(int32_t a, int32_t b) { return a - b; }

int32_t ALU::lui(int32_t imm) { return imm << 12; }

int32_t ALU::slti(int32_t a, int32_t imm) { return a < imm ? 1 : 0; }

int32_t ALU::sltiu(int32_t a, uint32_t imm) { return a < imm ? 1 : 0; }

int32_t ALU::andi(int32_t a, int32_t imm) { return a & imm; }

int32_t ALU::ori(int32_t a, int32_t imm) { return a | imm; }

int32_t ALU::xori(int32_t a, int32_t imm) { return a ^ imm; }

int32_t ALU::snez(int32_t a) { return a != 0 ? 1 : 0; }

int32_t ALU::and_op(int32_t a, int32_t b) { return a & b; }

int32_t ALU::or_op(int32_t a, int32_t b) { return a | b; }

int32_t ALU::xor_op(int32_t a, int32_t b) { return a ^ b; }

int32_t ALU::not_op(int32_t a) { return ~a; }

int32_t ALU::slli(int32_t a, int32_t shift) { return a << shift; }

int32_t ALU::srli(int32_t a, int32_t shift) { return a >> shift; }

int32_t ALU::srai(int32_t a, int32_t shift)
{
  int32_t mask = 1 << 31;
  return (a >> shift) | (mask >> shift);
}

int32_t ALU::sll(int32_t a, int32_t b) { return a << b; }

int32_t ALU::srl(int32_t a, int32_t b) { return a >> b; }

int32_t ALU::sra(int32_t a, int32_t b)
{
  int32_t mask = 1 << 31;
  return (a >> b) | (mask >> b);
}

void ALU::processRType(Instruction &I, MachineState &MS) {
    switch(I.get_funct3()) {
    case 0x0: // ADD or SUB
        if(I.get_funct7() == 0x00)
            MS.writeRegister(I.get_rd(), add(MS.readRegister(I.get_rs1()),
                                             MS.readRegister(I.get_rs2())));
        else if(I.get_funct7() == 0x20)
            MS.writeRegister(I.get_rd(), sub(MS.readRegister(I.get_rs1()),
                                             MS.readRegister(I.get_rs2())));
        break;
    case 0x7: // AND
        MS.writeRegister(I.get_rd(), and_op(MS.readRegister(I.get_rs1()),
                                            MS.readRegister(I.get_rs2())));
        break;
    case 0x6: // OR
        MS.writeRegister(I.get_rd(), or_op(MS.readRegister(I.get_rs1()),
                                           MS.readRegister(I.get_rs2())));
        break;
    case 0x4: // XOR
        MS.writeRegister(I.get_rd(), xor_op(MS.readRegister(I.get_rs1()),
                                            MS.readRegister(I.get_rs2())));
        break;
    case 0x1: // SLL (Shift Left Logical)
        MS.writeRegister(I.get_rd(), slli(MS.readRegister(I.get_rs1()),
                                          MS.readRegister(I.get_rs2()) & 0x1F));
        break;
    case 0x5: // SRL or SRA (Shift Right Logical or Arithmetic)
        if(I.get_funct7() == 0x00)
            MS.writeRegister(I.get_rd(),
                             srli(MS.readRegister(I.get_rs1()),
                                  MS.readRegister(I.get_rs2()) & 0x1F));
        else if(I.get_funct7() == 0x20)
            MS.writeRegister(I.get_rd(),
                             srai(MS.readRegister(I.get_rs1()),
                                  MS.readRegister(I.get_rs2()) & 0x1F));
        break;
    // Handle other R-type instructions...
    default:
        std::cerr << "Unknown funct3 for R-type: " << I.get_funct3()
                  << std::endl;
        break;
    }
}

// Process I-type instructions
void ALU::processIType(Instruction &I, MachineState &MS) {
    switch(I.get_funct3()) {
    case 0x0: // ADDI
        MS.writeRegister(I.get_rd(),
                         addi(MS.readRegister(I.get_rs1()), I.get_immediate()));
        break;
    case 0x2: // SLTI
        MS.writeRegister(I.get_rd(),
                         slti(MS.readRegister(I.get_rs1()), I.get_immediate()));
        break;
    case 0x3: // SLTIU
        MS.writeRegister(
            I.get_rd(), sltiu(MS.readRegister(I.get_rs1()), I.get_immediate()));
        break;
    case 0x7: // ANDI
        MS.writeRegister(I.get_rd(),
                         andi(MS.readRegister(I.get_rs1()), I.get_immediate()));
        break;
    case 0x6: // ORI
        MS.writeRegister(I.get_rd(),
                         ori(MS.readRegister(I.get_rs1()), I.get_immediate()));
        break;
    case 0x4: // XORI
        MS.writeRegister(I.get_rd(),
                         xori(MS.readRegister(I.get_rs1()), I.get_immediate()));
        break;
    case 0x1: // SLLI (Shift Left Logical Immediate)
        MS.writeRegister(I.get_rd(),
                         slli(MS.readRegister(I.get_rs1()), I.getShamt()));
        break;
    case 0x5: // SRLI or SRAI (Shift Right Logical Immediate or Arithmetic)
        if(I.get_funct7() == 0x00)
            MS.writeRegister(I.get_rd(),
                             srli(MS.readRegister(I.get_rs1()), I.getShamt()));
        else if(I.get_funct7() == 0x20)
            MS.writeRegister(I.get_rd(),
                             srai(MS.readRegister(I.get_rs1()), I.getShamt()));
        break;
    // Handle other I-type instructions...
    default:
        std::cerr << "Unknown funct3 for I-type: " << I.get_funct3()
                  << std::endl;
        break;
    }
}

// Process S-type (Store) instructions
void ALU::processSType(Instruction &I, MachineState &MS) {
    int32_t address = MS.readRegister(I.get_rs1()) + I.get_immediate();
    switch(I.get_funct3()) {
    case 0x0: // SB (Store Byte)
        MS.writeMemory8(address, MS.readRegister(I.get_rs2()) & 0xFF);
        break;
    case 0x1: // SH (Store Halfword)
        MS.writeMemory16(address, MS.readRegister(I.get_rs2()) & 0xFFFF);
        break;
    case 0x2: // SW (Store Word)
        MS.writeMemory32(address, MS.readRegister(I.get_rs2()));
        break;
    // Handle other S-type instructions...
    default:
        std::cerr << "Unknown funct3 for S-type: " << I.get_funct3()
                  << std::endl;
        break;
    }
}


// Process U-type (Upper Immediate) instructions
void ALU::processUType(Instruction &I, MachineState &MS) {
    switch(I.get_opcode()) {
    case 0x37: // LUI (Load Upper Immediate)
        MS.writeRegister(I.get_rd(), lui(I.get_immediate()));
        break;
    case 0x17: // AUIPC (Add Upper Immediate to PC)
        MS.writeRegister(I.get_rd(), MS.readPC() + I.get_immediate());
        break;
    // Handle other U-type instructions...
    default:
        std::cerr << "Unknown opcode for U-type: " << I.get_opcode()
                  << std::endl;
        break;
    }
}

// Process the instruction based on its opcode
void ALU::processInstruction(Instruction &I, MachineState &MS) {
    switch(I.get_opcode()) {
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
        processBType(I, MS);
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