#include "defs.h"
#include "data.h"
#include "decl.h"

//将代码转化为汇编
void print_statements(void){
	struct ASTnode *tree;
	int reg;

	match(T_PRINT,"print");
	
	tree=binexpr(0);//生成语法树
	reg=genAST(tree,-1);//将语法树运算过程转为汇编
	genprintint(reg);//打印一个语句的结果
	genfreeregs();//释放所有寄存器

	semi();
}

//赋值语句
void assignment_statement(void) {
	struct ASTnode *left, *right, *tree;
	int id;

	ident();

	if ((id = findglob(Text)) == -1) {
		fatals("Undeclared variable", Text);
	}
	//赋值的AST node中，右子树是标识符在全局符号表中的下标，左子树为赋的值
	right = mkastleaf(A_LVIDENT, id);

	match(T_EQUALS, "=");

	//赋值可以是表达式
	left = binexpr(0);

	tree = mkastnode(A_ASSIGN, left, right, 0);

	genAST(tree, -1);
	genfreeregs();

	semi();
}

//解析所有语句
void statements(void) {

  while (1) {
    switch (Token.token) {
    case T_PRINT:
      print_statements();
      break;
    case T_INT:
      var_declaration();
      break;
    case T_IDENT:
      assignment_statement();
      break;
    case T_EOF:
      return;
    default:
      fatald("Syntax error, token", Token.token);
    }
  }
}