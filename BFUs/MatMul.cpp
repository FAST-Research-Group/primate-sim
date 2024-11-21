#include "MatMul.hpp"
#include <iostream>

namespace MatMul
{

    // Return a function pointer to processInstruction
    // void (*getFunctionPTR())(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)
    // {
    //     return &processInstruction;
    // }

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

    matrices Reg2struct(const Register &packedData)
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

    float **createMatrix(int rows, int columns)
    {
        float **matrix = new float *[rows];
        for (int i = 0; i < rows; i++)
        {
            matrix[i] = new float[columns];
        }
        return matrix;
    }

    void freeMatrix(float **matrix, int rows)
    {
        for (int i = 0; i < rows; i++)
        {
            delete[] matrix[i];
        }
        delete[] matrix;
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

    // void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)
    // {
    //     bool regFile = (index < 0) ? 1 : 0;
    //     matrices result; // this should be where rs1 is extracted to
    //     if (regFile)
    //     {
    //         result = Reg2struct(CMS.getRegister(I.get_rs1()));
    //     }
    //     else
    //     {
    //         result = Reg2struct(CMS.getInterconnectValue(index));
    //     }

    //     matrixMultiply(result.weightsPTR, result.inputPTR, result.activationPTR, result.m1r, result.m1c, result.m2r, result.m2c);

    //     Register output = struct2Reg(result);

    //     if (regFile)
    //     {
    //         NMS.setRegister(I.get_rd(), output);
    //     }
    //     else
    //     {
    //         NMS.setInterconnectValue(index + 2, output);
    //     }
    // }
}
