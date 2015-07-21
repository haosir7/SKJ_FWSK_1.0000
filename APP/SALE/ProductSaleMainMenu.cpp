/*! \file    ProductSaleMainMenu.cpp
\brief    awe4000r销售程序中商品销售档主界面
\author   Yu Yan
\version  1.0
\date     2007-11-28
*/


#include "ProductSaleMainMenu.h"
#include "beep.h"
#include "CClient.h"
#include "Scanner.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"
#include <string>

CProductSaleMainMenu::CProductSaleMainMenu():CaWindow()
{
	if( pSaleData == NULL )
	{
		pSaleData = new SaleData;
		DBG_ASSERT_EXIT((pSaleData != NULL), (" pSaleData == NULL "));
	}	
	m_strSuffix = "";
}

CProductSaleMainMenu::~CProductSaleMainMenu()
{
	if( pSaleData != NULL )
	{
		delete pSaleData;
        pSaleData = NULL;
	}
}


int CProductSaleMainMenu::Create(int iX,int iY,int iW,int iH)
{
	DBG_PRINT(("进入CProductSaleMainMenu::Create函数"));
	
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
	//	string curtime = TDateTime::CurrentDateTime().FormatString(YYYYMMDDHHMMSS);
	string curtime = TDateTime::CurrentDate().FormatString(YYYYMMDD);

	//curtime += "  ";
    int titleLen=0;
	
    m_iBtnW = (SCREEN_W/2 - 20) ;	//Label的宽度
	m_iColW = m_iBtnW+5;			//Label的列宽
	
	m_pFrame->RegsiterWin(this,PRODUCT_SALE_MAIN_MENU);
	
	CaWindow::Create(iX,iY,iW,iH); // creat a window
	//创建一个标签 第一行,当前时间
	label[0] = new CaLabel(false,CaObject::ALIGN_RIGHT);
	curH = SCREEN_TOP_OFFSET;
	label[0]->Create(0,curH, SCREEN_W, CHAR_H);
	label[0]->SetTitle(curtime.c_str(), curtime.size());
	
	
	//创建一个标签  第二行
	label[1] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[1]->Create(0,curH, SCREEN_W, CHAR_H); //参数是坐标
	label[1]->SetTitle(title_array[1], strlen(title_array[1]));
	
	//创建一个标签  第三行
	label[2] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[2]->Create(0,curH, SCREEN_W, CHAR_H); //参数是坐标
	label[2]->SetTitle(title_array[2], strlen(title_array[2]));
	
	//创建一个标签  第四行
	label[3] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[3]->Create(0,curH, m_iBtnW, CHAR_H);
	label[3]->SetTitle(title_array[3], strlen(title_array[3]));
	//创建一个标签  第四行
	label[4] = new CaLabel(false,CaObject::ALIGN_LEFT);
	label[4]->Create(m_iColW,curH, m_iBtnW+40, CHAR_H);
	label[4]->SetTitle(title_array[4], strlen(title_array[4]));
	
	//创建一个Input  第五行
	strcpy(title, "[输入]");
	titleLen = strlen(title) * CHAR_W;
	curH += LINE_H;
	m_pInput2=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput2->Create(0,curH,SCREEN_W,LINE_H);
	m_pInput2->SetTitle(title,titleLen / CHAR_W);	
	m_pInput2->SetMaxLen(PAYER_NAME_LEN);
	
	OnActive = S_OnActive;
	
	this->End();
	
	DBG_PRINT(("退出CProductSaleMainMenu::Create函数"));
	return SUCCESS;
}

//--------------------------------------------------------------------------------------
//处理事件
//--------------------------------------------------------------------------------------
int CProductSaleMainMenu::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	DBG_PRINT(("进入CProductSaleMainMenu::ProcEvent函数"));
	
	UINT8 ret = 0;
	double ii;
	UINT32 deptNo;
	char *content;
	char *blank = "";
	string code = "";
	INT32 dotNum;//小数位数
	UINT8 *chScan;
	INT32 nLen  = 0;
	UINT8 nIndex = 0;
	string strErr("");
	
	//-----------------------------------------------------
	//用户每按一次键，就触发一个事件。
	//-----------------------------------------------------
	switch( iEvent )
	{
	case OPEN_CASH_BOX:
		DBG_PRINT(("启动开钱箱"));
		MoneyBox_Open();
		break;
		
	case DISCOUNT_KEY:
		content = (char*)(m_pInput2->m_contentBuf);
		if (strlen(content)==0)
		{
			CaMsgBox::ShowMsg("请先输入折扣率");
			return SUCCESS;
		}		
		ii = atof(content);
		ii = (INT32)ii;//向零取整
		m_pInput2->Clear();
		ret = pSaleData->Discount(ii);
		if (ret != SUCCESS) 
		{
			return(ErrMsgBox(ret));
		}
		ReFresh();
		return SUCCESS;
		break;
		//	CaMsgBox::ShowMsg("暂不支持此功能");
		//	return SUCCESS;
		//	break;
		
	case PRICE_INPUT:
		return(PriceInputProc());
		break;
		
	case SUM_INPUT:
		return(SumInputProc());
		break;
		
	case CLIENT_NAME_INPUT:		
		return(ClientInputProc());
		break;
		
	case FIND_KEY:
		return(ClientCodeInputProc());
		break;
		
	case PLUS_KEY:
		return(PlusProc());
		break;
		
		//手动输入商品编码
	case PLU_KEY:
		if (pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE) 
		{
			CaMsgBox::ShowMsg("商品行数超限");
			return FAILURE;
		}
		content = (char*)(m_pInput2->m_contentBuf);
		code = "";
        nLen = GOODS_CODE_LEN - strlen(content);
		if (nLen > 0)	//不足16位，则前补零到16位
		{
			while(nLen > 0)
			{
				code += "0";
				nLen--;
			}
		}
		code += content;
		//ret = pSaleData->PLUSale(code);
		ret = pSaleData->DeptSale(0, code);
		m_pInput2->Clear();
		if (ret != SUCCEED) 
		{
			m_strSuffix = "";
			return(ErrMsgBox(ret));
		}
		//添加实现内容调整的函数 
		DeptSaleShow();
		pPSWin = (CProductSaleWin*)m_pFrame->GetWin(PRODUCT_SALE_WIN);
		if (pPSWin!=NULL)
		{
			pPSWin->ChangeTitle();
			pPSWin->ReFresh();
		}
		//切换到商品销售界面
		ChangeWin(PRODUCT_SALE_WIN);
		return SUCCESS;
		break;
	case SHIFT_PLU_KEY: //临时输入商品名称
		if (pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE) 
		{
			CaMsgBox::ShowMsg("商品行数超限");
			return FAILURE;
		}
		content = (char*)(m_pInput2->m_contentBuf);
		ret = pSaleData->PLUSale(content, strErr);	
		if (ret != SUCCEED) 
		{
			m_strSuffix = "";
			if (FAILURE == ret) 
			{
				CaMsgBox::ShowMsg(strErr);
				strErr = "";
			}
			else
			{
				ErrMsgBox(ret);
			}		
			return FAILURE;
		}
		m_pInput2->Clear();
		//添加实现内容调整的函数 
		DeptSaleShow();
		pPSWin = (CProductSaleWin*)m_pFrame->GetWin(PRODUCT_SALE_WIN);
		if (pPSWin!=NULL)
		{
			pPSWin->ChangeTitle();
			pPSWin->ReFresh();
		}
		//切换到商品销售界面
		ChangeWin(PRODUCT_SALE_WIN);
		return SUCCESS;
		break;
		
	case CHANGE_DEPT_KEY:
		if (pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE) 
		{
			CaMsgBox::ShowMsg("商品行数超限");
			return FAILURE;
		}
		content = (char*)(m_pInput2->m_contentBuf);
		deptNo = atoi(content);
		m_pInput2->Clear();
		return(DeptSaleProc(deptNo));
		break;
		
		//部类1
	case DEPT1_KEY:
		if (pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE) 
		{
			CaMsgBox::ShowMsg("商品行数超限");
			return FAILURE;
		}
		//QuickSaleProc函数的作用是
		if( QuickSaleProc() != SUCCESS )
		{
			DBG_PRINT(("QuickSaleProc函数执行失败。退出CProductSaleMainMenu::ProcEvent函数"));
			return FAILURE;
		}
		DBG_PRINT(("case DEPT1_KEY:"));
		return(DeptSaleProc(1));
		break;
		
		//部类2
	case DEPT2_KEY:
		if (pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE) 
		{
			CaMsgBox::ShowMsg("商品行数超限");
			return FAILURE;
		}
		if (QuickSaleProc()!=SUCCESS)
		{
			return FAILURE;
		}
		return(DeptSaleProc(2));
		break;
		
		//部类3
	case DEPT3_KEY:
		if (pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE) 
		{
			CaMsgBox::ShowMsg("商品行数超限");
			return FAILURE;
		}
		if (QuickSaleProc()!=SUCCESS)
		{
			return FAILURE;
		}
		return(DeptSaleProc(3));
		break;
		
		//部类4
	case DEPT4_KEY:
		if( pSaleData->m_tmpGoodsNum >= MAX_GOODS_LINE ) 
		{
			CaMsgBox::ShowMsg("商品行数超限");
			return FAILURE;
		}
		if (QuickSaleProc()!=SUCCESS)
		{
			return FAILURE;
		}
		return(DeptSaleProc(4));
		break;
		
		//部类5
	case DEPT5_KEY:
		if (pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE) 
		{
			CaMsgBox::ShowMsg("商品行数超限");
			return FAILURE;
		}
		if (QuickSaleProc()!=SUCCESS)
		{
			return FAILURE;
		}
		return(DeptSaleProc(5));
		break;
		
		// 	case SCANNER_EVENT_KEY:
		// 		DBG_PRINT((" SCANNER_EVENT_KEY 扫描枪处理函数"));
		// 		if (pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE) 
		// 		{
		// 			CaMsgBox::ShowMsg("商品行数超限");
		// 			return FAILURE;
		// 		}
		// 		content = (char*)ScannerCode();
		// 		DBG_PRINT((" 扫描枪读出的PLU码：%s", content));
		// 		code = "";
		//         nLen = GOODS_CODE_LEN - strlen(content);
		// 		if (nLen > 0)//不足13位，则前补零到13位
		// 		{
		// 			while(nLen > 0)
		// 			{
		// 				code += "0";
		// 				nLen--;
		// 			}
		// 		}
		// 		code += content;
		// 		//ret = pSaleData->PLUSale(code);
		// 		ret = pSaleData->DeptSale(0, code);
		// 		if (ret != SUCCEED) 
		// 	   {
		// 			DBG_PRINT((" 错误 ret = %u", ret));
		// 			ErrMsgBox(ret);
		// 			ClearScannerBuf();
		// 			return FAILURE;
		// 	   }
		// 	   //添加实现内容调整的函数 
		// 	   DeptSaleShow();
		// 	   pPSWin = (CProductSaleWin*)m_pFrame->GetWin(PRODUCT_SALE_WIN);
		// 	   if (pPSWin!=NULL)
		// 	   {
		// 		   pPSWin->ChangeTitle();
		// 		   pPSWin->ReFresh();
		// 	   }
		// 	   //切换到商品销售界面
		// 	   ChangeWin(PRODUCT_SALE_WIN);
		// 	   return SUCCESS;
		// 		break;
		
	case RETURN_MAIN_MENU:  //return to SYSTEMMENU
		ret = SYSTEM_MAIN_MENU;
		if ((pSaleData->m_invtype==RETURN_INV)||(pSaleData->m_invtype==RET_MANUAL_INV)||
			(pSaleData->m_invtype==RET_SPECIAL_INV))
		{
			ret = INV_MANAGE_MENU;
		}
		//		ScannerRelease();						//停止扫描枪扫描
		pSaleData->InitSaleData(1);				//销售信息初始化
		pSaleData->InitInvHead();				//单张发票信息初始化
		pSaleData->m_workState = WORK_COMPLETE; //销售状态初始化
		ChangeWin(ret);
		
		return SUCCESS;
		
		
	case FORWARD_KEY:
		return(ForwardKeyProc());
		break;
		
	case BACKWARD_KEY:
		ret = BackwardPaper();
		if (ret != SUCCESS) 
		{
			return(ErrMsgBox(NO_PAPER));
		}
		return SUCCESS;
		break;
		
	case CASH_KEY:
		//if (0 == g_globalArg->m_curInvVol->m_remain)
		if (0==g_globalArg->m_curInvVol->m_curInvNo)
		{
			return(ErrMsgBox(INV_ZERO));	
		}
		pSaleData->InitSaleData(0);//复原销售相关变量
		CaMsgBox::ShowMsg("未销售任何商品");
		return SUCCESS;	
		break;
		
	case CANCEL_GOODS:
		CaMsgBox::ShowMsg("未销售任何商品");
		return SUCCESS;	
		break;
		
	case TOTAL_CANCEL:
		CaMsgBox::ShowMsg("未销售任何商品");
		return SUCCESS;	
		break;
		
	case GOODSREDUCT_KEY:
		//	CaMsgBox::ShowMsg("未销售任何商品");
		CaMsgBox::ShowMsg("暂不支持此功能");
		return SUCCESS;	
		break;
		
	default: break;		
	}
	
	DBG_PRINT(("退出CProductSaleMainMenu::ProcEvent函数"));
 	  
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

int CProductSaleMainMenu::ReFresh()
{   
	DBG_PRINT(("进入ReFresh函数"));
	ReFreshTitle();
	CaGroup::ReFresh();
	if(m_pCurObj != NULL )
	{
		m_pCurObj->SetFocusIn();
	}
	LCDRedraw();
	
	return 1;
	DBG_PRINT(("退出ReFresh函数"));
}

UINT8 CProductSaleMainMenu::ErrMsgBox(UINT8 ret)
{
	DBG_PRINT(("进入ErrMsgBox函数"));
	
	string pText;
	switch(ret)
	{
	   case NO_SUCH_GOODS:
		   pText = "未找到该商品";
		   break;
	   case EXCEED_PRICE:
		   pText = "单价超限";
		   break;
	   case SHORT_OF_INV:
		   pText = "超过剩余发票份数";
		   break;
	   case TAX_ID_ILLEGAL:
		   pText = "税种税目错误";
		   break;
	   case TAX_UNAUTHORIZED:
		   pText = "税率未授权";
			break;	
	   case NM_EXCEED:
		   pText = "单张开票金额超限";
		   break;
	   case NM_SUM_EXCEED:
		   pText = "正票累计金额超限";
		   break;
	   case RM_SUM_EXCEED:
		   pText = "红票累计金额超限";
		   break;
	   case NVM_SUM_EXCEED:
		   pText = "单卷正票累计金额超限";
		   break;
	   case RVM_SUM_EXCEED:
		   pText = "单卷红票累计金额超限";
		   break;
	   case MONEY_NOT_ENOUGH:
		   pText = "现金金额不足";
		   break;
	   case REBATE_ERROR:
		   pText = "折扣率超限";
		   break;
	   case PRICE_NOT_ENOUGH:
		   pText = "价格不得小于0.01";
		   break;
	   case EXCEED_NAME_LEN:
		   pText = "付款人名称过长";
		   break;
	   case NO_NAME:
		   pText = "未输入付款人名称";
		   break;
	   case MUST_INPUT_SUM:
		   DBG_PRINT(("Please input total money!"));
		   pText = "请输入总价金额";
		   break;
	   case EXCEED_AMOUNT:
		   pText = "数量超限";
		   break;
	   case ILLEGAL_AMOUNT:
		   pText = "数量非法";
		   break;
	   case NO_PAPER:
		   pText = "没有纸质发票";
		   break;
	   case MONEY_ZERO:
		   pText = "商品金额为0,不得开具";
		   break;
	   case INV_ZERO:
		   pText = "无可用发票！";
		   break;
	   case DISCOUNT_MONEY_ZERO:
		   pText = "折扣行金额为零";
		   break;
	   default:;
	}  
	
	CaMsgBox::ShowMsg(pText);
	
	DBG_PRINT(("退出ErrMsgBox函数"));
	return FAILURE;	
}

UINT8 CProductSaleMainMenu::DeptSaleShow()
{
	string strTemp = "";
	INT32 nCurrentInvNo = g_globalArg->m_curInvVol->m_ieno + 1 - g_globalArg->m_curInvVol->m_remain;
	INT8 tmp[16];
	
    sprintf(title_arr[0], "当前发票号: %08d", g_globalArg->m_curInvVol->m_curInvNo);
	
	if ((DETAIL_GOODS_DISCOUNT == pSaleData->m_invDet->m_property) && (0 == m_strSuffix.length()))//有折扣 
	{
		m_strSuffix = "<折>";
	}
	
	strTemp = "商品: ";
    strTemp.append(pSaleData->m_invDet->m_spmc);
	strTemp.append(m_strSuffix);
    m_strSuffix = "";
    strcpy(title_arr[1], (strTemp.c_str()));
	
	sprintf(title_arr[2], "单价:%.2lf", pSaleData->m_invDet->m_spdj);
	sprintf(tmp, "数量:%%.%luf", pSaleData->m_invDet->m_dotNum);		//数量输出处理
	sprintf(title_arr[3], tmp, pSaleData->m_invDet->m_spsl);
	if ((pSaleData->m_invtype==RETURN_INV)||(pSaleData->m_invtype==RET_MANUAL_INV)||(pSaleData->m_invtype==RET_SPECIAL_INV)) 
	{
		sprintf(title_arr[4], "合计:-%.2lf", (pSaleData->m_singleInvInfo->m_kphjje)/100.0);
	}
	else
	{
		sprintf(title_arr[4], "合计:%.2lf", (pSaleData->m_singleInvInfo->m_kphjje)/100.0);
	}
	
	sprintf(title_arr[5], "商品行数:%u", pSaleData->m_tmpGoodsNum);
	
	return SUCCESS;	
}

void CProductSaleMainMenu::S_OnActive(CaWindow *obj)
{
	((CProductSaleMainMenu *)obj)->DoActive();
}


void CProductSaleMainMenu::DoActive()
{
	DBG_PRINT(("进入CProductSaleMainMenu::DoActive函数"));
	DBG_ENTER("CProductSaleMainMenu::DoActive()");

	// 	if (ScannerInit()!=SUCCESS)
	// 	{
	// 		CaMsgBox::ShowMsg("扫描枪初始化失败");
	// 	}
	
	if (pSaleData == NULL)
	{
		pSaleData = new SaleData;
		DBG_ASSERT_EXIT((pSaleData != NULL), (" pSaleData == NULL "));
	}
	
	UpdateSaleValue();	//更新销售变量中对应全局变量的值
	
	m_strSuffix = "";
	m_pInput2->Clear();	//清除Input里的内容

	NormalShow();
	
	ChangeTitle();

	//每卷第一张时提醒用户换卷
	if (1 == g_globalArg->m_curInvVol->m_curInvNo%DEFAULTE_VOL_NUM)
	{
	//	CaMsgBox::ShowMsg("请确认纸质发票卷与当前发票号对应!");
	}
	
	ReFresh();
	DBG_PRINT(("*****工作模式******：%u", g_globalArg->pSaveTemplateInfo->workMode));
	DBG_PRINT(("退出CProductSaleMainMenu::DoActive函数"));
}

int CProductSaleMainMenu::ChangeTitle()
{
	DBG_PRINT(("进入ChangeTitle函数"));
	
	string curtime = TDateTime::CurrentDate().FormatString(YYYYMMDD);
	//curtime += "  ";
	label[0]->SetTitle(curtime.c_str(), curtime.size());
	label[1]->SetTitle(title_array[1], strlen(title_array[1]));
	label[2]->SetTitle(title_array[2], strlen(title_array[2]));
	label[3]->SetTitle(title_array[3], strlen(title_array[3]));
	label[4]->SetTitle(title_array[4], strlen(title_array[4]));
	return 1;
	DBG_PRINT(("退出ChangeTitle函数"));
}

UINT8 CProductSaleMainMenu::ClientShow()
{
	string strTemp = "付款人: ";
    strTemp.append(pSaleData->m_singleInvInfo->m_fkdw); 
    strncpy(title_array[2], (strTemp.c_str()), OBJ_TITLE_MAX_LEN);//付款人较长，只能显示部分
	
	return SUCCESS;
}

UINT8 CProductSaleMainMenu::ClientCodeShow()
{
	string strTemp = "付款人税号: ";
    strTemp.append(pSaleData->m_singleInvInfo->m_fkdwsh); 
    strncpy(title_array[2], (strTemp.c_str()), OBJ_TITLE_MAX_LEN);//付款人较长，只能显示部分
	
	return SUCCESS;
}

//正常显示
UINT8 CProductSaleMainMenu::NormalShow()
{
	DBG_PRINT(("进入NormalShow函数"));
	string strErr;
	UINT8 ret = SUCCESS;
	INT32 nCurrentInvNo  = 0;

	//当前发票号
	if (g_globalArg->m_operator->m_role==DEMO_ROLE) 
	{
		nCurrentInvNo = g_globalArg->m_curInvVol->m_ieno + 1 - g_globalArg->m_curInvVol->m_remain;
		sprintf(title_array[1], "当前发票号: %08d", nCurrentInvNo);
	}
	else
	{
 		if (0 == g_globalArg->m_curInvVol->m_curInvNo) //避免反复读盘
		 {
		 	ret = SALE_GetCurInv(g_globalArg->m_curInvVol,strErr);
		 	DBG_PRINT(("strErr= %s",strErr.c_str()));
		 	if (ret ==FAILURE)
		 	{
		 		sprintf(title_array[1], strErr.c_str()); //错误
		 	} 	
		 }	
	}
	
	DBG_PRINT((" g_globalArg->m_curInvVol->m_ieno = %u !", g_globalArg->m_curInvVol->m_ieno));
	DBG_PRINT((" g_globalArg->m_curInvVol->m_remain = %u !", g_globalArg->m_curInvVol->m_remain));
	
	//第一行
	if( 0 != g_globalArg->m_curInvVol->m_curInvNo) 
	 {
		sprintf(title_array[1], "当前发票号: %08d", g_globalArg->m_curInvVol->m_curInvNo);
	 }
	
	
	
	//第二行
	strcpy(title_array[2], "");
	
	//第三行
	if (g_globalArg->m_operator->m_role==DEMO_ROLE) 
	{
		strcpy(title_array[3], "模式: 训练");		
	}
	else
	{
		strcpy(title_array[3], "模式: 正常");
	}
	
	sprintf(title_array[4], "操作员: %s", g_globalArg->m_operator->m_name.c_str());
	
	DBG_PRINT(("退出NormalShow函数"));
	return SUCCESS;	
}

UINT8 CProductSaleMainMenu::ForwardKeyProc()
{
	UINT8 ret;
	
	if (g_globalArg->pSaveTemplateInfo->markFlag == 1)//若找黑标
	{
		CaMsgBox msgBox("是否进纸?",CaMsgBox::MB_YESNO);
		msgBox.ShowBox();
		if ((msgBox.m_iStatus == NO_PRESSED)||(msgBox.m_iStatus == CANCEL_PRESSED))
		{
			return SUCCESS;
		}
		
		if(msgBox.m_iStatus == OK_PRESSED)
		{
			this->ReFresh();
			ret = ForwardPaper();
			if (ret != SUCCESS) 
			{
				return(ErrMsgBox(NO_PAPER));
			}		
		}
	}
	else 
	{
		ret = ForwardPaper();
		if (ret != SUCCESS) 
		{
			return(ErrMsgBox(NO_PAPER));
		}		
	}
	
	ReFresh();
	return SUCCESS;
}

UINT8 CProductSaleMainMenu::DeptSaleProc(UINT32 deptNo)
{
	UINT8 ret;
	DBG_PRINT(("进入CProductSaleMainMenu::DeptSaleProc函数"));
	DBG_PRINT(("deptNo==%u", deptNo));
	
	ret = pSaleData->DeptSale(deptNo, "");
	if( ret != SUCCEED ) 
	{
		m_strSuffix = "";
		DBG_PRINT(("准备退出DeptSaleProc函数"));
		return(ErrMsgBox(ret));
	}
	//添加实现内容调整的函数 
	DeptSaleShow();
	pPSWin = (CProductSaleWin*)m_pFrame->GetWin(PRODUCT_SALE_WIN);
	if (pPSWin!=NULL)
	{
		pPSWin->ChangeTitle();
		pPSWin->ReFresh();
	}
	//切换到商品销售界面
	ChangeWin(PRODUCT_SALE_WIN);
	
	DBG_PRINT(("退出CProductSaleMainMenu::DeptSaleProc函数"));
	return SUCCESS;
}

UINT8 CProductSaleMainMenu::ClientInputProc(void)
{
	char *content;
	UINT8 ret;
	char *blank = "";
	INT8 chValue[128];
	memset((void*)chValue, 0, sizeof(chValue));
	
	content = (char*)(m_pInput2->m_contentBuf);
	if ('.' == *content) 
	{
		CClient client;
		sprintf(chValue, "where KH_BH = '%s'", (content+1));
		client.m_filter = chValue;
		client.Requery();
		if(client.LoadOneRecord()!= SQLITE_OK)
		{
			CaMsgBox::ShowMsg("没有对应的付款人");
			return FAILURE;
		}
		ret = pSaleData->ClientNameInput(client.m_khmc.c_str(), client.m_khsh.c_str());		
	}
	else
	{
		ret = pSaleData->ClientNameInput(content);
	}
	
	m_pInput2->Clear();
	if (ret != SUCCESS) 
	{
		return(ErrMsgBox(ret));
	}
	ClientShow();
	ChangeTitle();
	ReFresh();
	strcpy(title_array[2], blank);
	ChangeTitle();
	return SUCCESS;
}

UINT8 CProductSaleMainMenu::ClientCodeInputProc(void)
{
	char *content;
	UINT8 ret;
	char *blank = "";
	INT8 chValue[128];
	memset((void*)chValue, 0, sizeof(chValue));
	string srrErr("");
	
	content = (char*)(m_pInput2->m_contentBuf);

	ret = pSaleData->ClientCodeInput(content,srrErr);

	m_pInput2->Clear();
	if (ret != SUCCESS) 
	{
		CaMsgBox::ShowMsg(srrErr);
		return FAILURE;
	}

	ClientCodeShow();
	ChangeTitle();
	ReFresh();
	strcpy(title_array[2], blank);
	ChangeTitle();
	return SUCCESS;
}

UINT8 CProductSaleMainMenu::PriceInputProc(void)
{
	DBG_PRINT(("进入CProductSaleMainMenu::PriceInputProc函数"));
	UINT8 ret = 0;
	double ii;
	char *content;
	INT32 dotNum;//小数位数
	
	content = (char*)(m_pInput2->m_contentBuf);
	ii = atof(content);
	DBG_PRINT(("ii= %lf",ii));
 	if(ii > MAX_MONEY)
	{
			CaMsgBox::ShowMsg("金额超过最大允许值");
	 		return FAILURE;
 	}	
	dotNum = CheckFloatBit(ii);
	if (dotNum>3) 
	{
		DBG_PRINT(("--------ii = %f--------", ii));
		DBG_PRINT(("--------dotNum = %d--------", dotNum));
		CaMsgBox::ShowMsg("小数位数不得超过三位");
		return FAILURE;
	}
	m_pInput2->Clear();
	ret = pSaleData->PriceInput(ii);
	if (ret != SUCCESS) 
	{
		return(ErrMsgBox(ret));
	}
	ReFresh();
	DBG_PRINT(("退出CProductSaleMainMenu::PriceInputProc函数"));
	return SUCCESS;
}

UINT8 CProductSaleMainMenu::SumInputProc(void)
{
	UINT8 ret = 0;
	double ii;
	char *content;
	INT32 dotNum;//小数位数
	
	DBG_PRINT(("进入SumInputProc函数"));
	
	content = (char*)(m_pInput2->m_contentBuf);		//m_pInput2就是用户输入的总价
	ii = atof(content);
	DBG_PRINT(("ii= %lf",ii));
	 if(ii > MAX_MONEY)
	 {
			CaMsgBox::ShowMsg("金额超过最大允许值");
	 		return FAILURE;
 	}	
	dotNum = CheckFloatBit(ii);
	if (dotNum>2) 
	{
		DBG_PRINT(("--------ii = %f--------", ii));
		DBG_PRINT(("--------dotNum = %d--------", dotNum));
		CaMsgBox::ShowMsg("小数位数不得超过两位");
		return FAILURE;
	}
	m_pInput2->Clear();
	ret = pSaleData->SumInput(ii);
	if (ret != SUCCESS) 
	{
		return(ErrMsgBox(ret));
	}
	ReFresh();
	DBG_PRINT(("退出SumInputProc函数"));
	return SUCCESS;
}

UINT8 CProductSaleMainMenu::PlusProc(void)
{
	DBG_PRINT(("进入PlusProc函数"));
	UINT8 ret = 0;
	double ii;
	char *content;
	INT32 dotNum;//小数位数
	
	content = (char*)(m_pInput2->m_contentBuf);
	ii = atof(content);
	DBG_PRINT(("ii= %lf",ii));
	if(ii > MAX_MONEY)
 	{
 		CaMsgBox::ShowMsg("数量超限");
 		return FAILURE;
}
 		m_pInput2->Clear();
	ret = pSaleData->Plus(ii);
	if (ret != SUCCESS) 
	{
		return(ErrMsgBox(ret));
	}
	ReFresh();
	DBG_PRINT(("退出PlusProc函数"));
	return SUCCESS;
}

UINT8 CProductSaleMainMenu::QuickSaleProc(void)
{
	UINT8 ret;
	DBG_PRINT(("进入QuickSaleProc函数"));
	
	if( m_pInput2->IsEmpty() )
	{
		DBG_PRINT(("退出QuickSaleProc函数"));
		return SUCCESS;
	}
	
	//根据默认销售模式选择不同的函数执行。
	DBG_PRINT(("g_globalArg->m_saleMode == %d", g_globalArg->m_saleMode));
	switch( g_globalArg->m_saleMode ) 
	{
	case 0:
		ret = SumInputProc();
		break;
	case 1:
		ret = PriceInputProc();
		break;
	case 2:
		ret = PlusProc();
		break;
	default:
		break;
	}
	
	DBG_PRINT(("ret == %d", ret));
	DBG_PRINT(("退出QuickSaleProc函数"));
	return ret;	
}

//----------------------------------------------------------
//更新销售类对象中对应全局变量的值
//----------------------------------------------------------
UINT8 CProductSaleMainMenu::UpdateSaleValue(void)
{
	DBG_PRINT(("进入UpdateSaleValue函数"));
	pSaleData->m_singleInvInfo->m_fkdw = g_globalArg->m_defaultPayerName;	//付款人名称
	pSaleData->m_singleInvInfo->m_fkdwsh = "";	//付款单位税号
	pSaleData->m_singleInvInfo->m_sky = g_globalArg->m_operator->m_name;	//收款员
	//	pSaleData->m_singleInvInfo->m_skyid = g_globalArg->m_operator->m_opid;	//收款员ID
	
	DBG_PRINT(("pSaleData->m_singleInvInfo->m_sky == %s", pSaleData->m_singleInvInfo->m_sky.c_str()));
	//	DBG_PRINT(("pSaleData->m_singleInvInfo->m_skyid == %u", pSaleData->m_singleInvInfo->m_skyid));
	
	pSaleData->m_saleMode = g_globalArg->m_saleMode;						//销售模式，总价、临时价，数量	
	DBG_PRINT(("退出UpdateSaleValue函数"));
	
	return SUCCESS;
}

