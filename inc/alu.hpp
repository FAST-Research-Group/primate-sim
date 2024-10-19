#pragma once

#include "FunctionalUnit.hpp"

class BranchUnit : public FunctionalUnit
{
private:
  uint64_t pc;
public:
  BranchUnit(bool, unsigned);
  virtual ~BranchUnit();
  uint64_t j(uint64_t imm);
  uint64_t jr(uint64_t imm);
  uint64_t beq(uint64_t a, uint64_t b);
  uint64_t bne(uint64_t a, uint64_t b);
  uint64_t blt(uint64_t a, uint64_t b);
  uint64_t bge(uint64_t a, uint64_t b);
  uint64_t bltu(uint64_t a, uint64_t b);
  uint64_t bgeu(uint64_t a, uint64_t b);
  uint64_t end();
  void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS) override;
};

class ALU : public FunctionalUnit
{
private:
  uint64_t pc;
  uint64_t sign_extend(uint64_t imm, uint64_t size)
  {
    uint64_t mask = 1 << (size - 1);
    return (imm ^ mask) - mask;
  }

public:
  ALU(bool, unsigned);
  virtual ~ALU();

  // Arithmetic operations
  Register add(Register a, Register b);
  Register addi(Register a, Register imm);
  Register sub(Register a, Register b);
  Register lui(Register imm);

  // Logical operations
  Register slti(Register a, Register imm);
  Register sltiu(Register a, Register imm);
  Register slt(Register a, Register b);
  Register sltu(Register a, Register b);
  Register andi(Register a, Register imm);
  Register ori(Register a, Register imm);
  Register xori(Register a, Register imm);
  Register snez(Register a); // Set if not zero
  Register and_op(Register a, Register b);
  Register or_op(Register a, Register b);
  Register xor_op(Register a, Register b);
  Register not_op(Register a);

  // Shift operations
  Register slli(Register a, Register shift);
  Register srli(Register a, Register shift);
  Register srai(Register a, Register shift);
  Register sll(Register a, Register b);
  Register srl(Register a, Register b);
  Register sra(Register a, Register b);

  void processRType(Instruction &I, MachineState &CMS, MachineState &NMS);
  void processIType(Instruction &I, MachineState &CMS, MachineState &NMS);
  void processSType(Instruction &I, MachineState &CMS, MachineState &NMS);
  void processBType(Instruction &I, MachineState &CMS, MachineState &NMS);
  void processUType(Instruction &I, MachineState &CMS, MachineState &NMS);
  virtual void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS) override;
};
