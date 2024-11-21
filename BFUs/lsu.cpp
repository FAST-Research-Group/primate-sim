#include "../inc/lsu.hpp"
#include <iostream>

namespace lsu
{

    Register struct2Reg(const outputs &out)
    {
        return out.answer;
    }

    outputs Reg2struct(const Register &packedData)
    {
        outputs out;
        out.answer = packedData;
    }

}