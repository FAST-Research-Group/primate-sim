#ifndef BLUE_FUNCTIONAL_UNIT_HPP
#define BLUE_FUNCTIONAL_UNIT_HPP

#include "FunctionalUnit.hpp"

class BlueFunctionalUnit : public FunctionalUnit {
  public:
    BlueFunctionalUnit();
    virtual ~BlueFunctionalUnit();

    void processInstruction(Instruction &I, MachineState &MS) override;

  private:
    void handleBluematch(Instruction &I, MachineState &MS);
};

#endif // BLUE_FUNCTIONAL_UNIT_HPP