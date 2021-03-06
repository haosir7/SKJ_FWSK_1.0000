 /* Generated by Together */

#ifndef CAMULTILINEINPUT_H
#define CAMULTILINEINPUT_H
#include "CaObject.h"

//struct InputVal_t
//{
//	INPUT_TYPE  Type;
//	union 
//	{
//		int iVal;
//		double  fVal;
//		unsigned char  caVal[OBJ_TITLE_MAX_LEN];
//	};
//};
class CaMultiLineInput : public CaObject {
public:
	enum{MAX_BUF_LEN=255};
	enum{MAX_LINE_LEN = 30};
	enum{SYS_SUPPORT_MAX_LINES = 4};//最大支持多少行
	enum{FOCUS_BUF_LEN=1,FOCUS_IMG_LEN=20};
	enum{INPUT_BOTTOM_LINE_H=1,SCROLL_LEN=4};
typedef enum _INPUT_TYPE{aCHAR=0,aINT,aFLOAT,aPASSWORD,aNULL}TYPE;
public:
	/*! 当前光标在第几行*/
	int m_iCurScrLine;
	/*! 最多几行*/
	int m_iTotalLine;
	/*! 现在几行有内容*/
	int m_iValidLine;
	/*! 内容的最大长度*/
	int m_iAllConMaxLen;

	/*! 当前内容长度*/
	int m_iConLen;
	/*! 在屏幕上当前行显示得长度*/
/*	int m_iScrLen;*/
	/*! 屏幕当前行显示得最大长度*/
/*	int m_iScrMaxLen; */
	/*! 光标在当前行屏幕上的位置*/
	int m_iCurScrPos; 
	/*! 光标在内容上的位置 */
	int m_iCurConPos; 
	int m_iLbWidth;
	int m_iLbOffSet;
	
	bool m_bReadOnly;
	bool m_bDotFlag;
	TYPE m_InputType;
	ALIGN_TYPE m_alignType;
	LABEL_POS m_labelPos;
	
	/*! 输入的内容 */
	unsigned char m_contentBuf[MAX_BUF_LEN];
	/*! 当前行显示的内容 */
	unsigned char m_dispBuf[MAX_LINE_LEN];//当前行内容，用来反显光标
	unsigned char m_caImgBuf[MAX_LINE_LEN*IMG_BYTES+4];
	//每行有几个字符
	int m_iaCharCountBuf[SYS_SUPPORT_MAX_LINES];
	
static	unsigned char m_caFocusBuf[FOCUS_BUF_LEN];
static	unsigned char m_caFocusImgBuf[FOCUS_IMG_LEN];	
protected:
	virtual int  ReciveIntChar(int iEvent,char *pData,int iLen);
	virtual int	 ReciveFloatChar(int iEvent,char *pData,int iLen);
	virtual int	 ReciveStrChar(int iEvent,char *pData,int iLen);
	virtual void Insert(char *pData,int len,int scrIndex,int conIndex);
	virtual void Erase(int len,int srcIndex,int conIndex);
	virtual void ReciveLeftDirectKey(int iEvent);
	virtual void ReciveRightDirectKey(int iEvent);
	virtual void ReciveEraseKey(int iEvent);

	void RecreateCharCountBuf();
public:
	/*!
		@brief 真正创建控件	
		@param iX 左上角横坐标
		@param iY 左上角纵坐标
		@param iW 宽度
		@param iH 高度
		@param pOwner 所属管理控件句柄
		@return <=-1 失败，>=0 成功
	*/
   virtual int	Create(int iX,int iY,int iW,int iH);
   virtual int  SetFocusIn();
   virtual int  SetFocus();
   virtual	int ReFresh();
   virtual int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
   CaMultiLineInput(LABEL_POS lbPos,int lbWidth, int totalLine = 1);
   CaMultiLineInput(int totalLine = 1);
   virtual int SetContentBuf(unsigned char *pData, int iLen);
   virtual void Clear(void);
   virtual bool IsEmpty(void);
   virtual void SetMaxLen(int mlen);
   virtual void SetAlign(ALIGN_TYPE align){m_alignType=align;};
   static bool IsHZ(unsigned char *str);
   static int HalfHZCount(char *str, int beginIndex, int endIndex);

private:
	//根据当前行,计算当前行的最大长度
	int GetCurLineMaxLen(int curLineNum);

};
#endif //CAMULTILINEINPUT_H





