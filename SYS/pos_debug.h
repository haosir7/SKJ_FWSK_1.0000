/*! \file pos_debug.h
   \brief 本文件主要定义了一些通用调试接口,包括函数入口、出口调试接口
		  普通调试信息打印接口，断言接口
   \author zyk
 */


#ifdef __cplusplus 
extern "C" {  
#endif
#include <stdlib.h>

#ifdef	DBG_PRN
#undef	DBG_PRN
#endif
#ifdef	DBG_ASSERT_EXIT
#undef	DBG_ASSERT_EXIT
#endif
#ifdef	DBG_ASSERT_WARNING
#undef	DBG_ASSERT_WARNING
#endif
#ifdef	DBG_ENTER
#undef	DBG_ENTER
#endif
#ifdef	DBG_LEAVE
#undef	DBG_LEAVE
#endif
#ifdef	DBG_RETURN
#undef	DBG_RETURN
#endif
#ifdef	DBG_VOID_RETURN
#undef	DBG_VOID_RETURN
#endif
	

#ifdef NO_POS_DEBUG
#undef POS_ENTER_ON
#undef POS_DBGPRN_ON
#undef POS_ASSERT_ON
#endif

#if !defined(POS_ENTER_ON) && !defined(POS_DBGPRN_ON) && !defined(POS_ASSERT_ON)
#define NO_POS_DEBUG
#endif
	
#ifndef NO_POS_DEBUG
	/*! 调试信息打印接口开关，0－关闭，1－打开 */
	extern int _g_dbg_on_;
	/*! 函数入口、出口调试开关，0－关闭，1－打开*/
	extern int _g_dbg_enter_exit_on_;
	/*! 断言调试开关，0－关闭，1－打开*/
	extern int _g_assert_on_;

	/*! \brief 记录函数名，并打印函数入口信息
		\param[in] _func_name_ 函数名称
		\param[in] _file_name_ 文件名
		\param[in] _line_no_ 行号
		\param[out] _sfunc_name_ 输出要记录的函数名
	*/
	extern void _dbg_enter_ (const char *_func_name_,
				  const char *_file_name_,
				  unsigned int _line_no_,
				  const char **_sfunc_name_);
    /*! \brief 打印函数出口信息
		\param[in] _file_name_ 文件名
		\param[in] _line_no_ 行号
		\param[int] _sfunc_name_ 函数名
    */
	extern void _dbg_return_ (const char *_file_name_,
				   unsigned int _line_no_,
				   const char ** _sfunc_name_);

	/*! \brief 记录文件名、行号、关键字信息
		\param[in] _keyword_ 关键字
		\param[in] _file_name_ 文件名
		\param[in] _line_no_ 行号
	*/
	extern void _dbg_pargs_ (const char * _keyword_,
				  const char * _file_name_,
				  unsigned int _line_no_);
	/*! \brief 打印调试信息
		\param[in] fmt 格式化输出的格式串
		\param[in] ... 可变参数表
	*/
	extern void _dbg_prnt_ (const char *fmt,...);
#endif

//-----------------------------------------------------------------
#ifndef POS_ENTER_ON
#	define DBG_ENTER(f1)
#	define DBG_RETURN(a1) return(a1)
#	define DBG_VOID_RETURN return
#else
	/*! \def DBG_ENTER(f1)
		\brief 记录和打印函数入口调试信息
		\param f1 函数名
		\attention 由于DBG_ENTER 在函数的栈空间上分配空间，
		所以它必须在所有可执行语句之前执行
	*/
#	define DBG_ENTER(f1) \
	const char *_dbg_func_name_=NULL; \
	_dbg_enter_ (f1,__FILE__,__LINE__,&_dbg_func_name_) 
	/*! \def DBG_LEAVE
		\brief 打印函数出口信息
	*/
#	define DBG_LEAVE \
	(_dbg_return_(__FILE__,__LINE__,&_dbg_func_name_))
   /*! \def DBG_RETURN(a1)
		\brief 打印有返回值的函数的出口信息
	    \param [in] 函数的返回值   
	*/
#	define DBG_RETURN(a1) return(DBG_LEAVE, a1)
   /*! \def DBG_VOID_RETURN 
		\brief 打印有返回值的函数的出口信息
	*/
#	define DBG_VOID_RETURN {DBG_LEAVE; return;}
#endif

//------------------------------------------------------------------//
#ifndef POS_DBGPRN_ON
#	define DBG_PRN(keyword,arglist)
#else
    /*! \def DBG_RETURN(a1)
		\brief 打印有返回值的函数的出口信息
	*/
#	define DBG_PRN(keyword,arglist) \
	{ if(_g_dbg_on_ !=0 ) { _dbg_pargs_(keyword,__FILE__,__LINE__); _dbg_prnt_ arglist ; }}

#endif

//---------------------------------------------------------------------//
#ifndef  POS_ASSERT_ON
#	define DBG_ASSERT_EXIT(condition, arglist)
#	define DBG_ASSERT_WARNING(condition, arglist)
#else
	/*! \def  DBG_ASSERT_EXIT(condition,arglist)
		\brief 致命错误断言,如断言条件为假则退出程序
		\param[in] condition 断言条件
		\param[in] arglist 可变参数表
	*/
#	define DBG_ASSERT_EXIT(condition,arglist) \
	{ \
		if(_g_assert_on_) \
		{ \
			if(condition) { NULL;} \
			else{_dbg_pargs_("ASSERT ERROR",__FILE__,__LINE__); _dbg_prnt_ arglist; exit(1) ;} \
		} \
	}
	/*! \def  DBG_ASSERT_WARNING(condition,arglist)
		\brief 一般警告断言,如断言条件为假则给出警告
		\param[in] condition 断言条件
		\param[in] arglist 可变参数表
	*/	
#   define DBG_ASSERT_WARNING(condition,arglist) \
	{ \
		if(_g_assert_on_) \
		{ \
			if(condition) { NULL;} \
			else{_dbg_pargs_("ASSERT WARNING",__FILE__,__LINE__); _dbg_prnt_ arglist ; } \
		} \
	}
#endif
	

#ifdef __cplusplus 
} 
#endif


