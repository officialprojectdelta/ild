#include <iostream>
#include <vector>

#include "lexer.hpp"

Lexer::Lexer() {
}

std::vector<Token> Lexer::Lex(std::string src) {
    char c;
    std::vector<Token> tokens;

    for (int i = 0; i < src.length(); i++) {
        c = src[i];
        switch (c) {
            case ' ':{
                break;
            } case '(':{
                tokens.push_back(Token(TokenType(T_LPAREN)));
                break;
            } case ')':{
                tokens.push_back(Token(TokenType(T_RPAREN)));
                break;
            } case ':':{
                tokens.push_back(Token(TokenType(T_COLON)));
                break;
            } case ';':{
                tokens.push_back(Token(TokenType(T_SEMICOLON)));
                break;
            } default:{
                break;
            }
        }
    }
}