/**
 @file  netdbWork.h
 @brief 本文件定义了网络模块中的数据库访问类CNetDBWork。
 */

#ifndef	__H_NET_DATABASE_WORK_
#define	__H_NET_DATABASE_WORK_


#include "IncludeMe.h"
#include "sqlite3.h"
#include "netSysArgDef.h"
#include "netConnection.h"



/**
 *@class CNetDBWork 
 *@brief 数据库访问接口类。

  依照需求对数据库访问接口模块进行再次封装，
  简化上层调用数据库操作的代码量，也起到隔离网络模块与数据库模块的作用。
  这个类目前的全部成员函数均为静态函数，无数据成员。
 */
class CNetDBWork
{
public:	

	/*!
	@brief 存储当前网络模式。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveNetMode(INT32 mode, string &strErr);
	/*!
	@brief 读取当前网络模式。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadNetMode(INT32 *mode, string &strErr);
	
	/*!
	@brief 存储第二层网络模式。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveNetModeL2(INT32 mode, string &strErr);
	/*!
	@brief 读取第二层网络模式。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadNetModeL2(INT32 *mode, string &strErr);

	/*!
	@brief 存储自动拨号方式。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveAutoDialType(INT32 type, string &strErr);
	/*!
	@brief 读取自动拨号方式。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadAutoDialType(INT32 *type, string &strErr);

	/*!
	@brief 存储DHCP模式。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveDHCPMode(INT32 dhcpmode, string &strErr);
	/*!
	@brief 读取DHCP模式。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadDHCPMode(INT32 *dhcpmode, string &strErr);

	/*!
	@brief 存储IP信息。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveIPinfo(IPinfo *info, string &strErr);
	/*!
	@brief 读取IP信息。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadIPinfo(IPinfo *info, string &strErr);

	/*!
	@brief 存储ADSL用户名与密码。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveADSLUserNamePwd(string &name, string &pwd, string &strErr);
	/*!
	@brief 读取ADSL用户名与密码。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadADSLUserNamePwd(string &name, string &pwd, string &strErr);

	/*!
	@brief 存储3G用户名与密码。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 save3gUserNamePwd(string &name, string &pwd, string &strErr);
	/*!
	@brief 读取3G用户名与密码。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 load3gUserNamePwd(string &name, string &pwd, string &strErr);
	
	/*!
	@brief 存储3G运营商。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 save3gCarrier(INT32 carrierType, string &strErr);
	/*!
	@brief 读取3G运营商。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 load3gCarrier(INT32 *carrierType, string &strErr);

	/*!
	@brief 存储M2M激活状态。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 save_M2M_State(INT32 actState, string &strErr);
	/*!
	@brief 读取M2M激活状态。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 load_M2M_State(INT32 *actState, string &strErr);

	/*!
	@brief 存储APN号码与接入点。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 save_APN_Acc_Num(string &AccPoint, string &Number, string &strErr);
	/*!
	@brief 读取APN号码与接入点。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 load_APN_Acc_Num(string &AccPoint, string &Number, string &strErr);

	/*!
	@brief 存储VPN用户名与密码。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveVPNUserNamePwd(string &name, string &pwd, string &strErr);
	/*!
	@brief 读取VPN用户名与密码。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadVPNUserNamePwd(string &name, string &pwd, string &strErr);

	/*!
	@brief 存储VPN服务器IP。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveVPNServer(string &ip, string &strErr);
	/*!
	@brief 读取VPN服务器IP。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadVPNServer(string &ip, string &strErr);


	/*!
	@brief 存储应用服务器的IP与端口。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveAPPServerIP(string &ip, string &port, string &strErr);
	/*!
	@brief 读取应用服务器的IP与端口。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadAPPServerIP(string &ip, string &port, string &strErr);

	/*!
	@brief 存储应用服务器的HTTP路径。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveAPPServerFILE(string &file, string &strErr);
	/*!
	@brief 读取应用服务器的HTTP路径。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadAPPServerFILE(string &file, string &strErr);

	/*!
	@brief 存储时间服务器的IP与端口。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveTIMEServerIP(string &ip, string &port, string &strErr);
	/*!
	@brief 读取时间服务器的IP与端口。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadTIMEServerIP(string &ip, string &port, string &strErr);

	/*!
	@brief 存储时间服务器的HTTP路径。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveTimeServerFILE(string &file, string &strErr);
	/*!
	@brief 存储时间服务器的HTTP路径。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadTimeServerFILE(string &file, string &strErr);


private:

	/*!
	@brief 通过loadproc函数读一个整型值的系统参数。
	@param[in] id 系统参数标识号
	@param[out] v_int 系统参数
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadSysArg(INT32 id, INT32 *v_int, string &strErr);
	/*!
	@brief 通过loadproc函数读一个字符串值的系统参数。
	@param[in] id 系统参数标识号
	@param[out] v_int 系统参数
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadSysArg(INT32 id, string &m_vText, string &strErr);
	/*!
	@brief 读一个系统参数。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadproc(INT32 id, INT32 *v_int, string &v_text, string &strErr);
	
	/*!
	@brief 通过saveproc存一个整型值的系统参数。
	@param[in] id 系统参数标识号
	@param[out] v_int 系统参数
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveSysArg(INT32 id, INT32 v_int, string &strErr);
	/*!
	@brief 通过saveproc存一个字符串值的系统参数。
	@param[in] id 系统参数标识号
	@param[out] v_int 系统参数
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveSysArg(INT32 id, string &m_vText, string &strErr);
	/*!
	@brief 添加或更新一个系统参数（自动进行添加还是更新的判断）。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveproc(INT32 id, INT32 v_int, string &v_text, string &strErr);

	/*!
	@brief 通过saveproc存两个系统参数。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 saveTwoSysArgFunc(INT32 ID1, string &value1, INT32 ID2, 
									string &value2, string &strErr);
	/*!
	@brief 通过loadproc取两个系统参数。
	@return NET_SUCCESS - 成功；其他 - 失败
	*/
	static INT32 loadTwoSysArgFunc(INT32 ID1, string &value1, INT32 ID2, 
									string &value2, string &strErr);
	

	CNetDBWork();
	~CNetDBWork();

};




#endif

