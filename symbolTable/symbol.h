//
// Created by yuxin on 2021/10/20.
//

#ifndef COMPILER_SYMBOL_H
#define COMPILER_SYMBOL_H

#include "vector"
#include "../token/Token.h"
#include "unordered_map"

#define FUNC_SCOPE "FUNC_SCOPE SCOPE"
#define BLOCK_SCOPE "BLOCK_SCOPE SCOPE"
#define MACRO_SCOPE "COMP UNIT SCOPE"

/* Item:
 * -var
 * -constVar
 * -func
 * -param
 * */
class Item {
protected:
    Token *token;
public:
    Item(Token *tokenIn) {
        this->token = tokenIn;
    };

    virtual void print() {
        std::cout << "Item" << std::endl;
        std::cout << this->token->toString() << std::endl;
    }

    std::string getKey() {
        return this->token->value;
    }

    int getLine() {
        return this->token->getLine();
    }
};

class VarItem : public Item {
protected:
    bool isConst;
public:
    VarItem(Token *tokenIn, bool isConstIn) : Item(tokenIn) {
        this->isConst = isConstIn;
    };

    void print() override {
        std::cout << "Var Item" << std::endl;
        std::cout << this->token->toString() << std::endl;
        std::cout << "is const: " << this->isConst << std::endl;
    }
};

class ArrayItem : public VarItem {
    int dimension;
    int row1;
    int row2;
public:
    ArrayItem(Token *tokenIn, bool isConstIn, int dimensionIn = 0, int row1In = -1, int row2In = -1) : VarItem(tokenIn,
                                                                                                             isConstIn) {
        this->dimension = dimensionIn;
        this->row1 = row1In;
        this->row2 = row2In;
    };

    void print() override {
        std::cout << "Array Item" << std::endl;
        std::cout << this->token->toString() << std::endl;
        std::cout << "Dimension: " << this->dimension << std::endl;
        std::cout << "Row1: " << this->row1 << std::endl;
        std::cout << "Row2: " << this->row2 << std::endl;
    }
};

class FuncItem : public Item {
    bool hasReturn;
    std::vector<Item *> params;
public:
    FuncItem(Token *tokenIn, bool hasReturnIn) : Item(tokenIn) {
        this->hasReturn = hasReturnIn;
    };

    void addParams(std::vector<Item *> paramsIn) {
        params.insert(params.end(), paramsIn.begin(), paramsIn.end());
    }

    void print() override {
        std::cout << "Func Item" << std::endl;
        std::cout << this->token->toString() << std::endl;
        std::cout << "Return: " << this->hasReturn << std::endl;
        std::cout << "Params Num: " << this->params.size() << std::endl;
        for (auto iter: params) {
            iter->print();
        }
        std::cout << "Params End " << std::endl;
    }
};

class Scope {
    std::string type;
    std::unordered_map<std::string, Item *> table;
public:
    Scope(std::string type) {
        this->type = type;
    }

    void addItem(Item *item) {
        item->print();
        table.emplace(item->getKey(), item);
    }

    bool exist(const std::string &key) {
        std::cout << "check>>>>>>>>>>>>>" << key << std::endl;
        return !(this->table.find(key) == this->table.end());
    }

    void checkScope() {
        for (const auto& iter : table) {
            std::cout << "check--->" << iter.first << std::endl;
        }
    }

    std::string getType() {
        return this->type;
    }
};

/* Scope:
 * -CompUnit
 * -Function(main, func)
 * -Block
 * */

class SymbolTable {
    std::vector<Scope *> scopeStack;
public:
    void pushScope(Scope *scope) {
        std::cout << ">>> push " << scope->getType() << std::endl;
        scopeStack.emplace_back(scope);
    }

    void popScope() {
        Scope *pop = this->scopeStack.back();
        std::cout << ">>> pop " << pop->getType() << std::endl;
        scopeStack.pop_back();
    }

    bool exist(const std::string &key) {
        for (auto iter: this->scopeStack) {
            if (iter->exist(key)) {
                return true;
            }
        }
        return false;
    }

    Scope *getRecentScope() {
        return this->scopeStack.back();
    }
};

#endif //COMPILER_SYMBOL_H