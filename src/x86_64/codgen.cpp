#include "codegen.h"

#include <unordered_map>
#include <map>
#include <sstream>
#include <functional>

#include "error.h"

std::unordered_map<std::string, size_t> var_map;
std::unordered_map<size_t, std::string> iv_to_reg;

std::unordered_map<Type, std::unordered_map<Type, std::string>> mov_table({
    {{TypeKind::INT, 4}, {
        {{TypeKind::INT, 1}, "movsbl"},
        {{TypeKind::INT, 2}, "movswl"},
        {{TypeKind::INT, 4}, "movl"},
        {{TypeKind::INT, 8}, "mov"},
    }},
    {{TypeKind::INT, 8}, {
        {{TypeKind::INT, 1}, "movsbq"},
        {{TypeKind::INT, 2}, "movswq"},
        {{TypeKind::INT, 4}, "movslq"},
        {{TypeKind::INT, 8}, "movq"},
    }},
});

std::string get_movop(Type dst, Type src, bool src_isconst)
{
    // HACK 
    if (src_isconst) return mov_table[dst][dst];
    return mov_table[dst][src];
}

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

// Gets the AT&T prefix of a size
std::unordered_map<size_t, char> size_toc({
    {1, 'b'},
    {2, 'w'},
    {4, 'l'},
    {8, 'q'},
});

// CMP inverse map
std::unordered_map<std::string, std::string> cmp_invrs({
    {"e", "e"},
    {"ne", "ne"},
    {"l", "ge"},
    {"le", "g"},
    {"g", "le"},
    {"ge", "l"},
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

std::string allocRegister(bool is_float)
{
    if (is_float)
    {
        for (auto reg : freg_alloc)
        {
            if (!reg.second)
            {
                freg_alloc[reg.first] = true;
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
                return reg.first;
            }
        }

        throw compiler_error("Out of registers");
    }
}

void freeRegister(std::string reg, bool is_float)
{
    if (is_float)
    {
        freg_alloc[reg] = false;
    }
    else 
    {
        ireg_alloc[reg] = false;
    }
}

std::string allocRegister(size_t intermediate, bool is_float)
{
    iv_to_reg.insert({intermediate, allocRegister(is_float)});
    return iv_to_reg[intermediate];
}

void freeRegister(size_t intermediate, bool is_float)
{
    freeRegister(iv_to_reg[intermediate], is_float);
}

void printRegister(std::string* write, std::string base_reg, size_t size, bool is_float)
{   
    oprintf(write, "%");
    
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
                if (base_reg[0] == 'r')
                {
                    oprintf(write, base_reg, 'b');
                }
                else 
                {
                    oprintf(write, base_reg[0], 'l');
                }
                break;
            }
            case 2:
            {
                if (base_reg[0] == 'r')
                {
                    oprintf(write, base_reg, 'w');
                }
                else 
                {
                    oprintf(write, base_reg);
                }
                break;
            }
            case 4: 
            {
                if (base_reg[0] == 'r')
                {
                    oprintf(write, base_reg, 'd');
                }
                else 
                { 
                    oprintf(write, 'e', base_reg);
                }
                break;
            }
            case 8:
            {
                if (base_reg[0] == 'r')
                {
                    oprintf(write, base_reg);
                }
                else 
                { 
                    oprintf(write, 'r', base_reg);
                }
                break;
            }
        }
    }
}

void printRegister(std::string* write, size_t intermediate, size_t size, bool is_float, bool alloc)
{
    if (alloc) allocRegister(intermediate, is_float);
    else freeRegister(intermediate, is_float);
    
    printRegister(write, iv_to_reg[intermediate], size, is_float);

    return;
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

// Emits a move instruction moving the second operand to the first operand
void doMove(std::string* write, Operand dst, Operand src)
{
    oprintf(write, "    ", get_movop(dst.type/* DST */, src.type /* SRC */, src.kind == OKind::CONST ? 1 : 0));
                    
    std::array<std::string, 2> strs;
    for (char k = 1; k >= 0; k--) 
    {
        Operand cur = k ? src : dst;
        switch (cur.kind)
        {
            case OKind::TEMP:
            {
                printRegister(&strs[k], std::stoull(cur.value), 
                cur.type.size_of, 
                bfromTypeKind(cur.type.t_kind), 
                !k);
                break;
            }
            case OKind::CONST:
            {
                oprintf(&strs[k], "$", cur.value);
                break;
            }
            case OKind::MEMORY:
            {
                fetchMemory(&strs[k], cur);
                break;
            }
        }
    }
    
    oprintf(write, " ", strs[1], ", ", strs[0], "\n");
}

// Emits a comparison operation for 2 operands
bool doCmp(Operand dst, Operand src)
{
    // A lambda for almost every case (except for 2 memory and 2 const)
    auto up1_highest = [dst, src]() -> bool
    {
        // Check size of operands 
        // If they are the same do the cmp
        // If they are different, allocate a register and move the smaller one to the size of the bigger one
        // Compare the two operands
        // For non registers, the order of operands may have to be fliped, returning true on the bool
        // WHEN THERE ARE CONSTANTS THAT ARNT 32 BIT, THEY NEVER NEED TO BE UPSCALED, ONLY OTHER THINGS UPSCALED TO THEM

        bool rval = false;

        if (dst.type.size_of == src.type.size_of)
        {
            oprintf(&output, "    cmp", size_toc[dst.type.size_of], " ");
            
            std::array<std::string, 2> strs;
            for (char k = 1; k >= 0; k--) 
            {
                Operand cur;
                if (dst.kind == OKind::CONST) 
                {
                    cur = k ? dst : src;
                    rval = true;
                }
                else cur = k ? src : dst;
                switch (cur.kind)
                {
                    case OKind::TEMP:
                    {
                        printRegister(&strs[k], std::stoull(cur.value), 
                        cur.type.size_of, 
                        bfromTypeKind(cur.type.t_kind), 
                        !k);
                        break;
                    }
                    case OKind::CONST:
                    {
                        oprintf(&strs[k], "$", cur.value);
                        break;
                    }
                    case OKind::MEMORY:
                    {
                        fetchMemory(&strs[k], cur);
                        break;
                    }
                }
            }
            
            oprintf(&output, strs[1], ", ", strs[0], "\n");
        }
        else 
        {
            Operand smaller = dst.type.size_of < src.type.size_of ? dst : src;
            Operand bigger = dst.type.size_of < src.type.size_of ? src : dst;

            std::string regSave = allocRegister(false);
            oprintf(&output, "    ", get_movop(bigger.type/* DST */, smaller.type /* SRC */, smaller.kind == OKind::CONST ? 1 : 0));
            switch (smaller.kind)
            {
                case OKind::TEMP:
                {
                    printRegister(&output, std::stoull(smaller.value),
                    smaller.type.size_of, 
                    bfromTypeKind(smaller.type.t_kind),
                    false);
                    break;
                }
                case OKind::CONST:
                {
                    oprintf(&output, "$", smaller.value);
                    break;
                }
                case OKind::MEMORY:
                {
                    fetchMemory(&output, smaller);
                    break;
                }
            }

            oprintf(&output, ", ");
            printRegister(&output, regSave, bigger.type.size_of, false);
            oprintf(&output, "\n");

            freeRegister(regSave, false);

            std::string biggerStr; // The output for the bigger string
            switch (bigger.kind)
            {
                case OKind::TEMP:
                {
                    printRegister(&biggerStr, std::stoull(bigger.value),
                    bigger.type.size_of, 
                    bfromTypeKind(bigger.type.t_kind), 
                    false);
                    break;
                }
                case OKind::CONST:
                {
                    oprintf(&biggerStr, "$", bigger.value);
                    break;
                }
                case OKind::MEMORY:
                {
                    fetchMemory(&biggerStr, bigger);
                    break;
                }
            }

            if (dst.type.size_of < src.type.size_of)
            {
                // Bigger than smaller
                oprintf(&output, "    cmp", size_toc[bigger.type.size_of], " ", biggerStr, ", ");
                printRegister(&output, regSave, bigger.type.size_of, false);
                oprintf(&output, "\n");
            }
            else 
            {
                // Smaller than bigger
                if (dst.kind == OKind::CONST)
                {
                    oprintf(&output, "    cmp", size_toc[bigger.type.size_of], " ", biggerStr, ", ");
                    printRegister(&output, regSave, bigger.type.size_of, false);
                    oprintf(&output, "\n");
                    rval = true;
                }
                oprintf(&output, "    cmp", size_toc[bigger.type.size_of], " ");
                printRegister(&output, regSave, bigger.type.size_of, false);
                oprintf(&output, ", ", biggerStr, "\n");
            }
        }

        return rval;
    };

    // Lambda for the const const case
    auto const2 = [dst, src]() -> bool
    {
        bool rval = false;

        if (dst.type.size_of == src.type.size_of)
        {
            std::string regSave = allocRegister(false);
            oprintf(&output, "    ", get_movop(dst.type/* DST */, dst.type /* SRC */, src.kind == OKind::CONST ? 1 : 0));
            printRegister(&output, regSave, dst.type.size_of, false);
            oprintf(&output, "\n");
            freeRegister(regSave, false);

            oprintf(&output, "    cmp", size_toc[dst.type.size_of], " $", src.value, ", ");
            printRegister(&output, regSave, dst.type.size_of, false);
            oprintf(&output, "\n");
        }
        else 
        {
            Operand smaller = dst.type.size_of < src.type.size_of ? dst : src;
            Operand bigger = dst.type.size_of < src.type.size_of ? src : dst;

            std::string regSave = allocRegister(false);
            oprintf(&output, "    ", get_movop(bigger.type/* DST */, smaller.type /* SRC */, smaller.kind == OKind::CONST ? 1 : 0));
            printRegister(&output, regSave, bigger.type.size_of, false);
            oprintf(&output, "\n");
            freeRegister(regSave, false);

            oprintf(&output, "    cmp", size_toc[bigger.type.size_of], " $", bigger.value, ", ");
            printRegister(&output, regSave, bigger.type.size_of, false);
            oprintf(&output, "\n");

            if (dst.type.size_of < src.type.size_of) rval = true;
        }

        return rval;
    };

    // Lambda for the mem mem case
    auto mem2 = [dst, src]() -> bool
    {
        bool rval = false;
        if (dst.type.size_of == src.type.size_of)
        {
            std::string regSave = allocRegister(false);
            oprintf(&output, "    ", mov_table[dst.type][dst.type], " ");
            fetchMemory(&output, dst);
            oprintf(&output, ", ");
            printRegister(&output, regSave, dst.type.size_of, false);
            oprintf(&output, "\n");
            freeRegister(regSave, false);

            oprintf(&output, "    cmp", size_toc[dst.type.size_of], " ");
            fetchMemory(&output, src);
            oprintf(&output, ", ");
            printRegister(&output, regSave, dst.type.size_of, false);
            oprintf(&output, "\n");
        }
        else 
        {
            Operand smaller = dst.type.size_of < src.type.size_of ? dst : src;
            Operand bigger = dst.type.size_of < src.type.size_of ? src : dst;

            std::string regSave = allocRegister(false);
            oprintf(&output, "    ", mov_table[bigger.type][smaller.type], " ");
            fetchMemory(&output, smaller);
            oprintf(&output, ", ");
            printRegister(&output, regSave, bigger.type.size_of, false);
            oprintf(&output, "\n");
            freeRegister(regSave, false);

            std::string dstStr, srcStr;
            if (dst.type.size_of < src.type.size_of)
            {
                printRegister(&dstStr, regSave, bigger.type.size_of, false);
                fetchMemory(&srcStr, bigger);
            }
            else 
            {
                printRegister(&srcStr, regSave, bigger.type.size_of, false);
                fetchMemory(&dstStr, bigger);
            }

            oprintf(&output, "    cmp", size_toc[bigger.type.size_of], " ", srcStr, ", ", dstStr, "\n");
        }

        return false;
    };
    
    std::unordered_map<OKind, std::unordered_map<OKind, std::function<bool()>>> ops_tolambda({
        {OKind::TEMP, {
            {OKind::TEMP, up1_highest},
            {OKind::CONST, up1_highest},
            {OKind::MEMORY, up1_highest},
        }},
        {OKind::MEMORY, {
            {OKind::TEMP, up1_highest},
            {OKind::CONST, up1_highest},
            {OKind::MEMORY, mem2},
        }},
        {OKind::CONST, {
            {OKind::TEMP, up1_highest},
            {OKind::CONST, const2},
            {OKind::MEMORY, up1_highest},
        }},
    });

    return ops_tolambda[dst.kind][src.kind]();
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

                    if (operands[1].type.t_kind != TypeKind::NULLTP) 
                    {
                        doMove(&output, operands[0], operands[1]);
                    }
                    break;
                }
                case Operator::MOV:
                {
                    doMove(&output, operands[0], operands[1]);
                    break;
                }
                case Operator::ADD:
                {
                    // Check if output register and second register are same
                    // Make sure that they are
                    // Do add instruction
                    if (operands[0].kind == OKind::TEMP && operands[1].kind == OKind::TEMP && operands[0].value == operands[1].value) {}
                    else doMove(&output, operands[0], operands[1]);

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
                case Operator::SUB:
                {
                    if (operands[0].kind == OKind::TEMP && operands[1].kind == OKind::TEMP && operands[0].value == operands[1].value) {}
                    else doMove(&output, operands[0], operands[1]);

                    oprintf(&output, "    subl");

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
                case Operator::MUL:
                {
                    if (operands[0].kind == OKind::TEMP && operands[1].kind == OKind::TEMP && operands[0].value == operands[1].value) {}
                    else doMove(&output, operands[0], operands[1]);

                    oprintf(&output, "    imul");

                    bool threeops = false;

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
                                threeops = true;
                                break;
                            }
                            case OKind::MEMORY:
                            {
                                fetchMemory(&strs[k], operands[k * 2]);
                                break;
                            }
                        } 
                    }

                    if (threeops)
                    {
                        oprintf(&output, " ", strs[1], ", ", strs[0], ", ", strs[0], "\n");
                    }
                    else 
                    {
                        oprintf(&output, " ", strs[1], ", ", strs[0], "\n");
                    }
                    break; 
                }
                case Operator::DIV:
                {
                    // Check if rax or rdx are used
                    // Allocate those to registers
                    // Put values in registers
                    // Call cdq
                    // Output div instruction 
                    // Free used registers

                    std::string freeAx = "";
                    std::string freeDx = "";

                    if (ireg_alloc["ax"])
                    {
                        freeAx = allocRegister(false);
                        if (freeAx[0] == 'r') oprintf(&output, "    movq %rax, %", freeAx, "\n");
                        else oprintf(&output, "    movq %rax, %r", freeAx, "\n");
                    }

                    if (ireg_alloc["dx"])
                    {
                        freeDx = allocRegister(false);
                        if (freeDx[0] == 'r') oprintf(&output, "    movq %rdx, %", freeDx, "\n");
                        else oprintf(&output, "    movq %rdx, %r", freeDx, "\n");
                    }

                    // Move operand[1] to %rax 
                    switch (operands[1].kind)
                    {
                        case OKind::TEMP: 
                        {
                            oprintf(&output, "    ", get_movop(Type{TypeKind::INT, 8}, operands[1].type, operands[1].kind == OKind::CONST ? 1 : 0));
                            printRegister(&output, std::stoull(operands[1].value), 
                            operands[1].type.size_of,
                            bfromTypeKind(operands[1].type.t_kind),
                            false);
                            oprintf(&output, ", %rax\n");
                            break;
                        }
                        case OKind::CONST:
                        {
                            oprintf(&output, "    movq $", operands[1].value, ", %rax\n");
                            break;
                        }
                        case OKind::MEMORY:
                        {
                            oprintf(&output, "     ", mov_table[Type{TypeKind::INT, 8}][operands[1].type]);
                            fetchMemory(&output, operands[1]);
                            oprintf(&output,", %rax\n");
                            break;
                        }                    
                    }

                    oprintf(&output, "    cqo\n");
                    
                    std::string opasm; // Used because a constant has to be moved into a register
                    std::string freeConst = ""; // Used because the register the constant is moved into has to be freed 

                    switch (operands[2].kind)
                    {
                        case OKind::TEMP:
                        {
                            printRegister(&opasm, std::stoull(operands[2].value), 
                            operands[2].type.size_of, 
                            bfromTypeKind(operands[2].type.t_kind), 
                            false);
                            break;
                        }
                        case OKind::CONST:
                        {
                            freeConst = allocRegister(false);
                            if (freeConst[0] == 'r') 
                            {
                                oprintf(&output, "    movq $", operands[2].value, ", %", freeConst, "\n");
                                oprintf(&opasm, "%", freeConst);
                            }
                            else 
                            {
                                oprintf(&output, "    movq $", operands[2].value, ", %r", freeConst, "\n");
                                oprintf(&opasm, "%r", freeConst);
                            }
                            break;
                        }
                        case OKind::MEMORY:
                        {
                            fetchMemory(&opasm, operands[2]);
                            break;
                        }
                    } 

                    oprintf(&output, "    idivl ", opasm, "\n");

                    // Save this, because if ax/dx is free than we don't want output the data stored in its save location put back into it (it could be reallocated)
                    bool freeAx_flag = ireg_alloc["ax"];
                    bool freeDx_flag = ireg_alloc["dx"];

                    // Alloc register if needed 
                    switch (operands[0].kind)
                    {
                        case OKind::TEMP:
                        {
                            std::string regSave;
                            printRegister(&regSave, std::stoull(operands[0].value),
                            operands[0].type.size_of, 
                            bfromTypeKind(operands[0].type.t_kind),
                            true);
                            if (iv_to_reg[std::stoull(operands[0].value)] == "ax") break;
                            oprintf(&output, "    movq %rax, ", regSave, "\n");
                            break;
                        }
                        case OKind::MEMORY:
                        {
                            oprintf(&output, "    ", mov_table[operands[0].type][operands[0].type], " ");
                            printRegister(&output, "ax", operands[0].type.size_of, false);
                            oprintf(&output, ", ");
                            fetchMemory(&output, operands[0]);
                            oprintf(&output, "\n");
                            break;
                        }
                    }

                    if (freeAx != "") freeRegister(freeAx, false);
                    if (freeDx != "") freeRegister(freeDx, false);
                    if (freeConst != "") freeRegister(freeConst, false);

                    // Move back into ax/dx

                    if (freeAx != "" && freeAx_flag)
                    {
                        if (freeAx[0] == 'r') oprintf(&output, "    movq %", freeAx, ", %rax\n");
                        else oprintf(&output, "    movq %r", freeAx, ", %rax\n");
                    }
                    if (freeDx != "" && freeDx_flag)
                    {
                        if (freeDx[0] == 'r') oprintf(&output, "    movq %", freeDx, ", %rdx\n");
                        else oprintf(&output, "    movq %r", freeDx, ", %rdx\n");
                    }

                    break;
                } 
                case Operator::RET:
                {
                    switch (operands[0].kind)
                    {
                        case OKind::TEMP:
                        {
                            if (iv_to_reg[std::stoull(operands[0].value)] == "ax") break;

                            oprintf(&output, "    movq ");
                            printRegister(&output, std::stoull(operands[0].value), 
                            operands[0].type.size_of, 
                            bfromTypeKind(operands[0].type.t_kind), 
                            false);

                            oprintf(&output, ", %rax\n");
                            break;
                        }
                        case OKind::CONST:
                        {
                            oprintf(&output, "    ", get_movop(Type{TypeKind::INT, 8}, operands[0].type, 1), " $", operands[0].value, ", %rax\n");
                            break;
                        }
                        case OKind::MEMORY:
                        {
                            oprintf(&output, "    ", mov_table[Type{TypeKind::INT, 8}][operands[0].type], " ");
                            fetchMemory(&output, operands[0]);
                            oprintf(&output, ", %rax\n");
                            break;
                        }
                    }

                    cgfEpilogue();
                    oprintf(&output, "    ret\n");
                    break;
                }
                case Operator::LABLEDEF:
                {
                    oprintf(&output, src->fun_list[i].instruct_list[j].opval, "\n");
                    break;
                }
                case Operator::JMP:
                {
                    oprintf(&output, "    jmp ", operands[0].value, "\n");
                    break;
                }
                case Operator::JMPC:
                {
                    if (doCmp(operands[1], operands[2])) oprintf(&output, "j", cmp_invrs[src->fun_list[i].instruct_list[j].opval], " ", operands[0].value, "\n");
                    else oprintf(&output, "    j", src->fun_list[i].instruct_list[j].opval, " ", operands[0].value, "\n");
                    break;
                }
                case Operator::SET:
                {
                    if (doCmp(operands[1], operands[2])) oprintf(&output, "set", cmp_invrs[src->fun_list[i].instruct_list[j].opval], " ");
                    else oprintf(&output, "    set", src->fun_list[i].instruct_list[j].opval, " ");

                    printRegister(&output, std::stoull(operands[0].value), 
                    1, false, true);
                    output.push_back('\n');
                    break;
                }
            }
        }
    }

    return output;
}