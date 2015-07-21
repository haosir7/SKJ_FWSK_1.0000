/*! \file    CorpInfoWin.h
   \brief    awe4000r报表打印的企业信息界面
   \author   Yu Yan
   \version  1.0
   \date     2008-4-1
 */

#ifndef CORP_INFO_WIN_H
#define CORP_INFO_WIN_H

#include "MultiLabelCommonWindow.h"
#include "CMainFrame.h"
#include "CaButton.h"
#include "CaMsgBox.h"
#include "CaObject.h"
#include "printer.h"
#include "CGlobalArg.h"
#include <string>

using namespace std;

/**
 *@class CCorpInfoWin 
 *@brief 报表打印的企业信息界面
 */
class CCorpInfoWin : public CMultiLabelCommonWin
{
private:
// 	CTax *ptaxItem;							/**< 税种税目信息表的指针*/
	struct TCorpInfoPrnData	m_corpInfo;		/**< 企业信息结构体*/
	struct TCorpInfoPrnData	*m_pCorpInfo;	/**< 企业信息结构体指针*/
	
public:

	/*!
	@brief 构造函数	
	*/
	CCorpInfoWin();

	/*!
	@brief 析构函数	
	*/
	~CCorpInfoWin();

	/*!
	@brief 创建窗体	
	@param iX 左上角横坐标
	@param iY 左上角纵坐标
	@param iW 宽度
	@param iH 高度
	@return 1 ：成功；其它：失败
	*/
	int Create(int iX,int iY,int iW,int iH);

	/*!
	@brief 处理事件	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	@return   1  已处理； 0 无法处理
	*/
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);

	/*!
	@brief 刷新窗口显示	
	@return 1: SUCCESS
	*/
	virtual int ReFresh();

	/*!
	@brief 按钮响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	virtual void OnButton1(int iEvent,unsigned char *pEventData, int iDataLen);

	/*!
	@brief 按钮响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	virtual void OnButton2(int iEvent,unsigned char *pEventData, int iDataLen);
	
	/*!
	@brief 设置界面显示内容	 
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 NormalShow();

	/*!
	@brief 更新显示内容的静态函数	 
	*/
	static void S_OnActive(CaWindow *obj);
    
	/*!
	@brief 更新显示内容的函数	 
	*/
	void DoActive(); 

	/*!
	@brief 打印企业信息	 
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 PrintCorpInfo();
	
};


#endif