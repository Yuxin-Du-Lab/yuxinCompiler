//
// Created by yuxin on 2021/11/11.
//

#ifndef COMPILER_IRQUATERNION_H
#define COMPILER_IRQUATERNION_H

#include "vector"
#include "iostream"


const std::string BRANCH_EQUAL = "beq";
const std::string JUMP = "j";
const std::string JUMP_AND_LINK = "jal";
const std::string JUMP_REGISTER = "jr";

class QuaternionItem {
public:
    virtual void makeIR();

    virtual void makeMips();
};

class ConstVarDeclQ : public QuaternionItem {
    std::string name;
    int scopeId;
    int value;
public:
    ConstVarDeclQ(std::string nameIn, int scopeIdIn, int init) {
        this->name = std::move(nameIn);
        this->scopeId = scopeIdIn;
        this->value = init;
    }

    void makeIR() override;

    void makeMips() override;
};

class ConstArrDeclQ : public QuaternionItem {
    std::string name;
    int scopeId;
    int row;
    std::vector<int> values;
public:
    ConstArrDeclQ(std::string nameIn, int scopeIdIn, int rowIn, std::vector<int> inits) {
        this->name = std::move(nameIn);
        this->scopeId = scopeIdIn;
        this->row = rowIn;
        this->values.insert(this->values.end(), inits.begin(), inits.end());
    }

    void makeIR() override;

    void makeMips() override;
};

class VarDeclQ : public QuaternionItem {
    std::string name;
    int scopeId;
    std::string rVal;
public:
    VarDeclQ(std::string nameIn, int scopeIdIn, std::string rValIn) {
        this->name = std::move(nameIn);
        this->scopeId = scopeIdIn;
        this->rVal = std::move(rValIn);
    }

    VarDeclQ(std::string nameIn, int scopeIdIn) {
        this->name = std::move(nameIn);
        this->scopeId = scopeIdIn;
    }

    void makeIR() override;

    void makeMips() override;
};

class ArrDeclQ : public QuaternionItem {
    std::string name;
    int scopeId;
    int row;
public:
    ArrDeclQ(std::string nameIn, int scopeIdIn, int rowIn) {
        this->name = std::move(nameIn);
        this->scopeId = scopeIdIn;
        this->row = rowIn;
    }

    void makeIR() override;
};

class FuncDeclQ : public QuaternionItem {
    std::string name;
    std::string type;
    int paraNum;
public:
    FuncDeclQ(std::string nameIn, std::string typeIn, int paraNumIn) {
        this->name = std::move(nameIn);
        this->type = std::move(typeIn);
        this->paraNum = paraNumIn;
    }

    void makeIR() override;
};

class ParaDeclQ : QuaternionItem {
    std::string name;
    int scopeId;
    int row;
public:
    ParaDeclQ(std::string nameIn, int scopeIdIn, int rowIn) {
        this->name = std::move(nameIn);
        this->scopeId = scopeIdIn;
        this->row = rowIn;
    }

    std::string toString() {
        return this->name + "#" + std::to_string(this->scopeId);
    }

    void makeIR() override;
};

class ExpQ : public QuaternionItem {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string res;
    int opRel;
public:
    ExpQ(std::string op, std::string arg1, std::string arg2, std::string res) {
        this->op = std::move(op);
        this->arg1 = std::move(arg1);
        this->arg2 = std::move(arg2);
        this->res = std::move(res);
        this->opRel = 2;
    }

    ExpQ(std::string op, std::string arg1, std::string res) {
        this->op = std::move(op);
        this->arg1 = std::move(arg1);
        this->res = std::move(res);
        this->opRel = 1;
    }

    void makeIR() override;
};

class VarAssignQ : public QuaternionItem {
    std::string lVal;
    std::string rVal;
public:
    VarAssignQ(std::string lValIn, std::string rValIn) {
        this->lVal = std::move(lValIn);
        this->rVal = std::move(rValIn);
    }

    void makeIR() override;
};

class ArrAssignQ : public QuaternionItem {
    std::string name;
    std::string offset;
    std::string rVal;
public:
    ArrAssignQ(std::string nameIn, std::string offset, std::string rValIn) {
        this->name = std::move(nameIn);
        this->offset = std::move(offset);
        this->rVal = std::move(rValIn);
    }

    void makeIR() override;
};

class GetArrQ : public QuaternionItem {
    std::string name;
    std::string offset;
    std::string lVal;
public:
    GetArrQ(std::string nameIn, std::string offset, std::string lValIn) {
        this->name = std::move(nameIn);
        this->offset = std::move(offset);
        this->lVal = std::move(lValIn);
    }

    void makeIR() override;
};

class StackPushQ : public QuaternionItem {
    std::string rVal;
public:
    StackPushQ(std::string valueIn) {
        this->rVal = std::move(valueIn);
    }

    void makeIR() override;
};

class StackPopQ : public QuaternionItem {
    std::string lVal;
public:
    StackPopQ(std::string lValIn) {
        this->lVal = lValIn;
    }

    void makeIR() override;
};

class FuncCallQ : public QuaternionItem {
    std::string name;
public:
    FuncCallQ(std::string nameIn) {
        this->name = std::move(nameIn);
    }

    void makeIR() override;
};

class GetRetQ : public QuaternionItem {
    std::string lVal;
public:
    GetRetQ(std::string lVal) {
        this->lVal = std::move(lVal);
    }

    void makeIR() override;
};

class SetRetQ : public QuaternionItem {
    std::string rVal;
public:
    SetRetQ(std::string rVal) {
        this->rVal = std::move(rVal);
    }

    void makeIR() override;
};

class SetLabelQ : QuaternionItem {
    std::string label;
public:
    SetLabelQ(std::string label) {
        this->label = std::move(label);
    }

    void makeIR() override;
};

class BranchQ : QuaternionItem {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string label;
public:
    BranchQ(std::string op, std::string label) {
        this->op = std::move(op);
        this->label = std::move(label);
    }

    BranchQ(std::string op, std::string arg1, std::string arg2, std::string label) {
        this->op = std::move(op);
        this->label = std::move(label);
        this->arg1 = std::move(arg1);
        this->arg2 = std::move(arg2);
    }

    void makeIR() override;
};

class CallGetIntQ : public QuaternionItem {
public:
    void makeIR() override;
};

class CallPrintQ : public QuaternionItem {
    std::string format;
    bool isPrintNum;
    int formatIndex;
public:
    CallPrintQ(std::string formatIn, bool isNum, int formatIndex = -1) {
        this->format = std::move(formatIn);
        this->isPrintNum = isNum;
        this->formatIndex = formatIndex;
    }

    void makeIR() override;
};

class ExitQ : public QuaternionItem {
public:
    void makeIR() override;
};

#endif //COMPILER_IRQUATERNION_H
