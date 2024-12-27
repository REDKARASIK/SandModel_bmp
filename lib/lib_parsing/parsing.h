#pragma once
#include "lib/lib_errors/errors.h"
#include "lib/lib_help_functions/help_functions.h"
#include <iostream>

struct TerminalArgs {
    const char* input = nullptr;
    char* output_path = nullptr;
    long max_iter = -1;
    long freq = -1;
};

TerminalArgs ReadArgs(int argc, char** argv);

void CheckFillingArgs(TerminalArgs* args);