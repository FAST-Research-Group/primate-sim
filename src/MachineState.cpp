#include "MachineState.hpp"
#include "consts.hpp"

MachineState::MachineState(uint64_t starting_addr) : registerFile(NUM_REGISTERS, 0), //num registers is the only const used?
                                                     memory(),
                                                     pc(starting_addr),
                                                     running(true)
{
}