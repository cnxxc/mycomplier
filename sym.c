#include "defs.h"
#include "data.h"
#include "decl.h"

static int Globs=0;//全局符号表中下一个空闲位置

//在全局符号表中寻找字符串s，找到返回下标，找不到返回-1
int findglob(char *s)
{
    int i;

    for(i=0;i<Globs;++i){
        if(*s==*Gsym[i].name&&!strcmp(s,Gsym[i].name)){
            return (i);
        }
    }
    return (-1);
}

//返回下一个全局符号表空闲下标，超出限制则退出
static int newglob(void)
{
    int p;

    if((p=Globs++)>=NSYMBOLS){
        fatal("Too many global symbols!");
    }
    return (p);
}

//在全局符号表中增加一项，返回下标
int addglob(char *name, int type, int stype) {
  int y;

  // If this is already in the symbol table, return the existing slot
  if ((y = findglob(name)) != -1)
    return (y);

  // Otherwise get a new slot, fill it in and
  // return the slot number
  y = newglob();
  Gsym[y].name = strdup(name);
  Gsym[y].type = type;
  Gsym[y].stype = stype;
  return (y);
}