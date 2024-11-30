#include "IOUnit.hpp"
#include <fstream>
#include <iostream>

namespace io
{
    // Input stream used by IOUnit functions
    std::ifstream inputStream("input.txt");

    // Return a function pointer to processInstruction

    void handleInput(Instruction &I,
                     MachineState &CMS,
                     MachineState &NMS,
                     int &index)
    {
        switch (I.get_funct3())
        {
        case 1:
            handleInputRead(I, CMS, NMS, index);
            break;
        case 2:
            throw std::string("inputSeek is not an implemented instruction.");
            break;
        case 3:
            throw std::string("inputExtracti is not an implemented instruction.");
            break;
        case 4:
            throw std::string("inputDone is not an implemented instruction.");
            break;
        case 7:
            throw std::string(
                "inputExtractiDone is not an implemented instruction.");
            break;
        default:
            throw std::string("Not a custom instruction.");
            break;
        }
    }

    void handleInputRead(Instruction &I,
                         MachineState &CMS,
                         MachineState &NMS,
                         int &index)
    {
        bool regFile = (index < 0);

        int rd = I.get_rd();
        int imm = I.get_immediate();
        int holdInput = 0;
        char isLast, hasData;
        char separatorTrash;
        std::string temp;

        if (!inputStream.is_open())
        {
            inputStream.open("input.txt");
        }

        inputStream >> isLast >> separatorTrash >> hasData >> separatorTrash >> temp;

        for (int i = 0; i < imm; ++i)
            holdInput += (temp[i] - '0') << i;

        if (regFile)
        {
            NMS.setRegister(rd, holdInput);
        }
        else
        {
            NMS.setInterconnectValue(index + 2, holdInput);
        }
    }

    void handleOutputEmiti(Instruction &I,
                           MachineState &CMS,
                           MachineState &NMS,
                           int &index)
    {
        bool regFile = (index < 0);

        int rd = I.get_rd();
        int imm = I.get_immediate();
        Register mask = 0;
        for (int i = 1; i < imm; i++)
        {
            mask += Register(1) * i;
        }

        Register value;
        if (regFile)
        {
            value = CMS.getRegister(rd) & mask;
        }
        else
        {
            value = CMS.getInterconnectValue(index) & mask;
        }

        // Convert value to int for output, ensuring it's within int range
        int outputValue = value.convert_to<int>();
        std::cout << outputValue;
    }
} // namespace IOUnit