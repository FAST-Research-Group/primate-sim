#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include "Instruction.cpp"

using namespace std;



vector<string> binary;
vector<string> assembly;




vector<Instruction> instructions;

void read(const string& filename){
    ifstream inputFile(filename);
    string line;

    if(!inputFile.is_open()){
        cout<<"Error opening file"<< endl;
        return;
    }



    while (getline(inputFile,line)){
        istringstream iss(line);
        string word;
        Instruction help(stoi(word,0,16));
        while(iss>>word){
            instructions.push_back(help);
        }

    }
}


int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
