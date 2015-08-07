#ifndef __CINV_UP_FAIL_INFO_H
#define __CINV_UP_FAIL_INFO_H

 #include "CTable.h"
 #include "CDB.h"

/**
 *@class CInvUpFailInfo 
 *@brief 已退发票信息表
 *@brief 
 */
class CInvUpFailInfo
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CInvUpFailInfo();

    /*!
	@brief 析构函数	
	*/
	~CInvUpFailInfo();
	

	string m_fpdm;		/**< 发票代码 */
	UINT32 m_fphm;		/**< 发票号码 */
	string m_ErrInfo;	/**<错误描述*/
};

#endif
