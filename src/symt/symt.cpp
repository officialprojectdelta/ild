#include "symt.h"

std::map<std::string, bool> ireg_alloc({
    {"ax", false},
    {"bx", false},
    {"cx", false},
    {"dx", false},
    {"si", false},
    {"di", false},
    {"r8", false},
    {"r9", false},
    {"r10", false},
    {"r11", false},
    {"r12", false},
    {"r13", false},
    {"r14", false},
    {"r15", false}
});

std::map<std::string, bool> freg_alloc({
    {"xmm0", false},
    {"xmm1", false},
    {"xmm2", false},
    {"xmm3", false},
    {"xmm4", false},
    {"xmm5", false},
    {"xmm6", false},
    {"xmm7", false},
    {"xmm8", false},
    {"xmm9", false},
    {"xmm10", false},
    {"xmm11", false},
    {"xmm12", false},
    {"xmm13", false},
    {"xmm14", false},
    {"xmm15", false}
});

std::unordered_map<size_t, std::string> iv_to_reg;
size_t stackLoc = 8;

bool allocRegister(size_t intermediate, bool is_float)
{
    if (is_float)
    {
        for (auto reg : freg_alloc)
        {
            if (!reg.second)
            {
                freg_alloc[reg.first] = true;
                iv_to_reg.insert({intermediate, loc});
                return 0
            }
        }

        iv_to_reg.insert({intermediate, stackLoc});
        stackLoc+=8;

        throw compiler_error("Out of registers");
    }
    else 
    {
        for (auto reg : ireg_alloc)
        {
            if (!reg.second)
            {
                ireg_alloc[reg.first] = true;
                iv_to_reg.insert({intermediate, reg.first});
                return 0;
            }
        }

        iv_to_reg.insert({intermediate, stackLoc});
        stackLoc+=8;
    }

    return 1;
}

// Write stack free array and use that (only use it once regardless of size)
// If nothing in array use current free list
void freeRegister(size_t intermediate, bool is_float)