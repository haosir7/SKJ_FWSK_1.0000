/*! \file    HqlxsjWin.h
   \brief    awe4000r获取离线数据界面
   \author   Yu Yan
   \version  1.0
   \date     2008-4-1
 */

#ifndef HQLXSJ_WIN_H
#define HQLXSJ_WIN_H

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
 *@class CHqlxsjWin 
 *@brief 报表打印的企业信息界面
 */
class CHqlxsjWin : public CMultiLabelCommonWin
{
private:
	struct TGetOffLineData	m_OffLineDate;		/**< 离线数据结构体*/
	struct TGetOffLineData	*m_pOffLineDate;	/**< 离线数据结构体*/

	string m_wscfpzs;		//未上传发票张数
	string m_wscfpsj;		//未上传发票时间
	string m_wscfpljje;		//未上传发票累计金额
	string m_sczs;			//上传张数
	string m_scsjjg;		//上传时间间隔
	
public:

	/*!
	@brief 构造函数	
	*/
	CHqlxsjWin();

	/*!
	@brief 析构函数	
	*/
	~CHqlxsjWin();

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
	@brief 获取离线数据信息
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 GetOffLineDate();

	/*!
	@brief 打印离线数据信息
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 PrintOffLineDate();
	
	void Reset();

};

#endif