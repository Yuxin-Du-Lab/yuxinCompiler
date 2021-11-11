cmake_minimum_required(VERSION 3.20)
project(Compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compiler main.cpp lexer/lexer.cpp lexer/lexer.h token/Token.cpp token/Token.h syntax/syntax.cpp syntax/syntax.h AST/ASTBuilder.cpp AST/ASTBuilder.h jsonLike/jsonLike.cpp symbolTable/symbol.h error/errorHandler.cpp error/errorHandler.h IR/IRMaker.cpp IR/IRScope.h IR/IRScope.cpp IR/IRQuaternion.h)
