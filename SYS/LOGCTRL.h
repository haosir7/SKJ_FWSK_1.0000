/*! \file LOGCTRL.h
   \brief 本文件定义了一些控制LOG输出的宏开关
   \note 本文件在引用是应置于poslog.h与pos_debug.h之前
   \author zyk
 */


//-----------------------------------------
//此处的宏定义不可修改
#ifdef NO_POS_DEBUG
#undef NO_POS_DEBUG
#endif

#ifdef POS_ENTER_ON
#undef POS_ENTER_ON
#endif
#ifdef POS_DBGPRN_ON
#undef POS_DBGPRN_ON
#endif
#ifdef POS_ASSERT_ON
#undef POS_ASSERT_ON
#endif

#ifdef POS_DEBUG_TO_FILE
#undef POS_DEBUG_TO_FILE
#endif
#ifdef POS_DEBUG_TO_SERIAL
#undef POS_DEBUG_TO_SERIAL
#endif
#ifdef POS_DEBUG_TO_TTY
#undef POS_DEBUG_TO_TTY
#endif
//-----------------------------------------
//以下宏定义可以修改
//#define NO_POS_DEBUG //关闭整个工程的log信息

//#define POS_ENTER_ON
#define POS_DBGPRN_ON
#define POS_ASSERT_ON

//#define POS_DEBUG_TO_FILE
//#define POS_DEBUG_TO_SERIAL
#define POS_DEBUG_TO_TTY


