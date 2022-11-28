#include <iostream>
#include <vector>
#include <array>
#include <unordered_map>

#include "lexer.h"
#include "error.h" // A nice exception class with printf formatting

std::unordered_map<std::string, Type> str_type({
    {"i32", {TypeKind::INT, 4}},
    {"f32", {TypeKind::FLOAT, 4}},
});

std::unordered_map<std::string, std::pair<size_t /* Oprand count, so the generator knows how many oprands */, Operator /* Actual enum value */>> operator_map({
    {"mov", {2, Operator::MOV}},
    {"add", {3, Operator::ADD}},
    {"sub", {3, Operator::SUB}},
    {"mul", {3, Operator::MUL}},
    {"div", {3, Operator::DIV}},
    {"def", {2, Operator::DEF}},
    {"ret", {1, Operator::RET}},
    {"jmp", {1, Operator::JMP}},
});

// Go through string that could start with whitespace and make sure it doesn't
// Pos is now pointing to next non whitespace character after pos
void clear_whitespace(const std::string& src, size_t& pos)
{
    if (src[pos] != ' ' && src[pos] != '\n' && src[pos] != '\r' && src[pos] != ',') return;
    pos = src.find_first_not_of(" \n\r,", pos);
    if (pos == std::string::npos) pos = src.size();
    return;
}

// Generates a string, returns if whitespace (' ', newline, or '\r') is found
std::string gen_str(const std::string& src, size_t& pos)
{
    clear_whitespace(src, pos);
    size_t pos1 = pos;
    pos = src.find_first_of(" \n\r,", pos);
    if (pos == std::string::npos) pos = src.size();
    return src.substr(pos1, pos++ - pos1);
}

// MOVE THIS TO TYPE.CPP
// Generates a type from a string
Type gen_type(const std::string& src, size_t& pos)
{
    std::string check = gen_str(src, pos);
    return str_type.contains(check) ? str_type[check] : Type{TypeKind::NULLTP, 0};
}

// Generates 1 instruction
void gen_instruct(Func* current, const std::string& src, size_t& pos)
{
    Instruction instr;
    std::string check = gen_str(src, pos);
    if (check == "def") 
    {
        Type type = gen_type(src, pos);
        if (type.t_kind == TypeKind::NULLTP) throw compiler_error("Expected type of declaration");
        
        instr.op = Operator::DEF;
        instr.operands[0].type = type;
        instr.operands[0].kind = OKind::MEMORY;
        instr.operands[0].value = gen_str(src, pos).erase(0, 1);

        size_t pos1 = pos;
        type = gen_type(src, pos);
        if (type.t_kind == TypeKind::NULLTP) 
        {
            pos = pos1;
            instr.operands[1].type = {TypeKind::NULLTP, 0};
        }
        else
        {
            // Add type and check next for the value
            instr.operands[1].type = type;
            std::string check = gen_str(src, pos);

            std::unordered_map<char, OKind> okind_map({
                {'&', OKind::MEMORY},
                {'%', OKind::TEMP}
            });

            // Temporary or memory
            if (okind_map.contains(check[0]))
            {
                instr.operands[1].kind = okind_map[check[0]];
                check.erase(0, 2);
                instr.operands[1].value = check;
            }
            else // Must be number
            {
                instr.operands[1].kind = OKind::CONST;
                instr.operands[1].value = check;
            }
        }
    }
    else if (operator_map.contains(check))
    {
        instr.op = operator_map[check].second;
        for (size_t i = 0; i < operator_map[check].first; i++)
        {
            size_t pos1 = pos;
            Type type = gen_type(src, pos); 
            // Gen all subnodes
            if (type.t_kind == TypeKind::NULLTP)
            {
                // Must be lable
                pos = pos1;
                instr.operands[i].kind = OKind::LABLE;
                instr.operands[i].value = gen_str(src, pos);
            }
            else
            {
                // Add type and check next for the value
                instr.operands[i].type = type;
                std::string check = gen_str(src, pos);

                // Temporary 
                if (check[0] == '%')
                {
                    instr.operands[i].kind = OKind::TEMP;
                    check.erase(0, 2);
                    instr.operands[i].value = check;
                }
                else if (check[0] == '&')
                {
                    instr.operands[i].kind = OKind::MEMORY;
                    check.erase(0, 1);
                    instr.operands[i].value = check;
                }
                else // Must be number
                {
                    instr.operands[i].kind = OKind::CONST;
                    instr.operands[i].value = check;
                }
            }
        }
    }
    else if (check.back() == ':') 
    {
        instr.op = Operator::LABLEDEF;
        instr.opval = check;
    }
    else if (check[0] == 'j') 
    {
        instr.op = Operator::JMPC;
        instr.opval = check;

        for (size_t i = 0; i < 3; i++)
        {
            size_t pos1 = pos;
            Type type = gen_type(src, pos); 
            // Gen all subnodes
            if (type.t_kind == TypeKind::NULLTP)
            {
                // Must be lable
                pos = pos1;
                instr.operands[i].kind = OKind::LABLE;
                instr.operands[i].value = gen_str(src, pos);
            }
            else
            {
                // Add type and check next for the value
                instr.operands[i].type = type;
                std::string check = gen_str(src, pos);

                // Temporary 
                if (check[0] == '%')
                {
                    instr.operands[i].kind = OKind::TEMP;
                    check.erase(0, 2);
                    instr.operands[i].value = check;
                }
                else if (check[0] == '&')
                {
                    instr.operands[i].kind = OKind::MEMORY;
                    check.erase(0, 1);
                    instr.operands[i].value = check;
                }
                else // Must be number
                {
                    instr.operands[i].kind = OKind::CONST;
                    instr.operands[i].value = check;
                }
            }
        }
    }
    else if (check.substr(0, 3) == "set")
    {
        instr.op = Operator::SET;
        instr.opval = check;

        for (size_t i = 0; i < 3; i++)
        {
            size_t pos1 = pos;
            Type type = gen_type(src, pos); 

            // Add type and check next for the value
            instr.operands[i].type = type;
            std::string check = gen_str(src, pos);

            // Temporary 
            if (check[0] == '%')
            {
                instr.operands[i].kind = OKind::TEMP;
                check.erase(0, 2);
                instr.operands[i].value = check;
            }
            else if (check[0] == '&')
            {
                instr.operands[i].kind = OKind::MEMORY;
                check.erase(0, 1);
                instr.operands[i].value = check;
            }
            else // Must be number
            {
                instr.operands[i].kind = OKind::CONST;
                instr.operands[i].value = check;
            }
        }
    }
    else throw compiler_error("Invalid operator %s", check.c_str());

    current->instruct_list.emplace_back(instr);

    return;
}

// Generate all tokens in a text subsection (generates the function headers)
void gen_funs(Globals* current, const std::string& src, size_t& pos)
{
    // Loop through characters
    // If a new function is defined, push it back and continue
    // If a new variable is defined, do nothing 
    // For other instructions, add them as normal 
    // If a dot is reached, return the Func
    
    // No guarantee of compiliation, because this was written without testing
    while (pos < src.size())
    {
        if (gen_str(src, pos) == "global")
        {
            if (gen_str(src, pos) != "def") throw compiler_error("Invalid function decl");
               
            current->fun_list.emplace_back();
            current->fun_list.back().ret = gen_type(src, pos);
            current->fun_list.back().name = gen_str(src, pos);
            if (src[pos] != '(') throw compiler_error("Invalid function decl, missing '('");
            pos++;                                                     
            if (src[pos] == ')') pos++;
            else 
            {
               // Get args of function
               throw compiler_error("didnt expect args");
            }  
               
            if (src[pos] != ':') throw compiler_error("Expected proper end of function, missing ':'");   
            pos++;

            while (pos < src.size())
            {
                size_t pos1 = pos;
                if (gen_str(src, pos) == "global")
                {
                    pos = pos1;
                    break;
                }
                pos = pos1;

                gen_instruct(&current->fun_list.back(), src, pos);
                clear_whitespace(src, pos);
            }
        }
        else throw compiler_error("Invalid function decl"); 
    }

    return;
}

Globals* lex(const std::string& src)
{
    Globals* current = new Globals;
    size_t pos = 0;

    while (pos < src.size())
    {
        if (src[pos] != '.') throw compiler_error("Unexpected character %c", src[pos]);
        pos++;
        std::string check = gen_str(src, pos);
        if (check == "text")
        {
            gen_funs(current, src, pos);
        }
        else throw compiler_error("Invalid segment %s", check.c_str());
    }

    return current;
}
