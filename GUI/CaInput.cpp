#include"CaInput.h"
#include"CaLabel.h"
#include"PosFrame.h"
#include "display.h"

#include "LOGCTRL.h"
#include"pos_debug.h"

extern "C" void LCDRedraw();


unsigned char CaInput::m_caFocusBuf[CaInput::FOCUS_BUF_LEN]={'\0'};
#if CHN_FONT_SIZE == 16
unsigned char CaInput::m_caFocusImgBuf[CaInput::FOCUS_IMG_LEN]={0,0,0,0,0,0,0,0,0,0,
												0,0,0,0,0,0,0,0,0,0};	
#else
unsigned char CaInput::m_caFocusImgBuf[CaInput::FOCUS_IMG_LEN]={0,0,0,0,0,0,0,0,0,0,
												0,0,0,0,0,0};	
#endif
int is_gb_ex(unsigned char *str, int position);
CaInput::CaInput(LABEL_POS lbPos,int lbWidth):CaObject(CA_INPUT,true)
{
	m_iConLen=0;
	m_iScrLen=0;
	m_iConMaxLen=DEFAULT_MAX_LEN;
	m_iScrMaxLen=0;
	m_iCurScrPos=0;
	m_iCurConPos=0;
	
	m_bReadOnly=false;
	m_bDotFlag=false;
	m_InputType=aCHAR;
	m_alignType=ALIGN_LEFT;
	m_labelPos=lbPos;
	m_iLbWidth=lbWidth;
	m_iLbOffSet=0;
	m_iBoxType=BOX_BOTTOM_LINE;
	m_bFocus=true;
	memset(m_contentBuf,0,MAX_BUF_LEN);
	memset(m_dispBuf,0,MAX_LINE_LEN);
	memset(m_caImgBuf,0,MAX_LINE_LEN*IMG_BYTES);

}
CaInput::CaInput():CaObject(CA_INPUT,true)
{
	 m_iConLen=0;
	 m_iScrLen=0;
	 m_iConMaxLen=DEFAULT_MAX_LEN;
	 m_iScrMaxLen=0;
	 m_iCurScrPos=0;
	 m_iCurConPos=0;
	 m_bFocus=true;
	m_bReadOnly=false;
	m_bDotFlag=false;
	m_InputType =aCHAR;
	m_alignType=ALIGN_LEFT;
	m_labelPos=ON_NULL;
	m_iLbWidth=0;
	m_iLbOffSet=0;
	m_iBoxType=BOX_BOTTOM_LINE;
		
	memset(m_contentBuf,0,MAX_BUF_LEN);
	memset(m_dispBuf,0,MAX_LINE_LEN);
	memset(m_caImgBuf,0,MAX_LINE_LEN*IMG_BYTES);
}
CaInput:: ~CaInput()
{

}

int	CaInput::Create(int iX,int iY,int iW,int iH)
{
	if(m_labelPos == ON_LEFT )
	{
		m_iConMaxLen=(iW-m_iLbWidth)/CHAR_W; //缺省值
		m_iScrMaxLen=m_iConMaxLen;
		m_iLbOffSet=m_iLbWidth;
	}
	else
	{
		m_iConMaxLen=iW/CHAR_W; //缺省值
		m_iScrMaxLen=iW/CHAR_W;
		m_iLbOffSet=0;

	}
	
	CaObject::Create(iX, iY, iW, iH);
	if(m_labelPos != ON_NULL)
	{
		DBG_ASSERT_EXIT((m_iLbWidth >0),("m_iLbWidth=%d",m_iLbWidth));
		m_pLabel=new CaLabel(false,ALIGN_RIGHT,CA_INNER_LABEL);
		switch(m_labelPos) {
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
int CaInput::SetFocusIn()
{
	if(!IsRefreshAble())
	{
		return 0;
	}
	int width = CHAR_W;
	if (is_gb_ex(m_dispBuf, m_iCurScrPos))
	{
 		width = 2*CHAR_W;
	}
// 	printf("width = %d\n", width);
	str2image(m_dispBuf+m_iCurScrPos,FOCUS_BUF_LEN*width/CHAR_W,m_caFocusImgBuf);
	LCDPutImageRev(m_iX+m_iLbOffSet+m_iCurScrPos*CHAR_W,m_iY,width,GRID_LINE_H,
		(char *)m_caFocusImgBuf+2);

//	LCDRedraw();
	m_bRefreshFlag=true;
//	DBG_PRN("------------",("LCDRedraw"));
	return 1;	
}
int CaInput::SetFocus()
{
	SetFocusIn();
    return	CaObject::SetFocus();
}
int CaInput::ReFresh()
{
	if (false == Visible())
	{
		return 0;
	}
	if(!IsRefreshAble())
	{
		return 0;
	}
	LCDFillRect(m_iX+m_iLbOffSet,m_iY,m_iScrMaxLen*CHAR_W,GRID_LINE_H,0);   
	LCDFillRectLine(m_iX+m_iLbOffSet,m_iY+GRID_LINE_H,
				m_iScrMaxLen*CHAR_W,INPUT_BOTTOM_LINE_H,1);
	str2image(m_dispBuf,m_iScrLen,m_caImgBuf);

	LCDPutImage(m_iX+m_iLbOffSet,m_iY,
		m_iScrLen*CHAR_W,GRID_LINE_H,(char *)m_caImgBuf+2);

	if(m_pLabel != NULL )
	{
		switch(m_labelPos) {
		case ON_LEFT:
			m_pLabel->Create(m_iX, m_iY, m_iLbWidth, m_iH);
			break;
		case ON_TOP:
			m_pLabel->Create(m_iX, m_iY-WORD_H, m_iLbWidth, m_iH);
			break;
		default:
			DBG_ASSERT_EXIT(false,("m_labelPos =%d error! ",(int)m_labelPos));
		}
		m_pLabel->ReFresh();
	}

//	LCDRedraw();
	m_bRefreshFlag=true;
//	DBG_PRN("------------",("LCDRedraw"));

	return 1;
}
int CaInput::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	int res=0;

	//处理手写板事件
	if ((iEvent == 0xFF) && (pEventData != NULL))
	{
		switch(m_InputType) 
		{
		case aINT:
			if(ReciveIntChar(0xFF,(char *)pEventData,strlen((char *)pEventData)) == 1 )
			{	res=1; }
			break;
		case aFLOAT:
			if(ReciveFloatChar(0xFF,(char *)pEventData,strlen((char *)pEventData)) == 1 )
			{	res= 1;	}
			break;
		case aPASSWORD:
//			res= 1;	
// 			break;
		case aCHAR:
			if(ReciveStrChar(0xFF,(char *)pEventData,strlen((char *)pEventData)) == 1 )
			{	res= 1;	}

//			memset(m_dispBuf,0,OBJ_TITLE_MAX_LEN);
//			strncpy(m_dispBuf,(char *)m_contentBuf,m_iTitleLen);
			break;
		default:
			break;
		}
	}

 	if (iEvent == CHANGE_INPUT_METHOD_EVENT && !m_bReadOnly 
		&& m_InputType!= aINT && m_InputType != aFLOAT && m_InputType!= aPASSWORD)
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
		else if(iEvent == ENTER_KEY || (iEvent >= 0x11 && iEvent <= 0x18))//TODO响应回车和功能键
		{
			CaObject::ProcEvent(iEvent,pEventData,iDataLen);
			res = 1;
		}
		else
		{
			res=0;
		}
	}

	if(res == 1)
	{
		ReFresh();
		if(IsCurObj())
		{
			SetFocusIn();
		}
	}

	DBG_PRN("info",("m_contentBuf = %s",m_contentBuf));
	return res;

}

int CaInput::ReciveIntChar(int iEvent,char *pData,int iLen)
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
	else if (iEvent == 0xFF)	//处理手写板事件
	{
		Insert(pData, iLen, m_iCurScrPos,m_iCurConPos);
		return 1;
	}
	else
	{
		return 0;
	}
}
int CaInput::ReciveFloatChar(int iEvent,char *pData,int iLen)
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
		m_bDotFlag=true;
		return 1;
	}
	else
	{
		return 0;
	}
}

int CaInput::ReciveStrChar(int iEvent,char *pData,int iLen)
{

	if(ReciveIntChar(iEvent,pData,iLen) == 1 )
	{
		return 1;
	}

//	DBG_PRN("info", ("iEvent = %u , %c", iEvent, iEvent));
	if(    (iEvent >= 'a' && iEvent <= 'z' ) 
		|| (iEvent >= 'A' && iEvent <= 'Z' )
		|| (iEvent == DOT_KEY )
		|| (iEvent == '@')
		|| (iEvent == '#')
		|| (iEvent == '$')
		|| (iEvent == '^')
		|| (iEvent == '&')
		|| (iEvent == '(')
		|| (iEvent == ')')
		|| (iEvent == '%')
		|| (iEvent == '*')
		|| (iEvent == '-')
		|| (iEvent == '<')
		|| (iEvent == '>')
		|| (iEvent == '\\')
		|| (iEvent == '/')
		|| (iEvent == '_')
		|| (iEvent == '\'')
		|| (iEvent == '"')
		|| (iEvent == '~')
		|| (iEvent == ' ')
		) 
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
void CaInput::Insert(char *pData,int len,int scrIndex,int conIndex)
{
	
	if(m_iConLen+len > m_iConMaxLen )
	{
		DBG_ASSERT_WARNING(false,
			("m_iConLen=%d >= m_iConMaxLen=%d",m_iConLen,m_iConMaxLen));
		return ;
	}
	int	i=0;
   //	DBG_PRN("info",("m_iCurScrPos=%d,m_iCurConPos=%d",m_iCurScrPos,m_iCurConPos));

	int newScrPos=m_iCurScrPos;
	int oldScrStartIndex=m_iCurConPos-m_iCurScrPos;
	int newScrStartIndex=oldScrStartIndex;
	DBG_ASSERT_WARNING(oldScrStartIndex >=0 ,
		("oldScrStartIndex=%d is invalid ",oldScrStartIndex));
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
		newScrPos++;
	}

    //计算显示内容的位置
	//虚拟焦点没有到输入框的最右边，此时不发生滚动
	if(newScrPos < m_iScrMaxLen )
	{
		//输入框的起始位置与原先的相同
		newScrStartIndex=oldScrStartIndex;
	}//如果输入到了结束
	else if( newScrPos == m_iScrMaxLen && oldScrStartIndex+newScrPos >= m_iConMaxLen)
	{
		newScrPos--;
		newScrStartIndex=oldScrStartIndex;
	}
	else //此时发生滚动
	{
		
		int scrollLen=0;
		//先将虚拟焦点移到输入框的最右边，计算输入框起点在内容上位置
		newScrStartIndex=oldScrStartIndex + newScrPos + 1 -m_iScrMaxLen;
		newScrPos=m_iScrMaxLen-1;

		//再将输入框内容右移SCROLL_LEN
		if(m_iScrMaxLen <= SCROLL_LEN )
		{
			scrollLen=1;
		}
		else
		{
			scrollLen=SCROLL_LEN;
		}

		newScrStartIndex += scrollLen; 
		newScrPos -= scrollLen;
	}

	//计算显示内容的长度
	//内容的尾部还没到输入框的最右边
	if( newScrStartIndex+m_iScrMaxLen >= m_iConLen )
	{
		m_iScrLen=m_iConLen-newScrStartIndex;
	}
	else //内容的尾部超过了输入框的最右边
	{
		m_iScrLen=m_iScrMaxLen;
	}
	
	//针对显示首字符为半个汉字调整位置
	//新起点之前的半个汉字个数
	if((HalfHZCount((char *)m_contentBuf,newScrStartIndex)%2) == 1 )
	{
		//新起点再向右移一个字符
		newScrStartIndex++;
		newScrPos--;
		//重新调整显示内容长度
		if( newScrStartIndex+m_iScrMaxLen >= m_iConLen )
		{
			m_iScrLen=m_iConLen-newScrStartIndex;
		}
		else //内容的尾部超过了输入框的最右边
		{
			m_iScrLen=m_iScrMaxLen;
		}
	}

	if (is_gb_ex(m_contentBuf+newScrStartIndex, m_iScrLen-1) == 1)
	{
		m_iScrLen--;
	}
	m_iCurScrPos=newScrPos;
	memset(m_dispBuf,0,MAX_LINE_LEN);
	if(m_InputType == aPASSWORD )
	{
		memset(m_dispBuf,PASSWORD_CHAR,m_iScrLen);
	}
	else
	{
		memcpy(m_dispBuf,m_contentBuf+newScrStartIndex,m_iScrLen);
	}
	
//	DBG_PRN("info",("m_iCurScrPos=%d,m_iCurConPos=%d",m_iCurScrPos,m_iCurConPos));
	//已经在串的尾部 不能再动

	if(m_iCurConPos >= m_iConMaxLen )
	{
		if(m_iCurScrPos >= m_iScrMaxLen-1)
		{
			int tmp=m_iCurConPos;
			m_iCurConPos = m_iConLen;
			m_iCurConPos = m_iCurConPos >= m_iConMaxLen ? m_iConMaxLen-1 :m_iCurConPos ;
			m_iCurScrPos=m_iCurConPos-newScrStartIndex;
		}
	}

//	DBG_PRN("info",("m_contentBuf=%s",(char *)m_contentBuf));
//	DBG_PRN("info",("m_iCurScrPos=%d,m_iCurConPos=%d",m_iCurScrPos,m_iCurConPos));


}
void CaInput::Erase(int len,int srcIndex,int conIndex)
{
	;;
}
void CaInput::ReciveLeftDirectKey(int iEvent)
{
	DBG_ASSERT_EXIT( (iEvent == INPUT_LEFT ),("iEvent =%d is not INPUT_LEFT "));
//	DBG_PRN("info",("m_iCurScrPos=%d,m_iCurConPos=%d",m_iCurScrPos,m_iCurConPos));
	if(m_iConLen <=0 )
	{
		return;
	}


	if( m_iCurConPos <= 0 )
	{
		m_iCurConPos = 0;
		m_iCurScrPos = 0;
		return;
	}

	int oldScrStartIndex=m_iCurConPos-m_iCurScrPos;
	int newScrStartIndex=oldScrStartIndex;
	int step=0;

	if (is_gb_ex(m_contentBuf, m_iCurConPos-1) == 2)
	{
		step=2;
	}
	else
	{
		step=1;
	}

	m_iCurConPos -=step;
	
	//需要滚动
	if(m_iCurConPos < oldScrStartIndex )
	{
		int scrollLen= m_iScrMaxLen > SCROLL_LEN? SCROLL_LEN:1;
 		//
		if(oldScrStartIndex < scrollLen  )
		{
			newScrStartIndex=0;
		}
		else
		{
			newScrStartIndex=oldScrStartIndex-scrollLen; 

		}


		//针对显示首字符为半个汉字调整位置
		//新起点之前的半个汉字个数
		if((HalfHZCount((char *)m_contentBuf,newScrStartIndex)%2) == 1 )
		{
			//新起点再向左移一个字符
			newScrStartIndex--;
		}
		m_iCurScrPos=m_iCurConPos-newScrStartIndex;

		//计算显示内容长度
		if( newScrStartIndex+m_iScrMaxLen >= m_iConLen )
		{
			m_iScrLen=m_iConLen-newScrStartIndex;
		}
		else //内容的尾部超过了输入框的最右边
		{
			m_iScrLen=m_iScrMaxLen;
		}

		if (is_gb_ex(m_contentBuf+newScrStartIndex, m_iScrLen-1) == 1)
		{
			m_iScrLen--;
		}

		memset(m_dispBuf,0,MAX_LINE_LEN);
		if(m_InputType == aPASSWORD )
		{
			memset(m_dispBuf,PASSWORD_CHAR,m_iScrLen);
		}
		else
		{
			memcpy(m_dispBuf,m_contentBuf+newScrStartIndex,m_iScrLen);
		}
	}
	else
	{
		m_iCurScrPos-=step;
	}
}
void CaInput::ReciveEraseKey(int iEvent)
{
	DBG_ASSERT_EXIT( (iEvent == CLEAR_ONE_EVENT  ),
		("iEvent =%d is not CLEAR_ONE_EVENT"));
	if(m_iConLen <=0 )
	{
		return;
	}

	//已经在串的头部部 不能再动
	if(m_iCurConPos <=  0 )
	{
		m_iCurConPos = 0;
		m_iCurScrPos = 0;
		return;
	}

	int oldScrStartIndex=m_iCurConPos-m_iCurScrPos;
	int newScrStartIndex=oldScrStartIndex;
	int step=0;
	int i=0;
//	if( (m_contentBuf[m_iCurConPos-1] & 0x80) == 0x80  )
//	if( (m_contentBuf[m_iCurConPos-1] >= 0x40) 
//		&& (m_contentBuf[m_iCurConPos-1] <= 0xfe))
	if (m_contentBuf[m_iCurConPos-1] == DOT_KEY)//处理删除'.'情况
	{
		m_bDotFlag = false;
	}

	if (is_gb_ex(m_contentBuf, m_iCurConPos-1) == 2)
	{
		step=2;
	}
	else
	{
		step=1;
	}
	
	m_iCurConPos -=step;
	m_iConLen -= step;
	
	//移动buf
	if(m_iCurConPos < m_iConLen )
	{
		for(i=m_iCurConPos;i<m_iConLen;i++ )
		{
			m_contentBuf[i]=m_contentBuf[i+step];
			//m_contentBuf[i+step]=0;
		}
		m_contentBuf[m_iConLen]=0;

	}//在末尾删除
	else 
	{
		for(i=0;i<step;i++)
		{
			m_contentBuf[m_iConLen+i]=0;
		}
	}
	//需要滚动
	if(m_iCurConPos < oldScrStartIndex )
	{
		int scrollLen= m_iScrMaxLen > SCROLL_LEN? SCROLL_LEN:1;
 		//
		if(oldScrStartIndex < scrollLen  )
		{
			newScrStartIndex=0;
		}
		else
		{
			newScrStartIndex=oldScrStartIndex-scrollLen; 
		}


		//针对显示首字符为半个汉字调整位置
		//新起点之前的半个汉字个数
		if((HalfHZCount((char *)m_contentBuf,newScrStartIndex)%2) == 1 )
		{
			//新起点再向左移一个字符
			newScrStartIndex--;
		}
		m_iCurScrPos=m_iCurConPos-newScrStartIndex;
	}
	else
	{
		m_iCurScrPos-=step;
	}

	//计算显示内容长度
	if( newScrStartIndex+m_iScrMaxLen >= m_iConLen )
	{
		m_iScrLen=m_iConLen-newScrStartIndex;
	}
	else //内容的尾部超过了输入框的最右边
	{
		m_iScrLen=m_iScrMaxLen;
	}

	if (is_gb_ex(m_contentBuf+newScrStartIndex, m_iScrLen-1) == 1)
	{
		m_iScrLen--;
	}

	memset(m_dispBuf,0,MAX_LINE_LEN);
	if(m_InputType == aPASSWORD )
	{
		memset(m_dispBuf,PASSWORD_CHAR,m_iScrLen);
	}
	else
	{
		memcpy(m_dispBuf,m_contentBuf+newScrStartIndex,m_iScrLen);
	}
	
}
void CaInput::ReciveRightDirectKey(int iEvent)
{
	DBG_ASSERT_EXIT( (iEvent == INPUT_RIGHT  ),
		("iEvent =%d is not INPUT_RIGTH"));
	if(m_iConLen <=0 )
	{
		return;
	}

	int oldScrStartIndex=m_iCurConPos-m_iCurScrPos;
	int newScrStartIndex=oldScrStartIndex;
	int step=0;
	
	//已经在串的尾部 不能再动
	if(  ( m_iCurConPos >= m_iConLen ) )//	|| m_iCurConPos == m_iConMaxLen-1 )
	{
		if(m_iCurScrPos >= m_iScrMaxLen-1)
		{
			m_iCurConPos = m_iConLen;
			m_iCurConPos = m_iCurConPos >= m_iConMaxLen ? m_iConMaxLen:m_iCurConPos ;
			m_iCurScrPos = m_iCurConPos-newScrStartIndex;
		}
		return;
	}

	if (is_gb_ex(m_contentBuf, m_iCurConPos))
	{
		step=2;
	}
	else
	{
		step=1;
	}
	m_iCurConPos +=step;

//	if (is_gb_ex(m_contentBuf, m_iCurConPos) == 1)
	 //需要滚动
	if( (m_iCurConPos >= oldScrStartIndex + m_iScrMaxLen )
		|| ((is_gb_ex(m_contentBuf, m_iCurConPos) == 1) && (m_iCurConPos+1 == oldScrStartIndex+m_iScrMaxLen)))
	{
		int scrollLen= m_iScrMaxLen > SCROLL_LEN? SCROLL_LEN:1;
		int tmp=scrollLen;
		if((m_iConLen - oldScrStartIndex - m_iScrMaxLen ) <
			 scrollLen )
		{
			tmp=m_iConLen - oldScrStartIndex - m_iScrMaxLen;
		}

		if( tmp < scrollLen )
		{
			tmp=m_iConMaxLen - oldScrStartIndex - m_iScrMaxLen;
			tmp= tmp > scrollLen ? scrollLen :tmp;
		}
		
		newScrStartIndex += tmp;
		//调整
		if( HalfHZCount((char *)m_contentBuf,newScrStartIndex)%2 == 1)
		{
			DBG_PRN("info",("here"));
			newScrStartIndex++;
		}
		m_iCurScrPos = m_iCurConPos-newScrStartIndex;

		//计算显示内容长度
		if( newScrStartIndex+m_iScrMaxLen >= m_iConLen )
		{
			m_iScrLen=m_iConLen-newScrStartIndex;
		}
		else //内容的尾部超过了输入框的最右边
		{
			m_iScrLen=m_iScrMaxLen;
		}
		if (is_gb_ex(m_contentBuf+newScrStartIndex, m_iScrLen-1) == 1)
		{
			m_iScrLen--;
		}

		memset(m_dispBuf,0,MAX_LINE_LEN);
		if(m_InputType == aPASSWORD )
		{
			memset(m_dispBuf,PASSWORD_CHAR,m_iScrLen);
		}
		else
		{
			memcpy(m_dispBuf,m_contentBuf+newScrStartIndex,m_iScrLen);
		}

	}
	else
	{
		m_iCurScrPos +=step;
	}
	
	//已经在串的尾部 不能再动
	if(m_iCurConPos >= m_iConLen /*|| m_iCurConPos == m_iConMaxLen-1 */)
	{
		m_iCurConPos = m_iConLen;
		if(m_iCurScrPos >= m_iScrMaxLen-1)
		{
			m_iCurConPos = m_iCurConPos >= m_iConMaxLen ? m_iConMaxLen :m_iCurConPos ;
		//	m_iCurScrPos = m_iCurScrPos >= m_iScrMaxLen ? m_iScrMaxLen-1 :m_iCurScrPos ;
		}
		m_iCurScrPos = m_iCurConPos-newScrStartIndex;
	}
}
int CaInput::SetContentBuf(unsigned char *pData, int iLen)
{
	DBG_ASSERT_EXIT((pData != NULL ),("pData is NULL"));
	//DBG_ASSERT_EXIT((iLen > 0),("iLen=%d  is error!",iLen));
	if (iLen == 0)
	{
		Clear();
		ReFresh();
		return 0;
	}
	Clear();
	m_iConLen=iLen >MAX_BUF_LEN ? MAX_BUF_LEN:iLen;
	m_iScrLen=iLen > (m_iScrMaxLen - 1) ? (m_iScrMaxLen - 1) : iLen;

	if (is_gb_ex(pData, iLen - m_iScrLen) == 2)
	{
		m_iScrLen--;
	}

	if (!m_bReadOnly)//非只读的光标在最后
	{
		m_iCurScrPos=m_iScrLen;
		m_iCurConPos=m_iConLen;
	}
	else
	{
		m_iCurScrPos = 0;
		m_iCurConPos = 0;
	}
	memcpy(m_contentBuf,pData,m_iConLen);
	if(m_InputType == aPASSWORD)
	{
		memset(m_dispBuf,PASSWORD_CHAR,m_iScrLen);
	}
	else
	{
		if (!m_bReadOnly)
		{
			memcpy(m_dispBuf,pData + m_iConLen - m_iScrLen,m_iScrLen);
		}
		else
		{
			memcpy(m_dispBuf,pData,m_iScrLen);
		}
	}
	ReFresh();
	return m_iConLen;
}
void CaInput::Clear(void)
{
	LCDFillRect(m_iX+m_iLbOffSet,m_iY,m_iW-m_iLbOffSet,m_iH,0);   
	memset(m_contentBuf,'\0',MAX_BUF_LEN);
	memset(m_dispBuf,'\0',MAX_LINE_LEN);
	memset(m_caImgBuf,'\0',MAX_LINE_LEN*IMG_BYTES);
	m_iConLen=0;
	m_iScrLen=0;
	m_iCurScrPos=0;
	m_iCurConPos=0;
	m_bDotFlag=false;
}
bool CaInput::IsEmpty(void)
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


int CaInput::HalfHZCount( char *str,int len)
{
	DBG_ASSERT_EXIT(str != NULL,("str is NULL!"));
	int c=0;
	int p=0;
	while (p < len)
	{
		if (((str[p]&0xff) >= 0x81) && ((str[p]&0xff) <= 0xfe))
		{
			c++;
			p++;
			if (p < len)
			{
				if (((str[p]&0xff) >= 0x40) && ((str[p]&0xff) <= 0xfe))
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

int is_gb_ex(unsigned char *str, int position) 
{
	int i;
	int flag = 0;
 
	for(i=0; i<=position; )
	{
		if ((str[i] >= 0x81) && (str[i] <= 0xfe) &&
			(str[i+1] >= 0x40) && (str[i+1] <= 0xfe))
		{
			i = i+2;
			flag = 1;
		}
		else
		{
			i++;
			flag = 0;
		}
	}
 
	if (flag == 1)
	{
		if (i == position+1)
		{
			return 2;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 0;
	}

}

