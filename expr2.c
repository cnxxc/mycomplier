#include "defs.h"
#include "data.h"
#include "decl.h"

//判断并构建叶子结点（整数结点）
static struct ASTnode *primary(void)
{
	struct ASTnode *n;

	switch(Token.token)
	{
		case T_INTLIT:
			n=mkastleaf(A_INTLIT,Token.intvalue);
			scan(&Token);
			return (n);
		default:
			fprintf(stderr,"syntax error on line %d\n",Line);
			exit(1);
	}
}

//token转四则运算符
int arithop(int tok)
{
	switch(tok)
	{
		case T_PLUS:
			return (A_ADD);
		case T_MINUS:
			return (A_SUBTRACT);
		case T_STAR:
			return (A_MULTIPLY);
		case T_SLASH:
			return (A_DIVIDE);
		default:
			fprintf(stderr,"unknown token in arithop() on line %d\n",Line);
			exit(1);
	}
}

//构造以*和/为根的AST
struct ASTnode *multiplicative_expr(void)
{
	struct ASTnode *left,*right;
	int tokentype;

	left=primary();

	tokentype=Token.token;
	if(tokentype==T_EOF)
		return (left);
		
	while((tokentype==T_STAR)||(tokentype==T_SLASH))
	{
		scan(&Token);
		right=primary();

		left=mkastnode(arithop(tokentype),left,right,0);//把当前left树作为左子树，运算符右边的第一个数作为右子树，再把整棵树赋给
														//left作为下一轮迭代的左子树，直到运算符不为*或/

		tokentype=Token.token;
		if(tokentype==T_EOF)
			break;
	}

	return left;
}

//构造以+或-为根的AST
struct ASTnode *additive_expr(void)
{
	struct ASTnode *left,*right;
	int tokentype;

	left=multiplicative_expr();

	tokentype=Token.token;
	if(tokentype==T_EOF)
		return (left);

	while(1)
	{
		scan(&Token);//这个token是作为根结点的+或-

		right=multiplicative_expr();

		left=mkastnode(arithop(tokentype),left,right,0);

		tokentype=Token.token;
		if(tokentype==T_EOF)
			break;
	}

	return (left);
}

struct ASTnode *binexpr(int n)
{
	return (additive_expr());
}
