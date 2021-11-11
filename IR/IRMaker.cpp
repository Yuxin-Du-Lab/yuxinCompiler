//
// Created by yuxin on 2021/11/5.
//

#include "../AST/ASTBuilder.h"
#include "../symbolTable/symbol.h"
#include "unordered_map"
#include "IRScope.h"
#include "IRQuaternion.h"

std::ofstream IRFile("IRRes.txt");
std::vector<int> while_heads;
std::vector<int> while_ends;
std::unordered_map<std::string, int> func2label;

int tmpVar = 0;
int labelCnt = 0;
bool inMain = false;

auto *table = new IRSymbolTable();
std::vector<QuaternionItem *> quaternions;

std::string toTmpVar(int t) {
    return "%" + std::to_string(t);
}

std::string toLabel(int labelIn, bool isDefine = false) {
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
    for (auto iter: this->varDecls) {
        iter->makeIR();
    }
    int mainLabel = labelCnt++;

    auto *jump2mainQ = new BranchQ(JUMP, toLabel(mainLabel));
    quaternions.emplace_back((QuaternionItem *) jump2mainQ);
    printIR(JUMP + toLabel(mainLabel));

//    std::vector<int> func_labels;
    for (auto iter: this->funcDefs) {
        int func_label = labelCnt++;
//        func_labels.emplace_back(func_label);
        func2label.emplace(iter->getIdent()->getKey(), func_label);
        auto *funcLabelQ = new SetLabelQ(toLabel(func_label));
        quaternions.emplace_back((QuaternionItem *) funcLabelQ);
        printIR(toLabel(func_label, true));
        iter->makeIR();
    }
//    int i = 0;
//    for (auto iter: this->funcDefs) {
//        int funcLabel = func_labels[i++];
//        auto *funcLabelQ = new SetLabelQ(toLabel(funcLabel));
//        quaternions.emplace_back((QuaternionItem *) funcLabelQ);
//
//        printIR(toLabel(funcLabel, true));
//        iter->makeIR();
//    }

    auto *mainLabelQ = new SetLabelQ(toLabel(mainLabel));
    quaternions.emplace_back((QuaternionItem *) mainLabelQ);
    printIR(toLabel(mainLabel, true));

    inMain = true;
    this->mainFuncDef->makeIR();
    inMain = false;
    printIR("exit");

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

        auto *constVarDeclQ = new ConstVarDeclQ(this->ident->getKey(), table->getCurrentScopeId(),
                                                this->constInitVal->getValue());
        quaternions.emplace_back((QuaternionItem *) constVarDeclQ);
        printIR("const int " + this->ident->getKey() + " = " + std::to_string(this->constInitVal->getValue()));
    } else if (this->row == 1) {
        auto *item = new ConstArrIRItem(this->ident->getKey(),
                                        this->constExps[0]->getConstValue(),
                                        this->constInitVal->getValues());
        table->addItem4currentScope(item);

        auto *constArrDeclQ = new ConstArrDeclQ(this->ident->getKey(), table->getCurrentScopeId(),
                                                this->getConstExp(0)->getConstValue(), this->constInitVal->getValues());
        quaternions.emplace_back((QuaternionItem *) constArrDeclQ);
        printIR("arr const int " + this->ident->getKey() + "[" + std::to_string(this->getConstExp(0)->getConstValue()) +
                "]");
        this->constInitVal->makeIR(0, true, this->ident);
    } else if (this->row == 2) {
        auto *item = new ConstArrIRItem(this->ident->getKey(),
                                        this->constExps[0]->getConstValue() * this->constExps[1]->getConstValue(),
                                        this->constInitVal->getValues(),
                                        this->getConstExp(1)->getConstValue()
        );
        table->addItem4currentScope(item);

        auto *constArrDeclQ = new ConstArrDeclQ(this->ident->getKey(),
                                                table->getCurrentScopeId(),
                                                this->getConstExp(0)->getConstValue() *
                                                this->getConstExp(1)->getConstValue(),
                                                this->constInitVal->getValues());
        quaternions.emplace_back((QuaternionItem *) constArrDeclQ);
        printIR("arr const int " + this->ident->getKey() +
                "[" + std::to_string(this->getConstExp(0)->getConstValue()) + "]" +
                "[" + std::to_string(this->getConstExp(1)->getConstValue()) + "]");
        this->constInitVal->makeIR(0, true, this->ident);
    }
}

int ConstInitVal::makeIR(int index, bool isArr, Token *ident) {
    if (this->constExp != nullptr) {
        if (isArr) {
            auto *arrAssignQ = new ArrAssignQ(ident->getKey(), std::to_string(index),
                                              std::to_string(this->constExp->getConstValue()));
            quaternions.emplace_back((QuaternionItem *) arrAssignQ);
            printIR(ident->getKey() + "[" + std::to_string(index) + "] = " +
                    std::to_string(this->constExp->getConstValue()));
            return index;
        } else {
            return this->constExp->makeIR();
        }
    } else {
        for (auto iter: this->constInitVals) {
            index = iter->makeIR(index, true, ident);
            index++;
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

        auto *expQ = new ExpQ(this->op->getKey(), toTmpVar(leftT), toTmpVar(rightT), toTmpVar(res));
        quaternions.emplace_back((QuaternionItem *) expQ);
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

        auto *expQ = new ExpQ(this->op->getKey(), toTmpVar(leftT), toTmpVar(rightT), toTmpVar(res));
        quaternions.emplace_back((QuaternionItem *) expQ);
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

    auto *expQ = new ExpQ(this->unaryOp->getOp()->getKey(), toTmpVar(tExp), toTmpVar(resT));
    quaternions.emplace_back((QuaternionItem *) expQ);
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

    auto *assignQ = new VarAssignQ(toTmpVar(numT), std::to_string(this->number->getConstValue()));
    quaternions.emplace_back((QuaternionItem *) assignQ);
    printIR(toTmpVar(numT) + " = " + std::to_string(this->number->getConstValue()));
    return numT;
}

/*
 * var
 */

int VarDecl::makeIR() {
    for (auto iter: this->varDefs) {
        iter->makeIR();
    }
}

int VarDef::makeIR() {
    if (this->row == 0) {
        if (this->hasInitVal) {
            int initT = this->initVal->makeIR();

            auto *varDeclQ = new VarDeclQ(this->ident->getKey(), table->getCurrentScopeId(), toTmpVar(initT));
            quaternions.emplace_back((QuaternionItem *) varDeclQ);
            printIR("var int " + this->ident->getKey() + " = " + toTmpVar(initT));
            return initT;
        } else {
            auto *varDeclQ = new VarDeclQ(this->ident->getKey(), table->getCurrentScopeId());
            quaternions.emplace_back((QuaternionItem *) varDeclQ);
            printIR("var int " + this->ident->getKey());
        }
        auto *item = new VarIRItem(this->ident->getKey());
        table->addItem4currentScope(item);
    } else {
        if (this->row == 1) {
            auto *item = new ArrIRItem(this->ident->getKey(), this->getConstExp(0)->getConstValue());
            table->addItem4currentScope(item);

            auto *arrDecl = new ArrDeclQ(this->ident->getKey(), table->getCurrentScopeId(),
                                         this->constExps[0]->getConstValue());
            quaternions.emplace_back((QuaternionItem *) arrDecl);
            printIR("arr int " + this->ident->getKey() + "[" + std::to_string(this->constExps[0]->getConstValue()) +
                    "]");
            if (hasInitVal) {
                this->initVal->makeIR(0, true, this->ident);
            }
        } else if (this->row == 2) {
            auto *item = new ArrIRItem(this->ident->getKey(),
                                       this->getConstExp(0)->getConstValue() * this->getConstExp(1)->getConstValue(),
                                       this->getConstExp(1)->getConstValue()
            );
            table->addItem4currentScope(item);

            auto *arrDecl = new ArrDeclQ(this->ident->getKey(),
                                         table->getCurrentScopeId(),
                                         this->constExps[0]->getConstValue() * this->constExps[1]->getConstValue());
            quaternions.emplace_back((QuaternionItem *) arrDecl);
            printIR("arr int " + this->ident->getKey() +
                    "[" + std::to_string(this->constExps[0]->getConstValue()) + "]" +
                    "[" + std::to_string(this->constExps[1]->getConstValue()) + "]");
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

            auto *arrAssign = new ArrAssignQ(ident->getKey(), std::to_string(index), toTmpVar(initT));
            quaternions.emplace_back((QuaternionItem *) arrAssign);
            printIR(ident->getKey() + "[" + std::to_string(index) + "] = " + toTmpVar(initT));
            return index;
        } else {
            return this->exp->makeIR();
        }
    } else {
        for (auto iter: this->initVals) {
            index = iter->makeIR(index, true, ident);
            index++;
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

    auto *funcDeclQ = new FuncDeclQ(this->ident->getKey(), this->funcType->getToken()->getKey(),
                                    this->funcFParams->getFParamNum());
    quaternions.emplace_back((QuaternionItem *) funcDeclQ);
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
    auto *paraDeclQ = new ParaDeclQ(this->ident->getKey(), table->getCurrentScopeId());
    quaternions.emplace_back((QuaternionItem *) paraDeclQ);
}

int FuncUnaryExp::makeIR() {
    this->funcRParams->makeIR();

    auto *pushStack = new StackPushQ("ra");
    quaternions.emplace_back((QuaternionItem *) pushStack);
    printIR("push ra");

    auto *callFunc = new FuncCallQ(this->ident->getKey());
    quaternions.emplace_back((QuaternionItem *) callFunc);
    printIR("call " + this->ident->getKey());

    if (func2label.find(this->ident->getKey()) != func2label.end()) {
        auto *jump = new BranchQ(JUMP, toLabel(func2label.find(this->ident->getKey())->second));
        quaternions.emplace_back((QuaternionItem *) jump);
        printIR(JUMP_AND_LINK + " " + toLabel(func2label.find(this->ident->getKey())->second));
    }
    int ret = tmpVar++;
    auto *getRet = new GetRetQ(toTmpVar(ret));
    quaternions.emplace_back((QuaternionItem *) getRet);
    printIR(toTmpVar(ret) + " = ret");

    auto *loadRaQ = new LoadRaFromStackQ();
    quaternions.emplace_back((QuaternionItem *) loadRaQ);
    printIR("load ra from stackTop");


    auto *funcIRItem = (FuncIRItem *) table->findItemFromTable(this->ident->getKey());
    auto *popStackQ = new StackPopQ(funcIRItem->getParaNum() + 1);
    quaternions.emplace_back((QuaternionItem *) popStackQ);
    printIR("pop stack " + std::to_string(funcIRItem->getParaNum() + 1));

    return ret;
}

int FuncRParams::makeIR() {
    for (auto iter: this->paramExps) {
        int paraT = iter->makeIR();

        auto *pushStack = new StackPushQ(toTmpVar(paraT));
        quaternions.emplace_back((QuaternionItem *) pushStack);
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
    for (auto iter: this->blockItems) {
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

        auto *assignQ = new VarAssignQ(this->lVal->getIdent()->getKey(), toTmpVar(expT));
        quaternions.emplace_back((QuaternionItem *) assignQ);
        printIR(this->lVal->getIdent()->getKey() + " = " + toTmpVar(expT));
        return expT;
    } else if (this->lVal->getDimension() == 1) {
        int expT = this->exp->makeIR();
        int indexT = this->lVal->getExp(0)->makeIR();

        auto *arrAssignQ = new ArrAssignQ(this->lVal->getIdent()->getKey(), toTmpVar(indexT), toTmpVar(expT));
        quaternions.emplace_back((QuaternionItem *) arrAssignQ);
        printIR(this->lVal->getIdent()->getKey() + "[" + toTmpVar(indexT) + "]" + " = " + toTmpVar(expT));
        return indexT;
    } else if (this->lVal->getDimension() == 2) {
        int expT = this->exp->makeIR();
        int indexT1 = this->lVal->getExp(0)->makeIR();
        int row2 = this->lVal->getRow2();
        int t1 = tmpVar++;

        auto *expQ = new ExpQ("*", toTmpVar(indexT1), std::to_string(row2), toTmpVar(t1));
        quaternions.emplace_back((QuaternionItem *) expQ);
        printIR(toTmpVar(t1) + " = " + toTmpVar(indexT1) + " * " + std::to_string(row2));
        int indexT2 = this->lVal->getExp(1)->makeIR();
        int indexT = tmpVar++;

        auto *expQ1 = new ExpQ("+", toTmpVar(t1), toTmpVar(indexT2), toTmpVar(indexT));
        quaternions.emplace_back((QuaternionItem *) expQ1);
        printIR(toTmpVar(indexT) + " = " + toTmpVar(t1) + " + " + toTmpVar(indexT2));

        auto *arrAssign = new ArrAssignQ(this->lVal->getIdent()->getKey(), toTmpVar(indexT), toTmpVar(expT));
        quaternions.emplace_back((QuaternionItem *) arrAssign);
        printIR(this->lVal->getIdent()->getKey() + "[" + toTmpVar(indexT) + "]" + " = " + toTmpVar(expT));
    }
}

int ExpStmt::makeIR() {
////    TODO inVain?
//    if (this->exp != nullptr) {
//        return this->exp->makeIR();
//    }
}

int BlockStmt::makeIR() {
    this->block->makeIR();
}

int IfStmt::makeIR() {
    int if_label = labelCnt++;
    int else_label = labelCnt++;
    int end_label = labelCnt++;
    this->cond->makeIR(if_label, else_label, end_label);

    auto *setLabel = new SetLabelQ(toLabel(if_label));
    quaternions.emplace_back((QuaternionItem *) setLabel);
    printIR(toLabel(if_label, true));

    this->ifStmt->makeIR();

    auto *jumpQ = new BranchQ(JUMP, toLabel(end_label));
    quaternions.emplace_back((QuaternionItem *) jumpQ);
    printIR(JUMP + " " + toLabel(end_label));

    auto *setLabel1 = new SetLabelQ(toLabel(else_label));
    quaternions.emplace_back((QuaternionItem *) setLabel1);
    printIR(toLabel(else_label, true));

    if (this->elseStmt != nullptr) {
        this->elseStmt->makeIR();
    }

    auto *setLabel2 = new SetLabelQ(toLabel(end_label));
    quaternions.emplace_back((QuaternionItem *) setLabel2);
    printIR(toLabel(end_label, true));
}

int WhileStmt::makeIR() {
    int loop_head = labelCnt++;
    int while_stmt = labelCnt++;
    int while_end = labelCnt++;
    while_heads.emplace_back(loop_head);
    while_ends.emplace_back(while_end);

    auto *setLabel = new SetLabelQ(toLabel(loop_head));
    quaternions.emplace_back((QuaternionItem *) setLabel);
    printIR(toLabel(loop_head, true));

    this->cond->makeIR(while_stmt, while_end, while_end);

    auto *setLabel1 = new SetLabelQ(toLabel(while_stmt));
    quaternions.emplace_back((QuaternionItem *) setLabel1);
    printIR(toLabel(while_stmt, true));


    this->stmt->makeIR();

    auto *jumpQ = new BranchQ(JUMP, toLabel(loop_head));
    quaternions.emplace_back((QuaternionItem *) jumpQ);
    printIR(JUMP + " " + toLabel(loop_head));

    auto *setLabel2 = new SetLabelQ(toLabel(while_end));
    quaternions.emplace_back((QuaternionItem *) setLabel2);
    printIR(toLabel(while_end, true));

    while_heads.pop_back();
    while_ends.pop_back();
}

int LoopStmt::makeIR() {
    if (this->break_continue->getIdentity() == CONTINUETK) {
        auto *jumpQ = new BranchQ(JUMP, toLabel(while_heads[while_heads.size() - 1]));
        quaternions.emplace_back((QuaternionItem *) jumpQ);
        printIR(JUMP + " " + toLabel(while_heads[while_heads.size() - 1]));
    } else {
        auto *jumpQ = new BranchQ(JUMP, toLabel(while_ends[while_ends.size() - 1]));
        quaternions.emplace_back((QuaternionItem *) jumpQ);
        printIR(JUMP + " " + toLabel(while_ends[while_ends.size() - 1]));
    }
}

int ReturnStmt::makeIR() {
    if (this->exp != nullptr) {
        int expResT = this->exp->makeIR();

        auto *setRetQ = new SetRetQ(toTmpVar(expResT));
        quaternions.emplace_back((QuaternionItem *) setRetQ);
        printIR("ret = " + toTmpVar(expResT));
    }
    if (!inMain) {
        printIR(JUMP + " ra");
    }
}

int GetintStmt::makeIR() {
    auto *getIntCallQ = new CallGetIntQ();
    quaternions.emplace_back((QuaternionItem *) getIntCallQ);
    printIR("^Syscall_GetInt");

    if (this->lVal->getDimension() == 0) {
        auto *varAssign = new VarAssignQ(this->lVal->getIdent()->getKey(), "^GETINT_RET");
        quaternions.emplace_back((QuaternionItem *) varAssign);
        printIR(this->lVal->getIdent()->getKey() + " = " + "^GETINT_RET");
    } else if (this->lVal->getDimension() == 1) {
        int indexT = this->lVal->getExp(0)->makeIR();

        auto *arrAssign = new ArrAssignQ(this->lVal->getIdent()->getKey(), toTmpVar(indexT), "^GETINT_RET");
        quaternions.emplace_back((QuaternionItem *) arrAssign);
        printIR(this->lVal->getIdent()->getKey() + "[" + toTmpVar(indexT) + "]" + " = " + "^GETINT_RET");
        return indexT;
    } else if (this->lVal->getDimension() == 2) {
        int indexT1 = this->lVal->getExp(0)->makeIR();
        int row2 = this->lVal->getRow2();
        int t1 = tmpVar++;

        auto *expQ2 = new ExpQ("*", toTmpVar(indexT1), std::to_string(row2), toTmpVar(t1));
        quaternions.emplace_back((QuaternionItem *) expQ2);
        printIR(toTmpVar(t1) + " = " + toTmpVar(indexT1) + " * " + std::to_string(row2));
        int indexT2 = this->lVal->getExp(1)->makeIR();

        int indexT = tmpVar++;

        auto *expQ1 = new ExpQ("+", toTmpVar(t1), toTmpVar(indexT2), toTmpVar(indexT));
        quaternions.emplace_back((QuaternionItem *) expQ1);
        printIR(toTmpVar(indexT) + " = " + toTmpVar(t1) + " + " + toTmpVar(indexT2));

        auto *arrAssign = new ArrAssignQ(this->lVal->getIdent()->getKey(), toTmpVar(indexT), "^GETINT_RET");
        quaternions.emplace_back((QuaternionItem *) arrAssign);
        printIR(this->lVal->getIdent()->getKey() + "[" + toTmpVar(indexT) + "]" + " = " + "^GETINT_RET");
    }
}

int PrintfStmt::makeIR() {
    std::string origin = this->formatString->getKey().substr(1, this->formatString->getKey().size() - 2);
    std::string buf;
    int begin = 0, len = 0;
    int varIndex = 0;
    for (int i = 0; i < origin.size(); i++) {
        if (origin[i] == '%') {
            buf = origin.substr(begin, len);
            if (buf.length() > 0) {
                auto *callPrint = new CallPrintQ(buf, false);
                quaternions.emplace_back((QuaternionItem *) callPrint);
                printIR("print ^" + buf);
            }
            begin = i + 2;
            len = 0;

            int varResT = this->formatExps[varIndex++]->makeIR();

            auto *callPrint = new CallPrintQ(toTmpVar(varResT), true);
            quaternions.emplace_back((QuaternionItem *) callPrint);
            printIR("print " + toTmpVar(varResT));
            i++;
        } else {
            len++;
        }
    }
    buf = origin.substr(begin, len);
    if (buf.length() > 0) {
        auto *callPrint = new CallPrintQ(buf, false);
        quaternions.emplace_back((QuaternionItem *) callPrint);
        printIR("print ^" + buf);
    }
}

// cond

int Cond::makeIR(int if_label, int else_label, int end_label) {
    return this->lOrExp->makeIR(if_label, else_label, end_label);
}

int LOrExp::makeIR(int if_label, int else_label, int end_label) {
    for (auto *iter: this->lAndExps) {
        int and_part_end = labelCnt++;
        iter->makeIR(and_part_end);

        auto *jumpQ = new BranchQ(JUMP, toLabel(if_label));
        quaternions.emplace_back((QuaternionItem *) jumpQ);
        printIR(JUMP + " " + toLabel(if_label));

        auto *setLabelQ = new SetLabelQ(toLabel(and_part_end));
        quaternions.emplace_back((QuaternionItem *) setLabelQ);
        printIR(toLabel(and_part_end, true));
    }
    printIR(JUMP + " " + toLabel(else_label));
}

int LAndExp::makeIR(int and_part_end) {
    for (auto *iter: this->eqExps) {
        int eqResT = iter->makeIR();

        auto *beqQ = new BranchQ(BRANCH_EQUAL, toTmpVar(eqResT), "$0", toLabel(and_part_end));
        quaternions.emplace_back((QuaternionItem *) beqQ);
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

        auto *expQ = new ExpQ(this->op->getKey(), toTmpVar(t1), toTmpVar(t2), toTmpVar(resT));
        quaternions.emplace_back((QuaternionItem *) expQ);
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

        auto *expQ = new ExpQ(this->op->getKey(), toTmpVar(t1), toTmpVar(t2), toTmpVar(resT));
        quaternions.emplace_back((QuaternionItem *) expQ);
        printIR(toTmpVar(resT) + " = " + toTmpVar(t1) + this->op->getKey() + toTmpVar(t2));
        return resT;
    }
}

int LVal::makeIR() {
    auto *item = table->findItemFromTable(this->ident->getKey());
    if (item != nullptr) {
        if (item->getType() == ConstVarIRItemType) {
            int lValT = tmpVar++;
            printIR(toTmpVar(lValT) + " = " + std::to_string(((ConstVarIRItem*)item)->getValue()));
            return lValT;
        }
    }
    if (this->constValue == VALUE_ERROR) {
        if (this->dimension == 0) {
            int lValT = tmpVar++;

            auto *varAssignQ = new VarAssignQ(toTmpVar(lValT), this->ident->getKey());
            quaternions.emplace_back((QuaternionItem *) varAssignQ);
            printIR(toTmpVar(lValT) + " = " + this->ident->getKey());
            return lValT;
        } else if (this->dimension == 1) {
            int index1T = this->exps[0]->makeIR();
            int lValT = tmpVar++;

            auto *getArrQ = new GetArrQ(this->ident->getKey(), toTmpVar(index1T), toTmpVar(lValT));
            quaternions.emplace_back((QuaternionItem *) getArrQ);
            printIR(toTmpVar(lValT) + " = " + this->ident->getKey() + "[" + toTmpVar(index1T) + "]");
            return lValT;
        } else if (this->dimension == 2) {
            auto *item = table->findItemFromTable(this->ident->getKey());
            int row_2 = -1;
            if (item != nullptr) {
                if (item->getType() == ArrIRItemType) {
                    row_2 = ((ArrIRItem *) item)->getRow_2();
                } else if (item->getType() == ConstArrIRItemType) {
                    row_2 = ((ConstArrIRItem *) item)->getRow_2();
                }
            }
            int index1T = this->exps[0]->makeIR();
            int t1 = tmpVar++;

            auto *expQ1 = new ExpQ("*", toTmpVar(index1T), std::to_string(row_2), toTmpVar(t1));
            quaternions.emplace_back((QuaternionItem*)expQ1);
            printIR(toTmpVar(t1) + " = " + toTmpVar(index1T) + " * " + std::to_string(row_2));

            int index2T = this->exps[1]->makeIR();
            int index = tmpVar++;

            auto *expQ2 = new ExpQ("+", toTmpVar(t1), toTmpVar(index2T), toTmpVar(index));
            quaternions.emplace_back((QuaternionItem*)expQ2);
            printIR(toTmpVar(index) + " = " + toTmpVar(t1) + " + " + toTmpVar(index2T));


            int lValT = tmpVar++;

            auto *getArr = new GetArrQ(this->ident->getKey(), toTmpVar(index), toTmpVar(lValT));
            quaternions.emplace_back((QuaternionItem*)getArr);
            printIR(toTmpVar(lValT) + " = " + this->ident->getKey() +
                    "[" + toTmpVar(index) + "]"
            );
            return lValT;
        }
    } else {
        int lValT = tmpVar++;
        auto *assignQ = new VarAssignQ(toTmpVar(lValT), std::to_string(this->getConstValue()));
        quaternions.emplace_back((QuaternionItem*)assignQ);
        printIR(toTmpVar(lValT) + " = " + std::to_string(this->getConstValue()));
        return lValT;
    }
}