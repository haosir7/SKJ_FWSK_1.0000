#include "CNetPara.h"
#include "LOGCTRL.h"
#include "pos_debug.h"


CNetPara::CNetPara()
{
	
	SET_TABLE_NAME("NETPARA");
	SET_FIELD_NUM(12);

	SET_FIELD_MEMBER(DB_UINT8, &m_IsDhcp, "ISDHCP", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_LocalIP, "LOCIP", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_LocalGate, "LOCGATE", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_LocalMask, "LOCMASK", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_LocalDNS, "LOCDNS", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_ServIP, "SERVIP", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_ServPort, "SERVPORT", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_ServAddr, "SERVADDR", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_FtpIP, "FTPIP", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_FtpPort, "FTPPORT", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_FtpUser, "FTPUSER", 0);		
	SET_FIELD_MEMBER(DB_TEXT, &m_FtpPwd, "FTPPWD", 0);		

	ResetMember();
}

CNetPara::~CNetPara()
{
}

void CNetPara::ResetMember(void)
{
	m_IsDhcp = 0;			/**< 是否自动分配IP */
	m_LocalIP = "";			/**< 本机IP */
	m_LocalGate = "";		/**< 本机网关 */
	m_LocalMask = "";		/**< 子网掩码 */
	m_LocalDNS = "";		/**< DNS */
	m_ServIP = "";			/**< 服务器IP地址 */
	m_ServPort = "";		/**< 服务器端口号 */
	m_ServAddr = "";		/**< 服务器部署路径 */
	m_FtpIP = "";			/**< ftp服务器地址 */
	m_FtpPort = "";			/**< ftp端口号 */
	m_FtpUser = "";			/**< 用户名 */
	m_FtpPwd = "";			/**< 密码 */
}

