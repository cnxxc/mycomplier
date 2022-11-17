#include "defs.h"
#include "data.h"
#include "decl.h"

void statements(void){
	struct ASTnode *tree;
	int reg;

	while(1){
		match(T_PRINT,"print");
		
		tree=binexpr(0);//生成语法树
		reg=genAST(tree);//生成汇编
		genprintint(reg);//打印一个语句的结果
		genfreeregs();

		semi();
		if(Token.token==T_EOF)
			return;
	}
}
