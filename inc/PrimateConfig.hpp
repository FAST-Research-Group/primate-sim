#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdint>

class PrimateConfig {
public:
  int instruction_width = 0;
  int num_BFU = 0;
  int num_merged = 0;
  int num_branch = 1;

  // ctor to read file
  PrimateConfig(std::string fpath) {
    std::cout << "Opening config file at: " << fpath << "\n";
    std::ifstream inputFile(fpath);
    std::string line;

    if (!inputFile.is_open()) {
      std::cerr << "Error opening primate config file at: " << fpath << std::endl;
      exit(-1);
    }
    while (std::getline(inputFile, line)) {
      std::istringstream iss(line);
      std::string word;
      while (iss >> word) {
        std::string word1 = word.substr(0, 9);
        if (word1.compare("NUM_ALUS=") == 0) {
          std::cout << word.substr(9) << std::endl;
          num_merged = stoi(word.substr(9), 0, 10);
        }
        if (word1.compare("NUM_BFUS=") == 0) {
          std::cout << word.substr(9) << std::endl;
          num_BFU = stoi(word.substr(9), 0, 10); 
        }
      }
    }
    instruction_width = num_BFU + num_branch + num_merged;
  }
  
};
