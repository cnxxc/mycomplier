#include "defs.h"
#include "data.h"
#include "decl.h"

//返回所有if语句的跳转序号
static int label(void) {
  static int id = 1;
  return (id++);
}

// Generate the code for an IF statement
// and an optional ELSE clause
static int genIF(struct ASTnode *n) {
  int Lfalse, Lend;

  //Lfalse标签用于跳转执行else分支，Lend用于跳出if语句，在if语句汇编的最后，对于没有else分支的if语句，无需Lend，Lfalse就可以跳出if语句
  Lfalse = label();
  if (n->right)
    Lend = label();

  //生成条件语句的汇编，Lfalse是label序号，这里传入genAST()，不作为寄存器使用，在cgcompare_and_jump()中作为标签序号
  genAST(n->left, Lfalse, n->op);
  genfreeregs();

  //生成条件为true时执行的{}语句汇编，执行该分支无需跳转
  genAST(n->mid, NOREG, n->op);
  genfreeregs();

  //有else分支时跳转到相应标签
  if (n->right)
    cgjump(Lend);

  // Now the false label
  cglabel(Lfalse);

  // Optional ELSE clause: generate the
  // false compound statement and the
  // end label
  if (n->right) {
    genAST(n->right, NOREG, n->op);
    genfreeregs();
    cglabel(Lend);
  }

  return (NOREG);
}

//生成while语句汇编
static int genWHILE(struct ASTnode *n) {
  int Lstart, Lend;

  Lstart = label();
  Lend = label();
  //判断条件前就要设好标签，执行体结束后跳转
  cglabel(Lstart);

  //条件不满足时跳转Lend
  genAST(n->left, Lend, n->op);
  genfreeregs();

  // Generate the compound statement for the body
  genAST(n->right, NOREG, n->op);
  genfreeregs();

  //执行体结束后无条件跳转Lstart
  cgjump(Lstart);
  //执行体后设结束标签，条件不满足时跳转
  cglabel(Lend);
  return (NOREG);
}

//输入AST，汇编代码生成，返回返回值所在寄存器
int genAST(struct ASTnode *n, int reg, int parentASTop) {
  int leftreg, rightreg;

  //这两种情况需要用到parentASTop
  switch (n->op) {
    case A_IF:
      return (genIF(n));
    case A_WHILE:
      return (genWHILE(n));
    case A_GLUE:
      // Do each child statement, and free the
      // registers after each child
      genAST(n->left, NOREG, n->op);
      genfreeregs();
      genAST(n->right, NOREG, n->op);
      genfreeregs();
      return (NOREG);
    case A_FUNCTION:
      // Generate the function's preamble before the code
      cgfuncpreamble(Gsym[n->v.id].name);
      genAST(n->left, NOREG, n->op);
      cgfuncpostamble();
      return (NOREG);
  }

  // Get the left and right sub-tree values
  if (n->left)
    leftreg = genAST(n->left, NOREG, n->op);
  if (n->right)
    rightreg = genAST(n->right, leftreg, n->op);

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
    case A_NE:
    case A_LT:
    case A_GT:
    case A_LE:
    case A_GE:
      //碰到比较运算符时，分是否是if/while语句的条件两种情况
      if (parentASTop == A_IF || parentASTop == A_WHILE)
	      return (cgcompare_and_jump(n->op, leftreg, rightreg, reg));
      else
	      return (cgcompare_and_set(n->op, leftreg, rightreg));
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
    case A_PRINT:
      // Print the left-child's value
      // and return no register
      genprintint(leftreg);
      genfreeregs();
      return (NOREG);
    default:
      fatald("Unknown AST operator", n->op);
  }
}

void genpreamble() {
  cgpreamble();
}

//释放所有寄存器
void genfreeregs() {
  freeall_registers();
}

void genprintint(int reg) {
  cgprintint(reg);
}

void genglobsym(int id) {
  cgglobsym(id);
}