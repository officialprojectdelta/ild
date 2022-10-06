#include <iostream>
#include <vector>
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
});

// Go through string that could start with whitespace and make sure it doesn't
// Pos is now pointing to next non whitespace character after pos
void clear_whitespace(const std::string& src, size_t& pos)
{
    if (src[pos] != ' ' && src[pos] != '\n' && src[pos] != '\r') return;
    pos = src.find_first_of(" \n\r", pos);
    pos++;
    return;
}

// Generates a string, returns if whitespace (' ', newline, or '\r') is found
std::string gen_str(const std::string& src, size_t& pos)
{
    clear_whitespace(src, pos);
    size_t pos1 = pos;
    pos = src.find_first_of(" \n\r", pos);
    return src.substr(pos1, pos++ - pos1 - 1);
}

// MOVE THIS TO TYPE.CPP
// Generates a type from a string
Type gen_type(const std::string& src, size_t& pos)
{
    return str_type.contains(gen_str(src, pos)) ? str_type[gen_str(src, pos)] : {TokenType::NULLTP};
}

// Generates 1 instruction
void gen_instruct(Func* current, const std::string& src, size_t& pos)
{
    Instruction instr;
    std::string check = gen_str(src, pos);
    if (check == "def") 
    {
        Type type = gen_type(src, pos);
        if (type.type == TypeKind::NULLTP) throw compiler_error("Expected type of declaration");
        
        instr.op = Operator::DEF;
        operands[0].type = type;
        operands[0].kind = OKind::MEMORY;
        operands[0].value = gen_str(src, pos).erase(0, 1);

        size_t pos1 = pos;
        type = gen_str(src, pos);
        if (type.type == TokenType::NULLTP) 
        {
            pos = pos1;
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
            if (okind_map.contains[check[0]])
            {
                instr.operands[1].kind = okind_map[check[0]];
                check.erase(0, 1);
                instr.oprands[1].value = check;
            }
            else // Must be numner
            {
                instr.operands[1].kind = OKind::NUMBER;
                instr.operand[1].value = check;
            }
        }
    }
    else if (operator_map.contains(check))
    {
        instr.operator = operator_map[check].second;
        for (size_t i = 0; i < operator_map[check].first; i++)
        {
            size_t pos1 = pos;
            Type type = gen_type(src, pos); 
            // Gen all subnodes
            if (type.type == TypeKind::NULLTP)
            {
                // Must be lable
                pos = pos1;
                instr.operands[i].kind == OKind::LABLE;
                instr.operands[i].value = gen_str(src, pos);
            }
            else
            {
                // Add type and check next for the value
                instr.operands[i].type = type;
                std::string check = gen_str(src, pos);

                std::unordered_map<char, OKind> okind_map({
                    {'&', OKind::MEMORY},
                    {'%', OKind::TEMP}
                });

                // Temporary or memory
                if (okind_map.contains[check[0]])
                {
                    instr.operands[i].kind = okind_map[check[0]];
                    check.erase(0, 1);
                    instr.oprands[i].value = check;
                }
                else // Must be numner
                {
                    instr.operands[i].kind = OKind::NUMBER;
                    instr.operand[i].value = check;
                }
            }
        }
    }
    else 
    {
        throw compiler_error("Invalid operator", check.c_str());
    }   

    current->fun_list.back().instruct_list.emplace_back(instr);

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
    while (true)
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

            while (true)
            {
                clear_whitespace(src, pos);
                size_t pos1 = pos;
                if (gen_str(src, pos) == "global")
                {
                    pos = pos1;
                    break;
                }

                gen_instruct(&current->fun_list.back(), src, pos);
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
        if (gen_str(src, pos) == "text")
        {
            gen_funs(current, src, pos);
        }
        else throw compiler_error("Invalid segment %s", gen_str(src, pos).c_str());
    }
}
