/*! \file    MaintenanceMenu.cpp
   \brief    awe4000r销售程序中系统维护主菜单
   \author   Yu Yan
   \version  1.0
   \date     2008-3-19 
 */

#include "MaintenanceMenu.h"
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "MultiBtnCommonWin.h"
#include "CGlobalArg.h"
#include "CInvServ.h"

CMaintenanceMenu::CMaintenanceMenu():CMultiBtnCommonWin(9, 2)
{
}

int CMaintenanceMenu::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this,MAINTENANCE_MENU);    
    
	CMultiBtnCommonWin::Adjustm_iH(1);
	CMultiBtnCommonWin::Create(iX, iY, iW, iH);
	
	SetTitle(1,"A.机器自检");
	SetTitle(2,"B.机器清库");
	SetTitle(3,"C.修改时钟");
	SetTitle(4,"D.编码下载");
	SetTitle(5,"E.软件升级");
	SetTitle(6,"F.修改密码");
	//SetTitle(7,"G.更新当前号");
//	SetTitle(8,"H.主管密码");
//	SetTitle(9,"I.网络管理");
// 	SetTitle(10,"J.删除缓存");
	SetTitle(7,"G.主管密码");
	SetTitle(8,"H.网络管理");
	SetTitle(9,"I.删除缓存");
	//SetTitle(9,"I.APDU检测");
	
	return 1;
}

//机器自检
void CMaintenanceMenu::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	ChangeWin(SELF_TEST_MENU);
	
}

//机器清库
void CMaintenanceMenu::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	string strErr;
	//判断系统参数，是否进入下一级界面

	strErr = "是否清库？";

	if(YesNoMsBox(strErr)==FAILURE)
	{
		return;
	}
	else
	{
		ChangeWin(CLEAR_DEPOT_WIN);
		return;
	}
}

//修改时钟
void CMaintenanceMenu::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(CHANGE_DATETIME_WIN);		
}

//编码下载
void CMaintenanceMenu::OnButton4(int iEvent, unsigned char * pEventData, int iDataLen)
{
/*	string strErr;
	//判断系统参数，是否进入下一级界面

	strErr = "是否下载机器编码？";

	if(YesNoMsBox(strErr)==FAILURE)
	{
		return;
	}
	else
	{
		//下载机器编码
		if(DownloadFCRCode()==FAILURE)
		{
			CaMsgBox::ShowMsg("下载机器编码失败");
		}
		else
		{
			CaMsgBox::ShowMsg("下载机器编码成功");
		}
		this->ReFresh();
		return;
	} */
		string strErr;
	//判断系统参数，是否进入下一级界面
	CaProgressBar proBar("");
	strErr = "下载机器编码和MAC?";

	if(YesNoMsBox(strErr)==FAILURE)
	{
		return;
	}
	else
	{
		//下载机器编码
		if(DownloadFCRCode()==FAILURE)
		{
			CaMsgBox::ShowMsg("下载机器编码失败");
			this->ReFresh();
     		return;
		}
		else
		{
		//	CaMsgBox::ShowMsg("下载机器编码成功");
			CaProgressBar proBar("");
			proBar.SetText("下载机器编码成功");
	        proBar.ReFresh();
		}
//		this->ReFresh();
//		return;
	}
	

	DBG_PRINT(("下载Mac!"));
	UINT8 nRet = SUCCESS;
	
	INT8 m_MAC[MAC_LEN];//存放mac地址 by yy 20120524
	memset(m_MAC, 0, sizeof(m_MAC));
	nRet = DownloadMAC(m_MAC, MAC_LEN);
	if(SUCCESS != nRet)
	{
		return;
	}
    proBar.SetText("下载成功,请重启");
	proBar.Show();
	while(1);
	return;

	
}

//软件升级
void CMaintenanceMenu::OnButton5(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(SYSTEM_UPGRADE_WIN);	
}


//修改密码
void CMaintenanceMenu::OnButton6(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(PASSWD_EDIT_WIN);	
}

//void CMaintenanceMenu::OnButton7(int iEvent, unsigned char * pEventData, int iDataLen)
//{
//	string strErr = "";
//
//	DBG_PRINT(("g_globalArg->m_curInvVol->m_ieno = %u", g_globalArg->m_curInvVol->m_ieno));
//	DBG_PRINT(("g_globalArg->m_curInvVol->m_remain = %u", g_globalArg->m_curInvVol->m_remain));
//	INT32 nOldCurNo = g_globalArg->m_curInvVol->m_ieno + 1 - 
//		g_globalArg->m_curInvVol->m_remain;
//	DBG_PRINT(("g_globalArg->m_curInvVol->m_ieno = %u", nOldCurNo));
//
//	if (g_globalArg->m_initFlag == 0)
//	{
//		strErr = "机器未初始化";
//		CaMsgBox::ShowMsg(strErr);
//		return ;
//	}
//
//	string strInvNo("");
//	INT32 status;
//	INT8 tmpstr[128];
//	UINT32 curInvNo;
//	CInvVol invVol;
//
//	DBG_ENTER("CDeptEditWin::DoActive()");
//	ReFresh();
//
//	CaMsgBox msgBox("新当前号:",CaMsgBox::MB_INPUT);
//	msgBox.SetInputType(CaInput::aINT);
//	msgBox.SetMaxInputLen(8);
//	while (1)
//	{
//		msgBox.ShowBox();
//		status = msgBox.m_iStatus;
//		strInvNo = (INT8 *)msgBox.GetInputContent();
//		if (status != OK_PRESSED || strInvNo != "")
//			break;
//
//		CaMsgBox::ShowMsg("请输入有效发票号");
//	}
//
//	if (status == OK_PRESSED)
//	{
//		curInvNo  = atoi(strInvNo.c_str());
//		DBG_PRINT(("curInvNo = %u", curInvNo));
//
//		if (curInvNo <= 0)
//		{
//			CaMsgBox::ShowMsg("当前发票号必须大于0");
//			this->ReFresh();
//			return;
//		}
//
//		DBG_PRINT(("m_InvStartNo = %u", g_globalArg->m_curInvVol->m_isno));
//		DBG_PRINT(("m_InvEndNo = %u", g_globalArg->m_curInvVol->m_ieno));
////		if((curInvNo < g_globalArg->m_curInvVol->m_InvStartNo)||
////			((curInvNo - g_globalArg->m_curInvVol->m_InvStartNo) >= MAX_INV_VOL_NUM))
//		if( curInvNo < g_globalArg->m_curInvVol->m_isno || 
//			curInvNo > g_globalArg->m_curInvVol->m_ieno)
//		{
//			CaMsgBox::ShowMsg("当前发票号必须属于当前卷");
//			this->ReFresh();
//			return;
//		}
//
//		DBG_PRINT(("nOldCurNo = u%", nOldCurNo));
//    	DBG_PRINT(("curInvNo = u%", curInvNo));
//		if (curInvNo <= nOldCurNo)
//		{
//			CaMsgBox::ShowMsg("不能向前更新");
//			this->ReFresh();
//			return;
//		}
//
//		//更新INV_VOL中的当前卷信息
//		string sqlstr= "";
//		INT32 errcode;
//		UINT32 nRemain = g_globalArg->m_curInvVol->m_ieno - curInvNo + 1;
//		
//		sprintf(tmpstr,"update INV_VOL set REMAIN = %u where CODE = '%s' and IS_NO = %u",
//			nRemain, g_globalArg->m_curInvVol->m_code.c_str(), 
//			g_globalArg->m_curInvVol->m_isno);
//		sqlstr=tmpstr;
//		invVol.SetSQL(sqlstr);
//		errcode = invVol.ExecuteSQL();
//		if (invVol.ExecuteSQL() != SQLITE_OK)
//		{
//			DBG_PRINT(("更新当前发票号错误 errcode=%d", errcode));
//			CaMsgBox::ShowMsg("更新当前发票号失败");
//			return;
//		}
//		g_globalArg->m_curInvVol->m_remain = nRemain;
//		CaMsgBox::ShowMsg("更新当前发票号成功");		
//	}
//}
void CMaintenanceMenu::OnButton7(int iEvent, unsigned char * pEventData, int iDataLen)
{
	string strMsg("");
	COperator oper;
	INT8 chValue[64];
	memset((void*)chValue, 0, sizeof(chValue));
	
	sprintf(chValue, "where ROLE = %u", DIRECTOR_ROLE);
	oper.m_filter = chValue;
	oper.Requery();
	INT32 errcode = oper.LoadOneRecord();
	if (SQLITE_DONE == errcode)
	{
		strMsg = "无主管员工";
	}
	else if (SQLITE_OK == errcode)
	{
		sprintf(chValue, "密码：%s", oper.m_passwd.c_str());
		strMsg = chValue;		
	}
	else
	{
		strMsg = "查询主管员工失败";	
	}
	CaMsgBox::ShowMsg(strMsg);
	
}

void CMaintenanceMenu::OnButton8(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(NET_SERV_MENU);	
}

void CMaintenanceMenu::OnButton9(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if (g_globalArg->m_initFlag == 0)
	{
		CaMsgBox::ShowMsg("机器未初始化");
		return ;
	}

	CInvServ invServ;
	CInvHead invHead;
	CInvDet  invDet;
	INT32 nErrCode;
	INT8 chValue[128];

	invServ.Requery();
	nErrCode = invServ.LoadOneRecord();
   DBG_PRINT(("nErrCode= %u",nErrCode));
	if( nErrCode != SQLITE_OK )
	{
		CaMsgBox::ShowMsg("无缓存记录");
		return;
	}

	memset(chValue, 0, sizeof(chValue));
	sprintf(chValue,"代码:%s 号码:%u 确认删除?", invServ.m_code.c_str(), invServ.m_InvNo);
	CaMsgBox msgBox(chValue,CaMsgBox::MB_YESNO);
	msgBox.ShowBox();	
	if (msgBox.m_iStatus == OK_PRESSED)
	{
		memset((void*)chValue, 0, sizeof(chValue));
		//sprintf(chValue, "where CODE = '%s' and INV_NO = %u", invServ.m_code.c_str(), invServ.m_InvNo);
		// 更改inv_serv中的两个字段
		sprintf(chValue, "where FPDM = '%s' and FPHM = %u", invServ.m_code.c_str(), invServ.m_InvNo);
		invHead.m_filter = chValue;
		nErrCode = invHead.Delete();
		if (SQLITE_OK != nErrCode)
		{
			CaMsgBox::ShowMsg("删除失败");
			return;
		}
		invDet.m_filter = chValue;
		nErrCode = invDet.Delete();
		if (SQLITE_OK != nErrCode)
		{
			CaMsgBox::ShowMsg("删除失败");
			return;
		}
		
		sprintf(chValue, "where CODE = '%s' and INV_NO = %u", invServ.m_code.c_str(), invServ.m_InvNo);
		invServ.m_filter = chValue;
		nErrCode = invServ.Delete();
		if (SQLITE_OK != nErrCode)
		{
	
			CaMsgBox::ShowMsg("删除失败");
			return;
		}
		CaMsgBox::ShowMsg("删除成功");

		if(0 != g_globalArg->m_InvServNum)
		{
			g_globalArg->m_InvServNum--;
		}
		DBG_PRINT(("m_InvServNum = %u", g_globalArg->m_InvServNum ));
	}
}

int CMaintenanceMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	if (LOCKED_KEY == iEvent || ESC_KEY == iEvent)
	{
		CaMsgBox msgBox("退出当前操作员?",CaMsgBox::MB_YESNO);
		msgBox.ShowBox();	
		if (msgBox.m_iStatus == OK_PRESSED)
		{	
			ChangeWin(LOGIN_WIN);
			return SUCCESS;
		}
	}

#if (0 == RELEASE_VER)
	CInvServ invServ;
	INT32 nErrCode;
	INT8 chValue[64];
	memset((void*)chValue, 0, sizeof(chValue));
	if(DISCOUNT_KEY == iEvent)
	{
		invServ.Requery();
		invServ.LoadOneRecord();

		sprintf(chValue, "where no = %u", invServ.m_no);
		invServ.m_filter = chValue;
		nErrCode = invServ.Delete();
		if (SQLITE_OK != nErrCode)
		{
			CaMsgBox::ShowMsg("删除第一条失败");
			return FAILURE;
		}
		CaMsgBox::ShowMsg("删除第一条成功");
		
		if(0 != g_globalArg->m_InvServNum)
		{
          g_globalArg->m_InvServNum--;
		}
		DBG_PRINT(("m_InvServNum = %u", g_globalArg->m_InvServNum ));
		return SUCCESS;
	}

#endif

	return CMultiBtnCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

