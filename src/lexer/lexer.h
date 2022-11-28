#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <array>

enum class Operator 
{
    MOV,
    ADD,
    SUB,
    MUL,
    DIV,
    DEF,
    LABLEDEF, 
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

    Type() 
        : t_kind(TypeKind::NULLTP), size_of(0)
    {

    }

    Type(TypeKind t_kind, size_t size_of) 
        : t_kind(t_kind), size_of(size_of)
    {

    }

    bool operator==(const Type& type) const 
    {
        return this->t_kind == type.t_kind && this->size_of == type.size_of;
    }
};

template<>
struct std::hash<Type>
{
    std::size_t operator()(const Type& t) const noexcept
    {
        std::size_t h1 = std::hash<TypeKind>{}(t.t_kind);
        std::size_t h2 = std::hash<size_t>{}(t.size_of);
        return h1 ^ (h2 << 1);
    }
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
