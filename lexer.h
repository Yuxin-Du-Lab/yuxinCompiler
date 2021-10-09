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
const int LINELENGTH = 1024 * 1024;

// func claims
//std::vector<Token> worldsAnalyse ();
std::vector<Token> worldsAnalyse (std::ifstream &fileRead);
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
