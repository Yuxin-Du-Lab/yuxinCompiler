//
// Created by yuxin on 2021/10/3.
//

#ifndef COMPILER_SYNTAXLIB_H
#define COMPILER_SYNTAXLIB_H

#include "iostream"
#include "vector"
#include "Token.h"

class FuncDef {

};

class FuncFParam {

};

class FuncFParams {

};

class FuncType {

};

class FuncRParams {

};

class Exp {

};

class UnaryOp {

};

class Number {

};

class PrimaryExp {

};

class UnaryExp {

};

class MulExp {

};

class AddExp {

};

class RelExp {

};

class EqExp {

};

class LAndExp {

};

class LOrExp {

};

class Cond {

};

class LVal {

};

class InitVal {

};

class ConstExp {

};

class VarDef {
//public:
//    Token *ident;
//    ConstExp constExp1, constExp2;
//    InitVal initVal;
//    int dimension;
//public:
//    VarDef(Token token);
};


class VarDecl {
public:
    std::vector<VarDef> varDefs;
};

class ConstInitVal {

};

class ConstDef {

};

class ConstDecl {

};


class Stmt {

};


class Block {

};

class MainFuncDef {

};

class CompUnit {
public:
    std::vector<FuncDef> funcDefs;
//    MainFuncDef
    Block block;
public:
};

#endif //COMPILER_SYNTAXLIB_H
