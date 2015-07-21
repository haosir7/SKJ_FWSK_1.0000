#include "CaLabelBar.h"

#include"LOGCTRL.h"
#include"pos_debug.h"

CaLabelBar::CaLabelBar()
{
	m_iBoxType=BOX_ALL;
	m_iType=CA_LABEL;
	memset( m_caTitleImgBuf, 0, LABEL_BAR_MAX_LEN*IMG_BYTES );
	m_iCurPos = 0;
}

CaLabelBar::~CaLabelBar()
{

}

int CaLabelBar::ReFresh()
{
	DynamicChange();
	DrawBox();
	return 1;
}

void CaLabelBar::UpdateCurPos(int pos)
{
	DBG_ASSERT_EXIT(pos>=0 && pos<= 100, ("invalid progress bar position"));
	m_iCurPos = pos;
}

void CaLabelBar::DynamicChange()
{//产生动态效果未完成
	if (m_iCurPos)
	{
		LCDFillRect(m_iX+(m_iW-(m_iW/CHAR_W)*CHAR_W)/2,m_iY,
		(m_iW/CHAR_W)*CHAR_W*m_iCurPos/100,GRID_LINE_H,1);
	}
		
}


