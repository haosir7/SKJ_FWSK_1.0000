#ifndef __CINVSERV_H
#define __CINVSERV_H

#include "CTable.h"
#include "CDB.h"

#include "CInvHead.h"

/**
 *@class CInvServ 
 *@brief 发票服务表
 *@brief 
 */
class CInvServ : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CInvServ();

    /*!
	@brief 析构函数	
	*/
	~CInvServ();

	/*!
	@brief 保存一条记录
	@param[in]  pInv  发票记录
	@param[in]  upFlag  上传标志 0=未上传，1 = 已上传
	@param[in]  backup  作废类型
	@param[in]  caSign  CA签名+证书序列号
	@param[out]  nNum  缓存的张数（存入Inv_Serv表的）
	@return 1  成功， 0  失败
	*/
	UINT8 Save(const CInvHead *pInv, UINT32 &nNum, INT64 &nSum, string &StrEr);

	UINT32 m_no; 			/**< 索引号 */
	UINT32 m_InvNo;			/**< 发票号码 */
	string m_code;       /**< 发票代码 */
	
	UINT32 m_issueDate;		/**< 开票日期 */
	UINT32 m_issueTime;		/**< 开票时间 */
	UINT8  m_issuetype;		/**< 开票类型 */
	
	INT64 m_moneySum;		/**< 开票总金额 */
	UINT8 m_upFlag;			/**< 上传标志 0=未上传，1 = 已上传  */
	UINT8 m_wstFlag;		/**< 作废标志  0=未作废， 1=已作废*/
	string m_caSign;		/**< CA签名+证书序列号*/
	string m_backup;		/**< 备用字段 */
};
#endif
