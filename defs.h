#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

enum{
	T_EOF,T_PLUS,T_MINUS,T_STAR,T_SLASH,T_INTLIT	
};//文件结束符、加、减、乘、除、数

struct token{
	int token;
	int intvalue;
};

enum{
	A_ADD,A_SUBTRACT,A_MULTIPLY,A_DIVIDE,A_INTLIT
};

struct ASTnode{
	int op;//取值为上面的枚举类型
	struct ASTnode *left;//左子树
	struct ASTnode *right;//右子树
	int intvalue;//整数
};//抽象语法树，单个结点为(op+left+right)(非叶子结点)或(intvalue)(叶子结点)
