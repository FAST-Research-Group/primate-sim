#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdint>
#include <cassert>
#include <algorithm>
#include <cctype>
#include <locale>

class PrimateConfig
{
public:
  enum FunctionalUnitType
  {
    BFU,
    GFU,
    MERGED,
    BRANCH,
    INSERT,
    EXTRACT
  };
  std::vector<FunctionalUnitType> instrLayout;
  int Reg_Width = 0; // handled in parsing
  std::vector<int> Reg_Block_Width;
  int Num_Regblocks = 0;
  std::vector<int> Src_Pos;
  std::vector<int> Src_Mode;
  int Max_Field_Width = 0;
  int Num_Src_Pos = 0;
  int Num_Src_Modes = 0;
  std::vector<int> Dst_Pos;
  std::vector<int> Dst_Encode;
  std::vector<std::pair<int, int>> Dst_En_Encode;
  std::vector<int> Dst_En;
  std::vector<std::string> BFUNames;
  int Num_Dst_Pos = 0;
  int Num_Wb_Ens = 0;
  int Num_Threads = 0;
  int Num_Regs = 0;
  int num_merged = 0;
  int num_ALU = 0;
  int num_BFU = 0;
  int IP_Width = 0;
  int Imm_Width = 0;

  // these are added to make functional unit distribution more clear
  int num_branch = 1;
  int instruction_width = 0;

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

  // parse the bfu list and populated the BFUNames list
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

    num_BFU = BFUNames.size(); // bug, you should have this many BFUS ...

    // std::cout << BFUNames.size() << std::endl;
  }

  // ctor to read file
  PrimateConfig(std::string fpath, std::string bfuListsPath)
  {
    // std::cout << "Opening config file at: " << fpath << "\n";
    std::ifstream inputFile(fpath);
    std::string line;

    if (!inputFile.is_open())
    {
      std::cerr << "Error opening primate config file at: " << fpath << std::endl;
      exit(-1);
    }

    // the substrings will throw out of range errors if no check in place for length
    while (std::getline(inputFile, line))
    {
      std::istringstream iss(line);
      std::string word;
      std::string word1;
      while (iss >> word)
      {
        if (word.length() >= 7)
        {
          word1 = word.substr(0, 7);
          if (word1.compare("DST_EN=") == 0)
          {

            // std::cout << "Initial word: " << word << std::endl;
            // std::cout << "Attempting to convert: " << word.substr(7) << std::endl;
            Dst_En.push_back(stoi(word.substr(7), 0, 10));
            while (iss >> word)
            {
              Dst_En.push_back(stoi(word));
            }
          }
        }

        if (word.length() >= 8)
        {
          word1 = word.substr(0, 8);
          if (word1.compare("SRC_POS=") == 0)
          {
            // std::cout << "Initial word: " << word << std::endl;
            // std::cout << "Attempting to convert: " << word.substr(8) << std::endl;
            Src_Pos.push_back(stoi(word.substr(8), 0, 10));
            while (iss >> word)
            {
              Src_Pos.push_back(stoi(word, 0, 10));
            }
          }
          if (word1.compare("DST_POS=") == 0)
          {

            // std::cout << "Initial word: " << word << std::endl;
            // std::cout << "Attempting to convert: " << word.substr(8) << std::endl;
            Dst_Pos.push_back(stoi(word.substr(8), 0, 10));
            while (iss >> word)
            {
              Dst_Pos.push_back(stoi(word, 0, 10));
            }
          }
        }

        if (word.length() >= 9)
        {
          word1 = word.substr(0, 9);
          if (word1.compare("NUM_ALUS=") == 0)
          {

            num_ALU = stoi(word.substr(9), 0, 10);
          }
          if (word1.compare("NUM_BFUS=") == 0)
          {

            num_BFU = stoi(word.substr(9), 0, 10);
          }
          if (word1.compare("SRC_MODE=") == 0)
          {

            Src_Mode.push_back(stoi(word.substr(9), 0, 10));
            while (iss >> word)
            {
              Src_Mode.push_back(stoi(word));
            }
          }
          if (word1.compare("NUM_REGS=") == 0)
          {
            Num_Regs = stoi(word.substr(9), 0, 10);
          }
          if (word1.compare("IP_WIDTH=") == 0)
          {
            IP_Width = stoi(word.substr(9), 0, 10);
          }
        }

        if (word.length() >= 10)
        {
          word1 = word.substr(0, 10);
          if (word1.compare("REG_WIDTH=") == 0)
          {
            Reg_Width = stoi(word.substr(10), 0, 10);
          }

          if (word1.compare("IMM_WIDTH=") == 0)
          {
            Imm_Width = stoi(word.substr(10), 0, 10);
          }
        }

        if (word.length() >= 11)
        {
          word1 = word.substr(0, 11);
          if (word1.compare("DST_ENCODE=") == 0)
          {
            Dst_Encode.push_back(stoi(word.substr(11), 0, 10));
            while (iss >> word)
            {
              Dst_Encode.push_back(stoi(word, 0, 10));
            }
          }
          if (word1.compare("NUM_WB_ENS=") == 0)
          {
            Num_Wb_Ens = stoi(word.substr(11), 0, 10);
          }
        }

        if (word.length() >= 12)
        {
          word1 = word.substr(0, 12);
          if (word1.compare("NUM_SRC_POS=") == 0)
          {
            Num_Src_Pos = stoi(word.substr(12), 0, 10);
          }
          if (word1.compare("NUM_DST_POS=") == 0)
          {
            Num_Dst_Pos = stoi(word.substr(12), 0, 10);
          }
          if (word1.compare("NUM_THREADS=") == 0)
          {
            Num_Threads = stoi(word.substr(12), 0, 10);
          }
        }

        if (word.length() >= 14)
        {
          word1 = word.substr(0, 14);
          if (word1.compare("NUM_SRC_MODES=") == 0)
          {
            Num_Src_Modes = stoi(word.substr(14), 0, 10);
          }
          if (word1.compare("NUM_REGBLOCKS=") == 0)
          {
            Num_Regblocks = stoi(word.substr(14), 0, 10);
          }
          if (word1.compare("DST_EN_ENCODE=") == 0)
          {
            line = line.substr(11);
            std::stringstream iss(line);
            std::string pair;

            while (std::getline(iss, pair, ';'))
            {
              std::stringstream pair_stream(pair);
              int first, second;

              if (pair_stream >> first >> second)
              {
                Dst_En_Encode.push_back({first, second});
              }
            }
          }
        }

        if (word.length() >= 16)
        {
          word1 = word.substr(0, 16);
          if (word1.compare("REG_BLOCK_WIDTH=") == 0)
          {
            Reg_Block_Width.push_back(stoi(word.substr(16), 0, 10));
            while (iss >> word)
            {
              Reg_Block_Width.push_back(stoi(word, 0, 10));
            }
          }
          if (word1.compare("MAX_FIELD_WIDTH=") == 0)
          {
            Max_Field_Width = stoi(word.substr(16), 0, 10);
          }
        }
      }
    }
    parseBFULists(bfuListsPath);
    int aluTemp = num_ALU;

    int bfuTemp = num_BFU;

    num_merged = 0;
    while (aluTemp > 0 && bfuTemp > 0)
    {
      instrLayout.push_back(FunctionalUnitType::EXTRACT);
      instrLayout.push_back(FunctionalUnitType::EXTRACT);
      instrLayout.push_back(FunctionalUnitType::MERGED);
      instrLayout.push_back(FunctionalUnitType::INSERT);
      aluTemp--;
      bfuTemp--;
      num_merged++;
    }

    while (aluTemp > 0)
    {
      assert(bfuTemp == 0 && "placing ALUs while remaining BFUs. Should be merged....");
      instrLayout.push_back(FunctionalUnitType::EXTRACT);
      instrLayout.push_back(FunctionalUnitType::EXTRACT);
      instrLayout.push_back(FunctionalUnitType::GFU);
      instrLayout.push_back(FunctionalUnitType::INSERT);
      aluTemp--;
    }
    while (bfuTemp > 0)
    {
      instrLayout.push_back(FunctionalUnitType::BFU);
      bfuTemp--;
    }
    instrLayout.push_back(FunctionalUnitType::BRANCH);
    instruction_width = instrLayout.size();
  }

  void get_data()
  {
    std::cout << "REG_WIDTH: " << Reg_Width << std::endl;

    std::cout << "REG_BLOCK_WIDTH: ";
    for (int i = 0; i < Reg_Block_Width.size(); i++)
    {
      std::cout << Reg_Block_Width.at(i) << " ";
    }
    std::cout << "" << std::endl;

    std::cout << "NUM_REGBLOCKS: " << Num_Regblocks << std::endl;

    std::cout << "SRC_POS: ";
    for (int i = 0; i < Src_Pos.size(); i++)
    {
      std::cout << Src_Pos.at(i) << " ";
    }
    std::cout << "" << std::endl;

    std::cout << "SRC_MODE: ";
    for (int i = 0; i < Src_Mode.size(); i++)
    {
      std::cout << Src_Mode.at(i) << " ";
    }
    std::cout << "" << std::endl;

    std::cout << "MAX_FIELD_WIDTH: " << Max_Field_Width << std::endl;
    std::cout << "MAX_FIELD_WIDTH: " << Num_Src_Pos << std::endl;
    std::cout << "NUM_SRC_MODES: " << Num_Src_Modes << std::endl;
    std::cout << "NUM_SRC_MODS: " << Num_Dst_Pos << std::endl;

    std::cout << "DST_POS: ";
    for (int i = 0; i < Dst_Pos.size(); i++)
    {
      std::cout << Dst_Pos.at(i) << " ";
    }
    std::cout << "" << std::endl;

    std::cout << "DST_ENCODE: ";
    for (int i = 0; i < Dst_Encode.size(); i++)
    {
      std::cout << Dst_Encode.at(i) << " ";
    }
    std::cout << "" << std::endl;

    std::cout << "DST_EN_ENCODE: ";
    for (int i = 0; i < Dst_En_Encode.size(); i++)
    {
      std::cout << Dst_En_Encode.at(i).first << ",";
      std::cout << Dst_En_Encode.at(i).second << "; ";
    }
    std::cout << "" << std::endl;

    std::cout << "DST_EN: ";
    for (int i = 0; i < Dst_En.size(); i++)
    {
      std::cout << Dst_En.at(i) << " ";
    }
    std::cout << "" << std::endl;
    std::cout << "NUM_DST_POS: " << Num_Dst_Pos << std::endl;
    std::cout << "NUM_WB_ENS: " << Num_Wb_Ens << std::endl;
    std::cout << "NUM_THREADS: " << Num_Threads << std::endl;
    std::cout << "NUM_REGS: " << Num_Regs << std::endl;
    std::cout << "NUM_MERGED: " << num_merged << std::endl;
    std::cout << "NUM_BFU: " << num_BFU << std::endl;
    std::cout << "IP_WIDTH: " << IP_Width << std::endl;
    std::cout << "IMM_WIDTH: " << Imm_Width << std::endl;
    std::cout << "NUM_BRANCH: " << num_branch << std::endl;
    std::cout << "INSTRUCTION_WIDTH: " << instruction_width << std::endl;
  }

  std::vector<std::string> getBFUNames()
  {
    return BFUNames;
  }
};
