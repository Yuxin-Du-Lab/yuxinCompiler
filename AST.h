//
// Created by yuxin on 2021/10/3.
//

#ifndef COMPILER_AST_H
#define COMPILER_AST_H

#include "iostream"
#include "vector"
#include "Token.h"

class Stmt;

class Exp;

class FuncRParams;

class LVal;

class UnaryOp {
    Token *op;   // + - !
//    ! only can exist in cond
// TODO
public:
    void setUnaryOp(Token *token);
};

class Number {
    Token *intConst;
public:
    void setIntConst(Token *token);
};

class PrimaryExp {

};

class ExpPrimaryExp : public PrimaryExp {
    Exp *exp;
public:
    void setPrimaryExp(Exp *exp);
};

class LValPrimaryExp : public PrimaryExp {
    LVal *lVal;
public:
    void setPrimaryLVal(LVal *lVal);
};

class NumberPrimaryExp : public PrimaryExp {
    Number *number;
public:
    void setPrimaryNumber(Number *number);
};

class UnaryExp {

};

class PrimaryUnaryExp : public UnaryExp {
    PrimaryExp *primaryExp;
public:
    void setPrimaryExp(PrimaryExp *primaryExp);
};

class FuncUnaryExp : public UnaryExp {
    Token *ident;
    FuncRParams *funcRParams;
public:
    void setFuncUnaryIdent(Token *token);
    void setFuncRParams(FuncRParams *funcRParams);
};

class UnaryUnaryExp : public UnaryExp {
    UnaryOp *unaryOp;
    UnaryExp *unaryExp;
public:
    void setUnaryOp(UnaryOp *unaryOp);
    void setUnaryExp(UnaryExp *unaryExp);
};

class MulOpTree {
    bool isLeaf;
    Token *op;
    MulOpTree *branch[2];
    UnaryExp *value;
public:
    void setIsLeaf(bool isLeaf);
    void setOp(Token *token);
    void setBranch(MulOpTree *mulOpTree, int child);
    void serValue(UnaryExp *value);
};

class MulExp {
    MulOpTree *root;
public:
    void setRoot(MulOpTree *root);
};

class AddOpTree {
    bool isLeaf;
    Token *op;
    AddOpTree *branch[2];
    MulExp *value;
public:
    void setIsLeaf(bool isLeaf);
    void setOp(Token *token);
    void setBranch(AddOpTree *addOpTree, int child);
    void serValue(MulExp *value);
};

class AddExp {
    AddOpTree *root;
public:
    void setRoot(AddOpTree *root);
};

class RelOpTree {
    bool isLeaf;
    Token *op;
    RelOpTree *branch[2];
    AddExp *value;
public:
    void setIsLeaf(bool isLeaf);
    void setOp(Token *token);
    void setBranch(RelOpTree *relOpTree, int child);
    void setLeafValue(AddExp *addExp);
};

class RelExp {
    RelOpTree *root;
public:
    void setRoot(RelOpTree *root);
};

class EqOpTree {
    bool isLeaf;
    Token *op;
    EqOpTree *branch[2];
    RelExp *value;
public:
    void setIsLeaf(bool isLeaf);
    void setOp(Token *token);
    void setBranch(EqOpTree *eqOpTree, int child);
    void setLeafValue(RelExp *relExp);
};

class EqExp {
    EqOpTree *root;
public:
    void setEqExpRoot(EqOpTree *root);
};

class LAndExp {
    std::vector<EqExp> eqExps;
public:
    void addEqExp(EqExp eqExp);
};

class LOrExp {
    std::vector<LAndExp> lAndExps;
public:
    void addLAndExp(LAndExp lAndExp);
};

class Cond {
    LOrExp *lOrExp;
public:
    void setLOrExp(LOrExp *lOrExp);
};

class LVal {
    Token *ident;
    int row;
    Exp *exps[2];
public:
    void setLValIdent(Token *token);
    void setRow(int row);
    void setArrayExp(Exp *exp, int row);
};

class Exp {
    AddExp *addExp;
public:
    void setAddExp(AddExp *addExp);
};


class InitVal {
    Exp *exp;
    int row;
    std::vector<InitVal> initVals;
public:
    void setExp(Exp *exp);
    void setRow(int row);
    void addInitVal(InitVal initVal);
};

class ConstExp {
    AddExp *addExp;
//    ident used must be const HOW TO DO?
// TODO
public:
    void setAddExp(AddExp *addExp);
};

class VarDef {
    Token *ident;
    int row;
    ConstExp *constExps[2];
    bool hasInitVal;
    InitVal *initVal;
public:
    void setIdent(Token *token);
    void setRow(int row);
    void setArrayConstExp(ConstExp *constExp, int row);
    void setHasInitVal(bool hasInitVal);
    void setInitVal(InitVal *initVal);
};


class VarDecl {
    std::vector<VarDef> varDefs;
public:
    void addVarDef(VarDef varDef);
};

class ConstInitVal {
    ConstExp *constExp;
    int row;
    std::vector<ConstInitVal> constInitVals;
public:
    void setConstExp(ConstExp *constExp);
    void setRow(int row);
    void addConstInitVal(ConstInitVal constInitVal);
};

class ConstDef {
    Token *ident;
    int row;
    ConstExp *constExps[2];
    ConstInitVal *constInitVal;
public:
    void setIdent(Token *token);
    void setRow(int row);
    void setArrayConstExp(ConstExp *constExp, int row);
    void setConstInitVal(ConstInitVal *constInitVal);
};

class ConstDecl {
    std::vector<ConstDef> constDefs;
public:
    void addConstDef(ConstDef constDef);
};

class Block {
    std::vector<ConstDecl> constDecls;
    std::vector<VarDecl> varDecls;
    std::vector<Stmt> stmts;
public:
    void addConstDecl(ConstDecl constDecl);
    void addVarDecl(VarDecl varDecl);
    void addStmt(Stmt stmt);
};

class Stmt {

};

class LValStmt : public Stmt {
    LVal *lVal;
    Exp *exp;
public:
    void setLVal(LVal *lVal);
    void setExp(Exp *exp);
};

class ExpStmt : public Stmt {
    Exp *exp;
public:
    void setExp(Exp *exp);
};

class BlockStmt : public Stmt {
    Block *block;
public:
    void setBlock(Block *block);
};

class IfStmt : public Stmt {
    Cond *cond;
    Stmt *ifStmt;
    Stmt *elseStmt;
public:
    void setCond(Cond *cond);
    void setIfStmt(Stmt *stmt);
    void setElseStmt(Stmt *stmt);
};

class WhileStmt : public Stmt {
    Cond *cond;
    Stmt *stmt;
public:
    void setCond(Cond *cond);
    void setWhileStmt(Stmt *stmt);
};

class LoopStmt : public Stmt {
    Token *break_continue;
public:
    void setToken(Token *token);
};

class ReturnStmt : public Stmt {
    Exp *exp;
public:
    void setReturnExp(Exp *exp);
};

class GetintStmt : public Stmt {
    LVal *lVal;
public:
    void setLVal(LVal *lVal);
};

class PrintfStmt : public Stmt {
    Token *formatString;
    std::vector<Exp> formatExps;
public:
    void setFormatString(Token *token);
    void addFormatExp(Exp exp);
};

class FuncFParam {
    Token *ident;
    int row;
    ConstExp *constExp;
public:
    void setFuncFParamIdent(Token *token);
    void setRow(int rowIn);
    void setConstExp(ConstExp *constExp);
};

class FuncFParams {
    std::vector<FuncFParam> funcFParams;
public:
    void addFuncFParam(FuncFParam funcFParam);
};

class FuncType {
    Token *funcType; // void or int
public:
    void setFuncType(Token *funcType);
};

class FuncRParams {
    std::vector<Exp> paramExps;
};

class FuncDef {
    FuncType *funcType;
    Token *ident;
    FuncFParams *funcFParams;
    Block *block;
public:
    void setFuncType(FuncType *funcType);
    void setFuncIdent(Token *token);
    void setFuncFParams(FuncFParams *funcFParams);
    void setFuncBlock(Block *block);
};


class MainFuncDef {
    Block *block;
public:
    void setBlock(Block *block);
};

class CompUnit {
public:
    std::vector<ConstDecl> constDecls;
    std::vector<VarDecl> varDecls;
    std::vector<FuncDef> funcDefs;
    MainFuncDef *mainFuncDef;
public:
    void addConstDecl(ConstDecl constDecl);
    void addVarDecl(VarDecl varDecl);
    void addFuncDef(FuncDef funcDef);
    void setMainFuncDef(MainFuncDef *mainFuncDef);
};

#endif //COMPILER_AST_H
