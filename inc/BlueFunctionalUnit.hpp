#ifndef BLUE_FUNCTIONAL_UNIT_HPP
#define BLUE_FUNCTIONAL_UNIT_HPP

#include "FunctionalUnit.hpp"
#include <fstream>

#define BFU_INPUT 11
#define BFU_OUTPUT 1

/**
 * @brief This class controls the code for the custom instructions in primate
 */
class BFU : public FunctionalUnit {
  std::ifstream  inputStream;
  public:
    BFU(bool, unsigned);
    virtual ~BFU();

    /**
     * @brief This handles the main process to run custom instructions
     * @param I The current instruction
     * @param MS The current state of the machine
     */
    void processInstruction(Instruction& I, MachineState &CMS, MachineState &NMS) override;

  private:
    void handleInput(Instruction& I, MachineState &CMS, MachineState &NMS);
    void handleInputRead(Instruction& I, MachineState &CMS, MachineState &NMS);
    void handleOutputEmiti(Instruction& I, MachineState &CMS, MachineState &NMS);

    /* Not implimented

    INPUT INSTRUCTIONS
      void handleInputExtracti(Instruction &I, MachineState &MS);
      void handleInputExtractiDone(Instruction &I, MachineState &MS);
      void handleInputSeek(Instruction &I, MachineState &MS);
      void handleInputDone(Instruction &I, MachineState &MS);

    OUTPUT INSTRUCTIONS
      void handleOutputMeta(Instruction &I, MachineState &MS);
      void handleOutputEmitiDone(Instruction &I, MachineState &MS);
      void handleOutputDone(Instruction &I, MachineState &MS);
    */
};

#endif // BLUE_FUNCTIONAL_UNIT_HPP