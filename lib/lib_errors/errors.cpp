#include "errors.h"

void RiseErrorIncorrectNumber() {
    throw std::runtime_error("Incorrect value for <int> type argument\n");
}

void RiseErrorIncorrectArgument() {
    throw std::runtime_error("Getting incorrect arguments\n");
}

void RiseErrorNotFullArgs() {
    throw std::runtime_error("Not all parameters has been given\n");
}

void RiseErrorFileIsNotOpen() {
    throw std::runtime_error("Cannot open input file by path, please check path of your input file.");
}

void RiseErrorPathIsNotExist() {
    throw std::runtime_error("Output path is not exist");
}