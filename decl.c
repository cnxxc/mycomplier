#include "defs.h"
#include "data.h"
#include "decl.h"

//解析token，转化为类型
int parse_type(int t) {
  if (t == T_CHAR)
    return (P_CHAR);
  if (t == T_INT)
    return (P_INT);
  if (t == T_VOID)
    return (P_VOID);
  fatald("Illegal type, token", t);
}

//解析变量的声明
void var_declaration(void) {
  int id, type;

  // Get the type of the variable, then the identifier
  type = parse_type(Token.token);
  scan(&Token);
  ident();
  // Text now has the identifier's name.
  // Add it as a known identifier
  // and generate its space in assembly
  id = addglob(Text, type, S_VARIABLE);
  genglobsym(id);
  // Get the trailing semicolon
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