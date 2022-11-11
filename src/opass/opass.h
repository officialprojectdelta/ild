#pragma once

#include "lexer/lexer.h"

// Will contain all optimizations
struct OpassOpt
{
    bool opt;
};

// All optimization functions run while code is not assembly
void opassOne(Globals* src, OpassOpt opt);