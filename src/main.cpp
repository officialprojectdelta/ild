#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "lexer/lexer.h"
#include "x86_64/codegen.h"

int main(int argc, char** argv) {
    std::cout << "Welcome to the ILD Compiler" << std::endl;
    std::cout << "Reading file..." << std::endl;

    std::ifstream inFile;
    inFile.open(argv[1]);
    std::stringstream bf;
    bf << inFile.rdbuf();

    std::string input = bf.str();

    std::cout << "Lexing..." << std::endl;

    Globals* lexed = lex(input);
    codegen(lexed);


    return 0;
}