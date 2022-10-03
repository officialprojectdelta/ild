#include <iostream>
#include <vector>

#include "lexer.h"
#include "error.h" // A nice exception class with printf formatting

// Go through string that could start with whitespace and make sure it doesn't
// Pos is now pointing to next non whitespace character after pos
void clear_whitespace(

// Generates a string, returns if whitespace (' ', newline, or '\r') is found
std::string gen_str(const std::string& src, size_t& pos)
{
    size_t pos1 = pos;
    pos = src.find_first_of(" \n\r", pos);
    return str.substr(pos1, pos++ - pos1 - 1);
}

// MOVE THIS TO TYPE.CPP
// Generates a type from a string
Type gen_type(const std::string& src, size_t& pos)
{
    switch (gen_str) 
    {
        case "i32":
        {
            return {TokenType::INT, 4};
        }
        case "f32":
        {
            return {TokenType::FLOAT, 4};
        }
    }
}

// Generate all tokens in a text subsection (generates the function headers)
std::vector<FunList> gen_funs(Globals* current, const std::string& src, size_t& pos)
{
    // Loop through characters
    // If a new function is defined, push it back and continue
    // If a new variable is defined, do nothing 
    // For other instructions, add them as normal 
    // If a dot is reached, return the funlist
    
    // No guarantee of compiliation, because this was written without testing
    while (true)
    {
        switch (gen_str(src, pos))
        {
            case "global":
            {
               if (gen_str(src, pos) != "def") throw compiler_error("Invalid function decl);
               
               current->fun_list.push_back();
               current->fun_list.back().type = gen_type(src, pos);
               current->fun_list.back().name = gen_str(src, pos);
               if (src[pos] != '(') throw compiler_error("Invalid function decl, missing '('");
               pos++;                                                     
               if (src[pos] == ')') pos++;
               else 
               {
                   // Get args of function
               }  
               
               if (src[pos] != ":") throw compiler_error                                                     
            }
            default throw compiler_error("Invalid function decl");
        }
    }
}

Globals* lex(const std::string& src)
{
    Globals* current = new Globals;
    size_t pos = 0;
    while (pos < src.size())
    {
        if (src[pos] != '.') throw compiler_error("Unexpected character %c", src[pos]);
        pos++;
        switch (gen_str(src, pos))
        {
            case "text":
            {
               gen_text(current);
            }
        }
    }
}
