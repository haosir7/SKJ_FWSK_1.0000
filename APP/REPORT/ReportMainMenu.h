/*! \file    ReportMainMenu.h
   \brief    awe4000r报表打印的主菜单
   \author   Yu Yan
   \version  1.0
   \date     2008-4-1
 */

#ifndef REPORT_MAIN_MENU_H
#define REPORT_MAIN_MENU_H

#include "CaButton.h"
#include "CaLabel.h"
#include "MultiBtnCommonWin.h"


using namespace std;

/**
 *@class CReportMainMenu 
 *@brief 报表打印的主菜单
 */
class CReportMainMenu : public CMultiBtnCommonWin
{
public:
	CReportMainMenu();

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
	@brief 进纸	
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 ForwardKeyProc();

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
};

#endif
