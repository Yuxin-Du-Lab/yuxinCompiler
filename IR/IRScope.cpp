//
// Created by yuxin on 2021/11/10.
//

#include "iostream"
#include "IRScope.h"

std::string IRScope::getLinkedName(IRSymbolItem *itemIn)  {
    if (this->records.find(itemIn->getName()) != this->records.end()) {
        return itemIn->getName() + "#" + std::to_string(this->findScope4IRItem(itemIn));
    }
    return "^undefined name^";
}

int IRScope::findScope4IRItem(IRSymbolItem *itemIn) {
    if (this->records.find(itemIn->getName()) != this->records.end()) {
        return this->scopeId;
    }
    if (this->fatherScopeId == -1) {
        return -1;
    }
    auto *father = this->table->id2scope(this->getFatherId());
    if (father != nullptr) {
        return father->findScope4IRItem(itemIn);
    }
    return -1;
}

int IRSymbolTable::enterNewScope() {
    int fatherId = this->temporaryScopeId;
    this->temporaryScopeId = this->scopeCnt;
    this->scopeCnt ++;
    auto *newScope = new IRScope(this->temporaryScopeId, fatherId, this);
    this->IRTable.emplace(temporaryScopeId, newScope);
    if (fatherId != -1) {
        if (this->IRTable.find(fatherId) != this->IRTable.end()) {
            auto *father = this->IRTable.find(fatherId)->second;
            father->addChildScope(this->temporaryScopeId);
        }
    }
    return this->temporaryScopeId;
}

int IRSymbolTable::exitScope() {
    if (this->temporaryScopeId == 0) {
        this->temporaryScopeId = -1;
        return this->temporaryScopeId;
    }
    if (this->IRTable.find(this->temporaryScopeId) != this->IRTable.end()) {
        auto *temporaryScope = this->IRTable.find(this->temporaryScopeId)->second;
        int fatherId = temporaryScope->getFatherId();
        this->temporaryScopeId = fatherId;
        return this->temporaryScopeId;
    }
    return -2;
}

void IRScope::check() {
    std::cout << "#" << this->scopeId << std::endl;
    for (auto iter : this->records) {
        iter.second->check();
    }
    for (auto iter : this->childrenScopes) {
        auto *child = this->table->id2scope(iter);
        child->check();
    }
}

IRSymbolItem *IRScope::findItem(std::string name)  {
    if (this->records.find(name) != this->records.end()) {
        return this->records.find(name)->second;
    }
    if (this->fatherScopeId == -1) {
        return nullptr;
    }
    auto *father = table->id2scope(this->fatherScopeId);
    return father->findItem(name);
}