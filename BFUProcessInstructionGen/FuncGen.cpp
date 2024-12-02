#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <vector>
#include <sstream>
#include <string>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <cctype>
#include <locale>
#include <filesystem>

std::vector<std::string> BFUNames;

// Trim from start (in place)
inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

// Trim from end (in place)
inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
}

// Parse the BFU list from file
void parseBFULists(std::string bfuListsPath)
{
    enum ParserState
    {
        IN_SCOPE,
        OUT_SCOPE
    };

    std::ifstream inputFile(bfuListsPath);
    std::string line;

    if (!inputFile.is_open())
    {
        std::cerr << "Error opening BFU lists config file at: " << bfuListsPath << "!!!!" << std::endl;
        return;
    }

    ParserState parserState = OUT_SCOPE;
    while (std::getline(inputFile, line))
    {
        ltrim(line);
        rtrim(line);

        switch (parserState)
        {
        case IN_SCOPE:
            if (line == "}")
            {
                parserState = OUT_SCOPE;
            }
            break;

        case OUT_SCOPE:
            if (line == "{")
            {
                parserState = IN_SCOPE;
            }
            else if (!line.empty())
            {
                BFUNames.push_back(line);
            }
            break;
        }
    }

    BFUNames.push_back("lsu");
    BFUNames.push_back("io");
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <bfu_lists_file_path>" << std::endl;
        return 1;
    }

    // Parse the input BFU list
    parseBFULists(argv[1]);

    // Generate output files for each BFU
    for (int i = 0; i < BFUNames.size() - 2; i++)
    {
        std::string bfuName = BFUNames.at(i);
        std::string filename = bfuName + "_gen.cpp";
        std::ofstream outfile(filename);

        if (!outfile.is_open())
        {
            std::cerr << "Error opening file for writing: " << filename << std::endl;
            continue;
        }

        // Write the file content
        outfile << "#include \"../../BFUs/inc/" << bfuName << ".hpp\"" << std::endl;
        outfile << "namespace " << bfuName << " {" << std::endl;

        // processInstruction function
        outfile << "    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS, int &index) {" << std::endl;
        outfile << "        bool regFile = (index < 0) ? 1 : 0;" << std::endl;
        outfile << "        struct " << bfuName << " input;" << std::endl;
        outfile << "        struct " << bfuName << " output;" << std::endl;
        outfile << "        if (regFile) {" << std::endl;
        outfile << "            input = Reg2struct(CMS.getRegister(I.get_rs1()));" << std::endl;
        outfile << "        } else {" << std::endl;
        outfile << "            input = Reg2struct(CMS.getInterconnectValue(index));" << std::endl;
        outfile << "        }" << std::endl;
        outfile << "        struct " << bfuName << " output = process" << bfuName << "(input);" << std::endl;
        outfile << "        Register result = struct2Reg(output);" << std::endl;
        outfile << "        if (regFile) {" << std::endl;
        outfile << "            NMS.setRegister(I.get_rd(), result);" << std::endl;
        outfile << "        } else {" << std::endl;
        outfile << "            CMS.setInterconnectValue(index + 2, result);" << std::endl;
        outfile << "        }" << std::endl;
        outfile << "    }" << std::endl;

        // getFunctionPTR function
        outfile << "    void (*getFunctionPTR())(Instruction &I, MachineState &CMS, MachineState &NMS, int &index) {" << std::endl;
        outfile << "        return &processInstruction;" << std::endl;
        outfile << "    }" << std::endl;

        outfile << "}" << std::endl; // Close namespace

        outfile.close();

        // Move the file to the "src" folder
        try
        {
            std::filesystem::rename(filename, std::filesystem::path("src") / filename);
        }
        catch (const std::filesystem::filesystem_error &e)
        {
            std::cerr << "Error moving file: " << filename << " to src directory. " << e.what() << std::endl;
        }
    }

    return 0;
}
