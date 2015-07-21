/*! \file    SaleData.cpp
\brief    awe4000r销售程序中的销售数据类
\author   Yu Yan
\version  1.0
\date     2007-11-28
*/


#include "SaleData.h"
//#include "SysMacDef.h"
#include "TDateTime.h"
#include "VersionConfig.h"

#include "CaProgressBar.h"
#include "CaMsgBox.h"
#include "CInvServ.h"
#include "CInvSum.h"
#include "powoff_protect.h"
#include "CTax.h"


#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


#define POWEROF_SPEED_TEST	0  /**< 测试需要掉电保护的代码的运行时间*/

#ifndef WIN32
#if POWEROF_SPEED_TEST
#include <sys/timeb.h>
#endif
#endif



SaleData *pSaleData = NULL; /**< 销售数据类指针*/


SaleData::SaleData()
{
	DBG_PRINT(("进入SaleData函数。销售信息初始化。"));
	
	InitSaleData(1);  
	m_deptInfo = NULL;				//部类信息				
	m_pInvPrnData = NULL;			//发票打印数据			
	m_singleInvInfo = NULL;	        //单张发票信息
	m_smallInvInfo = NULL;          //单张打印发票信息 
	m_invDet = NULL;				//?
	m_pHead = NULL;					//?
	m_tmpGoodsNum = 0;
	m_returnOther = 0;	
	m_nInvCount=0;
	m_singleInvInfo = new CInvHead();	//单张发票信息初始化
	InitInvHead();
	
	DBG_PRINT(("退出SaleData函数。"));
}

SaleData::~SaleData()
{
	m_deptInfo = NULL;
	
	if( m_singleInvInfo != NULL )
	{
		delete m_singleInvInfo;
		m_singleInvInfo = NULL;
	}
	
	m_smallInvInfo = NULL;	
}


//UINT8 SaleData::SearchGoodsByPLU(string PLUCode, CPlu *pluInfo)
//{
// 	DBG_ENTER("SaleData::SearchGoodsByPLU");
// 	DBG_ASSERT_EXIT((pluInfo != NULL), (" pluInfo == NULL!"));
// 	
// 	char value[256];
// 	memset((void*)value, 0, sizeof(value));
// 	string filter;
// 	INT32 errorcode;
// 	UINT8 ret = SUCCESS;
// 
// 	pluInfo->m_filter.append("WHERE CODE = '");
// 	pluInfo->m_filter.append(PLUCode);
// 	pluInfo->m_filter.append("'");
// 
// 	//查询
// 	pluInfo->Requery();
// 	errorcode = pluInfo->LoadOneRecord();
// 	DBG_PRINT(("errorcode : %d ", errorcode));	
// 	if (errorcode != SQLITE_OK)
// 	{
// 		DBG_PRINT((" Warning: No matched goods!"));
// 		ret = NO_SUCH_GOODS;
// 		DBG_RETURN(ret);	
// 	}
// 
//     DBG_PRINT(("m_code : %s ", pluInfo->m_code.c_str()));
// 	DBG_PRINT(("m_name : %s ", pluInfo->m_name.c_str()));
// 	DBG_PRINT(("m_nameAbbr : %s ", pluInfo->m_nameAbbr.c_str()));
// 	DBG_PRINT(("m_dept : %d ", pluInfo->m_dept));
// 	DBG_PRINT(("m_taxId : %d ", pluInfo->m_taxId));
// 	DBG_PRINT(("m_price : %lf ", pluInfo->m_price));
// 	DBG_PRINT(("m_rate : %f ", pluInfo->m_rate));
// 	DBG_PRINT(("m_unit : %s ", pluInfo->m_unit.c_str()));
// 
// 	DBG_RETURN(ret);
//}


UINT8 SaleData::PLUSale(string strName, string &strErr)
{
	DBG_ENTER("SaleData::PLUSale");
	
	//	char value[256];
	//	memset((void*)value, 0, sizeof(value));
	//	string filter;
	//	INT32 errorcode;
	UINT8 ret = SUCCESS;
	INT32 nLen = strName.length();
	
	DBG_PRINT(("strName : %s ", strName.c_str()));
	if (nLen > GOODS_NAME_LEN) 
	{
		strErr = "商品名称过长";
		DBG_RETURN(FAILURE);		
	}
	else if (nLen <= 0) 
	{
		strErr = "请输入商品名称";
		DBG_RETURN(FAILURE);		
	}
	
	//查询商品信息
	CDept curDept;
	m_deptInfo = &curDept;
	
	//造假商品信息
	m_deptInfo->m_spbm = "0000000000000000";		/**< 编码 */
	m_deptInfo->m_spmc = strName;		/**< 名称 */
	m_deptInfo->m_spjm = "";		/**< 简称 */
	m_deptInfo->m_spgg = "";		/**< 规格 */
	m_deptInfo->m_spdw = "件";		/**< 单位 */
	m_deptInfo->m_zspmdm = "";	/**< 征收商品代码(税目) */
	m_deptInfo->m_spsl = 0.0;		/**< 税率 */
	m_deptInfo->m_spzsl = 0.0;		/**< 征收率 */
	m_deptInfo->m_spdj = m_tmpPrice;		/**< 单价 */
	m_deptInfo->m_rate = 100.0;		/**< 商品折扣率 */
	m_deptInfo->m_blh = 0;       /**< 对应的部类号，默认为零 */
	
	DBG_PRINT(("临时商品信息如下:"));
    DBG_PRINT(("m_spbm : %s ", m_deptInfo->m_spbm.c_str()));
	DBG_PRINT(("m_spmc : %s ", m_deptInfo->m_spmc.c_str()));
	DBG_PRINT(("m_spsl : %f ", m_deptInfo->m_spsl));
	DBG_PRINT(("m_spjm : %s ", m_deptInfo->m_spjm.c_str()));
	DBG_PRINT(("m_spdj : %f ", m_deptInfo->m_spdj));
	
	//若商品信息无单价，则必须总价销售
	DBG_PRINT(("m_tmpSum==%d", m_tmpSum));
	if( m_deptInfo->m_spdj == 0 )
	{
		if( (m_cancel != CANCEL_PRODUCT) && (m_tmpSum == 0) )
		{
			ret = MUST_INPUT_SUM;			
			InitSaleData(0);  //销售信息初始化
			DBG_PRINT(("退出DeptSale函数"));
			DBG_RETURN(ret);			
		}
		//如果商品数量为零,那么改为1
		if (m_tmpAmount == 0)
		{
			m_tmpAmount = 1;
		}
		DBG_PRINT(("数量: %.2f", m_tmpAmount));
		// 		m_deptInfo->m_spdj = ((double)m_tmpSum)/SUM_EXTENSION;//将总价赋值给单价
		// 		DBG_PRINT(("m_spdj == %f ", m_deptInfo->m_spdj));
		m_deptInfo->m_spdj = ((double)m_tmpSum)/SUM_EXTENSION / m_tmpAmount;
		//处理参数
		m_tmpSum = 0;
		//		m_tmpAmount = 1.0;   //强制数量为1
		m_saveAmount = 1.0;  //强制数量为1
		m_dotNum = 0;
		m_tmpPrice = 0.0;    //不接受临时价销售
	}
	
	//m_deptInfo = &curDept;
	//	DBG_ASSERT_EXIT((m_deptInfo != NULL), (" m_deptInfo == NULL!"));
	
	//进入销售主流程函数
	ret = Sale(m_deptInfo);
	m_deptInfo = NULL;
	InitSaleData(0);  //销售信息初始化
	DBG_RETURN(ret);
}

//-------------------------------------------------------------------
//销售主流程
//-------------------------------------------------------------------
UINT8 SaleData::Sale( CDept *deptInfo )
{
	DBG_ENTER("SaleData::Sale");
	DBG_ASSERT_EXIT((m_deptInfo != NULL), (" m_deptInfo == NULL!"));
	DBG_PRINT(("进入Sale函数"));
	
	char value[256];
	memset((void*)value, 0, sizeof(value));
	string filter;
	INT32 errorcode;
	UINT8 ret;
	INT64 tmpDJ=0;
	
	//如果当前已无发票，则退出该函数。
	//if( 0 == g_globalArg->m_curInvVol->m_remain ) 
	if (0==g_globalArg->m_curInvVol->m_curInvNo)
	{
		InitSaleData(0);  //销售信息初始化
		DBG_RETURN(INV_ZERO);			
	}
	
	//设置开票类型和原发票号。
	m_singleInvInfo->m_kplx = m_invtype;		//开票类型
    m_singleInvInfo->m_yfphm = m_oldInv;		//原发票号
	m_singleInvInfo->m_yfpdm = m_oldCodeNo;		//原发票代码
	
	//如果商品的折扣率不等于100	//xsr 疑问。m_property在Discount函数里被赋了值。
	DBG_PRINT(("deptInfo->m_rate==%f", deptInfo->m_rate));
	if( 0==doublecmp(deptInfo->m_rate, 100.00) && (m_property != DETAIL_GOODS_DISCOUNT) )
	{
		m_property = DETAIL_GOODS_DISCOUNT;
		m_tmpRate = (INT32)(deptInfo->m_rate);
		DBG_PRINT(("m_tmpRate =  %f", m_tmpRate));
	}
	
	//判断临时价
    DBG_PRINT(("m_tmpPrice == %lf ", m_tmpPrice));
    if( m_tmpPrice >= BASE_PRICE )
	{
		deptInfo->m_spdj = m_tmpPrice;
		m_tmpPrice = 0;
	}
	
	//若是商品取消
	if (m_cancel == CANCEL_PRODUCT)
	{
		double dPrice;
		DBG_PRINT(("deptInfo->m_spbm = %s", deptInfo->m_spbm.c_str()));
		
		ret = CancelGoodsByDept(deptInfo->m_spbm, m_saveAmount, dPrice); 
		
		if( NO_SUCH_GOODS == ret )
		{
			m_invDet = m_singleInvInfo->pEnd;
			InitSaleData(0);  //销售信息初始化
			DBG_PRINT(("*******NO_SUCH_GOODS*******NO_SUCH_GOODS*******!"));
			DBG_RETURN(ret);		
		}
		m_invDet = &m_cancelDet; 
		DBG_ASSERT_EXIT((m_invDet != NULL), (" m_invDet == NULL!"));
		m_invDet->m_spmc = deptInfo->m_spmc;
		m_invDet->m_spdj = dPrice;
		m_invDet->m_dotNum = AmountRound(&m_realCancelNum); //商品数量四舍五入	
		m_invDet->m_spsl = m_realCancelNum;
		DBG_PRINT(("m_realCancelNum = %f!", m_realCancelNum));
		InitSaleData(0);  //销售信息初始化
		DBG_RETURN(ret);
	}
	
    //判断商品金额是否超单张开票限额
	INT64 moneySum, orgMoneySum;			//orgMoneySum是折扣前该商品行的金额，moneySum是折扣后该商品行的金额。
	INT64 moneyTaxSum=0;
	DBG_PRINT(("m_tmpSum==%lf ", m_tmpSum));//m_tmpSum在DeptSale函数里置成了0
	//如果用户输入了总价
	if( m_tmpSum > 0 )  
	{
		orgMoneySum = m_tmpSum;										//原始总金额
		m_tmpAmount = orgMoneySum/(deptInfo->m_spdj)/SUM_EXTENSION;	//商品数量
		m_saveAmount = m_tmpAmount;
		DBG_PRINT(("m_saveAmount : %f ", m_saveAmount));
		m_dotNum = AmountRound(&m_saveAmount); //商品数量四舍五入
		DBG_PRINT(("m_saveAmount : %f ", m_saveAmount));
		DBG_PRINT(("m_dotNum : %d ", m_dotNum)); 
		if (m_dotNum == -1)
		{
			m_dotNum = 0;
			m_tmpAmount = 1.0;
			m_saveAmount = 1.0;
			m_tmpSum = 0;
			DBG_PRINT(("The goods amount exceeds the limit!"));
			InitSaleData(0);  //销售信息初始化
			DBG_RETURN(EXCEED_AMOUNT);
		}
	}
	//如果用户没输入总价
	else
	{
		DBG_PRINT(("deptInfo->m_spdj==%lf ", deptInfo->m_spdj));
		DBG_PRINT(("m_tmpAmount==%lf ", m_tmpAmount));

		orgMoneySum = double2int(deptInfo->m_spdj * 1000.0 * m_tmpAmount * SUM_EXTENSION);//原始总金额
		DBG_PRINT(("orgMoneySum == %lld", orgMoneySum));
		orgMoneySum = double2int(orgMoneySum / 1000.0);
		DBG_PRINT(("orgMoneySum == %lld", orgMoneySum));
	}
	//至此，orgMoneySum里存入了该商品行的金额。100元。
	
    //如果有折扣。
    if( m_property==DETAIL_GOODS_DISCOUNT )  
	{
		moneySum = double2int(deptInfo->m_spdj * m_tmpRate/100*1000.0*m_tmpAmount*SUM_EXTENSION);//原始总金额
		moneySum = double2int(moneySum / 1000.0);
		DBG_PRINT(("moneySum = %lld", moneySum));
	}
	else
	{
		moneySum = orgMoneySum;
		DBG_PRINT((" moneySum = %lld", moneySum));
	}
	//至此，moneySum里存入了折扣后的该商品行的金额。80元。
	
	//单张金额超上限
	if ((moneySum > g_globalArg->m_invKind->m_maxSign)||
		((m_tmpMoneySum+moneySum) > g_globalArg->m_invKind->m_maxSign)) 
	{	
		ret = NM_EXCEED;
        DBG_PRINT((" g_globalArg->m_invKind->m_maxSign = %u!", g_globalArg->m_invKind->m_maxSign));
		DBG_PRINT((" Warning: sigal invoice money exceed the limit!"));
		InitSaleData(0);  //销售信息初始化
		DBG_RETURN(ret);	
	}
	
	//金额不足
	DBG_PRINT(("moneySum==%lld ", moneySum));
	if (moneySum <= 0)
	{
		DBG_PRINT(("moneySum==%d，not enough!", moneySum));
		InitSaleData(0);  //销售信息初始化
		DBG_RETURN(MONEY_NOT_ENOUGH);
	}
	//折扣掉的金额为0
	if( (m_property==DETAIL_GOODS_DISCOUNT) && (moneySum - orgMoneySum >= 0) )
	{
		DBG_PRINT((" 折扣掉的金额moneySum - orgMoneySum = %d", moneySum - orgMoneySum));
		InitSaleData(0);  //销售信息初始化
		DBG_RETURN(DISCOUNT_MONEY_ZERO);
	}
	
	//累计金额超限
	m_tmpMoneySum += moneySum; //单张发票金额累计
	DBG_PRINT((" g_globalArg->m_normalSum = %lld!", g_globalArg->m_normalSum));
	DBG_PRINT((" g_globalArg->m_returnSum = %lld!", g_globalArg->m_returnSum));
	if (g_globalArg->m_operator->m_role != DEMO_ROLE) //非学习模式
	{
		switch(m_singleInvInfo->m_kplx) 
		{
		case NORMAL_INV:
			//单卷正票累计金额超限
			//			if ((g_globalArg->m_normalSum + m_tmpMoneySum)>g_globalArg->m_invKind->m_maxSum)
			//			{
			//				ret = NM_SUM_EXCEED;//NVM_SUM_EXCEED;
			//				m_tmpMoneySum -= moneySum;//本张累计金额减去本商品行金额
			//				DBG_PRINT((" Warning: m_normalVolSum exceed the limit!"));
			//				InitSaleData(0);  //销售信息初始化
			//				DBG_RETURN(ret);
			//			}
			//			break;
		case RET_MANUAL_INV:   //负数手工发票（退货） 
		case RET_SPECIAL_INV:  //负数特殊发票
		case RETURN_INV:  
			//单卷红票累计金额超限
			//			if ((g_globalArg->m_returnSum + m_tmpMoneySum)>g_globalArg->m_invKind->m_maxRetSum)
			//			{
			//				ret = RM_SUM_EXCEED;//RVM_SUM_EXCEED;
			//				m_tmpMoneySum -= moneySum;//本张累计金额减去本商品行金额
			//				DBG_PRINT((" Warning: m_returnVolSum exceed the limit!"));
			//				InitSaleData(0);  //销售信息初始化
			//				DBG_RETURN(ret);
			//			}
			break;
		default:;
		}
	}
	
	//----判断税种税目是否合法
	//	 UINT8 nLeagal = 0;
	//	if (g_globalArg->m_operator->m_role != DEMO_ROLE) //非学习模式
	//	{
	// 		if (deptInfo->m_taxId ==0 )
	// 		{
	// 			ret = TAX_ID_ILLEGAL;
	// 			m_tmpMoneySum -= moneySum;//本张累计金额减去本商品行金额
	// 			InitSaleData(0);  //销售信息初始化 
	// 			DBG_RETURN(ret);
	// 		}
	// 		for(INT32 i=0; i<TAX_ID_NUM; i++)
	// 		{
	// 			if (pluInfo->m_taxId == g_globalArg->m_taxID[i]) 
	// 			{
	// 				nLeagal = 1;
	// 				break;
	// 			}
	// 		}
	// 		if (nLeagal==0)
	// 		{ 
	// 			ret = TAX_ID_ILLEGAL;
	// 			m_tmpMoneySum -= moneySum;//本张累计金额减去本商品行金额
	// 			DBG_PRINT((" Warning: the tax ID is illegal!"));
	// 			InitSaleData(0);  //销售信息初始化
	// 			DBG_RETURN(ret);		
	// 		}
	//	}
	
	
    //添加到发票明细
	m_invDet = m_invDet->GetNewInvDet();	//从静态数组获取一个可用的CInvDet对象
	DBG_ASSERT_EXIT((m_invDet != NULL), (" m_invDet == NULL!"));
	
	m_invDet->m_kplx = m_invtype;           //开票类型，正、废、红票
	m_invDet->m_sky = g_globalArg->m_operator->m_name; //操作员编码
	m_invDet->m_spbm = deptInfo->m_spbm;	//商品编码
	DBG_PRINT(("m_invDet->m_spbm == %s", m_invDet->m_spbm.c_str()));
	m_invDet->m_spmc = deptInfo->m_spmc;	//商品名称
	m_invDet->m_spsl = m_tmpAmount;			//商品数量
	m_invDet->m_dotNum = m_dotNum;			// 四舍五入后数量小数位数
	m_invDet->m_spdj = deptInfo->m_spdj;	//商品单价(含税)
	m_invDet->m_spje = orgMoneySum;			//商品金额(含税)
	m_invDet->m_sl = deptInfo->m_spsl;		//商品税率
//	m_invDet->m_spse = CountTax(((double)m_invDet->m_spje)/SUM_EXTENSION, m_invDet->m_sl); //商品税额

	m_invDet->m_spse = CountTax(m_invDet->m_spje, (UINT32)(m_invDet->m_sl*SUM_EXTENSION)); //商品税额

	m_invDet->m_property = m_property;		//商品行属性
	m_invDet->m_spdw = deptInfo->m_spdw;	//商品单位
	
	tmpDJ = double2int(m_invDet->m_spdj*SUM_EXTENSION);
	DBG_PRINT(("tmpDJ==%lld ", tmpDJ));

	//tmpDJ -=CountTax(m_invDet->m_spdj, m_invDet->m_sl);
	tmpDJ -=CountTax(double2int(m_invDet->m_spdj*SUM_EXTENSION), (UINT32)(m_invDet->m_sl*SUM_EXTENSION));
	DBG_PRINT(("tmpDJ==%lld ", tmpDJ));

	m_invDet->m_dj= (double)(tmpDJ*1.0)/SUM_EXTENSION; //商品单价(不含税)
	
	
	m_invDet->m_je= m_invDet->m_spje-m_invDet->m_spse; //商品金额(不含税)
	
	m_singleInvInfo->InsertNode(m_invDet);	//插入节点
	m_singleInvInfo->m_sphsl++;				//当前发票总商品行加1
	m_tmpGoodsNum++;						//当前总商品行加1
	
	moneyTaxSum =m_invDet->m_spse;
	
	
	DBG_PRINT(("m_invDet->m_dotNum==%d ", m_invDet->m_dotNum));
	DBG_PRINT(("m_tmpGoodsNum==%u", m_tmpGoodsNum));

	DBG_PRINT(("m_invDet->m_spsl==%lf ", m_invDet->m_spsl));
	DBG_PRINT(("m_invDet->m_spse==%lld ", m_invDet->m_spse));
	DBG_PRINT(("m_invDet->m_spje==%lld ", m_invDet->m_spje));
    DBG_PRINT(("m_invDet->m_je==%lld ", m_invDet->m_je));
	DBG_PRINT(("m_invDet->m_spdj==%lf ", m_invDet->m_spdj));
    DBG_PRINT(("m_invDet->m_dj==%lf ", m_invDet->m_dj));
 
	DBG_PRINT(("m_property==%d", m_property));
    DBG_PRINT(("*******m_invDet->m_spdw = %s m_spdw : %s ", m_invDet->m_spdw.c_str(), m_deptInfo->m_spdw.c_str()));
	
	//若有折扣
	INT64 nRebateSum;
	CInvDet *rebateDet;
    if(m_property==DETAIL_GOODS_DISCOUNT)
	{
		rebateDet = rebateDet->GetNewInvDet();//从静态数组获取一个可用的CInvDet对象
		DBG_ASSERT_EXIT((rebateDet != NULL), (" rebateDet == NULL!"));
		
		nRebateSum = moneySum - orgMoneySum;//折掉的金额
		rebateDet->m_kplx = m_invtype;
		
		rebateDet->m_spbm = deptInfo->m_spbm;
		rebateDet->m_spmc = "折扣";
		rebateDet->m_spsl= m_tmpAmount;//保存原始值
		m_invDet->m_dotNum = m_dotNum;
		rebateDet->m_spdj = m_tmpRate;
		rebateDet->m_spdw = deptInfo->m_spdw;	//商品单位
		
		rebateDet->m_spje = nRebateSum;
		rebateDet->m_spse =CountTax(rebateDet->m_spje,(UINT32)(deptInfo->m_spsl*SUM_EXTENSION));
		rebateDet->m_property = DETAIL_DISCOUNT;
		
		m_singleInvInfo->InsertNode(rebateDet);	//插入节点
		rebateDet = NULL;
		m_singleInvInfo->m_sphsl++;		//当前发票总商品行加1
		m_tmpGoodsNum++;				////当前总商品行加1
	}
	
	
    //判断剩余发票份数是否够用 
	
	//发票剩余份额不足(若有折扣，则删除两个节点)
	UINT32 nInvCount = CalculateInvNum();   //商品行所需发票数
	DBG_PRINT(("nInvCount==%d", nInvCount));

	m_nInvCount= nInvCount;
	DBG_PRINT(("m_nInvCount==%d", m_nInvCount));


	UINT32 nIfReturn = 0; //是否取消该商品行，停止开票
	DBG_PRINT((" g_globalArg->m_curInvVol->m_remain = %u !", g_globalArg->m_curInvVol->m_remain));
	
	if( nInvCount > g_globalArg->m_curInvVol->m_remain )
	{
		nIfReturn = 1;
			ret = SHORT_OF_INV; 
		DBG_PRINT((" g_globalArg->m_curInvVol->m_remain = %u !", g_globalArg->m_curInvVol->m_remain));
	}
	if ((nInvCount>1)&&(m_singleInvInfo->m_kplx==RETURN_INV)) 
	{
		nIfReturn = 1;
		ret = RETURN_GOODS_EXCEED;
	}
	if( 1==nIfReturn )
	{
		m_singleInvInfo->m_sphsl--;			//当前发票总商品行减1
		m_tmpGoodsNum--; ////当前总商品行减1
		m_singleInvInfo->DelLastNode();		//删除刚加入的节点
		if (m_property==DETAIL_GOODS_DISCOUNT) 
		{
			m_singleInvInfo->m_sphsl--;		//当前发票总商品行减1
			m_tmpGoodsNum--; ////当前总商品行减1
			m_singleInvInfo->DelLastNode();	//删除刚加入的节点
		}
		m_tmpMoneySum -= moneySum;			//本张累计金额减去本商品行金额
		InitSaleData(0);					//销售信息初始化
		DBG_RETURN(ret);
	}
	
    //释放空间,返回
	//当前发票累计金额增加
	m_singleInvInfo->m_kphjje += moneySum;
	DBG_PRINT((" m_singleInvInfo->m_kphjje = %u !", m_singleInvInfo->m_kphjje));
	
	
	//当前发票累计税额增加
	m_singleInvInfo->m_kpse += moneyTaxSum;
	DBG_PRINT((" m_singleInvInfo->m_kphjse = %u !", m_singleInvInfo->m_kpse));
    moneyTaxSum=0;
	
	InitSaleData(0);  //销售信息初始化
	
    ret = SUCCEED;
	DBG_PRINT(("退出Sale函数。成功。"));
	DBG_RETURN(ret);
	
}

//-------------------------------------------------
//初始化发票头中的成员变量
//-------------------------------------------------
UINT8 SaleData::InitInvHead()
{
	DBG_PRINT(("进入InitInvHead函数"));
	m_singleInvInfo->m_fpdm = g_globalArg->m_curInvVol->m_code;	//发票代码
	//	m_singleInvInfo->m_fphm = g_globalArg->m_curInvVol->m_ieno + 1 - g_globalArg->m_curInvVol->m_remain;
	m_singleInvInfo->m_fphm = g_globalArg->m_curInvVol->m_curInvNo;		/**< 发票号码 */
	
	
	m_singleInvInfo->m_kplx = NORMAL_INV;		//开票类型
	
	m_singleInvInfo->m_kphjje = 0;				//开票合计金额
	m_singleInvInfo->m_yfpdm = "";				//原发票号码
	
	m_singleInvInfo->m_fkdw = g_globalArg->m_defaultPayerName;  //付款单位
	m_singleInvInfo->m_fkdwsh = "";        //付款单位税务识别号
	m_singleInvInfo->m_sky = g_globalArg->m_operator->m_name;	//收款员
	m_singleInvInfo->m_skr = g_globalArg->m_operator->m_name;
	//	m_singleInvInfo->m_skyid = g_globalArg->m_operator->m_opid;	//收款员ID
	
	m_singleInvInfo->m_sphsl = 0;			//商品明细数量
	
	m_singleInvInfo->m_bzkz = "";
	m_singleInvInfo->m_backup1 = "";		//备用字段1
	//	m_singleInvInfo->m_backup2 = "";		//备用字段2
	//	m_singleInvInfo->m_backup3 = "";		//备用字段3
	
	m_singleInvInfo->m_scbz = 0;
	
    m_singleInvInfo->DelList();				//释放链表空间
	
	DBG_PRINT(("退出InitInvHead函数"));
	return SUCCESS;
}

UINT8 SaleData::InitSaleData(UINT8 set)
{	
	m_reductFlag = 0;							//加成标志		
	m_serviceFeeFlag = 0;						//服务费标志	
	
	m_tmpPrice = 0.0;							//临时价格		
	m_tmpSum = 0;								//临时金额			
	m_tmpAmount = 1.0;							//临时商品数量	
	m_tmpDept = 0;								//临时部类		
	m_tmpRate = 0.0;							//临时百分率
	m_tmpClient = "";                           //临时付款人
	
	m_saleMode = g_globalArg->m_saleMode;	    //销售模式，总价、临时价，数量	
	m_maxAllowCount = 6;					    //每张发票最大允许商品行数量	
	m_detailCount = 0;							//商品行总数量（多张发票）														
	m_goodsSum = 0;								//商品行累计金额			         
	
	m_property = DETAIL_GENERAL_GOODS;          //商品是否有折扣，加成等 
    m_cancel = 0;                               //是否取消商品
	m_realCancelNum = 0;
	
	m_dotNum = 0;                               //四舍五入后数量的小数位数
	m_saveAmount = 1.0;                         //四舍五入后的商品数量
	
	if (set == 1) 
	{
		m_tmpMoneySum = 0;                      //单张票的累计金额
		m_invtype = NORMAL_INV;                 //开票类型，正、废、红票
		m_oldInv = 0;                           //红票的发票号
		m_oldCodeNo = "";						//红票的发票代码
	}
	return SUCCESS;
}

//-----------------------------------------------------
//现金结帐
//-----------------------------------------------------
UINT8 SaleData::PayByCash( UINT8 &IfInvSum )
{
	DBG_PRINT(("进入PayByCash函数"));
	DBG_ENTER("SaleData::PayByCash");
	UINT8 ret;
	string strErr;
	IfInvSum = 0;

	//若无商品 
	if( m_singleInvInfo->m_sphsl == 0 )
	{
		ret = NO_GOODS;
		m_workState = WORK_COMPLETE;
		DBG_PRINT(("ret= %u",ret));
		DBG_RETURN(ret);
	}
	
	//判断装入纸质发票	
	if( isPaper() != 0 )
	{		
		ret = NO_PAPER;
		DBG_PRINT(("ret= %u",ret));
		DBG_RETURN(ret);
	}

	//发票金额超下限
	//	if (m_singleInvInfo->m_kphjje < g_globalArg->m_invKind->m_minSum) 
	//	{	
	//		ret = NM_EXCEED_MIN;
	//		DBG_PRINT((" g_globalArg->m_invKind->m_minSum = %u!", g_globalArg->m_invKind->m_minSum));
	//		DBG_PRINT((" m_singleInvInfo->m_kphjje = %ld!", m_singleInvInfo->m_kphjje));
	//		DBG_PRINT((" Warning: sigal invoice money exceed the min limit!"));
	//		DBG_RETURN(ret);	
	//	}
	
	
	//拆分发票并打印
	CInvHead smallInvHead;//存储拆分出的发票内容
	m_smallInvInfo = &smallInvHead; 
	DBG_ASSERT_EXIT((m_smallInvInfo != NULL), (" m_smallInvInfo == NULL!"));
	
    m_detailCount = m_singleInvInfo->m_sphsl;  //销售的商品总行数
	DBG_PRINT(("m_detailCount == %d", m_detailCount));
    m_pHead = m_singleInvInfo->pHead;
	
	g_YW_PowerOffData->ProcData.fptk_data.RemainLineCount = 0;
	DBG_PRINT(("*****工作模式******：%u", g_globalArg->pSaveTemplateInfo->workMode));
	while( !ScanGoodsLine(1) )
	{
		DBG_PRINT((" After ScanGoodsLine, begin printing the invoice"));
		
		//组装打印的发票头和明细
		ComposePrnInv();
		DBG_PRINT(("*****工作模式******：%u", g_globalArg->pSaveTemplateInfo->workMode));
		
		DBG_PRINT(("调用开票句柄"));
		if( (ret = MakeInvoiceHandle(1, INV_ROLL_PROC)) != SUCCESS )

		{
			m_tmpGoodsNum = 0;			//当前总商品行数清零
			InitSaleData(1);			//销售信息初始化
			InitInvHead();				//单张发票信息初始化，释放剩余发票的商品行节点
			m_smallInvInfo->DelList();  //释放本张发票的商品行节点
			DBG_PRINT(("m_smallInvInfo->DelList() 完毕"));
			m_workState = WORK_COMPLETE;
			m_smallInvInfo = NULL;
			DBG_RETURN(ret);			//如实返回错误类型			
		}
		
		m_smallInvInfo->DelList();  //释放已打印的商品行节点
		
	}
	
	//销售相关变量复位
    m_tmpGoodsNum = 0;  //当前总商品行数清零
	InitSaleData(1);	//销售信息初始化
	InitInvHead();		//单张发票信息初始化
	m_workState = WORK_COMPLETE;
	
	//判断，并进行自动汇总
	//if(g_globalArg->m_curInvVol->m_remain == 0)
	if (0==g_globalArg->m_curInvVol->m_curInvNo)
	{
		if (g_globalArg->m_operator->m_role==DEMO_ROLE)
		{
			m_smallInvInfo = NULL;
			DBG_PRINT((" g_globalArg->m_curInvVol->m_remain = 0!"));
			DBG_RETURN(MUST_RE_LOGIN);
		}
		else
		{
			// 			POWOFF_DISABLE(); //屏蔽掉电中断
			//  			if(!InvSum(IfInvSum))
			// 			{
			// 				m_smallInvInfo = NULL;
			// 				DBG_PRINT(("Get volume Sum error !"));
			// 				POWOFF_ENABLE(); //开掉电中断
			// 				DBG_RETURN(INV_SUM_ERROR);
			// 			}
			// 			POWOFF_ENABLE(); //开掉电中断
		}
		
	}
	
	m_smallInvInfo = NULL;
	ret = SUCCEED;
	
	DBG_PRINT(("退出PayByCash函数。成功。"));
	DBG_RETURN(ret);	
}


UINT8 SaleData::MakeInvoiceHandle(UINT8 nIfPrn, UINT8 nId)
{
	DBG_ENTER("SaleData::MakeInvoiceHandle");
	UINT32  errorcode;
	UINT32 preInvNo = 0;
	UINT8  nGetFiscalCode;
	UINT8  nIfRolled = 0;
	UINT8  errCodeJSK=0;
	
	UINT8 ret, TransRet;
	UINT32 nLIndex = 0;//连接失败计数器清零
	UINT32 nLmt = 3;	//重试次数
	UINT8 nLinkFlag = 0;//网络连通标志 0：连通，1：不通
	string strErr("");
	
	
	//	IssuedInvoice issuedInvoiceData;       /**< 执行开票命令发送的数据结构*/
	
	switch(nId)
	{
	case INV_ROLL_PROC:
		
		if( g_globalArg->m_operator->m_role != DEMO_ROLE ) //不是学习角色
		{
			POWOFF_DISABLE();					//屏蔽掉电中断
			SetPowerOffFlag();
			SetPowerOffProcID_M(INV_ROLL_PROC);	//设主过程ID		
			errorcode = RollData(nIfRolled);	//内含POWOFF_ENABLE();
			if(errorcode!= SUCCESS)
			{
				DBG_PRINT((" Roll() error !"));
				CleanPowerOffFlag();
				POWOFF_ENABLE();
				DBG_RETURN(errorcode);	
			}
			CleanPowerOffFlag();
			DBG_PRINT(("滚动结束"));
			
			//有容量测试时，滚动后要退出开票程序，以便核对滚动数据
			if( (1==AUTOISSUE) && (1==nIfRolled) )
			{
				DBG_PRINT((" Roll(): have rolled successfully!"));
				DBG_RETURN(HAVE_ROLLED); //若滚动过则退出程序					
			}
		}
		
#ifndef WIN32
#if POWEROF_SPEED_TEST
		fTime = tp.time * 1000 + tp.millitm;
		DBG_PRINT(("1 生成税控码，起始, Time = %llu ", fTime));
#endif
#endif
		
		//生成税控码
		case WRITE_ICCARD:
			if (g_globalArg->m_operator->m_role!=DEMO_ROLE) //不是学习角色
			{
				POWOFF_DISABLE(); //屏蔽掉电中断
				// 				SetPowerOffFlag();
				// 				SetPowerOffProcID_M(FPTK_NORMAL_PROC);//设主过程ID
				// 				SetPowerOffProcID_C(WRITE_INV); //设子过程ID
				DBG_PRINT(("进入掉电保护"));
				//				DBG_PRINT((" m_smallInvInfo->pHead = %x!", m_smallInvInfo->pHead));
				//				m_issuedInvoiceData = &issuedInvoiceData;
				//				if(m_issuedInvoiceData==NULL)
				//				{
				//					DBG_PRINT((" malloc(sizeof(IssuedInvoice) error !"));
				//					//					CleanPowerOffFlag();
				//					POWOFF_ENABLE();
				//					DBG_RETURN(MALLOC_MEM_ERROR);		
				//				}
				
				
				// 				DBG_PRINT((" m_smallInvInfo->pHead = %x!", m_smallInvInfo->pHead));
				//				if (ComposeIssuedInvData(m_smallInvInfo, m_issuedInvoiceData) == FAILURE)
				//				{
				//					m_workState = WORK_COMPLETE;
				//					//					CleanPowerOffFlag();
				//					POWOFF_ENABLE();
				//					DBG_RETURN(GET_FISCAL_CODE_ERROR);
				//				}				
				DBG_PRINT((" m_smallInvInfo->pHead = %x!", m_smallInvInfo->pHead));	
				
				//不是学习角色
				if( g_globalArg->m_operator->m_role != DEMO_ROLE ) 
				{
					//若允许离线，判断是否满足3个条件 

					ret = IsOffLineOk(m_smallInvInfo->m_kphjje, strErr);
					if (SUCCESS != ret) 
					{
						return ret;
					}										
				}
				
				DBG_PRINT(("m_smallInvInfo->m_kplx= %u",m_smallInvInfo->m_kplx));
				BAR_DEF();
				BAR_SHOW("获取税控码信息中...");
				
				if ("" == m_smallInvInfo->m_fkdwsh)
				{
				    m_smallInvInfo->m_fkdwsh=PAYER_NAME_DEF;
				}
				
				DBG_PRINT(("m_smallInvInfo->m_fkdwsh= %s",m_smallInvInfo->m_fkdwsh.c_str()));
				m_smallInvInfo->m_zskl =  g_globalArg->m_strZskl;
				DBG_PRINT(("m_smallInvInfo->m_zskl= %s",m_smallInvInfo->m_zskl.c_str()));

				errCodeJSK=SALE_MakeInvHand(m_smallInvInfo,g_globalArg->m_strMsg);					
					DBG_PRINT(("m_smallInvInfo->m_kprq= %u",m_smallInvInfo->m_kprq));
					DBG_PRINT(("m_smallInvInfo->m_kpsj= %u",m_smallInvInfo->m_kpsj));
					if (SUCCESS != errCodeJSK)
					{
					DBG_RETURN(INV_LIB_ERR);
					}

				
				//	m_issuedInvoiceData=NULL;
				DBG_PRINT((" m_smallInvInfo->pHead = 0x%x!", m_smallInvInfo->pHead));
				//	InvClassToStuct(&(g_YW_PowerOffData->ProcData.fptk_data.posInvInfo), m_smallInvInfo);
				POWOFF_ENABLE();//开掉电中断
				
			}
			
			
#ifndef WIN32
#if POWEROF_SPEED_TEST
			ftime(&tp);
			fTime2 = tp.time * 1000 + tp.millitm;
			DBG_PRINT((" ********fiscal code, Time = %llu ", (fTime2 - fTime)));
#endif
#endif
			
			//-----------------------------------------------------------------------------
			//写数据库
			//-----------------------------------------------------------------------------
		case WRITE_INV:
			
			//不是学习角色
			if( g_globalArg->m_operator->m_role != DEMO_ROLE ) 
			{
				POWOFF_DISABLE();				//屏蔽掉电中断
				DBG_PRINT(("填充发票打印结构体，写数据库"));
				SetPowerOffProcID_C(PRINT_INV); //设子过程ID
			}
			
			DBG_PRINT(("发票份数减1"));
			g_globalArg->m_curInvVol->m_curInvNo = 0;//当前票号复位
			if (g_globalArg->m_operator->m_role==DEMO_ROLE) 
			{
				g_globalArg->m_curInvVol->m_remain--;  //发票份数减1
				DBG_PRINT(("g_globalArg->m_curInvVol->m_remain == %d", g_globalArg->m_curInvVol->m_remain));
			}
			
			//不是学习角色
			if (g_globalArg->m_operator->m_role != DEMO_ROLE) 
			{
				
				// #ifndef WIN32
				// #if POWEROF_SPEED_TEST
				// 				ftime(&tp);
				// 				fTime = tp.time * 1000 + tp.millitm;
				// #endif
				// #endif				
				// 
				// 				//写数据库，更新INV_VOL表
				// 			//	DBG_PRINT(("g_globalArg->m_curInvVol->m_remain == %d", g_globalArg->m_curInvVol->m_remain));
				// 				if( UpdateVolRemain(g_globalArg->m_curInvVol->m_remain)== FAILURE)
				// 				{
				// 					DBG_PRINT((" UpdateVolRemain() error!"));
				// 				//	CleanPowerOffFlag();
				// 					POWOFF_ENABLE();
				// 					DBG_RETURN(UPDATE_INV_VOL_ERROR);			
				// 				}
				// 
				// #ifndef WIN32
				// #if POWEROF_SPEED_TEST
				// 				ftime(&tp);
				// 				fTime2 = tp.time * 1000 + tp.millitm;
				// 				DBG_PRINT((" ******INV_VOL, Time = %llu ", (fTime2 - fTime)));
				// #endif
				// #endif
				
				//	DBG_PRINT((" m_smallInvInfo->pHead = %x!", m_smallInvInfo->pHead));
				
				//更新INV_HEAD表、INV_DET表
				errorcode = m_smallInvInfo->Save();
//				errorcode=SUCCESS;
				if (errorcode != SUCCESS)
				{
					DBG_PRINT(("m_smallInvInfo->Save() wrong"));
					//	CleanPowerOffFlag();
					POWOFF_ENABLE();
					DBG_RETURN(DB_SAVE_ERROR);
				}
				(g_globalArg->m_invCount)++; //记录条数自增
				DBG_PRINT((" save SUCCESS"));
				
				
#ifndef WIN32
#if POWEROF_SPEED_TEST
				ftime(&tp);
				fTime = tp.time * 1000 + tp.millitm;
				DBG_PRINT((" *****INV_HEAD，INV_DET, Time = %llu ", (fTime-fTime2)));
#endif
#endif
				
				//更新RT_INV表
				if(( m_smallInvInfo->m_kplx == RETURN_INV )||(m_singleInvInfo->m_kplx==RET_MANUAL_INV)) 
				{
					DBG_PRINT(("更新RT_INV表"));
					CRtInv curRtInv;
					CRtInv *pRtInv = &curRtInv;
					if( m_oldCodeNo != "" )
					{
						pRtInv-> m_fpdm = m_oldCodeNo;			//用户输入的红票代码
						m_oldCodeNo = "";
					}
					else
					{
						pRtInv-> m_fpdm = m_smallInvInfo->m_fpdm;					
					}
					pRtInv-> m_fphm = m_smallInvInfo->m_yfphm;
					errorcode = pRtInv->AddNew();				//写已退发票信息表
					if( errorcode != SQLITE_OK )
					{
						DBG_PRINT((" pRtInv->AddNew() error!"));
						//		CleanPowerOffFlag();
						POWOFF_ENABLE();
						DBG_RETURN(DB_SAVE_ERROR);
					}
					(g_globalArg->m_rtInvCount)++; //记录条数自增
				}
				
				POWOFF_ENABLE();//开掉电中断
			}
			
#ifndef WIN32
#if POWEROF_SPEED_TEST
			ftime(&tp);
			fTime2 = tp.time * 1000 + tp.millitm;
			DBG_PRINT((" ****RT_INV, Time = %llu ", (fTime2- fTime)));
#endif
#endif
			
			//------------------------------------------------------------
			//填充发票打印结构体，打印
			//------------------------------------------------------------
		case PRINT_INV:  		
			DBG_PRINT(("填充发票打印结构体，打印"));
			if( nIfPrn == 1 )
			{
				//开辟打印数据结构体的内存
				m_pInvPrnData = &g_invPrnData;
				if(m_pInvPrnData==NULL)
				{
					DBG_PRINT((" malloc(sizeof(TPrnInvoiceInfo) error !"));
					//	POWOFF_DISABLE(); //屏蔽掉电中断
					//	CleanPowerOffFlag();
					//	POWOFF_ENABLE();
					DBG_RETURN(MALLOC_MEM_ERROR);		
				}
				//	DBG_PRINT(("*****工作模式******：%u", g_globalArg->pSaveTemplateInfo->workMode));
				memset((void *)m_pInvPrnData, 0, sizeof(TPrnInvoiceInfo));//打印结构体内容清零
				
				FillPrnInvHead(m_pInvPrnData, m_smallInvInfo);		
				FillPrnInvDetail(m_pInvPrnData, m_smallInvInfo);	
				FillPrnInvTail(m_pInvPrnData, m_smallInvInfo);
				
				
#ifndef WIN32
#if POWEROF_SPEED_TEST
				ftime(&tp);
				fTime2 = tp.time * 1000 + tp.millitm;
				DBG_PRINT((" ******* fill, Time = %llu ", (fTime2- fTime)));
#endif
#endif
				BAR_DEF();
				BAR_SHOW("发票信息打印中.....");
				
				//定长版，打印       	
				print_invoice_head(m_pInvPrnData);
				print_invoice_data_cycle(m_pInvPrnData, 0);						
				print_invoice_tail(m_pInvPrnData);
				m_pInvPrnData = NULL;
			}
			
		default:
			break;
			
	}
	
#ifndef WIN32
#if POWEROF_SPEED_TEST
	ftime(&tp);
	fTime = tp.time * 1000 + tp.millitm;
	DBG_PRINT((" ******print, Time = %llu ", (fTime-fTime2)));
#endif
#endif		
	
	if (g_globalArg->m_operator->m_role!=DEMO_ROLE) //不是学习角色
	{
		POWOFF_DISABLE(); //屏蔽掉电中断
		MakeInvLog(m_smallInvInfo->m_kplx, SUCCESS);//写系统日志
		//	CleanPowerOffFlag();
		POWOFF_ENABLE();
	}

	DBG_PRINT(("m_nInvCount= %u",m_nInvCount));
	if (m_nInvCount >1)
	{
		ret = SALE_GetCurInv(g_globalArg->m_curInvVol,g_globalArg->m_strMsg);
		DBG_PRINT(("ret= %u",ret))
		if (ret ==FAILURE)
		{
			g_globalArg->m_curInvVol->ResetVol();
			DBG_PRINT((",g_globalArg->m_strMsg= %s",g_globalArg->m_strMsg.c_str()));
		} 	
	}
	
	DBG_PRINT(("退出MakeInvoiceHandle函数"));
	DBG_RETURN(SUCCESS);	
}

//比较两个浮点数是否相等
UINT8 SaleData::doublecmp(double d1, double d2)
{
    if((d1>=d2-MINIMUM_PRECISION)&&(d1<=d2+MINIMUM_PRECISION))
	{
		return 1;
	}
	return 0;
}

//------------------------------------------------------------------
//函数功能：扫描商品行
//------------------------------------------------------------------
UINT8 SaleData::ScanGoodsLine(UINT8 flagMakeInv)
{
	DBG_PRINT(("进入ScanGoodsLine函数"));
	DBG_ENTER("SaleData::ScanGoodsLine");
	
	INT64  invSum = 0;
	INT64  invTaxSum=0;
	UINT32 namelength = 0;	
	UINT32 tempdetailCount = 0;	//统计本张打印发票的明细行。xsr：用来记录已经使用了当前这张发票中的几行
	UINT32 GoodLineNum = 0;		//本张打印发票的商品行，链表中的节点数
	
	DBG_PRINT(("此时的m_detailCount == %u",m_detailCount));
	DBG_PRINT(("此时的flagMakeInv == %u",flagMakeInv));
	DBG_PRINT(("m_pHead = 0x%x",m_pHead));
    
	if( flagMakeInv==1 )
	{
		GoodLineNum = 0;
		m_smallInvInfo->pHead = m_pHead;
	}
	
	CInvDet *p = m_pHead;
	
	//
	while( GoodLineNum < m_detailCount )
	{
		DBG_ASSERT_EXIT((p != NULL), (" 发票明细链表头指针 m_pHead == NULL!"));
		namelength = p->m_spmc.length();
		
		//本张打印发票无空行了
		if (tempdetailCount >= g_globalArg->pSaveTemplateInfo->MaxGoodsCount)	//xsr 暂时注掉
			//if (tempdetailCount >= 6)
		{			
			m_detailCount -= GoodLineNum;			//修改商品行数			
			m_pHead = p;							//头指针指向下次打印的起始节点
			
			if (flagMakeInv == 1)
			{
				m_singleInvInfo->pHead = m_pHead;		//m_singleInvInfo中链表包含剩余明细节点
				m_smallInvInfo->m_kphjje = invSum;
				m_smallInvInfo->m_kpse =invTaxSum;
				m_smallInvInfo->m_sphsl = GoodLineNum;	//待打印的单张发票商品行数目赋值 
			}
			DBG_RETURN(FAILURE);
		}
		//本张打印发票还剩一个空行
		else if (tempdetailCount == g_globalArg->pSaveTemplateInfo->MaxGoodsCount - 1)
		{
			//DBG_PRINT(("该张发票还剩一个空行"));
			//如果此时需要两行：商品行存在折扣，
			//                  商品行不存在折扣，商品行超长
			if ((p->m_property==DETAIL_GOODS_DISCOUNT)||(p->m_property==DETAIL_GOODS_REDUCT)
				|| ((p->m_property!=DETAIL_GOODS_DISCOUNT) && (namelength > g_globalArg->pSaveTemplateInfo->MaxSPMCCharacter))
				|| ((p->m_property!=DETAIL_GOODS_REDUCT) && (namelength > g_globalArg->pSaveTemplateInfo->MaxSPMCCharacter)))
			{
				//修改商品行数
				m_detailCount -= GoodLineNum;
				//头指针指向下次打印的起始节点
				m_pHead = p;				
				
				if (flagMakeInv == 1)
				{
					m_singleInvInfo->pHead = m_pHead;//m_singleInvInfo中链表包含剩余明细节点
					m_smallInvInfo->m_kphjje = invSum;
					m_smallInvInfo->m_kpse =invTaxSum;
					m_smallInvInfo->m_sphsl = GoodLineNum; 
				}				
				DBG_RETURN(FAILURE);
			}
		}
		//本张打印发票还剩两个空行
		else if (tempdetailCount == g_globalArg->pSaveTemplateInfo->MaxGoodsCount - 2)
		{
			//商品行超长并存在折扣
			//DBG_PRINT(("该张发票还剩两个空行"));
			if (((p->m_property==DETAIL_GOODS_DISCOUNT) && (namelength > g_globalArg->pSaveTemplateInfo->MaxSPMCCharacter))
				||((p->m_property==DETAIL_GOODS_REDUCT) && (namelength > g_globalArg->pSaveTemplateInfo->MaxSPMCCharacter)))
			{
				
				//修改商品行数
				m_detailCount -= GoodLineNum;
				//头指针指向下次打印的起始节点
				m_pHead = p;
				
				if (flagMakeInv == 1)
				{
					m_singleInvInfo->pHead = m_pHead;//m_singleInvInfo中链表包含剩余明细节点
					m_smallInvInfo->m_kphjje = invSum;
					m_smallInvInfo->m_kpse =invTaxSum;
					m_smallInvInfo->m_sphsl = GoodLineNum; 
				}					
				DBG_RETURN(FAILURE);
			}
		}
		
		//对于非折扣行，计算金额，判断单张金额是否超限
		INT64 realSum = 0;
		INT64 realTaxSum=0;
		CInvDet *s = NULL;
		
		//如果既不是折扣行也不是折让行
		if ((p->m_property != DETAIL_DISCOUNT)&&(p->m_property != DETAIL_REDUCTION_TEMPLATE))
		{
			realSum = p->m_spje ;
			realTaxSum =p->m_spse;
			
			if(p->m_property == DETAIL_GOODS_DISCOUNT) //有折扣
			{
				s = p->pNext;
				DBG_PRINT(("折扣率为%u",s->m_spdj));
				realSum = p->m_spje + s->m_spje; //折扣金额
				DBG_PRINT(("折后金额为%d",realSum));
				
				realTaxSum = p->m_spse + s->m_spse; //折扣后税额
				DBG_PRINT(("折让后税额为%d",realTaxSum));
			}
			else if(p->m_property == DETAIL_GOODS_REDUCT) //有折让
			{
				s = p->pNext;
				realSum = p->m_spje + s->m_spje; //折让后金额
				DBG_PRINT(("折让后金额为%d",realSum));
				
				realTaxSum = p->m_spse + s->m_spse; //折让后税额
				DBG_PRINT(("折让后税额为%d",realTaxSum));
			}
			
			//如果超单张限额
			if ( (invSum+realSum) > g_globalArg->m_invKind->m_maxSign)
			{
				DBG_PRINT(("此时商品行累加金额=====%d",invSum));
				DBG_PRINT(("本商品行金额=====%d",realSum));
				DBG_PRINT(("g_globalArg->m_invKind->m_maxSign=%u",g_globalArg->m_invKind->m_maxSign));
				
				m_detailCount -= GoodLineNum;
				//头指针指向下次打印的起始节点
				m_pHead = p;
				
				//修改商品行数
				if (flagMakeInv == 1)
				{
					m_singleInvInfo->pHead = m_pHead;//m_singleInvInfo中链表包含剩余明细节点
					m_smallInvInfo->m_kphjje = invSum;
					m_smallInvInfo->m_kpse =invTaxSum;
					m_smallInvInfo->m_sphsl = GoodLineNum; 
				}
				DBG_RETURN(FAILURE);
			}
		}
		
		
		//该节点可加入本张发票
		tempdetailCount++;
		invSum += realSum;	
		invTaxSum += realTaxSum;
		//商品行名称超长,数目加1
		if (namelength > g_globalArg->pSaveTemplateInfo->MaxSPMCCharacter)
		{
			tempdetailCount++;
		}
		
		//本张打印发票商品行加1
		GoodLineNum++;
		p = p->pNext;
		continue;
	}
	
	DBG_PRINT(("invSum == %u", invSum));
	//如果
	if( invSum > 0 )
	{		
		m_detailCount -= GoodLineNum;		//修改总商品行数		
		m_pHead = p;						//头指针指向下次打印的起始节点
		
		if( flagMakeInv == 1 )
		{
			m_singleInvInfo->pHead = m_pHead;			//m_singleInvInfo中链表包含剩余明细节点
			m_smallInvInfo->m_sphsl = GoodLineNum;		//商品行数量
            m_smallInvInfo->m_kphjje = invSum;			//开票加税合计金额
			m_smallInvInfo->m_kpse = invTaxSum;			//开票合计税额金额
			m_singleInvInfo->pHead = NULL;				//m_singleInvInfo商品行节点全部给出
			m_singleInvInfo->pEnd = NULL; 
			m_pHead = NULL;
			DBG_PRINT(("m_smallInvInfo->m_sphsl = %u",m_smallInvInfo->m_sphsl));
			DBG_PRINT(("SaleData->m_detailCount = %u",m_detailCount));
		}
		DBG_RETURN(FAILURE);
	}
	
	DBG_PRINT(("退出ScanGoodsLine函数"));
    DBG_RETURN(SUCCESS);
}

//-------------------------------------------------------
//组装打印的发票头和明细
//xsr 修改过
//-------------------------------------------------------
UINT8 SaleData::ComposePrnInv()
{
	DBG_PRINT(("进入ComposePrnInv函数"));
	DBG_ENTER("SaleData::ComposePrnInv");
	INT8 value[128];
	memset((void*)value, 0, sizeof(value));
	
	m_smallInvInfo->m_fpdm = g_globalArg->m_curInvVol->m_code;												//发票代码
	//m_smallInvInfo->m_fphm = g_globalArg->m_curInvVol->m_ieno + 1 - g_globalArg->m_curInvVol->m_remain;		//发票号码 
	m_smallInvInfo->m_fphm =g_globalArg->m_curInvVol->m_curInvNo;
	m_smallInvInfo->m_kprq = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);	//开票日期
	m_smallInvInfo->m_kpsj = TDateTime::CurrentDateTime().FormatInt(HHMMSS);	//开票时间
	
	m_smallInvInfo->m_kplx = m_singleInvInfo->m_kplx;		//开票类型
	
	m_smallInvInfo->m_yfpdm = m_singleInvInfo->m_yfpdm;	
	m_smallInvInfo->m_yfphm = m_singleInvInfo->m_yfphm;		//原发票号码	
	m_smallInvInfo->m_fkdw = m_singleInvInfo->m_fkdw;		//付款单位
	m_smallInvInfo->m_fkdwsh = m_singleInvInfo->m_fkdwsh;   //购方税号
	m_smallInvInfo->m_sky = m_singleInvInfo->m_sky;			//收款员
	//	m_smallInvInfo->m_skyid = m_singleInvInfo->m_skyid;		//收款员ID
	//	m_smallInvInfo->m_ptype = m_singleInvInfo->m_ptype;		//付款方式
	m_smallInvInfo->m_scbz = m_singleInvInfo->m_scbz;
	
	
	if(RET_MANUAL_INV == m_smallInvInfo->m_kplx)
	{
		sprintf(value, "对应正数发票代码:%s号码:%08lu",
			m_smallInvInfo->m_yfpdm.c_str(), m_smallInvInfo->m_yfphm);	
		m_smallInvInfo->m_backup1 = value;	
	}
    else
	{
		m_smallInvInfo->m_backup1 = m_singleInvInfo->m_backup1;		//备用字段1
	}
	
	//	m_smallInvInfo->m_backup2 = m_singleInvInfo->m_backup2;		//备用字段2 
	//	m_smallInvInfo->m_backup3 = m_singleInvInfo->m_backup3;		//备用字段3
    
	CInvDet *p = m_smallInvInfo->pHead;
	CInvDet *pe;
	
    //若为空链表
	if (p == NULL)
	{
		DBG_PRINT((" m_smallInvInfo->pHead = NULL !"));
		DBG_RETURN(FAILURE);		
	}    
    //若为非空链表，赋值
	DBG_PRINT(("商品行数量m_smallInvInfo->m_sphsl == %d", m_smallInvInfo->m_sphsl));
	for( INT32 i=0; i<m_smallInvInfo->m_sphsl; i++, p = p->pNext )
	{
		p->m_fpdm = g_globalArg->m_curInvVol->m_code; 
        //p->m_fphm = g_globalArg->m_curInvVol->m_ieno + 1 - g_globalArg->m_curInvVol->m_remain;
        p->m_fphm =g_globalArg->m_curInvVol->m_curInvNo;
		p->m_kplx = m_smallInvInfo->m_kplx;
		p->m_sky =g_globalArg->m_operator->m_name;
		p->m_sphxh = (i+1);
		//p->m_sphxh = i;
		p->m_kprq = m_smallInvInfo->m_kprq; 
        p->m_kpsj = m_smallInvInfo->m_kpsj;
		
		DBG_PRINT(("发票代码  p->m_fpdm == %s",p->m_fpdm.c_str()));
		DBG_PRINT(("发票号码  p->m_fphm == %d",p->m_fphm));
		DBG_PRINT(("商品行序号p->m_sphxh == %d",p->m_sphxh));
		pe = p;
	}
	pe->pNext = NULL;
    m_smallInvInfo->pEnd = pe;
	
	DBG_PRINT(("退出ComposePrnInv函数"));
	DBG_RETURN(SUCCESS);
}

//-------------------------------------------------------------------
//函数功能：计算当前商品行所需发票数目
//-------------------------------------------------------------------
UINT32 SaleData::CalculateInvNum()
{
	DBG_ENTER("SaleData::CalculateInvNum");
	DBG_PRINT(("进入CalculateInvNum函数"));
	
	UINT32 invoiceNum = 0;
	DBG_PRINT(("m_singleInvInfo->m_sphsl==%d", m_singleInvInfo->m_sphsl));
	m_detailCount = m_singleInvInfo->m_sphsl;		//销售的商品总行数
	DBG_PRINT(("m_detailCount==%d", m_detailCount));
	
	m_pHead = m_singleInvInfo->pHead;
	while( ScanGoodsLine(0)==FAILURE )			//扫描商品行，参数0代表不开票。
	{
		invoiceNum++;
	}
	DBG_PRINT(("实际需要发票张数 = invoiceNum = %u", invoiceNum));
	DBG_PRINT(("退出CalculateInvNum函数"));
	DBG_RETURN(invoiceNum);
}

//------------------------------------------------------------
//更新发票领用信息表中剩余发票份数
//------------------------------------------------------------
UINT8 SaleData::UpdateVolRemain(UINT32 remain)
{
	DBG_PRINT(("进入UpdateVolRemain函数"));
	DBG_PRINT(("remain == %d", remain));
	DBG_PRINT(("g_globalArg->m_curInvVol->m_code == %s", g_globalArg->m_curInvVol->m_code.c_str()));
	DBG_PRINT(("g_globalArg->m_curInvVol->m_isno == %d", g_globalArg->m_curInvVol->m_isno));
	DBG_ENTER("SaleData::UpdateVolRamain");
	
	char value[256];
	memset((void*)value, 0, sizeof(value));
	string codeNo = g_globalArg->m_curInvVol->m_code;
	UINT32 isNo = g_globalArg->m_curInvVol->m_isno;
    string strSql;
	
	DBG_PRINT(("codeNo == %s", codeNo.c_str()));
	DBG_PRINT(("isNo == %d", isNo));
	
    sprintf(value, "UPDATE INV_VOL SET REMAIN = %u ", remain);	
    strSql = "";
	strSql.append(value);
    
	sprintf(value, "WHERE CODE = '%s' AND IS_NO = %u", codeNo.c_str(), isNo);
	g_globalArg->m_curInvVol->m_filter = "";
	g_globalArg->m_curInvVol->m_filter.append(value);
	
    strSql.append(g_globalArg->m_curInvVol->m_filter);
    g_globalArg->m_curInvVol->SetSQL(strSql);
    DBG_PRINT(("修改发票剩余份数 strSql = %s", strSql.c_str()));
	
	if (g_globalArg->m_curInvVol->ExecuteSQL()!= SQLITE_OK)
	{
		DBG_RETURN(FAILURE);
	}
	
	DBG_PRINT(("退出UpdateVolRemain函数"));
	DBG_RETURN(SUCCESS);
}

//-------------------------------------------------------------------------------------
//用之前，pIssuedInvData需要new出来
//组装为获得税控码所发送的数据结构
//-------------------------------------------------------------------------------------
/*
UINT8 SaleData::ComposeIssuedInvData( CInvHead *smallInv, IssuedInvoice *pIssuedInvData )
{
DBG_PRINT(("进入ComposeIssuedInvData函数"));
DBG_ENTER("SaleData::ComposeIssuedInvData");

 UINT8 i;
	UINT8 j;
	
	 DBG_ASSERT_EXIT((smallInv != NULL)&&(pIssuedInvData != NULL), (" m_con == NULL or pIssuedInvData = NULL "));
	 
	  memset((void *)pIssuedInvData, 0, sizeof(IssuedInvoice));
	  
	   smallInv->m_kprq = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);
	   smallInv->m_kpsj = TDateTime::CurrentDateTime().FormatInt(HHMMSS);
	   CInvDet *p = smallInv->pHead;
	   for(; p != NULL; p = p->pNext)
	   {
	   p->m_kprq = smallInv->m_kprq; 
	   p->m_kpsj = smallInv->m_kpsj;
	   }
	   pIssuedInvData->invoice_date = smallInv->m_kprq;
	   DBG_PRINT(("m_smallInvInfo->m_kprq =  %u", smallInv->m_kprq));
	   DBG_PRINT(("m_smallInvInfo->m_kpsj =  %u", smallInv->m_kpsj));
	   DBG_PRINT(("pIssuedInvData->invoice_date =  %u", pIssuedInvData->invoice_date));
	   
		
		 pIssuedInvData->invoice_no = smallInv->m_fphm;
		 pIssuedInvData->invoice_type = smallInv->m_kplx;
		 
		  p = smallInv->pHead;
		  //  DBG_ASSERT_EXIT((p != NULL), (" smallInv->pHead = NULL "));
		  
		   //开票总金额
		   pIssuedInvData->sum_money = smallInv->m_kphjje;		//xsr 类型不同
		   DBG_PRINT(("pIssuedInvData->sum_money = %d",pIssuedInvData->sum_money));
		   
			DBG_PRINT(("退出ComposeIssuedInvData函数"));
			DBG_RETURN(SUCCESS);
			}
*/
//统计一张发票某税种税目的开票总金额
//INT64 SaleData::GetTaxIDMoneySUM(CInvDet * invDet, UINT32 CodeNo, UINT32 InvNo, UINT8 TaxID)
//{
// 	DBG_ENTER("SaleData::GetTaxIDMoneySUM");
//     DBG_ASSERT_EXIT((invDet != NULL), (" invDet == NULL!"));
// 
// 	char value[256];
// 	memset((void*)value, 0, sizeof(value));
// 	string strFilter = "";
//     
// 	strFilter = "SELECT SUM(M_SUM) FROM INV_DET";
// 	sprintf(value, "WHERE CODE_NO = %u AND INV_NO = %u AND TAX_ID = %u",CodeNo, InvNo, TaxID);
// 	strFilter.append(value);
// 	invDet->SetSQL(strFilter);
// 
//     INT64 nSum;
//     invDet->GetOneResult(DB_INT64, &nSum);
// 	DBG_PRINT(("nSum =  %d", nSum));
//     DBG_RETURN(nSum);
//}

//打折扣
UINT8 SaleData::Discount(double fRate)
{
	DBG_ENTER("SaleData::Discount");
	
	if ((fRate<1)||(fRate>=100))
	{
		DBG_RETURN(REBATE_ERROR);
	}
	
	m_property = DETAIL_GOODS_DISCOUNT;
	m_tmpRate = fRate;
	DBG_PRINT(("m_tmpRate =  %f", m_tmpRate));
	
	DBG_RETURN(SUCCESS);
	
}

//生成网票防伪码
// UINT8 SaleData::GetFiscalCode(IssuedInvoice *pIssuedInvData, string fpdm, UINT8 *fiscal_code)
// {
// 	DBG_ENTER("SaleData::GetFiscalCode");
// 	DBG_ASSERT_EXIT((pIssuedInvData != NULL), ("pIssuedInvData = NULL "));
// 	DBG_ASSERT_EXIT((fiscal_code != NULL), ("fiscal_code = NULL "));
// 
// 	UINT8 pdata[128];
// 	UINT8 poutdata[128];
// 	UINT8 skm[32];
// 	UINT8 tmpBuf[32];
// 	INT8 chValue[21] = "00000000000000000000";
// 	UINT32 outlen=0;
// 	int sqmlen=1024*8;
// 	int len = 0;
// 	memset(pdata,0,sizeof(pdata));
// 	memset(poutdata,0,sizeof(poutdata));
// 
// 	DBG_PRINT(("invoice_date=%d", pIssuedInvData->invoice_date));
// 	sprintf((INT8 *)pdata, "%08d", pIssuedInvData->invoice_date);	//开票日期
// 
// 	DBG_PRINT(("invoice_no=%d", pIssuedInvData->invoice_no));
// 	sprintf((INT8 *)(pdata+8), "%010d", pIssuedInvData->invoice_no); //发票号码 10个数字
// 
// 	len =  fpdm.length();
// 	sprintf((chValue+20-len), "%s", fpdm.c_str());
// 	sprintf((INT8 *)(pdata+8+10), "%s", chValue); //发票代码 20位
// 	DBG_PRINT(("pdata: %s", pdata));
// 
// 	DBG_PRINT((" m_smallInvInfo->pHead = %x!", m_smallInvInfo->pHead));
// 	if(!param_encrypt(1,pdata,poutdata,&outlen))
// 	{
// 		DBG_PRINT(("param_encrypt ERR !"));
// 		DBG_RETURN(FAILURE);
// 	}
// 	memset(skm,0,sizeof(skm));
// 
// 	sqmlen=strlen(g_globalArg->m_sq_wm);
// 	DBG_PRINT(("sqmlen = %d", sqmlen));
// 	if(!init_sqm(g_globalArg->m_sq_wm, sqmlen))
// 	{
// 		DBG_PRINT(("init_sqm ERR !"));
// 		DBG_RETURN(FAILURE);
// 	}
// 
// 	if(!get_wlkp_skm(1, (UINT8 *)g_globalArg->m_corpInfo->m_Nsrsbh.c_str(), 
// 						g_globalArg->m_corpInfo->m_Nsrsbh.length(), 
// 						poutdata, outlen, pIssuedInvData->sum_money, skm))
// 	{
// 		DBG_PRINT(("get_wlkp_skm ERR !"));
// 		DBG_RETURN(FAILURE);
// 	}
// 	else
// 	{
// 		DBG_PRINT(("skm=%s", skm));
// 	}	
// 	DBG_PRINT((" m_smallInvInfo->pHead = %x!", m_smallInvInfo->pHead));
// 	strcpy((INT8 *)fiscal_code, (const INT8 *)skm);
// 
// 	DBG_PRINT((" m_smallInvInfo->pHead = %x!", m_smallInvInfo->pHead));
// 	DBG_RETURN(SUCCESS);
// }

//生成税控码
UINT8 SaleData::GetFiscalCode(CInvHead *pInv,UINT8 *fiscalCode, UINT32 &nRetCode, string &strErr)
{
	DBG_ENTER("SaleData::GetFiscalCode");
	DBG_ASSERT_EXIT((pInv != NULL), ("pInv = NULL "));
	
	
	memset((void*)fiscalCode, 0, sizeof(fiscalCode));
	// 	nRetCode = CJSKInfoFunc::TaxCardInvHeadInfo(pInv,fiscalCode, strErr);
	// 	if (JSK_SUCCESS != nRetCode)//取税控码失败
	// 	{
	// 		DBG_PRINT(("GetFiscalCode ERR: %s !", strErr.c_str()));
	// 		DBG_RETURN(FAILURE);
	// 	}
	
	DBG_PRINT((" fiscalCode = %s!", (INT8*)fiscalCode));
	DBG_RETURN(SUCCESS);
}

UINT8 SaleData::PriceInput(double dPrice)
{
	DBG_ENTER("SaleData::PriceInput");
	DBG_PRINT(("进入SaleData::PriceInput函数"));
	if( dPrice < BASE_PRICE )
	{
		DBG_RETURN(PRICE_NOT_ENOUGH);
	}
    m_tmpPrice = dPrice;
	DBG_PRINT(("m_tmpPrice = %f", m_tmpPrice));
	DBG_PRINT(("退出SaleData::PriceInput函数"));
	DBG_RETURN(SUCCESS);
}

//------------------------------------------------------------------
//函数名称：总价输入
//函数功能：把用户输入的数字存到m_tmpSum中。
//------------------------------------------------------------------
UINT8 SaleData::SumInput(double dSum)
{
	DBG_ENTER("SaleData::SumInput");
	INT64 nSum = 0;
	nSum = double2int(dSum * SUM_EXTENSION*10.0);//原始总金额
	nSum = double2int(nSum / 10.0);
	//总价不足
	if (0 == nSum) 
	{
		DBG_RETURN(MONEY_ZERO);
	}
	if (nSum<=0)
	{
		DBG_RETURN(MONEY_NOT_ENOUGH);
	}
    m_tmpSum = nSum;
	DBG_PRINT(("m_tmpSum = %d", m_tmpSum));
	DBG_RETURN(SUCCESS);
}

UINT8 SaleData::ClientNameInput(const char *content)
{
	DBG_ENTER("SaleData::ClientNameInput");
	UINT8 len;
	
	//名称超长
	len = strlen(content);
    DBG_PRINT(("len = %u", len));
	if (len > PAYER_NAME_LEN) 
	{
		DBG_RETURN(EXCEED_NAME_LEN);	
	}
	else if(len == 0)
	{
		DBG_PRINT(("len = %u", len));
		DBG_RETURN(NO_NAME);
	}
	DBG_PRINT(("content = %s", content));
	
	m_singleInvInfo->m_fkdw = content;
	
	DBG_PRINT(("m_singleInvInfo->m_fkdw = %s", m_singleInvInfo->m_fkdw.c_str()));
	DBG_RETURN(SUCCESS);
}

UINT8 SaleData::ClientNameInput(const char *chfkdw, const char *chfkdwsh)
{
	DBG_ENTER("SaleData::ClientNameInput");
	UINT8 len=0;
	UINT8 shlen=0;
	//名称超长
	len = strlen(chfkdw);
    DBG_PRINT(("len = %u", len));
	if (len > PAYER_NAME_LEN) 
	{
		DBG_RETURN(EXCEED_NAME_LEN);	
	}
	else if(len == 0)
	{
		DBG_PRINT(("len = %u", len));
		DBG_RETURN(NO_NAME);
	}
	DBG_PRINT(("chfkdw = %s", chfkdw));
	
	m_singleInvInfo->m_fkdw = chfkdw;
	
	DBG_PRINT(("m_singleInvInfo->m_fkdw = %s", m_singleInvInfo->m_fkdw.c_str()));

	m_singleInvInfo->m_fkdwsh = chfkdwsh;
	DBG_PRINT(("m_singleInvInfo->m_fkdwsh = %s", m_singleInvInfo->m_fkdwsh.c_str()));

	DBG_RETURN(SUCCESS);
}

UINT8 SaleData::ClientCodeInput(const char *content, string &strErr)
{
	DBG_ENTER("SaleData::ClientCodeInput");
	UINT8 len;
	
	//名称超长
	len = strlen(content);
    DBG_PRINT(("len = %u", len));
	if (len > TAX_CODE_LEN) 
	{
		strErr = "付款方税号过长";
		DBG_RETURN(FAILURE);
	}
	else if(len == 0)
	{				
		strErr = "未输入付款方税号";
		DBG_RETURN(FAILURE);
	}
	DBG_PRINT(("content = %s", content));
	
	m_singleInvInfo->m_fkdwsh = content;
	
	DBG_PRINT(("m_singleInvInfo->m_fkdwsh = %s", m_singleInvInfo->m_fkdwsh.c_str()));
	DBG_RETURN(SUCCESS);
}

UINT8 SaleData::RemarksInput(const char *content, string &strErr)
{
	DBG_ENTER("SaleData::RemarksInput");
	UINT8 len;
	
	//名称超长
	len = strlen(content);
    DBG_PRINT(("len = %u", len));
	if (len > SALE_REMARKS_LEN) 
	{
		strErr = "备注内容过长";
		DBG_RETURN(FAILURE);
	}
	else if(len == 0)
	{				
		strErr = "未输入备注内容";
		DBG_RETURN(FAILURE);
	}
	DBG_PRINT(("content = %s", content));
	
	m_singleInvInfo->m_backup1 = content;
	
	DBG_PRINT(("m_singleInvInfo->m_backup1 = %s", m_singleInvInfo->m_backup1.c_str()));
	DBG_RETURN(SUCCESS);
}

UINT8 SaleData::CancelGoods(void)
{
	DBG_ENTER("SaleData::CancelGoods");
	m_cancel = CANCEL_PRODUCT;
	DBG_PRINT(("CancelGoods"));
	DBG_RETURN(SUCCESS);
}

UINT8 SaleData::NoCancelGoods(void)
{
	DBG_ENTER("SaleData::NoCancelGoods");
	m_cancel = 0;
	DBG_PRINT(("NoCancelGoods"));
	DBG_RETURN(SUCCESS);
}

UINT8 SaleData::CancelGoodsByPLU(string &strCode, double nCancelNum, double &dPrice)
{
	DBG_ENTER("SaleData::CancelGoods");
	CInvDet *invDet, *nextInvDet;
	INT64 moneySum = 0;
	INT64 orgMoneySum = 0;
	m_realCancelNum = 0; //实际删除的商品数量
	UINT32 nEnterTime = 0;//进入while循环的次数
	
	DBG_PRINT(("CancelGoodsByPLU 11"));
	DBG_PRINT(("strCode = %s", strCode.c_str()));
    DBG_PRINT(("nCancelNum = %f", nCancelNum));
	
	//用户设置了取消商品的数量，则严格按数量取消该商品
	while (1)
	{
		nEnterTime++;
		DBG_PRINT(("CancelGoodsByPLU nEnterTime = %u", nEnterTime));
		invDet = m_singleInvInfo->SearchNode(strCode);
		if(invDet==NULL) //若没找到该节点商品
		{
			if (1==nEnterTime) 
			{
				DBG_PRINT(("CancelGoodsByPLU 2"));
				DBG_RETURN(NO_SUCH_GOODS);//不存在该商品
			}
			break;
		}		
		nextInvDet = invDet->pNext;
		
		DBG_PRINT(("CancelGoodsByPLU 2"));
		
		m_realCancelNum += invDet->m_spsl;				//实际删除的商品数量增加
		dPrice = invDet->m_spdj;						//保存被删除商品的价格，以供屏幕显示
		m_tmpGoodsNum--;								//显示的当前商品行减1
		m_singleInvInfo->m_sphsl--;						//发票中商品行减1
		m_singleInvInfo->m_kphjje -= invDet->m_spje;	//开票总金额减少
        m_singleInvInfo->m_kpse -= invDet->m_spse;	    //开票税额减少
		m_tmpMoneySum -= invDet->m_spje;				//临时单张金额减少
		
		if (invDet->m_property == DETAIL_GOODS_DISCOUNT)		//有折扣
		{
			m_singleInvInfo->m_sphsl--;							//发票中商品行减1
			m_tmpGoodsNum--;									//显示的当前商品行减1
			m_singleInvInfo->m_kphjje -= nextInvDet->m_spje;	//开票总金额减少
			m_singleInvInfo->m_kpse -= nextInvDet->m_spse;	//开票税额减少
			m_tmpMoneySum -= nextInvDet->m_spje;				//临时单张金额减少
			m_singleInvInfo->DelNode(strCode);				
		}
		else if (invDet->m_property == DETAIL_GOODS_REDUCT)		//有折让
		{
			m_singleInvInfo->m_sphsl--;					//发票中商品行减1
			m_tmpGoodsNum--;									//显示的当前商品行减1
			m_singleInvInfo->m_kphjje -= nextInvDet->m_spje;	//开票总金额减少
			m_singleInvInfo->m_kpse -= nextInvDet->m_spse;	//开票税额减少
			m_tmpMoneySum -= nextInvDet->m_spje;				//临时单张金额减少
			m_singleInvInfo->DelNode(strCode);				
		}
		
		m_singleInvInfo->DelNode(strCode);
	}
	
	DBG_PRINT(("CancelGoodsByPLU over"));
	DBG_RETURN(SUCCESS);	
}

UINT8 SaleData::CancelGoodsByDept(string dpNo, double nCancelNum, double &dPrice)
{
	DBG_ENTER("SaleData::CancelGoods");
	CInvDet *invDet, *nextInvDet;
	INT64 moneySum = 0;
	INT64 orgMoneySum = 0;
	m_realCancelNum = 0;	//实际删除的商品数量
	UINT32 nEnterTime = 0;	//进入while循环的次数
	
	DBG_PRINT(("dpNo == %s", dpNo.c_str()));
    DBG_PRINT(("nCancelNum == %d", nCancelNum));
	
	//用户设置了取消商品的数量，则严格按数量取消该商品
	while (1)
	{
		nEnterTime++;
		DBG_PRINT(("CancelGoodsByDept nEnterTime = %u", nEnterTime));
		invDet = m_singleInvInfo->SearchNode(dpNo);
		
		if( invDet==NULL ) //若不存在该商品
		{
			if( 1==nEnterTime ) 
			{
				DBG_PRINT(("CancelGoodsByDept "));
				DBG_RETURN(NO_SUCH_GOODS);//不存在该商品
			}
			break;
		}
		nextInvDet = invDet->pNext;
		
		DBG_PRINT(("CancelGoodsByDept 2"));
		
		m_realCancelNum += invDet->m_spsl;				//实际删除的商品数量增加
		dPrice = invDet->m_spdj;						//保存被删除商品的价格，以供屏幕显示
		m_tmpGoodsNum--;								//显示的当前商品行减1
		m_singleInvInfo->m_sphsl--;						//发票中商品行减1
		m_singleInvInfo->m_kphjje -= invDet->m_spje;	//开票总金额减少
		m_singleInvInfo->m_kpse -= invDet->m_spse;	//开票税额减少
		m_tmpMoneySum -= invDet->m_spje;				//临时单张总金额减少
		
		if (invDet->m_property == DETAIL_GOODS_DISCOUNT)		//有折扣
		{
			m_tmpGoodsNum--;								//显示的当前商品行减1
			m_singleInvInfo->m_sphsl--;							//发票中商品行减1
			m_singleInvInfo->m_kphjje -= nextInvDet->m_spje;	//开票总金额减少
			m_singleInvInfo->m_kpse -= nextInvDet->m_spse;	//开票税额减少
			m_tmpMoneySum -= nextInvDet->m_spje;				//临时单张总金额减少
			m_singleInvInfo->DelNode(dpNo);			
		}
		else if (invDet->m_property == DETAIL_GOODS_REDUCT)		//有折让
		{
			m_tmpGoodsNum--;								//显示的当前商品行减1
			m_singleInvInfo->m_sphsl--;							//发票中商品行减1
			m_singleInvInfo->m_kphjje -= nextInvDet->m_spje;	//开票总金额减少
			m_singleInvInfo->m_kpse -= nextInvDet->m_spse;	//开票税额减少
			m_tmpMoneySum -= nextInvDet->m_spje;				//临时单张总金额减少
			m_singleInvInfo->DelNode(dpNo);				
		}
		m_singleInvInfo->DelNode(dpNo);
	}
	
	DBG_PRINT(("CancelGoodsByDept SUCCESS"));
	DBG_RETURN(SUCCESS);	
}

//乘法销售
UINT8 SaleData::Plus(double tmpAmount)
{
	DBG_ENTER("SaleData::Plus");
	DBG_PRINT(("进入Plus函数"));
	double amount = tmpAmount;
	
	if((doublecmp(tmpAmount, 0.0))==1)
	{
		DBG_PRINT(("The goods amount = 0.0! "));
		DBG_RETURN(ILLEGAL_AMOUNT);
	}
	
	m_dotNum = AmountRound(&tmpAmount);
	if (m_dotNum == -1) //数量超限
	{
		m_dotNum = 0;
		DBG_PRINT(("The goods amount exceeds the limit!"));
		DBG_RETURN(EXCEED_AMOUNT);
	}
	m_saveAmount = tmpAmount; //后续处理用的数量值
	m_tmpAmount = amount;     //计算总金额时用的原始数量值
	
	DBG_PRINT(("Plus"));
	
	INT8 tmp[16];
	INT8 tmp2[16];
	sprintf(tmp, "%%.%luf", m_dotNum);
	sprintf(tmp2, tmp, m_saveAmount);
    DBG_PRINT(("处理值 =  %s", tmp2));
	DBG_PRINT(("m_dotNum =  %d", m_dotNum));
	DBG_PRINT(("原始值 m_tmpAmount = %f", m_tmpAmount));
	
	DBG_PRINT(("退出Plus函数"));
	DBG_RETURN(SUCCESS);
}

UINT8 SaleData::TotalCancel(void)
{
	DBG_ENTER("SaleData::TotalCancel");
	
	m_tmpGoodsNum = 0;  //当前总商品行数清零
	if (m_returnOther == 1) //退非本机发票
	{
		m_tmpMoneySum = 0; //单张票的累计金额
		InitSaleData(0);  //销售信息初始化		
	}
	else
	{
		InitSaleData(1);  //销售信息初始化
	}
	
	InitInvHead();   //单张发票信息初始化
	m_workState = WORK_COMPLETE;
	
	DBG_PRINT(("TotalCancel"));
	DBG_RETURN(SUCCESS);
}

//--------------------------------------------------------
//部类销售
//--------------------------------------------------------
UINT8 SaleData::DeptSale(UINT32 nDeptNo, string strCode)
{
	DBG_ENTER("SaleData::DeptSale");
	DBG_PRINT(("进入DeptSale函数"));
	DBG_PRINT(("nDeptNo==%s", strCode.c_str()));
	DBG_PRINT(("nDeptNo==%u", strCode.c_str()));
	
	char value[128];
	memset((void*)value, 0, sizeof(value));
	string filter;
	INT32 errorcode;
	UINT8 ret = SUCCESS, len = 0;
	
	//查询商品信息
	CDept curDept;
	m_deptInfo = &curDept;
	
	if (strCode != "")//根据PLU检索 
	{
		sprintf(value,"WHERE SP_BM = '%s'",strCode.c_str());	
	}
	else //根据部类号检索
	{
		sprintf(value,"WHERE BLH = %u",nDeptNo);	
	}
	//组装查询条件	
	DBG_PRINT(("value==%s", value));
	m_deptInfo->m_filter = value;
	DBG_PRINT(("m_deptInfo->m_filter==%s", m_deptInfo->m_filter.c_str()));
	
	//查询
	m_deptInfo->Requery();
	errorcode = m_deptInfo->LoadOneRecord();
	if( errorcode != SQLITE_OK )
	{
		DBG_PRINT(("警告：没查到商品 errorcode = %d", errorcode));
		ret = NO_SUCH_GOODS;
		InitSaleData(0);
		DBG_RETURN(ret);	
	}
	//检查商品税率是否授权
	CTax temptax;
	memset((void*)value, 0, sizeof(value));
	sprintf(value,"where SL = %u", (UINT32)(m_deptInfo->m_spsl*DEPT_TAX_EXTENSION));//授权税率表CTax中税率以UINT32形式保存，与CDept中不一致
	temptax.m_filter = value;
	temptax.Requery();
	errorcode = temptax.LoadOneRecord();
	if( errorcode != SQLITE_OK )
	{
		DBG_PRINT(("警告：商品税率未授权 errorcode = %d", errorcode));
		ret = TAX_UNAUTHORIZED;
		InitSaleData(0);
		DBG_RETURN(ret);	
	}
	
	DBG_PRINT(("查到了商品，信息如下:"));
    DBG_PRINT(("m_spbm : %s ", m_deptInfo->m_spbm.c_str()));
	DBG_PRINT(("m_spmc : %s ", m_deptInfo->m_spmc.c_str()));
	DBG_PRINT(("m_spsl : %f ", m_deptInfo->m_spsl));
	DBG_PRINT(("m_spjm : %s ", m_deptInfo->m_spjm.c_str()));
	DBG_PRINT(("m_spdw : %s ", m_deptInfo->m_spdw.c_str()));
	
	//若商品信息无单价，则必须总价销售
	DBG_PRINT(("m_tmpSum==%d", m_tmpSum));
	if( m_deptInfo->m_spdj == 0 )
	{
		if( (m_cancel != CANCEL_PRODUCT) && (m_tmpSum == 0) )
		{
			ret = MUST_INPUT_SUM;			
			InitSaleData(0);  //销售信息初始化
			DBG_PRINT(("退出DeptSale函数"));
			DBG_RETURN(ret);			
		}
		
		m_deptInfo->m_spdj = ((double)m_tmpSum)/SUM_EXTENSION;//将总价赋值给单价
		DBG_PRINT(("m_spdj == %f ", m_deptInfo->m_spdj));
		//处理参数
		m_tmpSum = 0;
		m_tmpAmount = 1.0;   //强制数量为1
		m_saveAmount = 1.0;  //强制数量为1
		m_dotNum = 0;
		m_tmpPrice = 0.0;    //不接受临时价销售
	}
	
	//m_deptInfo = &curDept;
	DBG_ASSERT_EXIT((m_deptInfo != NULL), (" m_deptInfo == NULL!"));
	
	//构造商品详细信息。若是总价销售模式，则根据总价计算出单价来。
	//	if( g_globalArg->m_saleMode == 0 )
	//	{
	//		m_deptInfo->m_spdj = ((double)m_tmpSum)/SUM_EXTENSION;
	//	}
	//	m_deptInfo->m_rate = 100.0;				//若未打折，则默认设为100
	//	m_deptInfo->m_spdw = "piece";
	
	
	//	//处理参数
	//	m_tmpSum = 0;						//xsr 疑问	
	//	m_dotNum = 0;						//
	//	if( g_globalArg->m_saleMode == 0 )	//若是总价销售模式
	//	{
	//		m_tmpPrice = 0.0;				//不接受临时价销售
	//		m_tmpAmount = 1.0;				//强制数量为1
	//		m_saveAmount = 1.0;				//强制数量为1
	//	}
	
	//进入销售主流程函数
	ret = Sale(m_deptInfo);
	m_deptInfo = NULL;
	InitSaleData(0);  //销售信息初始化
	DBG_RETURN(ret);
}

//---------------------------------------------------------------------------------------
//填充打印发票结构体的发票头和明细
//---------------------------------------------------------------------------------------
UINT8 SaleData::InvClassToStuct(struct TInvFullInfo *invFullInfo, CInvHead *smallInvInfo)
{
	DBG_PRINT(("进入InvClassToStuct函数"));
	DBG_ENTER("SaleData::InvClassToStuct");
	
	DBG_ASSERT_EXIT((invFullInfo != NULL)&&(smallInvInfo != NULL), (" invFullInfo == NULL or smallInvInfo = NULL "));
	
	//转换发票整体信息 
	
	//发票代码
	memset((void *)invFullInfo->head.TypeCodeNo, 0, sizeof(invFullInfo->head.TypeCodeNo));
	strcpy( (char *)invFullInfo->head.TypeCodeNo, smallInvInfo->m_fpdm.c_str() );
	DBG_PRINT(("发票代码  : %s ", invFullInfo->head.TypeCodeNo));
	
	//发票号码
	invFullInfo->head.InvNo = smallInvInfo->m_fphm; 
	DBG_PRINT(("发票号码  : %u ", invFullInfo->head.InvNo));
	
	//开票日期 CCYYMMDD
	invFullInfo->head.Date = smallInvInfo->m_kprq;
	DBG_PRINT(("开票日期  : %u ", invFullInfo->head.Date));
	
	//开票时间
	invFullInfo->head.Time = smallInvInfo->m_kpsj;
    DBG_PRINT(("开票时间  : %u ", invFullInfo->head.Time));
	
	//开票类型
	invFullInfo->head.Type = smallInvInfo->m_kplx;
	DBG_PRINT(("开票类型  : %u ", invFullInfo->head.Type));
	
	//开票合计金额 单位：分 小写
	invFullInfo->head.TotalSum = smallInvInfo->m_kphjje;
	DBG_PRINT(("开票合计金额 : %d", invFullInfo->head.TotalSum));
	
	//原发票号码
	invFullInfo->head.PosInvNo = smallInvInfo->m_yfphm;
	DBG_PRINT(("原发票号码  : %u ", invFullInfo->head.PosInvNo));
	
	//付款单位  ASC
	UINT8 nLen = smallInvInfo->m_fkdw.length() + 1;
	memcpy((void *)invFullInfo->head.ClientName, (void *)smallInvInfo->m_fkdw.c_str(), nLen);
	DBG_PRINT(("付款单位 : %s ", invFullInfo->head.ClientName));
	
	//收款员 ASC
	nLen = smallInvInfo->m_sky.length() + 1;
	memcpy((void *)invFullInfo->head.OperatorName, (void *)smallInvInfo->m_sky.c_str(), nLen);
    DBG_PRINT(("收款员 : %s ", invFullInfo->head.OperatorName));
	
	//收款员ID
	//	invFullInfo->head.OpId = smallInvInfo->m_skyid;
	//	DBG_PRINT(("收款员ID : %u ", invFullInfo->head.OpId));
	
	//付款方式
	// 	invFullInfo->head.PayMode = smallInvInfo->m_ptype;
	// 	DBG_PRINT(("付款方式  : %u ", invFullInfo->head.PayMode));
	
	//商品行数量
	invFullInfo->head.detailCount = smallInvInfo->m_sphsl;	//xsr 疑问：类型不匹配
	DBG_PRINT(("商品行数量 : %u", invFullInfo->head.detailCount));
	
	//防伪税控码
	memset(invFullInfo->head.TaxCtrCode, 0, sizeof(invFullInfo->head.TaxCtrCode));
	memcpy(invFullInfo->head.TaxCtrCode, smallInvInfo->m_fwm.c_str(), smallInvInfo->m_fwm.length());
	DBG_PRINT(("防伪税控吗 : %s", invFullInfo->head.TaxCtrCode));
	
	//税率
	smallInvInfo->m_sl = 0.0;
	invFullInfo->head.sl = 0.0;
	DBG_PRINT(("税率      : %f", invFullInfo->head.sl));
	
	
	//发票类别
	//	smallInvInfo->m_fplb = 0;
	//	invFullInfo->head.fplb = 0;
	//	DBG_PRINT(("发票类别    : %d", invFullInfo->head.fplb));
	
	//上传标志
	smallInvInfo->m_scbz = 0;
	invFullInfo->head.scbz = 0;
	DBG_PRINT(("上传标志    : %d", invFullInfo->head.scbz));
	
	//打印次数
	smallInvInfo->m_dycs = 0;
	invFullInfo->head.dycs = 0;
	DBG_PRINT(("打印次数    : %d", invFullInfo->head.dycs));
	
	
	
	//转换发票明细信息
	CInvDet *p = smallInvInfo->pHead;
	
    //若为空链表
	if (p == NULL)
	{
		DBG_PRINT((" smallInvInfo->pHead = NULL !"));
		DBG_RETURN(SUCCESS);		
	}
	
	//--------------------------------------------------------------------------
    //若为非空链表
	//--------------------------------------------------------------------------
	for(UINT8 i=0; i<smallInvInfo->m_sphsl; i++, p = p->pNext)
	{
		//将带有折扣率的商品行分成两行来打
		invFullInfo->detail[i].Property = p->m_property;    //商品行属性
		invFullInfo->detail[i].Amount =  p->m_spsl;			//商品数量		   
		invFullInfo->detail[i].Price = p->m_spdj;			//商品单价		
		invFullInfo->detail[i].Sum = p->m_spje;				//商品金额
		invFullInfo->detail[i].TaxSum = p->m_spse;			//税额
		invFullInfo->detail[i].TaxItem = p->m_sl;			//税率
		memcpy((void *)invFullInfo->detail[i].GoodsCode, (void *)p->m_spbm.c_str(), p->m_spbm.length());	//商品编码
		nLen = p->m_spmc.length() + 1;
		memcpy((void *)invFullInfo->detail[i].GoodsName, (void *)p->m_spmc.c_str(), nLen);				//商品名称
		
		DBG_PRINT(("---------------------------------"));
		DBG_PRINT(("商品行属性  : %u ", invFullInfo->detail[i].Property));
		DBG_PRINT(("商品数量    : %f ", invFullInfo->detail[i].Amount));
		DBG_PRINT(("商品单价    : %f ", invFullInfo->detail[i].Price));
		DBG_PRINT(("商品金额    : %d ", invFullInfo->detail[i].Sum));
		DBG_PRINT(("商品税额    : %d ", invFullInfo->detail[i].TaxSum));
		DBG_PRINT(("商品税率    : %f ", invFullInfo->detail[i].TaxItem));
		DBG_PRINT(("商品编码    : %s ", invFullInfo->detail[i].GoodsCode));
		DBG_PRINT(("商品名称    : %s ", invFullInfo->detail[i].GoodsName));
	}
	
	DBG_PRINT(("退出InvClassToStuct函数"));
	DBG_RETURN(SUCCESS);
}

//------------------------------------------------------
//部分数据库内容滚动
//------------------------------------------------------
UINT8 SaleData::RollData(UINT8 &nIfRolled)
{
	CInvHead curInvHead;
	CRtInv curRtInv;
	CInvSum curInvSum;
	
	UINT8 ret = SUCCESS;
	
	UINT32 nInvDelDate = 0;
	UINT32 nSumDelDate = 0;
	UINT32 nRtDetNo = 0;
	nIfRolled = 0;
	
	// 	CInvDet countDet;			//压力测试用
	// 	countDet.Requery();			//压力测试用
	// 	countDet.LoadOneRecord();	//压力测试用
	// 	DBG_PRINT(("第一条明细的日期invDet.m_kprq = %u", countDet.m_kprq));
	DBG_PRINT(("g_globalArg->m_invCount = %u", g_globalArg->m_invCount));
	DBG_PRINT(("g_globalArg->m_invSumCount = %u", g_globalArg->m_invSumCount));
	
	
	TDateTime lastDate(g_globalArg->m_startDecDate, 000001);
	lastDate--;
	UINT32 nRefDate = lastDate.FormatInt(YYYYMMDD);//上次申报的截止日期
	
	//判断是否需要滚动，并计算需滚动的记录数量
	ret = curInvHead.CheckRoll(g_globalArg->m_invCount, nRefDate,nInvDelDate);
	if (ret!=SUCCESS)
	{
		DBG_RETURN(FAILURE);
	}
	
	ret = curInvSum.CheckRoll(g_globalArg->m_invSumCount, nRefDate,nSumDelDate);
	if (ret!=SUCCESS)
	{
		DBG_RETURN(FAILURE);
	}
	
	ret = curRtInv.CheckRoll(g_globalArg->m_rtInvCount,nRtDetNo);
	if (ret!=SUCCESS)
	{
		DBG_RETURN(FAILURE);
	}
	
	//掉电保护 滚动数据
	g_YW_PowerOffData->invRollPwOffData.invDatelimit = nInvDelDate;/**<InvHead表滚动删除的截止日期  */ 
	g_YW_PowerOffData->invRollPwOffData.sumDatelimit = nSumDelDate;/**<InvSum表滚动删除的截止日期  */  
	g_YW_PowerOffData->invRollPwOffData.rtNolimit = nRtDetNo;/**<RtInv表滚动删除的截止号码  */   
    g_YW_PowerOffData->invRollPwOffData.nRefDate = nRefDate; /**<上次申报的截止日期  */ 
	POWOFF_ENABLE();//开掉电中断	
	
	UINT8 retRoll;
	BAR_DEF();
	//滚动
	if (nInvDelDate>0)
	{
		DBG_PRINT(("发票信息整理中..... "));
		BAR_SHOW("发票信息整理中.....");
		retRoll = curInvHead.Roll(nInvDelDate, nRefDate);//InvHead表滚动
		if (FAILURE==retRoll) 
		{
			BAR_SHOW("请重启机器以完成发票整理");
			while (1);
		}
		CGlobalArg::SysLog(EVENT_INVOICE_ROLL , SUCCESS);		//写系统日志
		g_globalArg->m_invCount = curInvHead.GetInvHeadNum();	//InvHead表记录数
		nIfRolled = 1;
		BAR_SHOW("发票信息整理完成!");
	}	
	
	
	if (nSumDelDate>0)
	{
		DBG_PRINT(("卷汇总信息整理中..... "));
		BAR_SHOW("卷汇总信息整理中.....");
		curInvSum.Roll(nSumDelDate);//InvSum表滚动
		CGlobalArg::SysLog(EVENT_VOLUME_ROLL , SUCCESS);//写系统日志
		g_globalArg->m_invSumCount = curInvSum.GetRecordNum();//InvSum表记录数
		nIfRolled = 1;
		BAR_SHOW("卷汇总信息整理完成!");
	}
	
	
	if (nRtDetNo>0)
	{
		DBG_PRINT(("红票信息整理中..... "));
		BAR_SHOW("红票信息整理中.....");
		curRtInv.Roll(nRtDetNo);//RtInv表滚动
		CGlobalArg::SysLog(EVENT_RTINV_ROLL , SUCCESS);//写系统日志
		g_globalArg->m_rtInvCount = curRtInv.GetRecordNum();//RtInv表记录数
		nIfRolled = 1;
		BAR_SHOW("红票信息整理完成!");
	}
	
	
	DBG_RETURN(SUCCESS);
}


UINT8 SaleData::GoodsReduct(double money)
{
	UINT8 ret;
	INT64 nReDuctSum;
	
	if (NULL == m_singleInvInfo->pHead) 
	{
		return NO_GOODS;
	}
	
	//把金额换算成整形
	nReDuctSum = double2int(money*SUM_EXTENSION*10);		
	nReDuctSum = double2int(nReDuctSum / 10.0);//折让掉的金额
	DBG_PRINT((" money = %f，not enough!", money));
	DBG_PRINT((" 取整后 nReDuctSum = %d!", nReDuctSum));
	
	//金额不足
	if (nReDuctSum <= 0)
	{
		DBG_PRINT((" nReDuctSum = %d，not enough!", nReDuctSum));
		InitSaleData(0);  //销售信息初始化
		DBG_RETURN(REDUCT_MONEY_SMALL);
	}
	
	//修改最后一个节点的属性
	m_invDet = m_singleInvInfo->pEnd;
	if (m_invDet->m_property != DETAIL_GENERAL_GOODS) 
	{
		DBG_PRINT((" 非普通商品行 m_property = %u!", m_invDet->m_property));
		InitSaleData(0);  //销售信息初始化
		DBG_RETURN(NO_REDUCT);
	}
	//折让金额超过被折让商品行总金额
	if (m_invDet->m_spje <= nReDuctSum) 
	{
		DBG_PRINT((" nReDuctSum = %d，not enough!", nReDuctSum));
		InitSaleData(0);  //销售信息初始化
		DBG_RETURN(REDUCT_EXCEED);
	}
	
	m_invDet->m_property = DETAIL_GOODS_REDUCT;//改成带折让的商品行
	nReDuctSum = 0 - nReDuctSum;//折让的金额以负数记录
	
	//新增一个结点（折让行）	
	CInvDet *reductDet; 
	reductDet = reductDet->GetNewInvDet();//从静态数组获取一个可用的CInvDet对象
	DBG_ASSERT_EXIT((reductDet != NULL), (" reductDet == NULL!"));	
	reductDet->m_spbm = ""; 
	reductDet->m_kplx = m_invDet->m_kplx;
	//	reductDet->m_issuetype = m_invDet->m_issuetype;
	reductDet->m_spbm = m_invDet->m_spbm;
	reductDet->m_spmc = "折让";
	reductDet->m_sl = m_invDet->m_sl;	//税率
	reductDet->m_spsl = m_saveAmount;
	m_invDet->m_dotNum = m_dotNum;
	reductDet->m_spdj = 0;				//??
	reductDet->m_spje = nReDuctSum;
//	reductDet->m_spje = CountTax(((double)reductDet->m_spje)/SUM_EXTENSION,m_invDet->m_sl);
    reductDet->m_spje =CountTax(reductDet->m_spje,(UINT32)(m_invDet->m_sl*SUM_EXTENSION));
	reductDet->m_property = DETAIL_REDUCTION_TEMPLATE;//???折让行
	//	reductDet->m_backup = "";
	
	m_singleInvInfo->InsertNode(reductDet);		//插入节点
	m_singleInvInfo->m_sphsl++;					//当前发票总商品行加1
	reductDet = NULL;
	
	
    //----判断剩余发票份数是否够用(若有折让，则删除两个节点)
	UINT32 nInvCount = CalculateInvNum();
	UINT32 nIfReturn = 0; //是否取消该商品行，停止开票
	if (nInvCount > g_globalArg->m_curInvVol->m_remain)
	{
		//	nIfReturn = 1;
		ret = SHORT_OF_INV; 
		DBG_PRINT((" g_globalArg->m_curInvVol->m_remain = %u !", g_globalArg->m_curInvVol->m_remain));
	}
	if ((nInvCount>1)&&((m_singleInvInfo->m_kplx==RETURN_INV)||(m_singleInvInfo->m_kplx==RET_MANUAL_INV))) 
	{
		nIfReturn = 1;
			ret = RETURN_GOODS_EXCEED;
	}
// 	if ((nInvCount>1)) //为保证上传，不让拆张
	// 	{
	// 		nIfReturn = 1;
// 		//	ret = RETURN_GOODS_EXCEED;
// 		ret = ONE_INV;
	// 	}
	if (1 == nIfReturn)
	{
		DBG_PRINT((" 取消折让行!"));
		m_singleInvInfo->m_sphsl--;//当前发票总商品行减1
		m_singleInvInfo->DelLastNode();	//删除刚加入的节点
		m_singleInvInfo->pEnd->m_property = DETAIL_GENERAL_GOODS;//改成带折让的商品行
		InitSaleData(0);  //销售信息初始化
		DBG_RETURN(ret);
	}
	
    //----释放空间,返回
	//当前发票累计金额增加
	m_singleInvInfo->m_kphjje += nReDuctSum;
	m_tmpGoodsNum++;								//当前总商品行加1
	m_tmpMoneySum += nReDuctSum;					//本张累计金额加上折让行金额
	DBG_PRINT((" m_singleInvInfo->m_kphjje = %u !", m_singleInvInfo->m_kphjje));
	
	InitSaleData(0);  //销售信息初始化
	
    ret = SUCCESS;
	DBG_PRINT((" GoodsReduct Succeed !"));
	DBG_RETURN(ret);
	
}
#if 0
UINT8 SaleData::InvSum(UINT8 &nIfSum)
{
	DBG_ENTER("SaleData::InvSum");
	
	nIfSum = 0;
	UINT8  ret = SUCCESS;
	string strTmp;
	char chValue[256], chTmpValue[128];
	memset((void*)chValue, 0, sizeof(chValue));
	memset((void*)chTmpValue, 0, sizeof(chTmpValue));
	
	CInvHead sumInvHead;
	CInvDet  sumInvDet; 
	CInvSum  sumInvSum;
	CInvVol  sumInvVol;
	
	CInvHead *pInvHead = &sumInvHead;
	CInvDet  *pInvDet = &sumInvDet; 
	CInvSum  *pInvSum = &sumInvSum;
	CInvVol  *pInvVol = &sumInvVol;
	DBG_ASSERT_EXIT((pInvHead != NULL)&&(pInvDet != NULL), (" pInvHead == NULL or pInvDet = NULL "));
    DBG_ASSERT_EXIT((pInvSum != NULL), (" pInvSum == NULL!"));
	DBG_ASSERT_EXIT((pInvVol != NULL), (" pInvVol == NULL!"));
	
    if (!((0==g_globalArg->m_curInvVol->m_usedflag)&&(1==g_globalArg->m_curInvVol->m_overflag))) 
	{
		DBG_PRINT(("没有导入的发票卷"));
		DBG_RETURN(SUCCESS);		
    }
	
	//发票代码，起始号码，终止号码
    pInvSum->m_code = g_globalArg->m_curInvVol->m_code;
    pInvSum->m_InvStartNo = g_globalArg->m_curInvVol->m_isno;
	pInvSum->m_InvEndNo = g_globalArg->m_curInvVol->m_ieno;
	
	sprintf(chValue, "where CODE = '%s' and IS_NO = %u",pInvSum->m_code.c_str(),pInvSum->m_InvStartNo);
	pInvSum->m_filter = chValue;
	pInvSum->Requery();
	if ((pInvSum->LoadOneRecord())==SQLITE_OK) 
	{
		DBG_PRINT(("该卷已汇总"));
		pInvHead = NULL;
		pInvDet = NULL;	
		pInvSum = NULL;
		pInvVol = NULL;
		DBG_RETURN(SUCCESS);
	}
	
    //正票，红票，废票份数
	pInvHead->m_filter = "";
	sprintf(chValue, "where FPHM>=%u and FPHM<=%u and FPDM= '%s'",pInvSum->m_InvStartNo,pInvSum->m_InvEndNo,pInvSum->m_code.c_str());
    pInvHead->m_filter.append(chValue);
	pInvHead->GetInvoiceSum(pInvSum->m_normalInvSum, pInvSum->m_returnInvSum, pInvSum->m_wasteInvSum);
    
    DBG_PRINT(("pInvSum->m_normalInvSum = %u", pInvSum->m_normalInvSum));
    DBG_PRINT(("pInvSum->m_returnInvSum = %u", pInvSum->m_returnInvSum));
	DBG_PRINT(("pInvSum->m_wasteInvSum = %u", pInvSum->m_wasteInvSum));
	
	//正票总金额
	strTmp = "";
	strTmp = "select sum(KPHJJE) from INV_HEAD "; 
	strTmp.append(chValue);
	sprintf(chTmpValue, " and KPLX = %u",NORMAL_INV);
	strTmp.append(chTmpValue);
    pInvHead->SetSQL(strTmp);
	pInvHead->GetOneResult(DB_UINT32, (void*)&(pInvSum->m_nomalMoneySum)); 
    
	//红票总金额
	strTmp = "";
	strTmp = "select sum(KPHJJE) from INV_HEAD "; 
	strTmp.append(chValue);
	sprintf(chTmpValue, " and (KPLX = %u or KPLX = %u or KPLX = %u)",RETURN_INV, RET_MANUAL_INV, RET_SPECIAL_INV);
	strTmp.append(chTmpValue);
    pInvHead->SetSQL(strTmp);
	pInvHead->GetOneResult(DB_UINT32, (void*)&(pInvSum->m_returnMoneySum)); 
	
	//开票起始日期
    strTmp = "";
	strTmp = "select KPRQ from INV_HEAD "; 
	//	sprintf(chValue, "where INV_NO=%u and CODE_NO=%u",pInvSum->m_InvStartNo,pInvSum->m_codeNo);
	sprintf(chValue, "where FPHM>=%u and FPDM= '%s'",pInvSum->m_InvStartNo,pInvSum->m_code.c_str());
	strTmp.append(chValue);
    pInvHead->SetSQL(strTmp);
	pInvHead->GetOneResult(DB_UINT32, (void*)&(pInvSum->m_issueStartDate)); 
    
	//开票截止日期
	strTmp = "";
	strTmp = "select KPRQ from INV_HEAD "; 
	sprintf(chValue, "where FPHM=%u and FPDM= '%s'",pInvSum->m_InvEndNo,pInvSum->m_code.c_str());
	strTmp.append(chValue);
    pInvHead->SetSQL(strTmp);
	pInvHead->GetOneResult(DB_UINT32, (void*)&(pInvSum->m_issueEndDate)); 
	
	//添加记录
    pInvSum->AddNew();
	
	//修改发票领用信息
	strTmp = "";
	strTmp = "update INV_VOL set OVER_FLAG = 0 "; 
    sprintf(chValue, "where IS_NO=%u and CODE= '%s'",pInvSum->m_InvStartNo,pInvSum->m_code.c_str());
    strTmp.append(chValue);
	DBG_PRINT(("strTmp = %s", strTmp.c_str()));
	pInvVol->SetSQL(strTmp);
    if(pInvVol->ExecuteSQL()!= SQLITE_OK)
	{
		DBG_PRINT(("修改发票领用信息失败！"));
		ret = FAILURE;
	}
	
	
    pInvHead = NULL;
	pInvDet = NULL;
	pInvSum = NULL;
	pInvVol = NULL;
	
	nIfSum = 1;//成功进行了卷汇总
	g_globalArg->m_curInvVol->m_overflag = 0;//当前卷已用完
	(g_globalArg->m_invSumCount)++; //记录条数自增
	//	g_globalArg->m_normalVolSum = 0;//发票卷正票累计金额清零
	//	g_globalArg->m_returnVolSum = 0;//发票卷红票累计金额清零
	//	DBG_PRINT(("*****g_globalArg->m_normalVolSum  = %d", g_globalArg->m_normalVolSum ));
	//	DBG_PRINT(("*****g_globalArg->m_returnVolSum  = %d", g_globalArg->m_returnVolSum ));
	DBG_RETURN(ret);
	
}
#endif

// 判断离线金额、张数和天数的条件检查
UINT8 SaleData::IsOffLineOk(INT64 nSum, string& strErr)
{
#if 0
	//是否超发票张数
	DBG_PRINT(("m_InvServNum = %u, m_nNum = %u", g_globalArg->m_InvServNum, g_globalArg->m_invKind->m_nNum));
	if ((g_globalArg->m_InvServNum+1) > g_globalArg->m_invKind->m_nNum) 
	{
		DBG_PRINT(("离线开票张数超限"));
		strErr = "离线开票张数超限";
		return GX_SV_NUM_EXCEED;
	}
	
	//是否超发票金额
	if ((g_globalArg->m_InvServSum+nSum) > g_globalArg->m_invKind->m_nSum) 
	{
		DBG_PRINT(("离线开票金额超限 m_InvServSum = %u", g_globalArg->m_InvServSum));
		strErr = "离线开票金额超限";
		return GX_SV_SUM_EXCEED;
	}
	
	
	//是否超过开票时间  
	CInvServ invServ;
	//	invServ.m_filter = "where UP_FLAG = 0 ";//第一张未上传的发票
	invServ.Requery();
	UINT8 errCode = invServ.LoadOneRecord();
	if (SQLITE_DONE == errCode)
	{
		return SUCCESS;
	}
	else if (SQLITE_OK != errCode) 
	{
		DBG_PRINT(("Requery INV_SERV error = %d", errCode));
		strErr = "查询发票缓存表错误";
		return GX_SV_TIME_EXCEED;
	}
	
	// 	TDateTime fDate(invServ.m_issueDate, invServ.m_issueTime);
	// 	fDate += (INT32)(g_globalArg->m_invKind->m_nTime);//离线截止日期
	// 	DBG_PRINT(("离线截止日 = %s", fDate.FormatString(YYYYMMDDHHMMSS).c_str()));
	// 	
	// 	TDateTime curDate = TDateTime::CurrentDateTime();
	// 	if (curDate > fDate) 
	// 	{
	// 		DBG_PRINT(("离线开票时间超限"));
	// 		DBG_PRINT(("Eg_globalArg->m_usermore->m_nTime = %"));
	// 		strErr = "离线开票时间超限";
	// 		return GX_SV_TIME_EXCEED;		
	// 	}
	
	
	TDateTime fDate(invServ.m_issueDate, invServ.m_issueTime);
	//fDate = fDate.HourAdd(fDate, g_globalArg->m_invKind->m_nTime);
	DBG_PRINT(("离线截止时间 = %s", fDate.FormatString(YYYYMMDDHHMMSS).c_str()));
	// 	UINT32 nfHour = fDate.Hour();
	// 	DBG_PRINT(("nfHour = %u", nfHour));
	// 	
	// 	UINT32 nDay = (nfHour + g_globalArg->m_invKind->m_nTime)/24;
	// 	UINT32 nHour = (nfHour + g_globalArg->m_invKind->m_nTime)%24;
	// 	DBG_PRINT(("g_globalArg->m_invKind->m_nTime = %u", g_globalArg->m_invKind->m_nTime));	
	// 	DBG_PRINT(("nDay = %u", nDay));	
	// 	DBG_PRINT(("nHour = %u", nHour));
	
	// 	fDate += nDay;//缓存时间的截止日期
	// 	UINT32 fMinute = fDate.Minute();
	// 	UINT32 fSecond = fDate.Second();
	// 
	// 
	// 
	// 	fDate.ComposeTime(nHour, fMinute, fSecond);//缓存时间的截止时间
	TDateTime curDate = TDateTime::CurrentDateTime();
	// 	if (NULL != pEndDate) 
	// 	{
	// 		*pEndDate = fDate;
	// 		DBG_PRINT(("*pEndDate = %s", (*pEndDate).FormatString(YYYYMMDDHHMMSS).c_str()));
	// 	}
	DBG_PRINT(("离线开票时间超限, fDate = %s", fDate.FormatString(YYYYMMDDHHMMSS).c_str()));
	DBG_PRINT(("当前时间, curDate = %s", curDate.FormatString(YYYYMMDDHHMMSS).c_str()));
	if (curDate > fDate) 
	{
		// 		DBG_PRINT(("离线开票时间超限, fDate = %s", fDate.FormatString(YYYYMMDDHHMMSS).c_str()));
		// 		DBG_PRINT(("离线开票时间超限, curDate = %s", curDate.FormatString(YYYYMMDDHHMMSS).c_str()));
		//		nM2MRet = wmmp_trapAlarmCode(M2M_EXCD_SV_TIME);
		//		DBG_PRINT((" nM2MRet = %d!", nM2MRet));
		DBG_PRINT(("g_globalArg->m_invKind->m_nTime = %u", g_globalArg->m_invKind->m_nTime));
		strErr = "离线开票时间超限";
		return GX_SV_TIME_EXCEED;	
	}
#endif	
	return SUCCESS;
}

/*
//计算商品税额
INT64 SaleData::CountTax(double fJe, double fSl)
{
	DBG_ENTER("SaleData::CountTax");
	
	DBG_PRINT(("fJe= %.4f",fJe));
	DBG_PRINT(("fSl= %.4f",fSl));
    INT64 nSe =0;

	double fTmp =0.0;
	
	fTmp =fSl;
    fTmp += 1.0;
	DBG_PRINT(("fTmp= %.4f",fTmp));
	
	fTmp  = fJe / fTmp;
	DBG_PRINT(("fTmp= %.4f",fTmp));
	
	fTmp *= fSl;
    DBG_PRINT(("fTmp= %.4f",fTmp));
		
    nSe =double2int(fTmp*PRICE_EXTENSION);
	DBG_PRINT(("nSe= %u",nSe));
	
	return nSe;
	
}
*/

//计算商品税额
INT64 SaleData::CountTax(INT64 nJe, INT32 nSl)
{
	DBG_ENTER("SaleData::CountTax");
	
	DBG_PRINT(("nJe= %lld",nJe));
	DBG_PRINT(("nSl= %d",nSl));
    INT64 nSe =0;
	
	INT64 nTmp =0;
	
	nTmp =nSl;
    nTmp += SUM_EXTENSION;
	DBG_PRINT(("nTmp= %lld",nTmp));

	double fTmp= (double)(nJe*1.0) / nTmp;
	DBG_PRINT(("fTmp= %.4f",fTmp));

	nTmp  = double2int(fTmp*SUM_EXTENSION);
	
	DBG_PRINT(("nTmp= %lld",nTmp));
	
	nSe = nJe-nTmp;

	DBG_PRINT(("nSe= %lld",nSe));
	
	return nSe;
	
}