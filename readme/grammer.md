LVal '=' Exp ';'
LVal '=' 'getint''('')'';'
*Ident*


[Exp] ';'
*;* or *Ident* or *(* or *LVal* or *Number*


Block   
*{*

UnaryExp → PrimaryExp | Ident '(' [FuncRParams] ')'
PrimaryExp
'(' or LVal(Ident) or Number


'if' '( Cond ')' Stmt [ 'else' Stmt ] // 1.有else 2.无else
'while' '(' Cond ')' Stmt
'break' ';' | 'continue' ';'
'return' [Exp] ';' // 1.有Exp 2.无Exp
'printf''('FormatString{,Exp}')'';' // 1.有Exp 2.无Exp

exchange grammer:
*
LOrExp → LAndExp | LOrExp '||' LAndExp
LOrExp → LAndExp {'||' LAndExp}

*
LAndExp → EqExp | LAndExp '&&' EqExp
LAndExp → EqExp {'&&' EqExp}

*
EqExp → RelExp | EqExp ('==' | '!=') RelExp
EqExp → RelExp {('==' | '!=') RelExp}

*
RelExp → AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp
RelExp → AddExp {('<' | '>' | '<=' | '>=') AddExp}

*
AddExp → MulExp | AddExp ('+' | '−') MulExp
AddExp → MulExp {('+' | '−') MulExp}

*
MulExp → UnaryExp | MulExp ('*' | '/' | '%') UnaryExp
MulExp → UnaryExp {('*' | '/' | '%') UnaryExp}


---------
scan:
getExp
getAddExp
getMulExp
getUnaryExp
getUnaryOp
getPrimaryExp

getLVal
getNumber
getFuncRParams

------------
a op b op c op d