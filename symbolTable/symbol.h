//
// Created by yuxin on 2021/10/20.
//

#ifndef COMPILER_SYMBOL_H
#define COMPILER_SYMBOL_H

#include "vector"
#include "../token/Token.h"
#include "unordered_map"
#include "../AST/ASTBuilder.h"
#include "../error/errorHandler.h"

#define FUNC_SCOPE "FUNC_SCOPE SCOPE"
#define BLOCK_SCOPE "BLOCK_SCOPE SCOPE"
#define MACRO_SCOPE "COMP UNIT SCOPE"

#define ITEM "ITEM"
#define ARRAY_ITEM_D1 "ARRAY_ITEM_D1"
#define ARRAY_ITEM_D2 "ARRAY_ITEM_D2"
#define FUNC_ITEM "FUNC_ITEM"
#define VAR_ITEM "VAR_ITEM"

// interface of ast


/* Item:
 * -var
 * -constVar
 * -func
 * -param
 * */
class Item {
protected:
    Token *token;
    AstItem *astItem;
public:
    Item(Token *tokenIn, AstItem *astItem = nullptr) {
        this->token = tokenIn;
        this->astItem = astItem;
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

    virtual std::string getType() {
        return ITEM;
    }
};

class VarItem : public Item {
protected:
    bool isConst;
public:
    VarItem(Token *tokenIn, bool isConstIn, AstItem *astItem = nullptr) : Item(tokenIn, astItem) {
        this->isConst = isConstIn;
    };

    void print() override {
        std::cout << "Var Item" << std::endl;
        std::cout << this->token->toString() << std::endl;
        std::cout << "is const: " << this->isConst << std::endl;
    }

    std::string getType() override {
        return VAR_ITEM;
    }

    bool getIsConst() {
        return this->isConst;
    }
};

class ArrayItem : public VarItem {
    int dimension;
    int row1;
    int row2;
public:
    ArrayItem(Token *tokenIn, bool isConstIn, int dimensionIn = 0, AstItem *astItem = nullptr, int row1In = -1,
              int row2In = -1) : VarItem(tokenIn,
                                         isConstIn, astItem) {
        this->dimension = dimensionIn;
        this->row1 = row1In;
        this->row2 = row2In;
    };

    int getDimension() {
        return dimension;
    }

    void print() override {
        std::cout << "Array Item" << std::endl;
        std::cout << this->token->toString() << std::endl;
        std::cout << "Dimension: " << this->dimension << std::endl;
        std::cout << "Row1: " << this->row1 << std::endl;
        std::cout << "Row2: " << this->row2 << std::endl;
    }

    std::string getType() override {
        return dimension == 1 ? ARRAY_ITEM_D1 :
               dimension == 2 ? ARRAY_ITEM_D2 :
               VAR_ITEM;
    }
};

class FuncItem : public Item {
    bool hasReturn;
    std::vector<Item *> params;
public:
    FuncItem(Token *tokenIn, bool hasReturnIn, AstItem *astItem = nullptr) : Item(tokenIn, astItem) {
        this->hasReturn = hasReturnIn;
    };

    int getFParamNum() {
        return this->params.size();
    }

    std::vector<Item *> getParamsAsItem() {
        return this->params;
    }

    void setFParam() {
        if (astItem == nullptr) {
            std::cout << "Not assign FuncItem's AST item" << std::endl;
            return;
        }
        auto *funDef = (FuncDef *) this->astItem;
        std::vector<FuncFParam *> funcFParams = funDef->getFParams();
        for (auto iter: funcFParams) {
            if (iter->getRow() > 0) {
                auto *arrayItem = new ArrayItem(iter->getIdent(), false, iter->getRow(), iter);
                this->params.emplace_back(arrayItem);
            } else {
                auto *varItem = new VarItem(iter->getIdent(), false, iter);
                this->params.emplace_back(varItem);
            }
        }
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

    bool typeMatch(std::vector<Exp *> &RParams, int funcCallLine) {
//        std::cout << ">>\tType Match Check" << std::endl;
        for (int index = 0; index < RParams.size() && index < params.size(); index++) {
            if (RParams[index]->getRealDimension() == 0 && params[index]->getType() != VAR_ITEM ||
                RParams[index]->getRealDimension() == 1 && params[index]->getType() != ARRAY_ITEM_D1 ||
                RParams[index]->getRealDimension() == 2 && params[index]->getType() != ARRAY_ITEM_D2) {
//                std::cout << RParams[index]->getRealDimension() << "R---F" << params[index]->getType() << std::endl;
                throwError(FuncParamTypeNotMatch, funcCallLine);
            }
        }
        return false;
    }

    std::string getType() override {
        return FUNC_ITEM;
    }
};

class Scope {
    bool shouldHasReturn;
    bool returned;
    std::string type;
    std::unordered_map<std::string, Item *> table;
public:
    Scope(std::string type, bool shouldHasReturn = false) {
        this->type = type;
        this->shouldHasReturn = shouldHasReturn;
        this->returned = false;
    }

    void addItem(Item *item) {
//        item->print();
        table.emplace(item->getKey(), item);
    }

    void setReturned() {
        this->returned = true;
    }

    bool exist(const std::string &key) {
        return !(this->table.find(key) == this->table.end());
    }

    Item *findItem(const std::string &key) {
        return (this->table.find(key) == this->table.end()) ? nullptr : this->table.find(key)->second;
    }

    std::string getType() {
        return this->type;
    }

    bool getShouldHasReturn() {
        return this->shouldHasReturn;
    }

    bool getReturned() {
        return this->returned;
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
//        std::cout << ">>> push " << scope->getType() << std::endl;
        scopeStack.emplace_back(scope);
    }

    void popScope() {
        Scope *pop = this->scopeStack.back();
//        std::cout << ">>> pop " << pop->getType() << std::endl;
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

    Item *findItem(const std::string &key) {
        for (auto iter: this->scopeStack) {
            if (iter->exist(key)) {
                return iter->findItem(key);
            }
        }
        return nullptr;
    }

    Scope *getRecentScope() {
        return this->scopeStack.back();
    }

    bool isIdentConst(Token *token) {
        Item *item = findItem(token->getKey());
        if (item != nullptr) {
            return ((VarItem *) item)->getIsConst();
        }
        return false;
    }

    bool isRecentFuncScopeShouldHasReturn() {
        for (int iter = scopeStack.size() - 1; iter >= 0; iter--) {
            if (scopeStack[iter]->getType() == FUNC_SCOPE) {
                return scopeStack[iter]->getShouldHasReturn();
            }
        }
        return false;
    }

    void setRecentFuncScopeReturned() {
        for (int iter = scopeStack.size() - 1; iter >= 0; iter--) {
            if (scopeStack[iter]->getType() == FUNC_SCOPE) {
                scopeStack[iter]->setReturned();
                break;
            }
        }
    }
};

#endif //COMPILER_SYMBOL_H
