#ifndef __CINV_VOL_H
#define __CINV_VOL_H

#include "CTable.h"
#include "CDB.h"
#include "SysMacDef.h"

/**
 *@class CInvVol 
 *@brief 发票领用信息表
 *@brief 
 */
class CInvVol : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CInvVol();

    /*!
	@brief 析构函数	
	*/
	~CInvVol();

	/*!
	@brief 删除已用完的发票卷信息	
	*/
	void DelUsedVol();

	/*!
	@brief 删除已用完的发票卷信息	
	*/
	void ResetVol();

	UINT32 m_no; 			/**< 索引号 */
	UINT8  m_invtype;		/**< 发票类别 */
	string m_code;			/**< 发票代码 */
	UINT32 m_isno;			/**< 发票起始号码 */
	UINT32 m_ieno;			/**< 发票终止号码 */
	UINT32 m_remain;		/**< 剩余份数 */
	UINT32 m_fpzfs;			/**< 发票总份数 */
	UINT8  m_usedflag;		/**< 发票卷使用标志 1:该发票卷未用来开票 0:已用来开票 */
	UINT8  m_overflag;		/**< 发票卷用完标志 1:未用完 0:用完 */
	string m_kindcode;		/**< 发票种类代码 */
	UINT32 m_date;			/**< 购买日期 */
	UINT32 m_backup1;		/**< 备用字段1 */
	string m_backup2;		/**< 备用字段2 */
	string m_fplxdm;	

	//---不存库----//
	UINT32 m_curInvNo;    /**< 当前发票号，不存库 */
	string m_buyDate;    /**< 购买日期(盘上获取的时间)*/	
	UINT32 m_FlagJSorXZ;		/**< 业务九:发票卷解锁or下载标识，1表示解锁，0表示下载*/
	UINT32 m_fpzl;		/**< 发票种类*/
	string m_lbdm;		/**< 类别代码*/
	string m_flag;		/**< 写盘锁死发票卷密文信息*/
	string m_gpsq;		/**< 购票授权*/
	string m_yssh;		/**<压缩税号*/
	string m_dqbh;		/**<地区编号*/
	string m_dcbb;		/**<底层版本*/
	string m_fpjjsmw;		/**<发票卷解锁密文*/
	string m_fpjmw;		/**<发票密文信息*/
};

#endif
