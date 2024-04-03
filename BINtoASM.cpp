#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>



using namespace std;

vector<string> hexadecimal;
vector<string> binary;
vector<string> assembly;

/*
 * This function will put all of the assembly functions in the assembly vector
 *
 * SHOULD WORK
 */

void read(const string& filename, vector<string> destination){
    ifstream inputFile(filename);
    string line;

    if(!inputFile.is_open()){
        cout<<"Error opening file"<< endl;
        return;
    }



    while (getline(inputFile,line)){
        istringstream iss(line);
        string word;

        while(iss>>word){
            destination.push_back(word);
        }
        destination.emplace_back("\n");
    }
}




/*
 * This function will take in a hex number and convert it into a string of 0's and 1's
 * the input is the hexadecimal vector, and the index of the hexadecimal vector
 * Input: Hexadecimal vector
 * Output: Binary vector
 *
 * SHOULD WORK
 * ******** BIG SWITCH STATEMENT*********
 */
vector<string> Convert_Binary(vector<string> input, vector<string> output){
    int i = 0;
    int j = 0;
    string result;

    while(i < input.size()){
        string hex_num = input.at(i);
        while(hex_num[j] != '\0'){
            switch(hex_num[i]){
                case '0':
                    result.append("0000");
                    break;
                case '1':
                    result.append("0001");
                    break;
                case '2':
                    result.append("0010");
                    break;
                case '3':
                    result.append("0011");
                    break;
                case '4':
                    result.append("0100");
                    break;
                case '5':
                    result.append("0101");
                    break;
                case '6':
                    result.append("0110");
                    break;
                case '7':
                    result.append("0111");
                    break;
                case '8':
                    result.append("1000");
                    break;
                case '9':
                    result.append("1001");
                    break;
                case 'a':
                case 'A':
                    result.append("1010");
                    break;
                case 'b':
                case 'B':
                    result.append("1011");
                    break;
                case 'c':
                case 'C':
                    result.append("1100");
                    break;
                case 'd':
                case 'D':
                    result.append("1101");
                    break;
                case 'e':
                case 'E':
                    result.append("1110");
                    break;
                case 'f':
                case 'F':
                    result.append("1111");
                default:
                    result.append("\n");
                    break;
            }
        }
        output.push_back(result);
        result = '\0';
    }

}



/*
 * This function takes
 *
 * Input: binary vector (input), assembly vector (output)
 * Output: assembly vector
 * *******NOTE FOR LATER********
 * The assembly vector might not be needed as it can be initialized in the function and returned
 */
vector<string> Convert_Assembly(vector<string> input, vector<string> output){
    //the first type will correspond with the first binary input; needed to simplify my conversion to
    //assembly
    vector<char> type; //R,I,S,B,U,J,C,N ***** C for custom ******* N for New Line **********
    string opcode;
    int i = 0;
    while(i < input.size()){
        const string& current = input.at(i);
        for(int j = 25; j < 32; j++){
            if(j < current.length()){
                opcode += current[j];
            }
            else{
                break;
            }

        } //this takes the opcode unless there is a new line
        if(opcode == "0010011" || opcode == "0000011" || opcode == "0001111" || opcode == "1110011"){
            type.emplace_back('I');
        }
        else if(opcode == "0110111" || opcode == "0010111"){
            type.emplace_back('U');
        }
        else if(opcode == "0110011"){
            type.emplace_back('R');
        }
        else if(opcode == "1101111" || opcode == "1100111"){
            type.emplace_back('J');
        }
        else if(opcode == "0100011"){
            type.emplace_back('S');
        }
        else if(opcode == "1100011"){
            type.emplace_back('B');
        }
        else if(opcode == "0001011" || opcode == "1011011" || opcode == "0101011" || opcode == "1111011"){
            type.emplace_back('C');
        }
        else{
            type.emplace_back('N');
        }
    }


}

/*
 * This function will write each assembly instruction in the Assembly.txt file
 * input: vector with assembly instructions
 */
void write(){

}

// ADDi,R0,X4,511
//   0001 1111 1111 00100 000 00000 0010011
int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
