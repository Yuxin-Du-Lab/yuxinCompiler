//
// Created by yuxin on 2021/10/3.
//

#ifndef COMPILER_SYNTAX_H
#define COMPILER_SYNTAX_H

#include "Token.h"
#include "vector"
#include "AST.h"


void syntaxAnalysis(std::vector<Token> wordList);

ConstExp *getConstExp(std::vector<Token> wordList, int *pointer);

AddExp *getAddExp(std::vector<Token> wordList, int *pointer, bool scanning = false);

CompUnit *getCompUnit(std::vector<Token> wordList, int *pointer);

MainFuncDef *getMainFuncDef(std::vector<Token> wordList, int *pointer);

Block *getBlock(std::vector<Token> wordList, int *pointer);

ConstDecl *getConstDecl(std::vector<Token> wordList, int *pointer);

ConstDef *getConstDef(std::vector<Token> wordList, int *pointer);

ConstInitVal *getConstInitVal(std::vector<Token> wordList, int *pointer);

VarDecl *getVarDecl(std::vector<Token> wordList, int *pointer);

Stmt *getStmt(std::vector<Token> wordList, int *pointer);

LVal *getLVal(std::vector<Token> wordList, int *pointer, bool scanning = false);

Cond *getCond(std::vector<Token> wordList, int *pointer);

LOrExp *getLOrExp(std::vector<Token> wordList, int *pointer);

LAndExp *getLAndExp(std::vector<Token> wordList, int *pointer);

EqExp *getEqExp(std::vector<Token> wordList, int *pointer);

RelExp *getRelExp(std::vector<Token> wordList, int *pointer);


MulExp *getMulExp(std::vector<Token> wordList, int *pointer, bool scanning = false);

UnaryExp *getUnaryExp(std::vector<Token> wordList, int *pointer, bool scanning = false);

PrimaryExp *getPrimaryExp(std::vector<Token> wordList, int *pointer, bool scanning = false);

Number *getNumber(std::vector<Token> wordList, int *pointer, bool scanning = false);

VarDef *getVarDef(std::vector<Token> wordList, int *pointer);

InitVal *getInitVal(std::vector<Token> wordList, int *pointer);

UnaryOp *getUnaryOp(std::vector<Token> wordList, int *pointer, bool scanning = false);

Exp *getExp(std::vector<Token> wordList, int *pointer, bool scanning = false);

FuncRParams *getFuncRParams(std::vector<Token> wordList, int *pointer, bool scanning = false);

FuncType *getFuncType(std::vector<Token> wordList, int *pointer);

FuncFParam *getFuncFParam(std::vector<Token> wordList, int *pointer);

FuncFParams *getFuncFParams(std::vector<Token> wordList, int *pointer);

FuncDef *getFuncDef(std::vector<Token> wordList, int *pointer);

#endif //COMPILER_SYNTAX_H
