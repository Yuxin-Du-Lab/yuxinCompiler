//
// Created by yuxin on 2021/9/27.
//


#include "Token.h"

#include <utility>
Token::Token(std::string &name, const std::string &valueIn) {
    this->identity = std::move(name);
    this->value = valueIn;

}

Token::Token(std::string &name) {
    this->identity = std::move(name);
    this->value = "";
}

std::string Token::toString() {
    return this->identity + " " + this->value;
}

std::string Token::getIdentity() {
    return identity;
}

Token::~Token() {
//    std::cout << "destroy" << std::endl;
}