#pragma once

#include "MachineState.hpp"
#include "FunctionalUnit.hpp"
#include <fstream>

#define IO_UNIT_INPUT 11
#define IO_UNIT_OUTPUT 1

namespace io
{

    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS, int &index);
    void handleInput(Instruction &I, MachineState &CMS, MachineState &NMS);
    void handleInputRead(Instruction &I, MachineState &CMS, MachineState &NMS);
    void handleOutputEmiti(Instruction &I, MachineState &CMS, MachineState &NMS);
    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS, int &index);
    void (*getFunctionPTR())(Instruction &, MachineState &, MachineState &, int &);

}
