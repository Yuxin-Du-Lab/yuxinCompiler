#include "lexer/lexer.h"
#include "vector"
#include "syntax/syntax.h"

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
    const bool printLexerRes = true;
    const bool printSyntaxRex = true;
    const bool printASTRes = true;
    const bool printIR_MipsRes = true;
    std::ifstream readLexerFile("testfile.txt");
    std::vector<Token> wordList = worldsAnalyse(readLexerFile);
    if (printLexerRes) writeFile4WordList(wordList);
    auto *compUnit = syntaxAnalysis(wordList, printSyntaxRex);
    compUnit->storeGlobalValues();
    if (printASTRes) compUnit->print(0);

    if (printIR_MipsRes) {
        compUnit->makeIR();
        compUnit->_dataMake();
    }
    return 0;
}
