/*! \file    ProductSaleWin.cpp
   \brief    awe4000r销售程序中商品销售窗口
   \author   Yu Yan
   \version  1.0
   \date     2007-11-28
 */


#include "ProductSaleWin.h"
//#include "VolSumInfoWin.h"
#include "printer.h"
#include "CaProgressBar.h"
#include "CClient.h"
#include "Scanner.h"
#include "beep.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

char title_arr[7][33];		//CProductSaleWin的Label的标题


CProductSaleWin::CProductSaleWin():CaWindow()
{
	m_IfCancel = false;
	m_strSuffix = "";
}

int CProductSaleWin::Create(int iX,int iY,int iW,int iH)
{
	int curH;
	char title[OBJ_TITLE_MAX_LEN + 1];
//	string curtime = TDateTime::CurrentDateTime().FormatString(YYYYMMDDHHMMSS);
	string curtime = TDateTime::CurrentDate().FormatString(YYYYMMDD);
	curtime += "  ";
    int titleLen=0;
	
    m_iBtnW = (SCREEN_W/2 - 0 ) ;	//Label的宽度
	m_iColW = m_iBtnW + 0;			//Label的列宽
    
	m_pFrame->RegsiterWin(this,PRODUCT_SALE_WIN);
   
	CaWindow::Create(iX,iY,iW,iH); // creat a window
	//创建一个标签 第一行
	label[0] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH = SCREEN_TOP_OFFSET;
	label[0]->Create(0,curH, SCREEN_W, CHAR_H);
	label[0]->SetTitle(title_arr[0], strlen(title_arr[0]));

	//创建一个标签 第二行
	label[1] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[1]->Create(0,curH, SCREEN_W, CHAR_H); //参数是坐标
	label[1]->SetTitle(title_arr[1], strlen(title_arr[1]));
    
	//创建一个标签 第三行
	label[2] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[2]->Create(0,curH, m_iBtnW, CHAR_H); //参数是坐标
	label[2]->SetTitle(title_arr[2], strlen(title_arr[2]));

	//创建一个标签 第三行
	label[3] = new CaLabel(false,CaObject::ALIGN_LEFT);
	label[3]->Create(m_iColW,curH, m_iBtnW, CHAR_H);
	label[3]->SetTitle(title_arr[3], strlen(title_arr[3]));

    //创建一个标签 第四行
	label[4] = new CaLabel(false,CaObject::ALIGN_LEFT);
	curH += LINE_H;
	label[4]->Create(0,curH, m_iBtnW, CHAR_H); //参数是坐标
	label[4]->SetTitle(title_arr[4], strlen(title_arr[4]));

	//创建一个标签 第四行
	label[5] = new CaLabel(false,CaObject::ALIGN_LEFT);
	label[5]->Create(m_iColW,curH, m_iBtnW, CHAR_H);
	label[5]->SetTitle(title_arr[5], strlen(title_arr[5]));
	
   	curH += LINE_H;
	strcpy(title, "[输入]");
	titleLen = strlen(title) * CHAR_W;
	m_pInput2=new CaInput(CaObject::ON_LEFT,titleLen);
	m_pInput2->Create(0,curH,SCREEN_W,LINE_H);	
	m_pInput2->SetTitle(title,titleLen / CHAR_W);	
	m_pInput2->SetMaxLen(PAYER_NAME_LEN);

	this->End();
	return 0;
}

//---------------------------------------------------------------------------------
//处理事件
//---------------------------------------------------------------------------------
int CProductSaleWin::ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen)
{
	UINT8 ret;
	double ii;
	char *content;
	string code = "";
	UINT32 deptNo;
	UINT8 IfInvSum = 0;
	INT32 dotNum = 0;
	INT32 nWin;
	INT32 nLen  = 0;
	UINT8 nIndex = 0;
	UINT32 nCurDate;
	string strErr("");

	switch(iEvent) 
	{
	case ENTER_KEY:
		m_strSuffix = "";

		nCurDate = TDateTime::CurrentDateTime().FormatInt(YYYYMMDD);

		//判断装入纸质发票	
		if (isPaper() != 0)
		{
			DBG_PRINT(("没有纸质发票"));
			return(ErrMsgBox(NO_PAPER));
		}
		content = (char*)(m_pInput2->m_contentBuf);
		ii = atof(content);
		m_pInput2->Clear();
		ret = CashShow(ii);
		if (ret != SUCCEED) 
		{
			return(ErrMsgBox(ret));
		}	
		ChangeTitle();
		ReFresh();
		
		if (m_IfCancel)		//之前按了商品取消键
		{
			pSaleData->NoCancelGoods();
			m_IfCancel = false;
			m_strSuffix = "";
		}
		DBG_PRINT(("*****工作模式******：%u", g_globalArg->pSaveTemplateInfo->workMode));
 		ret = pSaleData->PayByCash(IfInvSum);
		DBG_PRINT(("IfInvSum == %u", IfInvSum));
		if ((ret == NO_GOODS)||(ret == DB_FULL))
		{
			ErrMsgBox(ret);
			ChangeWin(PRODUCT_SALE_MAIN_MENU); //返回销售档主界面
			return SUCCESS;
		}

		if (ret != SUCCEED) 
		{
			ErrMsgBox(ret);
			switch(ret) 
			{
				case GET_FISCAL_CODE_ERROR://取税控码失败
					break;
				case UPDATE_INV_VOL_ERROR://写数据库失败
					break;
				case DB_SAVE_ERROR://写数据库失败
					break;
				case MALLOC_MEM_ERROR:
					break;
				case INV_SUM_ERROR:
					break;
				case MUST_RE_LOGIN:
					break;
				case HAVE_ROLLED:
					break;
				case NM_EXCEED_MIN:   //发票金额超下限,退回到部类销售显示，等待再次结帐
				case NO_PAPER: //没纸，退回到部类销售显示，等待再次结帐
					DeptSaleShow();
					ChangeTitle();
					ReFresh();
					return FAILURE;
					break;
				case INV_LIB_ERR:
					break;
				case GX_SV_NUM_EXCEED:
					break;
				case GX_SV_TIME_EXCEED:
					break;
				case GX_SV_SUM_EXCEED: 
					break;
				default:
					return FAILURE;//留在商品销售界面
			}
		}
		else
		{
			DBG_PRINT(("启动开钱箱"));
			MoneyBox_Open();
		}

		if (pSaleData->m_returnOther == 1)
		{
			pSaleData->m_returnOther = 0;
//			ScannerRelease();//停止扫描枪扫描
//			g_winNo = 3;//对应INV_MANAGE_MENU界面
			nWin = INV_MANAGE_MENU;
		}
		else
		{
//			g_winNo = 2;//对应INV_MANAGE_MENU界面
			nWin = PRODUCT_SALE_MAIN_MENU;
		}
// 		if (1==IfInvSum) 
// 		{
// 			InvSumShowPrn();//显示卷汇总并打印
// 		}
// 		else
// 		{
			ChangeWin(nWin); //返回销售档主界面
//		}

		return SUCCESS;
		break;

    case DISCOUNT_KEY:

		if ((pSaleData->m_tmpGoodsNum>=(MAX_GOODS_LINE-1))&&(!m_IfCancel)) 
		{
			CaMsgBox::ShowMsg("商品行数超限");
			return FAILURE;
		}
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
// 		CaMsgBox::ShowMsg("暂不支持此功能");
// 		return SUCCESS;
		break;

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

	case HELP_KEY:
		return(RemarksInputProc());
		break;

	case CANCEL_GOODS:
		
		if (m_IfCancel)//之前按了商品取消键
		{
			pSaleData->NoCancelGoods();
			m_IfCancel = false;
			m_strSuffix = "";
			DeptSaleShow();
		}
		else
		{
			if (pSaleData->m_tmpGoodsNum==0)//无商品可取消
			{
				ErrMsgBox(NO_GOODS_CANCEL);	
				ChangeWin(PRODUCT_SALE_MAIN_MENU);
				return SUCCESS;
			}
			pSaleData->CancelGoods();
			m_IfCancel = true;
			m_strSuffix = "<消>";
			CancelGoodsShow();
		}		
		ChangeTitle();
	    ReFresh();
		return SUCCESS;
		break;

	case PLUS_KEY:
		return(PlusProc());
		break;

	case TOTAL_CANCEL:
		if (m_IfCancel)//之前按了商品取消键
		{
			pSaleData->NoCancelGoods();
			m_IfCancel = false;
			m_strSuffix = "";
		}
		return(TotalCancelProc());
		break;

	case PLU_KEY:
		if ((pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE)&&(!m_IfCancel)) 
		{
			CaMsgBox::ShowMsg("商品行数超限");
			return FAILURE;
		}
		content = (char*)(m_pInput2->m_contentBuf);
		code = "";
        nLen = GOODS_CODE_LEN - strlen(content);
		if (nLen > 0)//不足13位，则前补零到13位
		{
			while(nLen > 0)
			{
				code += "0";
				nLen--;
			}
		}		
		code += content;
		ret = pSaleData->DeptSale(0, code);
		m_pInput2->Clear();
		if (m_IfCancel&&(ret == NO_SUCH_GOODS))//按商品取消键后,未在发票明细中找到该商品
		{
			ErrMsgBox(ret);
			pSaleData->NoCancelGoods();
			m_IfCancel = false;
			m_strSuffix = "";
			DeptSaleShow();
			ChangeTitle();
			ReFresh();
			return SUCCESS;
		}
		if (ret != SUCCEED) 
	   {
			m_strSuffix = "";
		   return(ErrMsgBox(ret));
	   }
	   //添加实现内容调整的函数 
	   DeptSaleShow();
	   ChangeTitle();
	   ReFresh();	
	   //切换到商品销售界面
	   ChangeWin(PRODUCT_SALE_WIN);
	   return SUCCESS;
		break;
	
	case SHIFT_PLU_KEY: //临时输入商品名称
		if ((pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE)&&(!m_IfCancel)) 
		{
			CaMsgBox::ShowMsg("商品行数超限");
			return FAILURE;
		}
		content = (char*)(m_pInput2->m_contentBuf);
		ret = pSaleData->PLUSale(content, strErr);			
		if (m_IfCancel&&(ret == NO_SUCH_GOODS))//按商品取消键后,未在发票明细中找到该商品
		{
			ErrMsgBox(ret);
			pSaleData->NoCancelGoods();
			m_IfCancel = false;
			m_strSuffix = "";
			DeptSaleShow();
			ChangeTitle();
			ReFresh();
			return SUCCESS;
		}
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
	   ChangeTitle();
	   ReFresh();	
	   //切换到商品销售界面
	   ChangeWin(PRODUCT_SALE_WIN);
	   return SUCCESS;
		break;
	
// 	case SCANNER_EVENT_KEY:
// 		if ((pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE)&&(!m_IfCancel)) 
// 		{
// 			CaMsgBox::ShowMsg("商品行数超限");
// 			ClearScannerBuf();
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
// 		m_pInput2->Clear();
// 		if (m_IfCancel&&(ret == NO_SUCH_GOODS))//按商品取消键后,未在发票明细中找到该商品
// 		{
// 			ErrMsgBox(ret);
// 			ClearScannerBuf();
// 			pSaleData->NoCancelGoods();
// 			m_IfCancel = false;
// 			m_strSuffix = "";
// 			DeptSaleShow();
// 			ChangeTitle();
// 			ReFresh();
// 			return SUCCESS;
// 		}
// 		if (ret != SUCCEED) 
// 	   {
// 			ErrMsgBox(ret);
// 			ClearScannerBuf();
// 			return FAILURE;
// 	   }
// 	   //添加实现内容调整的函数 
// 	   DeptSaleShow();
// 	   ChangeTitle();
// 	   ReFresh();	
// 	   //切换到商品销售界面
// 	   ChangeWin(PRODUCT_SALE_WIN);
// 	   return SUCCESS;
// 		break;

	case CHANGE_DEPT_KEY:
		if ((pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE)&&(!m_IfCancel)) 
		{
			CaMsgBox::ShowMsg("商品行数超限");
			return FAILURE;
		}
		content = (char*)(m_pInput2->m_contentBuf);
		deptNo = atoi(content);
		m_pInput2->Clear();
		return(DeptSaleProc(deptNo));
		break;	

	case DEPT1_KEY:
		if ((pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE)&&(!m_IfCancel)) 
		{
			CaMsgBox::ShowMsg("商品行数超限");
			return FAILURE;
		}
		if (QuickSaleProc()!=SUCCESS)
		{
			return FAILURE;
		}
		return(DeptSaleProc(1));
	    break;

	case DEPT2_KEY:
		if ((pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE)&&(!m_IfCancel)) 
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
	   
	case DEPT3_KEY:
		if ((pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE)&&(!m_IfCancel)) 
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
	   
	case DEPT4_KEY:
		if ((pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE)&&(!m_IfCancel)) 
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

	case DEPT5_KEY:
		if ((pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE)&&(!m_IfCancel)) 
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
		
	case RETURN_MAIN_MENU:  //return to SYSTEMMENU
		if (pSaleData->m_tmpGoodsNum==0)
		{
			ChangeWin(PRODUCT_SALE_MAIN_MENU);
		}
		else
		{
			CaMsgBox::ShowMsg("商品行数非零，不能返回");
		}
		return SUCCESS;
		break;

	case GOODSREDUCT_KEY:
/*
		if ((pSaleData->m_tmpGoodsNum>=MAX_GOODS_LINE)&&(!m_IfCancel)) 
		{
			CaMsgBox::ShowMsg("商品行数超限");
			return FAILURE;
		}
		GoodesReductProc();*/
		CaMsgBox::ShowMsg("暂不支持此功能");
		return SUCCESS;
		break;
		
	default:
		break;
	}
	
	return CaWindow::ProcEvent(iEvent,pEventData,iDataLen);
}

//bool CProductSaleWin::IsCurWin()
//{
//	return true;
//}

int CProductSaleWin::ReFresh()
{
	ReFreshTitle();
	CaGroup::ReFresh();
	if(m_pCurObj != NULL )
	{
 		m_pCurObj->SetFocusIn();
	}
	LCDRedraw();

	return 1;
}

int CProductSaleWin::ChangeTitle()
{
	label[0]->SetTitle(title_arr[0], strlen(title_arr[0]));
	label[1]->SetTitle(title_arr[1], strlen(title_arr[1]));
	label[2]->SetTitle(title_arr[2], strlen(title_arr[2]));
	label[3]->SetTitle(title_arr[3], strlen(title_arr[3]));
	label[4]->SetTitle(title_arr[4], strlen(title_arr[4]));
	label[5]->SetTitle(title_arr[5], strlen(title_arr[5]));

	return 1;
}

int CProductSaleWin::ErrMsgBox(UINT8 ret)
{
	string pText;
	switch(ret)
	   {
	   case MONEY_NOT_ENOUGH:
		    pText = "现金金额不足";
			break;
	   case MONEY_EXCEED:
		    pText = "现金金额超限";
			break;
	   case NO_PAPER:
		    pText = "没有纸质发票";
			break;
	   case PRN_INV_ERROR:
		   pText = "开票失败";
			break;
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
	   case UPDATE_INV_VOL_ERROR:
		    pText = "更新发票领用信息表失败";
			break;
	   case GET_FISCAL_CODE_ERROR:
		    //pText = "取税控码失败";
		    pText = "生成防伪码失败";
			break;
	   case DB_SAVE_ERROR:
		    pText = "存数据库失败";
			break;
	   case MALLOC_MEM_ERROR:
		    pText = "申请动态内存失败";
			break;
	   case INV_SUM_ERROR:
			pText = "卷汇总失败";
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
			pText = "请输入总价金额";
			break;
		case EXCEED_AMOUNT:
			pText = "数量超限";
			break;
		case NO_GOODS:
			pText = "没有商品";
			break;
		case MUST_RE_LOGIN:
			pText = "发票卷已用完，\n必须重登录";
			break;
		case NO_GOODS_CANCEL:
			pText = "无商品可取消";
			break;
		case ILLEGAL_AMOUNT:
			pText = "数量非法";
			break;
		case RETURN_GOODS_EXCEED:
			pText = "退货所需发票张数大于1";
			break;
		case DB_FULL:
			pText = "存储空间已满，无法开票 请尽快申报";
			break;
		case HAVE_ROLLED:
			pText = "信息整理成功";
			break;
		case REDUCT_EXCEED:
			pText = "折让金额超过商品行金额";
			break;
		case NO_REDUCT:
			pText = "非普通商品行，不能折让";
			break;
		case MONEY_ZERO:
			pText = "商品金额为0，不得开具";
			break;
		case INV_ZERO:
			pText = "无可用发票！";
			break;
		case DISCOUNT_MONEY_ZERO:
			pText = "折扣行金额为零";
			break;
		case REDUCT_MONEY_SMALL:
			pText = "折让金额过小";
			break;
		case ONE_INV:
			pText = "超过一张发票允许的商品行数";
			break;
		case NO_NET:
			pText = "没有网络连接";
			break;
 		case INV_LIB_ERR:
 	//		pText = "发票上传失败";
			pText = g_globalArg->m_strMsg;
			g_globalArg->m_strMsg = "";
 			break; 
		case GX_SV_NUM_EXCEED:
			pText = "离线开票张数超限";
			break;
		case GX_SV_SUM_EXCEED:
			pText = "离线开票金额超限";
			break;
		case GX_SV_TIME_EXCEED:
			pText = "离线开票时间超限";
			break;
		case NM_EXCEED_MIN:
			pText =  "总金额低于最小开票金额";
			break;
	   default:;
	   }
	   
		CaMsgBox::ShowMsg(pText);
	
		return FAILURE;
}

//--------------------------------------------------------------
//让Label显示找零打印内容
//--------------------------------------------------------------
UINT8 CProductSaleWin::CashShow(double cash)
{
	DBG_PRINT(("ENTER --CashShow-- FUNCTION!!!"));
	
	string strTemp = "";
	UINT8 ret;
    double payback = 0.0;
	INT32 nCurrentInvNo = g_globalArg->m_curInvVol->m_ieno + 1 - g_globalArg->m_curInvVol->m_remain;
	double moneySum = ((double)(pSaleData->m_singleInvInfo->m_kphjje))/SUM_EXTENSION;

	DBG_PRINT(("nCurrentInvNo == %d", nCurrentInvNo));
	DBG_PRINT(("cash == %f", cash));

	//若输入了现金值
	if( (cash<-MINIMUM_PRECISION) || (cash>MINIMUM_PRECISION) )
	{
		if (cash<moneySum)
		{
			ret = MONEY_NOT_ENOUGH;	
			return ret;
		}
		if(cash > MAX_MONEY)
		{
			ret = MONEY_EXCEED;
			return ret;
		}	
		payback = cash - moneySum; //找回现金
	}
	//若未输入现金值
	else
	{
		cash = moneySum;
	}
	
	DBG_PRINT(("合计 == %f", moneySum));
	
	DBG_PRINT(("g_globalArg->m_curInvVol->m_curInvNo= %u",g_globalArg->m_curInvVol->m_curInvNo));
    sprintf(title_arr[0], "当前发票号: %08d", g_globalArg->m_curInvVol->m_curInvNo);

	if ((pSaleData->m_invtype==RETURN_INV)||(pSaleData->m_invtype==RET_MANUAL_INV)||(pSaleData->m_invtype==RET_SPECIAL_INV)) 
	{
		sprintf(title_arr[1], "合计:-%.2lf", moneySum);
		sprintf(title_arr[2], "实付:");
		sprintf(title_arr[3], "找回:", payback);	
	}
	else
	{
		sprintf(title_arr[1], "合计:%.2lf", moneySum);
		sprintf(title_arr[2], "实付:%.2lf", cash);
		sprintf(title_arr[3], "找回:%.2lf", payback);

	}

#if 0
	   DBG_PRINT(("g_globalArg->m_invKind->m_nNum = %u",g_globalArg->m_invKind->m_nNum));
		if(0 == g_globalArg->m_invKind->m_nNum)
		{
         	strcpy(title_arr[4], "发票信息上传中");
		}
		else
		{
          	strcpy(title_arr[4], "发票信息打印中");
		}
#endif

	//strcpy(title_arr[4], "发票信息打印中");
	strcpy(title_arr[5], "......");
    
	ret = SUCCEED;
	return ret;
}

UINT8 CProductSaleWin::DeptSaleShow()
{
	string strTemp = "";
	INT32 nCurrentInvNo = g_globalArg->m_curInvVol->m_ieno + 1 - g_globalArg->m_curInvVol->m_remain;
	INT8 tmp[16];

    DBG_PRINT(("g_globalArg->m_curInvVol->m_curInvNo= %u",g_globalArg->m_curInvVol->m_curInvNo));
    sprintf(title_arr[0], "当前发票号: %08d", g_globalArg->m_curInvVol->m_curInvNo);
	
	//有折扣
	if( (DETAIL_GOODS_DISCOUNT == pSaleData->m_invDet->m_property) && (0 == m_strSuffix.length()) ) 
	{
		m_strSuffix = "<折>";
	}
	strTemp = "商品: ";
    strTemp.append(pSaleData->m_invDet->m_spmc);
	strTemp.append(m_strSuffix);
    m_strSuffix = "";
    strcpy(title_arr[1], (strTemp.c_str()));

	sprintf(title_arr[2], "单价:%.2lf", pSaleData->m_invDet->m_spdj);
	sprintf(tmp, "数量:%%.%luf", pSaleData->m_invDet->m_dotNum); //数量输出处理
	sprintf(title_arr[3], tmp, pSaleData->m_invDet->m_spsl);
	if (((pSaleData->m_invtype==RETURN_INV)||(pSaleData->m_invtype==RET_MANUAL_INV)||(pSaleData->m_invtype==RET_SPECIAL_INV))
		&&(0!=pSaleData->m_singleInvInfo->m_kphjje)) 
	{
		sprintf(title_arr[4], "合计:-%.2lf", (pSaleData->m_singleInvInfo->m_kphjje)/100.0);
	}
	else
	{
		sprintf(title_arr[4], "合计:%.2lf", (pSaleData->m_singleInvInfo->m_kphjje)/100.0);
	}
	
	sprintf(title_arr[5], "商品行数:%u", pSaleData->m_tmpGoodsNum);
	
	if (m_IfCancel)
	{	
		m_IfCancel = false;
	}

	return SUCCESS;	
}

UINT8 CProductSaleWin::ClientShow()
{
	string strTemp = "付款人: ";
	
    strTemp.append(pSaleData->m_singleInvInfo->m_fkdw); 
	strncpy(title_arr[1], (strTemp.c_str()), OBJ_TITLE_MAX_LEN);//付款人较长，只能显示部分
	sprintf(title_arr[2], "");
	sprintf(title_arr[3], "");	
	
	return SUCCESS;	
}

UINT8 CProductSaleWin::ClientCodeShow()
{
	string strTemp = "付款人税号: ";
	
    strTemp.append(pSaleData->m_singleInvInfo->m_fkdwsh); 
	strncpy(title_arr[1], (strTemp.c_str()), OBJ_TITLE_MAX_LEN);//付款人较长，只能显示部分
	sprintf(title_arr[2], "");
	sprintf(title_arr[3], "");	
	
	return SUCCESS;	
}

UINT8 CProductSaleWin::CancelGoodsShow()
{
	string strTemp = "";
	INT32 nCurrentInvNo = g_globalArg->m_curInvVol->m_ieno + 1 - g_globalArg->m_curInvVol->m_remain;
	
    DBG_PRINT(("g_globalArg->m_curInvVol->m_curInvNo= %u",g_globalArg->m_curInvVol->m_curInvNo));
    sprintf(title_arr[0], "当前发票号: %08d <商品取消>", g_globalArg->m_curInvVol->m_curInvNo);
	sprintf(title_arr[1], "");
	sprintf(title_arr[2], "");
	sprintf(title_arr[3], "");
	sprintf(title_arr[5], "商品行数:%u", pSaleData->m_tmpGoodsNum);
	
	if ((pSaleData->m_invtype==RETURN_INV)||(pSaleData->m_invtype==RET_MANUAL_INV)||(pSaleData->m_invtype==RET_SPECIAL_INV)) 
	{
		sprintf(title_arr[4], "合计:-%.2lf", (pSaleData->m_singleInvInfo->m_kphjje)/100.0);
	}
	else
	{
		sprintf(title_arr[4], "合计:%.2lf", (pSaleData->m_singleInvInfo->m_kphjje)/100.0);
	}
	return SUCCESS;	
}

UINT8 CProductSaleWin::TotalCancelProc()
{
	UINT8 ret;
	CaMsgBox msgBox("是否整单取消?",CaMsgBox::MB_YESNO);
	msgBox.ShowBox();
	if ((msgBox.m_iStatus == NO_PRESSED)||(msgBox.m_iStatus == CANCEL_PRESSED))
	{
		return SUCCESS;
	}
	
	if(msgBox.m_iStatus == OK_PRESSED)
	{
		this->ReFresh();
		ret = pSaleData->TotalCancel();
		if (ret != SUCCESS) 
		{
		   return(ErrMsgBox(ret));
		}
		m_strSuffix = "";
		m_pInput2->Clear();
		ChangeWin(PRODUCT_SALE_MAIN_MENU); //返回销售档主界面
	}
	return SUCCESS;
}

UINT8 CProductSaleWin::DeptSaleProc(UINT32 deptNo)
{
	UINT8 ret;
	DBG_PRINT(("进入CProductSaleWin::DeptSaleProc函数"));
	DBG_PRINT(("deptNo == %u", deptNo));
	ret = pSaleData->DeptSale(deptNo, "");
	
	if (m_IfCancel&&(ret == NO_SUCH_GOODS))//按商品取消键后,未在发票明细中找到该商品
	{
		ErrMsgBox(ret);
		pSaleData->NoCancelGoods();
		m_IfCancel = false;
		m_strSuffix = "";
		DeptSaleShow();
		ChangeTitle();
		ReFresh();
		return SUCCESS;
	}
	if (ret != SUCCEED) 
	{
		m_strSuffix = "";
	   return(ErrMsgBox(ret));
	}
	//添加实现内容调整的函数 
	DeptSaleShow();
	ChangeTitle();
	ReFresh();
	//切换到商品销售界面
	ChangeWin(PRODUCT_SALE_WIN);
	DBG_PRINT(("退出CProductSaleWin::DeptSaleProc函数"));
	return SUCCESS;
}

UINT8 CProductSaleWin::InvSumShowPrn(void)
{	

// 	g_codeNo = g_globalArg->m_curInvVol->m_code;//发票代码简码
// 	g_invNo = g_globalArg->m_curInvVol->m_isno;//发票号码 
// 	ChangeWin(VOL_SUM_INFO_WIN);//时间段汇总界面

	return SUCCESS;
}

UINT8 CProductSaleWin::ClientInputProc(void)
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
		ret = pSaleData->ClientNameInput(client.m_khmc.c_str(),client.m_khsh.c_str());		
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
	strcpy(title_arr[1], blank);
	strcpy(title_arr[2], blank);
	strcpy(title_arr[3], blank);
	ChangeTitle();
	return SUCCESS;
}

UINT8 CProductSaleWin::ClientCodeInputProc(void)
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
	strcpy(title_arr[1], blank);
	strcpy(title_arr[2], blank);
	strcpy(title_arr[3], blank);
	ChangeTitle();
	return SUCCESS;
}

UINT8 CProductSaleWin::RemarksInputProc(void)
{
	char *content;
	UINT8 ret;
	char *blank = "";
	INT8 chValue[128];
	memset((void*)chValue, 0, sizeof(chValue));
	string srrErr("");

	content = (char*)(m_pInput2->m_contentBuf);
//	if ('.' == *content) 
//	{
//		CClient client;
//		sprintf(chValue, "where ID = '%s'", (content+1));
//		client.m_filter = chValue;
//		client.Requery();
//		if(client.LoadOneRecord()!= SQLITE_OK)
//		{
//			CaMsgBox::ShowMsg("没有对应的付款人");
//			return FAILURE;
//		}
//		ret = pSaleData->ClientNameInput(client.m_khmc.c_str());		
//	}
//	else
//	{
		ret = pSaleData->RemarksInput(content,srrErr);
//	}
	
	if (ret != SUCCESS) 
	{
		CaMsgBox::ShowMsg(srrErr);
		return FAILURE;
	}
	m_pInput2->Clear();
//	ClientShow();
//	ChangeTitle();
	ReFresh();
	return SUCCESS;
}

UINT8 CProductSaleWin::GoodesReductProc(void)
{
	char *content;
	double reductMoney;
	UINT8 ret;

	if(m_pInput2->IsEmpty())
	{
		CaMsgBox::ShowMsg("未输入折让金额");
		return FAILURE;
	}
	content = (char*)(m_pInput2->m_contentBuf);
	reductMoney = atof(content);
	m_pInput2->Clear();
	ret = pSaleData->GoodsReduct(reductMoney);
	if (ret != SUCCESS) 
	{
	   return(ErrMsgBox(ret));
	}
	DeptSaleShow();
	ChangeTitle();
	ReFresh();
	return SUCCESS;	
}

UINT8 CProductSaleWin::PriceInputProc(void)
{	
	UINT8 ret;
	double ii;
	char *content;
	INT32 dotNum = 0;//小数位数
	
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
		CaMsgBox::ShowMsg("小数位数不得超过两位");
		return FAILURE;
	}
	m_pInput2->Clear();
	ret = pSaleData->PriceInput(ii);
	if (ret != SUCCESS) 
	{
	   return(ErrMsgBox(ret));
	}
	ReFresh();
	return SUCCESS;
}

UINT8 CProductSaleWin::SumInputProc(void)
{
	UINT8 ret;
	double ii;
	char *content;
	INT32 dotNum = 0;//小数位数
	
	content = (char*)(m_pInput2->m_contentBuf);
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
	return SUCCESS;
}

UINT8 CProductSaleWin::PlusProc(void)
{
	UINT8 ret;
	double ii;
	char *content;
	INT32 dotNum = 0;//小数位数
	
	content = (char*)(m_pInput2->m_contentBuf);
	ii = atof(content);
    DBG_PRINT(("ii= %lf",ii));

// 	if(ii > MAX_MONEY)
// 	{
// 		CaMsgBox::ShowMsg("数量超限");
// 		return FAILURE;
// 	}	
	m_pInput2->Clear();
	ret = pSaleData->Plus(ii);
	if (ret != SUCCESS) 
	{
	   return(ErrMsgBox(ret));
	}
	ReFresh();
	return SUCCESS;
}

UINT8 CProductSaleWin::QuickSaleProc(void)
{
	UINT8 ret;

	if(m_pInput2->IsEmpty())
	{
		return SUCCESS;
	}

	switch(g_globalArg->m_saleMode) 
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
	return ret;	
}