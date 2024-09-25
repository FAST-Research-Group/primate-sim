#include "FunctionalUnit.hpp"

class BranchUnit : public FunctionalUnit
{
private:
  int32_t pc;

public:
  BranchUnit();
  ~BranchUnit();
  int32_t j(int32_t imm);
  int32_t jr(int32_t imm);
  int32_t beq(int32_t a, int32_t b);
  int32_t bne(int32_t a, int32_t b);
  int32_t blt(int32_t a, int32_t b);
  int32_t bge(int32_t a, int32_t b);
  int32_t bltu(int32_t a, uint32_t b);
  int32_t bgeu(int32_t a, uint32_t b);
  int32_t end();
  void processInstruction(Instruction &I, MachineState &MS) override;
};

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
    MS.setPC(MS.getPC() + I.get_immediate());
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

class ALU : public FunctionalUnit
{
private:
  int32_t sign_extend(int32_t imm, int32_t size)
  {
    int32_t mask = 1 << (size - 1);
    return (imm ^ mask) - mask;
  }
  int32_t pc;

public:
  ALU();
  ~ALU();
  // Arithmetic operations
  int32_t add(int32_t a, int32_t b);
  int32_t addi(int32_t a, int32_t imm);
  int32_t sub(int32_t a, int32_t b);
  int32_t lui(int32_t imm);

  // Logical operations
  int32_t slti(int32_t a, int32_t imm);
  int32_t sltiu(int32_t a, uint32_t imm);
  int32_t andi(int32_t a, int32_t imm);
  int32_t ori(int32_t a, int32_t imm);
  int32_t xori(int32_t a, int32_t imm);
  int32_t snez(int32_t a); // Set if not zero
  int32_t and_op(int32_t a, int32_t b);
  int32_t or_op(int32_t a, int32_t b);
  int32_t xor_op(int32_t a, int32_t b);
  int32_t not_op(int32_t a);

  // Shift operations
  int32_t slli(int32_t a, int32_t shift);
  int32_t srli(int32_t a, int32_t shift);
  int32_t srai(int32_t a, int32_t shift);
  int32_t sll(int32_t a, int32_t b);
  int32_t srl(int32_t a, int32_t b);
  int32_t sra(int32_t a, int32_t b);
};

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
