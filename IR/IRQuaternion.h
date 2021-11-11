//
// Created by yuxin on 2021/11/11.
//

#ifndef COMPILER_IRQUATERNION_H
#define COMPILER_IRQUATERNION_H

#include "iostream"
#include "vector"

const std::string BRANCH_EQUAL = "beq";
const std::string JUMP = "j";
const std::string JUMP_AND_LINK = "jal";

class QuaternionItem {

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
};

class VarDeclQ : public QuaternionItem {
    std::string name;
    int scopeId;
    std::string rVal{};
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
};

class ParaDeclQ : QuaternionItem {
    std::string name;
    int scopeId;
public:
    ParaDeclQ(std::string nameIn, int scopeIdIn) {
        this->name = std::move(nameIn);
        this->scopeId = scopeIdIn;
    }
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
};

class VarAssignQ : public QuaternionItem {
    std::string lVal;
    std::string rVal;
public:
    VarAssignQ(std::string lValIn, std::string rValIn) {
        this->lVal = std::move(lValIn);
        this->rVal = std::move(rValIn);
    }
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
};

class StackPushQ : public QuaternionItem {
    std::string value;
public:
    StackPushQ(std::string valueIn) {
        this->value = std::move(valueIn);
    }
};

class StackPopQ : public QuaternionItem {
    int popNum;
public:
    StackPopQ(int num) {
        this->popNum = num;
    }
};

class LoadRaFromStackQ : public QuaternionItem {

};

class FuncCallQ : public QuaternionItem {
    std::string name;
public:
    FuncCallQ(std::string nameIn) {
        this->name = std::move(nameIn);
    }
};

class GetRetQ : public QuaternionItem {
    std::string lVal;
public:
    GetRetQ(std::string lVal) {
        this->lVal = std::move(lVal);
    }
};

class SetRetQ : public QuaternionItem {
    std::string rVal;
public:
    SetRetQ(std::string rVal) {
        this->rVal = std::move(rVal);
    }
};

class SetLabelQ : QuaternionItem {
    std::string label;
public:
    SetLabelQ(std::string label) {
        this->label = std::move(label);
    }
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
};

class CallGetIntQ : public QuaternionItem {

};

class CallPrintQ : public QuaternionItem {
    std::string format;
    bool isPrintNum;
public:
    CallPrintQ(std::string formatIn, bool isNum) {
        this->format = std::move(formatIn);
        this->isPrintNum = isNum;
    }
};
#endif //COMPILER_IRQUATERNION_H
