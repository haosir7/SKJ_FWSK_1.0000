#include"CaMultiLineInput.h"
#include"CaLabel.h"
#include "PosFrame.h"
#include "ctype.h"
#include "display.h"

#include "LOGCTRL.h"
#include"pos_debug.h"

extern int is_gb_ex(unsigned char *str, int position);
unsigned char CaMultiLineInput::m_caFocusBuf[CaMultiLineInput::FOCUS_BUF_LEN]={'\0'};//没有用上
unsigned char CaMultiLineInput::m_caFocusImgBuf[CaMultiLineInput::FOCUS_IMG_LEN]={0,0,0,0,0,0,0,0,0,0,
												0,0,0,0,0,0,0,0,0,0};
CaMultiLineInput::CaMultiLineInput(LABEL_POS lbPos,int lbWidth, int totalLine):CaObject(CA_MULTILINE_INPUT,true)
{
	DBG_ASSERT_EXIT(totalLine <= SYS_SUPPORT_MAX_LINES, ("最大行数越界!"));
	m_iConLen=0;
	m_iCurScrPos=0;
	m_iCurConPos=0;
	m_iTotalLine = totalLine > 1 ? totalLine : 1;	
	m_iAllConMaxLen = 0;
	m_iCurScrLine = 1;
	m_iValidLine = 1;
	memset(m_iaCharCountBuf, 0, SYS_SUPPORT_MAX_LINES*sizeof(int));
	memset(m_contentBuf,0,MAX_BUF_LEN);

	m_bReadOnly=false;
	m_bDotFlag=false;
	m_InputType=aCHAR;
	m_alignType=ALIGN_LEFT;
	m_labelPos=lbPos;
	m_iLbWidth=lbWidth;
	m_iLbOffSet=0;
	m_iBoxType=BOX_BOTTOM_LINE;
	m_bFocus=true;
	memset(m_dispBuf,0,MAX_LINE_LEN);	
	memset(m_caImgBuf,0,MAX_LINE_LEN*IMG_BYTES);

}
CaMultiLineInput::CaMultiLineInput(int totalLine):CaObject(CA_INPUT,true)
{
	DBG_ASSERT_EXIT(totalLine <= SYS_SUPPORT_MAX_LINES, ("最大行数越界!"));
	 m_iConLen=0;
	 m_iCurScrPos=0;
	 m_iCurConPos=0;
	m_iTotalLine = totalLine > 1 ? totalLine : 1;	
	m_iAllConMaxLen = 0;
	 m_iCurScrLine = 1;
	 m_iValidLine = 1;
	memset(m_iaCharCountBuf, 0, SYS_SUPPORT_MAX_LINES*sizeof(int));
	memset(m_contentBuf,0,MAX_BUF_LEN);


	m_bFocus=true;
	m_bReadOnly=false;
	m_bDotFlag=false;
	m_InputType =aCHAR;
	m_alignType=ALIGN_LEFT;
	m_labelPos=ON_NULL;
	m_iLbWidth=0;
	m_iLbOffSet=0;
	m_iBoxType=BOX_BOTTOM_LINE;
	memset(m_dispBuf,0,MAX_LINE_LEN);	
	memset(m_caImgBuf,0,MAX_LINE_LEN*IMG_BYTES);
}

int	CaMultiLineInput::Create(int iX,int iY,int iW,int iH)
{
	if(m_labelPos == ON_LEFT )
	{
		m_iAllConMaxLen = m_iTotalLine*MAX_LINE_LEN - m_iLbWidth/CHAR_W;
		m_iLbOffSet=m_iLbWidth;
	}
	else
	{
		m_iAllConMaxLen = m_iTotalLine*MAX_LINE_LEN;
		m_iLbOffSet=m_iLbWidth;
	}
	
	CaObject::Create(iX, iY, iW, iH);
	if(m_labelPos != ON_NULL)
	{
		DBG_ASSERT_EXIT((m_iLbWidth >0),("m_iLbWidth=%d",m_iLbWidth));
		m_pLabel=new CaLabel(false,ALIGN_RIGHT,CA_INNER_LABEL);
		switch(m_labelPos) 
		{
		case ON_LEFT:
			m_pLabel->Create(m_iX, m_iY, m_iLbWidth, m_iH);
			break;
		case ON_TOP:
			m_pLabel->Create(m_iX, m_iY-WORD_H, m_iLbWidth, m_iH);
			break;
		default:
			DBG_ASSERT_EXIT(false,("m_labelPos =%d error! ",(int)m_labelPos));
		}
	}
	return 0;
}
int CaMultiLineInput::SetFocusIn()
{
	int lbOffset;
	if(!IsRefreshAble())
	{
		return 0;
	}
	if (m_iCurScrLine == 1)
	{
		lbOffset = m_iLbOffSet;
	}
	else
	{
		lbOffset = m_iX;
	}
	//反显光标
	str2image(m_dispBuf+m_iCurScrPos,FOCUS_BUF_LEN,m_caFocusImgBuf);//反显一个字节

	LCDPutImageRev(m_iX+lbOffset+m_iCurScrPos*CHAR_W,
		           m_iY + (m_iCurScrLine - 1) * (GRID_LINE_H + INPUT_BOTTOM_LINE_H),
				   CHAR_W,
				   GRID_LINE_H,
		          (char *)m_caFocusImgBuf+2);//点阵信息要偏移2个字节跳过点阵的坐标
	m_bRefreshFlag=true;
	return 1;	
}
int CaMultiLineInput::SetFocus()
{
	SetFocusIn();
    return	CaObject::SetFocus();
}
int CaMultiLineInput::ReFresh()
{
	if(!IsRefreshAble())
	{
		return 0;
	}
	int rowH = GRID_LINE_H + INPUT_BOTTOM_LINE_H;
	int offSet = 0;
	int lbOffset;
	m_iH = rowH*(m_iTotalLine-1) + GRID_LINE_H;
	for (int i = 0; i < m_iTotalLine; i ++)
	{
		if (i == 0)
		{
			lbOffset = m_iLbOffSet;
		}
		else
		{
			lbOffset = m_iX;
		}
		LCDFillRect(m_iX+lbOffset,m_iY + rowH*i,(MAX_LINE_LEN - lbOffset/CHAR_W)*CHAR_W,GRID_LINE_H,0);
		LCDFillRectLine(m_iX+lbOffset,m_iY + rowH*i + GRID_LINE_H,
				(MAX_LINE_LEN - lbOffset/CHAR_W)*CHAR_W,INPUT_BOTTOM_LINE_H,1);	

		if(i <= m_iValidLine)
		{
			DBG_PRN("info",("m_iaCharCountBuf[%d] = %d",i,m_iaCharCountBuf[i]));
			str2image(m_contentBuf + offSet, m_iaCharCountBuf[i],m_caImgBuf);//转换显示内容到点阵

			LCDPutImage(m_iX+lbOffset,m_iY + rowH*i,
				m_iaCharCountBuf[i]*CHAR_W,GRID_LINE_H,(char *)m_caImgBuf+2);//显示内容
			m_bRefreshFlag=true;
		}
		
		offSet += m_iaCharCountBuf[i];
	}

	if(m_pLabel != NULL )
	{
		m_pLabel->ReFresh();
	}
	return 1;
}
int CaMultiLineInput::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{

    DBG_PRN("info",("iEvent=%x",iEvent));
	DBG_PRN("info",("***before ievent m_iCurScrPos=%d,m_iCurConPos=%d **",m_iCurScrPos,m_iCurConPos));
	DBG_PRN("info",("***before ievent m_contentBuf=%s",(char *)m_contentBuf));

	int res=0;
	if (iEvent == CHANGE_INPUT_METHOD_EVENT && !m_bReadOnly 
		&& m_InputType!= aINT && m_InputType != aFLOAT)
	{
		CPosFrame::OnChangeIME(this,iEvent,pEventData,iDataLen);
		res = 1;
	}
	else
	{
		switch(m_InputType) {
		case aINT:
			if(!m_bReadOnly && ReciveIntChar(iEvent,(char *)pEventData,iDataLen) == 1 )
			{	res=1; }
			break;
		case aFLOAT:
			if(!m_bReadOnly &&ReciveFloatChar(iEvent,(char *)pEventData,iDataLen) == 1 )
			{	res= 1;	}
			break;
		case aCHAR:
		case aPASSWORD:
			if(!m_bReadOnly && ReciveStrChar(iEvent,(char *)pEventData,iDataLen) == 1 )
			{	res= 1;	}
			break;
		default:
			break;
		}
	}
    if(res == 0)
	{
		if (iEvent == CLEAR_ALL_EVENT && !m_bReadOnly )
		{
			Clear();
			res=1;
		}
		else if(iEvent == INPUT_LEFT )
		{
			ReciveLeftDirectKey(iEvent);
			res= 1;
		}
		else if(iEvent == INPUT_RIGHT )
		{
			ReciveRightDirectKey(iEvent);
			res= 1;
		}
		else if(iEvent == CLEAR_ONE_EVENT && !m_bReadOnly)
		{
			ReciveEraseKey(iEvent);
			res=1;
		}
		else
		{
			res=0;
		}
	}

	if(res == 1)
	{
		ReFresh();
		SetFocusIn();
	}
	DBG_PRN("info",("***after ievent m_iCurScrPos=%d,m_iCurConPos=%d **",m_iCurScrPos,m_iCurConPos));
	DBG_PRN("info",("***after ievent m_contentBuf=%s",(char *)m_contentBuf));
	return res;

}

int CaMultiLineInput::ReciveIntChar(int iEvent,char *pData,int iLen)
{
	
	if( (iEvent >= '0' && iEvent <= '9') )
	{
		char buf[4]={0,0,0,0};
		buf[0]=iEvent;
		Insert(buf,1,m_iCurScrPos,m_iCurConPos);
		return 1;
	}
	else if (iEvent == DOUBLE_ZERO_KEY )
	{
		ReciveIntChar('0',NULL,0);
		ReciveIntChar('0',NULL,0);
		return 1;
	}
	else
	{
		return 0;
	}
}
int CaMultiLineInput::ReciveFloatChar(int iEvent,char *pData,int iLen)
{

	if(ReciveIntChar(iEvent,pData,iLen) == 1 )
	{
		return 1;
	}
	if( (iEvent == DOT_KEY ) && !m_bDotFlag )
	{
		char buf[4]={0,0,0,0};
		buf[0]=iEvent;
		Insert(buf,1,m_iCurScrPos,m_iCurConPos);
		m_bDotFlag=true;//限制不能重复输入小数点
		return 1;
	}
	else
	{
		return 0;
	}
}

int CaMultiLineInput::ReciveStrChar(int iEvent,char *pData,int iLen)
{

	if(ReciveIntChar(iEvent,pData,iLen) == 1 )
	{
		return 1;
	}

	if (isgraph(iEvent) && (iEvent != 0x22)                                               
        && (iEvent != 0x27) && (iEvent != 0x09))
	{
		char buf[4]={0,0,0,0};
		buf[0]=iEvent;
		Insert(buf,1,m_iCurScrPos,m_iCurConPos);
		return 1;
	}
	else if( iEvent == INPUT_EVENT )
	{
		DBG_ASSERT_EXIT(pData != NULL,("pData is NULL!"));
		Insert(pData,iLen,m_iCurScrPos,m_iCurConPos);
		return 1;
	}
	else
	{
		return 0;
	}
}
void CaMultiLineInput::Insert(char *pData,int len,int scrIndex,int conIndex)
{
	int num = 0;
	int curLineLen;
	int tmpLen;
	bool isPreLineFull;
	int step;
	if(m_iConLen+len > m_iAllConMaxLen)
	{
		return ;
	}
	int	i=0;

	if (m_iCurScrLine > 1 && m_iaCharCountBuf[m_iCurScrLine - 2] < GetCurLineMaxLen(m_iCurScrLine - 2))
	{
		isPreLineFull = false;
	}
	else
	{
		isPreLineFull = true;
	}
	//往后移动插入点之后的内容	
	for(i=m_iConLen;i>=conIndex;i--)
	{
		m_contentBuf[i+len]=m_contentBuf[i];
	}
    //插入内容
	m_iCurConPos=conIndex;
	for(i=0;i<len;i++)
	{
		m_contentBuf[m_iCurConPos++]=pData[i];
		m_iConLen++;
	}
//GB2312
//	if(m_iCurConPos < m_iConLen && (m_contentBuf[m_iCurConPos] & 0x80) == 0x80  )
//	{
//		step = 2;
//	}
//	else
//	{
//		step = 1;
//	}

//GB18030
	if ((m_iCurConPos < m_iConLen) && (is_gb_ex(m_contentBuf, m_iCurConPos) == 1))
	{
		step = 2;
	}
	else
	{
		step = 1;
	}

	RecreateCharCountBuf();

	curLineLen = m_iaCharCountBuf[m_iCurScrLine - 1];
	tmpLen = GetCurLineMaxLen(m_iCurScrLine - 1);
    //处理光标
	if ((m_iCurScrPos == 0) && !isPreLineFull)//插入了字符，上行没满，不移动光标
	{//不换行
		if (len == 1)
		{
			m_iCurScrPos = 0;
		}
		else
		{
			m_iCurScrPos = 2;
		}
	}
	else if (m_iCurScrPos + len >= tmpLen)
	{
		if (m_iCurScrLine == m_iTotalLine)//最后一行,且没有位置显示光标
		{		
			m_iCurConPos -= len;//光标不动回退	
		}
		else
		{//换行
			if (len == 1)//插入了一个字符
			{
				m_iCurScrPos = 0;
			}
			else
			{
				if (tmpLen - m_iCurScrPos == 2)
				{
					m_iCurScrPos = 0;
				}
				else
				{
					m_iCurScrPos = 2;
				}
			}
			m_iCurScrLine ++;			
		}					
	}
	else if (tmpLen - m_iCurScrPos == 2 && len == 1 && step == 2)
	{//光标没有超出显示范围，但是输入的汉字超出显示范围
		m_iCurScrPos = 0;
		m_iCurScrLine ++;
	}
	else
	{
		m_iCurScrPos += len;
	}

	curLineLen = m_iaCharCountBuf[m_iCurScrLine - 1];

	memset(m_dispBuf,0,MAX_LINE_LEN);
	DBG_ASSERT_WARNING(0,("aPASSWORD can't support multiline input"));

	num = 0;
	for (int j = 0; j < m_iCurScrLine - 1; j ++)//计算当前行在字符串中的起始位置
	{
		num += m_iaCharCountBuf[j];
	}

	if(m_InputType == aPASSWORD && m_iTotalLine == 1)
	{
		memset(m_dispBuf,PASSWORD_CHAR,curLineLen);
	}
	else
	{
		memcpy(m_dispBuf,m_contentBuf+num,curLineLen);
	}
 	
}
void CaMultiLineInput::Erase(int len,int srcIndex,int conIndex)
{
	;;
}
void CaMultiLineInput::ReciveLeftDirectKey(int iEvent)
{
	int curLineLen;
	int oldScrStartIndex;
	int newScrStartIndex;
	int step;
	if(m_iConLen <=0 )
	{
		return;
	}


	if( m_iCurConPos <= 0 )
	{
		return;
	}

	oldScrStartIndex = m_iCurConPos-m_iCurScrPos;
	newScrStartIndex = oldScrStartIndex;
//GB2312
//	if( (m_contentBuf[m_iCurConPos-1] & 0x80) == 0x80  )
//	{
//		step=2;
//	}
//	else
//	{
//		step=1;
//	}

//GB18030
	if (is_gb_ex(m_contentBuf, m_iCurConPos-1) == 2)
	{
		step=2;
	}
	else
	{
		step=1;
	}

	m_iCurConPos -= step;
	m_iCurScrPos -= step;
	//光标需要回到上一行
	if(m_iCurScrPos < 0)
	{
		//计算出newScrStartIndex的位置
		newScrStartIndex -= m_iaCharCountBuf[m_iCurScrLine - 2];
		curLineLen = m_iaCharCountBuf[m_iCurScrLine - 2];

		m_iCurScrPos = curLineLen - step;	
		memset(m_dispBuf,0,MAX_LINE_LEN);
		memcpy(m_dispBuf,m_contentBuf+newScrStartIndex,curLineLen);
		m_iCurScrLine --;
	}
}
void CaMultiLineInput::ReciveRightDirectKey(int iEvent)
{
	int oldScrStartIndex;
	int newScrStartIndex;
	int step;
	int curLineLen;
	if(m_iConLen <=0 )
	{
		return;
	}

	oldScrStartIndex = m_iCurConPos-m_iCurScrPos;
	newScrStartIndex = oldScrStartIndex;

//GB2312
//	if((m_contentBuf[m_iCurConPos] & 0x80) == 0x80)
//	{
//		step=2;
//	}
//	else
//	{
//		step=1;
//	}
//GB18030
	if (is_gb_ex(m_contentBuf, m_iCurConPos) == 1)
	{
		step=2;
	}
	else
	{
		step=1;
	}

	if (m_iCurConPos + step > m_iConLen)//到了内容的尾部
	{
		return;
	}

	curLineLen = m_iaCharCountBuf[m_iCurScrLine - 1];
	m_iCurConPos += step;
	m_iCurScrPos += step;

	if (m_iCurScrPos >= curLineLen && curLineLen < GetCurLineMaxLen(m_iCurScrLine - 1) && m_iValidLine > m_iCurScrLine)//还有下行，但是这行由于最后的汉字这行了
	{//换行
		m_iCurScrPos = 0;
		m_iCurScrLine ++;
		newScrStartIndex += m_iaCharCountBuf[m_iCurScrLine - 2];
		curLineLen = m_iaCharCountBuf[m_iCurScrLine - 1];
		memset(m_dispBuf,0,MAX_LINE_LEN);
		memcpy(m_dispBuf,m_contentBuf+newScrStartIndex,curLineLen);
	}
	else if (m_iCurScrPos >= GetCurLineMaxLen(m_iCurScrLine - 1))//不换行不需要重新刷新m_dispBuf
	{
		if (m_iTotalLine == m_iCurScrLine)//最后一行
		{
			m_iCurScrPos -= step;
			m_iCurConPos -= step;
		}
		else//换行
		{
			m_iCurScrPos = 0;
			m_iCurScrLine ++;
			newScrStartIndex += m_iaCharCountBuf[m_iCurScrLine - 2];
			curLineLen = m_iaCharCountBuf[m_iCurScrLine - 1];
			memset(m_dispBuf,0,MAX_LINE_LEN);
			memcpy(m_dispBuf,m_contentBuf+newScrStartIndex,curLineLen);
		}
	}
}
void CaMultiLineInput::ReciveEraseKey(int iEvent)
{
	int curLineLen;
	int oldScrStartIndex;
	int newScrStartIndex;
	int step,step2;
	bool isPreLineFull;
	int i=0;
	if(m_iConLen <=0 )
	{
		return;
	}

	//已经在串的头部部 不能再动
	if(m_iCurConPos <=  0 )
	{
		return;
	}

	if (m_iCurScrLine > 1 && m_iaCharCountBuf[m_iCurScrLine - 2] < GetCurLineMaxLen(m_iCurScrLine - 2))
	{
		isPreLineFull = false;
	}
	else
	{
		isPreLineFull = true;
	}

	oldScrStartIndex=m_iCurConPos-m_iCurScrPos;
	newScrStartIndex = oldScrStartIndex;
	
//GB2312
//	if( (m_contentBuf[m_iCurConPos-1] & 0x80) == 0x80  )//汉字
//	{
//		step=2;
//	}
//	else
//	{
//		step=1;
//	}

//GB18030
	if (is_gb_ex(m_contentBuf, m_iCurConPos-1) == 2)
	{
		step=2;
	}
	else
	{
		step=1;
	}
	
	
	//移动buf
	if(m_iCurConPos < m_iConLen )
	{
		for(i = m_iCurConPos; i < m_iConLen;i++ )
		{
			m_contentBuf[i - step] = m_contentBuf[i];
		}

	}
	//在末尾删除
	for(i=0;i<step;i++)
	{
		m_contentBuf[m_iConLen - 1 - i] = 0;
	}
	
	m_iCurConPos -= step;
	m_iCurScrPos -= step;
	
	m_iConLen -= step;
	DBG_PRN("info",("m_iCurScrPos=%d,m_iCurConPos=%d m_iConLen=%d ",
		m_iCurScrPos,m_iCurConPos,m_iConLen));

	RecreateCharCountBuf();

	//光标需要上提一行
	if (m_iCurScrPos < 0)
	{
		if (m_iValidLine == m_iCurScrLine - 1)//光标后面没有字符
		{
			m_iCurScrPos = GetCurLineMaxLen(m_iCurScrLine - 2) - step;
		}
		else
		{
//			if( (m_contentBuf[m_iCurConPos] & 0x80) == 0x80  )
//			{
//				step2=2;
//			}
//			else
//			{
//				step2=1;
//			}
			//GB18030
			if (is_gb_ex(m_contentBuf, m_iCurConPos-1) == 2)
			{
				step2 = 2;
			}
			else
			{
				step2 = 1;
			}

			if (step2 == 1)//光标后是一个字符
			{
				m_iCurScrPos = GetCurLineMaxLen(m_iCurScrLine - 2) - step;;
			}
			else//光标后是一个汉字
			{
				if (step == 1 && isPreLineFull)
				{//光标不移动
					m_iCurScrPos = 0;
					m_iCurScrLine ++;//不换行
				}
				else
				{
					m_iCurScrPos = m_iaCharCountBuf[m_iCurScrLine - 2] - 2;
				}
			}
		}
		m_iCurScrLine --;
	}

	curLineLen = m_iaCharCountBuf[m_iCurScrLine - 1];
	newScrStartIndex = m_iCurConPos - m_iCurScrPos;
	memset(m_dispBuf,0,MAX_LINE_LEN);
	memcpy(m_dispBuf,m_contentBuf+newScrStartIndex,curLineLen);
}
void CaMultiLineInput::SetMaxLen(int mlen)
{
// 	DBG_ASSERT_EXIT(mlen < m_iTotalLine*m_iScrMaxLen,
// 		("Max content len(%d) is longer than screen_len * line (%d)",
// 		   mlen, m_iTotalLine*m_iScrMaxLen));
	int maxLen;
	if(m_labelPos == ON_LEFT )
	{
		maxLen = m_iTotalLine*MAX_LINE_LEN - m_iLbWidth/CHAR_W;
	}
	else
	{
		maxLen = m_iTotalLine*MAX_LINE_LEN;
	}
	m_iAllConMaxLen = mlen>maxLen?maxLen:mlen;
}
int CaMultiLineInput::SetContentBuf(unsigned char *pData, int iLen)
{
	int tmpLen, curLineLen;
	int step;
	DBG_ASSERT_EXIT((pData != NULL ),("pData is NULL"));
/*	DBG_ASSERT_EXIT((iLen > 0),("iLen=%d  is error!",iLen));*/
	if (iLen == 0)
	{
		Clear();
		return 0;
	}

	Clear();
	m_iConLen=iLen >m_iAllConMaxLen ? m_iAllConMaxLen:iLen;
	memcpy(m_contentBuf,pData,m_iConLen);
	RecreateCharCountBuf();
	curLineLen = m_iaCharCountBuf[m_iValidLine - 1];
	tmpLen = GetCurLineMaxLen(m_iValidLine - 1);
//	if( (m_contentBuf[m_iConLen - 1] & 0x80) == 0x80  )
//	{
//		step=2;
//	}
//	else
//	{
//		step=1;
//	}

	//GB18030
	if (is_gb_ex(m_contentBuf, m_iCurConPos-1) == 2)
	{
		step=2;
	}
	else
	{
		step=1;
	}
	
	
	m_iCurScrLine = m_iValidLine;

	if (curLineLen >= tmpLen)
	{
		if(m_iValidLine == m_iTotalLine)//最后一行没有位置显示光标，回退
		{
			m_iCurScrPos = curLineLen - step;
			m_iCurConPos = m_iConLen - step;
		}
		else//光标换行
		{
			m_iCurScrPos = 0;
			m_iCurConPos = m_iConLen;
			m_iCurScrLine ++;
			curLineLen = 0;
		}
	}
	else
	{
		m_iCurScrPos = curLineLen;
		m_iCurConPos = m_iConLen;
	}
	if(m_InputType == aPASSWORD)
	{
		memset(m_dispBuf,PASSWORD_CHAR,curLineLen);
	}
	else
	{
		memcpy(m_dispBuf,m_contentBuf + m_iConLen -curLineLen ,curLineLen);
	}
	ReFresh();
	return m_iConLen;
}
void CaMultiLineInput::Clear(void)
{
	for (int i = 1; i <= m_iValidLine; i ++)
	{
		LCDFillRect(m_iX+m_iLbOffSet,m_iY + GRID_LINE_H*(i-1),m_iW-m_iLbOffSet,GRID_LINE_H,0);
	}  
	memset(m_contentBuf,'\0',m_iAllConMaxLen);
	memset(m_dispBuf,'\0',MAX_LINE_LEN);
	memset(m_caImgBuf,'\0',MAX_LINE_LEN*IMG_BYTES);
	m_iConLen=0;
	m_iCurScrPos=0;
	m_iCurConPos=0;
	m_bDotFlag=false;
	m_iValidLine = 1;
	m_iCurScrLine = 1;
}
bool CaMultiLineInput::IsEmpty(void)
{
	int iBlankNum=0;
	if(m_iConLen == 0)
	{
		return true;
	}
	else
	{
		for(int i=0;i<m_iConLen;i++)
		{
			if(m_contentBuf[i] != BLANK )
			{
				break;
			}
			iBlankNum++;
		}
		
		if(iBlankNum == m_iConLen)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

int CaMultiLineInput::HalfHZCount( char *str, int beginIndex, int endIndex)
{
  DBG_ASSERT_EXIT(str != NULL && beginIndex >=0 && endIndex >= beginIndex,("str is NULL!"));
  int c=0;
	int p=beginIndex;
	while (p < endIndex)
  {
		if ((str[p] >= 0x81) && (str[p] <= 0xfe))
	 {
		 c++;
			p++;
			if (p < endIndex)
			{
				if ((str[p] >= 0x40) && (str[p] <= 0xfe))
				{
					c++;
					p++;
	 }
  }
		}
		else
		{
			p++;
		}
	}

  return c;
}

//从新计算每行的字符数
void CaMultiLineInput::RecreateCharCountBuf()
{
	int tmpCount = 0;
	int leftLen;
	int curLineMaxLen;
	if (m_iConLen <=0)
	{
		return;
	}
	
	memset((void *)m_iaCharCountBuf, 0, SYS_SUPPORT_MAX_LINES * sizeof(int));
	m_iValidLine = 0;
	for (int i = 0; i < m_iTotalLine; i ++)
	{
		curLineMaxLen = GetCurLineMaxLen(i);
		leftLen = m_iConLen - tmpCount;
		if (leftLen <= curLineMaxLen)//已经不足一行或者正好一行
		{
			m_iaCharCountBuf[i] = leftLen; 
			m_iValidLine ++;
			return;
		}

		if( HalfHZCount((char *)m_contentBuf,tmpCount, tmpCount + curLineMaxLen)%2 == 1)
		{
			m_iaCharCountBuf[i] = curLineMaxLen - 1;
			tmpCount += curLineMaxLen - 1;
		}
		else
		{
			m_iaCharCountBuf[i] = curLineMaxLen;
			tmpCount += curLineMaxLen;
		}
		m_iValidLine ++;
	}
}

int CaMultiLineInput::GetCurLineMaxLen(int curLineNum)
{
	int curLineLen;
	if (curLineNum == 0)
	{
		curLineLen = MAX_LINE_LEN - m_iLbOffSet/CHAR_W;
	}
	else
	{
		curLineLen = MAX_LINE_LEN;
	}
	return curLineLen;
}

