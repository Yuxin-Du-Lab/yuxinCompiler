//
// Created by yuxin on 2021/11/15.
//
#include <fstream>
#include "IRQuaternion.h"


std::ofstream IRFile("IRRes.txt");

void QuaternionItem::makeIR() {
    IRFile << "^default^" << std::endl;
}

void ConstVarDeclQ::makeIR() {
    IRFile
            << "const int "
            << this->name
            << "#"
            << this->scopeId
            << " = "
            << this->value
            << std::endl;
}

void ConstArrDeclQ::makeIR() {
    IRFile
            << "const arr "
            << this->name
            << "#"
            << this->scopeId
            << "["
            << this->row
            << "]"
            << std::endl;
}

void VarDeclQ::makeIR() {
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

void ArrDeclQ::makeIR() {
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

void FuncDeclQ::makeIR() {
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

void ParaDeclQ::makeIR() {
    IRFile
            << "para int "
            << this->name
            << "#"
            << this->scopeId
            << std::endl;
}

void ExpQ::makeIR() {
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

void VarAssignQ::makeIR() {
    IRFile
            << this->lVal
            << " = "
            << this->rVal
            << std::endl;
}

void ArrAssignQ::makeIR() {
    IRFile
            << this->name
            << "["
            << this->offset
            << "] = "
            << this->rVal
            << std::endl;
}

void GetArrQ::makeIR() {
    IRFile
            << this->lVal
            << " = "
            << this->name
            << "["
            << this->offset
            << "]"
            << std::endl;
}

void StackPushQ::makeIR() {
    IRFile
            << "^push "
            << this->rVal
            << std::endl;
}

void StackPopQ::makeIR() {
    IRFile
            << "^pop "
            << this->lVal
            << std::endl;
}

void FuncCallQ::makeIR() {
    IRFile
            << "^call func "
            << this->name
            << std::endl;
}

void GetRetQ::makeIR() {
    IRFile
            << this->lVal
            << " = ^ret"
            << std::endl;
}

void SetRetQ::makeIR() {
    IRFile
            << "^ret = "
            << this->rVal
            << std::endl;
}

void SetLabelQ::makeIR() {
    IRFile
            << this->label
            << " : "
            << std::endl;
}

void BranchQ::makeIR() {
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

void CallGetIntQ::makeIR() {
    IRFile
            << "^call GETINT"
            << std::endl;
}

void CallPrintQ::makeIR() {
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

void ExitQ::makeIR() {
    IRFile
            << "exit"
            << std::endl;
}