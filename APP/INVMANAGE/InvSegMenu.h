/*! \file    InvSegMenu.h
   \brief    awe4000r号段管理的主菜单
   \author   Yu Yan
   \version  1.0
   \date     2010-12-26
 */

#ifndef INV_SEG_MENU_H
#define INV_SEG_MENU_H

#include "CaButton.h"
#include "CaLabel.h"
#include "MultiBtnCommonWin.h"


using namespace std;

/**
 *@class CInvSegMenu 
 *@brief 号段管理主菜单
 */
class CInvSegMenu : public CMultiBtnCommonWin
{
public:
	CInvSegMenu();

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
	@brief 按钮响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 按钮响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);

		/*!
	@brief 按钮响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnButton3(int iEvent, unsigned char * pEventData, int iDataLen);

};

#endif
