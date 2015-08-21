/*! \file    FiscalMainMenu.h
   \brief    税控功能主菜单
   \author   zl
   \version  1.0
   \date     2008-01-01
 */

#ifndef FISCAL_MENU_H
#define FISCAL_MENU_H

#include "MultiBtnCommonWin.h"
#include "CaLabel.h"
#include "CaButton.h"
#include "CMainFrame.h"
#include "CGlobalArg.h"

/**
 *@class CFiscalMenu 
 *@brief 税控主界面
 */
class CFiscalMenu : public CMultiBtnCommonWin
{
public:
	CFiscalMenu();
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
	@brief 初始化按钮
	*/
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	/*!
	@brief 申报按钮
	*/
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	/*!
	@brief 发票分发按钮
	*/
	void OnButton3(int iEvent, unsigned char * pEventData, int iDataLen);
	/*!
	@brief 完税按钮
	*/
	void OnButton4(int iEvent, unsigned char * pEventData, int iDataLen); 
	/*!
	@brief 信息更新按钮
	*/
	void OnButton5(int iEvent, unsigned char * pEventData, int iDataLen); 
	/*!
	@brief 发票上传按钮
	*/
	void OnButton6(int iEvent, unsigned char * pEventData, int iDataLen); 
		/*!
	@brief 发票补录按钮
	*/
	void OnButton7(int iEvent, unsigned char * pEventData, int iDataLen); 

	/*!
	@brief 2获取上传出错发票信息
	*/
	void OnButton8(int iEvent, unsigned char * pEventData, int iDataLen); 

	/*!
	@brief 发票补传
	*/
	void OnButton9(int iEvent, unsigned char * pEventData, int iDataLen); 

	/*!
	@brief 离线数据按钮
	*/
	void OnButton10(int iEvent, unsigned char * pEventData, int iDataLen); 

};


#endif
