#include "defs.h"
#include "data.h"
#include "decl.h"

//汇编代码生成
int genAST(struct ASTnode *n, int reg) {
  int leftreg, rightreg;

  // Get the left and right sub-tree values
  if (n->left)
    leftreg = genAST(n->left, -1);
  if (n->right)
    rightreg = genAST(n->right, leftreg);

  switch (n->op) {
  case A_ADD:
    return (cgadd(leftreg, rightreg));
  case A_SUBTRACT:
    return (cgsub(leftreg, rightreg));
  case A_MULTIPLY:
    return (cgmul(leftreg, rightreg));
  case A_DIVIDE:
    return (cgdiv(leftreg, rightreg));
  case A_EQ:
    return (cgequal(leftreg, rightreg));
  case A_NE:
    return (cgnotequal(leftreg, rightreg));
  case A_LT:
    return (cglessthan(leftreg, rightreg));
  case A_GT:
    return (cggreaterthan(leftreg, rightreg));
  case A_LE:
    return (cglessequal(leftreg, rightreg));
  case A_GE:
    return (cggreaterequal(leftreg, rightreg));
  case A_INTLIT:
    return (cgloadint(n->v.intvalue));
  case A_IDENT:
    return (cgloadglob(Gsym[n->v.id].name));
  case A_LVIDENT:
	//赋值时，将左子树的常量所在reg的值赋给标识符
    return (cgstorglob(reg, Gsym[n->v.id].name));
  case A_ASSIGN:
    //当根结点是赋值运算符时，leftreg存放常量，在生成右子树时，在case A_LVIDENT中已完成赋值，值存在rightreg中
    return (rightreg);
  default:
    fatald("Unknown AST operator", n->op);
  }
}

void genpreamble() {
  cgpreamble();
}

void genpostamble() {
  cgpostamble();
}

//释放所有寄存器
void genfreeregs() {
  freeall_registers();
}

void genprintint(int reg) {
  cgprintint(reg);
}

void genglobsym(char *s) {
  cgglobsym(s);
}