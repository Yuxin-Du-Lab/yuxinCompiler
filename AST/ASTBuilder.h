//
// Created by yuxin on 2021/10/3.
//

#ifndef COMPILER_ASTBUILDER_H
#define COMPILER_ASTBUILDER_H

#include <fstream>
#include "iostream"
#include "vector"
#include "../token/Token.h"

#define VALUE_ERROR -999

const int LeftChild = 0, RightChild = 1;

class AstItem {
public:
    virtual void print(int tab) { std::cout << "need to be override" << std::endl; }

    virtual int makeIR() { return VALUE_ERROR; }
};

class Stmt;

class Exp;

class FuncRParams;

class LVal;

class ConstDef;

class UnaryOp : public AstItem {
    Token *op;   // + - !
//    ! only can exist in cond
// TODO
public:
    void setUnaryOp(Token *token);

    Token *getOp() {
        return this->op;
    }

    void print(int tab) override;
};

class Number : public AstItem {
    Token *intConst;
    int value;
public:
    void setIntConst(Token *token);

    void print(int tab) override;

    int getConstValue();
};

class PrimaryExp : public AstItem {
public:
    virtual int getRealDimension() { return VALUE_ERROR; };

    virtual int getConstValue() { return VALUE_ERROR; };

    virtual int makeIR() { return VALUE_ERROR; };
};

class ExpPrimaryExp : public PrimaryExp {
    Exp *exp{};
public:
    void setPrimaryExp(Exp *exp);

    void print(int tab) override;

    int getRealDimension() override;

    int getConstValue() override;

    int makeIR() override;
};

class LValPrimaryExp : public PrimaryExp {
    LVal *lVal{};
public:
    void setPrimaryLVal(LVal *lVal);

    void print(int tab) override;

    int getRealDimension() override;

    int getConstValue() override;

    int makeIR() override;
};

class NumberPrimaryExp : public PrimaryExp {
    Number *number{};
public:
    void setPrimaryNumber(Number *number);

    void print(int tab) override;

    int getRealDimension() override {
        return 0;
    }

    int getConstValue() override;

    int makeIR() override;
};

class UnaryExp : public AstItem {
public:
    virtual int getRealDimension() { return VALUE_ERROR; };

    virtual int getConstValue() { return VALUE_ERROR; };

    virtual int makeIR() { return VALUE_ERROR; };
};

class PrimaryUnaryExp : public UnaryExp {
    PrimaryExp *primaryExp{};
public:
    void setPrimaryExp(PrimaryExp *primaryExp);

    void print(int tab) override;

    int getRealDimension() override {
        return primaryExp->getRealDimension();
    }

    int makeIR() override;

    int getConstValue() override {
        return this->primaryExp->getConstValue();
    }
};

class FuncUnaryExp : public UnaryExp {
    Token *ident{};
    FuncRParams *funcRParams{};
public:
    void setFuncUnaryIdent(Token *token);

    void setFuncRParams(FuncRParams *funcRParams);

    void print(int tab) override;

    Token *getIdent() {
        return this->ident;
    }

    FuncRParams *getFuncRParams() {
        return this->funcRParams;
    }

    int getRealDimension() override;

    int makeIR() override;
};

class UnaryUnaryExp : public UnaryExp {
    UnaryOp *unaryOp{};
    UnaryExp *unaryExp{};
public:
    void setUnaryOp(UnaryOp *unaryOp);

    void setUnaryExp(UnaryExp *unaryExp);

    void print(int tab) override;

    int getRealDimension() override {
        return unaryExp->getRealDimension();
    }

    int getConstValue() override {
        if (this->unaryOp->getOp()->getIdentity() == PLUS) {
            return this->unaryExp->getConstValue();
        } else if (this->unaryOp->getOp()->getIdentity() == MINU) {
            return -this->unaryExp->getConstValue();
        }
//        TODO cond !
        return VALUE_ERROR;
    }

    int makeIR() override;
};

class MulOpTree : public AstItem {
    bool isLeaf;
    Token *op;
    MulOpTree *branch[2];
    UnaryExp *value;
public:
    void setIsLeaf(bool isLeaf);

    void setOp(Token *token);

    void setBranch(MulOpTree *mulOpTree, int child);

    void serValue(UnaryExp *value);

    void print(int tab) override;

    bool getIsLeaf() {
        return isLeaf;
    }

    int getRealDimension() {
        if (isLeaf) {
            return value->getRealDimension();
        } else {
            return 0;
        }
    }

    int getConstValue() {
        if (getIsLeaf()) {
            return this->value->getConstValue();
        } else {
            if (this->op->getIdentity() == MULT) {
                return this->branch[0]->getConstValue() * this->branch[1]->getConstValue();
            } else if (this->op->getIdentity() == DIV) {
                return this->branch[0]->getConstValue() / this->branch[1]->getConstValue();
            } else if (this->op->getIdentity() == MOD) {
                return this->branch[0]->getConstValue() % this->branch[1]->getConstValue();
            }
        }
        return VALUE_ERROR;
    }

    int makeIR();
};

class MulExp : public AstItem {
    MulOpTree *root;
public:
    void setRoot(MulOpTree *root);

    void print(int tab) override;

    int getRealDimension() {
        if (root->getIsLeaf()) {
            return root->getRealDimension();
        } else {
            return 0;
        }
    }

    int getConstValue() {
        return this->root->getConstValue();
    }

    int makeIR();
};

class AddOpTree : public AstItem {
    bool isLeaf;
    Token *op;
    AddOpTree *branch[2];
    MulExp *value;
public:
    void setIsLeaf(bool isLeaf);

    void setOp(Token *token);

    void setBranch(AddOpTree *addOpTree, int child);

    void serValue(MulExp *value);

    void print(int tab) override;

    bool getIsLeaf() {
        return isLeaf;
    }

    int getRealDimension() {
        if (isLeaf) {
            return value->getRealDimension();
        } else {
            return 0;
        }
    }

    int getConstValue() {
        if (getIsLeaf()) {
            return this->value->getConstValue();
        } else {
            if (this->op->getIdentity() == PLUS) {
                return this->branch[0]->getConstValue() + this->branch[1]->getConstValue();
            } else if (this->op->getIdentity() == MINU) {
                return this->branch[0]->getConstValue() - this->branch[1]->getConstValue();
            }
        }
        return VALUE_ERROR;
    }

    int makeIR();
};

class AddExp : public AstItem {
    AddOpTree *root;
public:
    void setRoot(AddOpTree *root);

    void print(int tab) override;

    int getRealDimension() {
        if (root->getIsLeaf()) {
            return root->getRealDimension();
        } else {
            return 0;
        }
    }

    int getConstValue() {
        return this->root->getConstValue();
    }

    int makeIR();
};

class RelOpTree : public AstItem {
    bool isLeaf;
    Token *op;
    RelOpTree *branch[2];
    AddExp *value;
public:
    void setIsLeaf(bool isLeaf);

    void setOp(Token *token);

    void setBranch(RelOpTree *relOpTree, int child);

    void setLeafValue(AddExp *addExp);

    void print(int tab) override;

    int makeIR();
};

class RelExp : public AstItem {
    RelOpTree *root;
public:
    void setRoot(RelOpTree *root);

    void print(int tab) override;

    int makeIR();
};

class EqOpTree : public AstItem {
    bool isLeaf;
    Token *op;
    EqOpTree *branch[2];
    RelExp *value;
public:
    void setIsLeaf(bool isLeaf);

    void setOp(Token *token);

    void setBranch(EqOpTree *eqOpTree, int child);

    void setLeafValue(RelExp *relExp);

    void print(int tab) override;

    int makeIR();
};

class EqExp : public AstItem {
    EqOpTree *root;
public:
    void setEqExpRoot(EqOpTree *root);

    void print(int tab);

    int makeIR();
};

class LAndExp : public AstItem {
    std::vector<EqExp *> eqExps;
public:
    void addEqExp(EqExp *eqExp);

    void print(int tab) override;

    int makeIR();
};

class LOrExp : public AstItem {
    std::vector<LAndExp *> lAndExps;
public:
    void addLAndExp(LAndExp *lAndExp);

    void print(int tab) override;

    int makeIR();
};

class Cond : public AstItem {
    LOrExp *lOrExp;
public:
    void setLOrExp(LOrExp *lOrExp);

    void print(int tab) override;

    int makeIR();
};

class LVal : public AstItem {
    Token *ident;
    int dimension;
    int usedDimension;
    Exp *exps[2];
    int constValue = VALUE_ERROR;
public:
    void setLValIdent(Token *token);

    void setUsedDimension(int row);

    void setDimension(int dimension) {
        this->dimension = dimension;
    }

    void setArrayExp(Exp *exp, int row);

    void print(int tab) override;

    Token *getIdent() {
        return this->ident;
    }

    int getRealDimension() {
        return dimension - usedDimension;
    }

    int makeIR();

    void setConstValue(int constValueIn) {
        this->constValue = constValueIn;
    }

    int getConstValue() {
        return this->constValue;
    }

    int getDimension() {
        return this->dimension;
    }

    Exp *getExp(int index) {
        return this->exps[index];
    }
};

class Exp : public AstItem {
    AddExp *addExp;
public:
    void setAddExp(AddExp *addExp);

    void print(int tab) override;

    int getRealDimension();

    int makeIR();
};


class InitVal : public AstItem {
    Exp *exp{};
    int row;
    std::vector<InitVal *> initVals;
public:
    void setExp(Exp *exp);

    void setRow(int row);

    void addInitVal(InitVal *initVal);

    void print(int tab) override;

    int makeIR(int index = 0, bool isArr = false, Token *ident = nullptr);
};

class ConstExp : public AstItem {
    AddExp *addExp;
//    ident used must be const HOW TO DO?
// TODO
public:
    void setAddExp(AddExp *addExp);

    void print(int tab) override;

    int getConstValue() {
        return this->addExp->getConstValue();
    }

    int makeIR();
};

class VarDef : public AstItem {
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

    void print(int tab) override;

    Token *getIdent() {
        return this->ident;
    }

    int getRow() {
        return this->row;
    }

    int makeIR();
};


class VarDecl : public AstItem {
    std::vector<VarDef *> varDefs;
public:
    void addVarDef(VarDef *varDef);

    void print(int tab) override;

    int makeIR() override;
};

class ConstInitVal : public AstItem {
    ConstExp *constExp{};
    int row;
    std::vector<ConstInitVal *> constInitVals;
    std::vector<int> values;
    ConstDef* constDef{};
public:
    void setConstDef(ConstDef *constDefIn) {
        this->constDef = constDefIn;
    }

    void setConstExp(ConstExp *constExp);

    void setRow(int row);

    void addConstInitVal(ConstInitVal *constInitVal);

    void print(int tab) override;

    void setValues() {
        for (auto iter : this->constInitVals) {
            std::vector<int> subValues = iter->initValues();
            this->values.insert(this->values.end(), subValues.begin(), subValues.end());
        }
        for (auto iter : this->values) {
            std::cout << std::to_string(iter) << std::endl;
        }
        std::cout << "check end" << std::endl;
    }

    std::vector<int> initValues() {
        std::vector<int> tmpValues;
        if (this->constExp != nullptr) {
            tmpValues.emplace_back(this->constExp->getConstValue());
        } else {
            for (auto iter : this->constInitVals) {
                std::vector<int> subValues = iter->initValues();
                tmpValues.insert(tmpValues.end(), subValues.begin(), subValues.end());
            }
        }
        return tmpValues;
    }

    int getValue(int row1=0, int row2=0);

    int makeIR(int index = 0, bool isArr = false, Token *ident = nullptr);

    int getRow() {
        return this->row;
    }
};

class ConstDef : public AstItem {
    Token *ident;
    int row;
    ConstExp *constExps[2];
    ConstInitVal *constInitVal{};
public:
    void setIdent(Token *token);

    void setRow(int row);

    void setArrayConstExp(ConstExp *constExp, int row);

    void setConstInitVal(ConstInitVal *constInitVal);

    void print(int tab) override;

    Token *getIdent() {
        return this->ident;
    }

    int getRow() {
        return this->row;
    }

    ConstExp *getConstExp(int index) {
        return this->constExps[index];
    }

    ConstInitVal *getConstInitVal() {
        return this->constInitVal;
    }

    int makeIR();
};

class ConstDecl : public AstItem {
    std::vector<ConstDef *> constDefs;
public:
    void addConstDef(ConstDef *constDef);

    void print(int tab) override;

    int makeIR() override;
};

class Block : public AstItem {
//    std::vector<ConstDecl *> constDecls;
//    std::vector<VarDecl *> varDecls;
//    std::vector<Stmt *> stmts;
    std::vector<AstItem *> blockItems;
public:
    void addConstDecl(ConstDecl *constDecl);

    void addVarDecl(VarDecl *varDecl);

    void addStmt(Stmt *stmt);

    void print(int tab) override;

    int makeIR();
};

class Stmt : public AstItem {
    virtual int makeIR() override { return VALUE_ERROR; };
};

class LValStmt : public Stmt {
    LVal *lVal{};
    Exp *exp{};
public:
    void setLVal(LVal *lVal);

    void setExp(Exp *exp);

    void print(int tab) override;

    int makeIR() override;
};

class ExpStmt : public Stmt {
    Exp *exp{};
public:
    void setExp(Exp *exp);

    void print(int tab) override;

    int makeIR() override;
};

class BlockStmt : public Stmt {
    Block *block{};
public:
    void setBlock(Block *block);

    void print(int tab) override;

    int makeIR() override;
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

    int makeIR() override;
};

class WhileStmt : public Stmt {
    Cond *cond{};
    Stmt *stmt{};
public:
    void setCond(Cond *cond);

    void setWhileStmt(Stmt *stmt);

    void print(int tab) override;

    int makeIR() override;
};

class LoopStmt : public Stmt {
    Token *break_continue{};
public:
    void setToken(Token *token);

    void print(int tab) override;

    int makeIR() override;
};

class ReturnStmt : public Stmt {
    Exp *exp{};
public:
    void setReturnExp(Exp *exp);

    void print(int tab) override;

    int makeIR() override;
};

class GetintStmt : public Stmt {
    LVal *lVal{};
public:
    void setLVal(LVal *lVal);

    void print(int tab) override;

    int makeIR() override;
};

class PrintfStmt : public Stmt {
    Token *formatString;
    std::vector<Exp *> formatExps;
public:
    void setFormatString(Token *token);

    void addFormatExp(Exp *exp);

    void print(int tab) override;

    Token *getFormatStringTK() {
        return formatString;
    }

    int getFormatNum() {
        int num = 0;
        for (int i = 0; i < formatString->getKey().size(); i++) {
            if (formatString->getKey()[i] == '%' && formatString->getKey()[i + 1] == 'd') {
                num++;
                i++;
            }
        }
        return num;
    }

    bool illegalFormatString() {
        for (int index = 1; index < formatString->getKey().size() - 1; index++) {
            char c = formatString->getKey()[index];
            if (c == '%' && formatString->getKey()[index + 1] == 'd') {
                index++;
                continue;
            } else if (c == 32 || c == 33 || c >= 40 && c <= 126) {
                continue;
            } else {
                return true;
            }
        }
        return false;
    }

    int makeIR() override;
};

class FuncFParam : public AstItem {
    Token *ident;
    int row;
    ConstExp *constExp;
public:
    void setFuncFParamIdent(Token *token);

    void setRow(int rowIn);

    void setConstExp(ConstExp *constExp);

    void print(int tab) override;

    Token *getIdent() {
        return ident;
    }

    int getRow() {
        return this->row;
    }

    int makeIR();
};

class FuncFParams : public AstItem {
    std::vector<FuncFParam *> funcFParams;
public:
    void addFuncFParam(FuncFParam *funcFParam);

    void print(int tab) override;

    std::vector<FuncFParam *> getFParams() {
        return this->funcFParams;
    }

    int getFParamNum() {
        return this->funcFParams.size();
    }

    int makeIR();
};

class FuncType : public AstItem {
    Token *funcType; // void or int
public:
    void setFuncType(Token *funcType);

    void print(int tab) override;

    bool isVoid() {
        return this->funcType->getIdentity() == VOIDTK;
    }
};

//TODO
class FuncRParams : public AstItem {
    std::vector<Exp *> paramExps;
public:
    void addParamsExp(Exp *exp);

    void print(int tab) override;

    int getRParamNum() {
        return this->paramExps.size();
    }

    std::vector<Exp *> &getRParams() {
        return this->paramExps;
    }

    int makeIR();
};

class FuncDef : public AstItem {
    FuncType *funcType;
    Token *ident;
    FuncFParams *funcFParams;
    Block *block;
public:
    void setFuncType(FuncType *funcType);

    void setFuncIdent(Token *token);

    void setFuncFParams(FuncFParams *funcFParams);

    void setFuncBlock(Block *block);

    void print(int tab) override;

    FuncType *getFuncType() {
        return this->funcType;
    }

    std::vector<FuncFParam *> getFParams() {
        return this->funcFParams->getFParams();
    }

    int makeIR();
};


class MainFuncDef : public AstItem {
    Block *block;
public:
    void setBlock(Block *block);

    void print(int tab) override;

    int makeIR();
};

class CompUnit : public AstItem {
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

    void print(int tab) override;

    int makeIR();
};

#endif //COMPILER_ASTBUILDER_H
