//
// Created by yuxin on 2021/11/10.
//

#ifndef COMPILER_IRSCOPE_H
#define COMPILER_IRSCOPE_H

#define IRSymbolItemType 0
#define VarIRItemType 1
#define ConstVarIRItemType 2
#define ArrIRItemType 3
#define ConstArrIRItemType 4
#define FuncIRItemType 5
#define ParaVarItemType 6
#define ParaArrItemType 7

#include "vector"
#include "unordered_map"
class IRSymbolTable;

class IRSymbolItem {
    std::string name;
    int scopeId = -1;
public:
    IRSymbolItem(std::string identIn) {
        this->name = identIn;
    }

    std::string getName() {
        return this->name;
    }

    void setScope(int scopeIn) {
        this->scopeId = scopeIn;
    }

    int getScope() {
        return this->scopeId;
    }

    virtual int getType() {
        return IRSymbolItemType;
    }

    virtual void check() {
        std::cout << "<IRSymbolItem>" << std::endl;
    }
};

class VarIRItem : public IRSymbolItem {

public:
    VarIRItem(std::string identIn) : IRSymbolItem(std::move(identIn)) {

    }

    int getType() override {
        return VarIRItemType;
    }

    void check() override {
        std::cout << "<VarIRItem> " << this->getName() << std::endl;
    }
};

class ConstVarIRItem : public VarIRItem {
    int value;
public:
    ConstVarIRItem(std::string identIn, int valueIn) : VarIRItem(std::move(identIn)) {
        this->value = valueIn;
    }

    int getType() override {
        return ConstVarIRItemType;
    }

    void check() override {
        std::cout << "<ConstVarIRItem> " << this->getName() << " : " << std::to_string(this->value) << std::endl;
    }

    int getValue() {
        return this->value;
    }
};

class ArrIRItem : public IRSymbolItem {
    int row;
    int row_2;
public:
    ArrIRItem(std::string identIn, int rowIn, int row_2In=-1) : IRSymbolItem(std::move(identIn)) {
        this->row = rowIn;
        this->row_2 = row_2In;
    }

    int getType() override {
        return ArrIRItemType;
    }

    int getRow() {
        return this->row;
    }

    int getRow_2() {
        return this->row_2;
    }

    void check() override {
        std::cout << "<ArrIRItem> " << this->getName() << "[" << std::to_string(this->row) << "]" << std::endl;
    }
};

class ConstArrIRItem : public ArrIRItem {
    std::vector<int> values;
    int row_2;
public:
    ConstArrIRItem(std::string identIn, int rowIn, std::vector<int> valuesIn, int row_2In=-1) : ArrIRItem(std::move(identIn), rowIn, row_2In) {
        this->values.insert(this->values.end(), valuesIn.begin(), valuesIn.end());
        this->row_2 = row_2In;
    }

    int getType() override {
        return ConstArrIRItemType;
    }

    void check() override {
        std::cout << "<ConstArrIRItem> " << this->getName() << "[" << std::to_string(this->getRow()) << "]" << std::endl;
        for (auto iter : this->values) {
            std::cout << std::to_string(iter) << std::endl;
        }
        std::cout << "Const Arr End" << std::endl;
    }
};

class FuncIRItem : public IRSymbolItem {
    int paraNum;
    std::vector<std::string> tmpVars;
public:
    FuncIRItem(std::string identIn, int paraNumIn) : IRSymbolItem(std::move(identIn)) {
        this->paraNum = paraNumIn;
    }

    std::vector<std::string> getTmpVars() {
        return this->tmpVars;
    }

    int getType() override {
        return FuncIRItemType;
    }

    void check() override {
        std::cout << "<FuncIRItem> " << this->getName() << " para num: " << std::to_string(this->paraNum) << std::endl;
    }

    int getParaNum() {
        return this->paraNum;
    }

    void addTmpVar(std::string tmp) {
        for (auto iter : this->tmpVars) {
            if (iter == tmp) {
                return;
            }
        }
        this->tmpVars.emplace_back(tmp);
    }

    void checkTmpVars() {
        std::cout << "checkTmpVars for func" << std::endl;
        for (auto iter : this->tmpVars) {
            std::cout << iter << std::endl;
        }
        std::cout << "check end" << std::endl;
    }
};

class ParaVarIRItem : public IRSymbolItem {
public:
    ParaVarIRItem(std::string identIn) : IRSymbolItem(std::move(identIn)) {

    }

    int getType() override {
        return ParaVarItemType;
    }

    void check() override {
        std::cout << "<ParaVarIRItem> " << this->getName() << std::endl;
    }
};

class ParaArrIRItem : public IRSymbolItem {
    int row2_row;
public:
    ParaArrIRItem(std::string identIn, int row2_rowIn=-1) : IRSymbolItem(std::move(identIn)) {
        this->row2_row = row2_rowIn;
    }

    int getType() override {
        return ParaArrItemType;
    }

    void check() override {
        if (this->row2_row == -1) {
            std::cout << "<ParaArrIRItem> " << this->getName() << std::endl;
        } else {
            std::cout << "<ParaArrIRItem> " << this->getName() << "[][" << std::to_string(this->row2_row) << "]" << std::endl;
        }
    }
};

/*
 * IR scope table
 */
class IRScope {
    int scopeId;
    int fatherScopeId;
    IRSymbolTable *table{};
    std::unordered_map<std::string, IRSymbolItem *> records;
    std::vector<int> childrenScopes;
public:
    IRScope(int scopeIdIn, int fatherId, IRSymbolTable *tableIn) {
        this->scopeId = scopeIdIn;
        this->fatherScopeId = fatherId;
        this->table = tableIn;
    }

    void addChildScope(int childId) {
        this->childrenScopes.emplace_back(childId);
    }

    int getFatherId() {
        return this->fatherScopeId;
    }

    int getMyScopeId() {
        return this->scopeId;
    }

    void addItem(IRSymbolItem *item) {
        item->setScope(this->scopeId);
        this->records.emplace(item->getName(), item);
    }

    std::string getLinkedName(IRSymbolItem *itemIn);

    int findScope4IRItem(IRSymbolItem *itemIn);

    IRSymbolItem *findItem(std::string name);

    void check();
};

class IRSymbolTable {
    int temporaryScopeId;
    int scopeCnt;
    std::unordered_map<int, IRScope *> IRTable;
    FuncIRItem *currentFunc{};
public:
    IRSymbolTable() {
        this->temporaryScopeId = -1;
        this->scopeCnt = 0;
    }

    void setCurrentFunc(FuncIRItem *item) {
        this->currentFunc = item;
    }

    FuncIRItem *getCurrentFunc() {
        return this->currentFunc;
    }

    int enterNewScope();

    int exitScope();

    IRScope *id2scope(int scopeId) {
        if (this->IRTable.find(scopeId) != this->IRTable.end()) {
            return this->IRTable.find(scopeId)->second;
        }
        return nullptr;
    }

    void addItem4currentScope(IRSymbolItem *item) {
        auto *currentScope = id2scope(temporaryScopeId);
        if (currentScope != nullptr) {
            currentScope->addItem(item);
        }
    }

    void check() {
        auto *rootScope = this->IRTable.find(0)->second;
        rootScope->check();
    }

    int getCurrentScopeId() {
        return this->temporaryScopeId;
    }

    IRSymbolItem *findItemFromTable(std::string name) {
        IRScope *currentScope = IRTable.find(this->temporaryScopeId)->second;
        if (currentScope->findItem(name) != nullptr) {
            return currentScope->findItem(name);
        }
        return nullptr;
    }

    int findScope4name(std::string name) {
        auto *item = findItemFromTable(name);
        if (item != nullptr) {
            return item->getScope();
        }
        return -1;
    }
};

#endif //COMPILER_IRSCOPE_H
