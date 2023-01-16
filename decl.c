#include "defs.h"
#include "data.h"
#include "decl.h"

//解析变量的声明
void var_declaration(void) {
  match(T_INT, "int");
  ident();
  addglob(Text);
  genglobsym(Text);
  semi();
}

//函数声明
struct ASTnode *function_declaration(void) {
  struct ASTnode *tree;
  int nameslot;

  // Find the 'void', the identifier, and the '(' ')'.
  // For now, do nothing with them
  match(T_VOID, "void");
  ident();
  nameslot= addglob(Text);
  lparen();
  rparen();

  // Get the AST tree for the compound statement
  tree= compound_statement();

  //一元结点，左子树是包含大括号的函数体
  return(mkastunary(A_FUNCTION, tree, nameslot));
}