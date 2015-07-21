/*! \file SYSModuleHeader.h
   \brief 本文件定义了SYS模块的各个配置宏开关
 */

#ifndef	__H_SYS_MODULE_MACRO_DEF_
#define	__H_SYS_MODULE_MACRO_DEF_







//////////////////////////////////////////////////////////////////////////
//为配置参数赋默认值。
//如果SYSModuleConfig.h文件中没有对某个宏做定义，则编译时会使用默认值。
//移植时不要修改这部分定义。
//////////////////////////////////////////////////////////////////////////
//#include "SYSModuleConfig.h"

//各模块的编译开关
#ifndef USE_ICONV_LIB
#define USE_ICONV_LIB						0
#endif

#ifndef USE_XML_FUNC
#define USE_XML_FUNC						1
#endif

#ifndef USE_JSON_FUNC
#define USE_JSON_FUNC						0
#endif

#ifndef USE_GZIP_LIB
#define USE_GZIP_LIB						0
#endif

#ifndef USE_MD5_FUNC
#define USE_MD5_FUNC						0
#endif

#ifndef USE_DES_FUNC
#define USE_DES_FUNC						0
#endif

#ifndef USE_RSA_FUNC
#define USE_RSA_FUNC						0
#endif

#ifndef USE_SERIAL_PROTOCOL
#define USE_SERIAL_PROTOCOL					1
#endif


//一些模块的配置参数
#ifndef SYSLOG_FUN_ON
#define	SYSLOG_FUN_ON						1		/**< 是否写系统日志开关。1-写；0-不写 */		
#endif

#ifndef ICONV_CONVERT_BUF_LEN
#define	ICONV_CONVERT_BUF_LEN				4096			/**< ICONV库所需使用的缓存大小 */
#endif



#endif	//__H_SYS_MODULE_MACRO_DEF_

