#include <iostream>
#include <vector>

#include "lexer.h"
#include "error.h" // A nice exception class with printf formatting

// Generate all tokens in a text subsection
std::vector<FunList> gen_text(const std::string& src, size_t pos)
{
    // Loop through characters
    // If a new function is defined, push it back and continue
    // If a new variable is added, do nothing
    // F
    // Actually, 
    // If a dot is reached, return the funlist
}

Globals lex(const std::string& src)
{
    size_t pos = 0;
    while (pos < src.size())
    {
        if (src[pos] != '.') throw compiler_error("Unexpected character %c", src[pos]);
    }
}
