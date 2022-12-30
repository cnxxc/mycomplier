#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TEXTLEN	512

//Tokens
enum{
	T_EOF,T_PLUS,T_MINUS,T_STAR,T_SLASH,T_INTLIT,T_SEMI,T_PRINT	
};//文件结束符、加、减、乘、除、数、分号、print

//符号
struct token{
	int token;//取Tokens中的值
	int intvalue;//当token取T_INTLIT时有效
};

//AST node types
enum{
	A_ADD,A_SUBTRACT,A_MULTIPLY,A_DIVIDE,A_INTLIT
};

//抽象语法树结点，结点为表达式或整数。为表达式时，op为四则运算符，left、right分别为两个操作数；为整数时，op为A_INTLIT，intvalue生效
struct ASTnode{
	int op;//取值为上面的枚举类型
	struct ASTnode *left;//左子树
	struct ASTnode *right;//右子树
	int intvalue;//整数
};