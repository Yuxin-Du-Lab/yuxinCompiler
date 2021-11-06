//
// Created by yuxin on 2021/11/5.
//

#include "../AST/ASTBuilder.h"
#include "../symbolTable/symbol.h"

int tmp = 0;

std::string toTmpVar(int t) {
    return "t" + std::to_string(t);
}

void printIR(std::string format) {
    std::cout << format << std::endl;
}

int CompUnit::makeIR() {
    for (auto iter: this->constDecls) {
        iter->makeIR();
    }
    for (auto iter : this->varDecls) {
        iter->makeIR();
    }

    this->mainFuncDef->makeIR();
}

/*
 * const var
 */
int ConstDecl::makeIR() {
    for (auto iter: this->constDefs) {
        iter->makeIR();
    }
}

int ConstDef::makeIR() {
    if (this->row == 0) {
        // simple value
        printIR("const int " + this->ident->getKey() + " = " + std::to_string(this->constInitVal->getValue()));
    } else if (this->row == 1) {
        printIR("arr const int " + this->ident->getKey() + "[" + std::to_string(this->getConstExp(0)->getConstValue()) + "]");
        this->constInitVal->makeIR(0, true, this->ident);
    } else if (this->row == 2) {
        printIR("arr const int " + this->ident->getKey() +
        "[" + std::to_string(this->getConstExp(0)->getConstValue()) + "]" +
        "[" + std::to_string(this->getConstExp(1)->getConstValue()) + "]");
        this->constInitVal->makeIR(0, true, this->ident);
    }
}

int ConstInitVal::makeIR(int index, bool isArr, Token *ident) {
    if (this->constExp != nullptr) {
        if (isArr) {
            printIR(ident->getKey() + "[" + std::to_string(index) + "] = " + std::to_string(this->constExp->getConstValue()));
            return index;
        } else {
            return this->constExp->makeIR();
        }
    } else {
        for (auto iter: this->constInitVals) {
            index = iter->makeIR(index, true, ident);
            index ++;
        }
        return --index;
    }
}

int ConstExp::makeIR() {
    return this->addExp->makeIR();
}

// exp
int AddExp::makeIR() {
    return this->root->makeIR();
}

int AddOpTree::makeIR() {
    if (getIsLeaf()) {
        return this->value->makeIR();
    } else {
        int leftT = this->branch[LeftChild]->makeIR();
        int rightT = this->branch[RightChild]->makeIR();
        int res = tmp++;
        printIR(toTmpVar(res) + " = " + toTmpVar(leftT) + " " + this->op->getKey() + " " + toTmpVar(rightT));
        return res;
    }
}

int MulExp::makeIR() {
    return this->root->makeIR();
}

int MulOpTree::makeIR() {
    if (getIsLeaf()) {
        return this->value->makeIR();
    } else {
        int leftT = this->branch[LeftChild]->makeIR();
        int rightT = this->branch[RightChild]->makeIR();
        int res = tmp++;
        printIR(toTmpVar(res) + " = " + toTmpVar(leftT) + " " + this->op->getKey() + " " + toTmpVar(rightT));
        return res;
    }
}

int PrimaryUnaryExp::makeIR() {
    return this->primaryExp->makeIR();
}

int UnaryUnaryExp::makeIR() {
    int tExp = this->unaryExp->makeIR();
    int resT = tmp++;
    printIR(toTmpVar(resT) + " = " + this->unaryOp->getOp()->getKey() + " " + toTmpVar(tExp));
    return resT;
}

int ExpPrimaryExp::makeIR() {
    return this->exp->makeIR();
}

int LValPrimaryExp::makeIR() {
    return this->lVal->makeIR();
}

int NumberPrimaryExp::makeIR() {
    int numT = tmp++;
    printIR(toTmpVar(numT) + " = " + std::to_string(this->number->getConstValue()));
    return numT;
}

/*
 * var
 */

int VarDecl::makeIR() {
    for (auto iter : this->varDefs) {
        iter->makeIR();
    }
}

int VarDef::makeIR() {
    if (this->row == 0) {
        if (this->hasInitVal) {
            int initT = this->initVal->makeIR();
            printIR("var int " + this->ident->getKey() + " = " + toTmpVar(initT));
            return initT;
        } else {
            printIR("var int " + this->ident->getKey());
        }
    } else {
        if (this->row == 1) {
            printIR("arr int " + this->ident->getKey() + "[" + std::to_string(this->constExps[0]->getConstValue())  + "]");
            if (hasInitVal) {
                this->initVal->makeIR(0, true, this->ident);
            }
        }
    }
}

int InitVal::makeIR(int index, bool isArr, Token *ident) {
    if (this->exp != nullptr) {
        if (isArr) {
            int initT = this->exp->makeIR();
            printIR(ident->getKey() + "[" + std::to_string(index) + "] = " + toTmpVar(initT));
            return index;
        } else {
            return this->exp->makeIR();
        }
    } else {
        for (auto iter: this->initVals) {
            index = iter->makeIR(index, true, ident);
            index ++;
        }
        return --index;
    }
}

int Exp::makeIR() {
    return this->addExp->makeIR();
}

/*
 * func def
 */

int FuncDef::makeIR() {

}

int FuncFParams::makeIR() {

}

int FuncFParam::makeIR() {

}

int FuncUnaryExp::makeIR() {

}

int FuncRParams::makeIR() {

}

/*
 * main func
 */

int MainFuncDef::makeIR() {
    return this->block->makeIR();
}

int Block::makeIR() {
    for (auto iter : this->blockItems) {
        iter->makeIR();
    }
}

// stmt
int LValStmt::makeIR() {

}

int ExpStmt::makeIR() {
    std::cout << "ExpStmt::makeIR() run" << std::endl;
}

int BlockStmt::makeIR() {

}

int IfStmt::makeIR() {

}

int WhileStmt::makeIR() {

}

int LoopStmt::makeIR() {

}

int ReturnStmt::makeIR() {

}

int GetintStmt::makeIR() {

}

int PrintfStmt::makeIR() {

}

// cond

int Cond::makeIR() {

}

int LOrExp::makeIR() {

}

int LAndExp::makeIR() {

}

int EqExp::makeIR() {

}

int EqOpTree::makeIR() {

}

int RelExp::makeIR() {

}

int RelOpTree::makeIR() {

}

int LVal::makeIR() {
    if (this->constValue == VALUE_ERROR) {
        if (this->dimension == 0) {
            int lValT = tmp++;
            printIR(toTmpVar(lValT) + " = " + this->ident->getKey());
            return lValT;
        } else if (this->dimension == 1) {
            int index1T = this->exps[0]->makeIR();
            int lValT = tmp++;
            printIR(toTmpVar(lValT) + " = " + this->ident->getKey() + "[" + toTmpVar(index1T) + "]");
            return lValT;
        } else if (this->dimension == 2) {
            int index1T = this->exps[0]->makeIR();
            int index2T = this->exps[1]->makeIR();
            int lValT = tmp++;
            printIR(toTmpVar(lValT) + " = " + this->ident->getKey() +
            "[" + toTmpVar(index1T) + "]" +
            "[" + toTmpVar(index2T) + "]"
            );
        }
    } else {
        int lValT = tmp++;
        printIR(toTmpVar(lValT) + " = " + std::to_string(this->getConstValue()));
        return lValT;
    }

}