#include <stdio.h>
#include <FunctionalUnit.hpp>




class FunctionalUnitWrapper
{
private:
    std::vector<std::unique_ptr<FunctionalUnit>> functionalUnits;

public:
    // Constructor with parameters for number of units
    FunctionalUnitWrapper(int numBranchUnits, int numGreenUnits, int numMergedUnits)
    {
        // Add Branch functional units
        for (int i = 0; i < numBranchUnits; ++i)
        {
            functionalUnits.push_back(std::make_unique<BranchFunctionalUnit>()); // change this to green, branch, and blue
        }

        // Add Green functional units
        for (int i = 0; i < numGreenUnits; ++i)
        {
            functionalUnits.push_back(std::make_unique<GreenFunctionalUnit>());
        }

        // Add Merged functional units
        for (int i = 0; i < numMergedUnits; ++i)
        {
            functionalUnits.push_back(std::make_unique<MergedFunctionalUnit>());
        }
    }

    // Dispatch instructions to functional units in round-robin fashion
    void processInstructions(const std::vector<Instruction> &instructions)
    {
        int numUnits = functionalUnits.size();

        for (size_t i = 0; i < instructions.size(); ++i)
        {
            int functionalUnitIndex = i % numUnits;
            functionalUnits[functionalUnitIndex]->processInstruction(instructions[i].opcode);
        }
    }
};