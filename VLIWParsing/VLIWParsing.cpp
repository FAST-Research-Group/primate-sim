#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include "Instruction.cpp"

using namespace std;

int total_intructions = 14;

vector<vector<Instruction>> read(const string &filename)
{

    vector<vector<Instruction>> VLIW;
    vector<Instruction> CurPacket;

    ifstream inputFile(filename);
    string line;

    if (!inputFile.is_open())
    {
        cout << "Error opening file" << endl;
        return VLIW;
    }

    while (getline(inputFile, line))
    {

        if (CurPacket.size() >= total_intructions)
        {
            VLIW.push_back(CurPacket);
            CurPacket.clear();
        }

        istringstream iss(line);
        string word;

        Instruction CurInstruct(stoi(word, 0, 16));
        while (iss >> word)
        {
            CurPacket.push_back(CurInstruct);
        }
    }
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

int main()
{
    vector<vector<Instruction>> help = read("Assembly.txt");

    std::cout << "Hello, World!" << std::endl;
    return 0;
}