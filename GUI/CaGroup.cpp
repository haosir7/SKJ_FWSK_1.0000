#include "CaGroup.h"
#include "CaLabel.h"
#include "CaInput.h"
#include "VersionConfig.h"

#include "LOGCTRL.h"
#include "pos_debug.h"

CaGroup *CaGroup::m_pCrrentGroup=NULL;
CaGroup *CaGroup::GetCurrentGroup() {return m_pCrrentGroup;}
void CaGroup::SetCurrentGroup(CaGroup *g) {m_pCrrentGroup = g;}
extern "C" void LCDRedraw();

CaGroup::CaGroup(int objtype):CaObject(objtype,true)
{
	m_iObjCount=0;
	m_iFocusObjCount=0;
	m_pCurObj=NULL;
	m_iLbWidth=0;
	m_labelPos=ON_NULL;
	m_bEndFlag=false;
	m_pFocusList=NULL;
	m_bFocusObjChanged=false;
	Begin();
	
}

CaGroup::CaGroup():CaObject(CA_GROUP,true)
{
	m_iObjCount=0;
	m_iFocusObjCount=0;
	m_pCurObj=NULL;
	m_iLbWidth=0;
	m_labelPos=ON_NULL;
	m_pFocusList=NULL;
	m_bEndFlag=false;
	m_bFocusObjChanged=false;
	Begin();
}

CaGroup::~CaGroup()
{
	Clear();
}
void CaGroup::Begin() 
{
	m_pCrrentGroup = this;
}
void CaGroup::End() 
{
	DBG_ASSERT_WARNING(m_iObjCount > 0 ,
		 ("m_iObjCount=%d is zero! ",m_iObjCount));
	m_bEndFlag=true;
	ReCreateFocusList();
// 	DBG_PRN("info",("m_iObjCount=%d,m_iFocusObjCount=%d",m_iObjCount,m_iFocusObjCount));
	m_pCrrentGroup = (CaGroup*)m_pOwner;
}
int  CaGroup::Create(int iX,int iY,int iW,int iH)
{
	CaObject::Create(iX, iY, iW, iH);
	if(m_labelPos != ON_NULL)
	{
		DBG_ASSERT_EXIT((m_iLbWidth >0),("m_iLbWidth=%d",m_iLbWidth));
		m_pLabel=new CaLabel(true,ALIGN_LEFT);
		switch(m_labelPos) {
		case ON_LEFT:
			m_pLabel->Create(iX-m_iLbWidth, iY, m_iLbWidth, iH);
			break;
		case ON_TOP:
			m_pLabel->Create(iX, iY-WORD_H, m_iLbWidth, iH);
			break;
		case ON_INSIDE:
			m_pLabel->Create(iX, iY+WORD_H, m_iLbWidth, iH);			
		default:
			DBG_ASSERT_EXIT(false,("m_labelPos =%d error! ",(int)m_labelPos));
		}
	}
	return 1;
}

void CaGroup::Clear()
{
  CaObject*const* old_array = ObjList();
  int old_children = ObjCount();
  // clear everything now, in case 
  m_iObjCount = 0;
  m_pCurObj=NULL;
  // okay, now it is safe to destroy the children:
  CaObject*const* a = old_array;
  for (int i=old_children; i--;) { //if i == 0 break
    CaObject* o = *a++;
    if (o->m_pOwner == this && o != NULL )
	{
		o->m_pOwner=NULL; 
		delete o;
		o=NULL;
	};
  }
  if (old_children > 1) free((void*)old_array);	
  m_bFocusObjChanged=true;
}
void CaGroup::ClearFocusObj()
{
  if (m_iFocusObjCount > 1) 
	  free((void*)m_pFocusList);
  m_pFocusList=NULL;
  m_iFocusObjCount=0;
  m_pCurObj=NULL;
  m_bFocusObjChanged=true;

}

CaObject* const* CaGroup::ObjList() const
{
	 return m_iObjCount <= 1 ? (CaObject**)(&m_pList) : m_pList;
}

CaObject* const* CaGroup::FocusObjList() const
{
	 return m_iFocusObjCount <= 1 ? (CaObject**)(&m_pFocusList) : m_pFocusList;
}

int CaGroup::CalFocusObjCount()const
{
	CaObject*const* a = ObjList();
	int i=0,j=0; 
	for (i=0; i < m_iObjCount; i++)
	{
		 if((*a)->m_bFocus)j++;
		 a++;
	}
	return j;
}

int CaGroup::Find( CaObject *const*pList,int count,const CaObject *pObj ) const
{
	 CaObject*const* a = pList;
	int i; 
	for (i=0; i < count; i++)
	{
		if (*a++ == pObj) break;
	}
	//if i >= m_iObjCount mean that not found pObj
	return i;
}
int CaGroup::Find(const CaObject *pObj)
{
	return Find(m_pList,m_iObjCount,pObj);
}
int  CaGroup::FindFocusObj(const CaObject *pObj ) const
{
	return Find(m_pFocusList,m_iFocusObjCount,pObj);
}

void CaGroup::Add(CaObject *pObj)
{
	Insert(pObj,m_iObjCount);
// 	DBG_PRN("info",("m_iObjCount=%d",m_iObjCount));
// 	DBG_PRN("info",("m_iType=%d",pObj->m_iType));

//		Insert(&m_pList,m_iObjCount,pObj,m_iObjCount);
}
void CaGroup::Insert(CaObject***pList,int &oCount,CaObject *pObj,int index)
{
  DBG_ASSERT_EXIT(pObj != NULL,("pObj is NULL!"));
 //  DBG_PRN("info",("index=%d",index));
  if (oCount == 0) 
  { // use array pointer to point at single child
    *pList = (CaObject**)pObj;
  } 
  else if (oCount == 1) 
  { // go from 1 to 2 children
    CaObject* t = (CaObject*)(*pList);
    *pList = (CaObject**)malloc(2*sizeof(CaObject*));
    if (index==1) 
	{
		(*pList)[0] = t; 
		(*pList)[1] = pObj;
	}
    else 
	{
		(*pList)[0] = pObj;   //index非零插在最前面
		(*pList)[1] = t;
	}
  } 
  else 
  {
    if (!(oCount & (oCount-1))) // double number of children 按2,4,8,16...分配内存
    {  
		*pList = (CaObject**)realloc((void*)(*pList),
				    2*oCount*sizeof(CaObject*));
	}
    int j; 
	for (j = oCount; j > index; j--)  //insert pObj at (*pList)[index]
	{(*pList)[j] = (*pList)[j-1];}
	 (*pList)[j] = pObj;
  }
  oCount++;	
}


void CaGroup::Insert(CaObject *pObj,int index)
{
  DBG_ASSERT_EXIT(pObj != NULL,("pObj is NULL!"));
 // DBG_PRN("info",("index=%d",index));
  if (pObj->m_pOwner) 
  {
    CaGroup* g = (CaGroup*)(pObj->m_pOwner);
    int n = g->Find(pObj);
	if(n<index) //此对象已经在列表中了   ###
	{
		if (g == this) 
		{
		  if (index > n) index--;//      ###   repeat  need to delete this line
		  if (index == n) return;
		}
		g->Remove(pObj);
	}
  }
  pObj->m_pOwner = this;
  Insert(&m_pList,m_iObjCount,pObj,index);
  if(pObj->m_bFocus)
  {
	    m_bFocusObjChanged=true;
  }
  if( pObj->m_bFocus && m_bEndFlag)
  {
	ReCreateFocusList();
  }
 }

void CaGroup::InsertFocusObj(CaObject *pObj,int index)
{
  Insert(&m_pFocusList,m_iFocusObjCount,pObj,index);
}
int CaGroup::Remove(CaObject ***pList,int &oCount,CaObject *pObj)
{
  DBG_ENTER("void CaGroup::Remove(CaObject ***pList,int &oCount,CaObject *pObj)");
  if (oCount < 1) DBG_RETURN(0);
  int i=Find(*pList,oCount,pObj);
  if (i >= oCount) 
  {
	DBG_ASSERT_WARNING(false,
		("i=%d >= oCount=%d ,not fount the obj ",i,oCount));
	DBG_RETURN(0);
  }
  oCount--;
  if (oCount == 1) 
  { // go from 2 to 1 child
    CaObject *t = (*pList)[!i];
    free((void*)(*pList));
    *pList = (CaObject**)t;
  } 
  else if (oCount > 1) 
  { // delete from array
    for (; i < oCount; i++) (*pList)[i] = (*pList)[i+1]; 
  }
  DBG_RETURN(1);
}
void CaGroup::Remove(CaObject *pObj)
{
	if(Remove(&m_pList,m_iObjCount,pObj) == 1 )
	{
		pObj->m_pOwner=0;
		if(pObj->m_bFocus) 
		{
			m_bFocusObjChanged=true;
			ReCreateFocusList();
		}
	}
}
void CaGroup::RemoveFocusObj(CaObject *pObj)
{
	if(Remove(&m_pFocusList,m_iFocusObjCount,pObj) == 1)
	{
		if(m_pCurObj == pObj) 
			m_pCurObj=NULL;

		if(m_iFocusObjCount == 1)
		{
			m_pCurObj=*m_pFocusList;
		}
		else if(m_iFocusObjCount > 1)
		{
			if(m_pCurObj == NULL )
				m_pCurObj=m_pFocusList[0];
		}
	}
}
void CaGroup::ReCreateFocusList()
{
	 if(!m_bFocusObjChanged)
	 {
// 		 DBG_PRN("info",("focus list needn't recreate !"));
		 return;
	 }
	 else
	 {
		m_bFocusObjChanged=false;
	 }

	 CaObject*const* a = FocusObjList();
	 if(m_pFocusList != NULL && m_iFocusObjCount > 1 )
	 {
		 free((void *)a);
	 }
	 m_iFocusObjCount=CalFocusObjCount();
	 int i=0,j=0;
	 if(m_iFocusObjCount >1 )
	 {
		 m_pFocusList=(CaObject**)malloc(m_iFocusObjCount*sizeof(CaObject*));
		 CaObject*const *b =ObjList();
		 for(i=0;i<ObjCount();i++)
		 {
			 if((*b)->m_bFocus) 
			 {
				m_pFocusList[j++]=*b;
			 }
			b++;
		 }
		 if(m_pCurObj== NULL )
		 {
			 m_pCurObj=m_pFocusList[0];
		 }
		 
	 }
	 else if(m_iFocusObjCount == 1)
	 {
		 CaObject*const *b =ObjList();
		 if(ObjCount() == 1)
		 {
			 m_pFocusList=(CaObject**)&(*b);
			 m_pCurObj=*b;
		 }
		 else
		 {
 			 for(i=0;i<ObjCount();i++)
			 {
				 if((*b)->m_bFocus) 
				 {
					 m_pFocusList=(CaObject**)&(*b);
					 m_pCurObj=*b;
					 break;
				 }
				b++;
			 }
		 }
	 }
}	 
CaObject *CaGroup::NextFocusObj(CaObject *curObj)
{
	DBG_ASSERT_WARNING(m_iFocusObjCount > 0 ,(" m_iFocusObjCount=%d",m_iFocusObjCount));
	CaObject *obj=NULL;
	if(m_iFocusObjCount < 1)
	{
		obj=NULL;
	}
	else if(m_iFocusObjCount == 1 )
	{
		if(m_pCurObj==NULL)
		{
			m_pCurObj=*m_pFocusList;
		}
		obj= m_pCurObj;
	}
	else
	{
		if(curObj != NULL )
		{
			int n=FindFocusObj(curObj);

			DBG_ASSERT_WARNING( n < m_iFocusObjCount,
				("n=%d,m_iFocusObjCount=%d",n,m_iFocusObjCount) );
			if( n < m_iFocusObjCount-1)
			{
				obj=m_pFocusList[n+1];
			}
			else if( n == m_iFocusObjCount-1 )
			{
				obj=m_pFocusList[0]; //          ####
			}
			else
			{
				obj= m_pFocusList[0];//           ####
			}

		}
		else
		{
			obj= m_pFocusList[0];    //          ####
		}
		
	}
	return obj;
}

CaObject *CaGroup::PreFocusObj(CaObject *curObj)
{
 	DBG_ASSERT_WARNING(m_iFocusObjCount > 0 ,(" m_iFocusObjCount=%d",m_iFocusObjCount));
	CaObject *obj=NULL;
	if(m_iFocusObjCount < 1)
	{
		obj=NULL;
	}
	else if(m_iFocusObjCount == 1 )
	{
		if(m_pCurObj==NULL)
		{
			m_pCurObj=*m_pFocusList;
		}
		obj=m_pCurObj;
	}
	else
	{
		if(curObj != NULL )
		{
			int n=FindFocusObj(curObj);
			DBG_ASSERT_WARNING( n < m_iFocusObjCount,
				("n=%d,m_iFocusObjCount=%d",n,m_iFocusObjCount) );
			if( n == 0)
			{
				obj=m_pFocusList[m_iFocusObjCount-1];
			}
			else if( n > 0 && n <= m_iFocusObjCount-1 )
			{
				obj=m_pFocusList[n-1];
			}
			else
			{
				obj=m_pFocusList[0];
			}

		}
		else
		{
			obj=m_pFocusList[0];
		}
	}
	return obj;
}

int  CaGroup::ReFresh(void)
{
	CaObject*const* a = ObjList();
	int i; 
	ClearDisp();
	DrawBox();
	for (i=0; i < m_iObjCount; i++) 
	{
		if(a[i]->m_Visible)
			a[i]->ReFresh();
	}
	if(m_pCurObj != NULL )
	{
		if(this->m_iType >= CA_PAGE )
		{
			m_pCurObj->SetFocus();
		}
	}

	return 1;
}
int  CaGroup::SetFocus(void)
{
	ReFresh();
	if(m_pCurObj != NULL )
	{
			m_pCurObj->SetFocus();
	}
	else
	{
		CaObject *obj=NextFocusObj(NULL);
		if(obj != NULL )
		{
			m_pCurObj=obj;
			m_pCurObj->SetFocus();
		}
	}
	return 1;
}

//int CaGroup::SetTitle(unsigned char *pData,int iLen)
//{
//	return 1;
//}
int CaGroup::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	if(m_iObjCount <= 0)
	{
		return 0;
	}
	if(m_pCurObj != NULL ) //先让当前子对象处理
	{	
		if( m_pCurObj->ProcEvent(iEvent,pEventData,iDataLen) == 1 )
		{
			return 1;
		}
	}
	int res=0;
	switch(iEvent)
	{
		case PRE_OBJ_KEY:         //前一个控件
			 OnUpKey();
			 res=1;
			break;
#if(POS_TYPE !=POS_APE3000R)
		case NEXT_OBJ_KEY:        //后一个控件
			OnDownKey();
			res=1;
			break;
#endif
		case TAB_KEY:
		//	res=OnTabKey();
			OnDownKey();
			res=1;
			break;
		case LEFT_KEY:
			res=OnLeftKey();
			break;
		case RIGHT_KEY:
			res=OnRightKey();
			break;
		default:
			return res;
	}
	return res;
}


int CaGroup::OnUpKey(void)
{
	CaObject *obj=PreFocusObj(m_pCurObj);
	CaObject *firstFindObj=obj;
	for(;;)
	{
		if(obj == NULL )
		{
			break;
		}
		if(obj->m_Visible)
		{
			if(m_pCurObj != NULL && m_pCurObj->m_Visible )
			{
				m_pCurObj->ReFresh();
			}
			m_pCurObj=obj;
			m_pCurObj->SetFocus();
			break;
		}
		obj=PreFocusObj(obj);
		if(obj == firstFindObj)
		{
			if(m_pCurObj != NULL && m_pCurObj->m_Visible )
				m_pCurObj->ReFresh();
			m_pCurObj=firstFindObj;
			m_pCurObj->SetFocus();
			break;
		}
	}
	return 1;
}
int CaGroup::OnDownKey(void)
{

	CaObject *obj=NextFocusObj(m_pCurObj);
	CaObject *firstFindObj=obj;
	for(;;)
	{
		if(obj == NULL )
		{
			break;
		}
		if(obj->m_Visible)
		{
			if(m_pCurObj != NULL && m_pCurObj->m_Visible )
			{
				m_pCurObj->ReFresh();
			}
			m_pCurObj=obj;
			m_pCurObj->SetFocus();
			break;
		}
		obj=NextFocusObj(obj);
		if(obj == firstFindObj )
		{
			if(m_pCurObj != NULL && m_pCurObj->m_Visible )
				m_pCurObj->ReFresh();
			m_pCurObj=firstFindObj;
			m_pCurObj->SetFocus();
			break;
		}
	}
	return 1;
}
int CaGroup::OnLeftKey(void)
{
	return OnUpKey();
}
int CaGroup::OnRightKey(void)
{
	return OnDownKey();
}
int CaGroup::OnTabKey(void)
{
	return -1;	
}
int CaGroup::OnPgUpKey()
{
	return -1;
}
int CaGroup::OnPgDnKey()
{
	return -1;
}

bool CaGroup::GetRefreshFlag()
{
	CaObject*const* a = ObjList();
	int i; 
	for (i=0; i < m_iObjCount; i++) 
	{
		if( a[i]->GetRefreshFlag() )
		{
			m_bRefreshFlag=true;
			break;
		}
	}
	if (i == m_iObjCount)
	{
		m_bRefreshFlag = false;
	}
	return m_bRefreshFlag;
}

void CaGroup::ClearRefreshFlag()
{
	CaObject*const* a = ObjList();
	int i; 
	for (i=0; i < m_iObjCount; i++) 
	{
		a[i]->ClearRefreshFlag();
	}
	m_bRefreshFlag=false;
}

void CaGroup::ResetFocus()
{
	 if(m_pFocusList != NULL && m_iFocusObjCount > 1 )//m_iFocusObjCount ＝＝ 1不处理，如buttonlist
	 {
		 for (int i = 0; i < m_iFocusObjCount; i ++)
		 {
			 if (m_pFocusList[i]->m_Visible)
			 {
				 if(m_pCurObj != NULL && m_pCurObj->m_Visible )
				 {
					m_pCurObj->ReFresh();
				 }
				 m_pCurObj = m_pFocusList[i];
				 m_pCurObj->SetFocus();
				 if (m_pCurObj->m_iType >= CA_GROUP)
				 {
					((CaGroup *)m_pCurObj)->ResetFocus();
				 }
				 break;
			 }
		 }
	 }
}
// bool CaGroup::CurObjIsIME()
// {
// 	if(m_pCurObj != NULL && m_pCurObj->m_iType >= CA_PAGE && m_pCurObj->m_bFocus)
// 	{
// 		return ((CaGroup *)m_pCurObj)->CurObjIsIME();
// 	}
// 	else if (m_pCurObj != NULL && m_pCurObj->m_iType == CA_INPUT && m_pCurObj->m_bFocus)
// 	{
// 		if (!((CaInput *)m_pCurObj)->m_bReadOnly)
// 		{
// 			return true;
// 		}
// 	}
// 	return false;
// }
