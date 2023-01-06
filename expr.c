#include "defs.h"
#include "data.h"
#include "decl.h"

//判断并构建叶子结点（整数或标识符结点）
static struct ASTnode *primary(void) {
  struct ASTnode *n;
  int id;

  switch (Token.token) {
  case T_INTLIT:
    // For an INTLIT token, make a leaf AST node for it.
    n = mkastleaf(A_INTLIT, Token.intvalue);
    break;

  case T_IDENT:
    // Check that this identifier exists
    id = findglob(Text);
    if (id == -1)
      fatals("Unknown variable", Text);

    // Make a leaf AST node for it
    n = mkastleaf(A_IDENT, id);
    break;

  default:
    fatald("Syntax error, token", Token.token);
  }

  // Scan in the next token and return the leaf node
  scan(&Token);
  return (n);
}

//token转AST node type
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

//获取运算符优先级
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

//构造算式AST，ptp为上一个token优先级，首次调用时为0
struct ASTnode *binexpr(int ptp) {
  struct ASTnode *left, *right;
  int tokentype;

  left = primary();

  tokentype = Token.token;
  if (tokentype == T_SEMI)
    return (left);

  //由于main中调用binexpr时参数为0，因此会一直迭代到文件结束
  while (op_precedence(tokentype) > ptp) {
    scan(&Token);

    right = binexpr(OpPrec[tokentype]);//递归结束之后，输入参数ptp仍然为0

    left = mkastnode(arithop(tokentype), left, right, 0);

    tokentype = Token.token;
    if (tokentype == T_SEMI)
      return (left);
  }

  return (left);
}
