#include "defs.h"
#include "data.h"
#include "decl.h"

static struct ASTnode *single_statement(void);

//生成print语句的汇编
static struct ASTnode *print_statement(void) {
	struct ASTnode *tree;
	int reg;

	match(T_PRINT,"print");
	
	tree=binexpr(0);//生成语法树

  tree = mkastunary(A_PRINT, tree, 0);

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

//  for语句的执行顺序
//  preop_statement;
//  while ( true_false_expression ) {
//    compound_statement;
//    postop_statement;
//  }
static struct ASTnode *for_statement(void) {
  struct ASTnode *condAST, *bodyAST;
  struct ASTnode *preopAST, *postopAST;
  struct ASTnode *tree;

  // Ensure we have 'for' '('
  match(T_FOR, "for");
  lparen();

  // Get the pre_op statement and the ';'
  preopAST = single_statement();
  semi();

  // Get the condition and the ';'
  condAST = binexpr(0);
  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  semi();

  // Get the post_op statement and the ')'
  postopAST = single_statement();
  rparen();

  // Get the compound statement which is the body
  bodyAST = compound_statement();

  // For now, all four sub-trees have to be non-NULL.
  // Later on, we'll change the semantics for when some are missing

  // Glue the compound statement and the postop tree
  tree = mkastnode(A_GLUE, bodyAST, NULL, postopAST, 0);

  // Make a WHILE loop with the condition and this new body
  tree = mkastnode(A_WHILE, condAST, NULL, tree, 0);

  // And glue the preop tree to the A_WHILE tree
  return (mkastnode(A_GLUE, preopAST, NULL, tree, 0));
}

static struct ASTnode *single_statement(void) {
  switch (Token.token) {
    case T_PRINT:
      return (print_statement());
    case T_INT:
      var_declaration();
      return (NULL);		// No AST generated here
    case T_IDENT:
      return (assignment_statement());
    case T_IF:
      return (if_statement());
    case T_WHILE:
      return (while_statement());
    case T_FOR:
      return (for_statement());
    default:
      fatald("Syntax error, token", Token.token);
  }
}

//解析一个大括号中的所有语句
struct ASTnode *compound_statement(void) {
  struct ASTnode *left = NULL;
  struct ASTnode *tree;

  //匹配左大括号
  lbrace();

  while (1) {
    tree = single_statement();

    // Some statements must be followed by a semicolon
    if (tree != NULL && (tree->op == A_PRINT || tree->op == A_ASSIGN))
      semi();

    if (tree != NULL) {
      //left是之前语句生成的树
      if (left == NULL)
	      left = tree;
      else
        //glue将多个语句结合成一棵树
	      left = mkastnode(A_GLUE, left, NULL, tree, 0);
    }

    if (Token.token == T_RBRACE) {
      rbrace();
      return (left);
    }
  }
}