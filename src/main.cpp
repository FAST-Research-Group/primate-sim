#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdint>
#include "PrimateConfig.hpp"
#include "MachineState.hpp"
#include "alu.hpp"
#include "Instruction.hpp"
#include "FunctionalUnit.hpp"
using namespace std;

// stringToBinary function is used since issues with stoi staying in 32 bits
int stringToBinary(string test)
{
    int result = 0;
    for (int i = 0; i < 8; i++)
    {
        result = result << 4;
        switch (test[i])
        {

        case '0':
            result += 0b0000;
            break;
        case '1':
            result += 0b0001;
            break;
        case '2':
            result += 0b0010;
            break;
        case '3':
            result += 0b0011;
            break;
        case '4':
            result += 0b0100;
            break;
        case '5':
            result += 0b0101;
            break;
        case '6':
            result += 0b0110;
            break;
        case '7':
            result += 0b0111;
            break;
        case '8':
            result += 0b1000;
            break;
        case '9':
            result += 0b1001;
            break;
        case 'a':
        case 'A':
            result += 0b1010;
            break;
        case 'b':
        case 'B':
            result += 0b1011;
            break;
        case 'c':
        case 'C':
            result += 0b1100;
            break;
        case 'd':
        case 'D':
            result += 0b1101;
            break;
        case 'e':
        case 'E':
            result += 0b1110;
            break;
        default:
            result += 0b1111;
            break;
        }
    }
    return result;
}

vector<vector<Instruction>> read(const string &filename, PrimateConfig primateCfg)
{
    vector<vector<Instruction>> VLIW;
    vector<Instruction> CurPacket;

    ifstream inputFile(filename);
    string line;

    if (!inputFile.is_open())
    {
        cerr << "Error opening file" << endl;
        return VLIW;
    }

    int line_no = 0;
    while (getline(inputFile, line))
    {
        line_no++;
        if (line.size() == 0) {
          std::cout << "warning: empty line in program LINE: " << line_no;
          continue;
        }
          
        if (CurPacket.size() >= primateCfg.instruction_width)
        {
            VLIW.push_back(CurPacket);
            CurPacket.clear();
        }

	int i = 0;
        while (i < line.size())
        {
            string word = line.substr(i, i+8);
            int num = stringToBinary(word);
            Instruction CurInstruct(num);
            CurPacket.push_back(CurInstruct);
	    i += 8;
        }
    }

    if (!CurPacket.empty())
    {
        // Only push CurPacket if it contains exactly instruction_width instructions
        if (CurPacket.size() == primateCfg.instruction_width)
        {
            VLIW.push_back(CurPacket);
        }
        else
        {
          cout << "Warning: Incomplete packet: Expected size: " << primateCfg.instruction_width << " Actual: " << CurPacket.size() << endl;
        }
    }

    std::cout << "Found " << VLIW.size() << " Instructions\n";

    inputFile.close();
    return VLIW;
}

void get_data(Instruction dat)
{
    cout << "Immediate: " << dat.get_immediate() << endl;
    cout << "Destination Register: " << dat.get_rd() << endl;
    cout << "RS1: " << dat.get_rs1() << endl;
    cout << "RS2: " << dat.get_rs2() << endl;
    cout << "Opcode: " << dat.get_opcode() << endl;
    cout << "Funct3: " << dat.get_funct3() << endl;
    cout << "Funct7: " << dat.get_funct7() << endl;
    cout << "Type: " << dat.get_type() << endl;
}

int main(int argc, char *argv[])
{
  // error if wrong amount of arguments arguments
  if (argc != 3) {
    cerr << "Usage: " << argv[0] << " <path to program.bin> <path to primate.cfg>" << endl;
    return 1;
  }

  string filePath_instruction = argv[1];
  string filePath_config = argv[2];
  PrimateConfig primateCfg(filePath_config);

  std::cout << "done with confgi reading\n";

    // reading an parsing done here (bug will appear since "consts.hpp" has different dimensions for VLIW than my hardcoded main);
    vector<vector<Instruction>> instructions = read(filePath_instruction, primateCfg);

    vector<FunctionalUnit> allUnits; // IDK why this works, but stack overflow says it does

    for (int i = 0; i < primateCfg.num_branch; i++) {
        allUnits.push_back(BranchUnit());
    }
    
    for (int i = 0; i < primateCfg.num_merged; i++) {
      // This needs to change in order to accomodate between green and blue functional unit once wrapper class is written
      allUnits.push_back(ALU());
    }

    MachineState CurrentState(0), NextState(0); // initial machine state (!!!!!!!!!!!! This will be a bug that needs to be changed)

    // Initialize Functional Units over here

    // Throw warnings if same destination
    // If statement necessary in order to make sure that I don't get an out of range exception from the nested for loopss
    if (instructions.size() >= 2)
    {
        for (int i = 0; i < instructions.size(); i++)
        {
            for (int j = 0; i < instructions.at(i).size() - 1; j++)
            {
                for (int k = j + 1; k < instructions.at(i).size(); k++)
                    if (instructions.at(i).at(j).get_rd() == instructions.at(i).at(k).get_rd())
                    {
                        cout << j << " and " << k << " have the same destination register in Instruction " << i << endl;
                    }
            }
        }
    }

    while (CurrentState.running)
    {
        for (int i = 0; i < allUnits.size(); i++)
        {
            Instruction &temp_instr = instructions.at(CurrentState.getPC()).at(i);
            allUnits.at(i).processInstruction(temp_instr, NextState);
        }
        CurrentState = NextState;
        // processInstruction(instructions, Machine0);
    }

    cout << "Hello, World!" << endl;
    return 0;
}
