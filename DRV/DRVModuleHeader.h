/*! \file DRVModuleHeader.h
   \brief 本文件定义了DRV模块的各个配置宏开关
 */

#ifndef	__H_DRV_MODULE_MACRO_DEF_
#define	__H_DRV_MODULE_MACRO_DEF_


//////////////////////////////////////////////////////////////////////////
//为配置参数赋默认值。
//如果DRVModuleConfig.h文件中没有对某个宏做定义，则编译时会使用默认值。
//移植时不要修改这部分定义。
//////////////////////////////////////////////////////////////////////////
//#include "DRVModuleConfig.h"

//各模块的编译开关

#ifndef USE_HW_DRV
#define USE_HW_DRV							1
#endif

#ifndef USE_SCANNER_DRV
#define USE_SCANNER_DRV						1
#endif

#ifndef USE_RTC_DRV
#define USE_RTC_DRV							1
#endif


#ifndef USE_VFD_DRV
#define USE_VFD_DRV							1
#endif


#ifndef USE_POWOFF_PROTECT_DRV
#define USE_POWOFF_PROTECT_DRV				1
#endif


#endif	//__H_DRV_MODULE_MACRO_DEF_

