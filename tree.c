#include "defs.h"
#include "data.h"
#include "decl.h"

//构建AST结点
struct ASTnode *mkastnode(int op, int type,
			  struct ASTnode *left,
			  struct ASTnode *mid,
			  struct ASTnode *right, int intvalue) {
  struct ASTnode *n;

  // Malloc a new ASTnode
  n = (struct ASTnode *) malloc(sizeof(struct ASTnode));
  if (n == NULL)
    fatal("Unable to malloc in mkastnode()");

  // Copy in the field values and return it
  n->op = op;
  n->type = type;
  n->left = left;
  n->mid = mid;
  n->right = right;
  n->v.intvalue = intvalue;
  return (n);
}

//构建叶子结点
struct ASTnode *mkastleaf(int op, int type, int intvalue) {
  return (mkastnode(op, type, NULL, NULL, NULL, intvalue));
}

//构建一元结点
struct ASTnode *mkastunary(int op, int type, struct ASTnode *left,
			    int intvalue) {
  return (mkastnode(op, type, left, NULL, NULL, intvalue));
}