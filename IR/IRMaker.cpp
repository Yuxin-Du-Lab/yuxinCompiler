//
// Created by yuxin on 2021/11/5.
//

#include "../AST/ASTBuilder.h"
#include "../symbolTable/symbol.h"
#include "unordered_map"
#include "IRmaker.h"

const std::string BRANCH_EQUAL = "beq";
const std::string BRANCH_NOT_EQUAL = "bne";
const std::string JUMP = "j";
const std::string JUMP_REG = "jr";

std::ofstream IRFile("IRRes.txt");
std::vector<int> while_heads;
std::vector<int> while_ends;
std::unordered_map<std::string, int> func2label;

int tmpVar = 0;
int labelCnt = 0;
bool inMain = false;

auto *table = new IRSymbolTable();

std::string toTmpVar(int t) {
    return "%" + std::to_string(t);
}

std::string toLabel(int labelIn, bool isDefine= false) {
    if (isDefine) {
        return "&" + std::to_string(labelIn) + ":";
    } else {
        return "&" + std::to_string(labelIn);
    }
}

void printIR(std::string format) {
    IRFile << format << std::endl;
}

int CompUnit::makeIR() {
    table->enterNewScope();
    printIR(".data");
    for (auto iter: this->constDecls) {
        iter->makeIR();
    }
    printIR(".text");
    for (auto iter : this->varDecls) {
        iter->makeIR();
    }
    std::vector<int> func_labels;
    for (auto iter : this->funcDefs) {
        int func_label = labelCnt++;
        func_labels.emplace_back(func_label);
        func2label.emplace(iter->getIdent()->getKey(), func_label);
    }

    inMain = true;
    this->mainFuncDef->makeIR();
    inMain = false;
    printIR("exit");

    int i=0;
    for (auto iter : this->funcDefs) {
        int funcLabel = func_labels[i++];
        printIR(toLabel(funcLabel, true));
        iter->makeIR();
    }
    table->exitScope();
    table->check();
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
        auto *item = new ConstVarIRItem(this->ident->getKey(), this->constInitVal->getValue());
        table->addItem4currentScope(item);

        printIR("const int " + this->ident->getKey() + " = " + std::to_string(this->constInitVal->getValue()));
    } else if (this->row == 1) {
        auto *item = new ConstArrIRItem(this->ident->getKey(), this->getConstExp(0)->getConstValue(), this->constInitVal->getValues());
        table->addItem4currentScope(item);

        printIR("arr const int " + this->ident->getKey() + "[" + std::to_string(this->getConstExp(0)->getConstValue()) + "]");
        this->constInitVal->makeIR(0, true, this->ident);
    } else if (this->row == 2) {
        auto *item = new ConstArrIRItem(this->ident->getKey(),
                                        this->getConstExp(0)->getConstValue() * this->getConstExp(1)->getConstValue(),
                                        this->constInitVal->getValues());
        table->addItem4currentScope(item);

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
        int res = tmpVar++;
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
        int res = tmpVar++;
        printIR(toTmpVar(res) + " = " + toTmpVar(leftT) + " " + this->op->getKey() + " " + toTmpVar(rightT));
        return res;
    }
}

int PrimaryUnaryExp::makeIR() {
    return this->primaryExp->makeIR();
}

int UnaryUnaryExp::makeIR() {
    int tExp = this->unaryExp->makeIR();
    int resT = tmpVar++;
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
    int numT = tmpVar++;
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
        auto *item = new VarIRItem(this->ident->getKey());
        table->addItem4currentScope(item);
    } else {
        if (this->row == 1) {
            auto *item = new ArrIRItem(this->ident->getKey(), this->getConstExp(0)->getConstValue());
            table->addItem4currentScope(item);

            printIR("arr int " + this->ident->getKey() + "[" + std::to_string(this->constExps[0]->getConstValue())  + "]");
            if (hasInitVal) {
                this->initVal->makeIR(0, true, this->ident);
            }
        } else if (this->row == 2) {
            auto *item = new ArrIRItem(this->ident->getKey(),
                                       this->getConstExp(0)->getConstValue() * this->getConstExp(1)->getConstValue());
            table->addItem4currentScope(item);

            printIR("arr int " + this->ident->getKey() +
            "[" + std::to_string(this->constExps[0]->getConstValue())  + "]" +
            "[" + std::to_string(this->constExps[1]->getConstValue())  + "]");
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
    auto *item = new FuncIRItem(this->ident->getKey(), this->funcFParams->getFParamNum());
    table->addItem4currentScope(item);

    table->enterNewScope();
    printIR(this->funcType->getToken()->getKey() + " " + this->ident->getKey() + "()");
    this->funcFParams->makeIR();
    this->block->makeIR(true);
    table->exitScope();
}

int FuncFParams::makeIR() {
    for (auto iter: this->funcFParams) {
        iter->makeIR();
    }
}

int FuncFParam::makeIR() {
    if (this->row == 0) {
        auto *item = new ParaVarIRItem(this->ident->getKey());
        table->addItem4currentScope(item);

        printIR("para int " + this->ident->getKey());
    } else if (this->row == 1) {
        auto *item = new ParaArrIRItem(this->ident->getKey());
        table->addItem4currentScope(item);

        printIR("para int arr " + this->ident->getKey() + "[]");
    } else if (this->row == 2) {
        auto *item = new ParaArrIRItem(this->getIdent()->getKey(), this->getRow2ConstExp()->getConstValue());
        table->addItem4currentScope(item);

        printIR("para int arr " + this->ident->getKey() + "[][" + std::to_string(this->getRow2()) + "]");
    }
}

int FuncUnaryExp::makeIR() {
    this->funcRParams->makeIR();
    printIR("call " + this->ident->getKey());
    if (func2label.find(this->ident->getKey()) != func2label.end()) {
        printIR(JUMP + " " + toLabel(func2label.find(this->ident->getKey())->second));
    }
    int ret = tmpVar++;
    printIR(toTmpVar(ret) + " = ret");
    return ret;
}

int FuncRParams::makeIR() {
    for (auto iter : this->paramExps) {
        int paraT = iter->makeIR();
        printIR("push " + toTmpVar(paraT));
    }
}

/*
 * main func
 */

int MainFuncDef::makeIR() {
    table->enterNewScope();
    int tmp = this->block->makeIR(true);
    table->exitScope();
    return tmp;
}

int Block::makeIR(bool isFuncBlock) {
    if (!isFuncBlock) {
        table->enterNewScope();
    }
    for (auto iter : this->blockItems) {
        iter->makeIR();
    }
    if (!isFuncBlock) {
        table->exitScope();
    }
}

int Stmt::makeIR() { return VALUE_ERROR; }

// stmt
int LValStmt::makeIR() {
    if (this->lVal->getDimension() == 0) {
        int expT = this->exp->makeIR();
        printIR(this->lVal->getIdent()->getKey() + " = " + toTmpVar(expT));
        return expT;
    } else if (this->lVal->getDimension() == 1) {
        int expT = this->exp->makeIR();
        int indexT = this->lVal->getExp(0)->makeIR();
        printIR(this->lVal->getIdent()->getKey() + "[" + toTmpVar(indexT) +  "]" + " = " + toTmpVar(expT));
        return indexT;
    } else if (this->lVal->getDimension() == 2) {
        int expT = this->exp->makeIR();
        int indexT1 = this->lVal->getExp(0)->makeIR();
        int row2 = this->lVal->getRow2();
        int t1 = tmpVar++;
        printIR(toTmpVar(t1) + " = " + toTmpVar(indexT1) + " * " + std::to_string(row2));
        int indexT2 = this->lVal->getExp(1)->makeIR();
        int indexT = tmpVar++;
        printIR(toTmpVar(indexT) + " = " + toTmpVar(t1) + " + " + toTmpVar(indexT2));
        printIR(this->lVal->getIdent()->getKey() + "[" + toTmpVar(indexT) +  "]" + " = " + toTmpVar(expT));
    }
}

int ExpStmt::makeIR() {
//    TODO inVain?
    if (this->exp != nullptr) {
        return this->exp->makeIR();
    }
}

int BlockStmt::makeIR() {
    this->block->makeIR();
}

int IfStmt::makeIR() {
    int if_label = labelCnt++;
    int else_label = labelCnt++;
    int end_label = labelCnt++;
    this->cond->makeIR(if_label, else_label, end_label);
    printIR(toLabel(if_label, true));
    this->ifStmt->makeIR();
    printIR(JUMP + " " + toLabel(end_label));
    printIR(toLabel(else_label, true));
    if (this->elseStmt!= nullptr) {
        this->elseStmt->makeIR();
    }
    printIR(toLabel(end_label, true));
}

int WhileStmt::makeIR() {
    int loop_head = labelCnt++;
    int while_stmt = labelCnt++;
    int while_end = labelCnt++;
    while_heads.emplace_back(loop_head);
    while_ends.emplace_back(while_end);
    printIR(toLabel(loop_head, true));
    this->cond->makeIR(while_stmt, while_end, while_end);
    printIR(toLabel(while_stmt, true));
    this->stmt->makeIR();
    printIR(JUMP + " " + toLabel(loop_head));
    printIR(toLabel(while_end, true));
    while_heads.pop_back();
    while_ends.pop_back();
}

int LoopStmt::makeIR() {
    if (this->break_continue->getIdentity() == CONTINUETK) {
        printIR(JUMP + " " + toLabel(while_heads[while_heads.size()-1]));
    } else {
        printIR(JUMP + " " + toLabel(while_ends[while_ends.size() - 1]));
    }
}

int ReturnStmt::makeIR() {
    if (this->exp != nullptr) {
        int expResT = this->exp->makeIR();
        printIR("ret = " + toTmpVar(expResT));
    } else {
        printIR("ret = null");
    }
    if (!inMain) {
        printIR(JUMP_REG + " $ra");
    }
}

int GetintStmt::makeIR() {
    if (this->lVal->getDimension() == 0) {
        printIR(this->lVal->getIdent()->getKey() + " = " + "GETINT CALL");
    } else if (this->lVal->getDimension() == 1) {
        int indexT = this->lVal->getExp(0)->makeIR();
        printIR(this->lVal->getIdent()->getKey() + "[" + toTmpVar(indexT) +  "]" + " = " + "GETINT CALL");
        return indexT;
    } else if (this->lVal->getDimension() == 2) {
        int indexT1 = this->lVal->getExp(0)->makeIR();
        int row2 = this->lVal->getRow2();
        int t1 = tmpVar++;
        printIR(toTmpVar(t1) + " = " + toTmpVar(indexT1) + " * " + std::to_string(row2));
        int indexT2 = this->lVal->getExp(1)->makeIR();
        int indexT = tmpVar++;
        printIR(toTmpVar(indexT) + " = " + toTmpVar(t1) + " + " + toTmpVar(indexT2));
        printIR(this->lVal->getIdent()->getKey() + "[" + toTmpVar(indexT) +  "]" + " = " + "GETINT CALL");
    }
}

int PrintfStmt::makeIR() {
    std::string origin = this->formatString->getKey().substr(1, this->formatString->getKey().size()-2);
    std::string buf;
    int begin=0, len=0;
    int varIndex=0;
    for (int i=0; i< origin.size(); i++) {
        if (origin[i] == '%') {
            buf = origin.substr(begin, len);
            if (buf.length() > 0) {
                printIR("print ^" + buf);
            }
            begin = i+2;
            len = 0;

            int varResT = this->formatExps[varIndex++]->makeIR();
            printIR("print " + toTmpVar(varResT));
            i++;
        } else {
            len++;
        }
    }
    buf = origin.substr(begin, len);
    if (buf.length() > 0) {
        printIR("print ^" + buf);
    }
}

// cond

int Cond::makeIR(int if_label, int else_label, int end_label) {
    return this->lOrExp->makeIR(if_label, else_label, end_label);
}

int LOrExp::makeIR(int if_label, int else_label, int end_label) {
    for (auto *iter : this->lAndExps) {
        int and_part_end = labelCnt++;
        iter->makeIR(and_part_end);
        printIR(JUMP + " " + toLabel(if_label));
        printIR(toLabel(and_part_end, true));
    }
    printIR(JUMP + " " + toLabel(else_label));
}

int LAndExp::makeIR(int and_part_end) {
    for (auto *iter : this->eqExps) {
        int eqResT = iter->makeIR();
        printIR(BRANCH_EQUAL + " " +
        toTmpVar(eqResT) + " " +
        "$0" + " " + toLabel(and_part_end));
    }
}

int EqExp::makeIR() {
    return this->root->makeIR();
}

int EqOpTree::makeIR() {
    if (this->isLeaf) {
        return this->value->makeIR();
    } else {
        int t1 = this->branch[LeftChild]->makeIR();
        int t2 = this->branch[RightChild]->makeIR();
        int resT = tmpVar++;
        printIR(toTmpVar(resT) + " = " + toTmpVar(t1) + this->op->getKey() + toTmpVar(t2));
        return resT;
    }
}

int RelExp::makeIR() {
    return this->root->makeIR();
}

int RelOpTree::makeIR() {
    if (this->isLeaf) {
        return this->value->makeIR();
    } else {
        int t1 = this->branch[LeftChild]->makeIR();
        int t2 = this->branch[RightChild]->makeIR();
        int resT = tmpVar++;
        printIR(toTmpVar(resT) + " = " + toTmpVar(t1) + this->op->getKey() + toTmpVar(t2));
        return resT;
    }
}

int LVal::makeIR() {
    if (this->constValue == VALUE_ERROR) {
        if (this->dimension == 0) {
            int lValT = tmpVar++;
            printIR(toTmpVar(lValT) + " = " + this->ident->getKey());
            return lValT;
        } else if (this->dimension == 1) {
            int index1T = this->exps[0]->makeIR();
            int lValT = tmpVar++;
            printIR(toTmpVar(lValT) + " = " + this->ident->getKey() + "[" + toTmpVar(index1T) + "]");
            return lValT;
        } else if (this->dimension == 2) {
            int index1T = this->exps[0]->makeIR();
            int index2T = this->exps[1]->makeIR();
            int lValT = tmpVar++;
            printIR(toTmpVar(lValT) + " = " + this->ident->getKey() +
            "[" + toTmpVar(index1T) + "]" +
            "[" + toTmpVar(index2T) + "]"
            );
        }
    } else {
        int lValT = tmpVar++;
        printIR(toTmpVar(lValT) + " = " + std::to_string(this->getConstValue()));
        return lValT;
    }

}