#ifndef __CINV_DET_H
#define __CINV_DET_H

#include "CTable.h"
#include "CDB.h"

#define DET_ARRAY_LEN      64     //s_DetArray[]静态数组的长度

class CInvDet;
extern CInvDet *s_DetArray[DET_ARRAY_LEN];

/**
 *@class CInvDet 
 *@brief 发票明细信息表
 *@brief 
 */
class CInvDet : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CInvDet();

    /*!
	@brief 析构函数	
	*/
	~CInvDet();

	/*!
	@brief 滚动，即表内记录太多时，删除部分记录
	@return   SQLITE_OK 成功； 其他 sqlite3的错误号
	*/
	INT32 Roll(void);
    
	/*!
	@brief 统计某限定条件下税种税目的分类开票金额
	@param[in]  taxIDNum 税种税目最大数目
	@param[in] taxID 税种税目数组的起始地址
	@param[out] Money 保存分类金额的数组起始地址
	@return  实际开票的税种税目数量
	@attention 由m_filter描述限定条件 
	*/
//	UINT32 GetMoneySum(UINT32 *Money, UINT8 *taxID, UINT8 taxIDNum);

	/*!
	@brief 统计某限定条件下开票金额
	@return  某限定条件下开票金额
	@attention 由m_filter描述限定条件 
	*/
	INT64 PartMoneySum();

	/*!
	@brief 获取s_DetArray[]中一个可用的CInvDet类对象
	@return  CInvDet* 可用的CInvDet类对象的指针
	@attention 若无可用对象，则返回NULL 
	*/
	CInvDet* GetNewInvDet(void);

	/*!
	@brief 恢复s_DetArray[]中一个CInvDet类对象为可用状态
	@return  SUCCESS 成功
	*/
	UINT8 FreeInvDet();
    
	INT32  m_no; 			/**< 自增 */
	string m_fpdm;			/**< 发票代码 */
	UINT32 m_fphm;			/**< 发票号码 */
	
	UINT32 m_kprq;			/**< 开票日期 */
	UINT32 m_kpsj;			/**< 开票时间 */
	UINT8  m_kplx;			/**< 开票类型 */
	string m_sky;			/**< 收款员 */
	
	UINT32 m_sphxh;			/**< 商品行序号，从1开始 */
	string m_spbm;			/**< 商品编码 */
	string m_spmc;			/**< 商品名称 */
	double m_spdj;			/**< 商品单价(含税) */
	double m_spsl;			/**< 商品数量 */
	INT64  m_spje;			/**< 商品金额(含税) */
	INT64 m_spse;			/**< 商品税额 */
	float  m_sl;			/**< 税率 */
	float  m_zsl;			/**< 征收率 */
	UINT8  m_property;		/**< 商品行属性 */
	string m_spdw;			/**< 单位 */
	string m_ggxh;			/**< 规格型号 */

	double m_dj;		   /**< 商品单价(不含税) */
	INT64  m_je;		   /**< 商品金额 (不含税)*/

	
	INT32 m_dotNum;         /**< 数量四舍五入后的小数位数 */
	class CInvDet *pNext;	/**< 指向下一个商品明细信息表 */
	UINT8 m_useFlag;		/**< 是否可用标志，0==可用，1==不可用 */

	string m_backup;		/**< 备用 */


};

#endif
