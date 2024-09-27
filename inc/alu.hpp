#pragma once
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

  virtual void processInstruction(Instruction &I, MachineState &MS) override;
};

