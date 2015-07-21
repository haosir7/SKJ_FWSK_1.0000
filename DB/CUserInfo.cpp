#include "CUserInfo.h"
#include "LOGCTRL.h"
#include "pos_debug.h"


CUserInfo::CUserInfo()
{
	
	SET_TABLE_NAME("USERINFO");
	SET_FIELD_NUM(16);

	SET_FIELD_MEMBER(DB_TEXT, &m_Nsrsbh, "NSRSBH", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_Nsrmc, "NSRMC", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_Kpjhm, "KPJHM", 0);
	SET_FIELD_MEMBER(DB_UINT8, &m_zfFlag, "ZFFLAG", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_Qylx, "QY_LX", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_Jspsbh, "JSPSBH", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_Gsdjh, "GSDJH", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_Khyh, "KHYH", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_Khzh, "KHZH", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_Djzcdz, "DJZCDZ", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_Swjgdm, "SWJGDM", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_Swjgmc, "SWJGMC", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_Slgs, "SLGS", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_Kpjsl, "KPJSL", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_qysj, "QYSJ", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_backup, "BACKUP", 0);		

	ResetMember();
}

CUserInfo::~CUserInfo()
{
}

void CUserInfo::ResetMember(void)
{
	m_Nsrsbh = "";			/**< 纳税人识别号 */
	m_Nsrmc = "";			/**< 纳税人名称 */
	m_Kpjhm = 0;			/**< 开票机号码 */
	m_zfFlag = 0;            /**<主分机标志 0代表主机 1代表分机*/
	m_Qylx = "";				/**< 行业类型 */
	m_Jspsbh = "";			/**< 金税盘设备号 */
	m_Gsdjh = "";			/**< 工商登记号 */
	m_Khyh = "";			/**< 开户银行 */
	m_Khzh = "";			/**< 开户账号 */
	m_Djzcdz = "";			/**< 登记注册地址 */
	m_Swjgdm = "";			/**< 主管税务机关代码 */
	m_Swjgmc = "";			/**< 主管税务机关名称 */
	m_Slgs = 0;				/**< 普票授权税率个数 */
	m_Kpjsl = 0;			/**< 开票机数量 */
	m_backup = "";			/**< 备用 */
	m_qysj = "";
	m_ffbz = "";
	m_bbh = "";
	m_blxx = "";
	m_fplxdm = "";
	m_qtkzxx = "";
	m_sksbkl = "";
	m_Fplxgs = 0;
	m_Fplxsz = "";
	m_bspbh = "";
}

