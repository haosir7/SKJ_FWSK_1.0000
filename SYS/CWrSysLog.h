/*! \file    CWrSysLog.h
   \brief    系统日志管理类定义
   \author   ZL
   \version  1.0
   \date     2011-8-17
 */

#ifndef __WR_SYSLOG_H
#define __WR_SYSLOG_H

#include "IncludeMe.h"


#define		SYSLOG_FILE_NAME		"app_log.txt"
#define		SYSLOG_FILE_NAME_OLD	"app_log_old.txt"
#define		SYSLOG_ROLL_NUM			500

/**
* @class CWrSysLog
* @brief 系统日志管理类
*/
class CWrSysLog
{														 
public:

	CWrSysLog();
	~CWrSysLog();

	/**
	 * @brief 写系统日志
	 * @param[in]  Eventtype 事件类型号
	 * @param[in]  EventResult  事件结果
	 * @param[in]  Backup 备注信息
	 * @return 1: SUCCESS, 0: FAILURE
	 */
	static INT32 WrSysLog(UINT8 Eventtype, UINT32 EventResult, string Backup);

	/**
	 * @brief LOG滚动过程
	 * @return 1滚动成功 0滚动失败
	 */
	static INT32 LogRollProc(string &strErr);



private:
	
	/**
	 * @brief 判断LOG文件是否需要滚动
	 * @return 1需滚动 0不需要滚动
	 */
	static INT32 isLogRoll();

	/**
	 * @brief 取app_log.txt中的最后一条记录序号
	 * @param[in]  fp  日志文件
	 * @return 记录序号； -1 错误
	 */
	static INT32 GetLogNum(FILE *fp);

};

#endif


