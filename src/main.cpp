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
#include "insert.hpp"

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
      std::reverse(CurPacket.begin(), CurPacket.end());
      VLIW.push_back(CurPacket);
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
  if (argc != 3)
  {
    std::cerr << "Usage: " << argv[0] << " <path to program.bin> <path to primate.cfg> <path to BFUListsPath>" << std::endl;
    return 1;
  }

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
  std::vector<int> typeOfUnit; // Green - 0, Blue - 1, Merged - 2, Insert - 3, Extract - 4, Branch - 5

  int slotIdx = 0;
  auto bfuNameIter = primateCfg.getBFUNames().begin();
  for (auto &slotType : primateCfg.instrLayout)
  {
    switch (slotType)
    {
    case PrimateConfig::FunctionalUnitType::BRANCH:
      allUnits.push_back(std::make_unique<BranchUnit>(false, slotIdx));
      typeOfUnit.push_back(5);
      break;
    case PrimateConfig::FunctionalUnitType::BFU:
      allUnits.push_back(createBFU(*bfuNameIter, true, slotIdx));
      bfuNameIter++;
      typeOfUnit.push_back(1);
      break;
    case PrimateConfig::FunctionalUnitType::GFU:
      allUnits.push_back(std::make_unique<ALU>(false, slotIdx));
      typeOfUnit.push_back(0);
      break;
    case PrimateConfig::FunctionalUnitType::MERGED:
      allUnits.push_back(std::make_unique<MergedUnit>(createBFU(*bfuNameIter, false, slotIdx), std::make_unique<ALU>(false, slotIdx), false, slotIdx));
      bfuNameIter++;
      typeOfUnit.push_back(2);
      break;
    case PrimateConfig::FunctionalUnitType::EXTRACT:
      allUnits.push_back(std::make_unique<ExtractUnit>(primateCfg, true, slotIdx)); // Potential BUG: idk if this is intended behavior
      typeOfUnit.push_back(4);
      break;
    case PrimateConfig::FunctionalUnitType::INSERT:
      allUnits.push_back(std::make_unique<InsertUnit>(primateCfg, true, slotIdx)); // need to merge the insert unit
      typeOfUnit.push_back(3);
    default:
      assert(false && "unknown functional unit type (if you added a new unit add it to main.cpp)");
    }
    slotIdx++;
  }

  // initial machine state (!!!!!!!!!!!! This will be a bug that needs to be changed)
  MachineState CurrentState(0, primateCfg), NextState(0, primateCfg), TempState(0, primateCfg); // tempState will be used to store the extracts and load inserts Potential BUG

  // Initialize Functional Units over here

  // Throw warnings if same destination
  // If statement necessary in order to make sure that I don't get an out of range exception from the nested for loopss
  // This has to be updated in order to be more accurate
  if (instructions.size() >= 2)
  {
    for (auto &instr : instructions)
    {
      std::unordered_set<int> destRegs;
      for (auto &subInstr : instr)
      {
        int rd = subInstr.get_rd();
        if (destRegs.find(rd) == destRegs.end())
        {
          destRegs.insert(rd);
        }
        else
        {
          auto instrIter = std::find(instructions.begin(), instructions.end(), instr);
          std::cout << "WARNING: Two subinstrs write to same reg in Instr: "
                    << std::distance(instructions.begin(), instrIter)
                    << " dest reg: " << rd
                    << "\n";
        }
      }
    }
  }

  while (CurrentState.running)
  {
    if (CurrentState.getPC() >= instructions.size())
    {
      std::cout << "FATAL ERROR: PC larger than number of instructions!!!!!\n";
      exit(-1);
    }
    for (int i = 0; i < allUnits.size(); i++)
    {
      // Green - 0, Blue - 1, Merged - 2, Insert - 3, Extract - 4, Branch - 5
      // code brokie, determine how to assign to tempMachineState() for certain units
      Instruction &temp_instr = instructions.at(CurrentState.getPC()).at(i);
      // BUG: I hope this is right, idk how else to handle the different dataflow stuff :)
      switch (typeOfUnit.at(i))
      {
      case 0:
        allUnits.at(i)->processInstruction(temp_instr, TempState, TempState);
        break;
      case 1:
        allUnits.at(i)->processInstruction(temp_instr, CurrentState, NextState);
        break;
      case 2:
        allUnits.at(i)->processInstruction(temp_instr, TempState, TempState);
        break;
      case 3:
        allUnits.at(i)->processInstruction(temp_instr, TempState, NextState);
        break;
      case 4:
        allUnits.at(i)->processInstruction(temp_instr, CurrentState, TempState);
        break;
      case 5:
      default:
        allUnits.at(i)->processInstruction(temp_instr, CurrentState, NextState);
        break;
      }
    }
    CurrentState = NextState;
    // processInstruction(instructions, Machine0);
  }

  std::cout << "Sim ended with state:\n"
            << CurrentState << std::endl;
  return 0;
}
