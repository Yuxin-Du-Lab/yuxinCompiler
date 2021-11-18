#include "lexer/lexer.h"
#include "vector"
#include "syntax/syntax.h"
#include "mips/mips.h"

#define printLexerRes true
#define printSyntaxRex true
#define printASTRes false
#define printIR_MipsRes true

void writeFile4WordList(std::vector<Token> wordList) {
    std::ofstream writeLexerFile("LexerRes.txt");
    auto scan = wordList.begin();
    while (scan != wordList.end()) {
        writeLexerFile << scan->toString() << std::endl;
        scan ++;
    }
}

// main
int main() {
    std::ifstream readLexerFile("testfile.txt");
    std::vector<Token> wordList = worldsAnalyse(readLexerFile);
    if (printLexerRes) writeFile4WordList(wordList);
    auto *compUnit = syntaxAnalysis(wordList, printSyntaxRex);
    compUnit->storeGlobalValues();
    if (printASTRes) compUnit->print(0);

    if (printIR_MipsRes) {
        compUnit->makeIR();
        compUnit->_dataMake();
        makeMips();
    }
    return 0;
}
