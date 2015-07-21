
#include"CaInputList.h"
#include "g_def.h"

#include "LOGCTRL.h"
#include"pos_debug.h"

CaInputList::CaInputList(int colCount,int InnerObjNum):CaScrollGroup(CA_INPUT_LIST, InnerObjNum)
{
	m_iColCount=colCount;
	m_iInputCount = 0;
}
CaInputList::CaInputList()
{
	m_iInputCount = 0;
}
CaInputList::~CaInputList()
{
  
}

CaInput *CaInputList::CreateNewInput()
{
	CaInput *inp=new CaInput(CaObject::ON_LEFT,INPUT_MAX_TITLE_LEN);
	inp->Create(0,0,m_iColW,WORD_H);//SCREEN_W - 32//
	m_iInputCount ++;
	return inp;
}
CaButton *CaInputList::CreateNewButton()
{
	CaButton *btn=new CaButton();
	btn->Create(0,0,m_iColW/m_iInnerObjNum,WORD_H);
	btn->m_iType = CA_BUTTON_INNER;
	btn->BoxType(CaObject::BOX_NO);
	return btn;
}

void CaInputList::Add(CaObject *pObj)
{
	DBG_ASSERT_WARNING(pObj->m_iType == CA_INPUT,
						("object type=%d is not a input ",pObj->m_iType));
	DBG_PRN("info",("pObj->m_iType:%d",pObj->m_iType));
	CaScrollGroup::Add(pObj);
}


