/**
 @file  NetModelEditWin.cpp
 @brief 界面文件，网络模式选择
*/
#include "CMainFrame.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"

#include "netManager.h"
#include "NetModelEditWin.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


CNetModelEditWin::CNetModelEditWin():CaWindow()
{
	m_pageFlag=1;
}

CNetModelEditWin::~CNetModelEditWin()
{
}


int CNetModelEditWin::Create(int iX,int iY,int iW,int iH)
{
	int curH=0;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;

	INT32 left_offset = SCREEN_LEFT_OFFSET+8;
	INT32 leftoffset_btn = left_offset + 14;
	int inputW = SCREEN_W - left_offset*4;


	m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽
  
	m_pFrame->RegsiterWin(this,NETMODEL_EDIT_WIN);
  
	CaWindow::Create(iX,iY,iW,iH); // creat a window

	//创建一个Input  第一行
	curH = SCREEN_TOP_OFFSET+2;
	strcpy(title, "原 值: ");
	m_pLbl1 = new CaLabel(false,CaObject::ALIGN_LEFT);
	m_pLbl1->Create(left_offset,curH, SCREEN_W, CHAR_H);
	m_pLbl1->SetTitle(title, strlen(title));

	//创建一个Input  第一行
	strcpy(title, "新 值: ");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H+2;
	m_pInput1=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput1->Create(left_offset,curH,inputW+1,LINE_H);
	m_pInput1->SetTitle(title,titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(1);
	m_pInput1->m_InputType = m_pInput1->aINT; //该输入框只接受字符
	m_pInput1->OnObject = S_OnInput1;

	strcpy(title, "1-LAN 2-ADSL 3-3G 4-WMMP 5-APN");
	m_pLbl2 = new CaLabel(false,CaObject::ALIGN_CENTER);
	curH += LINE_H+2;
	m_pLbl2->Create(0,curH, SCREEN_W, CHAR_H);
	m_pLbl2->SetTitle(title, strlen(title));
	
	m_iBtnW = (SCREEN_W - 40)/2 - 4; //Button的宽度
 	m_iColW = (SCREEN_W - 40)/2; //Button的列宽

	//创建一个Button  第五行
	strcpy(title, "修改");
	m_pBtn1 = new CaButton();
	curH += LINE_H+6;
	m_pBtn1->Create(leftoffset_btn,curH,m_iBtnW,WORD_H); 
	m_pBtn1->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn1->SetTitle(title, strlen(title));
	m_pBtn1->BoxType(CaObject::BOX_NO);
	m_pBtn1->OnObject = S_OnButton1;
	
	//创建一个Button  第五行
	strcpy(title, "返回");
	m_pBtn2 = new CaButton();
	m_pBtn2->Create(leftoffset_btn + m_iColW,curH,m_iBtnW,WORD_H); 
	m_pBtn2->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn2->SetTitle(title, strlen(title));
	m_pBtn2->BoxType(CaObject::BOX_NO);
	m_pBtn2->OnObject = S_OnButton2;

	OnActive = S_OnActive;
	
	this->End();  
	return 0;
}

int CNetModelEditWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU: 
		if( m_pageFlag == 1 )
			ChangeWin(NET_SERV_MENU);
		else
			ChangeWin(NET_MANAGE_MENU);						
		return SUCCESS;
		break;		
	default: 
		break;		
	}
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}


int CNetModelEditWin::ReFresh()
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


void CNetModelEditWin::S_OnActive(CaWindow *obj)
{
	((CNetModelEditWin *)obj)->DoActive();
}


void CNetModelEditWin::DoActive()
{
	DispNetModelInfo();
	ReFresh();
}

void CNetModelEditWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CNetModelEditWin *win=(CNetModelEditWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}

void CNetModelEditWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	OnDownKey(); 
	return;		
}

void CNetModelEditWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CNetModelEditWin *win=(CNetModelEditWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}

void CNetModelEditWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CNetModelEditWin *win=(CNetModelEditWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CNetModelEditWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ChangeNetModel();
}

void CNetModelEditWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if( m_pageFlag == 1 )
		ChangeWin(NET_SERV_MENU);
	else
		ChangeWin(NET_MANAGE_MENU);
}

void CNetModelEditWin::DispNetModelInfo()
{
	char chValue[32];


	memset(chValue, 0, sizeof(chValue));
	sprintf(chValue, "原 值: %d", g_netManager->GetConnectionMode());
	m_pLbl1->SetTitle(chValue, strlen(chValue));

	m_pInput1->Clear();
}

void CNetModelEditWin::ChangeNetModel()
{
	int ret=0, flag=0;
	INT8 tmpChar[64];
	string strErr("");
	CaProgressBar proBar("网络模式设置中...");	
	CNetConnection *pnet=NULL;

	int netMode = atoi((char*)(m_pInput1->m_contentBuf));

	proBar.Show();
	if( CheckInputValid(strErr) != SUCCESS )
	{
		CaMsgBox::ShowMsg(strErr);
		return;
	}

	if( isNewModeCanSet(netMode, strErr) != SUCCESS )
	{
		CaMsgBox::ShowMsg(strErr);
		return;
	}

	ret = g_netManager->SetConnectionMode(netMode, &pnet, strErr);
	if (ret != NET_SUCCESS)
	{
		DBG_PRINT(("ret = %d", ret))
		CaMsgBox::ShowMsg(strErr);
		return;
	}
	ret = CNetDBWork::saveNetMode(netMode, strErr);
	if (ret != SQLITE_OK)
	{
		DBG_PRINT(("ret = %d", ret))
		CaMsgBox::ShowMsg(strErr);
		return;
	}
	
	CaMsgBox::ShowMsg("设置网络模式成功");
	ChangeWin(NET_MANAGE_MENU);

	return;
}

UINT8 CNetModelEditWin::CheckInputValid(string &strErr)
{
	int netMode = atoi((char*)(m_pInput1->m_contentBuf));
	
	if( netMode <= NET_NONE || netMode >= NET_VPN )
	{
		strErr = "无此模式";
		return FAILURE;
	}

#if (_CONFIG_LAN_MODULE == 0)
	if (netMode==NET_LAN)
	{
		strErr ="当前版本不支持";
		return FAILURE;
	}
#endif
#if (_CONFIG_ADSL_MODULE == 0)
	if (netMode==NET_ADSL)
	{
		strErr ="当前版本不支持";
		return FAILURE;
	}
#endif
#if (_CONFIG_3G_MODULE == 0)
	if (netMode==NET_3G)
	{
		strErr ="当前版本不支持";
		return FAILURE;
	}
#endif
#if (_CONFIG_M2M_MODULE == 0)
	if (netMode==NET_M2M_WMMP || netMode==NET_M2M_APN)
	{
		strErr ="当前版本不支持";
		return FAILURE;
	}
#endif

	if( netMode == g_netManager->GetConnectionMode() )
	{
		strErr = "当前模式,无需修改";
		return FAILURE;
	}

	return SUCCESS;
}


int CNetModelEditWin::isNewModeCanSet(int netMode, string &strErr)
{
	INT32 dhcp_mode=0;
	CNetDBWork::loadDHCPMode(&dhcp_mode, strErr);

	switch(netMode)
	{
	case NET_M2M_WMMP:	
	case NET_M2M_APN:
		{
			if( dhcp_mode == LAN_DHCP_ON )
			{
				strErr = "当前为动态IP模式,不能切换";
				return FAILURE;
			}
			INT32 m2m_state=0;
			CNetDBWork::load_M2M_State(&m2m_state, strErr);
			if(0 == m2m_state)
			{
				strErr = "请先激活M2M模块";
				return FAILURE;
			}
			break;
		}

	case NET_ADSL:
	case NET_3G:
		{
			if( dhcp_mode == LAN_DHCP_ON )
			{
				strErr = "当前为动态IP模式,不能切换";
				return FAILURE;
			}
			break;
		}

	default:
			break;
	}	
	return SUCCESS;
}



