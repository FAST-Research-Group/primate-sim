#ifndef MATMUL_H
#define MATMUL_H
#include "../inc/Instruction.hpp"
#include "../inc/MachineState.hpp"

// this will need to be changed since I don't know what primate actually compiles
namespace MatMul
{
    typedef struct matrixes
    {
        float **weightsPTR;
        float **inputPTR;
        float **activationPTR;
        int m1r, m1c, m2r, m2c;
    } matrices;

    // Function declarations
    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS);
    void *getFunctionPTR();

    Register struct2Reg(const matrices &mat);
    matrices Reg2struct(const Register &packedData);
    float **createMatrix(int rows, int columns);
    void freeMatrix(float **matrix, int rows);

} // namespace MatMul

#endif