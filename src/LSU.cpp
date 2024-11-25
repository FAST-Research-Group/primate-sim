#include "../inc/LSU.hpp"
namespace LSU
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
        {                                        // Store
            NMS.setMem(op.address, op.value, 8); // Adjust the size as needed
        }
        else
        {
            throw std::runtime_error("Invalid LSU operation code.");
        }
    }

    void processInstruction(Instruction &I, MachineState &CMS, MachineState &NMS, int &index)
    {
        bool regFile = (index < 0);
        MemOp memOp;

        // Extract the MemOp struct
        if (regFile)
        {
            memOp = Reg2struct(CMS.getRegister(I.get_rs1()));
        }
        else
        {
            memOp = Reg2struct(CMS.getInterconnectValue(index));
        }

        // Process the entire MemOp struct
        processMemOp(memOp, CMS, NMS);

        // Pack the updated struct back into a Register
        Register output = struct2Reg(memOp);

        // Write the output
        if (regFile)
        {
            NMS.setRegister(I.get_rd(), output);
        }
        else
        {
            NMS.setInterconnectValue(index + 2, output);
        }
    }

    void (*getFunctionPTR())(Instruction &, MachineState &, MachineState &, int &)
    {
        return &processInstruction;
    }

}