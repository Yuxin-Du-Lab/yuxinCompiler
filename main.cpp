#include "lexer.h"
#include "vector"

void writeFile4WordList(std::vector<Token> wordList) {
    std::ofstream writeFile("output.txt");
    auto scan = wordList.begin();
    while (scan != wordList.end()) {
        writeFile << scan->toString() << std::endl;
        scan ++;
    }
}

// main
int main() {
    std::vector<Token> wordList = worldsAnalyse();
    writeFile4WordList(wordList);
    return 0;
}

