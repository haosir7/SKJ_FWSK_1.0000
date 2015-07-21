#ifndef __CNETPARA_H
#define __CNETPARA_H

#include "CTable.h"
#include "CDB.h"
#include "SysMacDef.h"


/**
 *@class CNetPara 
 *@brief 转换器方案网络设置参数表
 *@brief 
 */
class CNetPara : public CTable
{
public:
    
	/*!
	@brief 构造函数	
	@attention   完成注册
	*/ 
	CNetPara();

    /*!
	@brief 析构函数	
	*/
	~CNetPara();

	/*!
	@brief 复位全体成员变量（清零）	
	*/
	void ResetMember(void);
	

	UINT8  m_IsDhcp;			/**< 是否自动分配IP */
	string m_LocalIP;			/**< 本机IP */
	string m_LocalGate;			/**< 本机网关 */
	string m_LocalMask;			/**< 子网掩码 */
	string m_LocalDNS;			/**< DNS */
	string m_ServIP;			/**< 服务器IP地址 */
	string m_ServPort;			/**< 服务器端口号 */
	string m_ServAddr;			/**< 服务器部署路径 */
	string m_FtpIP;				/**< ftp服务器地址 */
	string m_FtpPort;			/**< ftp端口号 */
	string m_FtpUser;			/**< 用户名 */
	string m_FtpPwd;			/**< 密码 */

};

#endif

