/*! \file TPLModuleHeader.h
   \brief 本文件定义了TPL模块的各个配置宏开关
 */

#ifndef	__H_TPL_MODULE_MACRO_DEF_
#define	__H_TPL_MODULE_MACRO_DEF_







//////////////////////////////////////////////////////////////////////////
//为配置参数赋默认值。
//如果TPLModuleConfig.h文件中没有对某个宏做定义，则编译时会使用默认值。
//移植时不要修改这部分定义。
//////////////////////////////////////////////////////////////////////////
//#include "TPLModuleConfig.h"

//各模块的编译开关
#ifndef PRINT_BIZNFO
#define PRINT_BIZNFO						0
#endif


#endif	//__H_TPL_MODULE_MACRO_DEF_

