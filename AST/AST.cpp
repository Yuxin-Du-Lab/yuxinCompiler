//
// Created by yuxin on 2021/10/3.
//

#include "AST.h"


void CompUnit::addConstDecl(ConstDecl *constDecl) {
    this->constDecls.emplace_back(constDecl);
}

void CompUnit::addVarDecl(VarDecl *varDecl) {
    this->varDecls.emplace_back(varDecl);
}

void CompUnit::addFuncDef(FuncDef *funcDef) {
    this->funcDefs.emplace_back(funcDef);
}

void CompUnit::setMainFuncDef(MainFuncDef *mainFuncDef) {
    this->mainFuncDef = mainFuncDef;
}

void MainFuncDef::setBlock(Block *block) {
    this->block = block;
}

void FuncRParams::addParamsExp(Exp *exp) {
    this->paramExps.emplace_back(exp);
}

void Block::addConstDecl(ConstDecl *constDecl) {
//    this->constDecls.emplace_back(constDecl);
    this->blockItems.emplace_back(constDecl);
}

void Block::addVarDecl(VarDecl *varDecl) {
//    this->varDecls.emplace_back(varDecl);
    this->blockItems.emplace_back(varDecl);
}

void Block::addStmt(Stmt *stmt) {
//    this->stmts.emplace_back(stmt);
    this->blockItems.emplace_back(stmt);
}

void IfStmt::setCond(Cond *cond) {
    this->cond = cond;
}

void IfStmt::setIfStmt(Stmt *stmt) {
    this->ifStmt = stmt;
}

void IfStmt::setElseStmt(Stmt *stmt) {
    this->elseStmt = stmt;
}

void WhileStmt::setCond(Cond *cond) {
    this->cond = cond;
}

void WhileStmt::setWhileStmt(Stmt *stmt) {
    this->stmt = stmt;
}

void LoopStmt::setToken(Token *token) {
    this->break_continue = token;
}

void PrintfStmt::setFormatString(Token *token) {
    this->formatString = token;
}

void PrintfStmt::addFormatExp(Exp *exp) {
    this->formatExps.emplace_back(exp);
}

void ReturnStmt::setReturnExp(Exp *exp) {
    this->exp = exp;
}

void BlockStmt::setBlock(Block *block) {
    this->block = block;
}

void GetintStmt::setLVal(LVal *lVal) {
    this->lVal = lVal;
}

void LValStmt::setLVal(LVal *lVal) {
    this->lVal = lVal;
}

void LValStmt::setExp(Exp *exp) {
    this->exp = exp;
}

void ExpStmt::setExp(Exp *exp) {
    this->exp = exp;
}

void FuncDef::setFuncType(FuncType *funcType) {
    this->funcType = funcType;
}

void FuncDef::setFuncIdent(Token *token) {
    this->ident = token;
}

void FuncDef::setFuncFParams(FuncFParams *funcFParams) {
    this->funcFParams = funcFParams;
}

void FuncDef::setFuncBlock(Block *block) {
    this->block = block;
}

void FuncType::setFuncType(Token *funcType) {
    this->funcType = funcType;
}

void FuncFParams::addFuncFParam(FuncFParam *funcFParam) {
    this->funcFParams.emplace_back(funcFParam);
}

void FuncFParam::setFuncFParamIdent(Token *token) {
    this->ident = token;
}

void FuncFParam::setRow(int rowIn) {
    this->row = rowIn;
}

void FuncFParam::setConstExp(ConstExp *constExp) {
    this->constExp = constExp;
}

void FuncUnaryExp::setFuncRParams(FuncRParams *funcRParams) {
    this->funcRParams = funcRParams;
}

void FuncUnaryExp::setFuncUnaryIdent(Token *token) {
    this->ident = token;
}

void PrimaryUnaryExp::setPrimaryExp(PrimaryExp *primaryExp) {
    this->primaryExp = primaryExp;
}

void UnaryUnaryExp::setUnaryExp(UnaryExp *unaryExp) {
    this->unaryExp = unaryExp;
}

void UnaryUnaryExp::setUnaryOp(UnaryOp *unaryOp) {
    this->unaryOp = unaryOp;
}

void UnaryOp::setUnaryOp(Token *token) {
    this->op = token;
}

void ExpPrimaryExp::setPrimaryExp(Exp *exp) {
    this->exp = exp;
}

void LValPrimaryExp::setPrimaryLVal(LVal *lVal) {
    this->lVal = lVal;
}

void NumberPrimaryExp::setPrimaryNumber(Number *number) {
    this->number = number;
}

void Number::setIntConst(Token *token) {
    this->value = std::stoi(token->getKey());
    this->intConst = token;
}

void Cond::setLOrExp(LOrExp *lOrExp) {
    this->lOrExp = lOrExp;
}

void LOrExp::addLAndExp(LAndExp *lAndExp) {
    this->lAndExps.emplace_back(lAndExp);
}

void LAndExp::addEqExp(EqExp *eqExp) {
    this->eqExps.emplace_back(eqExp);
}

void EqExp::setEqExpRoot(EqOpTree *root) {
    this->root = root;
}

void EqOpTree::setBranch(EqOpTree *eqOpTree, int child) {
    this->branch[child] = eqOpTree;
}

void EqOpTree::setIsLeaf(bool isLeaf) {
    this->isLeaf = isLeaf;
}

void EqOpTree::setLeafValue(RelExp *relExp) {
    this->value = relExp;
}

void EqOpTree::setOp(Token *token) {
    this->op = token;
}

void RelExp::setRoot(RelOpTree *root) {
    this->root = root;
}

void RelOpTree::setIsLeaf(bool isLeaf) {
    this->isLeaf = isLeaf;
}

void RelOpTree::setLeafValue(AddExp *addExp) {
    this->value = addExp;
}

void RelOpTree::setBranch(RelOpTree *relOpTree, int child) {
    this->branch[child] = relOpTree;
}

void RelOpTree::setOp(Token *token) {
    this->op = token;
}

void AddExp::setRoot(AddOpTree *root) {
    this->root = root;
}

void AddOpTree::setIsLeaf(bool isLeaf) {
    this->isLeaf = isLeaf;
}

void AddOpTree::setBranch(AddOpTree *addOpTree, int child) {
    this->branch[child] = addOpTree;
}

void AddOpTree::setOp(Token *token) {
    this->op = token;
}

void AddOpTree::serValue(MulExp *value) {
    this->value = value;
}

void MulExp::setRoot(MulOpTree *root) {
    this->root = root;
}

void MulOpTree::setIsLeaf(bool isLeaf) {
    this->isLeaf = isLeaf;
}

void MulOpTree::serValue(UnaryExp *value) {
    this->value = value;
}

void MulOpTree::setBranch(MulOpTree *mulOpTree, int child) {
    this->branch[child] = mulOpTree;
}

void MulOpTree::setOp(Token *token) {
    this->op = token;
}

void LVal::setUsedDimension(int row) {
    this->usedDimension = row;
}

void LVal::setArrayExp(Exp *exp, int row) {
    this->exps[row] = exp;
}

void LVal::setLValIdent(Token *token) {
    this->ident = token;
}

void ConstDecl::addConstDef(ConstDef *constDef) {
    this->constDefs.emplace_back(constDef);
}

void ConstDef::setRow(int row) {
    this->row = row;
}

void ConstDef::setArrayConstExp(ConstExp *constExp, int row) {
    this->constExps[row] = constExp;
}

void ConstDef::setIdent(Token *token) {
    this->ident = token;
}

void ConstDef::setConstInitVal(ConstInitVal *constInitVal) {
    this->constInitVal = constInitVal;
}

void ConstInitVal::setRow(int row) {
    this->row = row;
}

void ConstInitVal::setConstExp(ConstExp *constExp) {
    this->constExp = constExp;
}

void ConstInitVal::addConstInitVal(ConstInitVal *constInitVal) {
    this->constInitVals.emplace_back(constInitVal);
}

void VarDecl::addVarDef(VarDef *varDef) {
    this->varDefs.emplace_back(varDef);
}

void VarDef::setIdent(Token *token) {
    this->ident = token;
}

void VarDef::setRow(int row) {
    this->row = row;
}

void VarDef::setArrayConstExp(ConstExp *constExp, int row) {
    this->constExps[row] = constExp;
}

void VarDef::setHasInitVal(bool hasInitVal) {
    this->hasInitVal = hasInitVal;
}

void VarDef::setInitVal(InitVal *initVal) {
    this->initVal = initVal;
}

void InitVal::setRow(int row) {
    this->row = row;
}

void InitVal::setExp(Exp *exp) {
    this->exp = exp;
}

void InitVal::addInitVal(InitVal *initVal) {
    this->initVals.emplace_back(initVal);
}

void ConstExp::setAddExp(AddExp *addExp) {
    this->addExp = addExp;
}

void Exp::setAddExp(AddExp *addExp) {
    this->addExp = addExp;
}

int Exp::getRealDimension() {
//    std::cout << "Exp Real Dimension: " << std::to_string(this->addExp->getRealDimension()) << std::endl;
    return addExp->getRealDimension();
}

int ExpPrimaryExp::getRealDimension() {
    return this->exp->getRealDimension();
}

int LValPrimaryExp::getRealDimension() {
    return this->lVal->getRealDimension();
}

int ExpPrimaryExp::getConstValue() {
    return ((ConstExp *)this->exp)->getConstValue();
}

int Number::getConstValue() {
    return this->value;
}

int NumberPrimaryExp::getConstValue() {
    return this->number->getConstValue();
}

int LValPrimaryExp::getConstValue() {
    return this->lVal->getConstValue();
}

int ConstInitVal::getSpecificValue(int row1, int row2)  {
    if (this->constExp != nullptr) {
        return this->constExp->getConstValue();
    } else {
        if (this->values.empty()) {
            setValues();
        }
        if (this->constDef->getRow() == 1) {
            if (this->values.empty()) {
                return 0;
            }
            return this->values[row1];
        } else if (this->constDef->getRow() == 2) {
            return this->values[row1 * this->constDef->getConstExp(1)->getConstValue() + row2];
        }
    }
}