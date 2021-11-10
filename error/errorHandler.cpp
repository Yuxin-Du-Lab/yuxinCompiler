//
// Created by yuxin on 2021/10/21.
//

#include <iostream>
#include "errorHandler.h"

std::ofstream writeErrorFile("error.txt");

void printError(const std::string& errorType, int line) {
    switch (errorType[0]) {
        case 'a':
            std::cout << "⾮法符号" << " line: " << line << std::endl;
            break;
        case 'b':
            std::cout << "名字重定义" << " line: " << line << std::endl;
            break;
        case 'c':
            std::cout << "未定义的名字" << " line: " << line << std::endl;
            break;
        case 'd':
            std::cout << "函数参数个数不匹配" << " line: " << line << std::endl;
            break;
        case 'e':
            std::cout << "函数参数类型不匹配" << " line: " << line << std::endl;
            break;
        case 'f':
            std::cout << "⽆返回值的函数存在不匹配的return语句" << " line: " << line << std::endl;
            break;
        case 'g':
            std::cout << "有返回值的函数缺少return语句" << " line: " << line << std::endl;
            break;
        case 'h':
            std::cout << "不能改变常量的值" << " line: " << line << std::endl;
            break;
        case 'i':
            std::cout << "缺少分号" << " line: " << line << std::endl;
            break;
        case 'j':
            std::cout << "缺少右⼩括号’)’" << " line: " << line << std::endl;
            break;
        case 'k':
            std::cout << "缺少右中括号’]’" << " line: " << line << std::endl;
            break;
        case 'l':
            std::cout << "printf中格式字符与表达式个数不匹配" << " line: " << line << std::endl;
            break;
        case 'm':
            std::cout << "在⾮循环块中使⽤break和continue语句" << " line: " << line << std::endl;
            break;
        default:
            break;
    }
    std::cout << ">编译中止" << std::endl;
    exit(-1);
}

void throwError(const std::string& errorType, int line) {
    writeErrorFile << std::to_string(line) << " " << errorType << std::endl;
    printError(errorType, line);
}