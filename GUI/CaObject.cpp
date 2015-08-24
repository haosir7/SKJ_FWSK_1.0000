#include"CaObject.h"
#include"CaGroup.h"
#include"CaWindow.h"
#include "stdio.h"
	
#include"LOGCTRL.h"
#include"pos_debug.h"
//int CaObject::m_iObjCount=0;

CaObject::CaObject(int type,bool bfocus,CaGroup *pOwner )
{
	m_iX=0;
	m_iY=0;
	m_iW=0;
	m_iH=0;

	m_pLabel=NULL;
	m_iBoxType=BOX_NO;
//	m_iObjID=0;	
    m_PgNum=0;
    m_Visible=true;
	m_bRefreshFlag=false;
    m_bFocus=bfocus;
    OnObject=0;
	m_pOwner=pOwner;
	m_iType=type;
	if(pOwner != NULL )
	{
		pOwner->Add(this);
	}
	else
	{
		if (CaGroup::GetCurrentGroup()) CaGroup::GetCurrentGroup()->Add(this);
	}
//	OnObject = this->OnTest;
}
CaObject::CaObject(CaGroup *pOwner)
{
	DBG_ASSERT_EXIT(pOwner != NULL,("pOwner is NULL" ));
	m_iX=0;
	m_iY=0;
	m_iW=0;
	m_iH=0;

	m_pLabel=NULL;
	m_iBoxType=BOX_NO;
//	m_iObjID=0;	
    m_PgNum=0;
    m_Visible=true;
    m_bFocus=false;
	m_bRefreshFlag=false;
    OnObject=0;
	m_pOwner=pOwner;
	m_iType=0;
	pOwner->Add(this);
}

CaObject::CaObject()
{
	m_iX=0;
	m_iY=0;
	m_iW=0;
	m_iH=0;

	m_pLabel=NULL;
	m_iBoxType=BOX_NO;
//	m_iObjID=0;	
    m_PgNum=0;
    m_Visible=true;
	m_bRefreshFlag=false;

    m_bFocus=false;
    OnObject=0;
	m_pOwner=NULL;
	m_iType=0;
	if (CaGroup::GetCurrentGroup()) CaGroup::GetCurrentGroup()->Add(this);
}

CaObject::~CaObject()
{
//	DBG_PRN("info",("CaObject::~CaObject() m_iType=%d\n",m_iType));
	if(m_pOwner != NULL )
	{
		m_pOwner->Remove(this);
		m_pOwner=NULL;
	}
}
/*!
	@brief 真正创建控件	
	@param iX 左上角横坐标
	@param iY 左上角纵坐标
	@param iW 宽度
	@param iH 高度
	@param pOwner 所属管理控件句柄
	@return <=-1 失败，>=0 成功
*/
int CaObject::Create(int iX,int iY,int iW,int iH)
{
	DBG_ASSERT_EXIT(( iX>=0 && iY>=0 && iW>0 && iH>0),
		("iX=%d,iY=%d,iW=%d,iH=%d",iX,iY,iW,iH) );
	if(this->m_iType < CA_PAGE && this->m_iType >CA_OBJ_BASE)
	{
		m_iX=iX+m_pOwner->m_iX; //m_pOwner是相对原点的绝对坐标
		m_iY=iY+m_pOwner->m_iY;
	}
	else
	{
		m_iX=iX;
		m_iY=iY;
	}
	m_iW=iW;
	m_iH=iH;	
	return 0;
}
int CaObject::ProcEvent(int iEvent, unsigned char * pEventData, int iDataLen)
{
	if(OnObject!=0)
	{
//		if ((this->m_iType == CA_INPUT) && (iEvent >= 0x11) && (iEvent <= 0x18))
//		{
//			return 1;
//		}
		OnObject(this,iEvent,pEventData,iDataLen);
		return 1;
	}
	return 0;
}

int CaObject::ReFresh(void)
{
	return 0;
}

int CaObject::SetFocus(void)
{
	if(m_bFocus)
	{
		m_pOwner->CurObj(this);
		return 1;
	}
	else
	{
		return 0;
	}
}
int CaObject::SetFocusIn(void)
{
	return 0;	
}
int CaObject::SetTitle(const char * pData, int iLen)
{

	if(m_pLabel != NULL)
	{
		return m_pLabel->SetTitle(pData,iLen);
	}
	else
	{
		DBG_ASSERT_WARNING(false ,("m_pLabel is NULL "));
		return -1;
	}
}
int CaObject::GetTitle(char *pData,int &iLen)
{

	if(m_pLabel != NULL)
	{
		return m_pLabel->GetTitle(pData,iLen);
	}
	else
	{
		DBG_ASSERT_WARNING(false ,("m_pLabel is NULL "));
		return -1;
	}
}

int CaObject::ReFreshTitle(void)
{
	if(m_pLabel != NULL)
	{
		return m_pLabel->ReFresh();
	}
	else
	{
		return -1;
	}
}
CaObject::ALIGN_TYPE CaObject::GetTitleAlign()
{
	if(m_pLabel)
	{
		return m_pLabel->GetTitleAlign();
	}
	return ALIGN_LEFT;
}
void CaObject::SetTitleAlign(ALIGN_TYPE altype)
{
	if(m_pLabel)
	{
		 m_pLabel->SetTitleAlign(altype);
	}
}

int CaObject::AlignBuf( const char * str,char * buf, int bufSize,int align )
{
	int nLen=0;
	nLen=strlen(str);
	if (nLen > bufSize)
	{
		nLen  = bufSize;
	}
	DBG_ASSERT_EXIT((str != NULL ) && (buf !=NULL ),("str or buf is NULL "));
	DBG_ASSERT_EXIT(nLen <= bufSize,("nLen=%d > bufSize=%d ",nLen,bufSize));
	memset(buf,' ',bufSize);
    if( ALIGN_LEFT == align )
    {
         memcpy( buf, str, nLen );
    }
    else if( ALIGN_RIGHT == align )
    {
        memcpy( buf+bufSize-nLen, str, nLen );
    }
    else if( ALIGN_CENTER == align )
    {
        memcpy( buf+(bufSize-nLen)/2, str, nLen );
    }
    return 0;	
	
}
bool CaObject::IsRefreshAble()
{
	return GetdWin()->IsCurWin();
}

bool CaObject::IsCurObj()
{
	if(m_pOwner != NULL)
	{
		return m_pOwner->CurObj() == this;
	}
	else
	{
		return false;
	}
}

void CaObject::DrawBox()
{
	if(m_iBoxType <= BOX_NO)
	{
		return;
	}

	int x=m_iX >0 ? m_iX-1:0;
	int y=m_iY >0 ? m_iY-1:0;
	int w=m_iX-x + m_iW +1;
	int h=m_iY-y + m_iH+1 ;

	if(m_iBoxType == BOX_ALL )
	{
		LCDFillRectLine(x,y,w,h,1);
	}
	else
	{
		 if(m_iBoxType & BOX_BOTTOM_LINE )
		{
			LCDFillRectLine(x,m_iY+m_iH,w,1,1);
		}
		 if(m_iBoxType & BOX_TOP_LINE)
		{
			LCDFillRectLine(x,y,w,1,1);
		}
		 if(m_iBoxType & BOX_LEFT_LINE )
		 {
			 LCDFillRectLine(x,y,w,h,1);
		 }
		 if(m_iBoxType & BOX_RIGHT_LINE)
		 {
			 LCDFillRectLine(m_iX+m_iW+1,y,1,h,1);
		 }
	}
}
void CaObject::ClearDisp()
{
		LCDFillRect(m_iX,m_iY,m_iW,m_iH,0);
}
void CaObject::Visible(bool v)
{
	m_Visible=v;
	if(m_pLabel != NULL )
	{
		m_pLabel->Visible(v);
	}
}

#ifdef GUI_DBG
void CaObject::LCDRedraw()
{
	DBG_ENTER("LCDRedraw");
	DBG_VOID_RETURN;
}
//void CaObject::LCDInit(void)
//{
//	DBG_ENTER("LCDInit");
//	DBG_VOID_RETURN;
//}
//void CaObject::LCDFillRect(int x0, int y0, int w, int h, int color)
//{
//		DBG_ENTER("LCDFillRect");
//	DBG_VOID_RETURN;
//}
//void CaObject::LCDFillRectLine(int x0, int y0, int w, int h, int color)
//{
//	DBG_ENTER("LCDFillRectLine");
//	DBG_VOID_RETURN;
//}
void CaObject::LCDGetGeometry(void)
{
	DBG_ENTER("LCDGetGeometry");
	DBG_VOID_RETURN;
}
//void CaObject::LCDPutImage(int x0, int y0, int w, int h, char *buf)
//{
//	DBG_ENTER("LCDPutImage");
//	DBG_VOID_RETURN;
//}
//void CaObject::LCDPutImageRev(int x0, int y0, int w, int h, char *buf)
//{
//	DBG_ENTER("LCDPutImageRev");
//	DBG_VOID_RETURN;
//}
void CaObject::putpixel( int x, int y, int c )
{
	DBG_ENTER("putpixel");
	DBG_VOID_RETURN;
}
//void CaObject::str2image(unsigned char *str, int len, unsigned char *buffer)
//{
//	DBG_ENTER("str2image");
//	DBG_VOID_RETURN;
//}
#endif

void CaObject::OnTest(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen)
{

}


