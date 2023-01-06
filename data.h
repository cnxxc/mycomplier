#ifndef extern_
 #define extern_ extern
#endif

// Global variables
// Copyright (c) 2019 Warren Toomey, GPL3

extern_ int     	    Line;//当前行号
extern_ int		        Putback;//多读的字符，调用next()时若不为0则取出并置0
extern_ FILE		    *Infile;//读取的文件
extern_ FILE		    *Outfile;//输出的汇编文件
extern_ struct token	Token;//最近读取的token
extern_	char            Text[TEXTLEN+1];//最近读取的标识符或关键字
extern_ struct          symtable Gsym[NSYMBOLS];//全局符号表