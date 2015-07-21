// InvRetServerWin.h: interface for the CInvRetServerWin class.
//
//////////////////////////////////////////////////////////////////////

#ifndef __INV_RET_TO_SERVER_H
#define __INV_RET_TO_SERVER_H

#include "CaWindow.h"
#include "CaInput.h"
#include "CaButton.h"
#include "comdatatype.h"
#include <string>
using namespace std;
#include "CInvVol.h"

class CInvRetServerWin : public CaWindow  
{
private:
	CaInput *m_pInput1, *m_pInput2, *m_pInput3; /**< 指向输入栏的指针 */
	CaButton *m_pBtn1, *m_pBtn2;				/**< 指向按钮的指针 */
	
	int m_iBtnW;								/**< 两列时，控件的宽度 */  

	string m_strSno, m_strEno, m_strCode;
	INT32 m_sInvNo, m_eInvNo;
	int m_iColW;	
	CInvVol m_invVol;
public:
	CInvRetServerWin();
	~CInvRetServerWin();

	/*! @function
	*************************************************************************
	<PRE>
	函数名  : 
	功能    : 创建窗体
	参数    : [in] int iX 左上角横坐标
				[in] int iY 左上角纵坐标
				[in] int iW 宽度
				[in] int iH 高度
	返回值  : 1 ：成功；其它：失败
	作者    : 唐日升
	日期    : 2010-12-01
	备注    : 
	</PRE>
	************************************************************************/
	int Create(int iX,int iY,int iW,int iH);

	/*! @function
	*************************************************************************
	<PRE>
	函数名  : 
	功能    : 处理事件
	参数    : [in] iEvent 事件编号
				[in] pEventData 事件内容
				[in] iDataLen 事件内容长度
	返回值  : 1  已处理； 0 无法处理
	作者    : 唐日升
	日期    : 2010-12-01
	备注    : 
	</PRE>
	************************************************************************/
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);

	/*! @function
	*************************************************************************
	<PRE>
	函数名  : 
	功能    : 刷新窗口显示
	参数    : 
	返回值  : 1: SUCCESS
	作者    : 唐日升
	日期    : 2010-12-01
	备注    : 
	</PRE>
	************************************************************************/
	virtual int ReFresh();

	/*! @function
	*************************************************************************
	<PRE>
	函数名  : S_OnButton1
	功能    : 按钮1相应事件
	参数    : 
	返回值  :
	作者    : 唐日升
	日期    : 2010-12-01
	备注    : 
	</PRE>
	************************************************************************/
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	/*! @function
	*************************************************************************
	<PRE>
	函数名  : S_OnButton1
	功能    : 按钮1相应事件
	参数    : 
	返回值  :
	作者    : 唐日升
	日期    : 2010-12-01
	备注    : 
	</PRE>
	************************************************************************/
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);

	/*! @function
	*************************************************************************
	<PRE>
	函数名  : S_OnButton2
	功能    : 按钮2相应事件
	参数    : 
	返回值  :
	作者    : 唐日升
	日期    : 2010-12-01
	备注    : 
	</PRE>
	************************************************************************/
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	/*! @function
	*************************************************************************
	<PRE>
	函数名  : S_OnButton2
	功能    : 按钮2相应事件
	参数    : 
	返回值  :
	作者    : 唐日升
	日期    : 2010-12-01
	备注    : 
	</PRE>
	************************************************************************/
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	
	void DoActive();
	
	static	void S_OnActive(CaWindow *obj);

	/*!
	@brief 输入栏响应的静态函数
	@param[in] obj 控件指针
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	static void S_OnInput2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	
	/*!
	@brief 输入栏响应的静态函数
	@param[in] obj 控件指针
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	static void S_OnInput1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 输入栏响应的静态函数
	@param[in] obj 控件指针
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	static void S_OnInput3(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);

	/*! @function
	*************************************************************************
	<PRE>
	函数名  : 
	功能    : 检查输入是否有效 
	参数    : 
	返回值  : 1 SUCCESS; 其它: 错误代码
	作者    : 唐日升
	日期    : 2010-12-04
	备注    : 
	</PRE>
	************************************************************************/
	UINT8 CheckInput(string &strErr);
	
	/*! @function
	*************************************************************************
	<PRE>
	函数名  : 
	功能    : 错误消息提示框 
	参数    : 
	返回值  : ret   错误号 
	作者    : 唐日升
	日期    : 2010-12-04
	备注    : 
	</PRE>
	************************************************************************/
    UINT8 ErrMsgBox(UINT8 ret);

	/*!
	@brief 输入栏1响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnInput1(int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 输入栏2响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnInput2(int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 输入栏2响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnInput3(int iEvent, unsigned char * pEventData, int iDataLen);

};

#endif // !defined(AFX_INVRETSERVERWIN_H__98B88A47_BA1B_4EE2_BCA0_33F230CE2627__INCLUDED_)
