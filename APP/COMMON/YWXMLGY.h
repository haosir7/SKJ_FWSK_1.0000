#ifndef __YW_XML_GY_H
#define __YW_XML_GY_H

#include <string.h>
#include "IncludeMe.h"

class CYWXML_GY;
extern CYWXML_GY *g_YwXmlArg;

class CYWXML_GY
{
public:
	string m_strID;			//业务ID
	string m_jqbh;			//税控收款机编号
	string m_nsrsbh;		//纳税人识别号
	string m_nsrmc;			//纳税人名称
	string m_sksbbh;		//税控设备编号
	string m_sksbkl;		//税控设备口令
	string m_fplxdm;		//发票类型代码
	string m_bspbh;			//报税盘编号
	string m_bspkl;			//报税盘口令
	string m_kpjh;			//开票机号 0：主机 其他：分机
	string m_zskl;			//证书口令

public:
	CYWXML_GY();
	~CYWXML_GY();

	static CYWXML_GY* GetInstance(void);

private:
	static CYWXML_GY *m_YwXmlArg;	/**< CGlobalArg对象，静态 */

};


#endif

