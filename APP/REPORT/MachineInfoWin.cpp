/*! \file    MachineInfo.cpp
   \brief    awe4000r报表打印的机器信息界面
   \author   Yu Yan
   \version  1.0
   \date     2008-4-1
 */

#include "CaMsgBox.h"
#include "CMainFrame.h"
#include "ReportFunc.h"
#include "MachineInfoWin.h"
#include "drv_modem.h"
#include "command_at.h"
#include "CaProgressBar.h"


#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

CMachineInfoWin::CMachineInfoWin():CaWindow()
{
	m_IMEI = "";
}

CMachineInfoWin::~CMachineInfoWin()
{

}

int CMachineInfoWin::Create(int iX, int iY, int iW, int iH)
{
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;

	DBG_PRINT(("CMachineInfoWin::Create()"));

	m_pFrame->RegsiterWin(this, MACHINE_INFO_WIN);    
	CaWindow::Create(iX,iY,iW,iH); // creat a window
	
	curH = SCREEN_TOP_OFFSET;
	m_iButtonW = (SCREEN_W - 32)/2 - 4; //Button的宽度
	m_iButtonH = (SCREEN_W - 32)/2; //Button的列宽
	
	sprintf(title, "机器编号: %s", g_globalArg->m_machine->m_machineNo.c_str());
	m_pLabel1=new CaLabel(false,ALIGN_LEFT);
	m_pLabel1->Create(0, curH, SCREEN_W, CHAR_H);
	m_pLabel1->SetTitle(title,strlen(title));	
	curH += LINE_H;
	
	DBG_PRINT(("title = %s", title));

	sprintf(title, "生产日期: %s", g_globalArg->m_machine->m_pDate.c_str());
	m_pLabel2=new CaLabel(false,ALIGN_LEFT);
	m_pLabel2->Create(0, curH, SCREEN_W, CHAR_H);
	m_pLabel2->SetTitle(title,strlen(title));	
	curH += LINE_H;

	DBG_PRINT(("title = %s", title));
	sprintf(title, "软件版本: %s", g_globalArg->m_machine->m_sVer.c_str());
	m_pLabel3=new CaLabel(false,ALIGN_LEFT);
	m_pLabel3->Create(0, curH, SCREEN_W, CHAR_H);
	m_pLabel3->SetTitle(title,strlen(title));	
	curH += LINE_H;

	DBG_PRINT(("title = %s", title));
	sprintf(title, "硬件版本: %s", g_globalArg->m_machine->m_hVer.c_str());
	m_pLabel4=new CaLabel(false,ALIGN_LEFT);
	m_pLabel4->Create(0, curH, SCREEN_W, CHAR_H);
	m_pLabel4->SetTitle(title,strlen(title));	
	curH += LINE_H;

	INT32 m_iBtnW = (SCREEN_W - 40)/2 - 4; //Button的宽度
 	INT32 m_iColW = (SCREEN_W - 40)/2; //Button的列宽
	INT32 left_offset = SCREEN_LEFT_OFFSET+8;
	INT32 leftoffset_btn = left_offset + 14;

	//two button	
	strcpy(title, "打印");
	m_pButton1 = new CaButton();
	m_pButton1->Create(leftoffset_btn,curH,m_iBtnW,WORD_H); 
	m_pButton1->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pButton1->SetTitle(title, strlen(title));
	m_pButton1->BoxType(CaObject::BOX_NO);
	m_pButton1->OnObject = S_PrintMachineInfo;
	
	strcpy(title, "返回");
	m_pButton2 = new CaButton();
	m_pButton2->Create(leftoffset_btn+m_iColW,curH,m_iBtnW,WORD_H); //m_iColW 未定义，并赋值？？？
	m_pButton2->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pButton2->SetTitle(title, strlen(title));
	m_pButton2->BoxType(CaObject::BOX_NO);
	m_pButton2->OnObject = S_ReturnBack;
	
	OnActive = S_OnActive;
	
	this->End();
	return 0;
}

int CMachineInfoWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
    DBG_PRN("info:",("iEvent = %u", iEvent));
	//返回上一级菜单
	if (RETURN_MAIN_MENU == iEvent)
	{
		ChangeWin(BASIC_REPORT_MENU);
		return SUCCESS;
	}
#if _CONFIG_M2M_MODULE
	if( FIND_KEY == iEvent )
	{
		GetWmmpIMEINo();
	}
#endif
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}


int CMachineInfoWin::ReFresh()
{
	ReFreshTitle();
	CaGroup::ReFresh();
	if(m_pCurObj != NULL )
	{
		m_pCurObj->SetFocusIn();
	}
	LCDRedraw();
	
	return 1;
}

void CMachineInfoWin::S_PrintMachineInfo(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMachineInfoWin *win=(CMachineInfoWin *)obj->GetdWin();
	win->PrintMachineInfo(iEvent,pEventData,iDataLen);
}

void CMachineInfoWin::S_ReturnBack(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMachineInfoWin *win=(CMachineInfoWin *)obj->GetdWin();
	win->ReturnBack(iEvent,pEventData,iDataLen);
}

void CMachineInfoWin::PrintMachineInfo(int iEvent, unsigned char * pEventData, int iDataLen)
{
	string strErr;
	UINT8 ret;
	if (isPaper() != 0)
	{	
		strErr = "打印机未检测到纸";
		CaMsgBox::ShowMsg(strErr);
		this->ReFresh();
		return;
	}
//	strErr = "请确认打印机内放入白纸";
//	if (MsgBox(strErr) == FAILURE)
//	{
//		this->ReFresh();
//		return;	
//	}
	ret = YesNoMsgBox("请放入白纸，是否打印？");
	if (ret==FAILURE)
	{
		return;
	}

	//对打印结构体进行赋值
	m_pMachPrnInfo = &m_machinePrnInfo;
	memset((void *)m_pMachPrnInfo, 0x00, sizeof(struct TMachinePrnData));
	memcpy((void *)m_pMachPrnInfo->MachineNo, (void *)(g_globalArg->m_machine->m_machineNo.c_str()), g_globalArg->m_machine->m_machineNo.length());
	memcpy((void *)m_pMachPrnInfo->HardVer, (void *)(g_globalArg->m_machine->m_hVer.c_str()), g_globalArg->m_machine->m_hVer.length());
	memcpy((void *)m_pMachPrnInfo->SoftVer, (void *)(g_globalArg->m_machine->m_sVer.c_str()), g_globalArg->m_machine->m_sVer.length());
	m_pMachPrnInfo->MakeDate = atoi(g_globalArg->m_machine->m_pDate.c_str());

	if( m_IMEI != "" )
	{
		m_pMachPrnInfo->IMEIFlag = 1;
		memcpy((void *)m_pMachPrnInfo->IMEI, (void *)m_IMEI.c_str(), m_IMEI.length());
	}
	else
		m_pMachPrnInfo->IMEIFlag = 0;
	
	print_macinfo(m_pMachPrnInfo, 0);

	ChangeWin(BASIC_REPORT_MENU);	
}

void CMachineInfoWin::ReturnBack(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeWin(BASIC_REPORT_MENU);
}


void CMachineInfoWin::S_OnActive(CaWindow *obj)
{
	((CMachineInfoWin *)obj)->DoActive();
}


void CMachineInfoWin::DoActive()
{
	DBG_PRINT(("CMachineInfoWin::DoActive()"));

	char title[OBJ_TITLE_MAX_LEN + 1];
   
	sprintf(title, "机器编号: %s", g_globalArg->m_machine->m_machineNo.c_str());
	m_pLabel1->SetTitle(title,strlen(title));	
	sprintf(title, "生产日期: %s", g_globalArg->m_machine->m_pDate.c_str());
	m_pLabel2->SetTitle(title,strlen(title));	
	sprintf(title, "软件版本: %s", g_globalArg->m_machine->m_sVer.c_str());
	m_pLabel3->SetTitle(title,strlen(title));	
	sprintf(title, "硬件版本: %s", g_globalArg->m_machine->m_hVer.c_str());
	m_pLabel4->SetTitle(title,strlen(title));	

	ReFresh();
}

#if _CONFIG_M2M_MODULE
void CMachineInfoWin::GetWmmpIMEINo()
{
//	INT32 rc=0;
//	INT8 title[64];
//	INT8 buffer[64];
//    string strInfo = "";
//
//	if( g_netMode != WMMP_SOCKET )
//	{
//		CaProgressBar proBar("模块信息查询中,请稍后");
//		proBar.ReFresh();
//		modem_init();
//	}
//	
//	//读取IMEI编号
//	memset(title, 0, sizeof(title));
//	memset(buffer, 0, sizeof(buffer));
//	rc = at_getCGSN(buffer);
//	DBG_PRINT(("IMEI：%s", buffer));
//	if (rc != 0)
//	{
//		sprintf(title, "IMEI:获取失败");
//	}
//	else
//	{
//		m_IMEI = buffer;
//		sprintf(title, "IMEI:%s", buffer);
//	}
//	strInfo = title;
//	CaMsgBox::ShowMsg(strInfo);
}
#endif