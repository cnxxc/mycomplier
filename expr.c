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
			fprintf(stderr,"syntax error on line %d\n, token %d\n",Line,Token.token);
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

static int OpPrec[]={0,10,10,20,20,0};
//					EOF,+,-, *, /,INTLIT

static int op_precedence(int tokentype)
{
	int prec=OpPrec[tokentype];
	if(prec==0)
	{
		fprintf(stderr,"syntax error on line %d,token %d\n",Line,tokentype);
		exit(1);
	}
	return (prec);
}

//参数为上一个token优先级
struct ASTnode *binexpr(int ptp) {
  struct ASTnode *left, *right;
  int tokentype;

  left = primary();

  tokentype = Token.token;
  if (tokentype == T_SEMI)
    return (left);

  while (op_precedence(tokentype) > ptp) {//由于main中调用binexpr时参数为0，因此会一直迭代到文件结束
    scan(&Token);

    right = binexpr(OpPrec[tokentype]);

    left = mkastnode(arithop(tokentype), left, right, 0);

    tokentype = Token.token;
    if (tokentype == T_SEMI)
      return (left);
  }

  return (left);
}
