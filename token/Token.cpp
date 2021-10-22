//
// Created by yuxin on 2021/9/27.
//


#include "Token.h"

#include <utility>
Token::Token(std::string &name, const std::string &valueIn, int lineNum) {
    this->identity = std::move(name);
    this->value = valueIn;
    this->line = lineNum;
}

Token::Token(std::string &name, int lineNum) {
    this->identity = std::move(name);
    this->value = "";
    this->line = lineNum;
}

std::string Token::toString() {
    return this->identity + " " + this->value;
//    return this->identity + " " + this->value + " " + std::to_string(this->line);
}

std::string Token::getIdentity() {
    return identity;
}

std::string Token::getKey() {
    return value;
}

int Token::getLine() {
    return this->line;
}

Token::~Token() {
//    std::cout << "destroy" << std::endl;
}