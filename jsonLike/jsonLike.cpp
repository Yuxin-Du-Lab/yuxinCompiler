//
// Created by yuxin on 2021/10/16.
//

#include "../AST/ASTBuilder.h"

std::ofstream ASTFile("ASTRes.txt");

void jsonLikePrint(const std::string &value, int tab) {
    std::string tabString;
    while (tab > 0) {
        tabString += "\t";
        tab--;
    }
    ASTFile << tabString << value << std::endl;
}

void CompUnit::print(int tab) {
    jsonLikePrint("CompUnit", tab);
    jsonLikePrint("{", tab);
    for (auto iter: this->constDecls) {
        iter->print(tab + 1);
    }
    for (auto iter: this->varDecls) {
        iter->print(tab + 1);
    }
    for (auto iter: this->funcDefs) {
        iter->print(tab + 1);
    }
    this->mainFuncDef->print(tab + 1);
    jsonLikePrint("}", tab);
}

void ConstDecl::print(int tab) {
    jsonLikePrint("ConstDecl", tab);
    jsonLikePrint("{", tab);
    for (auto iter: this->constDefs) {
        iter->print(tab + 1);
    }
    jsonLikePrint("}", tab);
}

void VarDecl::print(int tab) {
    jsonLikePrint("VarDecl", tab);
    jsonLikePrint("{", tab);

    for (auto iter: this->varDefs) {
        iter->print(tab + 1);
    }

    jsonLikePrint("}", tab);
}

void MainFuncDef::print(int tab) {
    jsonLikePrint("MainFuncDef", tab);
    jsonLikePrint("{", tab);

    this->block->print(tab + 1);

    jsonLikePrint("}", tab);
}

void ConstDef::print(int tab) {
    jsonLikePrint("ConstDef", tab);
    jsonLikePrint("{", tab);

    jsonLikePrint(this->ident->toString(), tab + 1);
    jsonLikePrint("usedDimension: " + std::to_string(this->row), tab + 1);
    if (row >= 1) {
        this->constExps[0]->print(tab + 1);
    }
    if (row >= 2) {
        this->constExps[1]->print(tab + 1);
    }
    this->constInitVal->print(tab + 1);

    jsonLikePrint("}", tab);
}

void VarDef::print(int tab) {
    jsonLikePrint("VarDef", tab);
    jsonLikePrint("{", tab);

    jsonLikePrint(this->ident->toString(), tab + 1);
    jsonLikePrint("usedDimension: " + std::to_string(this->row), tab + 1);
    if (row >= 1) {
        this->constExps[0]->print(tab + 1);
    }
    if (row >= 2) {
        this->constExps[1]->print(tab + 1);
    }
    if (this->hasInitVal) {
        this->initVal->print(tab + 1);
    }
    jsonLikePrint("}", tab);
}

void ConstExp::print(int tab) {
    jsonLikePrint("ConstExp", tab);
    jsonLikePrint("{", tab);

    this->addExp->print(tab + 1);

    jsonLikePrint("}", tab);
}

void InitVal::print(int tab) {
    jsonLikePrint("InitVal", tab);
    jsonLikePrint("{", tab);

    jsonLikePrint("usedDimension: " + std::to_string(this->row), tab + 1);
    if (this->row == 0) {
        this->exp->print(tab + 1);
    } else {
        for (auto iter: this->initVals) {
            iter->print(tab + 1);
        }
    }

    jsonLikePrint("}", tab);
}

void ConstInitVal::print(int tab) {
    jsonLikePrint("ConstInitVal", tab);
    jsonLikePrint("{", tab);

    if (this->row == 0 && this->constExp != nullptr) {
        this->constExp->print(tab + 1);
    } else {
        for (auto iter: this->constInitVals) {
            iter->print(tab + 1);
        }
    }
    jsonLikePrint("}", tab);
}

void Block::print(int tab) {
    jsonLikePrint("Block", tab);
    jsonLikePrint("{", tab);

//    for (auto iter: this->constDecls) {
//        iter->print(tab + 1);
//    }
//
//    for (auto iter: this->varDecls) {
//        iter->print(tab + 1);
//    }
//
//    for (auto iter: this->stmts) {
//        iter->print(tab + 1);
//    }

    for (auto iter: this->blockItems) {
        iter->print(tab + 1);
    }

    jsonLikePrint("}", tab);
}

void AddExp::print(int tab) {
    jsonLikePrint("AddExp", tab);
    jsonLikePrint("{", tab);

    this->root->print(tab + 1);

    jsonLikePrint("}", tab);
}

void Exp::print(int tab) {
    jsonLikePrint("Exp", tab);
    jsonLikePrint("{", tab);

    this->addExp->print(tab + 1);

    jsonLikePrint("}", tab);
}

void AddOpTree::print(int tab) {
    jsonLikePrint("AddOpTree", tab);
    jsonLikePrint("{", tab);

    if (this->isLeaf) {
        jsonLikePrint("isLeaf: true", tab + 1);
        this->value->print(tab + 1);
    } else {
        jsonLikePrint("isLeaf: false", tab + 1);
        jsonLikePrint(this->op->toString(), tab + 1);
        this->branch[0]->print(tab + 1);
        this->branch[1]->print(tab + 1);
    }

    jsonLikePrint("}", tab);
}

void MulExp::print(int tab) {
    jsonLikePrint("MulExp", tab);
    jsonLikePrint("{", tab);

    this->root->print(tab + 1);

    jsonLikePrint("}", tab);
}

void MulOpTree::print(int tab) {
    jsonLikePrint("MulOpTree", tab);
    jsonLikePrint("{", tab);

    if (this->isLeaf) {
        jsonLikePrint("isLeaf: true", tab + 1);
        this->value->print(tab + 1);
    } else {
        jsonLikePrint("isLeaf: false", tab + 1);
        jsonLikePrint(this->op->toString(), tab + 1);
        this->branch[0]->print(tab + 1);
        this->branch[1]->print(tab + 1);
    }

    jsonLikePrint("}", tab);
}

void PrimaryUnaryExp::print(int tab) {
    jsonLikePrint("PrimaryUnaryExp", tab);
    jsonLikePrint("{", tab);

    this->primaryExp->print(tab + 1);

    jsonLikePrint("}", tab);
}

void FuncUnaryExp::print(int tab) {
    jsonLikePrint("FuncUnaryExp", tab);
    jsonLikePrint("{", tab);

    jsonLikePrint(this->ident->toString(), tab + 1);
    if (this->funcRParams != nullptr) {
        this->funcRParams->print(tab + 1);
    }

    jsonLikePrint("}", tab);
}

void UnaryUnaryExp::print(int tab) {
    jsonLikePrint("UnaryUnaryExp", tab);
    jsonLikePrint("{", tab);

    this->unaryOp->print(tab + 1);
    this->unaryExp->print(tab + 1);

    jsonLikePrint("}", tab);
}

void FuncRParams::print(int tab) {
    jsonLikePrint("FuncRParams", tab);
    jsonLikePrint("{", tab);

    for (auto iter: this->paramExps) {
        iter->print(tab + 1);
    }

    jsonLikePrint("}", tab);
}

void UnaryOp::print(int tab) {
    jsonLikePrint("UnaryOp", tab);
    jsonLikePrint("{", tab);

    jsonLikePrint(this->op->toString(), tab + 1);

    jsonLikePrint("}", tab);
}


void ExpPrimaryExp::print(int tab) {
    jsonLikePrint("ExpPrimaryExp", tab);
    jsonLikePrint("{", tab);

    this->exp->print(tab + 1);

    jsonLikePrint("}", tab);
}


void LValPrimaryExp::print(int tab) {
    jsonLikePrint("LValPrimaryExp", tab);
    jsonLikePrint("{", tab);

    this->lVal->print(tab + 1);

    jsonLikePrint("}", tab);
}

void NumberPrimaryExp::print(int tab) {
    jsonLikePrint("NumberPrimaryExp", tab);
    jsonLikePrint("{", tab);

    this->number->print(tab + 1);

    jsonLikePrint("}", tab);
}

void Number::print(int tab) {
    jsonLikePrint("Number", tab);
    jsonLikePrint("{", tab);

    jsonLikePrint(this->intConst->toString(), tab + 1);

    jsonLikePrint("}", tab);
}


void LVal::print(int tab) {
    jsonLikePrint("LVal", tab);
    jsonLikePrint("{", tab);

    jsonLikePrint(this->ident->toString(), tab + 1);
    if (this->usedDimension >= 1) {
        this->exps[0]->print(tab + 1);
    }
    if (this->usedDimension >= 2) {
        this->exps[1]->print(tab + 1);
    }

    jsonLikePrint("}", tab);
}

void LValStmt::print(int tab) {
    jsonLikePrint("LValStmt", tab);
    jsonLikePrint("{", tab);

    this->lVal->print(tab + 1);
    this->exp->print(tab + 1);

    jsonLikePrint("}", tab);
}

void ExpStmt::print(int tab) {
    jsonLikePrint("ExpStmt", tab);
    jsonLikePrint("{", tab);

    if (this->exp != nullptr) {
        this->exp->print(tab + 1);
    } else {
        jsonLikePrint("null exp", tab + 1);
    }

    jsonLikePrint("}", tab);
}

void BlockStmt::print(int tab) {
    jsonLikePrint("BlockStmt", tab);
    jsonLikePrint("{", tab);

    this->block->print(tab + 1);

    jsonLikePrint("}", tab);
}

void IfStmt::print(int tab) {
    jsonLikePrint("IfStmt", tab);
    jsonLikePrint("{", tab);

    this->cond->print(tab + 1);
    this->ifStmt->print(tab + 1);
    if (this->elseStmt != nullptr) {
        this->elseStmt->print(tab + 1);
    } else {
        jsonLikePrint("not else", tab + 1);
    }

    jsonLikePrint("}", tab);
}

void WhileStmt::print(int tab) {
    jsonLikePrint("WhileStmt", tab);
    jsonLikePrint("{", tab);

    this->cond->print(tab + 1);
    this->stmt->print(tab + 1);

    jsonLikePrint("}", tab);
}

void LoopStmt::print(int tab) {
    jsonLikePrint("LoopStmt", tab);
    jsonLikePrint("{", tab);

    jsonLikePrint(this->break_continue->toString(), tab + 1);

    jsonLikePrint("}", tab);
}

void ReturnStmt::print(int tab) {
    jsonLikePrint("ReturnStmt", tab);
    jsonLikePrint("{", tab);

    if (this->exp != nullptr) {
        this->exp->print(tab + 1);
    }

    jsonLikePrint("}", tab);
}

void GetintStmt::print(int tab) {
    jsonLikePrint("GetintStmt", tab);
    jsonLikePrint("{", tab);

    this->lVal->print(tab + 1);

    jsonLikePrint("}", tab);
}

void PrintfStmt::print(int tab) {
    jsonLikePrint("PrintfStmt", tab);
    jsonLikePrint("{", tab);

    jsonLikePrint(this->formatString->toString(), tab + 1);
//    std::cout << this->formatString->toString() << std::endl;
    for (auto iter: this->formatExps) {
        iter->print(tab + 1);
    }

    jsonLikePrint("}", tab);
}


void Cond::print(int tab) {
    jsonLikePrint("Cond", tab);
    jsonLikePrint("{", tab);

    this->lOrExp->print(tab + 1);

    jsonLikePrint("}", tab);
}

void LOrExp::print(int tab) {
    jsonLikePrint("LOrExp", tab);
    jsonLikePrint("{", tab);

    for (auto iter: this->lAndExps) {
        iter->print(tab + 1);
    }

    jsonLikePrint("}", tab);
}

void LAndExp::print(int tab) {
    jsonLikePrint("LAndExp", tab);
    jsonLikePrint("{", tab);

    for (auto iter: this->eqExps) {
        iter->print(tab + 1);
    }

    jsonLikePrint("}", tab);
}

void EqExp::print(int tab) {
    jsonLikePrint("EqExp", tab);
    jsonLikePrint("{", tab);

    this->root->print(tab + 1);

    jsonLikePrint("}", tab);
}

void EqOpTree::print(int tab) {
    jsonLikePrint("EqOpTree", tab);
    jsonLikePrint("{", tab);

    if (this->isLeaf) {
        jsonLikePrint("isLeaf: true", tab + 1);
        this->value->print(tab + 1);
    } else {
        jsonLikePrint("isLeaf: false", tab + 1);
        jsonLikePrint(this->op->toString(), tab + 1);
        this->branch[0]->print(tab + 1);
        this->branch[1]->print(tab + 1);
    }

    jsonLikePrint("}", tab);
}

void RelExp::print(int tab) {
    jsonLikePrint("RelExp", tab);
    jsonLikePrint("{", tab);

    this->root->print(tab + 1);

    jsonLikePrint("}", tab);
}

void RelOpTree::print(int tab) {
    jsonLikePrint("RelOpTree", tab);
    jsonLikePrint("{", tab);

    if (this->isLeaf) {
        jsonLikePrint("isLeaf: true", tab + 1);
        this->value->print(tab + 1);
    } else {
        jsonLikePrint("isLeaf: false", tab + 1);
        jsonLikePrint(this->op->toString(), tab + 1);
        this->branch[0]->print(tab + 1);
        this->branch[1]->print(tab + 1);
    }

    jsonLikePrint("}", tab);
}

void FuncDef::print(int tab) {
    jsonLikePrint("FuncDef", tab);
    jsonLikePrint("{", tab);

    jsonLikePrint(this->ident->toString(), tab + 1);
    this->funcType->print(tab + 1);
    if (this->funcFParams != nullptr) {
        this->funcFParams->print(tab + 1);
    }
    this->block->print(tab + 1);

    jsonLikePrint("}", tab);
}

void FuncType::print(int tab) {
    jsonLikePrint("FuncType", tab);
    jsonLikePrint("{", tab);

    jsonLikePrint(this->funcType->toString(), tab + 1);

    jsonLikePrint("}", tab);
}

void FuncFParams::print(int tab) {
    jsonLikePrint("FuncFParams", tab);
    jsonLikePrint("{", tab);

    for (auto iter: this->funcFParams) {
        iter->print(tab + 1);
    }

    jsonLikePrint("}", tab);
}

void FuncFParam::print(int tab) {
    jsonLikePrint("FuncFParam", tab);
    jsonLikePrint("{", tab);

    jsonLikePrint(this->ident->toString(), tab + 1);
    jsonLikePrint(std::to_string(this->row), tab + 1);
    if (this->row >= 2) {
        jsonLikePrint("[][const exp]", tab + 1);
        this->constExp->print(tab + 1);
    }

    jsonLikePrint("}", tab);
}