// THIS FILE IS A TEST OF WHAT I THINK A USER'S CODE SHOULD LOOK LIKE WHEN FEEDING IT INTO PRIMATE

#include <stdint.h>
#include "../inc/MatMul.hpp"

// These are from the virtual class
MatMul::MatMul(PrimateConfig cfg, bool reg, unsigned slot) : primateCFG(cfg), FunctionalUnit(reg, slot) {}

MatMul::~MatMul() {}

// These are for the actual Matrix Multiply

typedef struct matrixes
{
    float **weightsPTR;
    float **inputPTR;
    float **activationPTR;
    int m1r, m1c, m2r, m2c;
} matrices;

float **createMatrix(int rows, int columns) // we love potential memory leaks :)
{
    float **matrix = new float *[rows];
    for (int i = 0; i < rows; i++)
    {
        matrix[i] = new float[columns];
    }
    return matrix;
}

void freeMatrix(float **matrix, int rows) // prevent memory leaks
{
    for (int i = 0; i < rows; i++)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}

Register struct2Reg(const matrices &mat)
{
    // This might not work since pointers are of different type; however, this didn't give me red squiggles
    // Convert pointers to integers using uintptr_t
    uintptr_t weightsAddress = reinterpret_cast<uintptr_t>(mat.weightsPTR);
    uintptr_t inputAddress = reinterpret_cast<uintptr_t>(mat.inputPTR);
    uintptr_t activationAddress = reinterpret_cast<uintptr_t>(mat.activationPTR);

    Register result = 0;

    result |= Register(weightsAddress) << 192; // Assuming 64 bits for each pointer
    result |= Register(inputAddress) << 128;
    result |= Register(activationAddress) << 64;
    result |= Register(mat.m1r) << 48;
    result |= Register(mat.m1c) << 32;
    result |= Register(mat.m2r) << 16;
    result |= Register(mat.m2c);

    return result;
}

matrices reg2Struct(const Register &packedData)
{
    matrices mat;

    // Mask and shift to retrieve each part
    // Retrieve each pointer address (assuming 64 bits each for pointers)
    uintptr_t weightsAddress = static_cast<uintptr_t>((packedData >> 192) & Register(0xFFFFFFFFFFFFFFFF));
    uintptr_t inputAddress = static_cast<uintptr_t>((packedData >> 128) & Register(0xFFFFFFFFFFFFFFFF));
    uintptr_t activationAddress = static_cast<uintptr_t>((packedData >> 64) & Register(0xFFFFFFFFFFFFFFFF));

    // Retrieve each 16-bit integer
    mat.m1r = static_cast<int>((packedData >> 48) & Register(0xFFFF));
    mat.m1c = static_cast<int>((packedData >> 32) & Register(0xFFFF));
    mat.m2r = static_cast<int>((packedData >> 16) & Register(0xFFFF));
    mat.m2c = static_cast<int>(packedData & Register(0xFFFF));

    // Convert integer addresses back to pointers
    mat.weightsPTR = reinterpret_cast<float **>(weightsAddress);
    mat.inputPTR = reinterpret_cast<float **>(inputAddress);
    mat.activationPTR = reinterpret_cast<float **>(activationAddress);

    return mat;
}

void matrixMultiply(float **m1, float **m2, float **result, int m1r, int m1c, int m2r, int m2c)
{
    if (m1c != m2r)
    {
        // std::cout << "Matrix of invalid dimenstions" << std::endl;
        exit(0);
    }

    for (int i = 0; i < m1r; i++)
    {
        for (int j = 0; j < m2c; j++)
        {
            for (int k = 0; k < m1c; k++)
            {
                result[i][j] = 0;
            }
        }
    }

    // Matrix multiplication logic
    for (int i = 0; i < m1r; i++)
    {
        for (int j = 0; j < m2c; j++)
        {
            for (int k = 0; k < m1c; k++)
            {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
}

// this function actually needs data, right now, it's empty. I will probably define the variables into a different
// file since there are a lot of numbers

// this is just a multiply, no inference is done here, since the bias is not found, nor the neuron with the highest activation
void MatMul::processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS)
{

    matrices result = reg2Struct(CMS.getInterconnectValue(this->slotIdx - 2)); // this should be where rs1 is extracted to

    matrixMultiply(result.weightsPTR, result.inputPTR, result.activationPTR, result.m1r, result.m1c, result.m2r, result.m2c);

    Register output = struct2Reg(result);

    NMS.setInterconnectValue(this->slotIdx, output);
}