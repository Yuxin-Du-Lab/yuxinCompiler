//
// Created by yuxin on 2021/10/3.
//

#ifndef COMPILER_ASTBUILDER_H
#define COMPILER_ASTBUILDER_H

#include <fstream>
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

    void print(int tab);

    // std::string toString();;
};

class Number {
    Token *intConst;
public:
    void setIntConst(Token *token);

    void print(int tab);

    // std::string toString();;
};

class PrimaryExp {
public:
    virtual void print(int tab) {};

//    virtual // std::string toString(); { return "base"; };
};

class ExpPrimaryExp : public PrimaryExp {
    Exp *exp{};
public:
    void setPrimaryExp(Exp *exp);

    void print(int tab) override;

    // std::string toString();;
};

class LValPrimaryExp : public PrimaryExp {
    LVal *lVal{};
public:
    void setPrimaryLVal(LVal *lVal);

    void print(int tab) override;

    // std::string toString();;
};

class NumberPrimaryExp : public PrimaryExp {
    Number *number{};
public:
    void setPrimaryNumber(Number *number);

    void print(int tab) override;

    // std::string toString();;
};

class UnaryExp {
public:
    virtual void print(int tab) {};

//    virtual // std::string toString(); { return "base"; };
};

class PrimaryUnaryExp : public UnaryExp {
    PrimaryExp *primaryExp{};
public:
    void setPrimaryExp(PrimaryExp *primaryExp);

    void print(int tab) override;

    // std::string toString();;
};

class FuncUnaryExp : public UnaryExp {
    Token *ident{};
    FuncRParams *funcRParams{};
public:
    void setFuncUnaryIdent(Token *token);

    void setFuncRParams(FuncRParams *funcRParams);

    void print(int tab) override;

    // std::string toString();;
};

class UnaryUnaryExp : public UnaryExp {
    UnaryOp *unaryOp{};
    UnaryExp *unaryExp{};
public:
    void setUnaryOp(UnaryOp *unaryOp);

    void setUnaryExp(UnaryExp *unaryExp);

    void print(int tab) override;

    // std::string toString();;
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

    void print(int tab);

    // std::string toString();;
};

class MulExp {
    MulOpTree *root;
public:
    void setRoot(MulOpTree *root);

    void print(int tab);

    // std::string toString();;
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

    void print(int tab);

    // std::string toString();;
};

class AddExp {
    AddOpTree *root;
public:
    void setRoot(AddOpTree *root);

    void print(int tab);

    // std::string toString();;
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

    void print(int tab);

    // std::string toString();;
};

class RelExp {
    RelOpTree *root;
public:
    void setRoot(RelOpTree *root);

    void print(int tab);

    // std::string toString();;
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

    void print(int tab);

    // std::string toString();;
};

class EqExp {
    EqOpTree *root;
public:
    void setEqExpRoot(EqOpTree *root);

    void print(int tab);

    // std::string toString();;
};

class LAndExp {
    std::vector<EqExp *> eqExps;
public:
    void addEqExp(EqExp *eqExp);

    void print(int tab);

    // std::string toString();;
};

class LOrExp {
    std::vector<LAndExp *> lAndExps;
public:
    void addLAndExp(LAndExp *lAndExp);

    void print(int tab);

    // std::string toString();;
};

class Cond {
    LOrExp *lOrExp;
public:
    void setLOrExp(LOrExp *lOrExp);

    void print(int tab);

    // std::string toString();;
};

class LVal {
    Token *ident;
    int row;
    Exp *exps[2];
public:
    void setLValIdent(Token *token);

    void setRow(int row);

    void setArrayExp(Exp *exp, int row);

    void print(int tab);

    // std::string toString();;
};

class Exp {
    AddExp *addExp;
public:
    void setAddExp(AddExp *addExp);

    void print(int tab);

    // std::string toString();;
};


class InitVal {
    Exp *exp;
    int row;
    std::vector<InitVal *> initVals;
public:
    void setExp(Exp *exp);

    void setRow(int row);

    void addInitVal(InitVal *initVal);

    void print(int tab);

    // std::string toString();;
};

class ConstExp {
    AddExp *addExp;
//    ident used must be const HOW TO DO?
// TODO
public:
    void setAddExp(AddExp *addExp);

    void print(int tab);

    // std::string toString();;
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

    void print(int tab);

    // std::string toString();;
};


class VarDecl {
    std::vector<VarDef *> varDefs;
public:
    void addVarDef(VarDef *varDef);

    void print(int tab);

    // std::string toString();;
};

class ConstInitVal {
    ConstExp *constExp;
    int row;
    std::vector<ConstInitVal *> constInitVals;
public:
    void setConstExp(ConstExp *constExp);

    void setRow(int row);

    void addConstInitVal(ConstInitVal *constInitVal);

    void print(int tab);

    // std::string toString();;
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

    void print(int tab);

    // std::string toString();;
};

class ConstDecl {
    std::vector<ConstDef *> constDefs;
public:
    void addConstDef(ConstDef *constDef);

    void print(int tab);

    // std::string toString();;
};

class Block {
    std::vector<ConstDecl *> constDecls;
    std::vector<VarDecl *> varDecls;
    std::vector<Stmt *> stmts;
public:
    void addConstDecl(ConstDecl *constDecl);

    void addVarDecl(VarDecl *varDecl);

    void addStmt(Stmt *stmt);

    void print(int tab);

    // std::string toString();;
};

class Stmt {
public:
    virtual void print(int tab) {};

//    virtual // std::string toString(); { return "base"; };
};

class LValStmt : public Stmt {
    LVal *lVal{};
    Exp *exp{};
public:
    void setLVal(LVal *lVal);

    void setExp(Exp *exp);

    void print(int tab) override;

    // std::string toString();;
};

class ExpStmt : public Stmt {
    Exp *exp{};
public:
    void setExp(Exp *exp);

    void print(int tab) override;

    // std::string toString();;
};

class BlockStmt : public Stmt {
    Block *block{};
public:
    void setBlock(Block *block);

    void print(int tab) override;

    // std::string toString();;
};

class IfStmt : public Stmt {
    Cond *cond{};
    Stmt *ifStmt{};
    Stmt *elseStmt{};
public:
    void setCond(Cond *cond);

    void setIfStmt(Stmt *stmt);

    void setElseStmt(Stmt *stmt);

    void print(int tab) override;

    // std::string toString();;
};

class WhileStmt : public Stmt {
    Cond *cond{};
    Stmt *stmt{};
public:
    void setCond(Cond *cond);

    void setWhileStmt(Stmt *stmt);

    void print(int tab) override;

    // std::string toString();;
};

class LoopStmt : public Stmt {
    Token *break_continue{};
public:
    void setToken(Token *token);

    void print(int tab) override;

    // std::string toString();;
};

class ReturnStmt : public Stmt {
    Exp *exp{};
public:
    void setReturnExp(Exp *exp);

    void print(int tab) override;

    // std::string toString();;
};

class GetintStmt : public Stmt {
    LVal *lVal{};
public:
    void setLVal(LVal *lVal);

    void print(int tab) override;

    // std::string toString();;
};

class PrintfStmt : public Stmt {
    Token *formatString;
    std::vector<Exp *> formatExps;
public:
    void setFormatString(Token *token);

    void addFormatExp(Exp *exp);

    void print(int tab) override;

    // std::string toString();;
};

class FuncFParam {
    Token *ident;
    int row;
    ConstExp *constExp;
public:
    void setFuncFParamIdent(Token *token);

    void setRow(int rowIn);

    void setConstExp(ConstExp *constExp);

    void print(int tab);

    // std::string toString();;
};

class FuncFParams {
    std::vector<FuncFParam *> funcFParams;
public:
    void addFuncFParam(FuncFParam *funcFParam);

    void print(int tab);

    // std::string toString();;
};

class FuncType {
    Token *funcType; // void or int
public:
    void setFuncType(Token *funcType);

    void print(int tab);

    // std::string toString();;
};

//TODO
class FuncRParams {
    std::vector<Exp *> paramExps;
public:
    void addParamsExp(Exp *exp);

    void print(int tab);

    // std::string toString();;
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

    void print(int tab);

    // std::string toString();;
};


class MainFuncDef {
    Block *block;
public:
    void setBlock(Block *block);

    void print(int tab);

    // std::string toString();;
};

class CompUnit {
public:
    std::vector<ConstDecl *> constDecls;
    std::vector<VarDecl *> varDecls;
    std::vector<FuncDef *> funcDefs;
    MainFuncDef *mainFuncDef;
public:
    void addConstDecl(ConstDecl *constDecl);

    void addVarDecl(VarDecl *varDecl);

    void addFuncDef(FuncDef *funcDef);

    void setMainFuncDef(MainFuncDef *mainFuncDef);

    void print();

    // std::string toString();;
};

#endif //COMPILER_ASTBUILDER_H
