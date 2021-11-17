cmake_minimum_required(VERSION 3.20)
project(Compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compiler main.cpp lexer/lexer.cpp lexer/lexer.h token/Token.cpp token/Token.h syntax/syntax.cpp syntax/syntax.h AST/AST.cpp AST/AST.h jsonLike/jsonLike.cpp symbolTable/symbol.h error/errorHandler.cpp error/errorHandler.h IR/ASTParser.cpp IR/IRScope.h IR/IRScope.cpp IR/IRQuaternion.h test/test.c mips/mips.cpp mips/mips.h IR/IRmaker.cpp AST/globalValueStore.cpp)
