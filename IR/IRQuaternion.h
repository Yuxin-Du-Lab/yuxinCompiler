//
// Created by yuxin on 2021/11/11.
//

#ifndef COMPILER_IRQUATERNION_H
#define COMPILER_IRQUATERNION_H

#include "iostream"
#include "vector"

std::ofstream IRFile("IRRes.txt");

const std::string BRANCH_EQUAL = "beq";
const std::string JUMP = "j";
const std::string JUMP_AND_LINK = "jal";
const std::string JUMP_REGISTER = "jr";

class QuaternionItem {
public:
    virtual void makeIR() {
        IRFile << "^default^" << std::endl;
    };
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

    void makeIR() override {
        IRFile
                << "const int "
                << this->name
                << "#"
                << this->scopeId
                << " = "
                << this->value
                << std::endl;
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

    void makeIR() override {
        IRFile
                << "const arr "
                << this->name
                << "#"
                << this->scopeId
                << "["
                << this->row
                << "]"
                << std::endl;
//        int index = 0;
//        for (auto iter: this->values) {
//            IRFile
//                    << this->name
//                    << "#"
//                    << this->scopeId
//                    << "["
//                    << index
//                    << "]"
//                    << " = "
//                    << values[index]
//                    << std::endl;
//            index++;
//        }
    }
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

    void makeIR() override {
        if (this->rVal.empty()) {
            IRFile
                    << "int "
                    << this->name
                    << "#"
                    << this->scopeId
                    << std::endl;
        } else {
            IRFile
                    << "int "
                    << this->name
                    << "#"
                    << this->scopeId
                    << " = "
                    << this->rVal
                    << std::endl;
        }
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

    void makeIR() override {
        IRFile
                << "arr int "
                << this->name
                << "#"
                << this->scopeId
                << "["
                << this->row
                << "]"
                << std::endl;
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

    void makeIR() override {
        IRFile
                << "^func "
                << this->type
                << " "
                << this->name
                << "()"
                << " "
                << this->paraNum
                << std::endl;
    }
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

    void makeIR() override {
//        if (this->row == 0) {
//            IRFile
//                    << "para int "
//                    << this->name
//                    << "#"
//                    << this->scopeId
//                    << std::endl;
//        } else {
//            IRFile
//                    << "para arr "
//                    << this->name
//                    << "#"
//                    << this->scopeId
//                    << std::endl;
//        }
        IRFile
                    << "para int "
                    << this->name
                    << "#"
                    << this->scopeId
                    << std::endl;
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

    void makeIR() override {
        if (this->opRel == 2) {
            IRFile
                    << this->res
                    << " = "
                    << this->arg1
                    << " "
                    << this->op
                    << " "
                    << this->arg2
                    << std::endl;
        } else {
            IRFile
                    << this->res
                    << " = "
                    << this->op
                    << " "
                    << this->arg1
                    << std::endl;
        }
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

    void makeIR() override {
        IRFile
                << this->lVal
                << " = "
                << this->rVal
                << std::endl;
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

    void makeIR() override {
        IRFile
                << this->name
                << "["
                << this->offset
                << "] = "
                << this->rVal
                << std::endl;
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

    void makeIR() override {
        IRFile
                << this->lVal
                << " = "
                << this->name
                << "["
                << this->offset
                << "]"
                << std::endl;
    }
};

class StackPushQ : public QuaternionItem {
    std::string rVal;
public:
    StackPushQ(std::string valueIn) {
        this->rVal = std::move(valueIn);
    }

    void makeIR() override {
        IRFile
                << "^push "
                << this->rVal
                << std::endl;
    }
};

class StackPopQ : public QuaternionItem {
//    int popNum;
    std::string lVal;
public:
//    StackPopQ(int num) {
//        this->popNum = num;
//    }
    StackPopQ(std::string lValIn) {
        this->lVal = lValIn;
    }

    void makeIR() override {
        IRFile
                << "^pop "
//                << this->popNum
                << this->lVal
                << std::endl;
    }
};

//class LoadRaFromStackQ : public QuaternionItem {
//public:
//    void makeIR() override {
//        IRFile
//                << "^load ra from stack top"
//                << std::endl;
//    }
//};

class FuncCallQ : public QuaternionItem {
    std::string name;
public:
    FuncCallQ(std::string nameIn) {
        this->name = std::move(nameIn);
    }

    void makeIR() override {
        IRFile
                << "^call func "
                << this->name
                << std::endl;
    }
};

class GetRetQ : public QuaternionItem {
    std::string lVal;
public:
    GetRetQ(std::string lVal) {
        this->lVal = std::move(lVal);
    }

    void makeIR() override {
        IRFile
                << this->lVal
                << " = ^ret"
                << std::endl;
    }
};

class SetRetQ : public QuaternionItem {
    std::string rVal;
public:
    SetRetQ(std::string rVal) {
        this->rVal = std::move(rVal);
    }

    void makeIR() override {
        IRFile
                << "^ret = "
                << this->rVal
                << std::endl;
    }
};

class SetLabelQ : QuaternionItem {
    std::string label;
public:
    SetLabelQ(std::string label) {
        this->label = std::move(label);
    }

    void makeIR() override {
        IRFile
                << this->label
                << " : "
                << std::endl;
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

    void makeIR() override {
        if (this->op == BRANCH_EQUAL) {
            IRFile
                    << this->op
                    << " "
                    << this->arg1
                    << " "
                    << this->arg2
                    << " "
                    << this->label
                    << std::endl;
        } else {
            IRFile
                    << this->op
                    << " "
                    << this->label
                    << std::endl;
        }
    }
};

class CallGetIntQ : public QuaternionItem {
public:
    void makeIR() override {
        IRFile
                << "^call GETINT"
                << std::endl;
    }
};

class CallPrintQ : public QuaternionItem {
    std::string format;
    bool isPrintNum;
public:
    CallPrintQ(std::string formatIn, bool isNum) {
        this->format = std::move(formatIn);
        this->isPrintNum = isNum;
    }

    void makeIR() override {
        if (isPrintNum) {
            IRFile
                    << "^call print "
                    << this->format
                    << std::endl;
        } else {
            IRFile
                    << "^call print $"
                    << this->format
                    << std::endl;
        }

    }
};

class ExitQ : public QuaternionItem {
public:
    void makeIR() override {
        IRFile
                << "exit"
                << std::endl;
    }
};

#endif //COMPILER_IRQUATERNION_H
