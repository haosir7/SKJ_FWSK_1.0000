
#include "BusinessXmlProc.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

#if BUSINESS_TYPE_MODE == BUSINESS_XML_MODE

#include "YWXML_SKSBBHCX.h"
#include "YWXML_SKPXXCX.h"
#include "YWXML_BSPXXCX.h"
#include "YWXML_SQSLCX.h"
#include "YWXML_GPXXCX.h"
#include "YWXML_KLBG.h"
#include "YWXML_JKSJCX.h"
#include "YWXML_GPXXXP.h"
#include "YWXML_GPXXXPJS.h"
#include "YWXML_FPKJ.h"
#include "YWXML_FPZF.h"
#include "YWXML_FPCX.h"
#include "YWXML_SKPBSP.h"
#include "YWXML_BSPFPFF.h"
#include "YWXML_BSPFPSH.h"
#include "YWXML_SJCB.h"
#include "YWXML_JKHC.h"
#include "YWXML_TJXXCX.h"
#include "YWXML_LXXXXP.h"
#include "YWXML_FPSSSC.h"
#include "YWXML_FPGX.h"
#include "YWXML_FPDR.h"
#include "YWXML_FJTH.h"
#include "YWXML_BSPFPCX.h"
#include "YWXML_SKSBQTYXXCX.h"
#include "YWXML_BGZSKL.h"
#include "YWXML_FPBL.h"

#include "NETXML_FPSC.h"
#include "NETXML_FPSCJGHQ.h"
#include "NETXML_WLCB.h"
#include "NETXML_QLJS.h"
#include "NETXML_LXXXSC.h"
#include "NETXML_WLLQFP.h"
#include "NETXML_WLLQFPJGQR.h"


CBusinessXmlProc::CBusinessXmlProc()
{
	
}

CBusinessXmlProc::~CBusinessXmlProc()
{
	
}

//税控设备编号查询
INT32 CBusinessXmlProc::SKSBBH_Business(CYWXML_GY &ywxml_gy, string &sksbbh, string &bspbh, string &qtxx, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_SKSBBH;
	CSksbbhcx sksbbhcx(ywxml_gy, sksbbh,bspbh,qtxx);

	ret = sksbbhcx.YWXml_Proc(strErr);

	return ret;
}

//报税盘编号查询
// INT32 CBusinessXmlProc::BSPBH_Business(CYWXML_GY &ywxml_gy, string &bspbh, string &strErr)
// {
// 	INT32 ret = XML_SUCCESS;
// 	string tmpStr;
// 	ywxml_gy.m_strID = BUSINESS_YWID_SKSBBH;
// 	CSksbbhcx sksbbhcx(ywxml_gy, tmpStr, bspbh, tmpStr);
// 	
// 	ret = sksbbhcx.YWXml_Proc(strErr);
// 
// 	
// 	return ret;
// }

INT32 CBusinessXmlProc::SKPXXCX_Business(CYWXML_GY &ywxml_gy, CUserInfo *pUserInfo, string &strCurTime, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_SKPXXCX;
	CSkpxxcx Skpxxcx(ywxml_gy, *pUserInfo, strCurTime);

	ret = Skpxxcx.YWXml_Proc(strErr);

	return ret;
}

//税控设备其他信息查询
INT32 CBusinessXmlProc::SKSBQTYXXCX_Business(CYWXML_GY &ywxml_gy, UINT8 xxlx, string &sksbxx, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_SKSBQTYXXCX;
	CSksbqtxxcx Sksbqtxxcx(ywxml_gy, xxlx, sksbxx);

	ret = Sksbqtxxcx.YWXml_Proc(strErr);

	return ret;
}

//报税盘信息查询
INT32 CBusinessXmlProc::BSPXXCX_Business(CYWXML_GY &ywxml_gy, CUserInfo *pUserInfo, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_BSPXXCX;
	CBspxxcx Bspxxcx(ywxml_gy, *pUserInfo);

	ret = Bspxxcx.YWXml_Proc(strErr);

	return ret;
}

//监控数据查询
INT32 CBusinessXmlProc::JKSJCX_Business(CYWXML_GY &ywxml_gy, CInvKind *pInvKind, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_JKSJCX;
	CJksjcx Jksjcx(ywxml_gy, *pInvKind);

	ret = Jksjcx.YWXml_Proc(strErr);

	return ret;
}

//授权税率查询
INT32 CBusinessXmlProc::SQSLCX_Business(CYWXML_GY &ywxml_gy, CTax *pTax, UINT8 &taxNum, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_SQSLCX;

	CTaxInfo taxInfo;
	CSqslcx Sqslcx(ywxml_gy, taxInfo);

	ret = Sqslcx.YWXml_Proc(strErr);

	DBG_PRINT(("tax num = %u", taxInfo.m_TaxCoun));
	taxNum = taxInfo.m_TaxCoun;
	for(int i=0; i<taxNum; i++)
	{
		pTax[i].m_sl = taxInfo.m_Tax[i].m_sl;
	}

	return ret;
}

//购票信息查询
INT32 CBusinessXmlProc::GPXXCX_Business(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_GPXXCX;
	CGpxxcx Gpxxcx(ywxml_gy, *pInvVol);
	
	ret = Gpxxcx.YWXml_Proc(strErr);

	return ret;
}

//购票信息写盘
INT32 CBusinessXmlProc::GPXXXP_Business(CYWXML_GY &ywxml_gy, string gpxxmw, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_GPXXXP;
	CGpxxxp Gpxxxp(ywxml_gy, gpxxmw);
	
	ret = Gpxxxp.YWXml_Proc(strErr);


	return ret;
}

//购票信息写盘解锁
INT32 CBusinessXmlProc::GPXXXPJS_Business(CYWXML_GY &ywxml_gy, string wslpjsxx, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_GPXXXPJS;
	CGpxxxpjs Gpxxxpjs(ywxml_gy, wslpjsxx);
	
	ret = Gpxxxpjs.YWXml_Proc(strErr);

	return ret;
}

//口令变更
INT32 CBusinessXmlProc::KLBG_Business(CYWXML_GY &ywxml_gy, string oldPwd, string newPwd, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_KLBG;
	CKlbg Klbg(ywxml_gy, oldPwd, newPwd);

	ret = Klbg.YWXml_Proc(strErr);

	return ret;
}

//发票开具
INT32 CBusinessXmlProc::FPKJ_Business(CYWXML_GY &ywxml_gy, CInvHead *pInvhead, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_FPKJ;
	CFpkj Fpkj(ywxml_gy, *pInvhead);
	
	ret = Fpkj.YWXml_Proc(strErr);

	return ret;
}

//发票作废
INT32 CBusinessXmlProc::FPZF_Business(CYWXML_GY &ywxml_gy, CInvHead *pInvhead, UINT8 zflx, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_FPZF;
	CFpzf Fpzf(ywxml_gy, zflx, *pInvhead);
	
	ret = Fpzf.YWXml_Proc(strErr);

	return ret;
}

//发票查询
INT32 CBusinessXmlProc::FPCX_Business(CYWXML_GY &ywxml_gy, UINT8 cxfs, string cxtj, UINT32 &invNum, CInvHead *pInvhead, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_FPCX;
	CFpcx Fpcx(ywxml_gy, cxfs, cxtj, *pInvhead, invNum);
	
	ret = Fpcx.YWXml_Proc(strErr);

	return ret;
}

//税控设备报税盘组合操作
INT32 CBusinessXmlProc::SKPBSP_Business(CYWXML_GY &ywxml_gy, UINT8 czlx, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	string strTemp;
	ywxml_gy.m_strID = BUSINESS_YWID_SKPBSP;
	CSkpBsp SkpBsp(ywxml_gy, czlx, strTemp);
	
	ret = SkpBsp.YWXml_Proc(strErr);
	return ret;
}

//发票段信息查询(报税盘发票查询)
INT32 CBusinessXmlProc::BSPFPCX_Business(CYWXML_GY &ywxml_gy, UINT32 &invNum, CInvVol *pInvVol, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_BSPFPCX;

	CInvVolInfo InvVolInfo;
	CBspfpcx Bspfpcx(ywxml_gy, InvVolInfo);
	
	ret = Bspfpcx.YWXml_Proc(strErr);

	DBG_PRINT(("invvol num = %u", InvVolInfo.m_InvvolCount));
	invNum = InvVolInfo.m_InvvolCount;
	for(int i=0; i<invNum; i++)
	{
		pInvVol[i].m_code = InvVolInfo.m_InvVol[i].m_code;
		pInvVol[i].m_isno = InvVolInfo.m_InvVol[i].m_isno;
		pInvVol[i].m_ieno = InvVolInfo.m_InvVol[i].m_ieno;
		pInvVol[i].m_remain = InvVolInfo.m_InvVol[i].m_remain;
		pInvVol[i].m_fpzfs = InvVolInfo.m_InvVol[i].m_fpzfs;
		pInvVol[i].m_date = InvVolInfo.m_InvVol[i].m_date;
		DBG_PRINT(("pInvVol[%d].m_isno = %u", i, pInvVol[i].m_isno));
	}
	
	return ret;
}

//发票分发
INT32 CBusinessXmlProc::BSPFPFF_Business(CYWXML_GY &ywxml_gy, UINT8 jzlx, CInvVol *pInvVol, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_BSPFPFF;
	CBspfpff Bspfpff(ywxml_gy,  *pInvVol, jzlx);	

	ret = Bspfpff.YWXml_Proc(strErr);

	return ret;
}

//发票收回
INT32 CBusinessXmlProc::BSPFPSH_Business(CYWXML_GY &ywxml_gy, UINT8 jzlx, CInvVol *pInvVol, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_BSPFPSH;
	CBspfpsh Bspfpsh(ywxml_gy, *pInvVol, jzlx);	

	ret = Bspfpsh.YWXml_Proc(strErr);

	return ret;
}

//数据抄报
INT32 CBusinessXmlProc::SJCB_Business(CYWXML_GY &ywxml_gy, UINT8 jzlx, string &strSQ, string &strFphz, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_SJCB;
	CSjcb Sjcb(ywxml_gy, jzlx, strSQ, strFphz);
	
	ret = Sjcb.YWXml_Proc(strErr);

	return ret;
}

//监控回传
INT32 CBusinessXmlProc::JKHC_Business(CYWXML_GY &ywxml_gy, string strFpjkmw, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_JKHC;
	CJkhc Jkhc(ywxml_gy, strFpjkmw);
	
	ret = Jkhc.YWXml_Proc(strErr);

	return ret;
}

//发票统计信息查询
INT32 CBusinessXmlProc::TJXXCX_Business(CYWXML_GY &ywxml_gy, INT32 &monthcount, CTjxxhz *pTjxxhz, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_TJXXCX;
	CTjxxcx Tjxxcx(ywxml_gy, *pTjxxhz);
	
	ret = Tjxxcx.YWXml_Proc(strErr);

	return ret;
}

//离线信息写盘
INT32 CBusinessXmlProc::LXXXXP_Business(CYWXML_GY &ywxml_gy, string lxxxmw, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_LXXXXP;
	CLxxxxp Lxxxxp(ywxml_gy, lxxxmw);
	
	ret = Lxxxxp.YWXml_Proc(strErr);

	return ret;
}

//发票实时上传
INT32 CBusinessXmlProc::FPSC_Business(CYWXML_GY &ywxml_gy, UINT8 czlx, UINT32 &fpzx, string &strInvInfo, string &strErr)
{
	INT32 ret = XML_SUCCESS;

	ywxml_gy.m_strID = BUSINESS_YWID_FPSC;
	CFpsssc Fpsssc(ywxml_gy, fpzx, czlx, strInvInfo);
	
	ret = Fpsssc.YWXml_Proc(strErr);

	return ret;
}

//发票更新
INT32 CBusinessXmlProc::FPGX_Business(CYWXML_GY &ywxml_gy, string scqrbw, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_FPGX;
	CFpgx Fpgx(ywxml_gy, scqrbw);
	
	ret = Fpgx.YWXml_Proc(strErr);

	return ret;
}

//发票读入
INT32 CBusinessXmlProc::FPDR_Business(CYWXML_GY &ywxml_gy, UINT8 jzlx, string &strErr)
{
	INT32 ret = XML_SUCCESS;	
	ywxml_gy.m_strID = BUSINESS_YWID_FPDR;
	CFpdr Fpdr(ywxml_gy, jzlx);

	ret = Fpdr.YWXml_Proc(strErr);

	return ret;
}

//分机退回
INT32 CBusinessXmlProc::FJTH_Business(CYWXML_GY &ywxml_gy, UINT8 jzlx, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_FJTH;
	CYWXMLFjth Fjth(ywxml_gy, jzlx);

	ret = Fjth.YWXml_Proc(strErr);
	
	return ret;
}

//变更证书口令
INT32 CBusinessXmlProc::BGZSKL_Business(CYWXML_GY &ywxml_gy, string oldPwd, string newPwd, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_YWID_BGZSKL;
	CBgzskl Bgzskl(ywxml_gy, oldPwd, newPwd);
	
	ret = Bgzskl.YWXml_Proc(strErr);
	
	return ret;
}

//发票补录
INT32 CBusinessXmlProc::FPBL_Business(CYWXML_GY &ywxml_gy, UINT32 SDate, UINT32 EDate, string &strErr)
{
	INT32 ret = XML_SUCCESS;	
	ywxml_gy.m_strID = BUSINESS_YWID_FPBL;
	CFpbl Fpbl(ywxml_gy, SDate, EDate);
	
	ret = Fpbl.YWXml_Proc(strErr);
	
	return ret;
}

//3.1发票上传
INT32 CBusinessXmlProc::NETFPSC_Business(CYWXML_GY &ywxml_gy, const string &Fpmx, UINT32 Fpzs, string &Slxlh, string &strErr)
{	
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_NETID_FPSC;
	CFpsc Fpsc(ywxml_gy, Fpmx, Fpzs, Slxlh);
	
	ret = Fpsc.NETXml_Proc(strErr);

	return ret;
}

//3.2发票上传结果获取
INT32 CBusinessXmlProc::FPSCJGHQ_Business(CYWXML_GY &ywxml_gy, string qtxx, string &Mxjgmw, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_NETID_FPSCJGHQ;
	CFpscjghq Fpscjghq(ywxml_gy, qtxx, Mxjgmw);
	
	ret = Fpscjghq.NETXml_Proc(strErr);
	
	return ret;
}

//3.3网络抄报
INT32 CBusinessXmlProc::WLCB_Business(CYWXML_GY &ywxml_gy, string Sq, string Fphzsj, string Qtxx, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_NETID_WLCB;
	CWlcb Wlcb(ywxml_gy, Sq, Fphzsj, Qtxx);
	
	ret = Wlcb.NETXml_Proc(strErr);

	return ret;
}
	
//3.4清零解锁
INT32 CBusinessXmlProc::QLJS_Business(CYWXML_GY &ywxml_gy, string Qtxx,string &Fpjkmw, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	
	ywxml_gy.m_strID = BUSINESS_NETID_QLJS;
	CQljs Qljs(ywxml_gy, Qtxx, Fpjkmw);
	
	ret = Qljs.NETXml_Proc(strErr);
	
	return ret;

}

//3.5离线信息上传
INT32 CBusinessXmlProc::NETLXXXSC_Business(CYWXML_GY &ywxml_gy, CInvKind *invkind, string strQtxx, string &strLzkzxx, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_NETID_LXXXSC;
	CLxxxsc Lxxxsc(ywxml_gy, *invkind, strQtxx, strLzkzxx);
	
	ret = Lxxxsc.NETXml_Proc(strErr);

	return ret;
}

//3.6网络领取发票
INT32 CBusinessXmlProc::WLLQFP_Business(CYWXML_GY &ywxml_gy, CInvVol *pInvvol, string strQtxx, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_NETID_WLLQFP;
	CWllqfp Wllqfp(ywxml_gy, *pInvvol, strQtxx);
	
	ret = Wllqfp.NETXml_Proc(strErr);

	return ret;
}

//3.7网络领取发票结果确认
INT32 CBusinessXmlProc::WLLQFPJGQR_Business(CYWXML_GY &ywxml_gy, CInvVol *pInvvol, string strQtxx, string &strErr)
{
	INT32 ret = XML_SUCCESS;
	ywxml_gy.m_strID = BUSINESS_NETID_WLLQFPJGQR;
	CWllqfpjgqr Wllqfpjgqr(ywxml_gy, *pInvvol, strQtxx);
	
	ret = Wllqfpjgqr.NETXml_Proc(strErr);
	
	return ret;
}

#endif

