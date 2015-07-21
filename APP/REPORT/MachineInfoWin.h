/*! \file    MachineInfo.h
   \brief    awe4000r报表打印的机器信息界面
   \author   Yu Yan
   \version  1.0
   \date     2008-4-1
 */

#ifndef MACHINE_INFO_WIN_H
#define MACHINE_INFO_WIN_H

#include "CaWindow.h"
#include "CaButton.h"
#include "CaLabel.h"
#include "printer.h"
#include "CGlobalArg.h"
#include "netHeaderDef.h"

using namespace std;

/**
 *@class CMachineInfoWin 
 *@brief 报表打印的机器信息界面
 */
class CMachineInfoWin : public CaWindow
{
private:
	CaLabel *m_pLabel1;		/**< 指向标签的指针 */
	CaLabel *m_pLabel2;		/**< 指向标签的指针 */
	CaLabel *m_pLabel3;		/**< 指向标签的指针 */
	CaLabel *m_pLabel4;		/**< 指向标签的指针 */
	
	CaButton *m_pButton1;	/**< 指向按钮的指针 */
	CaButton *m_pButton2;	/**< 指向按钮的指针 */
	
	int m_iButtonW;			/**< 两列时，控件的宽度 */  
	int m_iButtonH;			/**< 两列时，第二列起始点的横坐标 */  
	
	struct TMachinePrnData m_machinePrnInfo; /**< 机器信息结构体 */  
	struct TMachinePrnData *m_pMachPrnInfo;  /**< 机器信息结构体指针 */  

	string m_IMEI;

public:

	/*!
	@brief 构造函数	
	*/
	CMachineInfoWin();

	/*!
	@brief 析构函数	
	*/
	~CMachineInfoWin();

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
	@brief “打印”按钮响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void PrintMachineInfo(int iEvent, unsigned char * pEventData, int iDataLen);
	
	/*!
	@brief “返回”按钮响应函数	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	void ReturnBack(int iEvent, unsigned char * pEventData, int iDataLen);
	
	/*!
	@brief 更新显示内容的静态函数	 
	*/
	static void S_OnActive(CaWindow *obj);
    
	/*!
	@brief 更新显示内容的函数	 
	*/
	void DoActive(); 

private:

	/*!
	@brief “打印”按钮响应的静态函数
	@param[in] obj 控件指针
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	static void S_PrintMachineInfo(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	
	/*!
	@brief “返回”按钮响应的静态函数
	@param[in] obj 控件指针
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	*/
	static void S_ReturnBack(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
#if _CONFIG_M2M_MODULE
	void GetWmmpIMEINo();
#endif

};
	

#endif
