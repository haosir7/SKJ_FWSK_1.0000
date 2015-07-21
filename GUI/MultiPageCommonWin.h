#ifndef MULTI_PAGE_COMMON_WIN_H
#define MULTI_PAGE_COMMON_WIN_H
#include"CaWindow.h"
#include"pos_debug.h"
#include "g_def.h"
#include "CaLabel.h"
#include "CaButton.h"

#define		PAGE_LINE_NUM	5

/*!
@class 多页（每页全是label 或全是button）
*/
class CMultiPageCommonWin : public CaWindow
{
private:
	CaButton *m_pBtn1;//当前页第1个按钮
	CaButton *m_pBtn2;//当前页第2个按钮
	CaButton *m_pBtn3;//当前页第3个按钮
	CaButton *m_pBtn4;//当前页第4个按钮
	CaButton *m_pBtn5;//当前页第5个按钮

	CaLabel *m_pLabel1;//当前页第1个label
	CaLabel *m_pLabel2;//当前页第2个label
	CaLabel *m_pLabel3;//当前页第3个label
	CaLabel *m_pLabel4;//当前页第4个label
	CaLabel *m_pLabel5;//当前页第5个label

protected:
	CaLabel *m_pPageLabel;//显示页码的label
	CaLabel *m_pPageLabel_2;//备用的显示页码的label

	int m_iCurWinLastComIndex;//当前窗口最后一行在总的行数中的index
	int m_iComType;//全是label 还是 全是button
	int m_iPressedBtnIndex;//当按enter键时按下的是第几个按钮
	int m_iTotalLine;//总行数
	int m_iCurPageNum;//当前页的页码
	int m_iTotalPage;//总页数
public:
	char **m_pStrList;//存放labels 或者buttons 的titles
public:
	enum{MULTI_LABELS_MULTI_PAGE,MULTI_BUTTONS_MULTI_PAGE};
	
	CMultiPageCommonWin(int type);

	virtual int Create(int iX,int iY,int iW,int iH);
	virtual int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	virtual ~CMultiPageCommonWin();
	virtual int ReFresh(void);
	virtual bool IsCurWin(void);
	virtual void EndProcess();//浏览完以后的处理
	virtual void OnBtnPressed();//根据用户选择做处理

	void ClearListBuff(void);
	void SetTitles(char ** titles, int totalLine);
	int GetSelectedIndex();
	//计算获得焦点的行
	int GetFocusIndex();
	inline int GetTotalPageNum(){return m_iTotalPage;}
	inline int GetCurPageNum(){return m_iCurPageNum;}
	inline int GetTotalLineNum(){return m_iTotalLine;}
	void GetNextPage();
	void GetPrePage();

	//在设置好m_pStrList后 调用设置第一页显示
	/*	@brief 设置要显示的页，默认是第一页，可以根据参数显示任意页
		@totalLine 总的行数
		@curPageNum 要显示第几页
		@startIndex 要显示的内容在m_pStrList中的起始索引
	*/
	void InitFirstPage(int totalLine, int curPageNum = 1, int startIndex = 0);


	void SetPageNumVisible(bool visible);
	void SetPageNumVisible_2(bool visible);

private:
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton4(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton5(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	/*!@brief 重新设置每页显示的行,显示前endNum行 	*/
	void ResetVisiable(int endNum);

protected:	

	virtual void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnButton3(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnButton4(int iEvent, unsigned char * pEventData, int iDataLen);
	virtual void OnButton5(int iEvent, unsigned char * pEventData, int iDataLen);

};

#endif


