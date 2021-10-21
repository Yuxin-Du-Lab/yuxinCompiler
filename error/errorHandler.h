//
// Created by yuxin on 2021/10/21.
//

#ifndef COMPILER_ERRORHANDLER_H
#define COMPILER_ERRORHANDLER_H

#include <fstream>
#define IllegalFormatString "a"
#define NameRedefinition "b"
#define UndefinedName "c"
#define FuncParamNumNotMatch "d"
#define FuncParamTypeNotMatch "e"
#define FuncShouldNotHaveReturn "f"
#define FuncReturnNeed "g"
#define ConstCannotBeREAssign "h"
#define SEMICNNeed "i"
#define RPARENTNeed "j"
#define RBRACKNeed "k"
#define PrintfParamNumNotMatch "l"
#define NotLoopUseBreakOrContinue "m"

void throwError(const std::string& errorType, int line);

#endif //COMPILER_ERRORHANDLER_H
