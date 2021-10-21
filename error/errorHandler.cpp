//
// Created by yuxin on 2021/10/21.
//

#include <iostream>
#include "errorHandler.h"

std::ofstream writeErrorFile("error.txt");

void throwError(const std::string& errorType, int line) {
    writeErrorFile << std::to_string(line) << " " << errorType << std::endl;
}