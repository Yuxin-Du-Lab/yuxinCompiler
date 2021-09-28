//
// Created by yuxin on 2021/9/27.
//

#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H
#include <iostream>
#include <fstream>
#include "vector"
#include <cstring>
#include "Token.h"
// consts
const std::string SKIP = "SKIP";
const int LINELENGTH = 1024 * 1024;

const std::string IDENFR = "IDENFR";
const std::string INTCON = "INTCON";
const std::string STRCON = "STRCON";
const std::string MAINTK = "MAINTK";
const std::string CONSTTK = "CONSTTK";
const std::string VOIDTK = "VOIDTK";
const std::string INTTK = "INTTK";
const std::string BREAKTK = "BREAKTK";
const std::string CONTINUETK = "CONTINUETK";
const std::string IFTK = "IFTK";
const std::string ELSETK = "ELSETK";
const std::string WHILETK = "WHILETK";
const std::string RETURNTK = "RETURNTK";
const std::string GETINTTK = "GETINTTK";
const std::string PRINTFTK = "PRINTFTK";
const std::string LBRACE = "LBRACE";
const std::string RBRACE = "RBRACE";
const std::string SEMICN = "SEMICN";
const std::string COMMA = "COMMA";
const std::string LPARENT = "LPARENT";
const std::string RPARENT = "RPARENT";
const std::string LBRACK = "LBRACK";
const std::string RBRACK = "RBRACK";
const std::string MOD = "MOD";
const std::string PLUS = "PLUS";
const std::string MINU = "MINU";
const std::string MULT = "MULT";
const std::string NEQ = "NEQ";
const std::string NOT = "NOT";
const std::string AND = "AND";
const std::string OR = "OR";
const std::string DIV = "DIV";
const std::string LSS = "LSS";
const std::string LEQ = "LEQ";
const std::string GRE = "GRE";
const std::string GEQ = "GEQ";
const std::string EQL = "EQL";
const std::string ASSIGN = "ASSIGN";

// func claims
std::vector<Token> worldsAnalyse ();
bool isNumberChar (char c);
void checkList(std::vector<Token> wordList);
int getFormString(char bufferLine[], int pointer, std::string *value);
int getConstInteger(char bufferLine[], int pointer, std::string *value);
bool isAlphaChar (char c);
bool isUnderLine (char c);

int getMAIN(char bufferLine[], int pointer);
int getRETURN(char bufferLine[], int pointer);
int getPRINTF(char bufferLine[], int pointer);
int getINT(char bufferLine[], int pointer);
int getIDENFR(char bufferLine[], int pointer, std::string *value);
int getCONST(char bufferLine[], int pointer);
int getBREAK(char bufferLine[], int pointer);
int getCONTINUE(char bufferLine[], int pointer);
int getIF(char bufferLine[], int pointer);
int getELSE(char bufferLine[], int pointer);
int getWHILE(char bufferLine[], int pointer);
int getGETINT(char bufferLine[], int pointer);
int getVOID(char bufferLine[], int pointer);
int keyWordAnalyse(char bufferLine[], int pointer, std::string *identity, std::string *value);
#endif //COMPILER_LEXER_H
