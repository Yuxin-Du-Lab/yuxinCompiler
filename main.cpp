#include "lexer.h"
#include "vector"
#include "syntax.h"

void writeFile4WordList(std::vector<Token> wordList) {
    std::ofstream writeLexerFile("outputWordList.txt");
    auto scan = wordList.begin();
    while (scan != wordList.end()) {
        writeLexerFile << scan->toString() << std::endl;
        scan ++;
    }
}

// main
int main() {
    std::ifstream readLexerFile("testfile.txt");
//    std::ofstream writeLexerFile("output.txt");
    std::vector<Token> wordList = worldsAnalyse(readLexerFile);
    writeFile4WordList(wordList);
    syntaxAnalysis(wordList);
    return 0;
}
