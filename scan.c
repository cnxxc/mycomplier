#include "defs.h"
#include "data.h"
#include "decl.h"

//将一个字符放回流，下次调用next()时，取出该字符，不是真的放回
static void putback(int c)
{
	Putback=c;
}

//在字符串s中找字符c，返回c的位置
static int chrpos(char *s,int c)
{
	char *p;
	p=strchr(s,c);//返回c的指针或NULL
	return (p?p-s:-1);
}

//读取下一个字符
static int next(void)
{
	int c;
	if(Putback)
	{
		c=Putback;
		Putback=0;
		return c;
	}

	c=fgetc(Infile);//从文件中读一个字符，光标后移一字符
	if('\n'==c)
	{
		++Line;
	}
	return c;
}

//扫描整数
static int scanint(int c)
{
	int k,val=0;
	while((k=chrpos("0123456789",c))>=0)
	{
		val=val*10+k;
		c=next();
	}

	putback(c);
	return val;
}

//跳过空白符
static int skip(void)
{
	int c;
	c=next();
	while(' '==c||'\t'==c||'\n'==c||'\r'==c||'\f'==c)
	{
		c=next();
	}
	return (c);
}

//扫描下一个字符赋给t
int scan(struct token *t)
{
	int c;
	c=skip();
	switch(c)
	{
		case EOF:
			t->token=T_EOF;
			return (0);
		case '+':
			t->token=T_PLUS;
			break;
		case '-':
			t->token=T_MINUS;
			break;
		case '*':
			t->token=T_STAR;
			break;
		case '/':
			t->token=T_SLASH;
			break;
		default:
			if(isdigit(c))
			{
				t->intvalue=scanint(c);
				t->token=T_INTLIT;
				break;
			}

			printf("Unrecognized character %c on line %d\n",c,Line);
			exit(1);
	}
	return (1);
}
