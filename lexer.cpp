//
// Created by yuxin on 2021/9/27.
//

#include "lexer.h"
#include "Token.h"

std::vector<Token> worldsAnalyse(std::ifstream &fileRead) {
    char bufferLine[LINELENGTH];
    std::vector<Token> wordList;
//    std::ifstream fileRead("testfile.txt");
    if (fileRead.bad()) {
        return wordList;
    }
//    std::cout << "file open success!" << std::endl;
    int pointer;
    bool invalid = false;
    while (!fileRead.eof()) {
        fileRead.getline(bufferLine, LINELENGTH);
//        std::cout << ">>>get line : " << bufferLine << std::endl;
        pointer = 0;
//      analyse:
        while (bufferLine[pointer] != '\0') {
            char c = bufferLine[pointer];
//            std::cout << ">>>now check : " << c << std::endl;
            std::string identity;
            std::string value;
            if (invalid) {
                if (c == '*' && bufferLine[pointer + 1] == '/') {
                    invalid = false;
                    pointer += 2;
                    continue;
                } else {
                    pointer++;
                    continue;
                }
            }
            if (c == '/') {
                if (bufferLine[pointer + 1] == '/') {
                    break;
                }
                if (bufferLine[pointer + 1] == '*') {
                    invalid = true;
                    pointer += 2;
                    continue;
                }
            }
            if (isNumberChar(c)) {
                identity = INTCON;
                pointer = getConstInteger(bufferLine, pointer, &value);
            } else if (isAlphaChar(c) || isUnderLine(c)) {
                pointer = keyWordAnalyse(bufferLine, pointer, &identity, &value);
            } else {
//                  simple char
                switch (c) {
                    case ' ':
                    case '\r':
                    case '\t':
                    case '\n':
                        identity = SKIP;
                        break;
                    case '"':
                        identity = STRCON;
//                        pointer = getFormString(bufferLine, pointer + 1, &value);
                        value = getFormString(bufferLine, &pointer);
                        break;
                    case '{':
                        identity = LBRACE;
                        value = "{";
                        break;
                    case '}':
                        identity = RBRACE;
                        value = "}";
                        break;
                    case ';':
                        identity = SEMICN;
                        value = ";";
                        break;
                    case ',':
                        identity = COMMA;
                        value = ",";
                        break;
                    case '(':
                        identity = LPARENT;
                        value = "(";
                        break;
                    case ')':
                        identity = RPARENT;
                        value = ")";
                        break;
                    case '[':
                        identity = LBRACK;
                        value = "[";
                        break;
                    case ']':
                        identity = RBRACK;
                        value = "]";
                        break;
                    case '%':
                        identity = MOD;
                        value = "%";
                        break;
                    case '+':
                        identity = PLUS;
                        value = "+";
                        break;
                    case '-':
                        identity = MINU;
                        value = "-";
                        break;
                    case '*':
                        identity = MULT;
                        value = "*";
                        break;
                    case '!':
                        if (bufferLine[pointer + 1] == '=') {
                            identity = NEQ;
                            value = "!=";
                            pointer++;
                        } else {
                            identity = NOT;
                            value = "!";
                        }
                        break;
                    case '&':
                        if (bufferLine[pointer + 1] == '&') {
                            identity = AND;
                            value = "&&";
                            pointer++;
                        } else {
                            identity = c;
                        }
                        break;
                    case '|':
                        if (bufferLine[pointer + 1] == '|') {
                            identity = OR;
                            value = "||";
                            pointer++;
                        } else {
                            identity = c;
                        }
                        break;
                    case '/':
                        identity = DIV;
                        value = "/";
                        break;
                    case '<':
                        if (bufferLine[pointer + 1] == '=') {
                            identity = LEQ;
                            value = "<=";
                            pointer++;
                        } else {
                            identity = LSS;
                            value = "<";
                        }
                        break;
                    case '>':
                        if (bufferLine[pointer + 1] == '=') {
                            identity = GEQ;
                            value = ">=";
                            pointer++;
                        } else {
                            identity = GRE;
                            value = ">";
                        }
                        break;
                    case '=':
                        if (bufferLine[pointer + 1] == '=') {
                            identity = EQL;
                            value = "==";
                            pointer++;
                        } else {
                            identity = ASSIGN;
                            value = "=";
                        }
                        break;
                    default:
                        identity = std::string(1, c);
                }
            }
            if (identity != SKIP) {
                if (value.empty()) {
                    wordList.emplace_back(identity);
                } else {
                    wordList.emplace_back(identity, value);
                }
            }
            pointer++;
        }
    }
//    checkList(wordList);
    fileRead.close();
    return wordList;
}

void checkList(std::vector<Token> wordList) {
    auto scan = wordList.begin();
    while (scan != wordList.end()) {
        std::cout << scan->toString() << std::endl;
        scan++;
    }
}

int keyWordAnalyse(char bufferLine[], int pointer, std::string *identity, std::string *value) {
    const int keyWordBegin = pointer;
    const char charBegin = bufferLine[keyWordBegin];
    int newPointer = pointer;
    switch (charBegin) {
        case 'm':
            newPointer = getMAIN(bufferLine, pointer);
            if (newPointer != pointer) {
                *identity = MAINTK;
                *value = "main";
            }
            break;
        case 'r':
            newPointer = getRETURN(bufferLine, pointer);
            if (newPointer != pointer) {
                *identity = RETURNTK;
                *value = "return";
            }
            break;
        case 'p':
            newPointer = getPRINTF(bufferLine, pointer);
            if (newPointer != pointer) {
                *identity = PRINTFTK;
                *value = "printf";
            }
            break;
        case 'i':
            newPointer = getINT(bufferLine, pointer);
            if (newPointer != pointer) {
                *identity = INTTK;
                *value = "int";
            } else {
                newPointer = getIF(bufferLine, pointer);
                if (newPointer != pointer) {
                    *identity = IFTK;
                    *value = "if";
                }
            }
            break;
        case 'c':
            newPointer = getCONST(bufferLine, pointer);
            if (newPointer != pointer) {
                *identity = CONSTTK;
                *value = "const";
            } else {
                newPointer = getCONTINUE(bufferLine, pointer);
                if (newPointer != pointer) {
                    *identity = CONTINUETK;
                    *value = "continue";
                }
            }
            break;
        case 'b':
            newPointer = getBREAK(bufferLine, pointer);
            if (newPointer != pointer) {
                *identity = BREAKTK;
                *value = "break";
            }
            break;
        case 'e':
            newPointer = getELSE(bufferLine, pointer);
            if (newPointer != pointer) {
                *identity = ELSETK;
                *value = "else";
            }
            break;
        case 'w':
            newPointer = getWHILE(bufferLine, pointer);
            if (newPointer != pointer) {
                *identity = WHILETK;
                *value = "while";
            }
            break;
        case 'g':
            newPointer = getGETINT(bufferLine, pointer);
            if (newPointer != pointer) {
                *identity = GETINTTK;
                *value = "getint";
            }
            break;
        case 'v':
            newPointer = getVOID(bufferLine, pointer);
            if (newPointer != pointer) {
                *identity = VOIDTK;
                *value = "void";
            }
            break;
        default:
            break;
    }
    if (newPointer == pointer) {
//        not a keyword
        pointer = getIDENFR(bufferLine, pointer, value);
        *identity = IDENFR;
    } else {
        pointer = newPointer;
    }
    return pointer;
}

int getIDENFR(char bufferLine[], int pointer, std::string *value) {
    const int identBegin = pointer;
    while (isAlphaChar(bufferLine[pointer]) ||
           isNumberChar(bufferLine[pointer]) ||
           isUnderLine(bufferLine[pointer])) {
        pointer++;
    }
    std::string buf = &bufferLine[identBegin];
    *value = buf.substr(0, pointer - identBegin);
    return pointer - 1;
}

int getMAIN(char bufferLine[], int pointer) {
    if (bufferLine[pointer] == 'm' &&
        bufferLine[pointer + 1] == 'a' &&
        bufferLine[pointer + 2] == 'i' &&
        bufferLine[pointer + 3] == 'n' &&
        !isAlphaChar(bufferLine[pointer + 4]) &&
        !isNumberChar(bufferLine[pointer + 4]) &&
        !isUnderLine(bufferLine[pointer + 4])) {
        pointer += 3;
    }
    return pointer;
}

int getCONST(char bufferLine[], int pointer) {
    if (bufferLine[pointer] == 'c' &&
        bufferLine[pointer + 1] == 'o' &&
        bufferLine[pointer + 2] == 'n' &&
        bufferLine[pointer + 3] == 's' &&
        bufferLine[pointer + 4] == 't' &&
        !isAlphaChar(bufferLine[pointer + 5]) &&
        !isNumberChar(bufferLine[pointer + 5]) &&
        !isUnderLine(bufferLine[pointer + 5])) {
        pointer += 4;
    }
    return pointer;
}

int getVOID(char bufferLine[], int pointer) {
    if (bufferLine[pointer] == 'v' &&
        bufferLine[pointer + 1] == 'o' &&
        bufferLine[pointer + 2] == 'i' &&
        bufferLine[pointer + 3] == 'd' &&
        !isAlphaChar(bufferLine[pointer + 4]) &&
        !isNumberChar(bufferLine[pointer + 4]) &&
        !isUnderLine(bufferLine[pointer + 4])) {
        pointer += 3;
    }
    return pointer;
}

int getIF(char bufferLine[], int pointer) {
    if (bufferLine[pointer] == 'i' &&
        bufferLine[pointer + 1] == 'f' &&
        !isAlphaChar(bufferLine[pointer + 2]) &&
        !isNumberChar(bufferLine[pointer + 2]) &&
        !isUnderLine(bufferLine[pointer + 2])) {
        pointer += 1;
    }
    return pointer;
}

int getELSE(char bufferLine[], int pointer) {
    if (bufferLine[pointer] == 'e' &&
        bufferLine[pointer + 1] == 'l' &&
        bufferLine[pointer + 2] == 's' &&
        bufferLine[pointer + 3] == 'e' &&
        !isAlphaChar(bufferLine[pointer + 4]) &&
        !isNumberChar(bufferLine[pointer + 4]) &&
        !isUnderLine(bufferLine[pointer + 4])) {
        pointer += 3;
    }
    return pointer;
}

int getCONTINUE(char bufferLine[], int pointer) {
    if (bufferLine[pointer] == 'c' &&
        bufferLine[pointer + 1] == 'o' &&
        bufferLine[pointer + 2] == 'n' &&
        bufferLine[pointer + 3] == 't' &&
        bufferLine[pointer + 4] == 'i' &&
        bufferLine[pointer + 5] == 'n' &&
        bufferLine[pointer + 6] == 'u' &&
        bufferLine[pointer + 7] == 'e' &&
        !isAlphaChar(bufferLine[pointer + 8]) &&
        !isNumberChar(bufferLine[pointer + 8]) &&
        !isUnderLine(bufferLine[pointer + 8])) {
        pointer += 7;
    }
    return pointer;
}

int getWHILE(char bufferLine[], int pointer) {
    if (bufferLine[pointer] == 'w' &&
        bufferLine[pointer + 1] == 'h' &&
        bufferLine[pointer + 2] == 'i' &&
        bufferLine[pointer + 3] == 'l' &&
        bufferLine[pointer + 4] == 'e' &&
        !isAlphaChar(bufferLine[pointer + 5]) &&
        !isNumberChar(bufferLine[pointer + 5]) &&
        !isUnderLine(bufferLine[pointer + 5])) {
        pointer += 4;
    }
    return pointer;
}

int getBREAK(char bufferLine[], int pointer) {
    if (bufferLine[pointer] == 'b' &&
        bufferLine[pointer + 1] == 'r' &&
        bufferLine[pointer + 2] == 'e' &&
        bufferLine[pointer + 3] == 'a' &&
        bufferLine[pointer + 4] == 'k' &&
        !isAlphaChar(bufferLine[pointer + 5]) &&
        !isNumberChar(bufferLine[pointer + 5]) &&
        !isUnderLine(bufferLine[pointer + 5])) {
        pointer += 4;
    }
    return pointer;
}

int getRETURN(char bufferLine[], int pointer) {
    if (bufferLine[pointer] == 'r' &&
        bufferLine[pointer + 1] == 'e' &&
        bufferLine[pointer + 2] == 't' &&
        bufferLine[pointer + 3] == 'u' &&
        bufferLine[pointer + 4] == 'r' &&
        bufferLine[pointer + 5] == 'n' &&
        !isAlphaChar(bufferLine[pointer + 6]) &&
        !isNumberChar(bufferLine[pointer + 6]) &&
        !isUnderLine(bufferLine[pointer + 6])) {
        pointer += 5;
    }
    return pointer;
}

int getPRINTF(char bufferLine[], int pointer) {
    if (bufferLine[pointer] == 'p' &&
        bufferLine[pointer + 1] == 'r' &&
        bufferLine[pointer + 2] == 'i' &&
        bufferLine[pointer + 3] == 'n' &&
        bufferLine[pointer + 4] == 't' &&
        bufferLine[pointer + 5] == 'f' &&
        !isAlphaChar(bufferLine[pointer + 6]) &&
        !isNumberChar(bufferLine[pointer + 6]) &&
        !isUnderLine(bufferLine[pointer + 6])) {
        pointer += 5;
    }
    return pointer;
}

int getGETINT(char bufferLine[], int pointer) {
    if (bufferLine[pointer] == 'g' &&
        bufferLine[pointer + 1] == 'e' &&
        bufferLine[pointer + 2] == 't' &&
        bufferLine[pointer + 3] == 'i' &&
        bufferLine[pointer + 4] == 'n' &&
        bufferLine[pointer + 5] == 't' &&
        !isAlphaChar(bufferLine[pointer + 6]) &&
        !isNumberChar(bufferLine[pointer + 6]) &&
        !isUnderLine(bufferLine[pointer + 6])) {
        pointer += 5;
    }
    return pointer;
}

int getINT(char bufferLine[], int pointer) {
    if (bufferLine[pointer] == 'i' &&
        bufferLine[pointer + 1] == 'n' &&
        bufferLine[pointer + 2] == 't' &&
        !isAlphaChar(bufferLine[pointer + 3]) &&
        !isNumberChar(bufferLine[pointer + 3]) &&
        !isUnderLine(bufferLine[pointer + 3])) {
        pointer += 2;
    }
    return pointer;
}

//int getFormString(const char bufferLine[], int pointer, std::string *value) {
std::string getFormString(const char bufferLine[], int *pointer) {
//    const int formStringBegin = *pointer - 1;
//    std::string formatString;
    char buf[1024] = "";
    int index = 0;
    buf[index++] = bufferLine[*pointer];
    (*pointer)++;
    while (bufferLine[*pointer] != '"') {
        buf[index++] = bufferLine[*pointer];
        (*pointer)++;
    }
    buf[index++] = bufferLine[*pointer];
    buf[index] = '\0';
    (*pointer)++;
    std::string str(buf);
//    formatString = &bufferLine[formStringBegin];
//    formatString = formatString.substr(0, pointer - formStringBegin + 1);
//    *value = formatString;
//    *value = buf;
    return str;
}

int getConstInteger(char bufferLine[], int pointer, std::string *value) {
    const int constIntegerBegin = pointer;
    std::string constInteger;
    while (isNumberChar(bufferLine[pointer])) {
        pointer++;
    }
    constInteger = &bufferLine[constIntegerBegin];
    constInteger = constInteger.substr(0, pointer - constIntegerBegin);
    *value = constInteger;
    return pointer - 1;
}

bool isNumberChar(char c) {
    if (c >= '0' && c <= '9') {
        return true;
    }
    return false;
}

bool isAlphaChar(char c) {
    if (c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z') {
        return true;
    }
    return false;
}

bool isUnderLine(char c) {
    return c == '_';
}