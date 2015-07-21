#include"StringWrap.h"
#include"string.h"
#include"ctype.h"

#include"LOGCTRL.h"
#define NO_POS_DEBUG
#include"pos_debug.h"

CStringWrap::CStringWrap(const char * sourStr,int maxWidth)
{
	m_pSourceStr=sourStr;
	m_iMaxWidth=maxWidth;
	m_pDestStr=NULL;
	m_iLines=0;
	m_iWidth=0;
	m_pPositions=NULL;
	m_pWidths=NULL;
	memset(m_cTempBuf,0,MAX_WIDTH+1);	
}
CStringWrap::~CStringWrap()
{
	Clear();
}
void CStringWrap::Clear()
{
	m_iLines=0;
	m_iWidth=0;
	memset(m_cTempBuf,0,MAX_WIDTH+1);	
	
	if(m_pDestStr != NULL )
	{
		free(m_pDestStr);
		m_pDestStr=NULL;
	}
	if(m_pPositions != NULL )
	{
		free(m_pPositions);
		m_pPositions=NULL;
	}
	if(m_pWidths != NULL )
	{
		free(m_pWidths);
		m_pWidths=NULL;
	}
}
char *CStringWrap::Line(int iLineID)
{
	DBG_ASSERT_EXIT(iLineID < m_iLines,("iLineID=%d,m_iLines=%d ,input is invalid!"));	
	memset(m_cTempBuf,0,MAX_WIDTH+1);	
	memcpy(m_cTempBuf,m_pDestStr+m_pPositions[iLineID],m_pWidths[iLineID]);
	DBG_PRN("info",("m_pWidths[%d]=%d",iLineID,m_pWidths[iLineID]));
	return m_cTempBuf;
}
int   CStringWrap::LineWidth(int iLineID)
{
	DBG_ASSERT_EXIT(iLineID < m_iLines,("iLineID=%d,m_iLines=%d ,input is invalid!"));	
	return m_pWidths[iLineID];
}
void CStringWrap::Wrap()
{
	DBG_ASSERT_EXIT(m_pSourceStr != NULL,("m_pSourceStr is NULL "));
	Clear();
	int len=strlen(m_pSourceStr);
	len=len+len/m_iMaxWidth+5;
	int maxLineCount=len/m_iMaxWidth +5;
	m_pDestStr=(char*)malloc(len*sizeof(char));
	m_pPositions=(int *)malloc(maxLineCount*sizeof(int*));
	m_pWidths=(int *)malloc(maxLineCount*sizeof(int*));
	stringwrap(m_pSourceStr,m_pDestStr,m_iMaxWidth,m_iLines,m_iWidth,m_pPositions,m_pWidths);
}

//mod by flb 20070409
void CStringWrap::stringwrap(const char *sourcestr, 
							 char *deststr,
							 const int maxwidth,  
							 int &lines,  
							 int &width ,
							 int *positions,
							 int *widths)
{
    int indexsour, indexdest;
    int lenthsour, gbnum;
    int i, k;
//	int j=0;
    int totalchars;
    unsigned char c=0;
	
	int oldlines=lines;
	
	
    lines = 0;
	
    lenthsour = strlen(sourcestr);
    if(lenthsour <= 0)
    {
        deststr="";
        return;
    }
    width = 0;
    k = 0;
	
	positions[0]=0;
	DBG_ASSERT_EXIT(maxwidth > 2,("maxwidth=%d,maxwidth must large than 2",maxwidth));
    if(lenthsour <= maxwidth)
    {
        lines = 1;
        for(i=0; i<lenthsour; i++)
        {
            k++;
            deststr[i] = sourcestr[i];
            if(deststr[i] == '\n')
            {
				widths[lines]=k-1;
                lines++;
				positions[lines]=i;	
                if(k > width)
                {
                    width = k-1;
                }
                k = 0;
            }
        }
        if(k>width)
			width=k;
		widths[lines]=k;
        return;
    }
	
    width = maxwidth;
    indexsour = 0;
    indexdest = 0;
    gbnum = 0;//一行中半个汉字个数
    k = 0;
    lines = 1;
    totalchars =strlen(sourcestr);
    while(indexsour < totalchars)
    {
        k++;
        if(k <= maxwidth)
        {
            c = sourcestr[indexsour];
            deststr[indexdest] = c;
            if(c == '\n')
            {
				positions[lines]=indexdest+1;
				widths[lines-1]=k-1;
                k = 0;
                gbnum = 0;
                lines++;
            }
            else if(c >= 0xa0)
             //else if(is_gb_ex((unsigned char *)sourcestr, indexsour))
            {
                gbnum++;
            }
            indexsour++;
            indexdest++;
        }
        else //( k== maxwidth+1)
		{
			if( gbnum%2 == 1 ) //拆分时碰到一个汉字被拆分的情况 ,将这半个汉字移到下一行
			{
				deststr[indexdest] = deststr[indexdest-1];
				deststr[indexdest-1]  = '\n';
				positions[lines]=indexdest;
				widths[lines-1]=k-2; //由于 maxwidth 大于2 所以k肯定大于2 
				k=1;
				gbnum=1; //此时下一行已经有半个汉字了
			}
			else 
			{
				deststr[indexdest]  = '\n';
				positions[lines]=indexdest+1;
				widths[lines-1]=k-1;
				k=0;
				gbnum=0;
			}
			lines++;
			indexdest++; 
		}
    }
	widths[lines-1]=k;	
}



