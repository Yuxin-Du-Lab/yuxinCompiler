//
// Created by yuxin on 2021/11/15.
//

#include "../IR/IRScope.h"
#include "../AST/ASTBuilder.h"
#include "mips.h"
#include "../IR/IRQuaternion.h"

std::ofstream MipsFile("mips.txt");

extern IRSymbolTable *table;
extern std::vector<std::string> formatStrings;

extern std::unordered_map<std::string, int> globalVar2value;
extern std::unordered_map<std::string, std::vector<int>> globalArr2value;

void printMips(std::string format) {
    MipsFile << format << std::endl;
}

void CompUnit::_dataMake() {
    printMips(".data");
    auto *main_scope = table->id2scope(0);
    for (auto iter: main_scope->getRecords()) {
        auto item = iter.second;
        std::string name = item->getName() + "_" + std::to_string(item->getScope());
        if (item->getType() == VarIRItemType) {
            if (globalVar2value.find(item->getName()) != globalVar2value.end()) {
                int value = globalVar2value.find(item->getName())->second;
                printMips(name + ":\t\t.word\t" + std::to_string(value));
            } else {
                printMips(name + ":\t\t.word\t0");
            }
        } else if (item->getType() == ConstVarIRItemType) {
            printMips(name + ":\t\t.word\t" +
                      std::to_string(((ConstVarIRItem *) item)->getValue()));
        } else if (item->getType() == ArrIRItemType) {
            if (globalArr2value.find(item->getName()) != globalArr2value.end()) {
                std::string inits;
                for (int value : globalArr2value.find(item->getName())->second) {
                    inits += std::to_string(value) + " ";
                }
                printMips(name + ":\t\t.word\t" + inits);
            } else {
                printMips(name + ":\t\t.space\t" +
                          std::to_string(((ArrIRItem *) item)->getRow() * word2bytes));
            }
        } else if (item->getType() == ConstArrIRItemType) {
            std::string inits;
            for (int value : ((ConstArrIRItem*)item)->getValues()) {
                inits += std::to_string(value) + " ";
            }
            printMips(name + ":\t\t.word\t" + inits);
        }
    }

    int index = 0;
    for (auto iter: formatStrings) {
        printMips(standardStrName + std::to_string(index) + ":\t\t.asciiz\t\"" + iter + "\"");
        index++;
    }
    printMips(".text");
}


void QuaternionItem::makeMips() {

}

void ConstVarDeclQ::makeMips() {

}

void ConstArrDeclQ::makeMips() {

}

void VarDeclQ::makeMips() {
    if (this->scopeId == 0) {
        if (!this->rVal.empty()) {

        }
    }

}