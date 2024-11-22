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

std::vector<std::string> BFUNames;

// trim from start (in place)
inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
                                    { return !std::isspace(ch); }));
}

// trim from end (in place)
inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
                         { return !std::isspace(ch); })
                .base(),
            s.end());
}

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
        std::cerr << "Error opening bfu_lists config file at: " << bfuListsPath << "!!!!" << std::endl;
    }
    ParserState parserState = OUT_SCOPE;
    while (std::getline(inputFile, line))
    {
        ltrim(line);
        rtrim(line);
        switch (parserState)
        {
        case IN_SCOPE:
        {
            if (line == "}")
            {
                parserState = OUT_SCOPE;
            }
            break;
        }
        case OUT_SCOPE:
        {
            if (line == "{")
            {
                parserState = IN_SCOPE;
            }
            else if (line.size() != 0)
            {
                BFUNames.push_back(line);
            }
        }
        }
    }
    BFUNames.push_back("lsu");
    BFUNames.push_back("io");
}

int main(int argc, char *argv[])
{
    // Parse the input BFU list
    parseBFULists(argv[1]);

    // Open the output file
    const char *filename = "BFUInstructions.cpp";
    std::ofstream outfile(filename);

    // Write standard library includes
    outfile << R"cpp(
#include <map>
#include <string>
#include <iostream>
#include "../inc/BFUInstructions.hpp"
)cpp";

    // Dynamically include user-defined namespace headers
    for (const auto &namespaceName : BFUNames)
    {
        outfile << "#include \"../BFUs/inc/" << namespaceName << ".hpp\"" << std::endl;
    }

    // Write the function definition
    outfile << "std::map<int, void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)> indexToFunction;" << std::endl;
    outfile << "std::map<void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index), std::string> functionToName;" << std::endl;
    outfile << "std::map<std::string, int> nameToIndex;" << std::endl;
    outfile
        << R"cpp(

void generateMaps(
    std::map<int, void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)> &indexToFunction,
    std::map<void (*)(Instruction &I, MachineState &CMS, MachineState &NMS, int &index), std::string> &functionToName,
    std::map<std::string, int> &nameToIndex
) {
)cpp";

    // Populate `indexToFunction` map
    outfile << "    // Populate indexToFunction map\n";
    for (size_t i = 0; i < BFUNames.size(); ++i)
    {
        outfile << "    indexToFunction[" << i << "] = " << BFUNames[i] << "::getFunctionPTR();" << std::endl;
    }

    // Populate `functionToName` map
    outfile << "\n    // Populate functionToName map\n";
    for (const auto &namespaceName : BFUNames)
    {
        outfile << "    functionToName[" << namespaceName << "::getFunctionPTR()] = \"" << namespaceName << "\";" << std::endl;
    }

    // Populate `nameToIndex` map
    outfile << "\n    // Populate nameToIndex map\n";
    for (size_t i = 0; i < BFUNames.size(); ++i)
    {
        outfile << "    nameToIndex[\"" << BFUNames[i] << "\"] = " << i << ";" << std::endl;
    }

    // Close the function and file
    outfile << R"cpp(
}
)cpp";

    outfile.close();
    return 0;
}