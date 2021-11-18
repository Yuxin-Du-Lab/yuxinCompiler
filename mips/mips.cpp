//
// Created by yuxin on 2021/11/15.
//

#include "../IR/IRScope.h"
#include "../AST/AST.h"
#include "mips.h"
#include "../IR/IRQuaternion.h"
#include "unordered_map"
#define standard_header "#0"

std::ofstream MipsFile("mips.txt");

std::unordered_map<std::string, int> name2offset;
std::unordered_map<std::string, int> func2stackSize;
std::unordered_map<std::string, int> arg2row;
std::unordered_map<std::string, int> arr2zero;

int mainStackSize = 0;

extern std::vector<QuaternionItem *> quaternions;

extern IRSymbolTable *table;
extern std::vector<std::string> formatStrings;

extern std::unordered_map<std::string, int> globalVar2value;
extern std::unordered_map<std::string, std::vector<int>> globalArr2value;
extern std::unordered_map<std::string, int> globalUndefinedVar;
extern std::unordered_map<std::string, int> globalUndefinedArr;

std::vector<int> stacks;

bool isNum(std::string str) {
    int index = 0;
    for (char i: str) {
        int tmp = (int) i;
        if (i == 45 && index == 0) {
            index ++;
            continue;
        }
        index ++;
        if (tmp >= 48 && tmp <= 57) {
            continue;
        } else {
            return false;
        }
    }
    return true;
}

bool isGlobal(std::string name) {
    if (globalArr2value.find(name) != globalArr2value.end()) {
        return true;
    }
    if (globalVar2value.find(name) != globalVar2value.end()) {
        return true;
    }
    if (globalUndefinedVar.find(name) != globalUndefinedVar.end() ||
        globalUndefinedArr.find(name) != globalUndefinedArr.end()) {
        return true;
    }
    return false;
}

bool isGlobalArr(std::string name) {
    if (globalArr2value.find(name) != globalArr2value.end() ||
        globalUndefinedArr.find(name) != globalUndefinedArr.end()) {
        return true;
    }
    return false;
}

bool isArgArr(std::string name) {
    if (arg2row.find(name) != arg2row.end()) {
        if (arg2row.find(name)->second > 0) {
            return true;
        }
    }
    return false;
}

bool isArr(std::string name) {
    if (arr2zero.find(name) != arr2zero.end()) {
        return true;
    }
    return false;
}

std::string replaceNameScope(std::string origen) {
    return origen.replace(origen.find("#"), 1, "_");
}

void printMips(std::string format) {
    MipsFile << format << std::endl;
}

void printMK(std::string words) {
    MipsFile << "\n# " << words << std::endl;
}

void syscall() {
    MipsFile << "syscall" << std::endl;
}

void lwFromStack_var(std::string name, std::string reg, bool fromLastStack = false) {
    if (name == reg_zero) {
        printMips("li\t" + reg + "\t0");
        return;
    }
    if (name == "^GETINT_RET") {
        printMips("move\t" + reg + "\t" + reg_v0);
        return;
    }
    if (isNum(name)) {
        printMips("li\t" + reg + "\t" + name);
        return;
    }
    int offset2sp = name2offset.find(name)->second * word2bytes;
    if (fromLastStack) {
        offset2sp += stacks[stacks.size() - 1];
    }
    if (isArr(name) && !isArgArr(name)) {
        printMips("la\t" + reg + "\t" + std::to_string(offset2sp) + "($sp)");
    } else {
        printMips("lw\t" + reg + "\t" + std::to_string(offset2sp) + "($sp)");
    }
}

void lwFromGlobal_var(std::string name, std::string reg) {
    if (isArr(name) && !isArgArr(name)) {
        printMips("la\t" + reg + "\t" + replaceNameScope(name));
    } else {
        printMips("lw\t" + reg + "\t" + replaceNameScope(name));
    }
}

void sw2stack_var(std::string name, std::string reg) {
    int offset2sp = name2offset.find(name)->second * word2bytes;
    printMips("sw\t" + reg + "\t" + std::to_string(offset2sp) + "($sp)");
}

void sw2global_var(std::string name, std::string reg) {
    printMips("sw\t" + reg + "\t" + replaceNameScope(name));
}

void CompUnit::_dataMake() {
    printMips(".data");
    auto *main_scope = table->id2scope(0);
    for (auto iter: main_scope->getRecords()) {
        auto item = iter.second;
        std::string name = item->getName() + "_" + std::to_string(item->getScope());
        if (item->getType() == VarIRItemType) {
            if (globalVar2value.find(item->getName() + standard_header) != globalVar2value.end()) {
                int value = globalVar2value.find(item->getName() + standard_header)->second;
                printMips(name + ":\t\t.word\t" + std::to_string(value));
            } else {
                printMips(name + ":\t\t.word\t0");
            }
        } else if (item->getType() == ConstVarIRItemType) {
            printMips(name + ":\t\t.word\t" +
                      std::to_string(((ConstVarIRItem *) item)->getValue()));
        } else if (item->getType() == ArrIRItemType) {
            if (globalArr2value.find(item->getName() + standard_header) != globalArr2value.end()) {
                std::string inits;
                for (int value: globalArr2value.find(item->getName() + standard_header)->second) {
                    inits += std::to_string(value) + " ";
                }
                printMips(name + ":\t\t.word\t" + inits);
            } else {
                printMips(name + ":\t\t.space\t" +
                          std::to_string(((ArrIRItem *) item)->getRow() * word2bytes));
            }
        } else if (item->getType() == ConstArrIRItemType) {
            std::string inits;
            for (int value: ((ConstArrIRItem *) item)->getValues()) {
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

void makeMips() {
    for (auto iter: quaternions) {
        iter->makeMips();
    }
}

void QuaternionItem::makeMips() {

}

void ConstVarDeclQ::makeMips() {
    if (!this->isGlobal) {
        printMK("const int " + this->toString() + " = " + std::to_string(this->value));
        printMips("li\t$t0\t" + std::to_string(this->value));
        sw2stack_var(this->toString(), reg_t0);
    }
}

void VarDeclQ::makeMips() {
    if (!this->rVal.empty() && !this->isGlobal) {
        printMK("int " + this->toString());
        lwFromStack_var(this->rVal, reg_t0);
        sw2stack_var(this->toString(), reg_t0);
    }
}

void ConstArrDeclQ::makeMips() {
    arr2zero.emplace(toString(),0);
}

void ArrDeclQ::makeMips() {
    arr2zero.emplace(toString(), 0);
}

void FuncDeclQ::makeMips() {
    int stackSize = this->getStackSize();
    for (auto iter: this->name2offset4stack) {
        name2offset.emplace(iter.first, stackSize - iter.second);
    }
    func2stackSize.emplace(this->name, stackSize);
}

void FuncCallQ::makeMips() {
    if (this->name == "^RETURN") {
        printMK("func return " + this->name);
        printMips("addi\t$sp\t$sp\t" + std::to_string(stacks[stacks.size() - 1]));
        stacks.pop_back();
        return;
    }
    std::string size;
    int current_func_stack_size;
    if (this->name == "main") {
        current_func_stack_size = mainStackSize * word2bytes + word2bytes;
    } else {
        current_func_stack_size = func2stackSize.find(this->name)->second * word2bytes + word2bytes;
    }
    stacks.emplace_back(current_func_stack_size);
    size = std::to_string(current_func_stack_size);
    printMK("func call " + this->name);
//    printMips("sub\t$sp\t$sp\t" + size);
    printMips("sub\t$t8\t$sp\t" + size);
//    $t8 hold the new $sp
}

void ParaDeclQ::makeMips() {
    arg2row.emplace(this->toString(), this->row);
    if (this->row > 0) {
        arr2zero.emplace(toString(), 0);
    }
}

void ExitQ::makeMips() {
    printMK("exit");
    printMips("li\t$v0\t10");
    syscall();
}

void StackPushQ::makeMips() {
    if (this->isArg) {
        if (isArgArr(this->rVal)) {
            printMK("stack push address " + this->rVal);
            int arr_off = name2offset.find(this->rVal)->second * word2bytes;
            printMips("addi\t$t0\t$sp\t" + std::to_string(arr_off));
        } else {
            printMK("stack push value " + this->rVal);
            lwFromStack_var(this->rVal, reg_t0);
        }
        int argOffset = stacks[stacks.size() - 1] - (1 + this->argNo) * word2bytes;
        printMips("sw\t$t0\t" + std::to_string(argOffset) + "($t8)");
        return;
    }
    if (this->rVal == "ra") {
        printMK("stack push " + this->rVal);
        printMips("sw\t$ra\t0($sp)");
        return;
    }
}

void StackPopQ::makeMips() {
    if (this->lVal == "ra") {
        printMips("lw\t$ra\t0($sp)");
    }
}

void GetRetQ::makeMips() {
    printMK("GetRet " + this->lVal);
    sw2stack_var(this->lVal, reg_v0);
}

void SetRetQ::makeMips() {
    printMK("setRet " + this->rVal);
    lwFromStack_var(this->rVal, reg_v0);
}

void SetLabelQ::makeMips() {
    printMips(this->label + " : ");
}

void BranchQ::makeMips() {
    if (this->isFuncCall) {
        printMips("move\t$sp\t$t8");
    }
    if (this->op == BRANCH_EQUAL) {
        lwFromStack_var(this->arg1, reg_t0);
        lwFromStack_var(this->arg2, reg_t1);
        printMips(this->op + "\t" + reg_t0 + "\t" + reg_t1 + "\t" + this->label);
    } else {
        if (this->label == "ra") {
            printMips(this->op + "\t" + reg_ra);
        } else {
            printMips(this->op + "\t" + this->label);
        }
    }
}

void CallGetIntQ::makeMips() {
    printMK("call getint");
    printMips("li\t$v0\t5");
    syscall();
}

void CallPrintQ::makeMips() {
    if (this->isPrintNum) {
        printMK("print num");
        printMips("li\t$v0\t1");
        lwFromStack_var(this->format, reg_a0);
    } else {
        printMK("print str");
        printMips("li\t$v0\t4");
        printMips("la\t$a0\tstr_" + std::to_string(this->formatIndex));
    }
    syscall();
}

void VarAssignQ::makeMips() {
    if (stacks.empty()) return;
    printMK("VarAssignQ " + this->lVal + " = " + this->rVal);
    if (isNum(this->rVal)) {
        printMips("li\t$t0\t" + this->rVal);
    } else if (isGlobal(this->rVal)) {
        lwFromGlobal_var(this->rVal, reg_t0);
    } else {
        lwFromStack_var(this->rVal, reg_t0);
    }
    if (isGlobal(this->lVal)) {
        sw2global_var(this->lVal, reg_t0);
    } else {
        sw2stack_var(this->lVal, reg_t0);
    }
}

void ArrAssignQ::makeMips() {
    if (stacks.empty()) return;
    printMK("ArrAssignQ " + this->name + "[" + this->offset + "] = " + this->rVal);
    if (isNum(this->rVal)) {
        printMips("li\t$t0\t" + this->rVal);
    } else if (isGlobal(this->rVal)) {
        lwFromGlobal_var(this->rVal, reg_t0);
    } else {
        lwFromStack_var(this->rVal, reg_t0);
    }
    if (isGlobalArr(this->name)) {
        if (isNum(this->offset)) {
            int off = atoi(this->offset.c_str()) * word2bytes;
            printMips("sw\t$t0\t" + replaceNameScope(this->name) + "+" + std::to_string(off));
        } else if (isGlobal(this->offset)) {
            lwFromGlobal_var(this->offset, reg_t1);
            printMips("sll\t$t1\t$t1\t2");
            printMips("sw\t$t0\t" + replaceNameScope(this->name) + "(" + reg_t1 + ")");
        } else {
            lwFromStack_var(this->offset, reg_t1);
            printMips("sll\t$t1\t$t1\t2");
            printMips("sw\t$t0\t" + replaceNameScope(this->name) + "(" + reg_t1 + ")");
        }
    } else {
        // stack arr
        if (isArgArr(this->name)) {
            lwFromStack_var(this->name, reg_t5);
//            $t5 = arr address
            if (isNum(this->offset)) {
                int off = atoi(this->offset.c_str()) * word2bytes;
                printMips("addi\t$t6\t$t5\t" + std::to_string(off));
                printMips("sw\t$t0\t($t6)");
            } else if (isGlobal(this->offset)) {
                lwFromGlobal_var(this->offset, reg_t2);
                printMips("sll\t$t2\t$t2\t2");
                printMips("add\t$t6\t$t5\t$t2");
                printMips("sw\t$t0\t($t6)");
            } else {
                lwFromStack_var(this->offset, reg_t2);
                printMips("sll\t$t2\t$t2\t2");
                printMips("add\t$t6\t$t5\t$t2");
                printMips("sw\t$t0\t($t6)");
            }
            return;
        }
        if (isNum(this->offset)) {
            int off = (name2offset.find(this->name)->second + atoi(this->offset.c_str())) * word2bytes;
            printMips("sw\t$t0\t" + std::to_string(off) + "($sp)");
        } else if (isGlobal(this->offset)) {
            int arrOff = name2offset.find(this->name)->second * word2bytes;
            printMips("li\t$t1\t" + std::to_string(arrOff));
            lwFromGlobal_var(this->offset, reg_t2);
            printMips("sll\t$t2\t$t2\t2");
            printMips("add\t$t3\t$t1\t$t2");
            printMips("add\t$t4\t$t3\t$sp");
            printMips("sw\t$t0\t($t4)");
        } else {
            int arrOff = name2offset.find(this->name)->second * word2bytes;
            printMips("li\t$t1\t" + std::to_string(arrOff));
            lwFromStack_var(this->offset, reg_t2);
            printMips("sll\t$t2\t$t2\t2");
            printMips("add\t$t3\t$t1\t$t2");
            printMips("add\t$t4\t$t3\t$sp");
            printMips("sw\t$t0\t($t4)");
        }
    }
}

void GetArrQ::makeMips() {
    if (stacks.empty()) return;
    printMK("GetArrQ " + this->lVal + " = " + this->name + "[" + this->offset + "]");
    if (isGlobalArr(this->name)) {
        if (isNum(this->offset)) {
            int off = atoi(this->offset.c_str()) * word2bytes;
            printMips("lw\t$t0\t" + replaceNameScope(this->name) + "+" + std::to_string(off));
        } else if (isGlobal(this->offset)) {
            lwFromGlobal_var(this->offset, reg_t1);
            printMips("sll\t$t1\t$t1\t2");
            printMips("lw\t$t0\t" + replaceNameScope(this->name) + "($t1)");
        } else {
            lwFromStack_var(this->offset, reg_t1);
            printMips("sll\t$t1\t$t1\t2");
            printMips("lw\t$t0\t" + replaceNameScope(this->name) + "($t1)");
        }
    } else {
        // stack arr
        if (isArgArr(this->name)) {
            lwFromStack_var(this->name, reg_t5);
//            $t5 = arr address
            if (isNum(this->offset)) {
                int off = atoi(this->offset.c_str()) * word2bytes;
                printMips("addi\t$t6\t$t5\t" + std::to_string(off));
                printMips("lw\t$t0\t($t6)");
            } else if (isGlobal(this->offset)) {
                lwFromGlobal_var(this->offset, reg_t2);
                printMips("sll\t$t2\t$t2\t2");
                printMips("add\t$t6\t$t5\t$t2");
                printMips("lw\t$t0\t($t6)");
            } else {
                lwFromStack_var(this->offset, reg_t2);
                printMips("sll\t$t2\t$t2\t2");
                printMips("add\t$t6\t$t5\t$t2");
                printMips("lw\t$t0\t($t6)");
            }
            if (isGlobal(this->lVal)) {
                sw2global_var(this->lVal, reg_t0);
            } else {
                sw2stack_var(this->lVal, reg_t0);
            }
            return;
        }
        if (isNum(this->offset)) {
            int arrOff = name2offset.find(this->name)->second * word2bytes;
            int indexOff = atoi(this->offset.c_str()) * word2bytes;
            int off = arrOff + indexOff;
            printMips("lw\t$t0\t" + std::to_string(off) + "($sp)");
        } else if (isGlobal(this->offset)) {
            lwFromGlobal_var(this->offset, reg_t1);
            printMips("sll\t$t1\t$t1\t2");
            int arrOff = name2offset.find(this->name)->second * word2bytes;
            printMips("li\t$t2\t" + std::to_string(arrOff));
            printMips("add\t$t3\t$t2\t$t1");
            printMips("add\t$t4\t$t3\t$sp");
            printMips("lw\t$t0\t($t4)");
        } else {
            lwFromStack_var(this->offset, reg_t1);
            printMips("sll\t$t1\t$t1\t2");
            int arrOff = name2offset.find(this->name)->second * word2bytes;
            printMips("li\t$t2\t" + std::to_string(arrOff));
            printMips("add\t$t3\t$t2\t$t1");
            printMips("add\t$t4\t$t3\t$sp");
            printMips("lw\t$t0\t($t4)");
        }
    }
    if (isGlobal(this->lVal)) {
        sw2global_var(this->lVal, reg_t0);
    } else {
        sw2stack_var(this->lVal, reg_t0);
    }
}

void ExpQ::makeMips() {
    if (this->opRel == 1) {
        printMK(this->res + " = " + this->op + "\t" + this->arg1);
        if (this->op == "-") {
            lwFromStack_var(this->arg1, reg_t0);
            printMips("sub\t$t1\t$0\t$t0");
            sw2stack_var(this->res, reg_t1);
        } else if (this->op == "!") {
            lwFromStack_var(this->arg1, reg_t0);
            printMips("slti\t$t1\t$t0\t1");
            sw2stack_var(this->res, reg_t1);
        } else {
            lwFromStack_var(this->arg1, reg_t0);
            sw2stack_var(this->res, reg_t0);
        }
        return;
    }
    printMK(this->res + " = " + this->arg1 + "\t" + this->op + "\t" + this->arg2);
//    TODO global arr offset increase, but stack arr offset decrease, arg arr ?
    if (this->op == "+") {
        lwFromStack_var(this->arg1, reg_t0);
        lwFromStack_var(this->arg2, reg_t1);
        if (isArr(this->arg1)) {
            printMips("sll\t$t1\t$t1\t2");
            printMips("add\t$t2\t$t0\t$t1");
        } else if (isArr(this->arg2)) {
            printMips("sll\t$t0\t$t0\t2");
            printMips("add\t$t2\t$t1\t$t0");
        } else {
            printMips("add\t$t2\t$t0\t$t1");
        }
        sw2stack_var(this->res, reg_t2);
    } else if (this->op == "-") {
        lwFromStack_var(this->arg1, reg_t0);
        lwFromStack_var(this->arg2, reg_t1);
        printMips("sub\t$t2\t$t0\t$t1");
        sw2stack_var(this->res, reg_t2);
    } else if (this->op == "*") {
        lwFromStack_var(this->arg1, reg_t0);
        lwFromStack_var(this->arg2, reg_t1);
        printMips("mult\t$t0\t$t1");
        printMips("mflo\t$t2");
        sw2stack_var(this->res, reg_t2);
    } else if (this->op == "/") {
        lwFromStack_var(this->arg1, reg_t0);
        lwFromStack_var(this->arg2, reg_t1);
        printMips("div\t$t0\t$t1");
        printMips("mflo\t$t2");
        sw2stack_var(this->res, reg_t2);
    } else if (this->op == "%") {
        lwFromStack_var(this->arg1, reg_t0);
        lwFromStack_var(this->arg2, reg_t1);
        printMips("div\t$t0\t$t1");
        printMips("mfhi\t$t2");
        sw2stack_var(this->res, reg_t2);
    } else if (this->op == ">") {
        lwFromStack_var(this->arg1, reg_t0);
        lwFromStack_var(this->arg2, reg_t1);
        printMips("slt\t$t2\t$t1\t$t0");
        sw2stack_var(this->res, reg_t2);
    } else if (this->op == "<") {
        lwFromStack_var(this->arg1, reg_t0);
        lwFromStack_var(this->arg2, reg_t1);
        printMips("slt\t$t2\t$t0\t$t1");
        sw2stack_var(this->res, reg_t2);
    } else if (this->op == ">=") {
        lwFromStack_var(this->arg1, reg_t0);
        lwFromStack_var(this->arg2, reg_t1);
        printMips("sub\t$t2\t$t1\t$t0");
        printMips("slti\t$t3\t$t2\t1");
        sw2stack_var(this->res, reg_t3);
    } else if (this->op == "<=") {
        lwFromStack_var(this->arg1, reg_t0);
        lwFromStack_var(this->arg2, reg_t1);
        printMips("sub\t$t2\t$t0\t$t1");
        printMips("slti\t$t3\t$t2\t1");
        sw2stack_var(this->res, reg_t3);
    } else if (this->op == "==") {
        lwFromStack_var(this->arg1, reg_t0);
        lwFromStack_var(this->arg2, reg_t1);
        printMips("sub\t$t2\t$t0\t$t1");
        printMips("sub\t$t3\t$t1\t$t0");
        printMips("slti\t$t2\t$t2\t1");
        printMips("slti\t$t3\t$t3\t1");
        printMips("and\t$t4\t$t2\t$t3");
        sw2stack_var(this->res, reg_t4);
    } else if (this->op == "!=") {
        lwFromStack_var(this->arg1, reg_t0);
        lwFromStack_var(this->arg2, reg_t1);
        printMips("sub\t$t2\t$t0\t$t1");
        printMips("sub\t$t3\t$t1\t$t0");
        printMips("slti\t$t2\t$t2\t0");
        printMips("slti\t$t3\t$t3\t0");
        printMips("or\t$t4\t$t2\t$t3");
        sw2stack_var(this->res, reg_t4);
    }
}