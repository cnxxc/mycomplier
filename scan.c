#include "defs.h"
#include "data.h"
#include "decl.h"

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
	//读取时先看有无Putback的字符
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

//将一个字符放回流，下次调用next()时，取出该字符，不是真的放回
static void putback(int c)
{
	Putback=c;
}

//跳过空白符，返回下一个非空白符
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

//扫描标识符，c是第一个字符，buf保存字符串，长度必须小于lim，返回标识符长度
static int scanident(int c,char *buf,int lim)
{
	int i=0;

	while(isalpha(c)||isdigit(c)||'_'==c)//标识符由数字、字母或_组成
	{
		if(lim-1==i)
			fatal("Identifier too long");
		else if(i<lim-1)
		{
			buf[i++]=c;
		}
		c=next();
	}
	putback(c);
	buf[i]='\0';
	return (i);
}

//判断字符串是否是关键字
static int keyword(char *s) {
  switch (*s) {
    case 'e':
      if (!strcmp(s, "else"))
		return (T_ELSE);
      break;
    case 'i':
      if (!strcmp(s, "if"))
	return (T_IF);
      if (!strcmp(s, "int"))
		return (T_INT);
      break;
    case 'p':
      if (!strcmp(s, "print"))
		return (T_PRINT);
      break;
	case 'w':
      if (!strcmp(s, "while"))
		return (T_WHILE);
      break;
  }
  return (0);
}

//扫描下一个token赋给t，扫描到文件结尾返回0
int scan(struct token *t)
{
	int c,tokentype;
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
	case ';':
		t->token=T_SEMI;
		break;
    case '{':
      t->token = T_LBRACE;
      break;
    case '}':
      t->token = T_RBRACE;
      break;
    case '(':
      t->token = T_LPAREN;
      break;
    case ')':
      t->token = T_RPAREN;
      break;
	case '=':
		if((c=next())=='='){
			t->token=T_EQ;
		} else{
			t->token=T_ASSIGN;
			putback(c);
		}
		break;
	case '!':
		if((c=next())=='='){
			t->token=T_NE;
		} else{
			fatalc("Unrecognised character", c);
		}
		break;
	case '<':
		if ((c = next()) == '=') {
			t->token = T_LE;
		} else {
			putback(c);
			t->token = T_LT;
		}
		break;
	case '>':
		if ((c = next()) == '=') {
			t->token = T_GE;
		} else {
			putback(c);
			t->token = T_GT;
		}
		break;
	default:
		if(isdigit(c))
		{
			t->intvalue=scanint(c);
			t->token=T_INTLIT;
			break;
		}
		else if(isalpha(c)||'_'==c)
		{
			scanident(c,Text,TEXTLEN);
			//判断Text是否关键字，不用==，不是关键字时为0
			if(tokentype=keyword(Text))
			{
				t->token=tokentype;
				break;
			}
			//不是关键字就是标识符
			t->token=T_IDENT;
			break;
		}
		fatalc("Unrecognised character", c);
	}
	return (1);
}
