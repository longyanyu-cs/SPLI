﻿#include "spli.h"
#include "prop.h"

int main(int argc, char *argv[])
{
	uint64_t i = 0;
	if(argc == 2)
	{
		//初始化命题表
		P = create_prop_table(PROP_MAX_SIZE);
		//将参数字符串复制到命题逻辑表达式字符串中
		strcpy(expr, argv[1]);

		//定义输入缓冲区
		YY_BUFFER_STATE bp;

		//第一次parsing，添加命题到命题表（初始化命题遍历）
		has_added = 0;
		//从字符串读取输入
		bp = yy_scan_string(expr);
		//设置词法分析器读取的缓冲区
		yy_switch_to_buffer(bp);
		//清除命题状态标记
		clean_prop_status();
		//开始调用parser
		yyparse();
		//放弃缓冲区的内容
		yy_flush_buffer(bp);
		//第一次parsing结束，已经添加命题到命题表
		has_added = 1;

		//打印真值表的标题，结果列头部的标题文本为命令行参数
		print_table_title(expr);

		//后续的parsing，遍历命题的真值（每次parsing时，parser会计算并保存出表达式运算结果）
		while(1)
		{
			bp = yy_scan_string(expr);
			//设置词法分析器读取的缓冲区
			yy_switch_to_buffer(bp);
			//清除命题状态标记
			clean_prop_status();
			//重新调用parser
			yyparse();
			//放弃缓冲区的内容
			yy_flush_buffer(bp);
			//递减以改变命题表中元素的值
			
			if(step_prop_val())
			{
				break;
			}
		}

		//释放缓冲区
		yy_delete_buffer(bp);
		//释放内存空间
		free(P);

		return 0;
	}
	else
	{
		//无命令行参数时报错
		fprintf(stderr, "Error: Parameter is Wrong!\n");
		return 1;
	}	
	
}
