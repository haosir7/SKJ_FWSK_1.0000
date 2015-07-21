/*! \file    DataIOFunc.cpp
   \brief    awe4000r报表打印的应用函数
   \author   Yu Yan
   \version  1.0
   \date     2008-4-1
 */


#include "DataIOFunc.h"
#include "CaProgressBar.h"
#include "keypad.h"
#include "CGlobalArg.h"
#include "CDept.h"
#include "CClient.h"
#include "USBMount.h"
#include "ReportFunc.h"

#ifndef WIN32
#include <sys/timeb.h>
#include "lcd_tiny.h"
#endif

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

#define IMPORT_DEPT_TXT_DIR "/mnt/importDept.txt"  //导入Dept表的txt文件
#define IMPORT_CLIENT_TXT_DIR "/mnt/importClient.txt"  //导入Client表的txt文件

#define IMPORT_DEPT_DIR "/mnt/importDept.sql"  //写Dept表的sql文件
#define IMPORT_DEPT_CMD "../sqlite3 AWE < /mnt/importDept.sql"

#define EXPORT_DEPT_TXT_DIR "/mnt/exportDept.txt"  //导出Dept表的txt文件
#define EXPORT_CLIENT_TXT_DIR "/mnt/exportClient.txt"  //导出Client表的txt文件

#define EXPORT_DEPT_CMD "../sqlite3 AWE \".dump DEPT\" > /mnt/exportDept.sql"
#define EXPORT_CLIENT_CMD "../sqlite3 AWE \".dump CLIENT\" > /mnt/exportClient.sql"

#define SQL_VER 0    /**< 导入的文件类型, 1: sql脚本文件， 0: txt文件*/

UINT8 DeptImportProc()
{
	DBG_PRINT(("DeptImportProc()!"));
	UINT8 nRet;
	nRet = YesNoMsgBox("是否导入商品信息？");
	if (nRet==FAILURE)
	{
		return SUCCESS;
	}

#if SQL_VER	
	nRet = ImportHandle(2);  //将sql文件信息导入数据库
#else
	nRet = ImportTxtHandle(2);//将txt文件信息导入数据库
#endif

	return nRet;
}

UINT8 ClientImportProc()
{
	DBG_PRINT(("DeptImportProc()!"));
	UINT8 nRet;
	nRet = YesNoMsgBox("是否导入客户信息？");
	if (nRet==FAILURE)
	{
		return SUCCESS;
	}

#if SQL_VER	
//	nRet = ImportHandle(3);  //该函数未实现！！！
#else
	nRet = ImportTxtHandle(3);//将txt文件信息导入数据库
#endif

	return nRet;
}

UINT8 ImportHandle(UINT8 nKind)
{
	DBG_PRINT(("ImportHandle()!"));
	UINT8 ret;
	
	string fileDir = "";
	string strCmd = "";
	switch(nKind) 
	{
	case 2:
		fileDir = IMPORT_DEPT_DIR;
		strCmd = IMPORT_DEPT_CMD;
		break;
	default:break;
	}

	//mount the USB disk
	ret = UsbDiskMount(NULL);
	if (SUCCESS != ret)
	{
		DBG_PRINT(("挂载U盘失败 ret = %u", ret));
		CaMsgBox::ShowMsg("挂载U盘失败");
		UsbDiskUnMount();
		return FAILURE;
	}

	//check the import file
	FILE *fp1; 
	if( (fp1=fopen(fileDir.c_str(),"r"))==NULL ) 
	{ 
		DBG_PRINT((" 导入文件打开错误"));
		CaMsgBox::ShowMsg("导入文件打开错误");
		UsbDiskUnMount();
		return FAILURE;
	} 
	fclose(fp1);
	DBG_PRINT((" After fclose()"));

	//drop the existed table
	CaProgressBar proBar("导入数据...");
	proBar.ReFresh();
	CDept curDept;
	string strSql = "";
	DBG_PRINT(("清空表"));
	switch(nKind) 
	{
	case 2:
		if (curDept.Delete()!= SQLITE_OK)
		{
			CaMsgBox::ShowMsg("清空表错误");
			UsbDiskUnMount();
			DBG_RETURN(FAILURE);
		}
		break;
	default: break;
	}

	//import new table
	DBG_PRINT(("导入数据"));
//	POWOFF_DISABLE(); //屏蔽掉电中断
	if ( system(strCmd.c_str()) != 0 )
	{
		CaMsgBox::ShowMsg("导入数据失败");
		UsbDiskUnMount();
        return FAILURE;
	}
//	POWOFF_ENABLE();//开掉电中断
	
	UsbDiskUnMount();
	curDept.Requery();
	curDept.LoadOneRecord();
	
	CaMsgBox::ShowMsg("导入数据成功");
	return SUCCESS;
}

UINT8 DeptExportProc()
{
	DBG_PRINT(("ExportDeptProc()!"));
	UINT8 nRet;
	nRet = YesNoMsgBox("是否导出商品信息？");
	if (nRet==FAILURE)
	{
		return SUCCESS;
	}
	
#if SQL_VER	
	nRet = ExportSqlHandle(EXPORT_DEPT_CMD); //将数据库部类信息导出到sql文件
#else
	nRet = ExportTxtHandle(2);//将数据库部类信息导出到TXT文件
#endif

	return nRet;	
}

UINT8 ClientExportProc()
{
	DBG_PRINT(("ExportDeptProc()!"));
	UINT8 nRet;
	nRet = YesNoMsgBox("是否导出客户信息？");
	if (nRet==FAILURE)
	{
		return SUCCESS;
	}

#if SQL_VER	
	nRet = ExportSqlHandle(EXPORT_CLIENT_CMD); //将数据库客户信息导出到sql文件
#else
	nRet = ExportTxtHandle(3);//将数据库客户信息导出到TXT文件
#endif

	return nRet;	
}

UINT8 ExportSqlHandle(string strCmd)
{
	DBG_PRINT(("ExportSqlHandle()!"));
	UINT8 ret;

	ret = UsbDiskMount(NULL);
	if (SUCCESS != ret)
	{
		CaMsgBox::ShowMsg("挂载U盘失败");
		DBG_PRINT(("挂载U盘失败 ret = %u", ret));
		UsbDiskUnMount();
		return FAILURE;
	}
	
	CaProgressBar proBar("导出数据...");
	proBar.ReFresh();
	if ( system(strCmd.c_str()) != 0 )
	{
		CaMsgBox::ShowMsg("导出数据失败");
		UsbDiskUnMount();
        return FAILURE;
	}
	
	UsbDiskUnMount();
	CaMsgBox::ShowMsg("导出数据成功");
	return SUCCESS;

}

UINT8 ImportTxtHandle(UINT8 nKind)
{
	DBG_PRINT(("ImportTxtHandle()!"));

	FILE *fp;
	INT8 *chaBlank = " ";//空格
	UINT8 nRet;	
	string fileDir = "";
	
	switch(nKind) 
	{
	case 2:
		fileDir = IMPORT_DEPT_TXT_DIR;
		break;
	case 3:
		fileDir = IMPORT_CLIENT_TXT_DIR;
		break;
	default:break;
	}

	//mount the USB disk
	nRet = UsbDiskMount(NULL);
	if (SUCCESS != nRet)
	{
		DBG_PRINT(("挂载U盘失败 ret = %u", nRet));
		CaMsgBox::ShowMsg("挂载U盘失败");
		UsbDiskUnMount();
		return FAILURE;
	}

	//open the import file 
	if( (fp=fopen(fileDir.c_str(),"r"))==NULL ) 
	{ 
		DBG_PRINT((" 导入文件打开错误"));
		CaMsgBox::ShowMsg("导入文件打开错误");
		UsbDiskUnMount();
		return FAILURE;
	} 

	CaProgressBar proBar("导入数据...");
	proBar.ReFresh();

	CDept curDept;
	CClient curClient;
	switch(nKind) 
	{
		case 2:
			if (SQLITE_OK != curDept.Delete())
			{
				CaMsgBox::ShowMsg("清空表错误");
				fclose(fp);
				UsbDiskUnMount();
				DBG_RETURN(FAILURE);
			}
			nRet = InsertOneDept(fp, chaBlank);
			break;
		case 3:
			if (SQLITE_OK != curClient.Delete())
			{
				CaMsgBox::ShowMsg("清空表错误");
				fclose(fp);
				UsbDiskUnMount();
				DBG_RETURN(FAILURE);
			}
			nRet = InsertOneClient(fp, chaBlank);
			break;
		default:break;
	}
	
	if (SUCCESS != nRet) 
	{
		CaMsgBox::ShowMsg("导入数据失败");
		fclose(fp);
		UsbDiskUnMount();
        return FAILURE;
	}

	fclose(fp);
	UsbDiskUnMount();

//	curDept.Requery();
//	curDept.LoadOneRecord();
	
	CaMsgBox::ShowMsg("导入数据成功");
	return SUCCESS;
}

UINT8 InsertOneDept(FILE *fp, const INT8 *pchToken)
{
	CDept curDept;
	INT8 *chTemp;
	INT8 chBuf[512];
	memset((void*)chBuf, 0, sizeof(chBuf));	
	INT8 chStr[32];
	memset((void*)chStr, 0, sizeof(chStr));
	UINT32 nIndex = 0;
	CaProgressBar proBar("");

	fgets(chBuf, sizeof(chBuf), fp);
	while (fgets(chBuf, sizeof(chBuf), fp)) //读一条部类信息到chBuf中
	{
		DBG_PRINT(("%s", chBuf));
		//部类号
		if( (chTemp = strtok(chBuf, pchToken)) == NULL ) 
		{
			continue;
		}
		else
		{
			curDept.m_blh = atoi(chTemp);		
		}
		//名称
		if( (chTemp = strtok(NULL, pchToken)) == NULL ) 
		{
			continue;
		}
		else
		{
			curDept.m_spmc = chTemp;
		}
		//对应PLU
		if( (chTemp = strtok(NULL, pchToken)) == NULL ) 
		{
			continue;
		}
		else
		{
			curDept.m_spbm = chTemp;
		}
		//单价
		if( (chTemp = strtok(NULL, "\r")) == NULL ) 
		{
			continue;
		}
		else
		{
			curDept.m_spdj = atof(chTemp);
		}

		if ((curDept.m_blh < 0 )||(curDept.m_blh > 99))
		{
			CaMsgBox::ShowMsg("部类号必须在0～99之间");
			continue;
			//return FAILURE;
		}
		if( curDept.m_spmc.length() > GOODS_NAME_LEN )
		{
			CaMsgBox::ShowMsg("商品名称不能超过20字节");
			continue;
		}

		if (SQLITE_OK != curDept.AddNew()) //插入一条PLU记录
		{
			return FAILURE;
		}
		nIndex++;
		sprintf(chStr, "导入商品条数：%u", nIndex);
		proBar.SetText(chStr);
		proBar.Show();		
	}

	if (0 != ferror(fp))
	{
		return FAILURE;//读文件失败
	}

	if (0 == nIndex) 
	{
		return FAILURE;//一条也没导入
	}
	return SUCCESS;
}

UINT8 InsertOneClient(FILE *fp, const INT8 *pchToken)
{
	CClient curClient;
	INT8 *chTemp;
	INT8 chBuf[512];
	memset((void*)chBuf, 0, sizeof(chBuf));	
	INT8 chStr[32];
	memset((void*)chStr, 0, sizeof(chStr));
	UINT32 nIndex = 0;
	CaProgressBar proBar("");

	fgets(chBuf, sizeof(chBuf), fp);
	while (fgets(chBuf, sizeof(chBuf), fp)) //读一条部类信息到chBuf中
	{
		DBG_PRINT(("%s", chBuf));
		//客户编号
		if( (chTemp = strtok(chBuf, pchToken)) == NULL ) 
		{
			continue;
		}
		else
		{
			curClient.m_khbh = chTemp;		
		}
		//客户代码
//		if( (chTemp = strtok(NULL, pchToken)) == NULL ) 
//		{
//			continue;
//		}
//		else
//		{
//			curClient.m_khdm = chTemp;		
//		}
		//代码简码
//		if( (chTemp = strtok(NULL, pchToken)) == NULL ) 
//		{
//			continue;
//		}
//		else
//		{
//			curClient.m_khjm = chTemp;		
//		}
		//客户名称
		if( (chTemp = strtok(NULL, "\r")) == NULL ) 
		{
			continue;
		}
		else
		{
			curClient.m_khmc = chTemp;
		}
	
		INT32 ret =curClient.AddNew();
		DBG_PRINT(("ret= %d",ret));
		if (SQLITE_OK != ret) //插入一条客户记录
		{
			return FAILURE;
		}
		
		nIndex++;
		sprintf(chStr, "导入付款人个数：%u", nIndex);
		proBar.SetText(chStr);
		proBar.Show();		
	}

	if (0 != ferror(fp))
	{
		return FAILURE;//读文件失败
	}
	
	if (0 == nIndex) 
	{
		return FAILURE;//一条也没导入
	}
	return SUCCESS;
}
INT8 ComposeClientTxt(const CClient *pClient, string &strTxt)
{
	DBG_ASSERT_EXIT((pClient != NULL), (" pClient == NULL!"));
	
	//组装写机器信息的指令
    strTxt = "";
	strTxt += pClient->m_khbh;
	strTxt += " ";
//	strTxt += pClient->m_khdm;
//	strTxt += " ";
//	strTxt += pClient->m_khjm;
// 	strTxt += " ";
	strTxt += pClient->m_khmc;
	strTxt += "\r\n";

	return SUCCESS;
}

INT8 ComposeDeptTxt(const CDept *pDept, string &strTxt)
{
	DBG_ASSERT_EXIT((pDept != NULL), (" pDept == NULL!"));
	
	INT8 chValue[256];
	//组装写机器信息的指令
	memset((void*)chValue, 0, sizeof(chValue));
	sprintf(chValue, "%u %s %s %.2f", pDept->m_blh, 
		pDept->m_spmc.c_str(), pDept->m_spbm.c_str(),
		pDept->m_spdj);

	strTxt = "";
	strTxt += chValue;
	strTxt += "\r\n";
	DBG_PRINT(("strTxt = %s", strTxt.c_str()));

	return SUCCESS;
}

UINT8 ExportTxtHandle(UINT8 nKind)
{
	DBG_PRINT(("ExportTxtHandle()!"));

	FILE *fp;
	INT8 *chaBlank = " ";//空格
	UINT8 nRet;	
	string fileDir = "";
	
	switch(nKind) 
	{
	case 2:
		fileDir = EXPORT_DEPT_TXT_DIR;
		break;
	case 3:
		fileDir = EXPORT_CLIENT_TXT_DIR;
		break;
	default:break;
	}

	//mount the USB disk
	nRet = UsbDiskMount(NULL);
	if (SUCCESS != nRet)
	{
		DBG_PRINT(("挂载U盘失败 ret = %u", nRet));
		CaMsgBox::ShowMsg("挂载U盘失败");
		UsbDiskUnMount();
		return FAILURE;
	}

	//create and open the export file 
	if( (fp=fopen(fileDir.c_str(),"w"))==NULL ) 
	{ 
		DBG_PRINT((" 导出文件打开错误"));
		CaMsgBox::ShowMsg("导出文件打开错误");
		UsbDiskUnMount();
		return FAILURE;
	} 

	CaProgressBar proBar("导出数据...");
	proBar.ReFresh();

	CDept curDept;
	CClient curClient;
	switch(nKind) 
	{
		case 2:
			nRet = OutputOneDept(fp);
			break;
		case 3:
			nRet = OutputOneClient(fp);
			break;
		default:break;
	}

	if (SUCCESS != nRet) 
	{
		CaMsgBox::ShowMsg("导出数据失败");
		fclose(fp);
		UsbDiskUnMount();
        return FAILURE;
	}

//	fflush(fp); //刷新文件大小
	fclose(fp);
	UsbDiskUnMount();
	
	CaMsgBox::ShowMsg("导出数据成功");
	return SUCCESS;

}
UINT8 OutputOneDept(FILE *fp)
{
	INT32 errorcode;
	CDept curDept;
	string strSql = "";
	INT32 nRealLen, nLen;
	UINT32 nIndex = 0;
	CaProgressBar proBar("");
	INT8 chBuf[128];
	memset((void*)chBuf, 0, sizeof(chBuf));

	strSql = "部类号(3) 商品名称(20) 商品编码(3) 商品单价(11)\r\n";
	nRealLen = strSql.length();
	//fwrite(strSql.c_str(),1, nRealLen,fp);
	fwrite(strSql.c_str(),nRealLen,1, fp);

	//read Dept and write it to file one by one
	curDept.Requery();
    errorcode = curDept.MoveFirst();
	while (errorcode == SQLITE_ROW)
	{
		strSql = "";
		ComposeDeptTxt(&curDept, strSql);	
		nRealLen = strSql.length();
		nLen = 0;
		//nLen = fwrite(strSql.c_str(),1, nRealLen,fp); 
		nLen = fwrite(strSql.c_str(),nRealLen,1, fp); 
// 		if (nLen!=nRealLen) 
// 		{
// 			curDept.MoveEnd();
// 			sprintf(chBuf, "导出第%d条商品错误!", nIndex+1);
// 			CaMsgBox::ShowMsg(chBuf);		
// 			return FAILURE;
// 		}
		if(0 != fflush(fp))//缓冲区的内容写入硬盘文件
		{
			curDept.MoveEnd();
			return FAILURE;
		}
		nIndex++;
		sprintf(chBuf, "导出商品条数：%u", nIndex);
		proBar.SetText(chBuf);
		proBar.Show();		
		errorcode = curDept.MoveNext(); // read next plu
	}
	curDept.MoveEnd();

	if (errorcode!=SQLITE_DONE) 
	{		
		CaMsgBox::ShowMsg("查询数据库出错");
		return FAILURE;			
	}

	if (0 == nIndex) 
	{
		CaMsgBox::ShowMsg("无商品信息可导出");
		return FAILURE;
	}

	return SUCCESS;
}

UINT8 OutputOneClient(FILE *fp)
{
	INT32 errorcode;
	CClient curClient;
	string strSql = "";
	INT32 nRealLen, nLen;
	UINT32 nIndex = 0;
	CaProgressBar proBar("");
	INT8 chBuf[128];
	memset((void*)chBuf, 0, sizeof(chBuf));

	strSql = "客户编号(3) 客户名称(80)\r\n";
	nRealLen = strSql.length();
	//fwrite(strSql.c_str(),1, nRealLen,fp);
	fwrite(strSql.c_str(),nRealLen,1, fp);

	//read Dept and write it to file one by one
	curClient.Requery();
    errorcode = curClient.MoveFirst();
	while (errorcode == SQLITE_ROW)
	{
		strSql = "";
		ComposeClientTxt(&curClient, strSql);	
		nRealLen = strSql.length();
		nLen = 0;
		//nLen = fwrite(strSql.c_str(),1, nRealLen,fp);//写入文件缓冲区 
		nLen = fwrite(strSql.c_str(),nRealLen,1, fp);//写入文件缓冲区 
// 		if (nLen!=nRealLen) 
// 		{
// 			curClient.MoveEnd();
// 			sprintf(chBuf, "导出第%d条客户错误!", nIndex+1);
// 			CaMsgBox::ShowMsg(chBuf);		
// 			return FAILURE;
// 		}
		if(0 != fflush(fp))//缓冲区的内容写入硬盘文件
		{
			curClient.MoveEnd();
			return FAILURE;
		}
		nIndex++;
		sprintf(chBuf, "导出客户条数：%u", nIndex);
		proBar.SetText(chBuf);
		proBar.Show();		
		errorcode = curClient.MoveNext(); // read next plu
	}
	curClient.MoveEnd();
	if (errorcode!=SQLITE_DONE) 
	{		
		CaMsgBox::ShowMsg("查询数据库出错");
		return FAILURE;			
	}

	if (0 == nIndex) 
	{
		CaMsgBox::ShowMsg("无客户信息可导出");
		return FAILURE;
	}

	return SUCCESS;
}