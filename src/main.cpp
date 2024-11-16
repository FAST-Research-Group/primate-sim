#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdint>
#include <unordered_set>
#include <memory>
// #include "PrimateConfig.hpp"
#include "MachineState.hpp"
#include "alu.hpp"
#include "Instruction.hpp"
#include "FunctionalUnit.hpp"
#include "extract.hpp"
#include "MergedUnit.hpp"
#include "Generated/BFUFactory.hpp"
#include "Insert.hpp"

#include "../Experimental Generator/BFUInstructions.cpp"

// these are the maps where the generated BFU instructions will go to
// for verification as well as to call by processInstruction in BFU class
extern std::map<int, void (*)(Instruction &I, MachineState &CMS, MachineState &NMS)> indexToFunction;
std::map<void (*)(Instruction &I, MachineState &CMS, MachineState &NMS), std::string> functionToName;
std::map<std::string, int> nameToIndex;

// using namespace std removed; it was still here

// stringToBinary function is used since issues with stoi staying in 32 bits
int stringToBinary(std::string test)
{
  int result = 0;
  for (int i = 0; i < 8; i++)
  {
    result = result << 4;
    switch (test[i])
    {

    case '0':
      result += 0b0000;
      break;
    case '1':
      result += 0b0001;
      break;
    case '2':
      result += 0b0010;
      break;
    case '3':
      result += 0b0011;
      break;
    case '4':
      result += 0b0100;
      break;
    case '5':
      result += 0b0101;
      break;
    case '6':
      result += 0b0110;
      break;
    case '7':
      result += 0b0111;
      break;
    case '8':
      result += 0b1000;
      break;
    case '9':
      result += 0b1001;
      break;
    case 'a':
    case 'A':
      result += 0b1010;
      break;
    case 'b':
    case 'B':
      result += 0b1011;
      break;
    case 'c':
    case 'C':
      result += 0b1100;
      break;
    case 'd':
    case 'D':
      result += 0b1101;
      break;
    case 'e':
    case 'E':
      result += 0b1110;
      break;
    default:
      result += 0b1111;
      break;
    }
  }
  return result;
}

std::vector<std::vector<Instruction>> read(const std::string &filename, PrimateConfig primateCfg)
{

  std::vector<std::vector<Instruction>> VLIW;
  std::vector<Instruction> CurPacket;

  std::ifstream inputFile(filename);
  std::string line;

  if (!inputFile.is_open())
  {
    std::cerr << "Error opening file" << std::endl;
    return VLIW;
  }

  int line_no = 0;
  while (getline(inputFile, line))
  {
    line_no++;
    if (line.size() == 0)
    {
      std::cout << "warning: empty line in program LINE: " << line_no;
      continue;
    }
    if (CurPacket.size() >= primateCfg.instruction_width)
    {
      // the line below reverses the order of instructions which isn't how the functional units are lined up
      std::reverse(CurPacket.begin(), CurPacket.end());
      VLIW.push_back(CurPacket);
      for (int i = 0; i < CurPacket.size(); i++)
      {
        // std::cout << CurPacket.at(i).get_rawinstruction() << std::endl;
      }
      CurPacket.clear();
    }

    int i = 0;
    while (i < line.size())
    {
      std::string word = line.substr(i, i + 8);
      int num = stringToBinary(word);
      Instruction CurInstruct(num);
      CurPacket.push_back(CurInstruct);
      i += 8;
    }
  }

  if (!CurPacket.empty())
  {
    // Only push CurPacket if it contains exactly instruction_width instructions
    if (CurPacket.size() == primateCfg.instruction_width)
    {
      std::reverse(CurPacket.begin(), CurPacket.end());
      VLIW.push_back(CurPacket);
    }
    else
    {
      std::cout << "Warning: Incomplete packet: Expected size: " << primateCfg.instruction_width << " Actual: " << CurPacket.size() << std::endl;
    }
  }

  std::cout << "Found " << VLIW.size() << " Instructions\n";

  inputFile.close();
  return VLIW;
}

void get_data(Instruction dat)
{
  std::cout << "Immediate: " << dat.get_immediate() << std::endl;
  std::cout << "Destination Register: " << dat.get_rd() << std::endl;
  std::cout << "RS1: " << dat.get_rs1() << std::endl;
  std::cout << "RS2: " << dat.get_rs2() << std::endl;
  std::cout << "Opcode: " << dat.get_opcode() << std::endl;
  std::cout << "Funct3: " << dat.get_funct3() << std::endl;
  std::cout << "Funct7: " << dat.get_funct7() << std::endl;
  std::cout << "Type: " << dat.get_type() << std::endl;
}

int main(int argc, char *argv[])
{
  // error if wrong amount of arguments arguments
  if (argc != 4)
  {
    std::cerr << "Usage: " << argv[0] << " <path to program.bin> <path to primate.cfg> <path to BFUListsPath>" << std::endl;
    return 1;
  }

  /*
   * The code below are three chained maps to ensure that the BFU instructions
   * are mapped to their proper index
   */

  // Call the function to populate the maps
  generateMaps(indexToFunction, functionToName, nameToIndex);

  // Validate the mappings
  for (const auto &[index, functionPtr] : indexToFunction)
  {
    // Retrieve the name from the function pointer
    std::string name = functionToName[functionPtr];

    // Retrieve the index from the name
    int derivedIndex = nameToIndex[name];

    // Assert consistency
    assert(index == derivedIndex && "Index mismatch in the mappings!");
    assert(indexToFunction[derivedIndex] == functionPtr && "Function pointer mismatch in the mappings!");
    assert(functionToName[indexToFunction[derivedIndex]] == name && "Name mismatch in the mappings!");

    // Optional: Print details for debugging
    // std::cout << "Index: " << index
    //           << ", Function Pointer: " << functionPtr
    //           << ", Name: " << name
    //           << ", Derived Index: " << derivedIndex
    //           << " [Valid]" << std::endl;
  }

  std::cout << "All mappings are consistent and validated." << std::endl;

  /* The added code ends here*/

  std::string filePath_instruction = argv[1];
  std::string filePath_config = argv[2];
  std::string BFUListsPath = argv[3];
  PrimateConfig primateCfg(filePath_config, BFUListsPath);

  // primateCfg.get_data();
  std::cout << "done with config reading\n";
  // std::cout << " \n\n\n " << std::endl;

  // reading an parsing done here (bug will appear since "consts.hpp" has different dimensions for VLIW than my hardcoded main);
  std::vector<std::vector<Instruction>> instructions = read(filePath_instruction, primateCfg);

  std::vector<std::unique_ptr<FunctionalUnit>> allUnits;
  int slotIdx = 0;
  auto bfuNameIter = primateCfg.getBFUNames().begin();
  for (auto &slotType : primateCfg.instrLayout)
  {
    switch (slotType)
    {
    case PrimateConfig::FunctionalUnitType::BRANCH:
      allUnits.push_back(std::make_unique<BranchUnit>(false, slotIdx));
      break;
    case PrimateConfig::FunctionalUnitType::BFU:
      allUnits.push_back(createBFU(*bfuNameIter, true, slotIdx));
      bfuNameIter++;
      break;
    case PrimateConfig::FunctionalUnitType::GFU:
      allUnits.push_back(std::make_unique<ALU>(false, slotIdx));
      break;
    case PrimateConfig::FunctionalUnitType::MERGED:
      allUnits.push_back(std::make_unique<MergedUnit>(createBFU(*bfuNameIter, false, slotIdx), std::make_unique<ALU>(false, slotIdx), false, slotIdx));
      bfuNameIter++;
      break;
    case PrimateConfig::FunctionalUnitType::EXTRACT:
      allUnits.push_back(std::make_unique<ExtractUnit>(primateCfg, true, slotIdx));
      break;
    case PrimateConfig::FunctionalUnitType::INSERT:
      allUnits.push_back(std::make_unique<InsertUnit>(primateCfg, true, slotIdx));
      break;
    default:
      assert(false && "unknown functional unit type (if you added a new unit add it to main.cpp)");
    }
    slotIdx++;
  }
  // std::cout << "NUM UNITS: " << allUnits.size() << std::endl; // used for debugging
  // initial machine state (!!!!!!!!!!!! This will be a bug that needs to be changed)
  MachineState CurrentState(0, primateCfg), NextState(0, primateCfg);

  // Initialize Functional Units over here

  // Throw warnings if same destination
  // If statement necessary in order to make sure that I don't get an out of range exception from the nested for loopss
  for (int i = 0; i < instructions.size(); i++)
  {
    std::set<int> CurrentDestinations;

    for (int j = 1; j < instructions.at(i).size(); j++)
    {
      int destination = instructions.at(i).at(j).get_rawinstruction();
      if (destination == 19)
      {
        continue;
      }

      if (CurrentDestinations.count(destination))
      {
        std::cout << "Duplicate entry found at instruction " << i << ". Rd = " << instructions.at(i).at(j).get_rd() << std::endl;
      }
      CurrentDestinations.insert(destination);
    }
  }
  std::cout << "Finished Destination Register Collision Check" << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;
  std::cout << std::endl;

  while (CurrentState.running)
  {
    if (CurrentState.getPC() >= instructions.size())
    {
      std::cout << "FATAL ERROR: PC larger than number of instructions!!!!!\n";
      exit(-1);
    }
    for (int i = 0; i < allUnits.size(); i++)
    {
      Instruction &temp_instr = instructions.at(CurrentState.getPC()).at(i);
      allUnits.at(i)->processInstruction(temp_instr, CurrentState, NextState);
    }
    CurrentState = NextState;
    // processInstruction(instructions, Machine0);
  }

  std::cout << "Sim ended with state:\n"
            << CurrentState << std::endl;
  return 0;
}
