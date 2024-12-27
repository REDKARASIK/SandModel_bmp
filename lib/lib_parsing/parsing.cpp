#include "parsing.h"

void ReadLongArgs(char* arg, TerminalArgs* args) {
    char* arg_suffix;
    int equal_index = SearchEqualInChar(arg);
    arg_suffix = arg + 1;
    arg = arg - equal_index;
    arg[equal_index] = '\0';
    if (*arg_suffix == '\0') {
        RiseErrorIncorrectArgument();
    }
    if (strcmp(arg, "-i") == 0 || strcmp(arg, "--input") == 0) {
        args->input = arg_suffix;
    } else if (strcmp(arg, "-o") == 0 || strcmp(arg, "--ouput") == 0) {
        args->output_path = arg_suffix;
    } else if (strcmp(arg, "-m") == 0 || strcmp(arg, "--max-iter") == 0) {
        args->max_iter = ConvertStringToNumber(arg_suffix);
    } else if (strcmp(arg, "-f") == 0 || strcmp(arg, "--freq") == 0) {
        args->freq = ConvertStringToNumber(arg_suffix);
    } else {
         RiseErrorIncorrectArgument();
    }
    
}

TerminalArgs ReadArgs(int argc, char** argv) {
    TerminalArgs args;
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-i") == 0 || strcmp(argv[i], "--input") == 0) {
            args.input = argv[++i];
        } else if (strcmp(argv[i], "-o") == 0 || strcmp(argv[i], "--ouput") == 0) {
            args.output_path = argv[++i];
        } else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--max-iter") == 0) {
            args.max_iter = ConvertStringToNumber(argv[++i]);
        } else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--freq") == 0) {
            args.freq = ConvertStringToNumber(argv[++i]);
        } else {
            ReadLongArgs(argv[i], &args);
        }
    }
    return args;
}

void CheckFillingArgs(TerminalArgs* args) {
    if (args->input == nullptr || args->output_path == nullptr || args->max_iter == -1 || args->freq == -1) {
        RiseErrorNotFullArgs();
    }
}



