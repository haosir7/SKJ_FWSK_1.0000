
#include "Scanner.h"
#include "CaMsgBox.h"
#include "CaProgressBar.h"
#include "CMainFrame.h"
#include "CGlobalArg.h"
#include "DeptEditWin.h"
#include "arithmetic.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

//构造函数
CDeptEditWin::CDeptEditWin():CMultiInputCommonWin(6)
{
	m_iNumOfInput = 6;
	m_iNumOfBtn = 3;

	m_Dept_edit = &m_dept;		//原本是CPlu
	m_isDeptExist = false;
}

//析构函数
CDeptEditWin::~CDeptEditWin()
{
//	if (m_Dept_edit != NULL)
//	{
//		delete m_Dept_edit;
//		m_Dept_edit = NULL;
//	}
}

//
int CDeptEditWin::Create(int iX,int iY,int iW,int iH)
{
	m_pFrame->RegsiterWin(this, DEPT_EDIT_WIN);    
	CMultiInputCommonWin::Create(iX,iY,iW,iH);

	SetObjPara(1,"商品编码：",	CaInput::aCHAR,GOODS_CODE_LEN);
	SetObjPara(2,"商品名称：",	CaInput::aCHAR,GOODS_NAME_LEN);
	SetObjPara(3,"单价：",		CaInput::aFLOAT,12);	//double?
	SetObjPara(4,"税率：",		CaInput::aFLOAT,4);	//与数据库一致,输入浮点数,如税率为17%,则输入0.17,最多两位小数20150507linzihao
	SetObjPara(5,"单位：",	    CaInput::aCHAR,8);	
	SetObjPara(6,"部类号：",	CaInput::aINT,2);

	SetObjPara(10," 确认 ");
	SetObjPara(11," 删除 ");
	SetObjPara(12," 返回 ");

	OnActive = S_OnActive;
	
	this->ReFresh();

	return 0;
}

int CDeptEditWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	char *content;
	string code("");
	INT8 nLen  = 0;

	switch(iEvent)
	{	   
	case RETURN_MAIN_MENU: 
		ClearDeptWin();
		ChangeWin(PROGRAM_CTRL_MAIN_MENU);						
		return SUCCESS;
		break;		
	case SCANNER_EVENT_KEY:
		content = (char*)ScannerCode();
		
		code = "";
        nLen = GOODS_CODE_LEN - strlen(content);
		if (nLen > 0)//不足16位，则前补零到16位
		{
			while(nLen > 0)
			{
				code += "0";
				nLen--;
			}
		}
		code += content;
		
		m_Dept_edit->m_spbm = code;
		m_pInput1->SetContentBuf((UINT8 *)m_Dept_edit->m_spbm.c_str(), 
			m_Dept_edit->m_spbm.length());
		ReFresh();
		return SUCCESS;		
	default: 
		break;		
	}
   return CMultiInputCommonWin::ProcEvent(iEvent,pEventData,iDataLen);
}

void CDeptEditWin::OnInput1(int iEvent, unsigned char * pEventData, int iDataLen)
{	
	m_pInputList->SetFocusToObj(m_pInput2);
	ReFresh(); 
	string strSpbm = (char *)(m_pInput1->m_contentBuf);
	DBG_PRINT(("strSpbm==%s", strSpbm.c_str()));

	if( strSpbm == DErrorCode() )
	{
		CaMsgBox::ShowMsg("此商品编号不能添加");
		this->ReFresh();
		return;
	}
}

void CDeptEditWin::OnInput2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pInput3);
	ReFresh(); 
}

void CDeptEditWin::OnInput3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pInput4);
	ReFresh(); 
}

void CDeptEditWin::OnInput4(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pInput5);
	ReFresh(); 
}

void CDeptEditWin::OnInput5(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pInput6);
	ReFresh(); 
}

void CDeptEditWin::OnInput6(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_pInputList->SetFocusToObj(m_pBtn1);
	ReFresh(); 
}


void CDeptEditWin::OnBtn1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	int errcode;
	string strErr;
	char sqlbuf[256];
	memset((void*)sqlbuf, 0, sizeof(sqlbuf));
	string sqlstr;	
	DBG_PRINT(("OnBtn1"));
	if (CheckInputValid(strErr) == FAILURE) 
	{
		CaMsgBox::ShowMsg(strErr);
		this->ReFresh();
		return ;
	}

	m_Dept_edit->m_spbm = (char *)(m_pInput1->m_contentBuf);
	DBG_PRINT(("m_Dept_edit->m_spbm==%s", m_Dept_edit->m_spbm.c_str()));
	if( m_Dept_edit->m_spbm == DErrorCode() )
	{
		m_pInputList->SetFocusToObj(m_pInput1);
		CaMsgBox::ShowMsg("此商品编号不能添加");
		this->ReFresh();
		return;
	}
	
	m_Dept_edit->m_spmc = (char *)(m_pInput2->m_contentBuf);
	m_Dept_edit->m_spdj = atof((char *)(m_pInput3->m_contentBuf));
	m_Dept_edit->m_spsl = atof((char *)(m_pInput4->m_contentBuf));
	m_Dept_edit->m_spdw = (char *)(m_pInput5->m_contentBuf);
	m_Dept_edit->m_blh = atoi((char *)(m_pInput6->m_contentBuf));

	DBG_PRINT(("blh = %d",m_Dept_edit->m_blh));

	CDept dept;
	if (0 != m_Dept_edit->m_blh)//部类号不为空不为0
	{ 	
		sprintf(sqlbuf, "where SP_BM <> '%s' and BLH = %u",		
			m_Dept_edit->m_spbm.c_str(), m_Dept_edit->m_blh);
		dept.m_filter = sqlbuf;
		dept.Requery();
		errcode = dept.LoadOneRecord();
		if (SQLITE_OK == errcode)
		{
			memset((void*)sqlbuf, 0, sizeof(sqlbuf));
			sprintf(sqlbuf, "与编码为'%s'部类号重复", dept.m_spbm.c_str());
			m_pInputList->SetFocusToObj(m_pInput6);
			CaMsgBox::ShowMsg(sqlbuf);
			this->ReFresh();
			return ;
		}
		else if(SQLITE_DONE != errcode)
		{
			m_pInputList->SetFocusToObj(m_pInput6);
			CaMsgBox::ShowMsg("查询商品信息表错误");
			this->ReFresh();
			return ;
		}
	}
	CTax temptax;
	memset((void*)sqlbuf, 0, sizeof(sqlbuf));
	sprintf(sqlbuf,"where SL = %u", (UINT32)(m_Dept_edit->m_spsl*DEPT_TAX_EXTENSION));//授权税率表CTax中税率以INT形式保存，与CDept中不一致
	temptax.m_filter = sqlbuf;
	temptax.Requery();
	errcode = temptax.LoadOneRecord();
	if (SQLITE_DONE == errcode)
	{
		m_pInputList->SetFocusToObj(m_pInput4);
		CaMsgBox::ShowMsg("无此授权税率");
		this->ReFresh();
		return ;
	} 
	else if(SQLITE_OK != errcode)
	{
		m_pInputList->SetFocusToObj(m_pInput4);
		CaMsgBox::ShowMsg("查询授权税率表错误");
		this->ReFresh();
		return ;
	}
	else
	{ }

	if (m_isDeptExist == false)
	{
		errcode = m_Dept_edit->AddNew();
		DBG_PRINT(("errorcode=%d", errcode));
		if (errcode == SQLITE_OK)
		{
			CaMsgBox::ShowMsg("商品添加成功");
		}
		else
		{
			CaMsgBox::ShowMsg("商品添加错误！");
		}
	}
	else
	{
		memset((void *)sqlbuf, 0, sizeof(sqlbuf));

	    sprintf(sqlbuf, "update DEPT set SP_MC = '%s', SP_DJ = %f, SP_SL = %f, SP_DW = '%s', BLH = %u where SP_BM = '%s'",
							m_Dept_edit->m_spmc.c_str(),  m_Dept_edit->m_spdj, m_Dept_edit->m_spsl,
							m_Dept_edit->m_spdw.c_str(),  m_Dept_edit->m_blh,
							m_Dept_edit->m_spbm.c_str()
				);
		DBG_PRINT(("sqlbuf = %s", sqlbuf));
		sqlstr=sqlbuf;
		m_Dept_edit->SetSQL(sqlstr);
		errcode = m_Dept_edit->ExecuteSQL();

		if (errcode != SQLITE_OK)
		{
			strErr = "商品修改错误！";			
			return ;
		}		

		CaMsgBox::ShowMsg("商品修改成功");
	}

	ClearDeptWin();
	ChangeWin(PROGRAM_CTRL_MAIN_MENU);
}

void CDeptEditWin::OnBtn2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	INT8 tmpstr[32];
	INT32 res;

	if (m_isDeptExist == true)
	{
		m_Dept_edit->m_spbm = (char *)(m_pInput1->m_contentBuf);
		sprintf(tmpstr, "WHERE SP_BM = '%s'", m_Dept_edit->m_spbm.c_str());
		m_Dept_edit->m_filter.append(tmpstr);
		res = m_Dept_edit->Delete();
		if (res == SQLITE_OK)
		{
			CaMsgBox::ShowMsg("删除成功");
		}
		else
		{
			CaMsgBox::ShowMsg("删除失败");
		}
		DBG_PRINT(("res = %d", res));
	}
	else
	{
		CaMsgBox::ShowMsg("尚无此编号的商品");
	}
	
	ClearDeptWin();
	ChangeWin(PROGRAM_CTRL_MAIN_MENU);
}

void CDeptEditWin::OnBtn3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	ClearDeptWin();
	ChangeWin(PROGRAM_CTRL_MAIN_MENU);
}


UINT8 CDeptEditWin::CheckInputValid(string &strErr)
{
	DBG_PRINT(("CheckInputValid(string &strErr)"));
	//商品编码是否输入正确
	if (m_pInput1->IsEmpty())
	{	
		strErr = "未输入商品编码";
		m_pInputList->SetFocusToObj(m_pInput1);
		return FAILURE;
	}

	//商品名称是否输入正确
	if (m_pInput2->IsEmpty())
	{	
		strErr = "未输入商品名称";
		m_pInputList->SetFocusToObj(m_pInput2);
		return FAILURE;
	}

	//单价输入是否正确
	if (m_pInput3->IsEmpty())
	{	
		strErr = "未输入单价";
		m_pInputList->SetFocusToObj(m_pInput3);
		return FAILURE;
	}
	double dPrice = atof((char *)(m_pInput3->m_contentBuf));
	//if (((UINT64)(PRICE_EXTENSION * dPrice)) > MAX_MONEY ) 
		if (((UINT64)(double2int(dPrice))) > MAX_MONEY_A )
	{
		DBG_PRINT(("dPrice = %f",  dPrice));
		DBG_PRINT(("MAX_MONEY_A = %lld",  MAX_MONEY_A));
		//DBG_PRINT(("PRICE_EXTENSION * dPrice = %lld", PRICE_EXTENSION * dPrice));
		strErr = "单价超限";
		m_pInputList->SetFocusToObj(m_pInput3);
		return FAILURE;		
	}
 	else if (((UINT64)(PRICE_EXTENSION * dPrice)) == 0 )
	{
		strErr = "单价输入为0";
		m_pInputList->SetFocusToObj(m_pInput3);
		return FAILURE;		
	}

	//税率输入是否正确
	if (m_pInput4->IsEmpty())
	{	
		strErr = "未输入税率";
		m_pInputList->SetFocusToObj(m_pInput4);
		return FAILURE;
	}
	double dTax = atof((char *)(m_pInput4->m_contentBuf));
	if ( dTax >= 1.0 )
	{
		DBG_PRINT(("dTax = %f",  dTax));
		strErr = "输入税率不合法";
		m_pInputList->SetFocusToObj(m_pInput4);
		return FAILURE;		
	}

	//部类号输入是否正确
	if (m_pInput6->IsEmpty())
	{	
		strErr = "未输入部类号";
		m_pInputList->SetFocusToObj(m_pInput6);
		return FAILURE;
	}

	if ( 0==atoi((char *)(m_pInput6->m_contentBuf)) )
	{
		strErr = "部类号输入为0";
		m_pInputList->SetFocusToObj(m_pInput6);
		return FAILURE;
	}
	
	return SUCCESS;
}

void CDeptEditWin::DispDeptInfo(CDept *dept)
{
	INT8 tempstr[32];
	
	m_pInput1->SetContentBuf((UINT8 *)dept->m_spbm.c_str(), dept->m_spbm.length());

	m_pInput2->SetContentBuf((UINT8 *)dept->m_spmc.c_str(), dept->m_spmc.length());

	sprintf(tempstr, "%0.2f", dept->m_spdj);
	m_pInput3->SetContentBuf((UINT8 *)tempstr, strlen(tempstr));

	sprintf(tempstr, "%0.2f", dept->m_spsl);
	m_pInput4->SetContentBuf((UINT8 *)tempstr, strlen(tempstr));
	
	m_pInput5->SetContentBuf((UINT8 *)dept->m_spdw.c_str(), dept->m_spdw.length());

	sprintf(tempstr, "%u", dept->m_blh);
	m_pInput6->SetContentBuf((UINT8 *)tempstr, strlen(tempstr));


	m_pInputList->SetFocusToObj(m_pInput2);
}

void CDeptEditWin::ClearDeptWin()
{
	m_pInput1->Clear();
	m_pInput2->Clear();
	m_pInput3->Clear();
	m_pInput4->Clear();
	m_pInput5->Clear();
	m_pInput6->Clear();

	m_pInputList->SetFocusToObj(m_pInput2);
}

int CDeptEditWin::ReFresh()
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

void CDeptEditWin::S_OnActive(CaWindow *obj)
{
	((CDeptEditWin *)obj)->DoActive();
}
/*打开商品管理界面后，首先弹出消息盒子，要求输入商品编码，
 *如果输入编码已存在于数据库，则将该编码对应的商品信息填入Input中(不输入)
 *如果不存在，则询问是否添加该商品，将光标移至input2
*/
void CDeptEditWin::DoActive()
{
	INT8 editFlag = 0;
	UINT8 errorcode;
	INT32 status;
	INT8 tmpstr[64];
	string deptNo("");
	INT32 nLen  = 0;
	string code = "";

	DBG_ENTER("CDeptEditWin::DoActive()");
	ReFresh();

//	ScannerInit();
	CaMsgBox msgBox("商品编码:",CaMsgBox::MB_INPUT);
	msgBox.SetInputType(CaInput::aINT);	
	msgBox.SetMaxInputLen(GOODS_CODE_LEN);
	while (1)
	{
		code = "";
		msgBox.ShowBox();
		status = msgBox.m_iStatus;
		deptNo = (INT8 *)msgBox.GetInputContent();
		if (status != OK_PRESSED || deptNo != "")//能进入这里一定按了OK_PRESSED键
		{
			if (status == OK_PRESSED)
			{
				nLen = GOODS_CODE_LEN - deptNo.length();
				if (nLen > 0)//不足16位，则前补零到16位
				{
					while(nLen > 0)
					{
						code += "0";
						nLen--;
					}
				}		
				code += deptNo;

				if( code == DErrorCode() )
				{
					CaMsgBox::ShowMsg("商品编号不合法");
					this->ReFresh();
					continue;//到这里说明先按了一个不是OK_PRESSED的键，最后按了OK_PRESSED键，导致deptNo非空，但如果code无效，需继续输入
				}
			}
			break;
		}
		CaMsgBox::ShowMsg("请输入商品编码");//只按了OK_PRESSED键
	}
			
	if (status == OK_PRESSED)
	{
		deptNo = code;

		DBG_PRINT(("deptNo==%s", deptNo.c_str()));
		sprintf(tmpstr, "WHERE SP_BM = '%s'", deptNo.c_str());
		m_Dept_edit->m_filter.append(tmpstr);
		m_Dept_edit->Requery();
		errorcode = m_Dept_edit->LoadOneRecord();
		DBG_PRINT(("errorcode = %u", errorcode));

		switch(errorcode)
		{
		case SQLITE_DONE:
			{
				CaMsgBox msgBox2("无此商品信息，是否添加?", CaMsgBox::MB_YESNO);
				msgBox2.ShowBox();
				if (msgBox2.m_iStatus == OK_PRESSED)
				{
					ClearDeptWin();
					m_pInput1->SetContentBuf((UINT8 *)deptNo.c_str(), deptNo.length());
					m_isDeptExist = false;
					editFlag = 1;
				}
				else
				{
					editFlag = 0;
				}
			}
			break;
			
		case SQLITE_OK:
			DispDeptInfo(m_Dept_edit);
			m_isDeptExist = true;
			editFlag = 1;
			break;

		default:
			CaMsgBox::ShowMsg("商品信息表错误");
					editFlag = 0;
			break;
		}
	}
	else
	{
		editFlag = 0;
	}
	
	if (editFlag == 1)
	{
		m_pInputList->SetFocusToObj(m_pInput1);
		m_pInputList->SetFocusToObj(m_pInput2);
		m_pInput1->m_bReadOnly = true;
		ReFresh(); 
	}
	else
	{
		m_pInput1->m_bReadOnly = false;
		ClearDeptWin();
		ChangeWin(PROGRAM_CTRL_MAIN_MENU);
	}
}

string CDeptEditWin::DErrorCode()
{
	string errorcode("");
	for (UINT32 tempi=1; tempi<=GOODS_CODE_LEN; tempi++)
	{
		errorcode +="0";
	}
	return errorcode;
}