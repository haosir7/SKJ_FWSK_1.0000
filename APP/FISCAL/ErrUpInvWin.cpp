/*! \file    ErrUpInvWin.CPP
   \brief    awe4000r获取上传出错发票信息界面
   \author   Yu Yan
   \version  1.0
   \date     2008-2-22
 */


#include "ErrUpInvWin.h"
#include "APIBase.h"
#include "TemplateGeneralFuc.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include <string>

#define  ERR_MES_MAX_LEN	54	//错误信息描述最大字节数
#define	 LINE_ERR_MES_LEN	22	//第三行显示的最大字节数


CErrInvUpWin::CErrInvUpWin():CaWindow()
{
	m_pageIndex = 1;
	m_pageNum = 0;
}

CErrInvUpWin::~CErrInvUpWin()
{

}

int CErrInvUpWin::Create(int iX,int iY,int iW,int iH)
{
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;
	

    m_iBtnW = (SCREEN_W/2 - 0) ; //控件的宽度
	m_iColW = m_iBtnW + 0;        //控件的列宽
  
	m_pFrame->RegsiterWin(this,ERR_UP_INV_WIN);
  
	CaWindow::Create(iX,iY,iW,iH); // creat a window

	//创建一个标签  第一行	
	label[0] = new CaLabel(false,CaObject::ALIGN_RIGHT);
	curH = SCREEN_TOP_OFFSET;
	label[0]->Create(0,curH, SCREEN_W, CHAR_H); //参数是坐标
	label[0]->SetTitle(title_array[0], strlen(title_array[0]));
  
	//创建一个标签  第二行
	label[1] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[1]->Create(0,curH, SCREEN_W, CHAR_H); //参数是坐标
	label[1]->SetTitle(title_array[1], strlen(title_array[1]));

	//创建一个标签  第三行
	label[2] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[2]->Create(0,curH, SCREEN_W, CHAR_H);
	label[2]->SetTitle(title_array[2], strlen(title_array[2]));

	//创建一个标签 第三行
	label[3] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[3]->Create(0,curH, SCREEN_W, CHAR_H);
	label[3]->SetTitle(title_array[3], strlen(title_array[3]));
	
	//创建一个标签  第四行
	label[4] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[4]->Create(0,curH, SCREEN_W, CHAR_H);
	label[4]->SetTitle(title_array[4], strlen(title_array[4]));

	OnActive = S_OnActive;
	
	this->End();  
	return 0;
}

int CErrInvUpWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
    UINT8 ret = 0;
	UINT32 ii;
	char *content;

	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU:  //return to SYSTEMMENU
			 ChangeWin(FISCAL_MAIN_MENU);			
	
		 return SUCCESS;
	 	 break;

	case PAGEUP_KEY:

		 DBG_PRINT(("PAGEUP_KEY"));
		 if ((m_pageIndex-1)<1)
		 {
			 return FAILURE;
		 }
		 m_pageIndex--;
		 QueryShow(m_InvUpFailInfo, m_pageIndex);
		 ChangeTitle();
	     ReFresh();	
		 DBG_PRINT(("PAGEUP_KEY"));
		 return SUCCESS;
		 break;

	case PAGEDOWN_KEY:
		 if ((m_pageIndex+1)>m_pageNum)
		 {
			 return FAILURE;
		 }
		 m_pageIndex++;
		 QueryShow(m_InvUpFailInfo, m_pageIndex);
		 ChangeTitle();
	     ReFresh();	
		 return SUCCESS;
		 break;

	default: break;		
	}
 	  
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

int CErrInvUpWin::ReFresh()
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

UINT8 CErrInvUpWin::ErrMsgBox(UINT8 ret)
{
	string pText;
	switch(ret)
	{
		case PRE_INV_ERROR:
		    pText = "查询上笔交易失败";
			break;
		case NO_DIST_INV:
		    pText = "当前0卷发票未导入";
			break;
		case QUERY_ERROR:
		    pText = "查询失败";
			break;
		default:;
	}  

	CaMsgBox::ShowMsg(pText);
	return FAILURE;	
}




void CErrInvUpWin::S_OnActive(CaWindow *obj)
{
	((CErrInvUpWin *)obj)->DoActive();
}


void CErrInvUpWin::DoActive()
{
	DBG_PRINT((" CErrInvUpWin::DoActive()!"));
	m_pageIndex = 1;
	string StrErr = "";
	INT32 ret;
	ret = g_pAPIBase->InvUpFailInfo_API(*g_YwXmlArg, m_InvUpFailInfo, m_pageNum, StrErr);//获得未导入的发票卷数
 	DBG_PRINT((" m_pageNum = %u!", m_pageNum));
	if (SUCCESS != ret)
 	{
		m_pageIndex = 1;
		m_pageNum = 0;
		CaMsgBox::ShowMsg(StrErr);
		ChangeWin(FISCAL_MAIN_MENU);
	}
	else
	{
		if(m_pageNum <= 0)
		{
			CaMsgBox::ShowMsg("无上传日志信息");
			ChangeWin(FISCAL_MAIN_MENU);
			return;
		}
		QueryShow(m_InvUpFailInfo, m_pageIndex);//组织屏幕显示信息
		ChangeTitle();
		ReFresh();	
	}	
}

UINT8 CErrInvUpWin::ChangeTitle()
{
	label[0]->SetTitle(title_array[0], strlen(title_array[0]));
	label[1]->SetTitle(title_array[1], strlen(title_array[1]));
	label[2]->SetTitle(title_array[2], strlen(title_array[2]));
	label[3]->SetTitle(title_array[3], strlen(title_array[3]));
    label[4]->SetTitle(title_array[4], strlen(title_array[4]));  
	return SUCCESS;
}

//--------------------------------------------------------------------------------------------------------------------
//获取未导入发票卷信息，并修改屏幕显示
//--------------------------------------------------------------------------------------------------------------------
UINT8 CErrInvUpWin::QueryShow(CDataInvServ *pInvUpFailInfo, UINT32 nPageIndex)
{
	DBG_ASSERT_EXIT((pInvUpFailInfo != NULL), (" pInvUpFailInfo == NULL!"));
	
	UINT8 ret = SUCCESS;
	UINT8 MesLen = 0;
	string TempErr = "";
	UINT8 buff[LINE_ERR_MES_LEN+1];

	DBG_PRINT((" nPageIndex == %u", nPageIndex));
	//页码序号超限
	if( (nPageIndex<1) || (nPageIndex>m_pageNum) )
	{
		return FAILURE;
	}
	
 	sprintf(title_array[0], "ESC键返回      [页%u/%u]", nPageIndex, m_pageNum);
 	sprintf(title_array[1], "发票代码: %s", pInvUpFailInfo[nPageIndex-1].m_fpdm.c_str());
 	sprintf(title_array[2], "发票号码: %08u", pInvUpFailInfo[nPageIndex-1].m_fphm);

	DBG_PRINT((" pInvUpFailInfo[%u].m_ErrInfo.length() == %u", nPageIndex-1, pInvUpFailInfo[nPageIndex-1].m_errMsg.length()));
	MesLen = pInvUpFailInfo[nPageIndex-1].m_errMsg.length();
	if (MesLen > ERR_MES_MAX_LEN)
	{
			MesLen = ERR_MES_MAX_LEN;
	}

	if (MesLen <= LINE_ERR_MES_LEN)
 	{
		sprintf(title_array[3], "错误描述: %s", pInvUpFailInfo[nPageIndex-1].m_errMsg.c_str());
 	}
 	else
 	{	
		TempErr = pInvUpFailInfo[nPageIndex-1].m_errMsg.substr(0, LINE_ERR_MES_LEN);
		DBG_PRINT(("截取的字符串为： %s", TempErr.c_str()));

		memset(buff, 0, sizeof(buff));
		sprintf((char *)buff, "%s", TempErr.c_str());
		if( GetHalfHZCount(buff, LINE_ERR_MES_LEN)%2 == 0 )
		{
			sprintf(title_array[3], "错误描述: %s",TempErr.c_str());
			TempErr = pInvUpFailInfo[nPageIndex-1].m_errMsg.substr(LINE_ERR_MES_LEN, MesLen - LINE_ERR_MES_LEN);
			sprintf(title_array[4], "%s", TempErr.c_str());
		}
		else
		{
			TempErr = pInvUpFailInfo[nPageIndex-1].m_errMsg.substr(0, LINE_ERR_MES_LEN - 1);
			DBG_PRINT(("截取的字符串为： %s", TempErr.c_str()));
			sprintf(title_array[3], "错误描述: %s",TempErr.c_str());

			if (MesLen ==  ERR_MES_MAX_LEN)
			{
				MesLen -= 1;
				TempErr = pInvUpFailInfo[nPageIndex-1].m_errMsg.substr(LINE_ERR_MES_LEN - 1, MesLen - LINE_ERR_MES_LEN);
			}

			TempErr = pInvUpFailInfo[nPageIndex-1].m_errMsg.substr(LINE_ERR_MES_LEN - 1, MesLen - LINE_ERR_MES_LEN + 1);
			
			sprintf(title_array[4], "%s", TempErr.c_str());
		}

 	}

	return ret;
}











