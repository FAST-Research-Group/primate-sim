#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include "Instruction.cpp"

using namespace std;

int total_instructions = 14;

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
        if (CurPacket.size() >= total_instructions)
        {
            VLIW.push_back(CurPacket);
            CurPacket.clear();
        }

        istringstream iss(line);
        string word;

        while (iss >> word)
        {
            Instruction CurInstruct(stoi(word, 0, 16));
            CurPacket.push_back(CurInstruct);
        }
    }

    if (!CurPacket.empty())
    {
        // Only push CurPacket if it contains exactly total_instructions instructions
        if (CurPacket.size() == total_instructions)
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
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <file_path>" << endl;
        return 1;
    }

    string filePath = argv[1];
    vector<vector<Instruction>> instructions = read(filePath);

    ofstream outfile("text.txt");
    if (!outfile.is_open())
    {
        cerr << "Error: Could not open the file 'text.txt' for writing." << endl;
        return 1;
    }

    int i = 0;
    while (i < instructions.size())
    {
        int j = 0; // Reset j for each new outer loop iteration
        while (j < instructions.at(i).size())
        {
            outfile << instructions.at(i).at(j).get_instruction() << endl;
            j++;
        }
        i++;
    }

    outfile.close();

    cout << "Hello, World!" << endl;
    return 0;
}
