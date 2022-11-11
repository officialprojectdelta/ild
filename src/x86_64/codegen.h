#pragma once

#include <string>

#include "lexer/lexer.h"

// Generates assembly output for an input il tree
std::string codegen(Globals* src);