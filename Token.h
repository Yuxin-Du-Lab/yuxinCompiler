//
// Created by yuxin on 2021/9/27.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H
#include "iostream"
class Token {
public:
    std::string identity;
    std::string value;
public:
    Token(std::string name, std::string valueIn);
    Token(std::string name);
    std::string toString();
    void writeFile();
    ~Token();
};
#endif //COMPILER_TOKEN_H
