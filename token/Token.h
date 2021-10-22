//
// Created by yuxin on 2021/9/27.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H
#include "iostream"

// consts
const std::string SKIP = "SKIP";

const std::string IDENFR = "IDENFR";
const std::string INTCON = "INTCON";
const std::string STRCON = "STRCON";
const std::string MAINTK = "MAINTK";
const std::string CONSTTK = "CONSTTK";
const std::string VOIDTK = "VOIDTK";
const std::string INTTK = "INTTK";
const std::string BREAKTK = "BREAKTK";
const std::string CONTINUETK = "CONTINUETK";
const std::string IFTK = "IFTK";
const std::string ELSETK = "ELSETK";
const std::string WHILETK = "WHILETK";
const std::string RETURNTK = "RETURNTK";
const std::string GETINTTK = "GETINTTK";
const std::string PRINTFTK = "PRINTFTK";
const std::string LBRACE = "LBRACE";
const std::string RBRACE = "RBRACE";
const std::string SEMICN = "SEMICN";
const std::string COMMA = "COMMA";
const std::string LPARENT = "LPARENT";
const std::string RPARENT = "RPARENT";
const std::string LBRACK = "LBRACK";
const std::string RBRACK = "RBRACK";
const std::string MOD = "MOD";
const std::string PLUS = "PLUS";
const std::string MINU = "MINU";
const std::string MULT = "MULT";
const std::string NEQ = "NEQ";
const std::string NOT = "NOT";
const std::string AND = "AND";
const std::string OR = "OR";
const std::string DIV = "DIV";
const std::string LSS = "LSS";
const std::string LEQ = "LEQ";
const std::string GRE = "GRE";
const std::string GEQ = "GEQ";
const std::string EQL = "EQL";
const std::string ASSIGN = "ASSIGN";


class Token {
public:
    std::string identity;
    std::string value;
    int line;
public:
    Token(std::string &name, const std::string &valueIn, int lineNum = -1);
    Token(std::string &name, int lineNum = -1);
    std::string toString();
    std::string getIdentity();
    std::string getKey();
    int getLine();
    ~Token();
};
#endif //COMPILER_TOKEN_H
