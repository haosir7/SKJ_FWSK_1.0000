/*! \file pos_debug.c
   \brief 本文件实现一些通用调试接口,包括函数入口、出口调试接口
		  普通调试信息打印接口
   \author zyk
 */

#include "LOGCTRL.h"
#include "pos_log.h"


#ifndef NO_POS_DEBUG
#include<string.h>
#include<stdarg.h>

int _g_dbg_on_=1;
int _g_dbg_enter_exit_on_=1;
int _g_assert_on_=1;

static char _dbg_buf1_[512];
static char _dbg_buf2_[1024];
static char _dbg_buf_[1024];

void _dbg_enter_ (const char *_func_name_,
				  const char *_file_name_,
				  unsigned int _line_no_,
				  const char **_sfunc_name_)
{

  *_sfunc_name_=_func_name_;
  if(_g_dbg_enter_exit_on_ != 0)
  {

	 memset((void*) _dbg_buf_,0,sizeof(_dbg_buf_));
	 sprintf(_dbg_buf_,"%s %4d  %s() -----------begin------------- \n\r",GetFileName(_file_name_),_line_no_,*_sfunc_name_);
	 prn_logx(_dbg_buf_);
  }
}

void _dbg_return_ (const char *_file_name_,
				   unsigned int _line_no_,
				    char ** _sfunc_name_)
{
  if(_g_dbg_enter_exit_on_ != 0)
  {
	 memset((void*) _dbg_buf_,0,sizeof(_dbg_buf_));
	 sprintf(_dbg_buf_,"%s %4d  %s() -----------end--------------- \n\r",GetFileName(_file_name_),_line_no_,*_sfunc_name_);
	 prn_logx(_dbg_buf_);
  }  
}


void _dbg_pargs_ (const char * _keyword_,
				  const char * _file_name_,
				  unsigned int _line_no_)
{

	 memset((void*) _dbg_buf1_,0,sizeof(_dbg_buf1_));
	 sprintf(_dbg_buf1_,"%s %4d %s: ",GetFileName(_file_name_),_line_no_, _keyword_);
	
}

void _dbg_prnt_ (const char *fmt,...)
{

	va_list args;

	memset((void*) _dbg_buf2_,0,sizeof(_dbg_buf2_));
	memset((void*) _dbg_buf_,0,sizeof(_dbg_buf_));

	va_start(args, fmt);   
	vsprintf(_dbg_buf2_, fmt, args);
	va_end(args);	 

	sprintf(_dbg_buf_,"%s %s \n\r",_dbg_buf1_,_dbg_buf2_);

	prn_logx(_dbg_buf_);
}

#endif
