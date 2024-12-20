//
// Created by 18323 on 4/10/2024.
//

#ifndef PROPERVLIWPARSING_INSTRUCTION_H
#define PROPERVLIWPARSING_INSTRUCTION_H

#include <iostream>
#include <string>
#include <unordered_set>

#include "InstrInfo.hpp"

// 20, 12, 31, 11 masks
class Instruction
{
public:
    enum type
    {
        I,
        U,
        R,
        S,
        B,
        J,
        C
    }; // C for custom instructions
private:
    int raw_instruction;
    int rd;
    int rs1;
    int rs2;
    int opcode;
    int funct3;
    int funct7;
    int immediate;
    std::string assembly;
    type inst;

public:
    Instruction() = default;
    Instruction(const Instruction &) = default;
    Instruction &operator=(const Instruction &other) = default;

    Instruction(int raw_instruction)
    {
        this->raw_instruction = raw_instruction;
        opcode = raw_instruction & ((1 << 7) - 1);
        // give each instruction a type in order to extract the data more easily
        switch (opcode)
        {
        case 19:
        case 3:
            this->inst = I;
            break;
        case 55:
            this->inst = U;
            break;
        case 51:
            this->inst = R;
            break;
        case 111:
            this->inst = J;
            break;
        case 99:
            this->inst = B;
            break;
        case 35:
            this->inst = S;
            break;
        default:
            this->inst = C;
            break;
        }

        // Extract the data based on the type of instruction
        // Do the assembly string build here using more switches in the switch
        // for opcode.
        switch (inst)
        {

        case I: // the instruction is tested and it works. The immediate is not
                // sign extended
            this->funct3 = (raw_instruction >> 12) & 7;
            this->rd = (raw_instruction >> 7) & 0x1F;
            this->rs1 = (raw_instruction >> 15) & 0x1F;
            this->immediate = ((raw_instruction >> 20) & 0x0FFF);
            if ((this->immediate & (1 << 11)) == (1 << 11))
            {
                this->immediate |= 0xFFFFF000;
            }
            break;
        case U:
            this->rd = (raw_instruction >> 7) & ((1 << 5) - 1);
            this->immediate = raw_instruction & (~0xFFF);
            break;
        case R:
            this->rd = (raw_instruction >> 7) & ((1 << 5) - 1);
            this->funct3 = (raw_instruction >> 12) & ((1 << 3) - 1);
            this->rs1 = (raw_instruction >> 15) & ((1 << 5) - 1);
            this->rs2 = (raw_instruction >> 20) & ((1 << 5) - 1);
            this->funct7 = (raw_instruction >> 25) & ((1 << 7) - 1);
            break;
        case S:
            this->immediate = ((raw_instruction >> 7) & ((1 << 5) - 1)) +
                              (((raw_instruction >> 25) & ((1 << 7) - 1)) << 5);
            this->funct3 = (raw_instruction >> 12) & ((1 << 3) - 1);
            this->rs1 = (raw_instruction >> 15) & ((1 << 5) - 1);
            this->rs2 = (raw_instruction >> 20) & ((1 << 5) - 1);
            if ((this->immediate & (1 << 11)) == (1 << 11))
            {
                this->immediate |= 0xFFFFF000;
            }
            break;
        case B:
            this->immediate = 0;
            this->immediate += (((raw_instruction >> 31) & 1) << 12);
            this->immediate +=
                (((raw_instruction >> 25) & ((1 << 7) - 1)) << 5);
            this->immediate += ((raw_instruction & (1 << 7)) << 4);
            this->immediate += (((raw_instruction >> 8) & ((1 << 4) - 1)) << 1);
            if ((this->immediate & (1 << 12)) == (1 << 12))
            {
                this->immediate |= 0xFFFFE000;
            }

            this->funct3 = (raw_instruction >> 12) & ((1 << 3) - 1);
            this->rs1 = (raw_instruction >> 15) & ((1 << 5) - 1);
            this->rs2 = (raw_instruction >> 20) & ((1 << 5) - 1);
            break;
        case J:
        { // what is going on here?
            int imm_19 = (((raw_instruction >> 31) & 1) << 19);
            int imm_18_11 = ((((raw_instruction >> 12) & (1 << 8) - 1)) << 11);
            int imm_9_0 = ((((raw_instruction >> 21) & ((1 << 10) - 1))) << 0);
            int imm_10 = (((raw_instruction >> 20) & 1) << 10);
            // std::cout << std::hex << imm_19 << " " << imm_18_11 << " " << imm_10 << " " << imm_9_0 << std::dec << "\n";
            this->immediate = imm_19 + imm_18_11 + imm_10 + imm_9_0;
            if ((this->immediate & (1 << 19)) == (1 << 19))
            {
                this->immediate |= 0xFFF00000;
            }
            this->rd = ((raw_instruction >> 7) & ((1 << 5) - 1));
            break;
        }
        case C:
            this->funct3 = (raw_instruction >> 12) & 7;
            this->rd = (raw_instruction >> 7) & 0x1F;
            this->rs1 = (raw_instruction >> 15) & 0x1F;
            this->immediate = ((raw_instruction >> 20) & 0x0FFF);
            if ((this->immediate & (1 << 11)) == (1 << 11))
            {
                this->immediate |= 0xFFFFF000;
            }
            break;
            break;
        }
    }
    int get_immediate() { return this->immediate; }
    int get_rd() { return this->rd; }
    int get_rs1() { return this->rs1; }
    int get_rs2() { return this->rs2; }
    int get_opcode() { return this->opcode; }
    int get_funct3() { return this->funct3; }
    int get_funct7() { return this->funct7; }
    type get_type() { return this->inst; }
    void to_string() { std::cout << this->assembly << std::endl; }
    bool isGFUInst()
    {
        return RISCV_INFO::riscv_opcodes.find(this->opcode) != RISCV_INFO::riscv_opcodes.end();
    }
    int get_rawinstruction() { return this->raw_instruction; }

public:
    bool operator==(const Instruction &other) const
    {
        return this == &other;
    }
};

#endif // PROPERVLIWPARSING_INSTRUCTION_H
