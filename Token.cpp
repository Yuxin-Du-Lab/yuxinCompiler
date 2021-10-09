//
// Created by yuxin on 2021/9/27.
//


#include "Token.h"
Token::Token(std::string name, std::string valueIn) {
    identity = name;
    value = valueIn;
}

Token::Token(std::string name) {
    identity = name;
    value = "";
}

std::string Token::toString() {
    return identity + " " + value;
}

std::string Token::getIdentity() {
    return identity;
}

Token::~Token() {
//    std::cout << "destroy" << std::endl;
}