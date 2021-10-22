//
// Created by yuxin on 2021/10/21.
//

#include <iostream>
#include "errorHandler.h"

std::ofstream writeErrorFile("error.txt");

void printError(const std::string& errorType, int line) {
    switch (errorType[0]) {
        case 'a':
            break;
        case 'b':
            break;
        case 'c':
            break;
        case 'd':
            break;
        case 'e':
            break;
        case 'f':
            break;
        case 'g':
            break;
        case 'h':
            break;
        case 'i':
            break;
        case 'j':
            break;
        case 'k':
            break;
        case 'l':
            break;
        case 'm':
            break;
        default:
            break;
    }
}

void throwError(const std::string& errorType, int line) {
    writeErrorFile << std::to_string(line) << " " << errorType << std::endl;
    printError(errorType, line);
}