#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TEXTLEN		512 //标识符长度限制
#define NSYMBOLS 	1024//符号表项数

//Tokens
enum{
	T_EOF,T_PLUS,T_MINUS,T_STAR,T_SLASH,T_INTLIT,T_SEMI,T_EQUALS,
	T_IDENT,
	//关键字
	T_PRINT,T_INT
};

//符号
struct token{
	int token;//取Tokens中的值
	int intvalue;//当token取T_INTLIT时有效
};

//AST node types
enum {
  A_ADD, A_SUBTRACT, A_MULTIPLY, A_DIVIDE, A_INTLIT,
  A_IDENT, A_LVIDENT, A_ASSIGN
};

//抽象语法树结点，结点为表达式或整数。为表达式时，op为四则运算符，left、right分别为两个操作数；为整数时，op为A_INTLIT，intvalue生效
struct ASTnode{
	int op;//取值为上面的枚举类型
	struct ASTnode *left;//左子树
	struct ASTnode *right;//右子树
	//op为A_INTLIT时，既可以是字面值整数，也可以是整数标识符在全局符号表中的下标
	union {
		int intvalue;
		int id;			
	} v;
};

//符号表，存放标识符相关数据
struct symtable{
	char *name;
};
