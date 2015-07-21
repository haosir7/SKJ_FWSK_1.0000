#ifndef __CUSERINFO_H
#define __CUSERINFO_H

#include "CTable.h"
#include "CDB.h"
#include "SysMacDef.h"

/**
 *@class CUserInfo 
 *@brief 用户信息表
 *@brief 
 */
class CUserInfo : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CUserInfo();

    /*!
	@brief 析构函数	
	*/
	~CUserInfo();

	/*!
	@brief 复位全体成员变量（清零）	
	*/
	void ResetMember(void);
	

	string m_Nsrsbh;			/**< 纳税人识别号 */
	string m_Nsrmc;				/**< 纳税人名称 */
	UINT32 m_Kpjhm;				/**< 开票机号码 */
	UINT8  m_zfFlag;            /**<主分机标志 0代表主机 1代表分机*/
	string  m_Qylx;				/**< 纳税人企业类型 */
	string m_Jspsbh;			/**< 金税盘设备号 */
	string m_Gsdjh;				/**< 工商登记号 */
	string m_Khyh;				/**< 开户银行 */
	string m_Khzh;				/**< 开户账号 */
	string m_Djzcdz;			/**< 登记注册地址 */
	string m_Swjgdm;			/**< 主管税务机关代码 */
	string m_Swjgmc;			/**< 主管税务机关名称 */
	UINT32 m_Slgs;				/**< 普票授权税率个数 */
	UINT32 m_Kpjsl;				/**< 开票机数量 */
	string m_qysj;				/**< 启用时间 YYYYMMDDHHMMSS */
	string m_backup;			/**< 备用 */

	//不存数据库
	
	string m_ffbz;		/**< 分发标志 */
	string m_bbh;		/**< 版本号 */
	string m_blxx;		/**< 保留信息 */
	string m_fplxdm;    /**< 发票类型代码 */
	string m_qtkzxx;    /**< 其他扩展信息 */
	string m_sksbkl;
	UINT32 m_Fplxgs;	/**< 发票类型个数 */
	string m_Fplxsz;	/**< 发票类型 */
	string m_bspbh;		/**< 报税盘编号 */
};

#endif
