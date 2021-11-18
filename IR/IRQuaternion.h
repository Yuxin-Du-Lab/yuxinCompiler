//
// Created by yuxin on 2021/11/11.
//

#ifndef COMPILER_IRQUATERNION_H
#define COMPILER_IRQUATERNION_H

#include "vector"
#include "iostream"
#include "unordered_map"

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
    bool isGlobal;
public:
    ConstVarDeclQ(std::string nameIn, int scopeIdIn, int init, bool isGlobal = false) {
        this->name = std::move(nameIn);
        this->scopeId = scopeIdIn;
        this->value = init;
        this->isGlobal = isGlobal;
    }

    std::string toString() {
        return this->name + "#" + std::to_string(this->scopeId);
    }

    void makeIR() override;

    void makeMips() override;
};

class ConstArrDeclQ : public QuaternionItem {
    std::string name;
    int scopeId;
    int row;
    std::vector<int> values;
    bool isGlobal;
public:
    ConstArrDeclQ(std::string nameIn, int scopeIdIn, int rowIn, std::vector<int> inits, bool isGlobal = false) {
        this->name = std::move(nameIn);
        this->scopeId = scopeIdIn;
        this->row = rowIn;
        this->values.insert(this->values.end(), inits.begin(), inits.end());
        this->isGlobal = isGlobal;
    }

    std::string toString() {
        return this->name + "#" + std::to_string(this->scopeId);
    }

    void makeIR() override;

    void makeMips() override;
};

class VarDeclQ : public QuaternionItem {
    std::string name;
    int scopeId;
    std::string rVal;
    bool isGlobal;
public:
    VarDeclQ(std::string nameIn, int scopeIdIn, std::string rValIn, bool isGlobal = false) {
        this->name = std::move(nameIn);
        this->scopeId = scopeIdIn;
        this->rVal = std::move(rValIn);
        this->isGlobal = isGlobal;
    }

    VarDeclQ(std::string nameIn, int scopeIdIn, bool isGlobal = false) {
        this->name = std::move(nameIn);
        this->scopeId = scopeIdIn;
    }

    std::string toString() {
        return this->name + "#" + std::to_string(this->scopeId);
    }

    void makeIR() override;

    void makeMips() override;
};

class ArrDeclQ : public QuaternionItem {
    std::string name;
    int scopeId;
    int row;
    bool isGlobal;
public:
    ArrDeclQ(std::string nameIn, int scopeIdIn, int rowIn, bool isGlobal = false) {
        this->name = std::move(nameIn);
        this->scopeId = scopeIdIn;
        this->row = rowIn;
        this->isGlobal = isGlobal;
    }

    std::string toString() {
        return this->name + "#" + std::to_string(this->scopeId);
    }

    void makeIR() override;

    void makeMips() override;
};

class FuncDeclQ : public QuaternionItem {
    std::string name;
    std::string type;
    int paraNum;
    std::unordered_map<std::string, int> name2offset4stack;
    std::unordered_map<std::string, int> name2size4stack;
public:
    FuncDeclQ(std::string nameIn, std::string typeIn, int paraNumIn) {
        this->name = std::move(nameIn);
        this->type = std::move(typeIn);
        this->paraNum = paraNumIn;
    }

    void makeIR() override;

    void initStacksInfo(std::unordered_map<std::string, int> name2offset4stack_init,
                        std::unordered_map<std::string, int> name2size4stack_init) {
        this->name2size4stack = name2size4stack_init;
        this->name2offset4stack = name2offset4stack_init;
    }

    int getStackSize() {
        int res = 0;
        for (auto iter : this->name2size4stack) {
            res += iter.second;
        }
        return res;
    }

    void makeMips() override;
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

    void makeMips() override;
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

    void makeMips() override;
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

    void makeMips() override;
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

    void makeMips() override;
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

    void makeMips() override;
};

class StackPushQ : public QuaternionItem {
    std::string rVal;
    bool isArg;
    int argNo;
public:
    StackPushQ(std::string valueIn, bool isArg = false, int argNo = -1) {
        this->rVal = std::move(valueIn);
        this->isArg = isArg;
        this->argNo = argNo;
    }

    void makeIR() override;

    void makeMips() override;
};

class StackPopQ : public QuaternionItem {
    std::string lVal;
public:
    StackPopQ(std::string lValIn) {
        this->lVal = lValIn;
    }

    void makeIR() override;

    void makeMips() override;
};

class FuncCallQ : public QuaternionItem {
    std::string name;
public:
    FuncCallQ(std::string nameIn) {
        this->name = std::move(nameIn);
    }

    void makeIR() override;

    void makeMips() override;
};

class GetRetQ : public QuaternionItem {
    std::string lVal;
public:
    GetRetQ(std::string lVal) {
        this->lVal = std::move(lVal);
    }

    void makeIR() override;

    void makeMips() override;
};

class SetRetQ : public QuaternionItem {
    std::string rVal;
public:
    SetRetQ(std::string rVal) {
        this->rVal = std::move(rVal);
    }

    void makeIR() override;

    void makeMips() override;
};

class SetLabelQ : QuaternionItem {
    std::string label;
public:
    SetLabelQ(std::string label) {
        this->label = std::move(label);
    }

    void makeIR() override;

    void makeMips() override;
};

class BranchQ : QuaternionItem {
    std::string op;
    std::string arg1;
    std::string arg2;
    std::string label;
    bool isFuncCall;
public:
    BranchQ(std::string op, std::string label, bool isFuncCall = false) {
        this->op = std::move(op);
        this->label = std::move(label);
        this->isFuncCall = isFuncCall;
    }

    BranchQ(std::string op, std::string arg1, std::string arg2, std::string label) {
        this->op = std::move(op);
        this->label = std::move(label);
        this->arg1 = std::move(arg1);
        this->arg2 = std::move(arg2);
        this->isFuncCall = false;
    }

    void makeIR() override;

    void makeMips() override;

    bool getIsFuncCall() {
        return this->isFuncCall;
    }
};

class CallGetIntQ : public QuaternionItem {
public:
    void makeIR() override;

    void makeMips() override;
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

    void makeMips() override;
};

class ExitQ : public QuaternionItem {
public:
    void makeIR() override;

    void makeMips() override;
};

#endif //COMPILER_IRQUATERNION_H
