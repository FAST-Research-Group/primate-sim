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

        while(iss>>word){
            hexadecimal.push_back(word);
        }
        hexadecimal.emplace_back("\n");
    }
}




/*
 * This function will take in a hex number and convert it into a string of 0's and 1's
 * the input is the hexadecimal vector, and the index of the hexadecimal vector
 * Input: Hexadecimal vector
 * Output: Global variable, binary, that stores all binary numbers with new line characters
 *
 * SHOULD WORK
 * ******** BIG SWITCH STATEMENT*********
 */
void Convert_Binary(){
    int i = 0;
    int j = 0;
    string result;
    // return this. Should be in binary with new lines for each instruction

    while(i < hexadecimal.size()){
        j=0;
        string hex_num = hexadecimal.at(i);
        while(hex_num[j] != '\0'){
            switch(hex_num[j]){
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
            j++;
        }
        binary.push_back(result);
        result = "";
        i++;
    }
}


/*
 * This function will convert a binary string into a 2's complement int and back to string
 * input: binary string
 * output: 2's complement string
 */
string bin_integer_signed(string binary_string){
    int number = stoi(binary_string,0,2);
    if(binary_string[0] == '1'){
        number -= (1<<binary_string.length());
    }
    return to_string(number);
}

string bin_integer_unsigned(const string& binary_string){
    int number = stoi(binary_string,0,2);
    return to_string(number);
}

/*
 * This function takes
 *
 * Input: binary vector (input), assembly vector (output)
 * Output: assembly vector
 * *******NOTE FOR LATER********
 * The assembly vector might not be needed as it can be initialized in the function and returned
 */
void Convert_Assembly(){
    //the first type will correspond with the first binary input; needed to simplify my conversion to
    //assembly
    vector<char> type; //R,I,S,B,U,J,C,N ***** C for custom ******* N for New Line **********
    string opcode; //holds the last 7 bits of an instruction
    string func3; //this will be the funct 3 bits
    string immediate; //this will hold the immediate bits
    string rd; // bits 7-11
    string rs1; // bits 15-19
    string rs2; // bits 24-20
    string funct7;


    string result; //this will be the final instruction
    int i = 0;
    int j;
    while(i < binary.size()){
        result = "\0";
        func3 = "\0";
        rd = "\0";
        rs1 = "\0";
        rs2 = "\0";
        opcode = "\0";
        funct7 = "\0";
        immediate = "";

        string& current = binary.at(i); //current binary instruction
        if(current.length() < 10){
            assembly.emplace_back("\n");
            i++;
            continue;
        }
        for(j = 25; j < 32; j++){
            if(j < current.length()){
                opcode += current[j];
            }
            else{
                break;
            }

        } //this takes the opcode unless there is a new line

        //end here if newline

        if(opcode == "0010011" || opcode == "0000011" || opcode == "0001111" || opcode == "1110011"){ //type I instruction (immediate)
            // these for loops will generate the proper numbers for me to append to the result before storing it in the assembly
            for(j = 17; j < 20; j ++){
                func3 += current[j];
            }
            for(j = 0; j < 12;j++){
                immediate += current[j];
            }
            for(j=12;j<17;j++){
                rs1 += current[j];
            }
            for(j = 20; j < 25; j++){
                rd += current[j];
            }


            if(opcode == "0010011"){ // I-Type instructions
                if(func3 == "000"){
                    result = "ADDi,R";
                    result.append(bin_integer_unsigned(rd));
                    result += ",R";
                    result.append(bin_integer_unsigned(rs1));
                    result += ",";
                    result.append(bin_integer_signed(immediate));
                    result += ";";
                }
                else if(func3 == "010"){
                    result += "SLTI,R";
                    result.append(bin_integer_unsigned(rd));
                    result += ",R";
                    result.append(bin_integer_unsigned(rs1));
                    result += ",";
                    result.append(bin_integer_signed(immediate));
                    result += ";";

                }
                else if(func3 == "011"){
                    result += "SLTIU,R";
                    result.append(bin_integer_unsigned(rd));
                    result += ",R";
                    result.append(bin_integer_unsigned(rs1));
                    result += ",";
                    result.append(bin_integer_signed(immediate));
                    result += ";";
                }
                else if(func3 == "111"){
                    result += "ANDi,R";
                    result.append(bin_integer_unsigned(rd));
                    result += ",R";
                    result.append(bin_integer_unsigned(rs1));
                    result += ",";
                    result.append(bin_integer_signed(immediate));
                    result += ";";
                }
                else if(func3 == "110"){
                    result += "ORi,R";
                    result.append(bin_integer_unsigned(rd));
                    result += ",R";
                    result.append(bin_integer_unsigned(rs1));
                    result += ",";
                    result.append(bin_integer_signed(immediate));
                    result += ";";
                }
                else if(func3 == "100"){
                    result += "XORi,R";
                    result.append(bin_integer_unsigned(rd));
                    result += ",R";
                    result.append(bin_integer_unsigned(rs1));
                    result += ",";
                    result.append(bin_integer_signed(immediate));
                    result += ";";
                }
                else if(func3 == "001"){
                    result += "SLLi,R";
                    result.append(bin_integer_unsigned(rd));
                    result += ",R";
                    result.append(bin_integer_unsigned(rs1));
                    result += ",";
                    result.append(bin_integer_signed(immediate.substr(7)));
                    result += ";";
                }
                else{
                    if(func3[1] == '1'){
                        result += "SRAi,R";
                    }
                    else{
                        result += "SRLi,R";
                    }
                    result.append(bin_integer_unsigned(rd));
                    result += ",R";
                    result.append(bin_integer_unsigned(rs1));
                    result += ",";
                    result.append(bin_integer_signed(immediate.substr(7)));
                    result += ";";
                }
            }
            else if(opcode == "0000011"){
                if(func3 == "010"){
                    result += "LW,R";
                }
                else if(func3 == "001"){
                    result += "LH,R";
                }
                else if(func3 == "101"){
                    result += "LHU,R";
                }
                else{
                    result += "LBU,R";
                }
                result.append(bin_integer_unsigned(rd));
                result += ",R";
                result.append(bin_integer_unsigned(rs1));
                result += ",";
                result.append(bin_integer_signed(immediate));
                result += ";";

            }
            else if(opcode == "0001111"){
                if(func3 == "000"){
                    result += "FENCE,R";
                }
                else{
                    result += "FENCE.I,R";
                }
                result.append(bin_integer_unsigned(rd));
                result += ",R";
                result.append(bin_integer_unsigned(rs1));
                result += ",";
                result.append(bin_integer_signed(immediate));
                result += ";";
            }
            else{
                if(func3 == "000"){
                    if(immediate[11] == '0'){
                        result += "ECALL,R";
                    }
                    else{
                        result += "EBREAK,R";
                    }
                }
                result.append(bin_integer_unsigned(rd));
                result += ",R";
                result.append(bin_integer_unsigned(rs1));
                result += ",";
                result.append(bin_integer_signed(immediate));
                result += ";";
            }

        }
        else if(opcode == "0110111" || opcode == "0010111"){ // U-Type Instructions
            for(j = 0; j < 20;j++){
                immediate += current[j];
            }
            for(j = 20;j < 25;j++){
                rd += current[j];
            }

            if(opcode == "0110111"){
                result += "LUI,R";
                result.append(bin_integer_unsigned(rd));
                result+= ",";
                result.append(bin_integer_signed(immediate));
                result += ";";
            }
            else if(opcode == "0010111"){
                result += "AUIPC,R";
                result.append(bin_integer_unsigned(rd));
                result += ",";
                result.append(bin_integer_signed(immediate));
                result += ";";
            }
        }
        else if(opcode == "0110011"){ // R-Type Instructions
            for(j = 17; j < 20; j ++){
                func3 += current[j];
            }
            for(j = 7; j < 12; j++){
                rs2 += current[j];
            }
            for(j=12;j<17;j++){
                rs1 += current[j];
            }

            if(func3 == "000"){
                if(current[1] == '1'){
                    result += "SUB,R";
                }
                else if(current[1] == '0'){
                    result += "ADD,R";
                }
                else{
                    result += "MUL,R";
                }
                result.append(bin_integer_unsigned(rd));
                result += ",R";
                result.append(bin_integer_unsigned(rs1));
                result += ",R";
                result.append(bin_integer_unsigned(rs2));
                result += ";";
            }
            else if(func3 == "010"){
                if(current[7] == '0'){
                    result += "SLT,R";
                }
                else{
                    result += "MULHSU,R";
                }
                result.append(bin_integer_unsigned(rd));
                result += ",R";
                result.append(bin_integer_unsigned(rs1));
                result += ",R";
                result.append(bin_integer_unsigned(rs2));
                result += ";";
            }
            else if(func3 == "011"){
                if(current[7] == '0'){
                    result += "SLTU,R";
                }
                else{
                    result += "MULHU,R";
                }
                result.append(bin_integer_unsigned(rd));
                result += ",R";
                result.append(bin_integer_unsigned(rs1));
                result += ",R";
                result.append(bin_integer_unsigned(rs2));
                result += ";";
            }
            else if(func3 == "111"){
                if(current[7] == '1'){
                    result += "REMU,R";
                }
                else{
                    result += "AND,R";
                }
                result.append(bin_integer_unsigned(rd));
                result += ",R";
                result.append(bin_integer_unsigned(rs1));
                result += ",R";
                result.append(bin_integer_unsigned(rs2));
                result += ";";
            }
            else if(func3 == "110"){
                if(current[7] == '1'){
                    result += "REM,R";
                }
                else{
                    result += "OR,R";
                }
                result.append(bin_integer_unsigned(rd));
                result += ",R";
                result.append(bin_integer_unsigned(rs1));
                result += ",R";
                result.append(bin_integer_unsigned(rs2));
                result += ";";
            }
            else if(func3 == "100"){
                if(current[i] == '1'){
                    result += "DIV,R";
                }
                else{
                    result += "XOR,R";
                }
                result.append(bin_integer_unsigned(rd));
                result += ",R";
                result.append(bin_integer_unsigned(rs1));
                result += ",R";
                result.append(bin_integer_unsigned(rs2));
                result += ";";
            }
            else if(func3 == "001"){
                if(current[7] == '1'){
                    result += "MULH,R";
                }
                else{
                    result += "SLL,R";
                }
                result.append(bin_integer_unsigned(rd));
                result += ",R";
                result.append(bin_integer_unsigned(rs1));
                result += ",R";
                result.append(bin_integer_unsigned(rs2));
                result += ";";
            }
            else{
                if(current[7] == '1'){
                    result += "DIVU,R";
                }
                if(current[1] == '1'){
                    result += "SRA,R";
                }
                else{
                    result += "SRL,R";
                }
                result.append(bin_integer_unsigned(rd));
                result += ",R";
                result.append(bin_integer_unsigned(rs1));
                result += ",";
                result.append(bin_integer_unsigned(rs2));
                result += ";";
            }

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
        assembly.push_back(result);
        i++;

    }


    for(i = 0; i<type.size();i++){
        const string& bin = binary.at(i);
        const string& current = binary.at(i);
        for(int l = 25; l < 32; l++){
            if(l < current.length()){
                opcode += current[l];
            }
            else{
                break;
            }

        } //this takes the opcode unless there is a new line

       if(type.at(i) == 'U'){
            return;
       }
       else if (type.at(i) == 'R'){
            return;
       }
       assembly.push_back(result);


    }
    i++;
}

/*
 * This function will write each assembly instruction in the Assembly.txt file
 * input: vector with assembly instructions
 * output new file with assembly instructions
 */
void write(const string& input_file){
    ofstream outputFile(input_file);
    for(int i = 0; i<assembly.size(); i++){
        outputFile<<assembly.at(i)<<flush;
    }
}

// ADDi,R0,X4,511
//   0001 1111 1111 00100 000 00000 0010011
int main() {
    read("test.txt");
    Convert_Binary();
    Convert_Assembly();
    write("Assembly.txt");
//    string hello = "hello";
//    hello += " world";
//    cout<<hello<<endl;
//    if(hello == "hello world"){
//        cout<<"true"<<endl;
//    }
//    ofstream outputFile("Assembly.txt"); // in order to write to Assembly.txt use outputFile<<"message"<<endl;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
