#include "comdatatype.h"
#include "SysMacDef.h"
#include "SysArgMac.h"
#include "TemplateDataDesign.h"
#include "TemplateIndep.h"
#include "COperator.h"
#include "TDateTime.h"
#include "CaLabel.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "CaInput.h"
#include "CMainFrame.h"
#include "CGlobalArg.h"

#include "SysArgMenu.h"
#include "SysArgEditWin.h"
#include "SysArgMenu.h"

#include "APIBase.h"

#if TYPE_MODE == ZHQ_MODE
#include "SerialProtocol.h"
#endif

#include "PrintDriver.h"
#include "printer.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

extern struct _Sys_Arg_info sys_arg_info[]; 
extern UINT8 bidirection;

CSysArgEditWin::CSysArgEditWin():CaWindow()
{
	m_sysArg = new CSysArg;
	m_sysarg_info = sys_arg_info;
	m_SysArgID = -1;
	m_nId = 0;
}

CSysArgEditWin::~CSysArgEditWin()
{
	if (m_sysArg != NULL)
	{
		delete m_sysArg;
		m_sysArg = NULL;
	}
}

int CSysArgEditWin::Create(int iX,int iY,int iW,int iH)
{
	DBG_PRINT(("进入CSysArgEditWin::Create函数"));
	
	int curH, lineinterval, left_offset;
	int inputW, leftoffset_btn;
	char title[OBJ_TITLE_MAX_LEN + 1];
    int titleLen=0;
	string errStr;

	m_pFrame->RegsiterWin(this, SYS_ARG_EDIT_WIN);    
	CaWindow::Create(iX,iY,iW,iH); 

	curH = SCREEN_TOP_OFFSET+4;
	lineinterval = LINE_H + 2;
	left_offset = SCREEN_LEFT_OFFSET+8;
	inputW = SCREEN_W - left_offset*2;

	strcpy( title, "" );
	m_pLabel1 = new CaLabel(false,CaObject::ALIGN_LEFT);
	m_pLabel1->Create(left_offset,curH, SCREEN_W, CHAR_H);
	m_pLabel1->SetTitle(title, strlen(title));
	curH += lineinterval;

	strcpy(title, "原 值：");
	m_pLabel2 = new CaLabel(false,CaObject::ALIGN_LEFT);
	m_pLabel2->Create(left_offset,curH, SCREEN_W, CHAR_H);
	m_pLabel2->SetTitle(title, strlen(title));
	curH += lineinterval;

	strcpy(title, "新 值：");
	titleLen = strlen(title) * CHAR_W + 1;
	m_pInput1=new CaInput(CaObject::ON_LEFT, titleLen);
	m_pInput1->Create(left_offset, curH, inputW+1, LINE_H);
	m_pInput1->SetTitle(title, titleLen / CHAR_W);	
	m_pInput1->SetMaxLen(12);
	m_pInput1->m_InputType = CaInput::aCHAR;
	m_pInput1->OnObject = S_OnInput1;
	curH += lineinterval;

 	m_iBtnW = (SCREEN_W - 40)/3 ; //Button的宽度
 	m_iColW = (SCREEN_W - 40)/3; //Button的列宽
	leftoffset_btn = left_offset + 14;
	strcpy(title, "确认");
	m_pBtn1 = new CaButton();
	m_pBtn1->Create(leftoffset_btn, curH, m_iBtnW, WORD_H); 
	m_pBtn1->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn1->SetTitle(title, strlen(title));
	m_pBtn1->BoxType(CaObject::BOX_NO);
	m_pBtn1->OnObject = S_OnButton1;
	
	strcpy(title, "返回");
	m_pBtn2 = new CaButton();
	m_pBtn2->Create(leftoffset_btn+m_iColW, curH, m_iBtnW, WORD_H); 
	m_pBtn2->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn2->SetTitle(title, strlen(title));
	m_pBtn2->BoxType(CaObject::BOX_NO);
	m_pBtn2->OnObject = S_OnButton2;	

	strcpy(title, "默认");
	m_pBtn3 = new CaButton();
	m_pBtn3->Create(leftoffset_btn+m_iColW*2, curH, m_iBtnW, WORD_H); 
	m_pBtn3->SetTitleAlign(CaObject::ALIGN_CENTER);
	m_pBtn3->SetTitle(title, strlen(title));
	m_pBtn3->BoxType(CaObject::BOX_NO);
	m_pBtn3->OnObject = S_OnButton3;	

	OnActive = S_OnActive;
	this->End();

	DBG_PRINT(("退出CSysArgEditWin::Create函数"));
	return 0;
}

int CSysArgEditWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	if (RETURN_MAIN_MENU == iEvent)
	{
		if (g_globalArg->m_operator->m_role == ADMINISTRATOR_ROLE) 
		{
			ChangeWin(SELF_TEST_MENU);
		}
		else
		{
			ChangeWin(SYSTEM_ARG_MENU);
		}
		return SUCCESS;
	}
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

bool CSysArgEditWin::IsCurWin()
{
	return true;
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
int CSysArgEditWin::ReFresh()
{	
	int curH, lineinterval, left_offset;
	int inputW;
    int titleLen=0;
	string errStr;
	char title[OBJ_TITLE_MAX_LEN + 1];
	char str_val[30];
	//获取更改的系统参数ID号
	m_SysArgID =  ((CSysArgMenu *)m_pFrame->GetWin(SYSTEM_ARG_MENU))->m_EditSysArgID;
	m_nId = m_SysArgID;
	DBG_PRINT(("m_SysArgID = %d", m_SysArgID));
	if (m_SysArgID < 0)
		return 0;

	curH = SCREEN_TOP_OFFSET+4;
	lineinterval = LINE_H + 2;
	left_offset = SCREEN_LEFT_OFFSET+8;
	inputW = SCREEN_W - left_offset*2;

	//获得系统参数ID号对应的系统参数表m_sysArg
	if (ReadArgByID(m_SysArgID, errStr) != SUCCESS)
	{
//		this->ShowMsg(errStr);
	}
	//系统参数ID号对应的名字
	strcpy( title, m_sysarg_info[m_nId].name);
	DBG_PRINT(("m_nId = %u", m_nId));
	DBG_PRINT(("m_sysarg_info[m_nId].name = %s", m_sysarg_info[m_nId].name));
	if (m_nId == 3)
	{
		strcpy(title+strlen(title), "(转换器专用串口)");
	}
	m_pLabel1->SetTitle(title, strlen(title));
	curH += lineinterval;

	strcpy(title, "原 值");
	if (*(sys_arg_info[m_nId].item[0])!=0 && m_sysArg->m_vInt<5)
	{
		if (sys_arg_info[m_nId].id == SYS_DEFAULT_PAYER_NAME)
		{
			sprintf(str_val, "：%s", m_sysArg->m_vText.c_str());
		}
		else
		{
			sprintf(str_val, "：%s", m_sysarg_info[m_nId].item[m_sysArg->m_vInt]);
		}
	}
	else
	{
		sprintf(str_val, "：%u", m_sysArg->m_vInt);
	}
	strcpy(title+strlen(title), str_val);
	m_pLabel2->SetTitle(title, strlen(title));
	curH += lineinterval;

	ReFreshTitle();
	CaGroup::ReFresh();

	if(m_pCurObj != NULL )
	{
 		m_pCurObj->SetFocusIn();
	}
	LCDRedraw();

	return 1;
}


void CSysArgEditWin::S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSysArgEditWin *win=(CSysArgEditWin *)obj->GetdWin();
	win->OnInput1(iEvent,pEventData,iDataLen);
}
void CSysArgEditWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSysArgEditWin *win=(CSysArgEditWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}
void CSysArgEditWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSysArgEditWin *win=(CSysArgEditWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}
void CSysArgEditWin::S_OnButton3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CSysArgEditWin *win=(CSysArgEditWin *)obj->GetdWin();
	win->OnButton3(iEvent,pEventData,iDataLen);
}

void CSysArgEditWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	OnDownKey(); //切换焦点到下一个控件
	return;
}

//
void CSysArgEditWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	DBG_PRINT(("进入CSysArgEditWin::OnButton1函数"));
	
	INT8 sqlbuf[128];
	UINT8 errorcode, state=0,tmpid;
	string strErr, sqlstr;
	
	m_sysArg->m_vInt = 0;
	m_sysArg->m_vText = "";
	tmpid = m_SysArgID;

	DBG_PRINT(("m_SysArgID = %d", m_SysArgID));
	state = ReadArgByID(m_SysArgID, strErr);
	
	if (CheckInputValid(strErr) == FAILURE) 
	{
		CaMsgBox::ShowMsg(strErr);
		this->ReFresh();
		return ;
	}

	if (m_SysArgID == SYS_DEFAULT_PAYER_NAME)
	{
		m_sysArg->m_vInt = 0;
		m_sysArg->m_vText = (char *)(m_pInput1->m_contentBuf);
	}
	else
	{
		m_sysArg->m_vInt = atoi((char *)(m_pInput1->m_contentBuf));
	}
	
	if (state != SUCCESS)
	{
		m_sysArg->m_said = m_SysArgID;
		
		errorcode = m_sysArg->AddNew();
		DBG_PRINT(("errorcode=%d", errorcode));
		if (errorcode == SQLITE_OK)
		{
			CaMsgBox::ShowMsg("系统参数添加成功");
		}
		else
		{
			CaMsgBox::ShowMsg("系统参数添加错误！");
		}

	}
	else
	{
		sprintf(sqlbuf, "update SYSARG set V_INT = %u, V_TEXT = '%s' where SA_ID = %d", 
				m_sysArg->m_vInt, m_sysArg->m_vText.c_str(), m_sysArg->m_said);
		DBG_PRINT(("sqlbuf = %s", sqlbuf));
		sqlstr=sqlbuf;
		m_sysArg->SetSQL(sqlstr);
		errorcode = m_sysArg->ExecuteSQL();
		if (errorcode != SQLITE_OK)
		{
			strErr = "系统参数修改错误！";			
			return ;
		}
		
#if TYPE_MODE == ZHQ_MODE
		if (SYS_COM2_BAUD_RATE == m_sysArg->m_said)
		{
			g_globalArg->m_com2BaudRate = m_sysArg->m_vInt;
		
			SerialProtocol* m_serialProtocol;
			m_serialProtocol = SerialProtocol::getInstance();
			INT8 ret = 0;
			ret = m_serialProtocol->ReleasePort();
			if (SUCCESS != ret)
			{
				CaMsgBox::ShowMsg("串口关闭失败, 请重启!");
				
				while(1);
				//return ;
			}
			ret = m_serialProtocol->InitPort(UART_DEV_NAME, g_globalArg->m_com2BaudRate);
			DBG_PRINT(("ret = %d", ret));
			if (ret != SUCCESS)
			{
				CaMsgBox::ShowMsg("重新打开串口失败, 请重启!");	
				while(1);
			}

		}
#endif
		
		CaMsgBox::ShowMsg("系统参数修改成功");
	}

	//-------------------------------------------------------------------
	// 修改g_globalArg中的系统参数变量
	//-------------------------------------------------------------------
	g_globalArg->InitSysArg();

	m_pInput1->Clear();

	char chValue[256];
	if(tmpid == SYS_BIDIRECTION_PRINT)
	{
		PrinterIni(bidirection);
		sprintf(chValue, "当前参数值:%u",bidirection);
		PrintAsciiLine(chValue, strlen(chValue));		
		
		//		sprintf(chValue, "双向打印参数打印:航天信息蜂鸣器正常异常状态 参数%uHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH",bidirection);
		sprintf(chValue, "双向打印参数打印:航天信息蜂鸣器正常异常状态 参数%u",bidirection);
		
		PrintAsciiLine(chValue, strlen(chValue));
		SetLineSpace(0);
		sprintf(chValue, "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH");	
		PrintAsciiLine(chValue, strlen(chValue));
		sprintf(chValue, "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH");
		PrintAsciiLine(chValue, strlen(chValue));		
		CarriageReturn();
		ForwardNLine(REPORT_LINE_SPACE);
	}

	if (g_globalArg->m_operator->m_role == ADMINISTRATOR_ROLE) 
	{
		((CSysArgMenu *)(m_pFrame->GetWin(SYSTEM_ARG_MENU)))->ReFresh();
			ChangeWin(SELF_TEST_MENU);
	}
	else
	{
		((CSysArgMenu *)(m_pFrame->GetWin(SYSTEM_ARG_MENU)))->ReFresh();
		ChangeWin(SYSTEM_ARG_MENU);
	}
	return;
}

void CSysArgEditWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInput1->Clear();

	if (g_globalArg->m_operator->m_role == ADMINISTRATOR_ROLE) 
	{
		ChangeWin(SELF_TEST_MENU);
	}
	else
	{
		ChangeWin(SYSTEM_ARG_MENU);
	}
}
void CSysArgEditWin::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInput1->SetContentBuf( (unsigned char *)(m_sysarg_info[m_nId].default_value), 
								strlen(m_sysarg_info[m_nId].default_value) );
	m_pInput1->ReFresh();
	m_pBtn1->SetFocus();
	this->ReFresh();
}

UINT8 CSysArgEditWin::CheckInputValid(string &strErr)
{	
	INT32 sysValue;
	string sysString("");

	strErr = "";
	
	if (m_pInput1->IsEmpty())
	{	
		strErr = "未输入参数值";
		m_pInput1->SetFocus();
		return FAILURE;
	}

	sysValue = atoi((INT8 *)m_pInput1->m_contentBuf);
	sysString = (INT8 *)m_pInput1->m_contentBuf;
	DBG_PRINT(("sysValue = %d", sysValue));
	//-------------------------------------------------------------------
	// 对输入系统参数进行合法性校验
	//-------------------------------------------------------------------
	switch (m_SysArgID)
	{
// 		case SYS_DECLARE_MEDIUM:
// 			if (sysValue != 0 && sysValue != 1)
// 			{
// 				strErr = "非法申报介质参数";
// 			}
// 			break;
			
		case SYS_BIDIRECTION_PRINT:
			if (sysValue > 9)
			{
				strErr = "非法双向打印参数";
			}
			break;
			
		case SYS_SALE_MODE:	
			if (sysValue > 2 || sysValue < 0 )
			{
				strErr = "非法快捷销售方式";
			}
			break;
		
// 		case SYS_NEED_DECLARE_ALARM:
// 			if (sysValue > 255)
// 			{
// 				strErr = "输入的申报提示天数过大";
// 			}
// 			break;
			
		case SYS_CUR_TEMPLATE_NO:
			if (sysValue > 16)
			{
				strErr = "输入的模板号过大";
			}

			//需保存的模板属性
			if (g_globalArg->ReadPrintTemplate(g_globalArg->m_templateBuffer, 
				(UINT8)sysValue, PRINT_TEMPLATE_BUF_LEN) != SUCCESS)
			{
				strErr = "无此编号的打印模板";
			}
			else
			{
				if (CheckTempValid() != 0)
				{
					strErr = "无效的打印模板";
				}
			}

			if (strErr != "")		//如果设置有问题，则恢复原来的模板
			{
				g_globalArg->ReadPrintTemplate(g_globalArg->m_templateBuffer, 
								(UINT8)m_sysArg->m_vInt, PRINT_TEMPLATE_BUF_LEN);
			}
			g_globalArg->pSaveTemplateInfo = SaveTemplate();
			g_globalArg->pSaveTemplateInfo->workMode = WORK_MODE; 
			g_globalArg->pSaveTemplateInfo->FixedModeFlag = 1; 

			break;
			
		case SYS_DEFAULT_PAYER_NAME:
			if (sysString.length() > 20)
			{
				strErr = "输入名称超长";
			}
			break;
			
		case SYS_COM1_BAUD_RATE:
			if (sysValue != 9600 && sysValue != 19200 && sysValue != 38400 && 
				sysValue != 57600 && sysValue != 115200 )
			{
				strErr = "非法波特率值";
			}
			break;
		case SYS_COM2_BAUD_RATE:
			{
			if (sysValue != 9600 && sysValue != 19200 && sysValue != 38400 && 
				sysValue != 57600 && sysValue != 115200 )
			{
				strErr = "非法波特率值";
			}
	
				if (sysValue != g_globalArg->m_zhqcomBaudRate)
				{
					CaMsgBox msgBox("与转换器波特率不一致，是否仍要更改?",CaMsgBox::MB_YESNO);
					msgBox.ShowBox();
					if (msgBox.m_iStatus != OK_PRESSED)
					{
						this->ReFresh();
						strErr = "放弃更改波特率";
					}

				}
			}
			break;
		case SYS_HW_TYPE:
			if (sysValue > 1) 
			{
				strErr = "无效手写板型号";
			}
			break;
      
		default:
			strErr = "";
	}

	if (strErr == "")
		return SUCCESS;
	else
		return FAILURE;
	
}

void CSysArgEditWin::ClearSysArgEditWin()
{
	m_pInput1->Clear();
	m_pInput1->ReFresh();
	m_pInput1->SetFocus();
	this->ReFresh();

}

int CSysArgEditWin::ReadArgByID(int argID, string &strErr)
{
	char value[30];
	int errorcode;

	memset(value,0,sizeof(value));
	sprintf(value, "WHERE SA_ID = %u", argID);
	m_sysArg->m_filter.append(value);
	m_sysArg->Requery();
	errorcode = m_sysArg->LoadOneRecord();
	DBG_PRINT(("errorcode = %u", errorcode));
	
	switch(errorcode)
	{
	case SQLITE_DONE:
		strErr = "数据库中无该编号的系统参数";
		return FAILURE;
		break;

	case SQLITE_OK:
		break;

	default:
		strErr = "数据库查询错误";
		return FAILURE;
		break;
	}
	
	return SUCCESS;
}

void CSysArgEditWin::S_OnActive(CaWindow *obj)
{
	((CSysArgEditWin *)obj)->DoActive();
}

void CSysArgEditWin::DoActive()
{
	DBG_PRINT((" CCurInvWin::DoActive()!"));
	if (SYS_DEFAULT_PAYER_NAME==m_SysArgID) 
	{
		m_pInput1->m_InputType = CaInput::aCHAR;
	}
	else
	{
		m_pInput1->m_InputType = CaInput::aINT;//只允许输入整数
	}
	m_pInput1->Clear();
//	m_pInput1->SetFocus();	
	ReFresh();
}
