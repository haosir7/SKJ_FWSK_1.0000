#ifndef CALABLEBAR_H
#define CALABLEBAR_H
#include "CaObject.h"
#include "CaLabel.h"

#define LABEL_BAR_MAX_LEN 32

class CaLabelBar : public CaLabel {
public:
	CaLabelBar();
	virtual ~CaLabelBar();	
	virtual int ReFresh(void);
	void UpdateCurPos(int pos);
private:
	unsigned char m_caTitleImgBuf[LABEL_BAR_MAX_LEN*IMG_BYTES];
	int m_iCurPos;
	void DynamicChange();
	
};
#endif //CALABLEBAR_H

