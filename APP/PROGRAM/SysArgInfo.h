#ifndef _SYS_ARG_INFO_H
#define _SYS_ARG_INFO_H

#define MAX_SYS_ARG_NUM	30

struct _Sys_Arg_info
{
	int  id;					//
	char name[40];				//
	char item[4][20];			//
	bool can_modify;			//
	char default_value[10];		//
};



#endif
