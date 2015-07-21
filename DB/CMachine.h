#ifndef __CMACHINE_H
#define __CMACHINE_H

#include "CTable.h"
#include "CDB.h"


/**
 *@class CMachine 
 *@brief 收款机出厂信息表
 *@brief 
 */
class CMachine : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CMachine();

    /*!
	@brief 析构函数	
	*/
	~CMachine();
	
	string m_machineNo;		/**< 机器编号 */
	string m_pDate;			/**< 生产日期 */		//原为UINT32
	string m_hVer;			/**< 硬件版本号 */
	string m_sVer;			/**< 软件版本号 */

	string m_IMEI;		/**< 无线模块IMEI编号 */
};

#endif
