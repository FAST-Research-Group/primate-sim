//
// Created by 18323 on 4/10/2024.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include "Instruction.h"
using namespace std;

class Instruction {
private:
    enum type{I,U,R,S,B,J,C}; //C for custom instructions
    int raw_instruction;
    int rd;
    int rs1;
    int rs2;
    int opcode;
    int funct3;
    int funct7;
    int index;
    int imm11_0; //This immediate stuff is a mess. Needs to get reduced to no more than two immediates
    int imm31_12;
    int imm11_5; 
    int imm4_0;
    string assembly;
    type inst;

public:
    Instruction()=default;

    

    Instruction(int raw_instruction){
        this->raw_instruction = raw_instruction;
        opcode = raw_instruction & ((1<<8)-1);
        //give each instruction a type in order to extract the data more easily
        switch(opcode){
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
        // Do the assembly string build here using more switches in the switch for opcode.
        switch(inst){

            case I:
                this->funct3 = (raw_instruction>>12)&((1<<3)-1);
                this->rd = (raw_instruction>>7)&((1<<5)-1);
                this->rs1 = (raw_instruction>>15)&((1<<5)-1);
                this->imm11_0 = (raw_instruction>>20)&((1<<12)-1);
                break;
            case U:
                this->rd = (raw_instruction>>7)&((1<<5)-1);
                this->imm31_12 = (raw_instruction>>12)&((1<<20)-1);
                break;
            case R:
                this->rd = (raw_instruction>>7) & ((1<<5)-1);
                this->funct3 = (raw_instruction>>12) & ((1<<3)-1);
                this->rs1 = (raw_instruction>>15)&((1<<5)-1);
                this->rs2 = (raw_instruction>>20) & ((1<<5)-1);
                this->funct7 = (raw_instruction>>25) & ((1<<7)-1);
                break;
            case S:
                this->imm4_0 = (raw_instruction>>7) & ((1<<5)-1);
                this->funct3 = (raw_instruction>>12) & ((1<<3)-1);
                this->rs1 = (raw_instruction>>15) & ((1<<5)-1);
                this->rs2 = (raw_instruction>>20) & ((1<<5)-1);
                this->imm11_5 = (raw_instruction>>25) & ((1<<7)-1);
                break;
            case B: //Complete Later, the immediates are a headache currently
                this->imm11_0 +=((raw_instruction>>7) & (1<<7))<<10;
                this->funct3 = (raw_instruction>>12) & ((1<<3)-1);
                this->rs1 = (raw_instruction>>15) & ((1<<5)-1);
                this->rs2 = (raw_instruction>>20) & ((1<<5)-1);
                break;
            case J:

                break;
            case C:
                break;
        }
    }
    void to_string(){
        cout<<this->assembly<<endl;
    }

};

