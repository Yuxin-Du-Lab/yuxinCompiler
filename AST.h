//
// Created by yuxin on 2021/10/3.
//

#ifndef COMPILER_AST_H
#define COMPILER_AST_H

#include "iostream"
#include "vector"
#include "Token.h"

const int LVAL = 0;
const int EXP = 1;
const int BLOCK = 2;
const int IF = 3;
const int WHILE = 4;
const int CONTINUE_BREAK = 5;
const int RETURN = 6;
const int GETINT = 7;
const int PRINTF = 8;

class Stmt;

class Exp;

class FuncRParams;

class LVal;

class UnaryOp {
    Token *op;   // + - !
//    ! only can exist in cond
};

class Number {
    Token *intConst;
};

class PrimaryExp {
    int mode;
//    mode 1
    Exp *exp;
//    mode 2
    LVal *lVal;
//    mode 3
    Number *number;
};

class UnaryExp {
    int mode;
//    mode 1
    PrimaryExp *primaryExp;
//    mode 2
    Token *ident;
    FuncRParams *funcRParams;
//    mode 3
    UnaryOp *unaryOp;
    UnaryExp *unaryExp;
};

class MulOpTree {
    bool isLeaf;
    Token *op;
    MulOpTree *branch[2];
    UnaryExp *value;
};

class MulExp {
    MulOpTree *root;
};

class AddOpTree {
    bool isLeaf;
    Token *op;
    AddOpTree *branch[2];
    MulExp *value;
};

class AddExp {
    AddOpTree *root;
};

class RelOpTree {
    bool isLeaf;
    Token *op;
    RelOpTree *branch[2];
    AddExp *value;
};

class RelExp {
    RelOpTree *root;
};

class EqOpTree {
    bool isLeaf;
    Token *op;
    EqOpTree *branch[2];
    RelExp *value;
};

class EqExp {
    EqOpTree *root;
};

class LAndExp {
    std::vector<EqExp> eqExps;
};

class LOrExp {
    std::vector<LAndExp> lAndExps;
};

class Cond {
    LOrExp *lOrExp;
};

class LVal {
    Token *ident;
    int row;
    Exp *exps[2];
};

class Exp {
    AddExp *addExp;
};


class InitVal {
    Exp *exp;
    int row;
    std::vector<InitVal> initVals;
};

class ConstExp {
    AddExp *addExp;
//    ident used must be const HOW TO DO?
};

class VarDef {
    Token *ident;
    int row;
    ConstExp *constExps[2];
    bool hasInitVal;
    InitVal *initVal;
};


class VarDecl {
public:
    std::vector<VarDef> varDefs;
};

class ConstInitVal {
    ConstExp *constExp;
    int row;
    std::vector<ConstInitVal> constInitVals;
};

class ConstDef {
    Token *ident;
    int row;
    ConstExp *constExps[2];
};

class ConstDecl {
    std::vector<ConstDef> constDefs;
};

class Block {
    std::vector<ConstDecl> constDecls;
    std::vector<VarDecl> varDecls;
    std::vector<Stmt> stmts;
};

class Stmt {
    int mode;
    LVal *baseLVal;  // for LVal, getint
    Exp *baseExp;    // for LVal, Exp,
    Block *block;
    Cond *cond;
    bool hasElse;
    Stmt *ifStmt, *elseStmt;
    Token *continue_break;
    Token *formatString;
    std::vector<Exp> formatExps;    // for formatString exps
};

class FuncFParam {
    Token *ident;
    int row;
    ConstExp *constExp;
};

class FuncFParams {
    std::vector<FuncFParam> funcFParams;
};

class FuncType {
    Token *funcType; // void or int
};

class FuncRParams {
    std::vector<Exp> paramExps;
};

class FuncDef {
    FuncType *funcType;
    Token *ident;
    FuncFParams *funcFParams;
    Block *block;
};


class MainFuncDef {
    Block *block;
};

class CompUnit {
public:
    std::vector<ConstDecl> constDecls;
    std::vector<VarDecl> varDecls;
    std::vector<FuncDef> funcDefs;
    MainFuncDef *mainFuncDef;
public:
};

#endif //COMPILER_AST_H
