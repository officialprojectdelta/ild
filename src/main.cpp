#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "lexer/lexer.h"
#include "x86_64/codegen.h"

int main(int argc, char** argv) 
{
    // Input data from file
    std::ifstream in_file;
    in_file.open(argv[1]);
    std::stringstream bf;
    bf << in_file.rdbuf();
    in_file.close();
    std::string input = bf.str();

    // Lex step
    Globals* lexed = lex(input);

    // Output data/codegeneration
    std::ofstream out_file;
    out_file.open(argv[2]);
    out_file << codegen(lexed);
    out_file.close();
}