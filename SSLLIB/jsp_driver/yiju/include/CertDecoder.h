/************************************************************************/
/* 证书解析函数                                                         */
/************************************************************************/
#ifndef CERTDECODER_H_
#define CERTDECODER_H_

__declspec(dllexport) DWORD __stdcall GetErrorInfo(int dwErrNo,char* pMsg);
extern "C" __declspec(dllexport) DWORD __stdcall GetCertData(DWORD dwIndex,BYTE* pbCert,DWORD cbCert,char* pInfo);
extern "C" __declspec(dllexport) int __stdcall CheckKey();

#endif//CERTDECODER_H_


