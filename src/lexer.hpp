#pragma once

#include <iostream>
#include <string>

enum TokenType {
    T_EOF,
    T_I32,
    T_I64,
    T_F32,
    T_F64,
    T_UI32,
    T_UI64,
    T_BOOL,
    T_LPAREN,
    T_RPAREN,
    T_COLON,
    T_SEMICOLON
};

class Token {
public:
    TokenType type;
    std::string value;
    Token(TokenType prtype, std::string prvalue = "")
        :type(prtype), value(prvalue) {};
private:
};

class Lexer {
public:
    Lexer();
    std::vector<Token> Lex(std::string src);
private:
};