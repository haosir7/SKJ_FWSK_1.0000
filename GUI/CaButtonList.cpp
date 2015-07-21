#include"CaButtonList.h"

#include"LOGCTRL.h"
#define NO_POS_DEBUG
#include"pos_debug.h"

CaButtonList::CaButtonList(int colCount):CaScrollGroup(CA_BUTTON_LIST)
{
	m_iColCount=colCount;
}
CaButtonList::CaButtonList():CaScrollGroup(0)
{
}
CaButtonList::~CaButtonList()
{
  
}
CaButton *CaButtonList::CreateNewButton()
{
	CaButton *btn=new CaButton();
	btn->Create(0,0,m_iColW,WORD_H);
	return btn;
}
void CaButtonList::Add(CaObject *pObj)
{
	DBG_ASSERT_WARNING(pObj->m_iType == CA_BUTTON,
						("object type=%d is not a button ",pObj->m_iType));
//	pObj->m_pOwner=NULL;
	CaScrollGroup::Add(pObj);
}

/*!
/ÕÒµ½µÚindex ¸öbutton
*/
// CaObject * CaButtonList::GetButton(int index)
// {
// 	DBG_ASSERT_EXIT(index > 0 && (index <= m_iBtnCount), 
// 		("index = %d is invalid", index));
// 	DBG_ASSERT_WARNING(m_iBtnCount == this->ObjCount(), ("button count doesn't match!"));
// 	CaObject **objList = (CaObject **)this->ObjList();
// 	return &(*objList)[index];
// 	//return *
//}

