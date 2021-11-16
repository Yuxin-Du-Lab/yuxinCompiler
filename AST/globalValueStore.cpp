//
// Created by yuxin on 2021/11/15.
//
#include "ASTBuilder.h"
#include "unordered_map"
#include "vector"

std::unordered_map<std::string, int> globalVar2value;
std::unordered_map<std::string, std::vector<int>> globalArr2value;
std::unordered_map<std::string, int> undefinedVar;

void CompUnit::storeGlobalValues() {
    for (auto constItem : this->constDecls) {
        constItem->storeGlobalValues();
    }
    for (auto varItem : this->varDecls) {
        varItem->storeGlobalValues();
    }
    for (auto scan : globalVar2value) {
        std::cout << "Global Var " << scan.first << "\t" << std::to_string(scan.second) << std::endl;
    }
    for (auto scan : globalArr2value) {
        std::cout << "Global Arr " << scan.first << std::endl;
        for (auto iter : scan.second) {
            std::cout << std::to_string(iter) << std::endl;
        }
    }
}

void ConstDecl::storeGlobalValues() {
    for (auto item : this->constDefs) {
        item->storeGlobalValues();
    }
}

void ConstDef::storeGlobalValues() {
    if (this->row == 0) {
        globalVar2value.emplace(this->getIdent()->getKey(), this->getConstInitVal()->getValue());
    } else {
        globalArr2value.emplace(this->getIdent()->getKey(), this->getConstInitVal()->getValues());
    }
}

void VarDecl::storeGlobalValues() {
    for (auto item : this->varDefs) {
        item->storeGlobalValues();
    }
}

void VarDef::storeGlobalValues() {
    if (!this->hasInitVal) {
        undefinedVar.emplace(this->getIdent()->getKey(), VALUE_ERROR);
        return;
    }
    if (this->row == 0) {
        globalVar2value.emplace(this->getIdent()->getKey(), this->initVal->getValue());
    } else {
        globalArr2value.emplace(this->getIdent()->getKey(), this->initVal->getValues());
    }
}

int Exp::getValueInGlobal() {
    return this->addExp->getConstValue();
}

int LVal::getValue() {
    if (undefinedVar.find(this->ident->getKey()) != undefinedVar.end()) {
        return VALUE_ERROR;
    }
    if (this->dimension == 0) {
        if (globalVar2value.find(this->ident->getKey()) != undefinedVar.end()) {
            return globalVar2value.find(this->ident->getKey())->second;
        } else {
            return VALUE_ERROR;
        }
    } else if (this->dimension == 1) {
        if (globalArr2value.find(this->ident->getKey()) != globalArr2value.end()) {
            std::vector<int> arr = globalArr2value.find(this->ident->getKey())->second;
            return arr[this->exps[0]->getValueInGlobal()];
        } else {
            return VALUE_ERROR;
        }
    } else if (this->dimension == 2) {
        if (globalArr2value.find(this->ident->getKey()) != globalArr2value.end()) {
            std::vector<int> arr = globalArr2value.find(this->ident->getKey())->second;
            return arr[this->exps[0]->getValueInGlobal() * this->row2 + this->exps[1]->getValueInGlobal()];
        } else {
            return VALUE_ERROR;
        }
    }
    return VALUE_ERROR;
}