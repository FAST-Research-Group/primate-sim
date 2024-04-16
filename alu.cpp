#include <stdint.h>

class branch {
 private:
  int32_t pc;

 public:
  branch();
  ~branch();
  int32_t j(int32_t imm);
  int32_t jr(int32_t imm);
  int32_t beq(int32_t a, int32_t b);
  int32_t bne(int32_t a, int32_t b);
  int32_t blt(int32_t a, int32_t b);
  int32_t bge(int32_t a, int32_t b);
  int32_t bltu(int32_t a, uint32_t b);
  int32_t bgeu(int32_t a, uint32_t b);
  int32_t end();
};

branch::branch() { pc = 0; }

branch::~branch() {}

int32_t branch::j(int32_t imm) { return pc + imm; }

int32_t branch::jr(int32_t imm) { return imm; }

int32_t branch::beq(int32_t a, int32_t b) { return a == b ? 1 : 0; }

int32_t branch::bne(int32_t a, int32_t b) { return a != b ? 1 : 0; }

int32_t branch::blt(int32_t a, int32_t b) { return a < b ? 1 : 0; }

int32_t branch::bge(int32_t a, int32_t b) { return a >= b ? 1 : 0; }

int32_t branch::bltu(int32_t a, uint32_t b) { return a < b ? 1 : 0; }

int32_t branch::bgeu(int32_t a, uint32_t b) { return a >= b ? 1 : 0; }

int32_t branch::end() { return 0; }

class alu {
 private:
  int32_t sign_extend(int32_t imm, int32_t size) {
    int32_t mask = 1 << (size - 1);
    return (imm ^ mask) - mask;
  }
  int32_t pc;

 public:
  alu();
  ~alu();
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
  int32_t snez(int32_t a);  // Set if not zero
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

alu::alu() { pc = 0; }

alu::~alu() {}

int32_t alu::add(int32_t a, int32_t b) { return a + b; }

int32_t alu::addi(int32_t a, int32_t imm) { return a + imm; }

int32_t alu::sub(int32_t a, int32_t b) { return a - b; }

int32_t alu::lui(int32_t imm) { return imm << 12; }

int32_t alu::slti(int32_t a, int32_t imm) { return a < imm ? 1 : 0; }

int32_t alu::sltiu(int32_t a, uint32_t imm) { return a < imm ? 1 : 0; }

int32_t alu::andi(int32_t a, int32_t imm) { return a & imm; }

int32_t alu::ori(int32_t a, int32_t imm) { return a | imm; }

int32_t alu::xori(int32_t a, int32_t imm) { return a ^ imm; }

int32_t alu::snez(int32_t a) { return a != 0 ? 1 : 0; }

int32_t alu::and_op(int32_t a, int32_t b) { return a & b; }

int32_t alu::or_op(int32_t a, int32_t b) { return a | b; }

int32_t alu::xor_op(int32_t a, int32_t b) { return a ^ b; }

int32_t alu::not_op(int32_t a) { return ~a; }

int32_t alu::slli(int32_t a, int32_t shift) { return a << shift; }

int32_t alu::srli(int32_t a, int32_t shift) { return a >> shift; }

int32_t alu::srai(int32_t a, int32_t shift) {
  int32_t mask = 1 << 31;
  return (a >> shift) | (mask >> shift);
}

int32_t alu::sll(int32_t a, int32_t b) { return a << b; }

int32_t alu::srl(int32_t a, int32_t b) { return a >> b; }

int32_t alu::sra(int32_t a, int32_t b) {
  int32_t mask = 1 << 31;
  return (a >> b) | (mask >> b);
}

int main() {
  alu alu;
  int32_t a = 10;
  int32_t b = 20;
  int32_t imm = 5;
  int32_t shift = 2;
  int32_t result;

  result = alu.add(a, b);
  result = alu.addi(a, imm);
  result = alu.sub(a, b);
  result = alu.lui(imm);
  result = alu.slti(a, imm);
  result = alu.sltiu(a, imm);
  result = alu.andi(a, imm);
  result = alu.ori(a, imm);
  result = alu.xori(a, imm);
  result = alu.snez(a);
  result = alu.and_op(a, b);
  result = alu.or_op(a, b);
  result = alu.xor_op(a, b);
  result = alu.not_op(a);
  result = alu.slli(a, shift);
  result = alu.srli(a, shift);
  result = alu.srai(a, shift);
  result = alu.sll(a, b);
  result = alu.srl(a, b);
  result = alu.sra(a, b);

  return 0;
}