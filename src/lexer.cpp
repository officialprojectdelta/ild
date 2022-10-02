#include <iostream>
#include <vector>

#include "lexer.h"
#include "error.h" // A nice exception class with printf formatting

// Generate all variables in a text subsection
std::vector<FunList> gen_text(const std::string& src, size_t pos)
{

}

Globals lex(const std::string& src)
{
    size_t pos = 0;
    while (pos < src.size())
    {
        if (src[pos] != '.') throw compiler_error("Unexpected character %c", src[pos]);
    }
}