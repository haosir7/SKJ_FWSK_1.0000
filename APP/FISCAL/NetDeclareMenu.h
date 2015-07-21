/*! \file    NetDeclareMenu.h
   \brief    网络抄报菜单
   \author   zcy
   \version  1.0
   \date     20150-05-06
 */

#ifndef NET_DECLARE_MENU_H
#define NET_DECLARE_MENU_H

#include "MultiBtnCommonWin.h"
#include "CaLabel.h"
#include "CaButton.h"
#include "CMainFrame.h"


/**
 *@class CNetDeclMenu
 *@brief 税控主界面
 */
class CNetDeclMenu : public CMultiBtnCommonWin
{
public:
	CNetDeclMenu();
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
	
private:
	/*!
	@brief 网络抄税
	*/
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	/*!
	@brief 网络清卡"
	*/
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	
};


#endif
