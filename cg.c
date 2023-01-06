#include "defs.h"
#include "data.h"
#include "decl.h"

static int freereg[4];//寄存器是否可用，1表示可用，0表示不可用
static char *reglist[4]={"%r8","%r9","%r10","%r11"};//寄存器名称

//释放所有寄存器
void freeall_registers(void)
{
  freereg[0]= freereg[1]= freereg[2]= freereg[3]= 1;
}

//分配一个空闲寄存器，返回序号
static int alloc_register(void)
{
  for (int i=0; i<4; i++) {
    if (freereg[i]) {
      freereg[i]= 0;
      return(i);
    }
  }
  fatal("Out of registers");
}

//释放一个寄存器
static void free_register(int reg)
{
  if (freereg[reg] != 0) {
    fprintf(stderr, "Error trying to free register %d\n", reg);
    exit(1);
  }
  freereg[reg]= 1;
}

//main函数前的动作
void cgpreamble()
{
  freeall_registers();
  fputs(
	"\t.text\n"
	".LC0:\n"
	"\t.string\t\"%d\\n\"\n"
	"printint:\n"
	"\tpushq\t%rbp\n"
	"\tmovq\t%rsp, %rbp\n"
	"\tsubq\t$16, %rsp\n"
	"\tmovl\t%edi, -4(%rbp)\n"
	"\tmovl\t-4(%rbp), %eax\n"
	"\tmovl\t%eax, %esi\n"
	"\tleaq	.LC0(%rip), %rdi\n"
	"\tmovl	$0, %eax\n"
	"\tcall	printf@PLT\n"
	"\tnop\n"
	"\tleave\n"
	"\tret\n"
	"\n"
	"\t.globl\tmain\n"
	"\t.type\tmain, @function\n"
	"main:\n"
	"\tpushq\t%rbp\n"
	"\tmovq	%rsp, %rbp\n",
  Outfile);
}

//exit(0)的汇编代码
void cgpostamble()
{
  fputs(
	"\tmovl	$0, %eax\n"
	"\tpopq	%rbp\n"
	"\tret\n",
  Outfile);
}

//将value加载到分配的寄存器中
int cgloadint(int value)
{
	int r=alloc_register();

	fprintf(Outfile,"\tmovq\t$%d, %s\n",value,reglist[r]);
	return (r);
}

//将标识符的值传入新寄存器
int cgloadglob(char *identifier) {
  // Get a new register
  int r = alloc_register();

  // Print out the code to initialise it
  fprintf(Outfile, "\tmovq\t%s(\%%rip), %s\n", identifier, reglist[r]);
  return (r);
}

//两个寄存器中的值相加，存入r2，释放r1
int cgadd(int r1,int r2)
{
	fprintf(Outfile,"\taddq\t%s, %s\n",reglist[r1],reglist[r2]);
	free_register(r1);
	return (r2);
}

//两个寄存器中的值相乘，存入r2，释放r1
int cgmul(int r1,int r2)
{
	fprintf(Outfile,"\timulq\t%s, %s\n",reglist[r1],reglist[r2]);
	free_register(r1);
	return (r2);
}

//从r1寄存器减r2寄存器，释放r2
int cgsub(int r1,int r2)
{
	fprintf(Outfile,"\tsubq\t%s, %s\n",reglist[r2],reglist[r1]);
	free_register(r2);
	return (r1);
}

//r1寄存器除以r2寄存器，释放r2
int cgdiv(int r1,int r2)
{
	fprintf(Outfile,"\tmovq\t%s,%%rax\n",reglist[r1]);//将被除数加载到%rax寄存器
	fprintf(Outfile,"\tcqo\n");//%rax扩展到8位
	fprintf(Outfile,"\tidivq\t%s\n",reglist[r2]);
	fprintf(Outfile,"\tmovq\t%%rax,%s\n",reglist[r1]);//将r1中的商存入%rax
	free_register(r2);
	return (r1);
}

//打印一个寄存器中的值，其中printint是用汇编写的输出十进制数的函数，通过call调用
void cgprintint(int r)
{
	fprintf(Outfile,"\tmovq\t%s,%%rdi\n",reglist[r]);//Linux x86-64期望函数的第一个参数在%rdi中
	fprintf(Outfile,"\tcall\tprintint\n");
	free_register(r);
}

//将寄存器r的值传给标识符identifier
int cgstorglob(int r, char *identifier) {
  fprintf(Outfile, "\tmovq\t%s, %s(\%%rip)\n", reglist[r], identifier);
  return (r);
}

//声明全局标识符
void cgglobsym(char *sym) {
  fprintf(Outfile, "\t.comm\t%s,8,8\n", sym);
}