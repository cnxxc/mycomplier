#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TEXTLEN		512 //标识符长度限制
#define NSYMBOLS 	1024//符号表项数

//Tokens，优先级从高到低（无优先级的除外）
enum {
  T_EOF,
  T_PLUS, T_MINUS,
  T_STAR, T_SLASH,
  T_EQ, T_NE,
  T_LT, T_GT, T_LE, T_GE,
  T_INTLIT, T_SEMI, T_ASSIGN, T_IDENT,
  T_LBRACE, T_RBRACE, T_LPAREN, T_RPAREN,
  // Keywords
  T_PRINT, T_INT, T_IF, T_ELSE, T_WHILE,
  T_FOR, T_VOID, T_CHAR
};

//符号
struct token{
	int token;//取Tokens中的值
	int intvalue;//当token取T_INTLIT时有效
};

//AST node types
enum {
  A_ADD = 1, A_SUBTRACT, A_MULTIPLY, A_DIVIDE,
  A_EQ, A_NE, A_LT, A_GT, A_LE, A_GE,
  A_INTLIT,
  A_IDENT, A_LVIDENT, A_ASSIGN, A_PRINT, A_GLUE,  //A_GLUE先执行左子树再执行右子树
  A_IF, A_WHILE, A_FUNCTION
};

//初始类型，用于表示语句声明时的类型
enum {
  P_NONE, P_VOID, P_CHAR, P_INT //P_NONE表示结点不是表达式，没有类型
};

//抽象语法树结点，结点为表达式或整数。为表达式时，op为四则运算符，left、right分别为两个操作数；为整数时，op为A_INTLIT，intvalue生效
struct ASTnode{
	int op;//取值为上面的枚举类型
  int type;//取值为初始类型
	struct ASTnode *left;//左子树
  struct ASTnode *mid;//中子树，A_IF结点需要三个子结点
	struct ASTnode *right;//右子树
	//op为A_INTLIT时，既可以是字面值整数，也可以是整数标识符在全局符号表中的下标
	union {
		int intvalue;
		int id;			
	} v;
};

#define NOREG	-1  //没有返回值的语句，genAST的传入参数

//符号的类别
enum {
  S_VARIABLE, S_FUNCTION
};

//符号表，存放标识符相关数据
struct symtable{
	char *name;
  int type;   //符号的初始类型
  int stype;  //符号的类别
};
