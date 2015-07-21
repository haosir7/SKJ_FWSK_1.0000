/**
 @file  NetworkingShowWin.h
 @brief 界面文件，IP信息显示
*/
#ifndef NETWORKING_SHOW_WIN_H
#define NETWORKING_SHOW_WIN_H

#include "IncludeMe.h"
#include "CaWindow.h"
#include "CaButton.h"
#include "CaLabel.h"
#include "CaMsgBox.h"



/**
 *@class CNetworkingShowWin 
 *@brief 本机IP显示界面
 */
class CNetworkingShowWin : public CaWindow
{
public:
	char title_array[5][OBJ_TITLE_MAX_LEN + 1];	/**< 标签的标题*/

	CaLabel *label[4];					
	CaButton *m_pBtn1;				
	
	int m_iBtnW;									
	int m_iColW;								

	UINT8 m_create;								

	IPinfo m_ipInfo;
		

private:
	static void S_OnButton1(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnButton2(CaObject *obj,int iEvent, unsigned char * pEventData, int iDataLen);
	
public:
	int Create(int iX,int iY,int iW,int iH);
	int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	virtual int ReFresh();
	void OnButton1(int iEvent, unsigned char * pEventData, int iDataLen);
	void OnButton2(int iEvent, unsigned char * pEventData, int iDataLen);
	static void S_OnActive(CaWindow *obj);
	void DoActive(); 

	CNetworkingShowWin();
	virtual ~CNetworkingShowWin();
      

	
	/*!
	@brief 改变Label所关联的title值	
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 ChangeTitle();

	/*!
	@brief 获取网络IP信息
	@param[in] pInvSum 卷汇总记录的指针
	@param[in] codeNo  发票代码关联号
	@param[in] invNo   发票号码
	@return  1: SUCCESS; 0: FAILURE
	*/
	void GetNetworkingInfo();

	void ClearNetworkingInfo();
	void DispNetworkingInfo();

};
	

#endif
