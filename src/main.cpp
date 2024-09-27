#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdint>
#include "MachineState.hpp"
#include "alu.hpp"
#include "Instruction.hpp"
#include "FunctionalUnit.hpp"
using namespace std;

int instruction_width = 0;
int num_BFU = 0;
int num_merged = 0;
int num_branch = 1;

void readNumUnits(string filename)
{
    ifstream inputFile(filename);
    string line;

    if (!inputFile.is_open())
    {
        cerr << "Error opening file" << endl;
        return;
    }
    while (getline(inputFile, line))
    {
        istringstream iss(line);
        string word;
        while (iss >> word)
        {
            string word1 = word.substr(0, 9);
            if (word1.compare("NUM_ALUS=") == 0)
            {
                num_merged = stoi(word.substr(10), 0, 10);
            }
            if (word1.compare("NUM_BFUS=") == 0)
            {
                num_BFU = stoi(word.substr(10), 0, 10) - 1; // -1 for the branch. I don't know what to do about the branch unit yet
            }
        }
    }
    instruction_width = num_BFU + num_branch + num_merged;
}

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

vector<vector<Instruction>> read(const string &filename)
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

    while (getline(inputFile, line))
    {
        if (CurPacket.size() >= instruction_width)
        {
            VLIW.push_back(CurPacket);
            CurPacket.clear();
        }

        istringstream iss(line);
        string word;

        while (iss >> word)
        {

            int num = stringToBinary(word);
            Instruction CurInstruct(num);
            CurPacket.push_back(CurInstruct);
        }
    }

    if (!CurPacket.empty())
    {
        // Only push CurPacket if it contains exactly instruction_width instructions
        if (CurPacket.size() == instruction_width)
        {
            VLIW.push_back(CurPacket);
        }
        else
        {
            cout << "Warning: Incomplete packet" << endl;
        }
    }

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

    if (argc != 3) // error if wrong amount of arguments arguments
    {
        cerr << "Usage: " << argv[0] << " <file_path>" << endl;
        return 1;
    }

    string filePath_instruction = argv[1];
    string filePath_config = argv[2];

    // reading an parsing done here (bug will appear since "consts.hpp" has different dimensions for VLIW than my hardcoded main)
    vector<vector<Instruction>> instructions = read(filePath_instruction);

    vector<shared_ptr<FunctionalUnit>> allUnits; // IDK why this works, but stack overflow says it does

    while (num_branch != 0)
    {
        allUnits.push_back(make_shared<BranchUnit>());
        num_branch--;
    }
    // while (num_BFU != 0) add back when BFU is written
    // {
    //     allUnits.push_back(make_shared<BFU>());
    //     num_BFU--;
    // }
    while (num_merged != 0)
    {
        allUnits.push_back(make_shared<ALU>()); // This needs to change in order to accomodate between green and blue functional unit once wrapper class is written
        num_merged--;
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
            allUnits.at(i)->processInstruction(temp_instr, NextState);
        }
        CurrentState = NextState;
        // processInstruction(instructions, Machine0);
    }

    cout << "Hello, World!" << endl;
    return 0;
}
