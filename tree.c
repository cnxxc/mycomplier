#include "defs.h"
#include "data.h"
#include "decl.h"

//构建AST结点
struct ASTnode *mkastnode(int op,struct ASTnode *left,struct ASTnode *right,int intvalue)
{
	struct ASTnode *n;
	n=(struct ASTnode *)malloc(sizeof(struct ASTnode));
	if(n==NULL)
	{
		fprintf(stderr,"Unable to malloc in mkastnode()\n");
		exit(1);
	}
	n->op=op;
	n->left=left;
	n->right=right;
	n->intvalue=intvalue;
	return (n);
}

//构建叶子结点
struct ASTnode *mkastleaf(int op,int intvalue)
{
	return (mkastnode(op,NULL,NULL,intvalue));
}

//构建一元结点
struct ASTnode *mkastunary(int op,struct ASTnode *left,int intvalue)
{
	return (mkastnode(op,left,NULL,intvalue));
}
