#include"CaLableSelect.h"

#include"LOGCTRL.h"
#include"pos_debug.h"

CaLabelSelect::CaLabelSelect(bool bFocus,ALIGN_TYPE alType,int objtype):CaObject(objtype,bFocus)
{
	m_bFocus=bFocus;
	m_iTitleLen=0;
	m_iAlignType=alType;
	memset( m_caTitle, 0, OBJ_TITLE_MAX_LEN );
	memset( m_caTitleImgBuf, 0, OBJ_TITLE_MAX_LEN*IMG_BYTES );
	m_iBoxType=BOX_NO;
	m_bFocus=true;

}
CaLabelSelect::CaLabelSelect(bool bFocus,ALIGN_TYPE alType,CaGroup *pOwner):CaObject(CA_LABEL,bFocus,pOwner)
{
	m_bFocus=bFocus;
	m_iTitleLen=0;
	m_iAlignType=alType;
	memset( m_caTitle, 0, OBJ_TITLE_MAX_LEN );
	memset( m_caTitleImgBuf, 0, OBJ_TITLE_MAX_LEN*IMG_BYTES );
	m_iBoxType=BOX_NO;
	m_bFocus=true;

}

CaLabelSelect::CaLabelSelect():CaObject(CA_LABEL,false)
{
	m_bFocus=false;
	m_iTitleLen=0;
	m_iAlignType=ALIGN_LEFT;
	m_iBoxType=BOX_NO;
	memset( m_caTitle, 0, OBJ_TITLE_MAX_LEN );
	memset( m_caTitleImgBuf, 0, OBJ_TITLE_MAX_LEN*IMG_BYTES );
	m_bFocus=true;
}
CaLabelSelect::~CaLabelSelect()
{

}

int CaLabelSelect::SetFocus()
{
// if(m_bFocus)
// {
	 return SetFocusIn();
// }
// else
// {
//	 return -1;
// }
}

void CaLabelSelect::SetAntiString(char *str)
{
	strcpy(m_antiString, str);
	return;
}

int CaLabelSelect::ProcEvent(int iEvent, unsigned char * pEventData, int iDataLen)
{
	int res=0;

	if(iEvent == TAB_KEY || iEvent == ENTER_KEY)
	{
		CaObject::ProcEvent(iEvent,pEventData,iDataLen);
		res=1;
	}
	else
	{
		res=0;
	}

	if(res == 1)
	{
		ReFresh();
		if(IsCurObj())
		{
			SetFocusIn();
		}
	}

//	DBG_PRN("info",("m_contentBuf = %s",m_contentBuf));
	return res;

}

int CaLabelSelect::ReFresh()
{
	int an_disp_pos;
	int an_disp_len = strlen((char *)m_antiString);
	int flag=0;


	if(!IsRefreshAble())
	{
		return 0;
	}
	if(m_iTitleLen>0)
	{
//		LCDPutImage(m_iX+(m_iW-m_iTitleLen*CHAR_W)/2,m_iY,m_iTitleLen*CHAR_W,GRID_LINE_H,(char *)m_caTitleImgBuf+BUF_OFFSET);
		if (an_disp_len)
		{
			for(an_disp_pos=0; an_disp_pos<m_iTitleLen; an_disp_pos++)
			{
				if (strncmp((char *)(m_caTitle+an_disp_pos), m_antiString, an_disp_len) == 0)
				{
					flag=1;
					break;
				}
			}
		}

		LCDPutImage(m_iX+(m_iW-(m_iW/CHAR_W)*CHAR_W)/2,m_iY,(m_iW/CHAR_W)*CHAR_W,GRID_LINE_H,(char *)m_caTitleImgBuf+BUF_OFFSET);
		if (flag == 1)
		{
			str2image((unsigned char *)m_antiString, an_disp_len, m_antiDispBuf);
			LCDPutImageRev(m_iX+(m_iW-(m_iW/CHAR_W)*CHAR_W)/2+an_disp_pos*CHAR_W, m_iY, an_disp_len*CHAR_W, 
							GRID_LINE_H, (char *)m_antiDispBuf+BUF_OFFSET);
		}
		
		m_bRefreshFlag=true;

	}
	DrawBox();
	return 1;
}

int CaLabelSelect::SetFocusIn()
{
	if(!IsRefreshAble())
	{
		return 0;
	}
//	LCDPutImageRev(m_iX+(m_iW-(m_iW/CHAR_W)*CHAR_W)/2,
//		m_iY,(m_iW/CHAR_W)*CHAR_W,GRID_LINE_H,(char *)m_caTitleImgBuf+BUF_OFFSET);

//	LCDRedraw();
	m_bRefreshFlag=true;
//	DBG_PRN("------------",("LCDRedraw"));

//	LCDPutImageRev(m_iX+(m_iW-m_iTitleLen*CHAR_W)/2,
//		m_iY,m_iTitleLen*CHAR_W,GRID_LINE_H,(char *)m_caTitleImgBuf+BUF_OFFSET);
	return 1;
}

//把数据写入某块内存区？
int CaLabelSelect::SetTitle(const char * pData, int iLen)
{
	DBG_ASSERT_EXIT((pData != 0 ),(" pData is null!"));

	memset( m_caTitle, 0, OBJ_TITLE_MAX_LEN );
	memset( m_caTitleImgBuf, 0, OBJ_TITLE_MAX_LEN*IMG_BYTES );

	int len = ( iLen <= OBJ_TITLE_MAX_LEN )?iLen:OBJ_TITLE_MAX_LEN;
	memcpy( m_caTitle, pData, len );
	m_iTitleLen = len;

	int bufLen=m_iW/CHAR_W;
	char buf[OBJ_TITLE_MAX_LEN+2];
	AlignBuf(pData,buf,bufLen,m_iAlignType);

	str2image((unsigned char *)buf,bufLen,m_caTitleImgBuf);

	return len;	
}

//把数据写从某块内存区读出?
int CaLabelSelect::GetTitle(char *pData,int &iLen)
{
	DBG_ASSERT_EXIT((pData != 0 ),(" pData is null!"));
	if (m_iTitleLen == 0)//m_iTitleLen = 0时，memecpy出错
	{
		iLen = 0;
		return 0;
	}
	int len = ( iLen <= m_iTitleLen )?iLen:m_iTitleLen;
	
	memcpy(pData, m_caTitle,  len );
	iLen=len ;
	return len;
}

void CaLabelSelect::SetTitleAlign(ALIGN_TYPE alType)
{
	if(	m_iAlignType==alType)
	{
		return;
	}
	else
	{
		m_iAlignType=alType;
		memset( m_caTitleImgBuf, 0, OBJ_TITLE_MAX_LEN*IMG_BYTES );
		int bufLen=m_iW/CHAR_W;
		char buf[OBJ_TITLE_MAX_LEN+2];
		AlignBuf((char *)m_caTitle,buf,bufLen,m_iAlignType);
		str2image((unsigned char *)buf,bufLen,m_caTitleImgBuf);		
	}
}


