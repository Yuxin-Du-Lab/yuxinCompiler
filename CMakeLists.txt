cmake_minimum_required(VERSION 3.20)
project(Compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(Compiler main.cpp lexer.cpp lexer.h Token.cpp Token.h syntax.cpp syntax.h ASTBuilder.cpp ASTBuilder.h jsonLike.cpp toSysY.cpp)
