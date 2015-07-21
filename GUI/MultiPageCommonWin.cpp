#include "MultiPageCommonWin.h"
#include "CaMsgBox.h"
#include "stdio.h"

/*!
@param type MULTI_LABELS_MULTI_PAGE或MULTI_BUTTONS_MULTI_PAGE
@param titles 存放labels 或者buttons 的titles
@param size 公有多少行
@param nextWinId 转向窗口的id
*/
CMultiPageCommonWin::CMultiPageCommonWin(int type)
{
	DBG_ASSERT_EXIT(type >=0 && type <=1, 
		("CMultiPageCommonWin construction failed,param is invalid"));
	
	m_pStrList = NULL;
	m_iCurPageNum = 1;
	m_iTotalPage = 1;
	m_iTotalLine = 0;
	m_iCurWinLastComIndex = 0;
	m_iPressedBtnIndex = 0;	
	m_iComType = type;
	
	m_pBtn1 = NULL;
	m_pBtn2 = NULL;
	m_pBtn3 = NULL;
	m_pBtn4 = NULL;
	m_pBtn5 = NULL;

	m_pLabel1 = NULL;
	m_pLabel2 = NULL;
	m_pLabel3 = NULL;
	m_pLabel4 = NULL;
	m_pLabel5 = NULL;
}

int CMultiPageCommonWin::Create(int iX,int iY,int iW,int iH)
{	
 	int curH = SCREEN_TOP_OFFSET;
 	CaWindow::Create(iX , iY, iW , iH);

	if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
	{
		m_pBtn1 = new CaButton;
		m_pBtn1->Create(SCREEN_LEFT_OFFSET,curH, SCREEN_W - SCREEN_LEFT_OFFSET, WORD_H);
		m_pBtn1->BoxType(CaObject::BOX_NO);		
		m_pBtn1->SetTitleAlign(ALIGN_LEFT);
		m_pBtn1->OnObject = S_OnButton1;
		curH += LINE_H;

		m_pBtn2 = new CaButton;
		m_pBtn2->Create(SCREEN_LEFT_OFFSET,curH, SCREEN_W - SCREEN_LEFT_OFFSET, WORD_H);
		m_pBtn2->BoxType(CaObject::BOX_NO);
		m_pBtn2->SetTitleAlign(ALIGN_LEFT);
		m_pBtn2->OnObject = S_OnButton2;
		curH += LINE_H;

		m_pBtn3 = new CaButton;
		m_pBtn3->Create(SCREEN_LEFT_OFFSET,curH, SCREEN_W - SCREEN_LEFT_OFFSET, WORD_H);
		m_pBtn3->BoxType(CaObject::BOX_NO);
		m_pBtn3->SetTitleAlign(ALIGN_LEFT);
		m_pBtn3->OnObject = S_OnButton3;
		curH += LINE_H;

		m_pBtn4 = new CaButton;
		m_pBtn4->Create(SCREEN_LEFT_OFFSET,curH, SCREEN_W - SCREEN_LEFT_OFFSET, WORD_H);
		m_pBtn4->BoxType(CaObject::BOX_NO);
		m_pBtn4->SetTitleAlign(ALIGN_LEFT);
		m_pBtn4->OnObject = S_OnButton4;
		curH += LINE_H;		

		m_pBtn5 = new CaButton;
		m_pBtn5->Create(SCREEN_LEFT_OFFSET,curH, SCREEN_W - SCREEN_LEFT_OFFSET, WORD_H);
		m_pBtn5->BoxType(CaObject::BOX_NO);
		m_pBtn5->SetTitleAlign(ALIGN_LEFT);
		m_pBtn5->OnObject = S_OnButton5;
		curH += LINE_H;		
	}
	else if (m_iComType == MULTI_LABELS_MULTI_PAGE)
	{
		m_pLabel1 = new CaLabel(false,CaObject::ALIGN_LEFT);
		m_pLabel1->Create(SCREEN_LEFT_OFFSET,curH, SCREEN_W - SCREEN_LEFT_OFFSET, CHAR_H);
		curH += LINE_H;

		m_pLabel2 = new CaLabel(false,CaObject::ALIGN_LEFT);
		m_pLabel2->Create(SCREEN_LEFT_OFFSET,curH, SCREEN_W - SCREEN_LEFT_OFFSET, CHAR_H);
		curH += LINE_H;

		m_pLabel3 = new CaLabel(false,CaObject::ALIGN_LEFT);
		m_pLabel3->Create(SCREEN_LEFT_OFFSET,curH, SCREEN_W - SCREEN_LEFT_OFFSET, CHAR_H);
		curH += LINE_H;

		m_pLabel4 = new CaLabel(false,CaObject::ALIGN_LEFT);
		m_pLabel4->Create(SCREEN_LEFT_OFFSET,curH, SCREEN_W - SCREEN_LEFT_OFFSET, CHAR_H);
		curH += LINE_H;

		m_pLabel5 = new CaLabel(false,CaObject::ALIGN_LEFT);
		m_pLabel5->Create(SCREEN_LEFT_OFFSET,curH, SCREEN_W - SCREEN_LEFT_OFFSET, CHAR_H);
		curH += LINE_H;
	}

	m_pPageLabel = new CaLabel(false,CaObject::ALIGN_RIGHT);
	m_pPageLabel->Create(PAGE_NUM_X,PAGE_NUM_Y, PAGE_NUM_W, CHAR_H);	
	m_pPageLabel->SetTitleAlign(ALIGN_RIGHT);

	m_pPageLabel_2= new CaLabel(false,CaObject::ALIGN_RIGHT);
	m_pPageLabel_2->Create(PAGE_NUM_X_2,PAGE_NUM_Y_2, PAGE_NUM_W, CHAR_H);	
	m_pPageLabel_2->SetTitleAlign(ALIGN_RIGHT);

	SetPageNumVisible(false);
	SetPageNumVisible_2(false);
	ResetVisiable(0);

	this->End();
	return 1;
}
int CMultiPageCommonWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	int indexOld, indexNew, res;
	if (iEvent == PAGEDOWN_KEY && m_iCurPageNum < m_iTotalPage)
	{
		GetNextPage();
	}
	else if(iEvent == PAGEUP_KEY && m_iCurPageNum > 1)
	{
		GetPrePage();
	}
	else if(iEvent == RETURN_MAIN_MENU)//浏览完按下一步键，进行的处理
	{
		DBG_PRN("info",("quit!!!!!!!!!!!!!!!!!!!"));		
		EndProcess();
	}
	indexOld = GetFocusIndex(); 
	res =  CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
	indexNew = GetFocusIndex();
	if (indexNew >= 0 && indexNew != indexOld)
	{
		if ((indexNew > 0) && ((indexNew % PAGE_LINE_NUM) == 4))//选中了第五个按钮
		{
			SetPageNumVisible_2(true);//显示备用页码
		}
		else
		{
			SetPageNumVisible_2(false);//不显示备用页码
		}
		ReFresh();
	}
	
	return res;
}

void CMultiPageCommonWin::SetTitles(char ** titles, int totalLine)
{
	DBG_ASSERT_EXIT(totalLine > 0,("行数不能小于零"));
	m_iTotalPage = (totalLine - 1) / PAGE_LINE_NUM + 1;
	m_iTotalLine = totalLine;

	m_iCurPageNum = 1;
	m_iPressedBtnIndex = 0;

	if (titles == NULL)
	{
		return;
	}

	if (totalLine > PAGE_LINE_NUM)
	{
		m_iCurWinLastComIndex = PAGE_LINE_NUM;
	}
	else
	{
		m_iCurWinLastComIndex = totalLine;
	}
	
	ResetVisiable(m_iCurWinLastComIndex);
	ClearListBuff();
	m_pStrList = new char *[m_iTotalLine];
	for (int i = 0; i < m_iTotalLine; i ++)
	{
		m_pStrList[i] = new char[OBJ_TITLE_MAX_LEN +1];
		DBG_ASSERT_EXIT(strlen(titles[i]) <= OBJ_TITLE_MAX_LEN,("title is too long!"));
		//strncpy(m_pStrList[i],titles[i],strlen(titles[i]) + 1);
		strcpy(m_pStrList[i],titles[i]);
	}

	if (m_iComType == MULTI_LABELS_MULTI_PAGE)
	{
		m_pLabel1->SetTitle(m_pStrList[0],strlen(m_pStrList[0]));
	}
	else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
	{
		DBG_PRN("info",("m_pStrList[0] = %s",m_pStrList[0]));
// 		m_pBtn1->m_iW = strlen(m_pStrList[0])*CHAR_W;
		m_pBtn1->SetTitle(m_pStrList[0],strlen(m_pStrList[0]));	
		m_pBtn1->SetFocus();
	}

	if (totalLine > 1)
	{
		if (m_iComType == MULTI_LABELS_MULTI_PAGE)
		{
			m_pLabel2->SetTitle(m_pStrList[1],strlen(m_pStrList[1]));
		}
		else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
		{
// 			m_pBtn2->m_iW =  strlen(m_pStrList[1])*CHAR_W;
			m_pBtn2->SetTitle(m_pStrList[1],strlen(m_pStrList[1]));
		}
	}

	if (totalLine > 2)
	{
		if (m_iComType == MULTI_LABELS_MULTI_PAGE)
		{
			m_pLabel3->SetTitle(m_pStrList[2],strlen(m_pStrList[2]));
		}
		else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
		{
// 			m_pBtn3->m_iW = strlen(m_pStrList[2])*CHAR_W;
			m_pBtn3->SetTitle(m_pStrList[2],strlen(m_pStrList[2]));
		}
	}

	if (totalLine > 3)
	{
		if (m_iComType == MULTI_LABELS_MULTI_PAGE)
		{
			m_pLabel4->SetTitle(m_pStrList[3],strlen(m_pStrList[3]));
		}
		else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
		{
// 			m_pBtn4->m_iW = strlen(m_pStrList[3])*CHAR_W;
			m_pBtn4->SetTitle(m_pStrList[3],strlen(m_pStrList[3]));
		}
	}

	if (totalLine > 4)
	{
		if (m_iComType == MULTI_LABELS_MULTI_PAGE)
		{
			m_pLabel5->SetTitle(m_pStrList[4],strlen(m_pStrList[4]));
		}
		else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
		{
			m_pBtn5->SetTitle(m_pStrList[4],strlen(m_pStrList[4]));
		}
	}
}

void CMultiPageCommonWin::InitFirstPage(int totalLine, int curPageNum, int startIndex)
{
	int disLineNum;
	DBG_ASSERT_EXIT(totalLine > 0 && m_pStrList != NULL && startIndex < totalLine,("行数不能小于零,且指针不能为空"));
	m_iTotalPage = (totalLine - 1) / PAGE_LINE_NUM + 1;
	m_iTotalLine = totalLine;

	m_iCurPageNum = curPageNum;
	m_iPressedBtnIndex = 0;

	if (m_pStrList == NULL)
	{
		return;
	}

	if (curPageNum < m_iTotalPage)
	{
		m_iCurWinLastComIndex = PAGE_LINE_NUM*curPageNum;
	}
	else
	{
		/*m_iCurWinLastComIndex = m_iTotalLine - (curPageNum - 1) * 4;*/
		m_iCurWinLastComIndex = m_iTotalLine;
	}
	disLineNum = m_iCurWinLastComIndex % PAGE_LINE_NUM;
	if (disLineNum == 0)
	{
		disLineNum = PAGE_LINE_NUM;
	}
	ResetVisiable(disLineNum);

	if (m_iComType == MULTI_LABELS_MULTI_PAGE)
	{
		m_pLabel1->SetTitle(m_pStrList[startIndex],strlen(m_pStrList[startIndex]));
	}
	else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
	{
// 		m_pBtn1->m_iW = strlen(m_pStrList[0])*CHAR_W;
		m_pBtn1->SetTitle(m_pStrList[startIndex],strlen(m_pStrList[startIndex]));	
		m_pBtn1->SetFocus();
	}
	DBG_PRN("info", ("m_pStrList[%d] = %s",startIndex, m_pStrList[startIndex]));
	if ((totalLine > 1) && (startIndex + 1 < totalLine))
	{
		if (m_iComType == MULTI_LABELS_MULTI_PAGE)
		{
			m_pLabel2->SetTitle(m_pStrList[startIndex + 1],strlen(m_pStrList[startIndex + 1]));
		}
		else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
		{
// 			m_pBtn2->m_iW =  strlen(m_pStrList[1])*CHAR_W;
			m_pBtn2->SetTitle(m_pStrList[startIndex + 1],strlen(m_pStrList[startIndex + 1]));
		}
		DBG_PRN("info", ("m_pStrList[%d] = %s",startIndex + 1, m_pStrList[startIndex + 1]));
	}
	
	if ((totalLine > 2) && (startIndex + 2 < totalLine))
	{
		if (m_iComType == MULTI_LABELS_MULTI_PAGE)
		{
			m_pLabel3->SetTitle(m_pStrList[startIndex + 2],strlen(m_pStrList[startIndex + 2]));
		}
		else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
		{
// 			m_pBtn3->m_iW = strlen(m_pStrList[2])*CHAR_W;
			m_pBtn3->SetTitle(m_pStrList[startIndex + 2],strlen(m_pStrList[startIndex + 2]));
		}
			DBG_PRN("info", ("m_pStrList[%d] = %s",startIndex + 2, m_pStrList[startIndex + 2]));
	}

	if((totalLine > 3) && (startIndex + 3 < totalLine))
	{
		if (m_iComType == MULTI_LABELS_MULTI_PAGE)
		{
			m_pLabel4->SetTitle(m_pStrList[startIndex + 3],strlen(m_pStrList[startIndex + 3]));
		}
		else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
		{
// 			m_pBtn4->m_iW = strlen(m_pStrList[3])*CHAR_W;
			m_pBtn4->SetTitle(m_pStrList[startIndex + 3],strlen(m_pStrList[startIndex + 3]));
		}
			DBG_PRN("info", ("m_pStrList[%d] = %s",startIndex + 3, m_pStrList[startIndex + 3]));
	}

	if((totalLine > 4) && (startIndex + 4 < totalLine))
	{
		if (m_iComType == MULTI_LABELS_MULTI_PAGE)
		{
			m_pLabel5->SetTitle(m_pStrList[startIndex + 4],strlen(m_pStrList[startIndex + 4]));
		}
		else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
		{
			m_pBtn5->SetTitle(m_pStrList[startIndex + 4],strlen(m_pStrList[startIndex + 4]));
		}
			DBG_PRN("info", ("m_pStrList[%d] = %s",startIndex + 4, m_pStrList[startIndex + 4]));
	}
	DBG_PRINT(("startIndex = %d",startIndex));	
	DBG_PRINT(("totalLine = %d",totalLine));

	ReFresh();
}
CMultiPageCommonWin::~CMultiPageCommonWin()
{
	ClearListBuff();
}

void CMultiPageCommonWin::ClearListBuff(void)
{
	if (m_pStrList != NULL)
	{
		for (int i = 0; i < m_iTotalLine; i ++)
		{
			if (m_pStrList[i] != NULL)
			{
				delete []m_pStrList[i];
				m_pStrList[i] = NULL;
			}			
		}	
		delete []m_pStrList;
		DBG_PRN("info",("**********释放 CMultiPageCommonWin::m_pStrList*****"));
	}	

	m_pStrList = NULL;
}

int CMultiPageCommonWin::GetSelectedIndex()
{
	int res;

	if (m_iComType == CMultiPageCommonWin::MULTI_LABELS_MULTI_PAGE)
	{
		return -1;
	}

	res = m_iPressedBtnIndex + PAGE_LINE_NUM * (m_iCurPageNum -  1);
	return res;
}

void CMultiPageCommonWin::S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiPageCommonWin *win=(CMultiPageCommonWin *)obj->GetdWin();
	win->OnButton1(iEvent,pEventData,iDataLen);
}

void CMultiPageCommonWin::S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiPageCommonWin *win=(CMultiPageCommonWin *)obj->GetdWin();
	win->OnButton2(iEvent,pEventData,iDataLen);
}

void CMultiPageCommonWin::S_OnButton3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiPageCommonWin *win=(CMultiPageCommonWin *)obj->GetdWin();
	win->OnButton3(iEvent,pEventData,iDataLen);
}

void CMultiPageCommonWin::S_OnButton4(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiPageCommonWin *win=(CMultiPageCommonWin *)obj->GetdWin();
	win->OnButton4(iEvent,pEventData,iDataLen);
}

void CMultiPageCommonWin::S_OnButton5(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{
	CMultiPageCommonWin *win=(CMultiPageCommonWin *)obj->GetdWin();
	win->OnButton5(iEvent,pEventData,iDataLen);
}

void CMultiPageCommonWin::GetNextPage()
{
	DBG_ASSERT_EXIT(m_iCurPageNum < m_iTotalPage,("No more pages"));

	m_iCurPageNum ++;

	if (m_iComType == MULTI_LABELS_MULTI_PAGE)//调用是保证了有下一页
	{
		m_pLabel1->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
	}
	else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
	{
		m_pBtn1->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		m_pBtn1->SetFocus();
	}


	if (m_iCurWinLastComIndex < m_iTotalLine)
	{
		if (m_iComType == MULTI_LABELS_MULTI_PAGE)
		{
			m_pLabel2->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		}
		else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
		{
			m_pBtn2->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		}
		
	}
	else
	{
		ResetVisiable(1);
		return;
	}

	if (m_iCurWinLastComIndex < m_iTotalLine)
	{
		if (m_iComType == MULTI_LABELS_MULTI_PAGE)
		{
			m_pLabel3->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		}
		else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
		{
			m_pBtn3->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		}
		
	}
	else
	{
		ResetVisiable(2);
		return;
	}

	if (m_iCurWinLastComIndex < m_iTotalLine)
	{
		if (m_iComType == MULTI_LABELS_MULTI_PAGE)
		{
			m_pLabel4->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		}
		else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
		{
			m_pBtn4->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		}
		
	}
	else
	{
		ResetVisiable(3);
		return ;
	}

	if (m_iCurWinLastComIndex < m_iTotalLine)
	{
		if (m_iComType == MULTI_LABELS_MULTI_PAGE)
		{
			m_pLabel5->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		}
		else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
		{
			m_pBtn5->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		}
		
	}
	else
	{
		ResetVisiable(4);
		return ;
	}

	ResetVisiable(5);
}
int CMultiPageCommonWin::ReFresh(void)
{
	//页码
	char title[OBJ_TITLE_MAX_LEN + 1];
	int titleLen;
	bool overLapFlag;

	titleLen = OBJ_TITLE_MAX_LEN;
	memset(title, 0, OBJ_TITLE_MAX_LEN + 1);

	overLapFlag = false;
// 	if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
// 	{
// 		if (m_pBtn4 != NULL && m_pBtn4->Visible())
// 		{
// 			if (m_pBtn4->GetTitle(title, titleLen) > PAGE_NUM_X/CHAR_W)
// 			{
// 				overLapFlag = true;
// 			}
// 		}
// 	}
	/*else */if (m_iComType == MULTI_LABELS_MULTI_PAGE)
	{
		if (m_pLabel5 != NULL && m_pLabel5->Visible())
		{
			if (m_pLabel5->GetTitle(title, titleLen) > PAGE_NUM_X/CHAR_W)
			{
				overLapFlag = true;
			}
		}
	}

	
	if (!overLapFlag)
	{
		memset(title, 0, OBJ_TITLE_MAX_LEN + 1);
		DBG_ASSERT_WARNING(m_iTotalPage < 100,
		("There are %d pages,some of the page num may not be displayed!"));

		sprintf(title, "%d/%d", m_iCurPageNum, m_iTotalPage);
	//DBG_PRN("___info______",("页码显示 = %s", pageDis));
		m_pPageLabel->SetTitle(title,strlen(title));
		m_pPageLabel_2->SetTitle(title,strlen(title));
	}
	else
	{
		m_pPageLabel->SetTitle("", 0);
		m_pPageLabel_2->SetTitle("", 0);
	}
	
	ReFreshTitle();
	CaGroup::ReFresh();
	if(m_pCurObj != NULL )
	{
 		m_pCurObj->SetFocusIn();
	}
	
	LCDRedraw();
//	ProcessKeyEvent();
	return 1;
}
void CMultiPageCommonWin::GetPrePage()
{
	DBG_ASSERT_EXIT(m_iCurPageNum > 1,("No more pages at front"));

	m_iCurPageNum --;

	int startLine;

	if (m_iCurWinLastComIndex % PAGE_LINE_NUM)
	{
		startLine = m_iCurWinLastComIndex%PAGE_LINE_NUM;
	}
	else
	{
		startLine = PAGE_LINE_NUM;
	}

	m_iCurWinLastComIndex = m_iCurWinLastComIndex - startLine - PAGE_LINE_NUM;//位于上一页第一条位置

	if (m_iComType == MULTI_LABELS_MULTI_PAGE)
	{
		m_pLabel1->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		m_pLabel2->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		m_pLabel3->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		m_pLabel4->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		m_pLabel5->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
	}
	else if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
	{
		m_pBtn1->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		m_pBtn2->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		m_pBtn3->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		m_pBtn4->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		m_pBtn5->SetTitle(m_pStrList[m_iCurWinLastComIndex++],strlen(m_pStrList[m_iCurWinLastComIndex]));
		m_pBtn1->SetFocus();
	}
	ResetVisiable(5);
}

void CMultiPageCommonWin::OnButton1(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_iPressedBtnIndex = 0;
	OnBtnPressed();
	DBG_PRN("info",("you select the button %d", GetSelectedIndex()));
}
void CMultiPageCommonWin::OnButton2(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_iPressedBtnIndex = 1;
	OnBtnPressed();
	DBG_PRN("info",("you select the button %d", GetSelectedIndex()));
}
void CMultiPageCommonWin::OnButton3(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_iPressedBtnIndex = 2;
	OnBtnPressed();
	DBG_PRN("info",("you select the button %d", GetSelectedIndex()));
}
void CMultiPageCommonWin::OnButton4(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_iPressedBtnIndex = 3;
	OnBtnPressed();
	DBG_PRN("info",("you select the button %d", GetSelectedIndex()));
}

void CMultiPageCommonWin::OnButton5(int iEvent, unsigned char * pEventData, int iDataLen)
{
	m_iPressedBtnIndex = 4;
	OnBtnPressed();
	DBG_PRN("info",("you select the button %d", GetSelectedIndex()));
}

bool CMultiPageCommonWin::IsCurWin()
{
	return true;
}

void CMultiPageCommonWin::ResetVisiable(int endNum)
{
	DBG_ASSERT_EXIT(endNum >= -1 && endNum <= PAGE_LINE_NUM, ("每页显示的不能超过5行"));

	switch(endNum)
	{
	case 5:
		{
			if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
			{
				m_pBtn5->Visible(true);
				m_pBtn4->Visible(true);
				m_pBtn3->Visible(true);
				m_pBtn2->Visible(true);
				m_pBtn1->Visible(true);
			}
			else if (m_iComType == MULTI_LABELS_MULTI_PAGE)
			{
				m_pLabel5->m_Visible = true;
				m_pLabel4->m_Visible = true;
				m_pLabel3->m_Visible = true;
				m_pLabel2->m_Visible = true;
				m_pLabel1->m_Visible = true;
			}
		}
		break;
	case 4:
		{
			if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
			{
				m_pBtn5->Visible(false);
				m_pBtn4->Visible(true);
				m_pBtn3->Visible(true);
				m_pBtn2->Visible(true);
				m_pBtn1->Visible(true);
			}
			else if (m_iComType == MULTI_LABELS_MULTI_PAGE)
			{
				m_pLabel5->m_Visible = false;
				m_pLabel4->m_Visible = true;
				m_pLabel3->m_Visible = true;
				m_pLabel2->m_Visible = true;
				m_pLabel1->m_Visible = true;
			}
		}
		break;
	case 3:
		{
			if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
			{
				m_pBtn5->Visible(false);
				m_pBtn4->Visible(false);
				m_pBtn3->Visible(true);
				m_pBtn2->Visible(true);
				m_pBtn1->Visible(true);
			}
			else if (m_iComType == MULTI_LABELS_MULTI_PAGE)
			{
				m_pLabel5->m_Visible = false;
				m_pLabel4->m_Visible = false;
				m_pLabel3->m_Visible = true;
				m_pLabel2->m_Visible = true;
				m_pLabel1->m_Visible = true;

			}
		}
		break;
	case 2:
		{
			if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
			{
				m_pBtn5->Visible(false);
				m_pBtn4->Visible(false);
				m_pBtn3->Visible(false);
				m_pBtn2->Visible(true);
				m_pBtn1->Visible(true);
			}
			else if (m_iComType == MULTI_LABELS_MULTI_PAGE)
			{
				m_pLabel5->m_Visible = false;
				m_pLabel4->m_Visible = false;
				m_pLabel3->m_Visible = false;
				m_pLabel2->m_Visible = true;
				m_pLabel1->m_Visible = true;

			}
		}
	    break;
	case 1:
		{
			if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
			{
				m_pBtn5->Visible(false);
				m_pBtn4->Visible(false);
				m_pBtn3->Visible(false);
				m_pBtn2->Visible(false);
				m_pBtn1->Visible(true);
			}
			else if (m_iComType == MULTI_LABELS_MULTI_PAGE)
			{
				m_pLabel5->m_Visible = false;
				m_pLabel4->m_Visible = false;
				m_pLabel3->m_Visible = false;
				m_pLabel2->m_Visible = false;
				m_pLabel1->m_Visible = true;

			}
		}
	    break;
	case 0:
		{
			if (m_iComType == MULTI_BUTTONS_MULTI_PAGE)
			{
				m_pBtn5->Visible(false);
				m_pBtn4->Visible(false);
				m_pBtn3->Visible(false);
				m_pBtn2->Visible(false);
				m_pBtn1->Visible(false);
			}
			else if (m_iComType == MULTI_LABELS_MULTI_PAGE)
			{
				m_pLabel5->m_Visible = false;
				m_pLabel4->m_Visible = false;
				m_pLabel3->m_Visible = false;
				m_pLabel2->m_Visible = false;
				m_pLabel1->m_Visible = false;
			}
		}
	    break;
	default:
	    break;
	}
	ReFresh();
}
void CMultiPageCommonWin::SetPageNumVisible(bool visible)
{
	m_pPageLabel->m_Visible = visible;
}
void CMultiPageCommonWin::SetPageNumVisible_2(bool visible)
{
	m_pPageLabel_2->m_Visible = visible;
}
void CMultiPageCommonWin::EndProcess()
{

}

void CMultiPageCommonWin::OnBtnPressed()
{

}

//通过坐标计算
int CMultiPageCommonWin::GetFocusIndex()
{
	int index = -1;
	if ((m_pBtn1 != NULL && m_pBtn1->IsCurObj()) || (m_pLabel1 != NULL && m_pLabel1->IsCurObj()))
	{
		index = 0;
	}
	else if ((m_pBtn2 != NULL && m_pBtn2->IsCurObj()) || (m_pLabel2 != NULL && m_pLabel2->IsCurObj()))
	{
		index = 1;
	}
	else if ((m_pBtn3 != NULL && m_pBtn3->IsCurObj()) || (m_pLabel3 != NULL && m_pLabel3->IsCurObj()))
	{
		index = 2;
	}
	else if ((m_pBtn4 != NULL && m_pBtn4->IsCurObj()) || (m_pLabel4 != NULL && m_pLabel4->IsCurObj()))
	{
		index = 3;
	}
	else if ((m_pBtn5 != NULL && m_pBtn5->IsCurObj()) || (m_pLabel5 != NULL && m_pLabel5->IsCurObj()))
	{
		index = 4;
	}
	else
	{
		return -1;
	}

	return   index + PAGE_LINE_NUM * (m_iCurPageNum -  1);
}

