/* Generated by Together */

#ifndef CASCROLLBAR_H
#define CASCROLLBAR_H
#include "CaObject.h"

class CaScrollBar : public CaObject {
private:
	int m_iTotalLines;
	int m_iDispLines;
	int m_iCurIndex;
	int m_iPreIndex;
	
//	double m_iStep;
public:
	int TotalLines()const{return m_iTotalLines;};
	void TotalLines(int t){m_iTotalLines=t;};
	int DispLines()const {return m_iDispLines;};
	void DispLines(int d){m_iDispLines=d;}
	int CurIndex()const{return m_iCurIndex;};
	void CurIndex(int i);
public:
    CaScrollBar();    
    virtual ~CaScrollBar();
	virtual int ReFresh(void);
	virtual int Create(int iX,int iY,int iW,int iH);
	virtual void DrawBox();
private:
	int GetBarPosition(void);
	int GetBarLength(void);
//    CaScrollGroup *m_pScrollGroup;
};
#endif //CASCROLLBAR_H




