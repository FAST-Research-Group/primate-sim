#include "../inc/lsu.hpp"
#include <iostream>

namespace lsu
{

    Register struct2Reg(const MemOp &op)
    {
        Register result = 0;
        result |= Register(op.operation) << 128; // Operation in higher bits
        result |= Register(op.address) << 64;    // Address in middle bits
        result |= op.value;                      // Value in lower bits
        return result;
    }

    MemOp Reg2struct(const Register &packedData)
    {
        MemOp op;
        op.operation = static_cast<uint8_t>((packedData >> 128) & Register(0xFF));
        op.address = static_cast<uint64_t>((packedData >> 64) & Register(0xFFFFFFFFFFFFFFFF));
        op.value = packedData & Register(0xFFFFFFFFFFFFFFFF);
        return op;
    }

    void processMemOp(MemOp &op, MachineState &CMS, MachineState &NMS)
    {
        if (op.operation == 0)
        { // Load
            op.value = CMS.getMem(op.address);
        }
        else if (op.operation == 1)
        {                                     // Store
            NMS.setMem(op.address, op.value); // Adjust the size as needed
        }
        else
        {
            throw std::runtime_error("Invalid LSU operation code.");
        }
    }
}