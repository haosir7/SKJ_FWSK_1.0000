/*! \file    FiscalFun.h
   \brief    几个税控通用函数
   \author   zl
   \version  1.0
   \date     2008-01-01
 */

#include "FiscalFun.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

#include "CInvKind.h"
#include "CUserInfo.h"
#include "CUserMore.h"
#include "CTax.h"
#include "CSysArg.h"
#include "CGlobalArg.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "SysArgMac.h"
#include "TDateTime.h"
#include "ManageBusinessFunc.h"
#include "YWXMLGY.h"
#include "APIBase.h"
#include "YWXmlBase.h"

void FSC_InvDateHex(UINT32 date, UINT32 time, UINT8 *hexInvDate)
{
	UINT8 day = 0;
	UINT8 month = 0;
	UINT16 year = 0;
	UINT8 hour = 0, minute = 0, second = 0;
	UINT32 decInvDate;
	UINT32 temp_year = 0, temp_month = 0, temp_day = 0;
	UINT32 temp_hour = 0, temp_minute = 0, temp_second = 0;
	
	TDateTime Date(date, time);
	Date.DecodeDate(temp_year, temp_month, temp_day);
	year = temp_year;
	month = temp_month;
	day = temp_day;
	
	Date.DecodeTime(temp_hour,temp_minute,temp_second);
	hour = temp_hour;
	minute = temp_minute;
	second = temp_second;
	
	year %= 100;
	decInvDate = year*1048576 + month*65536 + day*2048 + hour*64 + minute;
	
	int2hex(hexInvDate, decInvDate, 4);
}

//---------------------------------------------------------------------------
//初始化入口函数
//---------------------------------------------------------------------------

UINT8 FSC_InitProc(const string &strOldPsw, const string &strNewPsw, string &strErr)
{
	DBG_PRINT(("进入FSC_InitProc函数"));

	UINT8 retcode=0;
	CSysArg sysArg;
	char sqlbuf[256];
	CaProgressBar proBar("");

	//1.获取纳税户信息、金税盘时钟
	proBar.SetText("获取纳税人信息中...");
	proBar.Show();
	retcode = FSC_GetNsrInfo(strErr);
	if (retcode != SUCCESS)
	{
		DBG_PRINT(("纳税人信息初始化失败：%s",strErr.c_str()));
		return FAILURE;
	}
	g_globalArg->m_corpInfo->Requery();
	g_globalArg->m_corpInfo->LoadOneRecord();
	DBG_PRINT(("m_corpInfo->m_Kpjhm : %u", g_globalArg->m_corpInfo->m_Kpjhm));


	//2.获取授权税率
	proBar.SetText("获取授权税率中...");
	proBar.Show();
	retcode = FSC_GetTaxInfo(strErr);
	if (retcode != SUCCESS)
	{
		DBG_PRINT(("授权税率初始化失败：%s",strErr.c_str()));
		return FAILURE;
	}

#if 0
	//3.获取金税盘和报税盘参数信息
	//金税盘参数信息已在FSC_GetNsrInfo()函数中获取，存到了userinfo对象中，
	//如果需要存库，需将这些数据由userinfo转移到usermore类中
	proBar.SetText("获取金税盘、报税盘参数信息中...");
	proBar.Show();
	retcode = FSC_GetJSKInfo(strErr);	
	if (retcode != SUCCESS)
	{
		DBG_PRINT(("盘参数信息初始化失败：%s",strErr.c_str()));
		return FAILURE;
	}
	g_globalArg->m_usermore->Requery();
	g_globalArg->m_usermore->LoadOneRecord();
#endif

	//4.获取金税盘离线控制信息
	proBar.SetText("获取离线控制信息...");
	proBar.Show();
	retcode =FSC_GetInvKindInfo(strErr);
	if (retcode != SUCCESS)
	{
		DBG_PRINT(("离线控制信息初始化失败：%s",strErr.c_str()));
		return FAILURE;
	}
	g_globalArg->m_invKind->Requery();	
	g_globalArg->m_invKind->LoadOneRecord();


	//5.更新初始化日期、初始化标识以及update日期并更新系统参数表Sysarg
	// 	proBar.SetText("更新初始化日期和标识中...");
	// 	proBar.Show();
	DBG_PRINT(("更新初始化时间中..."));
	g_globalArg->m_initDate = TDateTime::CurrentDateTime().FormatInt();
	DBG_PRINT(("m_initDate=%d", g_globalArg->m_initDate));
	memset(sqlbuf,0,sizeof(sqlbuf));
	sprintf(sqlbuf, "update SYSARG set V_INT = %u where SA_ID = %d",
		g_globalArg->m_initDate, SYS_INIT_DATE);
	sysArg.SetSQL(sqlbuf);
	if (sysArg.ExecuteSQL() != SQLITE_OK)
	{
		strErr = "更新初始化时间失败";	
		DBG_PRINT(("更新初始化时间失败"));
		return FAILURE;
	}
	DBG_PRINT(("更新初始化时间成功"));

// 	DBG_PRINT(("更新update时间中..."));
// 	g_globalArg->m_updateDate = g_globalArg->m_initDate;
// 	sprintf(sqlbuf, "update SYSARG set V_INT = %u where SA_ID = %d",
// 		g_globalArg->m_updateDate, SYS_INFO_UPDATE_DATE);
// 	sysArg.SetSQL(sqlbuf);
// 	if (sysArg.ExecuteSQL() != SQLITE_OK)
// 	{
// 		strErr = "更新update时间失败";
// 		DBG_PRINT(("更新update时间失败"));
// 		return FAILURE;
// 	}
// 	DBG_PRINT(("更新update时间成功"));
	
	DBG_PRINT(("更新初始化标识中..."));
	memset(sqlbuf,0,sizeof(sqlbuf));	
	sprintf(sqlbuf, "update SYSARG set V_INT = %u where SA_ID = %d",\
		1, SYS_MACHINE_INIT_FLAG);
	sysArg.SetSQL(sqlbuf);
	if (sysArg.ExecuteSQL() != SQLITE_OK)
	{
		strErr = "更新初始化标识失败";
		DBG_PRINT(("更新初始化标识失败"));
		return FAILURE;
	}
	DBG_PRINT(("更新初始化标识成功"));

	DBG_PRINT(("更新证书口令中..."));
	memset((void*)sqlbuf, 0 ,sizeof(sqlbuf));
	sprintf(sqlbuf, "UPDATE SYSARG SET V_TEXT = '%s' where SA_ID = %d", strNewPsw.c_str(), SYS_CERTIF_PSW);
	sysArg.SetSQL(sqlbuf);
	if (sysArg.ExecuteSQL() != SQLITE_OK)
	{
		strErr = "更新证书口令失败";
		DBG_PRINT(("更新证书口令失败"));
		return FAILURE;
	}
	DBG_PRINT(("更新证书口令成功"));

	//更改金税盘中证书口令
	INT32 retAPI = g_pAPIBase->BGZSKL_API(*g_YwXmlArg, strOldPsw, strNewPsw, strErr);
	if (retAPI != SUCCESS)
	{
		DBG_PRINT(("证书口令更改失败：%s",strErr.c_str()));

		memset(sqlbuf,0,sizeof(sqlbuf));	
		sprintf(sqlbuf, "update SYSARG set V_INT = %u where SA_ID = %d", 0, SYS_MACHINE_INIT_FLAG);
		sysArg.SetSQL(sqlbuf);
		sysArg.ExecuteSQL();
		memset((void*)sqlbuf, 0 ,sizeof(sqlbuf));
		sprintf(sqlbuf, "UPDATE SYSARG SET V_TEXT = '%s' where SA_ID = %d", strOldPsw.c_str(), SYS_CERTIF_PSW);
		sysArg.SetSQL(sqlbuf);
		sysArg.ExecuteSQL();

		return FAILURE;
	}
	DBG_PRINT(("证书口令更改成功"));


	//更新证书口令
	g_globalArg->m_strZskl = strNewPsw;
	g_YwXmlArg->m_zskl = g_globalArg->m_strZskl;
	//更新初始化标志
	g_globalArg->m_initFlag = 1;


	CaMsgBox::ShowMsg("机器初始化成功");

	DBG_PRINT(("退出FSC_InitProc函数"));

	return SUCCESS;
}

UINT8 FSC_GetNsrInfo(string &strErr)
{
	DBG_PRINT(("进入FSC_GetNsrInfo函数"));

	INT32 ret=0;
	CUserInfo userinfo;//纳税人基本信息
	string strCurtime("");//从金税盘获得的当前时间

	//获取纳税人信息&&金税盘时钟
	DBG_PRINT(("获取纳税人信息中..."));

	ret = g_pAPIBase->GetTaxpayerInfo_API(*g_YwXmlArg, userinfo, strCurtime, strErr);

	if (ret != SUCCESS)
	{
		DBG_PRINT(("纳税人信息获取失败"));
	 	return FAILURE;
	}/**/
	DBG_PRINT(("纳税人信息获取成功"));

// 	g_YwXmlArg->m_bspbh = "bspbh4444444444";			//报税盘编号
// 	g_YwXmlArg->m_bspkl = "bspkl12345678";			//报税盘口令

	g_YwXmlArg->m_nsrsbh = userinfo.m_Nsrsbh;		//纳税人识别号
	g_YwXmlArg->m_nsrmc = userinfo.m_Nsrmc;			//纳税人名称
	g_YwXmlArg->m_fplxdm = userinfo.m_fplxdm;		//发票类型代码
	g_YwXmlArg->m_sksbbh =userinfo.m_Jspsbh;		//税控设备编号
	INT8 tempkpjh[8];
	memset(tempkpjh, 0, sizeof(tempkpjh));
	sprintf(tempkpjh, "%u", userinfo.m_Kpjhm);
	g_YwXmlArg->m_kpjh = tempkpjh;		//开票机号

	DBG_PRINT(("g_YwXmlArg->m_nsrsbh : %s", g_YwXmlArg->m_nsrsbh.c_str()));
	DBG_PRINT(("g_YwXmlArg->m_nsrmc : %s", g_YwXmlArg->m_nsrmc.c_str()));
	DBG_PRINT(("g_YwXmlArg->m_fplxdm : %s", g_YwXmlArg->m_fplxdm.c_str()));
	DBG_PRINT(("g_YwXmlArg->m_kpjh : %s", g_YwXmlArg->m_kpjh.c_str()));


	//将纳税户信息存入数据库useinfo表中
	DBG_PRINT(("纳税人信息存储中..."));
	userinfo.Delete();//清空userinfo表
	ret = userinfo.AddNew();	
	if (ret != SQLITE_OK)
	{
		strErr = "纳税人信息存储失败";
		DBG_PRINT(("纳税人信息存储失败"));
		return FAILURE;
	}
	DBG_PRINT(("纳税人信息存储成功"));

	//同步款机时间
	DBG_PRINT(("同步款机时间中..."));
	if(strCurtime.length()>=14)
	{
	string strDate = strCurtime.substr(0,8);
	string strTime = strCurtime.substr(8);
	UINT32 tmpDate = (UINT32)atoi(strDate.c_str());
	UINT32 tmpTime = (UINT32)atoi(strTime.c_str());	
	TDateTime tmpDateTime(tmpDate, tmpTime);
	if ( TDateTime::SetCurrentDateTime(tmpDateTime))
	{	
		DBG_PRINT(("同步款机时间成功"));
	}
	else
	{
		DBG_PRINT(("同步款机时间失败"));
		return FAILURE;
	}
	}
	else
	{
		DBG_PRINT(("获取当前时间不合法"));
		return FAILURE;
	}

	DBG_PRINT(("退出FSC_GetNsrInfo函数"));

	return SUCCESS;			
}

UINT8 FSC_GetTaxInfo(string &strErr)
{	
	DBG_PRINT(("进入FSC_GetTaxInfo函数"));
	
	INT32 ret=0;
	CTax pTax[MAX_TAX_NUM];//授权税率
	UINT8 chTaxNum = MAX_TAX_NUM;//授权税率个数

	DBG_PRINT(("授权税率获取中..."));

	ret = g_pAPIBase->GetTaxRateInfo_API(*g_YwXmlArg, pTax, chTaxNum, strErr);

	if (ret != SUCCESS)
	{
		DBG_PRINT(("授权税率获取失败"));
		return FAILURE;
	}/**/
	DBG_PRINT(("授权税率获取成功"));
	
	//将授权税率存入数据库TAX表中
	DBG_PRINT(("授权税率存储中..."));
	DBG_ASSERT_EXIT((pTax != NULL), (" pTax == NULL!"));
	DBG_PRINT(("chTaxNum= %u",chTaxNum));
	
	pTax[0].Delete();//清空TAX表
    for (int i=0; i<chTaxNum;i++)
	{
		//DBG_PRINT(("&(pTax[%d])= 0x%x", i, &(pTax[i])));
		
		ret= pTax[i].AddNew();
		if (ret != SQLITE_OK)
		{
			strErr = "授权税率存储失败";
			DBG_PRINT(("授权税率存储失败"));
			return FAILURE;
		}	
	}	
	DBG_PRINT(("授权税率存储成功"));

	DBG_PRINT(("退出FSC_GetTaxInfo函数"));
	
	return SUCCESS;			

}

UINT8 FSC_GetInvKindInfo(string &strErr)
{
	DBG_PRINT(("进入FSC_GetInvKindInfo函数"));

	INT32 ret=0;
	CInvKind pInvKind;//离线控制信息

	//获取离线控制信息
	//虽然金税盘中有多种对应不同发票类型的离线信息
	//但每次只根据查询的发票类型代码取出对应的一种离线信息
	//调用该函数前，需明确g_YwXmlArg中的fplxdm
	DBG_PRINT(("离线控制信息获取中..."));

 	ret = g_pAPIBase->GetJKSJ_API(*g_YwXmlArg, pInvKind, strErr);

	if (ret != SUCCESS)
	{
		DBG_PRINT(("金税盘离线控制信息获取失败"));
	 	return FAILURE;
 	}/**/
	DBG_PRINT(("离线控制信息获取成功"));

	DBG_PRINT(("pInvKind.m_fplxdm : %s", pInvKind.m_fplxdm.c_str()));
	DBG_PRINT(("pInvKind.m_bsqsrq  : %s", pInvKind.m_bsqsrq .c_str()));
	DBG_PRINT(("pInvKind.m_bszzrq  : %s", pInvKind.m_bszzrq .c_str()));
// 	DBG_PRINT(("pInvKind.m_maxSign : %lld", pInvKind.m_maxSign));
// 	DBG_PRINT(("pInvKind.m_Lxssr  : %u", pInvKind.m_Lxssr));
// 	DBG_PRINT(("pInvKind.m_Lxkjsj  : %u", pInvKind.m_Lxkjsj));
// 	DBG_PRINT(("pInvKind.m_maxSum  : %lld", pInvKind.m_maxSum));

	//将离线控制信息存入数据库invkind表中
	DBG_PRINT(("离线控制信息存储中..."));

	pInvKind.Delete();//清空invkind表
	ret= pInvKind.AddNew();
	DBG_PRINT(("!!!!!!!!!!!!!!ret = %d", ret));
	if (ret != SQLITE_OK)
	{
		strErr = "离线控制信息存储失败";
		DBG_PRINT(("离线控制信息存储失败"));
		return FAILURE;
	}	
	DBG_PRINT(("离线控制信息存储成功"));
	
	DBG_PRINT(("退出FSC_GetInvKindInfo函数"));

	return SUCCESS;			
}

UINT8 FSC_GetJSKInfo(string &strErr)
{
	INT32 ret=0;
	CUserMore usermore;//纳税人补充信息，存储金税盘和报税盘参数信息
	
	//获取金税盘和报税盘参数信息(包括同步金税盘时钟)
	
	/*INT32 ret=CJSKInfoFunc::GetTaxCardInfo(&m_userInfo,strErr);
	if (ret !=JSK_SUCCESS)
	{
	 	DBG_PRINT(("授权税率存储失败"));
	 	return FAILURE;
	 }*/

	//将盘信息存入数据库usermore表中
	usermore.Delete();//清空usermore表
	ret = usermore.AddNew();	
	if (ret != SQLITE_OK)
	{
		strErr = "盘参数信息存储失败";
		DBG_PRINT(("盘参数信息存储失败"));
		return FAILURE;
	}
	DBG_PRINT(("盘参数信息存储成功"));

	return SUCCESS;	
}

UINT8 FSC_InfoUpdate(string &strErr)
{
	UINT8 retcode=0;
	CSysArg sysArg;
	char sqlbuf[256];

	//1.更新纳税户信息	
	DBG_PRINT(("纳税人信息更新"));	
	CaProgressBar proBar("");
	proBar.SetText("纳税人信息更新中...");
	proBar.Show();
	retcode = FSC_GetNsrInfo(strErr);
	if (retcode != SUCCESS)
	{
		return FAILURE;
	}
	g_globalArg->m_corpInfo->Requery();
	g_globalArg->m_corpInfo->LoadOneRecord();

	//2.更新授权税率
	DBG_PRINT(("授权税率更新"));	
	proBar.SetText("授权税率更新中...");
	proBar.Show();
	retcode = FSC_GetTaxInfo(strErr);
	if (retcode != SUCCESS)
	{
		return FAILURE;
	}

	//3.更新离线控制信息	
	DBG_PRINT(("离线控制信息更新"));	
	proBar.SetText("离线控制信息更新中...");
	proBar.Show();
	retcode =FSC_GetInvKindInfo(strErr);
	if (retcode != SUCCESS)
	{
		return FAILURE;
	}
// 	CaProgressBar proBar3("读票种信息中...");
// 	proBar3.Show();
	g_globalArg->m_invKind->Requery();
	INT32 errorcode = g_globalArg->m_invKind->LoadOneRecord();
	if (SQLITE_OK != errorcode)
	{
		memset((void*)sqlbuf, 0, sizeof(sqlbuf));
		sprintf(sqlbuf, "读票种信息错误：%u", errorcode);
		strErr = sqlbuf;
		return FAILURE;
	}
	DBG_PRINT(("m_invKind->m_maxSum = %lld",g_globalArg->m_invKind->m_maxSum));
	DBG_PRINT(("g_globalArg->m_invKind->m_bsqsrq= %s",g_globalArg->m_invKind->m_bsqsrq.c_str()));

	return SUCCESS;
}

// UINT8 FSC_CheckDeclare(string &strErr)
// {
// 
// 	string curDate ="";
// 	curDate=TDateTime::CurrentDate().FormatString(YYYYMMDD);
// 	DBG_PRINT(("curDate= %s",curDate.c_str()));
// 
// 	string bsqsDate="";
// 	DBG_PRINT(("g_globalArg->m_invKind->m_bsqsrq= %s",g_globalArg->m_invKind->m_bsqsrq.c_str()));
// 	bsqsDate=g_globalArg->m_invKind->m_bsqsrq;
//     DBG_PRINT(("bsqsDate= %s",bsqsDate.c_str()));
//     bsqsDate=g_globalArg->m_invKind->m_bsqsrq.substr(0,8);
//     DBG_PRINT(("bsqsDate= %s",bsqsDate.c_str()));
// 
// 	if (curDate == bsqsDate)
// 	{
// 		strErr ="抄报条件不满足，请改天抄报";
// 		return FAILURE;
// 	}
// 
// 
// 	return SUCCESS;
// }


void FSC_NetDeclare()
{
	string strErr("");
	CaMsgBox msgBox("是否网络抄报?",CaMsgBox::MB_YESNO);
	msgBox.ShowBox();
	if ((msgBox.m_iStatus == NO_PRESSED)||(msgBox.m_iStatus == CANCEL_PRESSED))
	{
		return ;
	}

	CaProgressBar proBar("网络抄报中.....");
	proBar.Show();

	INT32 nRet = g_pAPIBase->NetDeclareProc_API(*g_YwXmlArg, strErr);
	DBG_PRINT(("nRet= %d",nRet));
	
	if (nRet != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
	}
	else
	{
		CaMsgBox::ShowMsg("网络抄报成功");
	}

	return ;
}

void FSC_DiskDeclare(UINT8 uJZlx)
{
	string strMsge("");
	if (1==uJZlx)
	{
		strMsge = "报税盘抄报中....";
	}
	else if (2==uJZlx)
	{
		strMsge = "金税盘抄报中....";
	}
	else
	{
		CaMsgBox::ShowMsg("介质类型错误");
		return;
	}
	
	CaProgressBar proBar("");
	proBar.SetText(strMsge.c_str());
	proBar.Show();
	
    string strErr("");
	INT32 nRet = g_pAPIBase->DeclareProc_API(*g_YwXmlArg,uJZlx, strErr);
	DBG_PRINT(("nRet= %d",nRet));

	if (nRet != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
	}
	else
	{
		CaMsgBox::ShowMsg("介质抄税成功");
	}

	return ;

}

void FSC_NetUpdateTax()
{
	string strErr("");
	CaMsgBox msgBox("是否网络清卡?",CaMsgBox::MB_YESNO);
	msgBox.ShowBox();
	if ((msgBox.m_iStatus == NO_PRESSED)||(msgBox.m_iStatus == CANCEL_PRESSED))
	{
		return ;
	}
	
	CaProgressBar proBar("网络清卡中.....");
	proBar.Show();
	
	INT32 nRet = g_pAPIBase->NetUpdateTaxProc_API(*g_YwXmlArg, strErr);
	if (nRet != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
	}
	else
	{
		CaMsgBox::ShowMsg("网络清卡成功");
	}
	
	return ;
}

void FSC_DiskUpdateTax()
{
	string strErr("");
	CaMsgBox msgBox("是否清零解锁?",CaMsgBox::MB_YESNO);
	msgBox.ShowBox();
	if ((msgBox.m_iStatus == NO_PRESSED)||(msgBox.m_iStatus == CANCEL_PRESSED))
	{
		return ;
	}
	
	CaProgressBar proBar("报税盘清零解锁中.....");
	proBar.Show();
	
	INT32 nRet = g_pAPIBase->UpdateTaxProc_API(*g_YwXmlArg, strErr);

	if (nRet != SUCCESS)
	{
		CaMsgBox::ShowMsg(strErr);
	}
	else
	{
		CaMsgBox::ShowMsg("清零解锁成功");
	}
	
	return ;
}

UINT8 FSC_ChangeDiskPsw(string strOldPsw, string strNewPsw,string &strErr)
{
	CSysArg sysArg;
	INT8 chValue[128];
	memset((void*)chValue, 0 ,sizeof(chValue));

	sprintf(chValue, "where SA_ID = %d", SYS_DISK_PSW);
	sysArg.m_filter.append(chValue);
	sysArg.Requery();
	sysArg.LoadOneRecord();

	if (sysArg.m_vText != strOldPsw)
	{
		strErr="旧口令输入错误";
	    return FAILURE;
	}

	CaProgressBar proBar("金税盘口令修改中.....");
	proBar.Show();

	UINT32 ret = 0; 
	ret = g_pAPIBase->SKPKLBG_API(*g_YwXmlArg, strOldPsw, strNewPsw, strErr);
	if (ret != SUCCESS)
	{
		memset((void*)chValue, 0 ,sizeof(chValue));
		sprintf(chValue, "UPDATE SYSARG SET V_TEXT = '%s' where SA_ID = %d", strOldPsw.c_str(), SYS_DISK_PSW);
		sysArg.SetSQL(chValue);
		sysArg.ExecuteSQL();

		return FAILURE;
	}

	memset((void*)chValue, 0 ,sizeof(chValue));
	sprintf(chValue, "UPDATE SYSARG SET V_TEXT = '%s' where SA_ID = %d", strNewPsw.c_str(), SYS_DISK_PSW);
	sysArg.SetSQL(chValue);
	INT32 nRetCode = sysArg.ExecuteSQL();
	if (SQLITE_OK != nRetCode)
	{
		strErr="修改口令错误，更新数据库错误";
		return FAILURE;
	}

	CaMsgBox::ShowMsg("金税盘口令修改成功");

	return SUCCESS;
}


UINT8 FSC_FpblProc(UINT32 SDate, UINT32 EDate, string &strErr)
{
	DBG_PRINT(("进入FSC_FpblProc函数"));
	
	INT32 ret=0;

	DBG_PRINT(("发票补录中..."));
	
	ret = g_pAPIBase->Fpbl_API(*g_YwXmlArg, SDate, EDate, strErr);
	
	if (ret != SUCCESS)
	{
		DBG_PRINT(("发票补录失败"));
		return FAILURE;
	}/**/
	DBG_PRINT(("发票补录成功"));
	return SUCCESS;
}