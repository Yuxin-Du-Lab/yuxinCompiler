//
// Created by yuxin on 2021/10/3.
//
#include <fstream>
#include <utility>
#include "Token.h"
#include "syntax.h"
#include "iostream"

std::ofstream writeSyntaxFile("output.txt");

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


void syntaxAnalysis(std::vector<Token> wordList) {
    int pointer = 0;
    getCompUnit(std::move(wordList), &pointer);
}

CompUnit *getCompUnit(std::vector<Token> wordList, int *pointer) {
    auto *compUnitptr = new CompUnit;
////      get decls
    while (wordList[*pointer].getIdentity() == CONSTTK ||
           wordList[*pointer].getIdentity() == INTTK &&
           wordList[(*pointer) + 1].getIdentity() == IDENFR &&
           wordList[(*pointer) + 2].getIdentity() != LPARENT) {
        if (wordList[*pointer].getIdentity() == CONSTTK) {
            getConstDecl(wordList, pointer);
        } else {
            getVarDecl(wordList, pointer);
        }
    }
////      get FuncDefs
    while ((wordList[*pointer].getIdentity() == INTTK || wordList[*pointer].getIdentity() == VOIDTK) &&
           wordList[(*pointer) + 1].getIdentity() == IDENFR &&
           wordList[(*pointer) + 2].getIdentity() == LPARENT) {
        getFuncDef(wordList, pointer);
    }

    getMainFuncDef(wordList, pointer);
    writeFile4syntax("CompUnit");
    return compUnitptr;
}

MainFuncDef *getMainFuncDef(std::vector<Token> wordList, int *pointer) {
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

    getBlock(wordList, pointer);
    writeFile4syntax("MainFuncDef");
    return mainFuncDefptr;
}

Block *getBlock(std::vector<Token> wordList, int *pointer) {
    auto *blockptr = new Block();
    //    here need a loop
    if (wordList[(*pointer)].getIdentity() == LBRACE) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    while (wordList[(*pointer)].getIdentity() != RBRACE) {
        if (wordList[(*pointer)].getIdentity() == CONSTTK) {
//            getConstDecl
            getConstDecl(wordList, pointer);
        } else if (wordList[(*pointer)].getIdentity() == INTTK) {
//            getVarDecl
            getVarDecl(wordList, pointer);
        } else {
//            getSmt
            getSmt(wordList, pointer);
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

Stmt *getSmt(std::vector<Token> wordList, int *pointer) {
    auto *stmtptr = new Stmt();
    if (wordList[(*pointer)].getIdentity() == IFTK) {
//        if
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        if (wordList[(*pointer)].getIdentity() == LPARENT) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
//            getCond
            getCond(wordList, pointer);
            if (wordList[(*pointer)].getIdentity() == RPARENT) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
            }
            getSmt(wordList, pointer);
            if (wordList[(*pointer)].getIdentity() == ELSETK) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
                getSmt(wordList, pointer);
            }
        }
    } else if (wordList[(*pointer)].getIdentity() == WHILETK) {
//        while
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        if (wordList[(*pointer)].getIdentity() == LPARENT) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
//            getCond
            getCond(wordList, pointer);
            if (wordList[(*pointer)].getIdentity() == RPARENT) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
                getSmt(wordList, pointer);
            }
        }
    } else if (wordList[(*pointer)].getIdentity() == BREAKTK) {
//        break
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        if (wordList[(*pointer)].getIdentity() == SEMICN) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        }
    } else if (wordList[(*pointer)].getIdentity() == CONTINUETK) {
//        continue
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
        if (wordList[(*pointer)].getIdentity() == LPARENT) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
//            getFormatString
            if (wordList[*pointer].getIdentity() == STRCON) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
            }
            while (wordList[(*pointer)].getIdentity() == COMMA) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
//                getExp
                getExp(wordList, pointer);
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
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        if (wordList[(*pointer)].getIdentity() == SEMICN) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        } else {
//            getExp
            getExp(wordList, pointer);
            if (wordList[(*pointer)].getIdentity() == SEMICN) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
            }
        }
    } else if (wordList[(*pointer)].getIdentity() == LBRACE) {
//        Block
        getBlock(wordList, pointer);
    } else if (wordList[(*pointer)].getIdentity() == SEMICN) {
//        [exp] ; no exp
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    } else if (wordList[(*pointer)].getIdentity() == IDENFR) {
        int scan = (*pointer);
        scan++;
//        check is LVal '=' ...
        if (wordList[scan].getIdentity() == LBRACK) {
//            getExp
            scan++;
            getExp(wordList, &scan, true);
            if (wordList[scan].getIdentity() == RBRACK) {
                scan++;
            }
        }
        if (wordList[scan].getIdentity() == LBRACK) {
//            getExp
            scan++;
            getExp(wordList, &scan, true);
            if (wordList[scan].getIdentity() == RBRACK) {
                scan++;
            }
        }
        if (wordList[scan].getIdentity() == ASSIGN) {
//            is LVal
            getLVal(wordList, pointer);
            if (wordList[(*pointer)].getIdentity() == ASSIGN) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
                if (wordList[(*pointer)].getIdentity() == GETINTTK) {
//                    getint
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
//                    getExp
                    getExp(wordList, pointer);
                    if (wordList[(*pointer)].getIdentity() == SEMICN) {
                        writeFile4syntax(wordList[(*pointer)]);
                        (*pointer)++;
                    }
                }
            }
        } else {
//            is exp
//            getExp
            getExp(wordList, pointer);
            if (wordList[*pointer].getIdentity() == SEMICN) {
                writeFile4syntax(wordList[*pointer]);
                (*pointer)++;
            }
        }
    } else {
//        getExp
        getExp(wordList, pointer);
        if (wordList[*pointer].getIdentity() == SEMICN) {
            writeFile4syntax(wordList[*pointer]);
            (*pointer)++;
        }
    }
    writeFile4syntax("Stmt");
    return stmtptr;
}

FuncDef *getFuncDef(std::vector<Token> wordList, int *pointer) {
    auto *funcDefptr = new FuncDef();
    getFuncType(wordList, pointer);
    if (wordList[*pointer].getIdentity() == IDENFR) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        if (wordList[*pointer].getIdentity() == LPARENT) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
            if (wordList[*pointer].getIdentity() != RPARENT) {
                getFuncFParams(wordList, pointer);
            }
            if (wordList[*pointer].getIdentity() == RPARENT) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
                getBlock(wordList, pointer);
            }
        }
    }
    writeFile4syntax("FuncDef");
    return funcDefptr;
}

FuncType *getFuncType(std::vector<Token> wordList, int *pointer) {
    auto *funcTypeptr = new FuncType();
    if (wordList[*pointer].getIdentity() == VOIDTK ||
        wordList[*pointer].getIdentity() == INTTK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    writeFile4syntax("FuncType");
    return funcTypeptr;
}

FuncFParams *getFuncFParams(std::vector<Token> wordList, int *pointer) {
    auto *funcFParamsptr = new FuncFParams();
    getFuncFParam(wordList, pointer);
    while (wordList[*pointer].getIdentity() == COMMA) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        getFuncFParam(wordList, pointer);
    }
    writeFile4syntax("FuncFParams");
    return funcFParamsptr;
}

FuncFParam *getFuncFParam(std::vector<Token> wordList, int *pointer) {
    auto *funcFParamptr = new FuncFParam();
    if (wordList[*pointer].getIdentity() == INTTK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    if (wordList[*pointer].getIdentity() == IDENFR) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    if (wordList[*pointer].getIdentity() == LBRACK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        if (wordList[*pointer].getIdentity() == RBRACK) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        }
        if (wordList[*pointer].getIdentity() == LBRACK) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
            getConstExp(wordList, pointer);
            if (wordList[*pointer].getIdentity() == RBRACK) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
            }
        }
    }
    writeFile4syntax("FuncFParam");
    return funcFParamptr;
}

FuncRParams *getFuncRParams(std::vector<Token> wordList, int *pointer, bool scanning) {
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

UnaryExp *getUnaryExp(std::vector<Token> wordList, int *pointer, bool scanning) {
    auto *unaryExpptr = new UnaryExp();
    if (wordList[*pointer].getIdentity() == IDENFR) {
        int scan = *pointer;
        scan++;
        if (wordList[scan].getIdentity() == LPARENT) {
//            function
            writeFile4syntax(wordList[(*pointer)], scanning);
            (*pointer)++;  // for ident
            if (wordList[*pointer].getIdentity() == LPARENT) {
//                [FuncRParams]
                writeFile4syntax(wordList[(*pointer)], scanning);
                (*pointer)++;   // for (
                if (wordList[*pointer].getIdentity() != RPARENT) {
                    getFuncRParams(wordList, pointer, scanning);
                }
                if (wordList[*pointer].getIdentity() == RPARENT) {
                    writeFile4syntax(wordList[(*pointer)], scanning);
                    (*pointer)++;
                }
            }
        } else {
//            LVal
//            getPrimaryExp
            getPrimaryExp(wordList, pointer, scanning);
        }
    } else {
//      PrimaryExp or UnaryOp UnaryExp or exception
        if (wordList[*pointer].getIdentity() == PLUS ||
            wordList[*pointer].getIdentity() == MINU ||
            wordList[*pointer].getIdentity() == NOT
                ) {
            getUnaryOp(wordList, pointer, scanning);
            getUnaryExp(wordList, pointer, scanning);
        } else {
//            getPrimaryExp
            getPrimaryExp(wordList, pointer, scanning);
        }
    }
    writeFile4syntax("UnaryExp", scanning);
    return unaryExpptr;
}

UnaryOp *getUnaryOp(std::vector<Token> wordList, int *pointer, bool scanning) {
    auto *unaryOpptr = new UnaryOp();
    if (wordList[*pointer].getIdentity() == PLUS ||
        wordList[*pointer].getIdentity() == MINU ||
        wordList[*pointer].getIdentity() == NOT
            ) {
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
    }
    writeFile4syntax("UnaryOp", scanning);
    return unaryOpptr;
}

PrimaryExp *getPrimaryExp(std::vector<Token> wordList, int *pointer, bool scanning) {
    auto *primaryExpptr = new PrimaryExp();
    if (wordList[*pointer].getIdentity() == LPARENT) {
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
//        getExp
        getExp(wordList, pointer, scanning);
        if (wordList[*pointer].getIdentity() == RPARENT) {
            writeFile4syntax(wordList[(*pointer)], scanning);
            (*pointer)++;
        }
    } else if (wordList[*pointer].getIdentity() == IDENFR) {
        getLVal(wordList, pointer, scanning);
    } else if (wordList[*pointer].getIdentity() == INTCON) {
        getNumber(wordList, pointer, scanning);
    }
    writeFile4syntax("PrimaryExp", scanning);
    return primaryExpptr;
}

Number *getNumber(std::vector<Token> wordList, int *pointer, bool scanning) {
    auto *numberptr = new Number();
    if (wordList[*pointer].getIdentity() == INTCON) {
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
    }
    writeFile4syntax("Number", scanning);
    return numberptr;
}

Cond *getCond(std::vector<Token> wordList, int *pointer) {
    auto *condptr = new Cond();
    getLOrExp(std::move(wordList), pointer);
    writeFile4syntax("Cond");
    return condptr;
}

LOrExp *getLOrExp(std::vector<Token> wordList, int *pointer) {
    auto *lOrExpptr = new LOrExp();
    getLAndExp(wordList, pointer);
    while (wordList[(*pointer)].getIdentity() == OR) {
        writeFile4syntax("LOrExp");
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        getLAndExp(wordList, pointer);
    }
    writeFile4syntax("LOrExp");
    return lOrExpptr;
}

LAndExp *getLAndExp(std::vector<Token> wordList, int *pointer) {
    auto *lANdExpptr = new LAndExp();
    getEqExp(wordList, pointer);
    while (wordList[(*pointer)].getIdentity() == AND) {
        writeFile4syntax("LAndExp");
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        getEqExp(wordList, pointer);
    }
    writeFile4syntax("LAndExp");
    return lANdExpptr;
}

EqExp *getEqExp(std::vector<Token> wordList, int *pointer) {
    auto *eqExpptr = new EqExp();
    getRelExp(wordList, pointer);
    while (wordList[(*pointer)].getIdentity() == EQL || wordList[(*pointer)].getIdentity() == NEQ) {
        writeFile4syntax("EqExp");
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        getRelExp(wordList, pointer);
    }
    writeFile4syntax("EqExp");
    return eqExpptr;
}

RelExp *getRelExp(std::vector<Token> wordList, int *pointer) {
    auto *relExpptr = new RelExp();
    getAddExp(wordList, pointer);
    while (wordList[(*pointer)].getIdentity() == LSS ||
           wordList[(*pointer)].getIdentity() == LEQ ||
           wordList[(*pointer)].getIdentity() == GRE ||
           wordList[(*pointer)].getIdentity() == GEQ
            ) {
        writeFile4syntax("RelExp");
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        getAddExp(wordList, pointer);
    }
    writeFile4syntax("RelExp");
    return relExpptr;
}

AddExp *getAddExp(std::vector<Token> wordList, int *pointer, bool scanning) {
    auto *addExpptr = new AddExp();
    getMulExp(wordList, pointer, scanning);
    while (wordList[(*pointer)].getIdentity() == PLUS ||
           wordList[(*pointer)].getIdentity() == MINU
            ) {
        writeFile4syntax("AddExp", scanning);
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
        getMulExp(wordList, pointer, scanning);
    }
    writeFile4syntax("AddExp", scanning);
    return addExpptr;
}

MulExp *getMulExp(std::vector<Token> wordList, int *pointer, bool scanning) {
    auto *mulExpptr = new MulExp();
    getUnaryExp(wordList, pointer, scanning);
    while (wordList[(*pointer)].getIdentity() == MULT ||
           wordList[(*pointer)].getIdentity() == DIV ||
           wordList[(*pointer)].getIdentity() == MOD
            ) {
        writeFile4syntax("MulExp", scanning);
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
        getUnaryExp(wordList, pointer, scanning);
    }
    writeFile4syntax("MulExp", scanning);
    return mulExpptr;
}

LVal *getLVal(std::vector<Token> wordList, int *pointer, bool scanning) {
    auto *lValptr = new LVal();
    if (wordList[(*pointer)].getIdentity() == IDENFR) {
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
    }
    if (wordList[(*pointer)].getIdentity() == LBRACK) {
//            getExp
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
        getExp(wordList, pointer, scanning);
        if (wordList[(*pointer)].getIdentity() == RBRACK) {
            writeFile4syntax(wordList[(*pointer)], scanning);
            (*pointer)++;
        }
    }
    if (wordList[(*pointer)].getIdentity() == LBRACK) {
//            getExp
        writeFile4syntax(wordList[(*pointer)], scanning);
        (*pointer)++;
        getExp(wordList, pointer, scanning);
        if (wordList[(*pointer)].getIdentity() == RBRACK) {
            writeFile4syntax(wordList[(*pointer)], scanning);
            (*pointer)++;
        }
    }
    writeFile4syntax("LVal", scanning);
    return lValptr;
}

ConstDecl *getConstDecl(std::vector<Token> wordList, int *pointer) {
    auto *constDeclptr = new ConstDecl();
    if (wordList[(*pointer)].getIdentity() == CONSTTK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    if (wordList[(*pointer)].getIdentity() == INTTK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    getConstDef(wordList, pointer);
    while (wordList[(*pointer)].getIdentity() == COMMA) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        getConstDef(wordList, pointer);
    }
    if (wordList[(*pointer)].getIdentity() == SEMICN) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    writeFile4syntax("ConstDecl");
    return constDeclptr;
}

ConstDef *getConstDef(std::vector<Token> wordList, int *pointer) {
    auto *constDefptr = new ConstDef();
    if (wordList[(*pointer)].getIdentity() == IDENFR) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    if (wordList[(*pointer)].getIdentity() == LBRACK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        getConstExp(wordList, pointer);
        if (wordList[(*pointer)].getIdentity() == RBRACK) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        }
    }
    if (wordList[(*pointer)].getIdentity() == LBRACK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        getConstExp(wordList, pointer);
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
    getConstInitVal(wordList, pointer);
    writeFile4syntax("ConstDef");
    return constDefptr;
}

ConstInitVal *getConstInitVal(std::vector<Token> wordList, int *pointer) {
    auto *constInitValptr = new ConstInitVal();
    if (wordList[(*pointer)].getIdentity() == LBRACE) {
//        '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
//        [ ConstInitVal { ',' ConstInitVal } ]
        if (wordList[(*pointer)].getIdentity() != RBRACE) {
            getConstInitVal(wordList, pointer);
            while (wordList[(*pointer)].getIdentity() == COMMA) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
                getConstInitVal(wordList, pointer);
            }
        }
        if (wordList[(*pointer)].getIdentity() == RBRACE) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        }
    } else {
//        getConstExp
        getConstExp(wordList, pointer);
    }
    writeFile4syntax("ConstInitVal");
    return constInitValptr;
}

VarDecl *getVarDecl(std::vector<Token> wordList, int *pointer) {
    auto *varDeclptr = new VarDecl();
    if (wordList[(*pointer)].getIdentity() == INTTK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    getVarDef(wordList, pointer);
    while (wordList[(*pointer)].getIdentity() == COMMA) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        getVarDef(wordList, pointer);
    }
    if (wordList[(*pointer)].getIdentity() == SEMICN) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    writeFile4syntax("VarDecl");
    return varDeclptr;
}

VarDef *getVarDef(std::vector<Token> wordList, int *pointer) {
    auto *varDefptr = new VarDef();
    if (wordList[(*pointer)].getIdentity() == IDENFR) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
    }
    if (wordList[(*pointer)].identity == LBRACK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        getConstExp(wordList, pointer);
        if (wordList[(*pointer)].getIdentity() == RBRACK) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        }
    }
    if (wordList[(*pointer)].identity == LBRACK) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        getConstExp(wordList, pointer);
        if (wordList[(*pointer)].getIdentity() == RBRACK) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        }
    }
    if (wordList[(*pointer)].getIdentity() == ASSIGN) {
//        has InitVal
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        getInitVal(wordList, pointer);
    }
    writeFile4syntax("VarDef");
    return varDefptr;
}

InitVal *getInitVal(std::vector<Token> wordList, int *pointer) {
    auto *initValptr = new InitVal();
    if (wordList[(*pointer)].getIdentity() == LBRACE) {
        writeFile4syntax(wordList[(*pointer)]);
        (*pointer)++;
        if (wordList[(*pointer)].getIdentity() == RBRACE) {
            writeFile4syntax(wordList[(*pointer)]);
            (*pointer)++;
        } else {
            getInitVal(wordList, pointer);
            while (wordList[(*pointer)].getIdentity() == COMMA) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
                getInitVal(wordList, pointer);
            }
            if (wordList[(*pointer)].getIdentity() == RBRACE) {
                writeFile4syntax(wordList[(*pointer)]);
                (*pointer)++;
            }
        }
    } else {
//        getExp();
        getExp(wordList, pointer);
    }
    writeFile4syntax("InitVal");
    return initValptr;
}

ConstExp *getConstExp(std::vector<Token> wordList, int *pointer) {
    auto *constExpptr = new ConstExp();
    getAddExp(std::move(wordList), pointer);
    writeFile4syntax("ConstExp");
    return constExpptr;
}

Exp *getExp(std::vector<Token> wordList, int *pointer, bool scanning) {
    auto *expptr = new Exp();
    getAddExp(std::move(wordList), pointer, scanning);
    writeFile4syntax("Exp", scanning);
    return expptr;
}