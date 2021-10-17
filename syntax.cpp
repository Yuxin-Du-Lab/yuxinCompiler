//
// Created by yuxin on 2021/10/3.
//
#include <fstream>
#include <utility>
#include "Token.h"
#include "syntax.h"
#include "iostream"

std::ofstream writeSyntaxFile("output.txt");
const int LeftChild = 0, RightChild = 1;

void writeFile4syntax(Token token, bool scanning = false) {
    if (!scanning) {
        writeSyntaxFile << token.toString() << std::endl;
    }
}

void writeFile4syntax(const std::string &funcName, bool scanning = false) {
    if (!scanning) {
        writeSyntaxFile << "<" << funcName << ">" << std::endl;
    }
}

void syntaxAnalysis(std::vector<Token> &wordList) {
    int pointer = 0;

    CompUnit *compUnit = getCompUnit(wordList, &pointer);
    compUnit->print();
}

CompUnit *getCompUnit(std::vector<Token> &wordList, int *pointer) {
    auto *compUnitptr = new CompUnit();
////      get decls
    while (wordList[*pointer].getIdentity() == CONSTTK ||
           wordList[*pointer].getIdentity() == INTTK &&
           wordList[(*pointer) + 1].getIdentity() == IDENFR &&
           wordList[(*pointer) + 2].getIdentity() != LPARENT) {
        if (wordList[*pointer].getIdentity() == CONSTTK) {
            auto *constDecl = getConstDecl(wordList, pointer);
            compUnitptr->addConstDecl(constDecl);
        } else {
            auto *varDecl = getVarDecl(wordList, pointer);
            compUnitptr->addVarDecl(varDecl);
        }
    }
////      get FuncDefs
    while ((wordList[*pointer].getIdentity() == INTTK || wordList[*pointer].getIdentity() == VOIDTK) &&
           wordList[(*pointer) + 1].getIdentity() == IDENFR &&
           wordList[(*pointer) + 2].getIdentity() == LPARENT) {
        auto *funcDef = getFuncDef(wordList, pointer);
        compUnitptr->addFuncDef(funcDef);
    }

    auto *mainFuncDef = getMainFuncDef(wordList, pointer);
    compUnitptr->setMainFuncDef(mainFuncDef);
    writeFile4syntax("CompUnit");
    return compUnitptr;
}

MainFuncDef *getMainFuncDef(std::vector<Token> &wordList, int *pointer) {
    auto *mainFuncDefptr = new MainFuncDef();
    if (wordList[(*pointer)].getIdentity() == INTTK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }

    if (wordList[(*pointer)].getIdentity() == MAINTK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }

    if (wordList[(*pointer)].getIdentity() == LPARENT) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }

    if (wordList[(*pointer)].getIdentity() == RPARENT) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }

    auto *block = getBlock(wordList, pointer);
    mainFuncDefptr->setBlock(block);
    writeFile4syntax("MainFuncDef");
    return mainFuncDefptr;
}

Block *getBlock(std::vector<Token> &wordList, int *pointer) {
    auto *blockptr = new Block();
    //    here need a loop
    if (wordList[(*pointer)].getIdentity() == LBRACE) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    while (wordList[(*pointer)].getIdentity() != RBRACE) {
        if (wordList[(*pointer)].getIdentity() == CONSTTK) {
//            getConstDecl
            auto *constDecl = getConstDecl(wordList, pointer);
            blockptr->addConstDecl(constDecl);
        } else if (wordList[(*pointer)].getIdentity() == INTTK) {
//            getVarDecl
            auto *varDecl = getVarDecl(wordList, pointer);
            blockptr->addVarDecl(varDecl);
        } else {
//            getStmt
            auto *stmt = getStmt(wordList, pointer);
            blockptr->addStmt(stmt);
        }
    }
//    get a '}'
    if (wordList[*pointer].getIdentity() == RBRACE) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    writeFile4syntax("Block");
    return blockptr;
}

Stmt *getStmt(std::vector<Token> &wordList, int *pointer) {
    Stmt *stmtptr = nullptr;
    if (wordList[(*pointer)].getIdentity() == IFTK) {
//        if
        stmtptr = new IfStmt();
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        if (wordList[(*pointer)].getIdentity() == LPARENT) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
//            getCond
            auto *cond = getCond(wordList, pointer);
            ((IfStmt *) stmtptr)->setCond(cond);
            if (wordList[(*pointer)].getIdentity() == RPARENT) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
            }
            auto *ifStmt = getStmt(wordList, pointer);
            ((IfStmt *) stmtptr)->setIfStmt(ifStmt);
            if (wordList[(*pointer)].getIdentity() == ELSETK) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
                auto *elseStmt = getStmt(wordList, pointer);
                ((IfStmt *) stmtptr)->setElseStmt(elseStmt);
            }
        }
    } else if (wordList[(*pointer)].getIdentity() == WHILETK) {
//        while
        stmtptr = new WhileStmt();
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        if (wordList[(*pointer)].getIdentity() == LPARENT) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
//            getCond
            auto *cond = getCond(wordList, pointer);
            ((WhileStmt *) stmtptr)->setCond(cond);
            if (wordList[(*pointer)].getIdentity() == RPARENT) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
                auto *whileStmt = getStmt(wordList, pointer);
                ((WhileStmt *) stmtptr)->setWhileStmt(whileStmt);
            }
        }
    } else if (wordList[(*pointer)].getIdentity() == BREAKTK) {
//        break
        stmtptr = new LoopStmt();
        ((LoopStmt *) stmtptr)->setToken(&wordList[(*pointer)]);
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        if (wordList[(*pointer)].getIdentity() == SEMICN) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        }
    } else if (wordList[(*pointer)].getIdentity() == CONTINUETK) {
//        continue
        stmtptr = new LoopStmt();
        ((LoopStmt *) stmtptr)->setToken(&wordList[(*pointer)]);
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        if (wordList[(*pointer)].getIdentity() == SEMICN) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        }
    } else if (wordList[(*pointer)].getIdentity() == PRINTFTK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
//        printf
        stmtptr = new PrintfStmt();
        if (wordList[(*pointer)].getIdentity() == LPARENT) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
//            getFormatString
            if (wordList[*pointer].getIdentity() == STRCON) {
                ((PrintfStmt *) stmtptr)->setFormatString(&wordList[*pointer]);
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
            }
            while (wordList[(*pointer)].getIdentity() == COMMA) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
//                getExp
                auto *formatExp = getExp(wordList, pointer);
                ((PrintfStmt *) stmtptr)->addFormatExp(formatExp);
            }
            if (wordList[(*pointer)].getIdentity() == RPARENT) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
                if (wordList[(*pointer)].getIdentity() == SEMICN) {
                    writeFile4syntax(wordList[(*pointer)]);
                    (*pointer)++;
                }
            }
        }
    } else if (wordList[(*pointer)].getIdentity() == RETURNTK) {
//        return
        stmtptr = new ReturnStmt();
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        if (wordList[(*pointer)].getIdentity() == SEMICN) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        } else {
//            getExp
            auto *returnExp = getExp(wordList, pointer);
            ((ReturnStmt *) stmtptr)->setReturnExp(returnExp);
            if (wordList[(*pointer)].getIdentity() == SEMICN) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
            }
        }
    } else if (wordList[(*pointer)].getIdentity() == LBRACE) {
//        Block
        stmtptr = new BlockStmt();
        auto *block = getBlock(wordList, pointer);
        ((BlockStmt *) stmtptr)->setBlock(block);
    } else if (wordList[(*pointer)].getIdentity() == SEMICN) {
//        [exp] ; no exp
        stmtptr = new ExpStmt();
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    } else if (wordList[(*pointer)].getIdentity() == IDENFR) {
        int scan = (*pointer);
        scan++;
//        check is LVal '=' ...
        if (wordList[scan].getIdentity() == LBRACK) {
//            get[Exp]
            scan++;
            getExp(wordList, &scan, true);
            if (wordList[scan].getIdentity() == RBRACK) {
                scan++;
            }
        }
        if (wordList[scan].getIdentity() == LBRACK) {
//            get[Exp]
            scan++;
            getExp(wordList, &scan, true);
            if (wordList[scan].getIdentity() == RBRACK) {
                scan++;
            }
        }
        if (wordList[scan].getIdentity() == ASSIGN) {
//            is LVal   LVal = getint() or LVal = Exp
            auto *lVal = getLVal(wordList, pointer);
            if (wordList[(*pointer)].getIdentity() == ASSIGN) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
                if (wordList[(*pointer)].getIdentity() == GETINTTK) {
//                    LVal = getint()
                    stmtptr = new GetintStmt();
                    ((GetintStmt *) stmtptr)->setLVal(lVal);
                    writeFile4syntax(wordList[(*pointer)]);
                    (*pointer)++;
                    if (wordList[(*pointer)].getIdentity() == LPARENT) {
                        writeFile4syntax(wordList[(*pointer)]);
                        (*pointer)++;
                        if (wordList[(*pointer)].getIdentity() == RPARENT) {
                            writeFile4syntax(wordList[(*pointer)]);
                            (*pointer)++;
                            if (wordList[(*pointer)].getIdentity() == SEMICN) {
                                writeFile4syntax(wordList[(*pointer)]);
                                (*pointer)++;
                            }
                        }
                    }
                } else {
//                    LVal = Exp
                    stmtptr = new LValStmt();
                    ((LValStmt *) stmtptr)->setLVal(lVal);
                    auto *exp = getExp(wordList, pointer);
                    ((LValStmt *) stmtptr)->setExp(exp);
                    if (wordList[(*pointer)].getIdentity() == SEMICN) {
                        writeFile4syntax(wordList[(*pointer)]);
                        (*pointer)++;
                    }
                }
            }
        } else {
//            is exp
//            getExp
            stmtptr = new ExpStmt();
            auto *exp = getExp(wordList, pointer);
            ((ExpStmt *) stmtptr)->setExp(exp);
            if (wordList[*pointer].getIdentity() == SEMICN) {
                writeFile4syntax(wordList[*pointer]);
                (*pointer)++;
            }
        }
    } else {
//        getExp
        stmtptr = new ExpStmt();
        auto *exp = getExp(wordList, pointer);
        ((ExpStmt *) stmtptr)->setExp(exp);
        if (wordList[*pointer].getIdentity() == SEMICN) {
            writeFile4syntax(wordList[*pointer]);
            (*pointer)++;
        }
    }
    writeFile4syntax("Stmt");
    return stmtptr;
}

FuncDef *getFuncDef(std::vector<Token> &wordList, int *pointer) {
    auto *funcDefptr = new FuncDef();
    auto *funcType = getFuncType(wordList, pointer);
    funcDefptr->setFuncType(funcType);
    if (wordList[*pointer].getIdentity() == IDENFR) {
        funcDefptr->setFuncIdent(&wordList[*pointer]);
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        if (wordList[*pointer].getIdentity() == LPARENT) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
            if (wordList[*pointer].getIdentity() != RPARENT) {
                auto *funcFParams = getFuncFParams(wordList, pointer);
                funcDefptr->setFuncFParams(funcFParams);
            }
            if (wordList[*pointer].getIdentity() == RPARENT) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
                auto *funcBlock = getBlock(wordList, pointer);
                funcDefptr->setFuncBlock(funcBlock);
            }
        }
    }
    writeFile4syntax("FuncDef");
    return funcDefptr;
}

FuncType *getFuncType(std::vector<Token> &wordList, int *pointer) {
    auto *funcTypeptr = new FuncType();
    if (wordList[*pointer].getIdentity() == VOIDTK ||
        wordList[*pointer].getIdentity() == INTTK) {
        funcTypeptr->setFuncType(&wordList[*pointer]);
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    writeFile4syntax("FuncType");
    return funcTypeptr;
}

FuncFParams *getFuncFParams(std::vector<Token> &wordList, int *pointer) {
    auto *funcFParamsptr = new FuncFParams();
    auto *funcFParam = getFuncFParam(wordList, pointer);
    funcFParamsptr->addFuncFParam(funcFParam);
    while (wordList[*pointer].getIdentity() == COMMA) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        funcFParam = getFuncFParam(wordList, pointer);
        funcFParamsptr->addFuncFParam(funcFParam);
    }
    writeFile4syntax("FuncFParams");
    return funcFParamsptr;
}

FuncFParam *getFuncFParam(std::vector<Token> &wordList, int *pointer) {
    auto *funcFParamptr = new FuncFParam();
    int row = 0;
    if (wordList[*pointer].getIdentity() == INTTK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    if (wordList[*pointer].getIdentity() == IDENFR) {
        funcFParamptr->setFuncFParamIdent(&wordList[*pointer]);
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    if (wordList[*pointer].getIdentity() == LBRACK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        row++;
        if (wordList[*pointer].getIdentity() == RBRACK) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        }
        if (wordList[*pointer].getIdentity() == LBRACK) {
            row++;
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
            auto *constExpRow = getConstExp(wordList, pointer);
            funcFParamptr->setConstExp(constExpRow);
            if (wordList[*pointer].getIdentity() == RBRACK) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
            }
        }
    }
    funcFParamptr->setRow(row);
    writeFile4syntax("FuncFParam");
    return funcFParamptr;
}

FuncRParams *getFuncRParams(std::vector<Token> &wordList, int *pointer, bool scanning) {
    auto *funcRParamsptr = new FuncRParams();
    getExp(wordList, pointer, scanning);
    while (wordList[*pointer].getIdentity() == COMMA) {
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
        getExp(wordList, pointer, scanning);
    }
    writeFile4syntax("FuncRParams", scanning);
    return funcRParamsptr;
}

UnaryExp *getUnaryExp(std::vector<Token> &wordList, int *pointer, bool scanning) {
    UnaryExp *unaryExpptr;
    if (wordList[*pointer].getIdentity() == IDENFR) {
        int scan = *pointer;
        scan++;
        if (wordList[scan].getIdentity() == LPARENT) {
//            function
            unaryExpptr = new FuncUnaryExp();
            ((FuncUnaryExp *) unaryExpptr)->setFuncUnaryIdent(&wordList[(*pointer)]);
            writeFile4syntax(wordList[(*pointer)], scanning);
            (*pointer)++;  // for ident
            if (wordList[*pointer].getIdentity() == LPARENT) {
//                [FuncRParams]
                writeFile4syntax(wordList[(*pointer)], scanning);
                (*pointer)++;   // for (
                if (wordList[*pointer].getIdentity() != RPARENT) {
                    auto *funcRParams = getFuncRParams(wordList, pointer, scanning);
                    ((FuncUnaryExp *) unaryExpptr)->setFuncRParams(funcRParams);
                }
                if (wordList[*pointer].getIdentity() == RPARENT) {
                    writeFile4syntax(wordList[(*pointer)], scanning);
                    (*pointer)++;
                }
            }
        } else {
//            LVal
//            getPrimaryExp
            unaryExpptr = new PrimaryUnaryExp();
            auto *primaryExp = getPrimaryExp(wordList, pointer, scanning);
            ((PrimaryUnaryExp *) unaryExpptr)->setPrimaryExp(primaryExp);
        }
    } else {
//      PrimaryExp or UnaryOp UnaryExp or exception
        if (wordList[*pointer].getIdentity() == PLUS ||
            wordList[*pointer].getIdentity() == MINU ||
            wordList[*pointer].getIdentity() == NOT
                ) {
//            UnaryOp UnaryExp
            unaryExpptr = new UnaryUnaryExp();
            auto *unaryOp = getUnaryOp(wordList, pointer, scanning);
            auto *unaryExp = getUnaryExp(wordList, pointer, scanning);
            ((UnaryUnaryExp *) unaryExpptr)->setUnaryOp(unaryOp);
            ((UnaryUnaryExp *) unaryExpptr)->setUnaryExp(unaryExp);
        } else {
//            getPrimaryExp
            unaryExpptr = new PrimaryUnaryExp();
            auto *primaryExp = getPrimaryExp(wordList, pointer, scanning);
            ((PrimaryUnaryExp *) unaryExpptr)->setPrimaryExp(primaryExp);
        }
    }
    writeFile4syntax("UnaryExp", scanning);
    return unaryExpptr;
}

UnaryOp *getUnaryOp(std::vector<Token> &wordList, int *pointer, bool scanning) {
    auto *unaryOpptr = new UnaryOp();
    if (wordList[*pointer].getIdentity() == PLUS ||
        wordList[*pointer].getIdentity() == MINU ||
        wordList[*pointer].getIdentity() == NOT
            ) {
        unaryOpptr->setUnaryOp(&wordList[*pointer]);
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
    }
    writeFile4syntax("UnaryOp", scanning);
    return unaryOpptr;
}

PrimaryExp *getPrimaryExp(std::vector<Token> &wordList, int *pointer, bool scanning) {
    PrimaryExp *primaryExpptr = nullptr;
    if (wordList[*pointer].getIdentity() == LPARENT) {
//        (exp)
        primaryExpptr = new ExpPrimaryExp();
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
//        getExp
        auto *exp = getExp(wordList, pointer, scanning);
        ((ExpPrimaryExp *) primaryExpptr)->setPrimaryExp(exp);
        if (wordList[*pointer].getIdentity() == RPARENT) {
            writeFile4syntax(wordList[(*pointer)], scanning);
            (*pointer)++;
        }
    } else if (wordList[*pointer].getIdentity() == IDENFR) {
//        lVal
        primaryExpptr = new LValPrimaryExp();
        auto *lVal = getLVal(wordList, pointer, scanning);
        ((LValPrimaryExp *) primaryExpptr)->setPrimaryLVal(lVal);
    } else if (wordList[*pointer].getIdentity() == INTCON) {
//        Number
        primaryExpptr = new NumberPrimaryExp();
        auto *number = getNumber(wordList, pointer, scanning);
        ((NumberPrimaryExp *) primaryExpptr)->setPrimaryNumber(number);
    }
    writeFile4syntax("PrimaryExp", scanning);
    return primaryExpptr;
}

Number *getNumber(std::vector<Token> &wordList, int *pointer, bool scanning) {
    auto *numberptr = new Number();
    if (wordList[*pointer].getIdentity() == INTCON) {
        numberptr->setIntConst(&wordList[*pointer]);
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
    }
    writeFile4syntax("Number", scanning);
    return numberptr;
}

Cond *getCond(std::vector<Token> &wordList, int *pointer) {
    auto *condptr = new Cond();
    auto *lOrExp = getLOrExp(wordList, pointer);
    condptr->setLOrExp(lOrExp);
    writeFile4syntax("Cond");
    return condptr;
}

LOrExp *getLOrExp(std::vector<Token> &wordList, int *pointer) {
    auto *lOrExpptr = new LOrExp();
    auto *lAndExp = getLAndExp(wordList, pointer);
    lOrExpptr->addLAndExp(lAndExp);
    while (wordList[(*pointer)].getIdentity() == OR) {
        writeFile4syntax("LOrExp");
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        lAndExp = getLAndExp(wordList, pointer);
        lOrExpptr->addLAndExp(lAndExp);
    }
    writeFile4syntax("LOrExp");
    return lOrExpptr;
}

LAndExp *getLAndExp(std::vector<Token> &wordList, int *pointer) {
    auto *lANdExpptr = new LAndExp();
    auto *eqExp = getEqExp(wordList, pointer);
    lANdExpptr->addEqExp(eqExp);
    while (wordList[(*pointer)].getIdentity() == AND) {
        writeFile4syntax("LAndExp");
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        eqExp = getEqExp(wordList, pointer);
        lANdExpptr->addEqExp(eqExp);
    }
    writeFile4syntax("LAndExp");
    return lANdExpptr;
}

EqExp *getEqExp(std::vector<Token> &wordList, int *pointer) {
    auto *eqExpptr = new EqExp();
    auto *root = new EqOpTree();

    auto *relExp = getRelExp(wordList, pointer);

//      set first value to root
    root->setLeafValue(relExp);
    root->setIsLeaf(true);

    while (wordList[(*pointer)].getIdentity() == EQL || wordList[(*pointer)].getIdentity() == NEQ) {
//        get new op
        auto *op = &wordList[(*pointer)];
        auto *newOp = new EqOpTree();
        newOp->setIsLeaf(false);
        newOp->setOp(op);
//        set left tree
        newOp->setBranch(root, LeftChild);

        writeFile4syntax("EqExp");
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
//        get new value
        relExp = getRelExp(wordList, pointer);
        auto newValue = new EqOpTree();
        newValue->setIsLeaf(true);
        newValue->setLeafValue(relExp);
        newOp->setBranch(newValue, RightChild);
//        move root
        root = newOp;
    }
//    set eqExpptr's root
    eqExpptr->setEqExpRoot(root);
    writeFile4syntax("EqExp");
    return eqExpptr;
}

RelExp *getRelExp(std::vector<Token> &wordList, int *pointer) {
    auto *relExpptr = new RelExp();
    auto *root = new RelOpTree();
    auto *addExp = getAddExp(wordList, pointer);

//      set first value to root
    root->setIsLeaf(true);
    root->setLeafValue(addExp);

    while (wordList[(*pointer)].getIdentity() == LSS ||
           wordList[(*pointer)].getIdentity() == LEQ ||
           wordList[(*pointer)].getIdentity() == GRE ||
           wordList[(*pointer)].getIdentity() == GEQ
            ) {
//        get new op
        auto *op = &wordList[(*pointer)];
        auto *newOp = new RelOpTree();
        newOp->setIsLeaf(false);
        newOp->setOp(op);
//        set left tree
        newOp->setBranch(root, LeftChild);

        writeFile4syntax("RelExp");
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
//        get new value
        addExp = getAddExp(wordList, pointer);
        auto *newValue = new RelOpTree();
        newValue->setIsLeaf(true);
        newValue->setLeafValue(addExp);
        newOp->setBranch(newValue, RightChild);
//        move root
        root = newOp;
    }
//    set relExpptr's root
    relExpptr->setRoot(root);
    writeFile4syntax("RelExp");
    return relExpptr;
}

AddExp *getAddExp(std::vector<Token> &wordList, int *pointer, bool scanning) {
    auto *addExpptr = new AddExp();
    auto *root = new AddOpTree();
    auto *mulExp = getMulExp(wordList, pointer, scanning);
//      set first value to root
    root->setIsLeaf(true);
    root->serValue(mulExp);

    while (wordList[(*pointer)].getIdentity() == PLUS ||
           wordList[(*pointer)].getIdentity() == MINU
            ) {
//        get new op
        auto *op = &wordList[(*pointer)];
        auto *newOp = new AddOpTree();
        newOp->setIsLeaf(false);
        newOp->setOp(op);
//        set left tree
        newOp->setBranch(root, LeftChild);

        writeFile4syntax("AddExp", scanning);
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
//        get new value
        mulExp = getMulExp(wordList, pointer, scanning);
        auto *newValue = new AddOpTree();
        newValue->setIsLeaf(true);
        newValue->serValue(mulExp);
        newOp->setBranch(newValue, RightChild);
//        move root
        root = newOp;
    }
//    set addExpptr's root
    addExpptr->setRoot(root);
    writeFile4syntax("AddExp", scanning);
    return addExpptr;
}

MulExp *getMulExp(std::vector<Token> &wordList, int *pointer, bool scanning) {
    auto *mulExpptr = new MulExp();
    auto *root = new MulOpTree();
    auto *unaryExp = getUnaryExp(wordList, pointer, scanning);

//      set first value to root
    root->setIsLeaf(true);
    root->serValue(unaryExp);

    while (wordList[(*pointer)].getIdentity() == MULT ||
           wordList[(*pointer)].getIdentity() == DIV ||
           wordList[(*pointer)].getIdentity() == MOD
            ) {
//        get new op
        auto *op = &wordList[(*pointer)];
        auto *newOp = new MulOpTree();
        newOp->setIsLeaf(false);
        newOp->setOp(op);
//        set left tree
        newOp->setBranch(root, LeftChild);

        writeFile4syntax("MulExp", scanning);
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
//        get new value
        unaryExp = getUnaryExp(wordList, pointer, scanning);
        auto *newValue = new MulOpTree();
        newValue->setIsLeaf(true);
        newValue->serValue(unaryExp);
        newOp->setBranch(newValue, RightChild);
//        move root
        root = newOp;
    }
//    set mulExpptr's root
    mulExpptr->setRoot(root);
    writeFile4syntax("MulExp", scanning);
    return mulExpptr;
}

LVal *getLVal(std::vector<Token> &wordList, int *pointer, bool scanning) {
    auto *lValptr = new LVal();
    int row = 0;
    if (wordList[(*pointer)].getIdentity() == IDENFR) {
        lValptr->setLValIdent(&wordList[(*pointer)]);
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
    }
    if (wordList[(*pointer)].getIdentity() == LBRACK) {
//            getExp
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
        auto *exp = getExp(wordList, pointer, scanning);
        lValptr->setArrayExp(exp, row);
        row++;
        if (wordList[(*pointer)].getIdentity() == RBRACK) {
            writeFile4syntax(wordList[(*pointer)], scanning);
            (*pointer)++;
        }
    }
    if (wordList[(*pointer)].getIdentity() == LBRACK) {
//            getExp
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
        auto *exp = getExp(wordList, pointer, scanning);
        lValptr->setArrayExp(exp, row);
        row++;
        if (wordList[(*pointer)].getIdentity() == RBRACK) {
            writeFile4syntax(wordList[(*pointer)], scanning);
            (*pointer)++;
        }
    }
    lValptr->setRow(row);
    writeFile4syntax("LVal", scanning);
    return lValptr;
}

ConstDecl *getConstDecl(std::vector<Token> &wordList, int *pointer) {
    auto *constDeclptr = new ConstDecl();
    if (wordList[(*pointer)].getIdentity() == CONSTTK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    if (wordList[(*pointer)].getIdentity() == INTTK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    auto *constDef = getConstDef(wordList, pointer);
    constDeclptr->addConstDef(constDef);
    while (wordList[(*pointer)].getIdentity() == COMMA) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        constDef = getConstDef(wordList, pointer);
        constDeclptr->addConstDef(constDef);
    }
    if (wordList[(*pointer)].getIdentity() == SEMICN) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    writeFile4syntax("ConstDecl");
    return constDeclptr;
}

ConstDef *getConstDef(std::vector<Token> &wordList, int *pointer) {
    auto *constDefptr = new ConstDef();
    int row = 0;
    if (wordList[(*pointer)].getIdentity() == IDENFR) {
        constDefptr->setIdent(&wordList[(*pointer)]);
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    if (wordList[(*pointer)].getIdentity() == LBRACK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        auto constExp = getConstExp(wordList, pointer);
        constDefptr->setArrayConstExp(constExp, row);
        row++;
        if (wordList[(*pointer)].getIdentity() == RBRACK) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        }
    }
    if (wordList[(*pointer)].getIdentity() == LBRACK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        auto constExp = getConstExp(wordList, pointer);
        constDefptr->setArrayConstExp(constExp, row);
        row++;
        if (wordList[(*pointer)].getIdentity() == RBRACK) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        }
    }
    if (wordList[(*pointer)].getIdentity() == ASSIGN) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
//    getConstInitVal();
    auto *constInitVal = getConstInitVal(wordList, pointer);
    constDefptr->setConstInitVal(constInitVal);
    constDefptr->setRow(row);
    writeFile4syntax("ConstDef");
    return constDefptr;
}

ConstInitVal *getConstInitVal(std::vector<Token> &wordList, int *pointer) {
    auto *constInitValptr = new ConstInitVal();
    int row = 0;
    if (wordList[(*pointer)].getIdentity() == LBRACE) {
//        '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
        row++;
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
//        [ ConstInitVal { ',' ConstInitVal } ]
        if (wordList[(*pointer)].getIdentity() != RBRACE) {
            auto *constInitVal = getConstInitVal(wordList, pointer);
            constInitValptr->addConstInitVal(constInitVal);
            while (wordList[(*pointer)].getIdentity() == COMMA) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
                constInitVal = getConstInitVal(wordList, pointer);
                constInitValptr->addConstInitVal(constInitVal);
            }
        }
        if (wordList[(*pointer)].getIdentity() == RBRACE) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        }
    } else {
//        getConstExp
        auto *constExp = getConstExp(wordList, pointer);
        constInitValptr->setConstExp(constExp);
    }
    constInitValptr->setRow(row);
    writeFile4syntax("ConstInitVal");
    return constInitValptr;
}

VarDecl *getVarDecl(std::vector<Token> &wordList, int *pointer) {
    auto *varDeclptr = new VarDecl();
    if (wordList[(*pointer)].getIdentity() == INTTK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    auto *varDef = getVarDef(wordList, pointer);
    varDeclptr->addVarDef(varDef);
    while (wordList[(*pointer)].getIdentity() == COMMA) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        auto *varDef = getVarDef(wordList, pointer);
        varDeclptr->addVarDef(varDef);
    }
    if (wordList[(*pointer)].getIdentity() == SEMICN) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    writeFile4syntax("VarDecl");
    return varDeclptr;
}

VarDef *getVarDef(std::vector<Token> &wordList, int *pointer) {
    auto *varDefptr = new VarDef();
    int row = 0;
    varDefptr->setHasInitVal(false);
    if (wordList[(*pointer)].getIdentity() == IDENFR) {
        varDefptr->setIdent(&wordList[(*pointer)]);
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    if (wordList[(*pointer)].identity == LBRACK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        auto *constExp = getConstExp(wordList, pointer);
        varDefptr->setArrayConstExp(constExp, row);
        row++;
        if (wordList[(*pointer)].getIdentity() == RBRACK) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        }
    }
    if (wordList[(*pointer)].identity == LBRACK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        auto *constExp = getConstExp(wordList, pointer);
        varDefptr->setArrayConstExp(constExp, row);
        row++;
        if (wordList[(*pointer)].getIdentity() == RBRACK) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        }
    }
    if (wordList[(*pointer)].getIdentity() == ASSIGN) {
//        has InitVal
        varDefptr->setHasInitVal(true);
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        auto *initVal = getInitVal(wordList, pointer);
        varDefptr->setInitVal(initVal);
    }
    varDefptr->setRow(row);
    writeFile4syntax("VarDef");
    return varDefptr;
}

InitVal *getInitVal(std::vector<Token> &wordList, int *pointer) {
    auto *initValptr = new InitVal();
    int row = 0;
    if (wordList[(*pointer)].getIdentity() == LBRACE) {
        row++;
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        if (wordList[(*pointer)].getIdentity() == RBRACE) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        } else {
            auto *initVal = getInitVal(wordList, pointer);
            initValptr->addInitVal(initVal);
            while (wordList[(*pointer)].getIdentity() == COMMA) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
                initVal = getInitVal(wordList, pointer);
                initValptr->addInitVal(initVal);
            }
            if (wordList[(*pointer)].getIdentity() == RBRACE) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
            }
        }
    } else {
//        getExp();
        auto *exp = getExp(wordList, pointer);
        initValptr->setExp(exp);
    }
    initValptr->setRow(row);
    writeFile4syntax("InitVal");
    return initValptr;
}

ConstExp *getConstExp(std::vector<Token> &wordList, int *pointer) {
    auto *constExpptr = new ConstExp();
    auto *addExp = getAddExp(wordList, pointer);
    constExpptr->setAddExp(addExp);
    writeFile4syntax("ConstExp");
    return constExpptr;
}

Exp *getExp(std::vector<Token> &wordList, int *pointer, bool scanning) {
    auto *expptr = new Exp();
    auto *addExp = getAddExp(wordList, pointer, scanning);
    expptr->setAddExp(addExp);
    writeFile4syntax("Exp", scanning);
    return expptr;
}