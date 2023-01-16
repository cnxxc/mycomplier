#include "defs.h"
#include "data.h"
#include "decl.h"

//判断并构建叶子结点（整数或标识符结点）
static struct ASTnode *primary(void) {
  struct ASTnode *n;
  int id;

  switch (Token.token) {
    case T_INTLIT:
      //在char范围内的整数当作char类型
      if ((Token.intvalue) >= 0 && (Token.intvalue < 256))
        n = mkastleaf(A_INTLIT, P_CHAR, Token.intvalue);
      else
        n = mkastleaf(A_INTLIT, P_INT, Token.intvalue);
      break;

    case T_IDENT:
      // Check that this identifier exists
      id = findglob(Text);
      if (id == -1)
        fatals("Unknown variable", Text);

      // Make a leaf AST node for it
      n = mkastleaf(A_IDENT, Gsym[id].type, id);
      break;

    default:
      fatald("Syntax error, token", Token.token);
  }

  // Scan in the next token and return the leaf node
  scan(&Token);
  return (n);
}

//token转AST node type，token和ast node type一一对应
static int arithop(int tokentype) {
  if (tokentype > T_EOF && tokentype < T_INTLIT)
    return(tokentype);
  fatald("Syntax error, token", tokentype);
}

static int OpPrec[] = {
  0, 10, 10,			// T_EOF, T_PLUS, T_MINUS
  20, 20,			// T_STAR, T_SLASH
  30, 30,			// T_EQ, T_NE
  40, 40, 40, 40		// T_LT, T_GT, T_LE, T_GE
};

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
  if (tokentype == T_SEMI || tokentype == T_RPAREN)
    return (left);

  //由于main中调用binexpr时参数为0，因此会一直迭代到文件结束
  while (op_precedence(tokentype) > ptp) {
    scan(&Token);

    right = binexpr(OpPrec[tokentype]);//递归结束之后，输入参数ptp仍然为0

    left = mkastnode(arithop(tokentype), left, NULL, right, 0);

    tokentype = Token.token;
    if (tokentype == T_SEMI || tokentype == T_RPAREN)
      return (left);
  }

  return (left);
}
