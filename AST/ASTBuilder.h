//
// Created by yuxin on 2021/10/3.
//

#ifndef COMPILER_ASTBUILDER_H
#define COMPILER_ASTBUILDER_H

#include <fstream>
#include "iostream"
#include "vector"
#include "../token/Token.h"

class AstItem {

};

class Stmt;

class Exp;

class FuncRParams;

class LVal;

class UnaryOp : public AstItem {
    Token *op;   // + - !
//    ! only can exist in cond
// TODO
public:
    void setUnaryOp(Token *token);

    void print(int tab);
};

class Number : public AstItem {
    Token *intConst;
public:
    void setIntConst(Token *token);

    void print(int tab);


};

class PrimaryExp : public AstItem {
public:
    virtual void print(int tab) {};

    virtual int getRealDimension() { return -1; };
};

class ExpPrimaryExp : public PrimaryExp {
    Exp *exp{};
public:
    void setPrimaryExp(Exp *exp);

    void print(int tab) override;

    int getRealDimension() override;
};

class LValPrimaryExp : public PrimaryExp {
    LVal *lVal{};
public:
    void setPrimaryLVal(LVal *lVal);

    void print(int tab) override;

    int getRealDimension() override;
};

class NumberPrimaryExp : public PrimaryExp {
    Number *number{};
public:
    void setPrimaryNumber(Number *number);

    void print(int tab) override;

    int getRealDimension() override {
        return 0;
    }
};

class UnaryExp : public AstItem {
public:
    virtual void print(int tab) {};

    virtual int getRealDimension() { return -1; };
};

class PrimaryUnaryExp : public UnaryExp {
    PrimaryExp *primaryExp{};
public:
    void setPrimaryExp(PrimaryExp *primaryExp);

    void print(int tab) override;

    int getRealDimension() override {
        return primaryExp->getRealDimension();
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

    int getRealDimension() override {
        return 0;
    }
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

    void print(int tab);

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
};

class MulExp : public AstItem {
    MulOpTree *root;
public:
    void setRoot(MulOpTree *root);

    void print(int tab);

    int getRealDimension() {
        if (root->getIsLeaf()) {
            return root->getRealDimension();
        } else {
            return 0;
        }
    }
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

    void print(int tab);

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
};

class AddExp : public AstItem {
    AddOpTree *root;
public:
    void setRoot(AddOpTree *root);

    void print(int tab);

    int getRealDimension() {
        if (root->getIsLeaf()) {
            return root->getRealDimension();
        } else {
            return 0;
        }
    }
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

    void print(int tab);
};

class RelExp : public AstItem {
    RelOpTree *root;
public:
    void setRoot(RelOpTree *root);

    void print(int tab);


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

    void print(int tab);


};

class EqExp : public AstItem {
    EqOpTree *root;
public:
    void setEqExpRoot(EqOpTree *root);

    void print(int tab);


};

class LAndExp : public AstItem {
    std::vector<EqExp *> eqExps;
public:
    void addEqExp(EqExp *eqExp);

    void print(int tab);


};

class LOrExp : public AstItem {
    std::vector<LAndExp *> lAndExps;
public:
    void addLAndExp(LAndExp *lAndExp);

    void print(int tab);


};

class Cond : public AstItem {
    LOrExp *lOrExp;
public:
    void setLOrExp(LOrExp *lOrExp);

    void print(int tab);


};

class LVal : public AstItem {
    Token *ident;
    int dimension;
    int usedDimension;
    Exp *exps[2];
public:
    void setLValIdent(Token *token);

    void setUsedDimension(int row);

    void setDimension(int dimension) {
        this->dimension = dimension;
    }

    void setArrayExp(Exp *exp, int row);

    void print(int tab);

    Token *getIdent() {
        return this->ident;
    }

    int getRealDimension() {
        return dimension - usedDimension;
    }
};

class Exp : public AstItem {
    AddExp *addExp;
public:
    void setAddExp(AddExp *addExp);

    void print(int tab);

    int getRealDimension();
};



class InitVal : public AstItem {
    Exp *exp;
    int row;
    std::vector<InitVal *> initVals;
public:
    void setExp(Exp *exp);

    void setRow(int row);

    void addInitVal(InitVal *initVal);

    void print(int tab);


};

class ConstExp : public AstItem {
    AddExp *addExp;
//    ident used must be const HOW TO DO?
// TODO
public:
    void setAddExp(AddExp *addExp);

    void print(int tab);


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

    void print(int tab);

    Token *getIdent() {
        return this->ident;
    }

    int getRow() {
        return this->row;
    }
};


class VarDecl : public AstItem {
    std::vector<VarDef *> varDefs;
public:
    void addVarDef(VarDef *varDef);

    void print(int tab);
};

class ConstInitVal : public AstItem {
    ConstExp *constExp;
    int row;
    std::vector<ConstInitVal *> constInitVals;
public:
    void setConstExp(ConstExp *constExp);

    void setRow(int row);

    void addConstInitVal(ConstInitVal *constInitVal);

    void print(int tab);
};

class ConstDef : public AstItem {
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

    Token *getIdent() {
        return this->ident;
    }

    int getRow() {
        return this->row;
    }
};

class ConstDecl : public AstItem {
    std::vector<ConstDef *> constDefs;
public:
    void addConstDef(ConstDef *constDef);

    void print(int tab);


};

class Block : public AstItem {
    std::vector<ConstDecl *> constDecls;
    std::vector<VarDecl *> varDecls;
    std::vector<Stmt *> stmts;
public:
    void addConstDecl(ConstDecl *constDecl);

    void addVarDecl(VarDecl *varDecl);

    void addStmt(Stmt *stmt);

    void print(int tab);
};

class Stmt : public AstItem {
public:
    virtual void print(int tab) {};
};

class LValStmt : public Stmt {
    LVal *lVal{};
    Exp *exp{};
public:
    void setLVal(LVal *lVal);

    void setExp(Exp *exp);

    void print(int tab) override;
};

class ExpStmt : public Stmt {
    Exp *exp{};
public:
    void setExp(Exp *exp);

    void print(int tab) override;


};

class BlockStmt : public Stmt {
    Block *block{};
public:
    void setBlock(Block *block);

    void print(int tab) override;


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


};

class WhileStmt : public Stmt {
    Cond *cond{};
    Stmt *stmt{};
public:
    void setCond(Cond *cond);

    void setWhileStmt(Stmt *stmt);

    void print(int tab) override;


};

class LoopStmt : public Stmt {
    Token *break_continue{};
public:
    void setToken(Token *token);

    void print(int tab) override;


};

class ReturnStmt : public Stmt {
    Exp *exp{};
public:
    void setReturnExp(Exp *exp);

    void print(int tab) override;


};

class GetintStmt : public Stmt {
    LVal *lVal{};
public:
    void setLVal(LVal *lVal);

    void print(int tab) override;


};

class PrintfStmt : public Stmt {
    Token *formatString;
    std::vector<Exp *> formatExps;
public:
    void setFormatString(Token *token);

    void addFormatExp(Exp *exp);

    void print(int tab) override;

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
        for (int index=1; index<formatString->getKey().size() - 1; index++) {
            char c = formatString->getKey()[index];
            if (c == '%' && formatString->getKey()[index+1] == 'd') {
                index ++;
                continue;
            } else if ( c == 32 || c == 33 || c >=40 && c <= 126) {
                continue;
            } else {
                return true;
            }
        }
        return false;
    }
};

class FuncFParam : public AstItem {
    Token *ident;
    int row;
    ConstExp *constExp;
public:
    void setFuncFParamIdent(Token *token);

    void setRow(int rowIn);

    void setConstExp(ConstExp *constExp);

    void print(int tab);

    Token *getIdent() {
        return ident;
    }

    int getRow() {
        return this->row;
    }
};

class FuncFParams : public AstItem {
    std::vector<FuncFParam *> funcFParams;
public:
    void addFuncFParam(FuncFParam *funcFParam);

    void print(int tab);

    std::vector<FuncFParam *> getFParams() {
        return this->funcFParams;
    }

    int getFParamNum() {
        return this->funcFParams.size();
    }
};

class FuncType : public AstItem {
    Token *funcType; // void or int
public:
    void setFuncType(Token *funcType);

    void print(int tab);

    bool isVoid() {
        return this->funcType->getIdentity() == VOIDTK;
    }
};

//TODO
class FuncRParams : public AstItem {
    std::vector<Exp *> paramExps;
public:
    void addParamsExp(Exp *exp);

    void print(int tab);

    int getRParamNum() {
        return this->paramExps.size();
    }

    std::vector<Exp *>& getRParams() {
        return this->paramExps;
    }
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

    void print(int tab);

    std::vector<FuncFParam *> getFParams() {
        return this->funcFParams->getFParams();
    }
};


class MainFuncDef : public AstItem {
    Block *block;
public:
    void setBlock(Block *block);

    void print(int tab);


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

    void print();

};

#endif //COMPILER_ASTBUILDER_H
