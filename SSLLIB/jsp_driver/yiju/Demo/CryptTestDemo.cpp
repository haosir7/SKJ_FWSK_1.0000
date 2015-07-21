//for x86:   gcc -o demo CryptTestDemo.cpp -I ../include -L . -lcryptapi -lgetcertinfo -lpthread -ldl -lstdc++
//for arm: arm-linux-gnueabihf-g++ -o armdemoa CryptTestDemo.cpp  armcryptlib.a -I ../include  -L /home/aisino/WTlib -lwt_skf -lwt_pkcs11 -lwt_hw -lcrypto -lssl -lpthread -lrt -ldl -lstdc++



#include "../include/isccryptapi.h"
#include "../include/decodecert.h"
#include "../include/ckwshare.h"


char g_LogPath[_MAX_PATH];
UINT g_nDbgDispLvl,g_bDbgToFile;

#include <iostream>

using namespace std;

typedef unsigned char byte;
typedef unsigned char BYTE;

BOOL bGBSDF=FALSE;

#define MAX_BUF		4096
#define ERR_LEN		200
#define BUF_LENGTH 200000


#pragma comment(lib,"../lib/cryp_api")
#pragma comment(lib,"../lib/decodecert")


int main()
{
	HCRYPTPROV hProv;
	char err[ERR_LEN]={0};
	char pOri[MAX_BUF] = "abcdefghijklmnopqrstuvwxyz中国12345";//测试数据test12345
	DWORD nOriLen = strlen(pOri);

	int inputnum=0;
	puts(" 使用哪种接口（1---CSP    2--- 国标接口）？");
	scanf("%d",&inputnum);
	if(inputnum == 2) bGBSDF=TRUE;
	else bGBSDF = FALSE;

	char container[ERR_LEN]="//Default/CTAS0002        ";//"//SM2/SM2CONTAINER";//{0};
	printf(" 容器（container）名字是(NULL代表空白,输入y表示默认 '%s')？\n",container);
	gets(container);
	gets(container);
	puts(" ");
	if(0 == stricmp(container,"NULL"))
		memset(container,0,ERR_LEN);
	else if(0 == stricmp(container,"y"))
		strcpy(container,"//Default/CTAS0002        ");
	printf("Now,  container = '%s'. \n",container);


	char provider[ERR_LEN]="Aisino Cryptographic Service Provider V1.0";//"./libNISEC_UKEY.so";//{0};
	printf(" 厂商（provider）名字是(NULL代表空白,输入y表示默认 '%s')？\n",provider);
	gets(provider);printf("------ provider= '%s'. \n",provider);
	puts(" ");
	if(0 == stricmp(provider,"NULL"))
		memset(provider,0,ERR_LEN);
	else if(0 == stricmp(provider,"y"))
		strcpy(provider,"Aisino Cryptographic Service Provider V1.0");
	printf("Now,  provider= '%s'. \n",provider);

	//打开密码设备--客户端
	int ret = Crypt_OpenDevice(&hProv,container,provider,bGBSDF ? 0x802 : 1,0);
	Crypt_GetErrorMsg(ret,err,ERR_LEN);
	if(ret != 0)
	{
		cout<<"密码设备打开失败(客户端)--"<<err<<endl;
		return 0;
	}
	cout<<"打开密码设备(客户端)--"<<err<<endl;

	//===============验证客户端口令===========================================================
	//验证客户端口令
	ret = Crypt_Login(hProv,"88888888");
	Crypt_GetErrorMsg(ret,err,ERR_LEN);
	cout<<"验证口令--"<<err<<endl;
	
	//打开密码设备--服务端：
	HCRYPTPROV hSrvProv = hProv;//此处用客户端代替服务端演示，实际与认证服务器认证身份.

	//==============读取用户证书===============================================
	//读取CA证书
	byte pbCACert[MAX_BUF]={0};
	DWORD nCACertLen = MAX_BUF;
	ret = Crypt_ReadCert(hProv,0,pbCACert,&nCACertLen);
	if(ret == 0)
	{
		cout<<"CA证书读取成功,证书长度为: "<<nCACertLen<<endl;
		//解析证书数据
		byte buf[2000]={0};
		int buflen = 2000;
		INT4 outIntN=0;
//		DWORD XX = GetCertInfo(nCACertLen,pbCACert,1,&outIntN,&buflen,buf);
		cout<<"证书序列号: "<<buf<<endl;		
	}
	else
	{
		Crypt_GetErrorMsg(ret,err,ERR_LEN);
		cout<<"CA证书读取--"<<err<<endl;
	}
	
	//读取用户证书
	byte pbCert[MAX_BUF]={0};
	DWORD nCertLen = MAX_BUF;
	ret = Crypt_ReadCert(hProv,1,pbCert,&nCertLen);
	if(ret == 0)
	{
		cout<<"加密证书读取成功,证书长度为: "<<nCertLen<<endl;
		//解析证书数据
		byte buf[2000]={0};
		int buflen = 2000;
		INT4 outIntN=0;
		//	DWORD XX = GetCertData(1,pbCert,nCertLen,buf);
//		DWORD XX = GetCertInfo(nCertLen,pbCert,1,&outIntN,&buflen,buf);
		cout<<"证书序列号: "<<buf<<endl;
	}
	else
	{
		Crypt_GetErrorMsg(ret,err,ERR_LEN);
		cout<<"加密证书读取--"<<err<<endl;
	}
	
	nCertLen = MAX_BUF;
	memset(pbCert,0,MAX_BUF);
	ret = Crypt_ReadCert(hProv,2,pbCert,&nCertLen);
	if(ret == 0)
	{
		cout<<"签名证书读取成功,证书长度为: "<<nCertLen<<endl;
		//解析证书数据
		byte buf[2000]={0};
		int buflen = 2000;
		INT4 outIntN=0;
		//	DWORD XX = GetCertData(1,pbCert,nCertLen,buf);
	//	DWORD XX = GetCertInfo(nCertLen,pbCert,1,&outIntN,&buflen,buf);
	//	cout<<"证书序列号: "<<buf<<endl;
	}
	else
	{
		Crypt_GetErrorMsg(ret,err,ERR_LEN);
		cout<<"签名证书读取--"<<err<<endl;
	}
	
//////////////////////////////////////////////////////////////////////////
	
	int menu=0;
CHOICE:
	puts("\n\n1.全部操作       2.仅身份认证 ");
	puts("3.仅数字签名  4.仅数字信封 ");
	puts("\n 请选择：");
	scanf("%d",&menu);
	if((menu <1 )||(menu > 4))goto CHOICE;
	bool bJumpToOver=true;
	if(menu == 1)bJumpToOver=FALSE;
	else bJumpToOver = TRUE;
	if(menu == 3) goto SIGN;
	if(menu == 4) goto ENVELOPE;

	//=========================身份认证流程==============================
	{
	//--ClientHello
	byte pbClientHello[MAX_BUF]={0};
	DWORD cbClientHello = MAX_BUF;
	ret = Crypt_ClientHello(hProv,0,pbClientHello,&cbClientHello);
	Crypt_GetErrorMsg(ret,err,ERR_LEN);
	cout<<"ClientHello( " << cbClientHello << " Bytes) --"<<err<<endl;
	
	//--ServerHello
	byte pbServerHello[MAX_BUF]={0};
	DWORD cbServerHello = MAX_BUF;
	byte pbServerRandom[MAX_BUF]={0};
	DWORD cbServerRandom = MAX_BUF;

	ret = Crypt_ServerHello(hSrvProv,pbClientHello,cbClientHello,pbServerHello,
						&cbServerHello,pbServerRandom,&cbServerRandom);
	Crypt_GetErrorMsg(ret,err,ERR_LEN);
	cout<<"ServerHello( " << cbServerHello << " Bytes) --"<<err<<endl;
	//--ClientAuth
	byte pbClientAuth[MAX_BUF]={0};
	DWORD cbClientAuth = MAX_BUF;
	ret = Crypt_ClientAuth(hProv,pbServerHello,cbServerHello,pbClientAuth,&cbClientAuth);
	Crypt_GetErrorMsg(ret,err,ERR_LEN);
	cout<<"ClientAuth( " << cbClientAuth << " Bytes) --"<<err<<endl;
	//--ServerAuth
	byte pbClientInfo[MAX_BUF]={0};
	DWORD cbClientInfo = MAX_BUF;
	byte pbSessionKey[MAX_BUF]={0};
	DWORD cbSessionKey = MAX_BUF;
	ret = Crypt_ServerAuth(hSrvProv,pbClientAuth,cbClientAuth,
						pbServerRandom,cbServerRandom,
						pbClientInfo,&cbClientInfo,
						pbSessionKey,&cbSessionKey);
	Crypt_GetErrorMsg(ret,err,ERR_LEN);
	cout<<"ServerAuth--"<<err<<endl;
	
	if(bJumpToOver) goto OVER;
	
	}

	//==================PKCS7数字签名==================================================
SIGN:
	{
	//PKCS7数字签名
	BYTE pSignedData[MAX_BUF]={0};
	DWORD nSignedLen = MAX_BUF;
	ret = Crypt_SignData(hProv,(byte*)pOri,nOriLen, CRYPT_ALGID_SHA1_RSA_PKCS,
				NULL,SIGN_INCLUDE_CERT,pSignedData,&nSignedLen);//SIGN_INCLUDE_DATA|
	Crypt_GetErrorMsg(ret,err,ERR_LEN);
	cout<<"P7数字签名RSA--"<<err<<endl;
	//验证PKCS7签名
	ret = Crypt_VerifySignedData(hProv,pSignedData,nSignedLen,0,(byte*)pOri,nOriLen,NULL,0);//NULL,0
	Crypt_GetErrorMsg(ret,err,ERR_LEN);
	cout<<"P7验证签名RSA--"<<err<<endl;

	//PKCS7数字签名
	nSignedLen = MAX_BUF;
	memset(pSignedData,0,MAX_BUF);
	ret = Crypt_SignData(hProv,(byte*)pOri,nOriLen, CRYPT_ALGID_GB_ECDSA_SM3 ,
				NULL,SIGN_INCLUDE_CERT,pSignedData,&nSignedLen);//SIGN_INCLUDE_DATA|
	Crypt_GetErrorMsg(ret,err,ERR_LEN);
	cout<<"P7数字签名SM2--"<<err<<endl;
	//验证PKCS7签名
	ret = Crypt_VerifySignedData(hProv,pSignedData,nSignedLen,0,(byte*)pOri,nOriLen,NULL,0);//NULL,0
	Crypt_GetErrorMsg(ret,err,ERR_LEN);
	cout<<"P7验证签名SM2--"<<err<<endl;
	//==================PKCS1签名===================================================
	// PKCS1签名
	nSignedLen = MAX_BUF;
	memset(pSignedData,0,nSignedLen);
	ret = Crypt_Sign(hProv, (byte*)pOri,nOriLen,  CRYPT_ALGID_SHA1_RSA_PKCS,
		2, pSignedData,&nSignedLen);
	Crypt_GetErrorMsg(ret,err,ERR_LEN);
	cout<<"P1数字签名RSA--"<<err<<endl;
	//验证PKCS1签名
	ret = Crypt_VerifySign(hProv, (byte*)pOri,nOriLen,  CRYPT_ALGID_SHA1_RSA_PKCS,
		 PUBKEY_CERTIFICATE, pSignedData,nSignedLen, pbCert,nCertLen);
	Crypt_GetErrorMsg(ret,err,ERR_LEN);
	cout<<"P1验证签名RSA--"<<err<<endl;
	

	// PKCS1签名
	nSignedLen = MAX_BUF;
	memset(pSignedData,0,nSignedLen);
	ret = Crypt_Sign(hProv, (byte*)pOri,nOriLen,  CRYPT_ALGID_GB_ECDSA_SM3 ,
		2, pSignedData,&nSignedLen);
	Crypt_GetErrorMsg(ret,err,ERR_LEN);
	cout<<"P1数字签名SM2--"<<err<<endl;
	//验证PKCS1签名
	ret = Crypt_VerifySign(hProv, (byte*)pOri,nOriLen, CRYPT_ALGID_GB_ECDSA_SM3 ,
		 PUBKEY_CERTIFICATE, pSignedData,nSignedLen, pbCert,nCertLen);
	Crypt_GetErrorMsg(ret,err,ERR_LEN);
	cout<<"P1验证签名SM2--"<<err<<endl;

	if(bJumpToOver) goto OVER;

	}

	//*
	//==================制作不带签名的数字信封====================================================
ENVELOPE:	
	{
		// 测试用接收者加密证书制作数字信封,例子中使用自己的加密证书演示。
		//读加密证书
		BYTE  pbReceiverInfo[MAX_BUF]={0};
		DWORD   cbReceiverInfo = MAX_BUF;
		
		ret = Crypt_ReadCert(hProv, 1, pbReceiverInfo, &cbReceiverInfo);
		Crypt_GetErrorMsg(ret,err,ERR_LEN);
		cout<<"读加密证书--"<<err<<endl;
		
		BYTE pbEnvelopedData[MAX_BUF]={0};//binary
		DWORD  cbEnvelopedData = MAX_BUF;
		ret = Crypt_EnvelopData(hProv, (BYTE*)pOri, nOriLen, CRYPT_ALGID_DES3_CBC,
			pbReceiverInfo,  cbReceiverInfo,		0, NULL, 			
			SIGN_INCLUDE_DATA|SIGN_EXCLUDE_SIGN, // dwFlags:接收者信息为证书，在数字信封中包含数据，不包含签名
			pbEnvelopedData, &cbEnvelopedData);
		Crypt_GetErrorMsg(ret,err,ERR_LEN);
		cout<<"----------------制作不带签名的数字信封------------"<<err<<endl;
		//cout<<""<< pbEnvelopedData <<endl;
		//	Write_CKW_DEBUGLog(NULL,"binary result：",pbEnvelopedData,cbEnvelopedData,NULL);
		printf("数字信封长度=%d\n", cbEnvelopedData);
		//*/
		
		char  strResult [BUF_LENGTH]={0};
		DWORD   lenResult = BUF_LENGTH;
		ret = Crypt_BinaryToBase64String(
			pbEnvelopedData,		//in 2进制数组
			cbEnvelopedData,		//in 数组长度
			0x40,					//in
			strResult,				//out 输出Base64字串
			&lenResult);			//in/out 缓冲区长度/输出的实际长度
		Crypt_GetErrorMsg(ret,err,ERR_LEN);
		cout<<"Base64编码--"<<err<<endl;
		
		
		char *strEnvelopedDataB64 = strdup(strResult);
		//数字信封结果
		printf("数字信封: %s\n", strEnvelopedDataB64);  
		
		//*
		//(解数字信封)
		// 在实际编程时，需要将Base64编码的字串转换为字节数组后运算
		
		cbEnvelopedData = MAX_BUF;
		memset(pbEnvelopedData,0,cbEnvelopedData);
		ret = Crypt_Base64StringToBinary(
			strEnvelopedDataB64,			//in 字符串(可含回车换行符和头尾标记)
			strlen(strEnvelopedDataB64),	//in 字符串最大长度
			pbEnvelopedData,				//out 输出二进制数组的缓冲区(可以为NULL)
			&cbEnvelopedData);				//in/out 缓冲区长度/返回长度
		Crypt_GetErrorMsg(ret,err,ERR_LEN);
		cout<<"转换为字节数组--"<<err<<"   结果长度 " << cbEnvelopedData << "字节" <<endl;
		//*/
		
		nOriLen = MAX_BUF;
		memset(pOri,0,nOriLen);
		ret = Crypt_VerifyEnvelopedData(hProv, 
			pbEnvelopedData, cbEnvelopedData, 
			NULL, 0, NULL, 0, 
			(BYTE*)pOri, &nOriLen);
		Crypt_GetErrorMsg(ret,err,ERR_LEN);
		cout<<"解数字信封--"<<err<<endl;
		
		if (ret == 0)
		{
			printf("解出的封装内容长度=%d\n", nOriLen);
			printf("解出的封装内容: %s\n", pOri);   	
		}
		
		
		//==================制作带签名的数字信封====================================================
		
		char pOri2[MAX_BUF] = "abcdefghijklmnopqrstuvwxyz中国12345";//测试数据test12345
		DWORD nOriLen2 = strlen(pOri2);
		
		cbEnvelopedData = MAX_BUF;
		memset(pbEnvelopedData,0,cbEnvelopedData);
		ret = Crypt_EnvelopData(hProv, (BYTE*)pOri2, nOriLen2, CRYPT_ALGID_GB_SMS4_CBC,//CRYPT_ALGID_DES3_CBC,
			pbReceiverInfo,  cbReceiverInfo,		bGBSDF ? CRYPT_ALGID_GB_ECDSA_SM3 : CRYPT_ALGID_SHA1_RSA_PKCS, NULL, 			
			SIGN_INCLUDE_DATA|SIGN_INCLUDE_CERT, // dwFlags:接收者信息为证书，在数字信封中不包含数据，包含签名
			pbEnvelopedData, &cbEnvelopedData);
		Crypt_GetErrorMsg(ret,err,ERR_LEN);
		cout<<"------------制作带签名的数字信封----------"<<err<<endl;
		//	cout<<"binary result："<< pbEnvelopedData <<endl;
		//	Write_CKW_DEBUGLog(NULL,"binary result：",pbEnvelopedData,cbEnvelopedData,NULL);
		
		printf("数字信封长度=%d\n", cbEnvelopedData);
		//*/
		
		lenResult = BUF_LENGTH;
		memset(strResult,0,lenResult);
		ret = Crypt_BinaryToBase64String(
			pbEnvelopedData,		//in 2进制数组
			cbEnvelopedData,		//in 数组长度
			0x40,					//in
			strResult,				//out 输出Base64字串
			&lenResult);			//in/out 缓冲区长度/输出的实际长度
		Crypt_GetErrorMsg(ret,err,ERR_LEN);
		cout<<"Base64编码--"<<err<<endl;
		
		
		strEnvelopedDataB64 = strdup(strResult);
		//数字信封结果
		printf("数字信封: %s\n", strEnvelopedDataB64);  
		
		//*
		//(解数字信封)
		// 在实际编程时，需要将Base64编码的字串转换为字节数组后运算
		
		cbEnvelopedData = MAX_BUF;
		memset(pbEnvelopedData,0,cbEnvelopedData);
		ret = Crypt_Base64StringToBinary(
			strEnvelopedDataB64,			//in 字符串(可含回车换行符和头尾标记)
			strlen(strEnvelopedDataB64),	//in 字符串最大长度
			pbEnvelopedData,				//out 输出二进制数组的缓冲区(可以为NULL)
			&cbEnvelopedData);				//in/out 缓冲区长度/返回长度
		Crypt_GetErrorMsg(ret,err,ERR_LEN);
		cout<<"转换为字节数组--"<<err<<"   结果长度 " << cbEnvelopedData << "字节" <<endl;
		//*/
		
		nOriLen = MAX_BUF;
		memset(pOri,0,nOriLen);
		ret = Crypt_VerifyEnvelopedData(hProv, 
			pbEnvelopedData, cbEnvelopedData, 
			NULL, 0, NULL, 0, 
			(BYTE*)pOri, &nOriLen);
		Crypt_GetErrorMsg(ret,err,ERR_LEN);
		cout<<"解数字信封--"<<err<<endl;
		
		if (ret == 0)
		{
			printf("解出的封装内容长度=%d\n", nOriLen);
			printf("解出的封装内容: %s\n", pOri);   	
		}  			
	}
OVER:
CLOSEDEVICE:

	
	//关闭服务端
	ret = Crypt_Logout(hSrvProv);
	Crypt_GetErrorMsg(ret,err,ERR_LEN);
	cout<<"Crypt_Logout--"<<err<<endl;
	
	if(hSrvProv != hProv)
		Crypt_CloseDevice(hSrvProv);
	//关闭密码设备
	Crypt_CloseDevice(hProv);
	
	system("pause");
	return 0;
}
