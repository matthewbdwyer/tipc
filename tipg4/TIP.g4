grammar TIP;
// Grammar for Moeller and Schwartzbach's Tiny Imperative Language (TIP)

// These rules are mapped to their names in Scala code and in the SPA document

////////////////////// TIP Programs ////////////////////////// 

// "Program" in TIP
program : (function)*
;

// "TipFunction" and "FunBlock" in TIP 
function : IDENTIFIER 
           '(' (IDENTIFIER (',' IDENTIFIER)*)? ')' 
           '{' (declaration*) (statement*) returnStmt '}' 
;

////////////////////// TIP Expressions ////////////////////////// 

// Expressions in TIP use a stratified grammar to capture precedence.
// ANTLR4 allows for operator precedence parsing where precendence 
// comes from ordering of rules.
//
// This rule implements "Expression", "Operation" and "Term" in TIP.
// Note that we incorporate the negated numeric literal here in order
// to enforce precedence relative to the other operators.
//
// NB: # creates rule label that can be accessed in visitor
//
expr : SUB NUMBER			#negNumber
     | expr op=(MUL | DIV) expr 	#multiplicativeExpr
     | expr op=(ADD | SUB) expr 	#additiveExpr
     | expr op=GT expr 			#relationalExpr
     | expr op=EQ expr 			#equalityExpr
     | atom				#atomExpr
;

// Must have labels for all rules (or none) 
// The "*Atom" are, in effect, placeholders for the corresponding "*Expr"
// Note that "atomExpr" plays this role for atom in the above rule.
atom : parenExpr	#parenAtom
     | funAppExpr	#funAppAtom
     | NUMBER		#numExpr
     | IDENTIFIER	#idExpr
     | KINPUT		#inputExpr
     | KALLOC expr	#allocExpr
     | '&' IDENTIFIER	#refExpr
     | deRefExpr	#deRefAtom
     | KNULL		#nullExpr
     | recordExpr	#recordAtom
     | accessExpr	#accessAtom
;

parenExpr : '(' expr ')' ;

deRefExpr : '*' atom ;

// "FunApp" and "FunActuals" in TIP
funAppExpr : (parenExpr | IDENTIFIER) '(' (expr (',' expr)*)? ')' ;

// "Record" in TIP
recordExpr : '{' (fieldExpr (',' fieldExpr)*)? '}' ;

// "Field" in TIP
fieldExpr : IDENTIFIER ':' expr ;

// "Access" in TIP
accessExpr : (IDENTIFIER | deRefExpr | parenExpr) '.' IDENTIFIER ;

// "AssignableExpression" in TIP
assignableExpr : (IDENTIFIER | deRefExpr) ;


////////////////////// TIP Statements ////////////////////////// 

// "Declaration" in TIP
declaration : KVAR IDENTIFIER (',' IDENTIFIER)* ';' ;

// "Statement" in TIP
statement : blockStmt
    | assignmentStmt
    | whileStmt
    | ifStmt
    | outputStmt
    | errorStmt
;

// "Assignment" in TIP
assignmentStmt : assignableExpr '=' expr ';' ;

// "Block" in TIP
blockStmt : '{' (statement*) '}' ;

// "While" in TIP
whileStmt : KWHILE '(' expr ')' statement ;

// "If" in TIP
ifStmt : KIF '(' expr ')' statement (KELSE statement)? ;

// "Output" in TIP
outputStmt : KOUTPUT expr ';'  ;

// "Error" in TIP
errorStmt : KERROR expr ';'  ;

// "Return" in TIP
returnStmt : KRETURN expr ';'  ;


////////////////////// TIP Lexicon ////////////////////////// 

// By convention ANTLR4 lexical elements use all caps

MUL : '*' ;
DIV : '/' ;
ADD : '+' ;
SUB : '-' ;
GT  : '>' ;
EQ  : '==' ;
NE  : '!=' ;

// "Number" (in Scala also "Digits") is "I" (in SPA)
NUMBER : [0-9]+ ;

// Placing the keyword definitions first causes ANTLR4 to prioritize
// their matching relative to IDENTIFIER (which comes later).
KALLOC  : 'alloc' ;
KINPUT  : 'input' ;
KWHILE  : 'while' ;
KIF     : 'if' ;
KELSE   : 'else' ;
KVAR    : 'var' ;
KRETURN : 'return' ;
KNULL   : 'null' ;
KOUTPUT : 'output' ;
KERROR  : 'error' ;

// "Identifier" (in Scala) is "X" (in SPA)
IDENTIFIER : [a-zA-Z_][a-zA-Z0-9_]* ;

// ANTLR4 has a nice mechanism for specifying the characters that should
// skipped during parsing.  You write "-> skip" after the pattern and
// let ANTLR4s pattern matching do the rest.
// 
// In the TIP parboiled2 grammar you need to build up this capability
// with a set of rules.

// This handles the 
WS : [ \t\n\r]+ -> skip ;

// This does not handle nested block comments.
BLOCKCOMMENT: '/*' .* '*/' -> skip ;

COMMENT : '//' ~[\n\r]* -> skip ;
