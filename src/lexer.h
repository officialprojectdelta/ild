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

struct Oprand
{
    OKind kind; // If it is memory, temporary, lable, constant
    Type type; // The type (i32, f32, etc., or none for lables)
    std::string value; // The actual oprand, such as 3 or &x0
};      

struct Instruction
{
    Operator op; // The operator, such as mov, add, cmp, or jmp
    Oprand dst; // The first oprand (the destination)
    Oprand src1; // The second oprand
    Oprand src2; // The third oprand
};

// Every function, and it's corresponding instruction list
struct FunList
{
    Type ret; // Return type
    std::vector<Oprand> args; // Args list
    std::string name; // Name of function
};

// Every function and defined globals (added later)
struct Globals 
{   
    std::vector<FunList> fun_list;
};

Globals lex(const std::string& src);