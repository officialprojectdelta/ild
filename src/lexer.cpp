#include <iostream>
#include <vector>

#include "lexer.h"
#include "error.h" // A nice exception class with printf formatting

// Generates a string, returns if whitespace (' ', newline, or '\r') is found
std::string gen_str(const std::string& src, size_t& pos)
{
    size_t pos1 = pos;
    pos = src.find_first_of(" \n\r", pos);
    return str.substr(pos1, pos++);
}

// Generate all tokens in a text subsection
std::vector<FunList> gen_text(const std::string& src, size_t& pos)
{
    // Loop through characters
    // If a new function is defined, push it back and continue
    // If a new variable is defined, do nothing 
    // For other instructions, add them as normal 
    // If a dot is reached, return the funlist
    
    // No guarantee of compiliation, because this was written without testing
    while (true)
    {
        
    }
}

Globals lex(const std::string& src)
{
    size_t pos = 0;
    while (pos < src.size())
    {
        if (src[pos] != '.') throw compiler_error("Unexpected character %c", src[pos]);
    }
}
