#pragma once

#include <iostream>
#include <string>
#include <vector>

enum class Operator 
{
    MOV,
    ADD,
    SUB,
    MUL,
    DIV,
    DEF,
    RET
};

enum class TypeKind
{
    INT,
    FLOAT,
    NULLTP
};

struct Type 
{
    TypeKind t_kind;
    size_t size_of;
};

enum class OKind
{
    MEMORY,
    TEMP,
    LABLE,
    CONST,
};

struct Operand
{
    OKind kind; // If it is memory, temporary, lable, constant
    Type type; // The type (i32, f32, etc., or none for lables)
    std::string value; // The actual Operand, such as 3 or &x0
};      

struct Instruction
{
    Operator op; // The operator, such as mov, add, cmp, or jmp
    std::array<Operand, 3> operands; // So it can be indexed, holds 3 operands
};

// Every function, and it's corresponding instruction list
struct Func
{
    Type ret; // Return type
    std::vector<Operand> args; // Args list
    std::string name; // Name of function
    
    std::vector<Instruction> instruct_list; // List of instructions
};

// Every function and defined globals (added later)
struct Globals 
{   
    std::vector<Func> fun_list;
};

Globals* lex(const std::string& src);
