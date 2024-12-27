#include "help_functions.h"

int ConvertStringToNumber(const char* string) {
    const char* check_string = string;
    while (*string != '\0') {
        if (!isdigit(*string)) {
            RiseErrorIncorrectNumber();
        }
        ++string;
    }
    return std::stol(check_string);
}

int SearchEqualInChar(char*& arg) {
    int equal_index = 0;
    while (*arg != '\0') {
        if (*arg == '=') {
            return equal_index;
        }
        ++arg;
        ++equal_index;
    }
    return equal_index;
}

char* ReformatPath(char* output_path) {
    if (output_path[std::strlen(output_path) - 1] != '/') {
        size_t size_original_path = std::strlen(output_path);
        char correct_output_path[size_original_path];
        std::strcpy(correct_output_path, output_path);
        char output_path[size_original_path + 1];
        std::strcpy(output_path, correct_output_path);
        output_path[size_original_path] = '/';
        output_path[size_original_path + 1] = '\0';
    }
    return output_path;
}
