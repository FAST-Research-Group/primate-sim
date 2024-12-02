#include "../inc/threebitadd.hpp"
#include <iostream>

namespace threebitadd
{
    Register struct2Reg(const threebitadd &op)
    {
        return op.testVal;
    }

    threebitadd Reg2struct(const Register &result)
    {
        threebitadd op;
        op.testVal = result;
        return op;
    }

    threebitadd processthreebitadd(threebitadd &op)
    {
        int val = static_cast<int>(op.testVal);
        val += val;         // Perform the addition
        val = val & 0b0111; // Mask to 3 bits
        op.testVal = val;   // Update the testVal
        return op;
    }
}
