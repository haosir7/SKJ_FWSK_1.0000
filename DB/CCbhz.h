#ifndef __CCBHZ_H
#define __CCBHZ_H

#include "CTable.h"
#include "CDB.h"


/**
 *@class CCbhz
 *@brief 抄报汇总表
 *@brief 
 */
class CCbhz : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CCbhz();

    /*!
	@brief 析构函数	
	*/
	~CCbhz();

	void clear();
	
	UINT32  m_no;		/**< 编号 */
	UINT32  m_Ssq;		/**< 所属期 */
	UINT32  m_Ssqq;		/**< 所属期起 */
	UINT32  m_Ssqz;		/**< 所属期止 */
	UINT32  m_Zpfs;		/**< 正票份数 */
	INT64	m_Zpkpje;	/**< 正票开票金额 */
	UINT32  m_Hpfs;		/**< 红票份数 */
	INT64   m_Hpkpje;	/**< 红票开票金额 */
	UINT32  m_Zffs;		/**< 废票份数 */
	INT64   m_Zfkpje;	/**< 废票开票金额 */
	UINT32  m_Hffs;		/**< 红废份数 */
	INT64   m_Hfkpje;	/**< 红废开票金额 */
	UINT32  m_Kbfpfs;	/**< 空白发票份数 */
	UINT32  m_Bsrq;		/**< 抄报日期 */
	UINT32  m_Bscgrq;	/**< 抄报成功日期 */
	string  m_Bssbyy;	/**< 失败原因 */
	UINT8   m_Bslx;		/**< 抄报类型 */
	UINT8   m_Bscgbz;	/**< 报税成功标志 */
	UINT8   m_Bsjzbz;	/**< 报送介质标志 */
	UINT32  m_Wscfps;	/**< 未上传发票份数 */
};

#endif
