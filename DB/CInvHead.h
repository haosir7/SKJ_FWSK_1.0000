#ifndef __CINV_HEAD_H
#define __CINV_HEAD_H

#include "CTable.h"
#include "CDB.h"
#include "CInvDet.h"
#include "SysMacDef.h"

/**
 *@class CInvHead 
 *@brief 发票整体信息表
 *@brief 
 */
class CInvHead : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CInvHead();

    /*!
	@brief 析构函数	
	*/
	~CInvHead();

	/*!
	@brief 读取指定日期的数据库内容
	@attention 调用前，先将pHead所指链表空间释放。
	@attention 查询，并将数据库内容写入对象成员变量中.不需要先调用Requery().
	@return SQLITE_OK:成功；SQLITE_DONE：未找到匹配结果；其它值：失败；
	*/
	INT32 Load();

	/*!
	@brief 存储内容到数据库（INV_HEAD和INV_DET表）
	@attention 调用之前，INV_DET的链表必须已经生成，且节点已被赋值。
	@return sqlite3 errorcode
	*/
	INT32 Save();

	/*!
	@brief 判断是否滚动，即表内记录太多时，是否删除部分记录
	@param[in] nCount 表实际存储的记录个数
	@param[in] strRefDate 上次申报数据的截止日期
	@param[out] nDelDate 实际要删除记录的截止日期
	@return  0 失败，  1 成功
	*/
	INT32 CheckRoll(INT32 nCount, UINT32 strRefDate, UINT32 &nDelDate);

	/*!
	@brief 删除部分记录
	@param[in] nDelDate 实际要删除记录的截止日期
	@return  0 失败，  1 成功
	*/
	INT8 DeleteRecord(UINT32 nDelDate);

	/*!
	@brief 滚动过程
	@param[in] nDelDate 实际要删除记录的截止日期
	@param[in] strRefDate 上次申报数据的截止日期
	@return  0 失败，  1 成功
	*/
	INT32 Roll(UINT32 nDelDate,  UINT32 strRefDate);


    /*!
	@brief 统计某限定条件下各开票类型的开票份数
	@param[out] norInv 正票份数
	@param[out] recInv 红票份数
	@param[out] canInv 废票份数
	@return  SQLITE_OK 成功；其他，数据库操作错误代码
	@attention 由m_filter描述限定条件
	*/
	INT32 GetInvoiceSum(UINT32 &norInv, UINT32 &recInv, UINT32 &canInv);

	/*!
	@brief 统计INV_HEAD表存储的发票数
	@return  >=0: 发票数; -1: 统计失败
	@attention 由m_filter描述限定条件
	*/
	INT32 GetInvHeadNum();
	UINT32 GetInvNumByDate();
 
//-------链表操作
    /*!
	@brief 插入新节点
	@param[in]  pNew  新节点的指针
	@return 1  成功， 0  失败
	*/
	INT32 InsertNode(CInvDet *pNew);

    /*!
	@brief 遍历链表	
	@attention  遍历每个节点，并在调试界面输出它一个字段的值
	*/
	INT32 ShowList();

	/*!
	@brief 删除节点	
	@param[in]  pCode  待删除节点的商品代码
	@attention  删除符合条件的第一个节点
	@return 1  成功， 0  失败
	*/
	INT32 DelNode(string &pCode);

	/*!
	@brief 删除节点	
	@param[in]  dpNo  待删除节点的部类号
	@attention  删除符合条件的第一个节点
	@return 1  成功， 0  失败
	*/
//	INT32 DelNode(UINT8 dpNo);

	/*!
	@brief 删除最后一个节点	
	@return SUCCESS  成功， FAILURE  失败
	*/
	INT32 DelLastNode();

	/*!
	@brief 删除链表，释放链表空间	
	@return 1  成功， 0  失败
	*/
	INT32 DelList();

	/*!
	@brief 搜索节点	
	@param[in]  pCode  待删除节点的商品代码
	@return 非NULL  搜索到的节点指针， NULL 失败
	*/
	CInvDet* SearchNode(string &pCode);

	/*!
	@brief 搜索节点	
	@param[in]  dpNo  待删除节点的部类号
	@return 非NULL  搜索到的节点指针， NULL 失败
	*/
//	CInvDet* SearchNode(UINT8 dpNo);


	UINT32 m_no; 			/**< 索引号 */
	string m_fpdm;			/**< 发票代码 */
	UINT32 m_fphm;			/**< 发票号码 */
	UINT32 m_kprq;			/**< 开票日期 */
	UINT32 m_kpsj;			/**< 开票时间 */
	UINT8  m_kplx;			/**< 开票类型(正、废、红票) */
	INT64  m_kphjje;		/**< 开票合计金额(含税) */
	INT64  m_kpse;		    /**< 开票税额 */
	string m_yfpdm;			/**< 原发票代码 */
	UINT32 m_yfphm;			/**< 原发票号码 */
	string m_fkdw;			/**< 付款单位 */
	string m_fkdwsh;        /**< 付款单位税号 */
	string m_sky;			/**< 收款员 */
	UINT32 m_sphsl;			/**< 商品行数量 */
	string  m_fwm;			/**< 防伪税控码 */
	UINT8 m_fplb;		     /**< 发票类别(卷票41 ) */
	UINT8  m_scbz;			/**< 上传标志 */
	string m_bzkz;          /**< 备注扩展 (0--未被冲红，1--已被冲红)*/

	INT64 m_kpje;			/**< 开票金额（不含税） */

    string m_fpsyh;           /**< 发票索引号*/
	string m_casign;			/**< CA签名*/
	
	string m_backup1;		/**< 备用1 */
	string m_backup2;		/**< 备用2 */
//	string m_backup3;		/**< 备用3 */

//不存数据库
	string m_zskl;			//证书口令
	float  m_sl;			/**< 税率 */
	UINT32 m_dycs;			/**< 打印次数 */
	string m_skr;			/**< 收款人 */
	UINT8  m_zfbz;			/**<  作废标志*/
	UINT32 m_zfrq;			/**<  作废日期*/
	UINT32 m_zfsj;			/**<  作废时间*/
	string  m_hczt;          /**< 红冲标志*/ 
	string m_hyfl;			/**< 行业分类 */

    CInvDet *pHead;			/**< 商品明细链表头 */
	CInvDet *pEnd;			/**< 商品明细链表尾？ */
	CInvDet m_InvDet;		/**< 商品明细子表 */
	CInvDet *m_pInvDet;		/**< 商品明细子表指针 */
	string m_xfmc;      /**< 销方名称 */
	string m_xfsh;      /**< 销方税号 */
};

#endif
