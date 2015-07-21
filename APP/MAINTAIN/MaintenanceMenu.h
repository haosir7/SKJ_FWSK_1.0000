/*! \file    MaintenanceMenu.h
   \brief    awe4000r销售程序中系统维护主菜单
   \author   Yu Yan
   \version  1.0
   \date     2008-3-19 
 */

#ifndef MAINTENANCE_MENU_H
#define MAINTENANCE_MENU_H

#include "MaintenanceFunc.h"
#include "CMainFrame.h"
#include "MultiBtnCommonWin.h"
#include "CaLabel.h"
#include "CaButton.h"
#include"pos_debug.h"
#include "g_def.h"
#include <string>
using namespace std;

/**
 *@class CMaintenanceMenu 
 *@brief 系统维护主菜单
 */
class CMaintenanceMenu : public CMultiBtnCommonWin
{
public:
	CMaintenanceMenu();

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

	/*!
	@brief 按钮响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnButton4(int iEvent, unsigned char * pEventData, int iDataLen);
	
	/*!
	@brief 按钮响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnButton5(int iEvent, unsigned char * pEventData, int iDataLen); 

	/*!
	@brief 按钮响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnButton6(int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 按钮响应函数
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnButton7(int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 主管密码	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnButton8(int iEvent, unsigned char * pEventData, int iDataLen);

	/*!
	@brief 网络管理	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void OnButton9(int iEvent, unsigned char * pEventData, int iDataLen);

//	void OnButton10(int iEvent, unsigned char * pEventData, int iDataLen);

};


#endif
