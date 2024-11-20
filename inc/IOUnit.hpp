#ifndef IOUNIT_HPP
#define IOUNIT_HPP

#include <fstream>
#include "Instruction.hpp"
#include "MachineState.hpp"

#define IO_UNIT_INPUT  11
#define IO_UNIT_OUTPUT 1

namespace IOUnit {
	// Input stream used by IOUnit functions
	extern std::ifstream inputStream;
	typedef struct IOinputs{
		
	} IOinputs;

	// Function to get the function pointer to processInstruction
	void (*getFunctionPTR())(Instruction  &I,
									 MachineState &CMS,
									 MachineState &NMS,
									 int			  &index);

	// Function declarations
	void processInstruction(Instruction	 &I,
									MachineState &CMS,
									MachineState &NMS,
									int			 &index);
	void handleInput(Instruction	&I,
						  MachineState &CMS,
						  MachineState &NMS,
						  int				&index);
	void handleInputRead(Instruction	 &I,
								MachineState &CMS,
								MachineState &NMS,
								int			 &index);
	void handleOutputEmiti(Instruction	&I,
								  MachineState &CMS,
								  MachineState &NMS,
								  int				&index);
	Register struct2Reg(const IOinputs &inp);
   IOinputs Reg2struct(const Register &packedData);
}	  // namespace IOUnit

#endif	 // IOUNIT_HPP
