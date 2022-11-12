#include "codegen.h"

#include <unordered_map>
#include <map>
#include <sstream>

#include "error.h"

std::unordered_map<std::string, size_t> var_map;
std::unordered_map<size_t, std::string> iv_to_reg;

std::unordered_map<Type, std::unordered_map<Type, std::string>> mov_table({
    {{TypeKind::INT, 4}, {
        {{TypeKind::INT, 4}, "movl"}
    }}
});

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

size_t stackLoc = 8;
std::string output; 

template <typename Ty>
void oprintf(std::string* write, Ty arg1)
{
    std::stringstream str;
    str << arg1;
    write->append(str.str());
    return;
}

template <typename Ty, typename... Args>
void oprintf(std::string* write, Ty arg1, Args... args)
{
    std::stringstream str;
    str << arg1;
    write->append(str.str());
    oprintf(write, args...);
    return;
}

bool bfromTypeKind(TypeKind t_kind)
{
    if (t_kind == TypeKind::FLOAT) return true;
    return false;
}

std::string allocRegister(size_t intermediate, bool is_float)
{
    if (is_float)
    {
        for (auto reg : freg_alloc)
        {
            if (!reg.second)
            {
                freg_alloc[reg.first] = true;
                iv_to_reg.insert({intermediate, reg.first});
                return reg.first;
            }
        }

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
                return reg.first;
            }
        }

        throw compiler_error("Out of registers");
    }
}

void freeRegister(size_t intermediate, bool is_float)
{
    if (is_float)
    {
        freg_alloc[iv_to_reg[intermediate]] = false;
    }
    else 
    {
        ireg_alloc[iv_to_reg[intermediate]] = false;
    }
    return;
}

void printRegister(std::string* write, size_t intermediate, size_t size, bool is_float, bool alloc)
{
    oprintf(write, "%");
    if (alloc) allocRegister(intermediate, is_float);
    else freeRegister(intermediate, is_float);

    if (is_float)
    {
        return;
    }
    else
    {
        switch (size)
        {
            case 1: 
            {
                if (iv_to_reg[intermediate][0] == 'r')
                {
                    oprintf(write, iv_to_reg[intermediate], 'b');
                }
                else 
                {
                    oprintf(write, iv_to_reg[intermediate][0], 'l');
                }
                break;
            }
            case 2:
            {
                if (iv_to_reg[intermediate][0] == 'r')
                {
                    oprintf(write, iv_to_reg[intermediate], 'w');
                }
                else 
                {
                    oprintf(write, iv_to_reg[intermediate]);
                }
                break;
            }
            case 4: 
            {
                if (iv_to_reg[intermediate][0] == 'r')
                {
                    oprintf(write, iv_to_reg[intermediate], 'd');
                }
                else 
                { 
                    oprintf(write, 'e', iv_to_reg[intermediate]);
                }
                break;
            }
            case 8:
            {
                if (iv_to_reg[intermediate][0] == 'r')
                {
                    oprintf(write, iv_to_reg[intermediate]);
                }
                else 
                { 
                    oprintf(write, 'r', iv_to_reg[intermediate]);
                }
                break;
            }

            return;
        }
    }
}

// Prints a memory location to the output string provided
// Only takes in stack locations for now
void fetchMemory(std::string* write, Operand op)
{
    oprintf(write, "-", var_map[op.value], "(%rbp)");
}

void cgfPrologue()
{
    oprintf(&output, "    pushq %rbp\n");
    oprintf(&output, "    movq %rsp, %rbp\n");
}

void cgfEpilogue()
{
    oprintf(&output, "    movq %rbp, %rsp\n");
    oprintf(&output, "    popq %rbp\n");
}

std::string codegen(Globals* src)
{
    for (size_t i = 0; i < src->fun_list.size(); i++)
    {
        oprintf(&output, ".global ", src->fun_list[i].name, "\n");
        oprintf(&output, src->fun_list[i].name, ":\n");

        cgfPrologue();


        for (size_t j = 0; j < src->fun_list[i].instruct_list.size(); j++)
        {
            auto operands = src->fun_list[i].instruct_list[j].operands;
            switch (src->fun_list[i].instruct_list[j].op)
            {
                case Operator::DEF:
                {
                    var_map.insert({operands[0].value, stackLoc});
                    stackLoc+=operands[0].type.size_of;
                    break;
                }
                case Operator::MOV:
                {
                    oprintf(&output, "    ", mov_table[operands[0].type][operands[1].type]);
                    
                    std::array<std::string, 2> strs;
                    for (long long k = 1; k >= 0; k--) 
                    {
                        switch (operands[k].kind)
                        {
                            case OKind::TEMP:
                            {
                                printRegister(&strs[k], std::stoull(operands[k].value), 
                                operands[k].type.size_of, 
                                bfromTypeKind(operands[k].type.t_kind), 
                                !k);
                                break;
                            }
                            case OKind::CONST:
                            {
                                oprintf(&strs[k], "$", operands[k].value);
                                break;
                            }
                            case OKind::MEMORY:
                            {
                                fetchMemory(&strs[k], operands[k]);
                            }
                        }
                    }
                    
                    oprintf(&output, " ", strs[1], ", ", strs[0], "\n");
                    break;
                }
                case Operator::ADD:
                {
                    // Check if output register and second register are same
                    // Make sure that they are
                    // Do add instruction
                    if (operands[0].kind == OKind::TEMP && operands[1].kind == OKind::TEMP && operands[0].value == operands[1].value) {}
                    else
                    {
                        oprintf(&output, "    ", mov_table[operands[0].type][operands[1].type]);
                    
                        std::array<std::string, 2> strs;
                        for (long long k = 1; k >= 0; k--) 
                        {
                            switch (operands[k].kind)
                            {
                                case OKind::TEMP:
                                {
                                    printRegister(&strs[k], std::stoull(operands[k].value), 
                                    operands[k].type.size_of, 
                                    bfromTypeKind(operands[k].type.t_kind), 
                                    !k);
                                    break;
                                }
                                case OKind::CONST:
                                {
                                    oprintf(&strs[k], "$", operands[k].value);
                                    break;
                                }
                                case OKind::MEMORY:
                                {
                                    fetchMemory(&strs[k], operands[k]);
                                }
                            }
                        }
                        
                        oprintf(&output, " ", strs[1], ", ", strs[0], "\n");
                    }

                    oprintf(&output, "    addl");

                    std::array<std::string, 2> strs;
                    for (long long k = 1; k >= 0; k--) 
                    {
                        switch (operands[k * 2].kind)
                        {
                            case OKind::TEMP:
                            {
                                printRegister(&strs[k], std::stoull(operands[k * 2].value), 
                                operands[k * 2].type.size_of, 
                                bfromTypeKind(operands[k].type.t_kind), 
                                !k);
                                break;
                            }
                            case OKind::CONST:
                            {
                                oprintf(&strs[k], "$", operands[k * 2].value);
                                break;
                            }
                            case OKind::MEMORY:
                            {
                                fetchMemory(&strs[k], operands[k * 2]);
                            }
                        }
                    }
                    oprintf(&output, " ", strs[1], ", ", strs[0], "\n");

                    break;   
                }
            }
        }
    }

    std::cout << output << std::endl;

    return std::string();
}