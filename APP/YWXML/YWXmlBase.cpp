
#include "YWXmlBase.h"
#include "OperateDiskProc.h"
#include "transfers_base.h"
#include "netManager.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

static	INT8 tmpBuff[MAX_SEND_BUF_LEN];

CYWXmlBase::CYWXmlBase(CYWXML_GY &ywxml_gy):m_ywxml_gy(ywxml_gy)
{
	m_ywlx = ywxml_gy.m_strID;
	m_servip = "";
	m_servport = "";
	m_servpath = "";
	m_pin = "";
	m_Err = "";
	m_nsrsbh = "";
	m_servId = "";
	//m_invUploadFlag = 0;
}
CYWXmlBase::~CYWXmlBase()
{
}

INT32 CYWXmlBase::BuildProc()
{
	DBG_PRINT(("BuildProc Begin"));
	m_pXmlConstruct.XMLConstructBegin("1.0", "gbk", XML_BUSINESS);
	m_pXmlConstruct.m_RootElement->SetAttribute(XML_BUSINESS_ID, m_ywlx.c_str());

	INT32 retcode = 0;
	retcode = XmlBuild();
	if(retcode != XML_SUCCESS)
	{
		return retcode;
	}

	if( 1 != m_pXmlConstruct.XMLConstructEnd(KJ_XML_REQUEST_FILE) )
	{
		return XML_INTERNAL_ERR_NO;
	}

	return XML_SUCCESS;
}
INT32 CYWXmlBase::ParseProc()
{
	DBG_PRINT(("ParseProc Begin"));
	m_pXmlParse.XMLParseBegin(KJ_XML_RESPONSE_FILE);
	m_ywlx = m_pXmlParse.m_RootElement->Attribute(XML_BUSINESS_ID);
	DBG_PRINT(("m_ywlx = %s", m_ywlx.c_str()));

	INT32 retcode = 0;
	retcode = XmlParse();
	if(retcode != XML_SUCCESS)
	{
		return retcode;
	}

	m_pXmlParse.XMLParseEnd();

	return XML_SUCCESS;
}

INT32 CYWXmlBase::YWXml_Proc(string &strErr)
{
	DBG_PRINT(("=======YWXml_Proc Begin============="));
	INT32 errcode = 0;
	errcode = BuildProc();
	if(errcode != XML_SUCCESS)
	{
		strErr = "组装请求信息错误";
		return XML_INTERNAL_ERR_NO;
	}

	FILE *fp;	
	string sInputInfo;
	string sOutputInfo;

	//读取XML文件至BUFF
	memset(tmpBuff, 0, sizeof(tmpBuff));
	if( (fp = fopen(KJ_XML_REQUEST_FILE, "rb")) == NULL )
	{
		strErr = "未找到请求XML文件";
		return XML_INTERNAL_ERR_NO;
	}
	fseek( fp, 0L, SEEK_END );	
	int dataLen = ftell( fp );				
	DBG_PRINT(("Req filelen: %d", dataLen));
	
	fseek( fp, 0L, SEEK_SET );	
	fread( (void *)tmpBuff, dataLen, 1, fp);
	fclose(fp);
	sInputInfo = tmpBuff;

	OperateDisk(sInputInfo, sOutputInfo);

	//接收数据写文件
	memset(tmpBuff, 0, sizeof(tmpBuff));
	sprintf((INT8 *)tmpBuff, "%s", sOutputInfo.c_str());
	UINT32 tmpLen = sOutputInfo.length();
	DBG_PRINT(("tmpLen = %u", tmpLen));
	
	if( (fp = fopen(KJ_XML_RESPONSE_FILE, "wb")) == NULL )
	{
		strErr = "未找到应答XML文件";
		return XML_INTERNAL_ERR_NO;
	}
	fwrite( (void *)tmpBuff, tmpLen, 1, fp);
	fclose(fp);

	errcode = ParseProc();
	if(errcode != XML_SUCCESS)
	{
		strErr = "解析应答数据错误";
		return XML_INTERNAL_ERR_NO;
	}

	errcode = atoi(m_retInfo.m_retCode.c_str());
	DBG_PRINT(("errcode = %d", errcode));
	if(errcode != 0)
	{
		strErr = m_retInfo.m_retMsg;
		return errcode;
	}

	DBG_PRINT(("=============YWXml_Proc End================"));
	return XML_SUCCESS;
}

void CYWXmlBase::SetServInfo(string ip, string port, string path)
{
	m_servip = ip;
	m_servport = port;
	m_servpath = path;
}

INT32 CYWXmlBase::NETXml_Proc(string &strErr)
{
	DBG_PRINT(("=======YWXml_Proc Begin============="));
	INT32 errcode = 0;
	errcode = BuildProc();
	if(errcode != XML_SUCCESS)
	{
		strErr = "组装请求信息错误";
		return XML_INTERNAL_ERR_NO;
	}
	
	FILE *fp;	
	string sInputInfo;
	string sOutputInfo;
	
	//读取XML文件至BUFF
	memset(tmpBuff, 0, sizeof(tmpBuff));
	if( (fp = fopen(KJ_XML_REQUEST_FILE, "rb")) == NULL )
	{
		strErr = "未找到请求XML文件";
		return XML_INTERNAL_ERR_NO;
	}
	fseek( fp, 0L, SEEK_END );	
	int dataLen = ftell( fp );				
	DBG_PRINT(("Req filelen: %d", dataLen));
	
	fseek( fp, 0L, SEEK_SET );	
	fread( (void *)tmpBuff, dataLen, 1, fp);

	fclose(fp);
	sInputInfo = tmpBuff;
	
	//设置服务器IP，端口和路径
	
	CNetTransfers *p=NULL;
	p=g_netManager->GetTransHandle();

	m_servip =p->m_serverIP;
	m_servport =p->m_serverPort;
	m_servpath =p->m_serverFile;
	DBG_PRINT(("m_servip= %s",m_servip.c_str()));
	DBG_PRINT(("m_servport= %s",m_servport.c_str()));
	DBG_PRINT(("m_servpath= %s",m_servpath.c_str()));

	OperateNet((char *)m_ywxml_gy.m_zskl.c_str(), m_servip, m_servport, m_servpath, "", m_ywxml_gy.m_nsrsbh, sInputInfo, sOutputInfo, strErr);
	
	//接收数据写文件
	memset(tmpBuff, 0, sizeof(tmpBuff));
	sprintf((INT8 *)tmpBuff, "%s", sOutputInfo.c_str());
	UINT32 tmpLen = sOutputInfo.length();
	DBG_PRINT(("tmpLen = %u", tmpLen));
	
	if(tmpLen > 0)
	{
	if( (fp = fopen(KJ_XML_RESPONSE_FILE, "wb")) == NULL )
	{
		strErr = "未找到应答XML文件";
		return XML_INTERNAL_ERR_NO;
	}
	fwrite( (void *)tmpBuff, tmpLen, 1, fp);
	fclose(fp);
	
	errcode = ParseProc();
	if(errcode != XML_SUCCESS)
	{
		strErr = "解析应答数据错误";
		return XML_INTERNAL_ERR_NO;
	}
	
	errcode = atoi(m_retInfo.m_retCode.c_str());
	DBG_PRINT(("errcode = %d", errcode));
	if(errcode != 0)
	{
		strErr = m_retInfo.m_retMsg;
		return errcode;
	}
	}
	else
	{
		return XML_FAILURE;
	}
	
	DBG_PRINT(("=============NETXml_Proc End================"));
	return XML_SUCCESS;
}
