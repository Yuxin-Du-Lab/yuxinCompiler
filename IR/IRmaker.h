//
// Created by yuxin on 2021/11/10.
//

#ifndef COMPILER_IRMAKER_H
#define COMPILER_IRMAKER_H

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
public:
    IRSymbolItem(std::string identIn) {
        this->name = identIn;
    }

    std::string getName() {
        return this->name;
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
};

class ArrIRItem : public IRSymbolItem {
    int row;
public:
    ArrIRItem(std::string identIn, int rowIn) : IRSymbolItem(std::move(identIn)) {
        this->row = rowIn;
    }

    int getType() override {
        return ArrIRItemType;
    }

    int getRow() {
        return this->row;
    }

    void check() override {
        std::cout << "<ArrIRItem> " << this->getName() << "[" << std::to_string(this->row) << "]" << std::endl;
    }
};

class ConstArrIRItem : public ArrIRItem {
    std::vector<int> values;
public:
    ConstArrIRItem(std::string identIn, int rowIn, std::vector<int> valuesIn) : ArrIRItem(std::move(identIn), rowIn) {
        this->values.insert(this->values.end(), valuesIn.begin(), valuesIn.end());
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
public:
    FuncIRItem(std::string identIn, int paraNumIn) : IRSymbolItem(std::move(identIn)) {
        this->paraNum = paraNumIn;
    }

    int getType() override {
        return FuncIRItemType;
    }

    void check() override {
        std::cout << "<FuncIRItem> " << this->getName() << " para num: " << std::to_string(this->paraNum) << std::endl;
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
        this->records.emplace(item->getName(), item);
    }

    std::string getLinkedName(IRSymbolItem *itemIn);

    int findScope4IRItem(IRSymbolItem *itemIn);

    void check();
};

class IRSymbolTable {
    int temporaryScopeId;
    int scopeCnt;
    std::unordered_map<int, IRScope *> IRTable;
public:
    IRSymbolTable() {
        this->temporaryScopeId = -1;
        this->scopeCnt = 0;
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
};

#endif //COMPILER_IRMAKER_H
