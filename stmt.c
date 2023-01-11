#include "defs.h"
#include "data.h"
#include "decl.h"

//生成print语句的汇编
static struct ASTnode *print_statement(void){
	struct ASTnode *tree;
	int reg;

	match(T_PRINT,"print");
	
	tree=binexpr(0);//生成语法树

  tree = mkastunary(A_PRINT, tree, 0);

	semi();
  return (tree);
}

//赋值语句
static struct ASTnode *assignment_statement(void) {
	struct ASTnode *left, *right, *tree;
	int id;

	ident();

	if ((id = findglob(Text)) == -1) {
		fatals("Undeclared variable", Text);
	}
	//赋值的AST node中，右子树是标识符在全局符号表中的下标，左子树为赋的值
	right = mkastleaf(A_LVIDENT, id);

	match(T_ASSIGN, "=");

	//赋值可以是表达式
	left = binexpr(0);

	tree = mkastnode(A_ASSIGN, left, NULL, right, 0);

	semi();
  return (tree);
}

//解析if语句
struct ASTnode *if_statement(void) {
  struct ASTnode *condAST, *trueAST, *falseAST = NULL;

  // Ensure we have 'if' '('
  match(T_IF, "if");
  lparen();

  // Parse the following expression
  // and the ')' following. Ensure
  // the tree's operation is a comparison.
  condAST = binexpr(0);

  //目前要求if的条件必须是比较表达式
  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  rparen();

  // Get the AST for the compound statement
  trueAST = compound_statement();

  // If we have an 'else', skip it
  // and get the AST for the compound statement
  if (Token.token == T_ELSE) {
    scan(&Token);
    falseAST = compound_statement();
  }
  // Build and return the AST for this statement
  return (mkastnode(A_IF, condAST, trueAST, falseAST, 0));
}

//解析while语句
struct ASTnode *while_statement(void) {
  struct ASTnode *condAST, *bodyAST;

  // Ensure we have 'while' '('
  match(T_WHILE, "while");
  lparen();

  // Parse the following expression
  // and the ')' following. Ensure
  // the tree's operation is a comparison.
  condAST = binexpr(0);
  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  rparen();

  // Get the AST for the compound statement
  bodyAST = compound_statement();

  // Build and return the AST for this statement
  return (mkastnode(A_WHILE, condAST, NULL, bodyAST, 0));
}

//解析一个大括号中的所有语句
struct ASTnode *compound_statement(void) {
  struct ASTnode *left = NULL;
  struct ASTnode *tree;

  //匹配左大括号
  lbrace();

  while (1) {
    switch (Token.token) {
      case T_PRINT:
	      tree = print_statement();
	      break;
      case T_INT:
	      var_declaration();
	      tree = NULL;		// No AST generated here
	      break;
      case T_IDENT:
	      tree = assignment_statement();
	      break;
      case T_IF:
	      tree = if_statement();
	      break;
      case T_WHILE:
	      tree = while_statement();
	      break;
      case T_RBRACE:
	      // When we hit a right curly bracket,
	      // skip past it and return the AST
	      rbrace();
	      return (left);
      default:
	      fatald("Syntax error, token", Token.token);
    }

    if (tree) {
      //left是之前语句生成的树
      if (left == NULL)
	      left = tree;
      else
        //glue将多个语句结合成一棵树
	      left = mkastnode(A_GLUE, left, NULL, tree, 0);
    }
  }
}