#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include "Instruction.cpp"
#include <cstdint>
#include "MachineState.cpp"

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
            uint32_t unum = (uint32_t)(stoul(word, nullptr, 16));
            int num = static_cast<int>(unum);
            Instruction CurInstruct(num);
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
    /*Pseudo code to be implemented as suggested by Kayvan and interpreted by me
    // error if not enough arguments
    // error if too many arguments

    // read in the file

    // parse the file

    // create initial machine state (Machine state new class)

    // create Backend instance (vector<FunctionalUnit>)

    // loop program until finished
    while(!halted) {


        // fetch - already done by parsing the file


        // decode - part of the execute since instruction will go to functional unit


        // execute - function proc(instruction) will be run, method written by someone else, can be modified to run each instruction of a vector or one VLIW
        !!!!!!! Throw a warning if the destination registers of a VLIW are the same to help debug the compiler LLVM stuff in the future
        !!!!!!! An error might occur from consts.hpp

        // writeback - TBD who updates the machine state
    }

    // print final machine state (eventually)
    */

    if (argc != 2) // error if wrong amount of arguments arguments
    {
        cerr << "Usage: " << argv[0] << " <file_path>" << endl;
        return 1;
    }

    // current bug with stoi that doesn't work with the TCP file

    string filePath = argv[1];
    vector<vector<Instruction>> instructions = read(filePath); // reading an parsing done here

    MachineState test = MachineState(0); // initial machine state (!!!!!!!!!!!! This will be a bug that needs to be changed)

    // Throw warnings if same destination
    for (int i = 0; i < instructions.size(); i++)
    {
        for (int j = 0; i < instructions.at(i).size() - 1; j++)
        {
            for (int k = 1; k < instructions.at(i).size(); k++)
                if (instructions.at(i).at(j).get_rd() == instructions.at(i).at(k).get_rd())
                {
                    cout << j << " and " << k << " have the same destination register in Instruction " << i << endl;
                }
        }
    }

    while (test.running)
    {
        for (int i = 0; i < INSTRS_PER_PACKET; i++)
        {
            proc_instruction(instructions.at(i));
        }
    }

    /*
    // starting here 1.0, this creates an output file that I needed in order to test that I was parsing the instructions properly

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
            outfile << instructions.at(i).at(j).get_instruction() << " ";
            j++;
        }
        outfile << endl;
        i++;
    }

    outfile.close();
    // ending here 1.0
    */

    cout << "Hello, World!" << endl;
    return 0;
}
