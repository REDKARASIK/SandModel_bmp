#include <iostream>
#include "lib/lib_parsing/parsing.h"
#include "lib/lib_sandmodel/sandmodel.h"

int main(int argc, char** argv) {
    TerminalArgs args;
    args = ReadArgs(argc, argv);
    CheckFillingArgs(&args);
    SandModel(args.input, args.output_path, args.max_iter, args.freq);
    return 0;
}
