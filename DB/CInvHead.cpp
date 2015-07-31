#include "CInvHead.h"

#include "VersionConfig.h"
#include "IncludeMe.h"
#include "beep.h"
#include "commonFunc.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

//#define PART_DEL
#ifdef PART_DEL
   #define DEL_NUM 200
#else
   #define DEL_NUM INV_HEAD_DEL_MAX
#endif

#define TRY_TIMES 3

CInvHead::CInvHead()
{
	m_roll = 1;
	
	m_fpdm = "";
	m_fphm = 0;
	m_kprq = 0;
	m_kpsj = 0;
	m_kplx = 0;
	m_kphjje = 0;
	m_kpse = 0;				/**< 税额*/
	m_yfpdm = "";
	m_yfphm = 0;
	m_fkdw = "";
	m_fkdwsh = PAYER_NAME_DEF;
	m_sky = "";
	m_sphsl = 0;
	m_fwm = "";
	m_fplb = 0;
	m_scbz = 0;
	m_bzkz = "";
	m_kpje = 0;
	m_fpsyh = "";
	m_casign = "";			/**< CA签名*/
	m_backup1 = "";
	m_backup2 = "";
	m_zskl = "";
	m_sl = 0;
	m_dycs = 0;
	m_skr = "";
	m_zfbz = 0;
	m_zfrq = 0;
	m_zfsj = 0;
	m_hczt = "";
	m_hyfl = "";
	pHead = NULL;
	pEnd = NULL;
	m_pInvDet = &m_InvDet;
	
	SET_TABLE_NAME("INV_HEAD");
	SET_FIELD_NUM(18);
	
	SET_FIELD_MEMBER(DB_UINT32, &m_no, "NO", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_fpdm, "FPDM", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_fphm, "FPHM", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_kprq, "KPRQ", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_kpsj, "KPSJ", 0);
	SET_FIELD_MEMBER(DB_UINT8, &m_kplx, "KPLX", 0);
	SET_FIELD_MEMBER(DB_INT64, &m_kphjje, "KPHJJE", 0);
	SET_FIELD_MEMBER(DB_INT64, &m_kpse, "KPSE", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_yfpdm, "YFPDM", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_yfphm, "YFPHM", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_fkdw, "FKDW", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_fkdwsh, "FKDWSH", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_sky, "SKY", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_sphsl, "SPHSL", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_fwm, "FWM", 0);
	SET_FIELD_MEMBER(DB_UINT8, &m_fplb, "FPLB", 0);
	SET_FIELD_MEMBER(DB_UINT8, &m_scbz, "SCBZ", 0);
    SET_FIELD_MEMBER(DB_TEXT, &m_backup1, "BACKUP1", 0);

}

CInvHead::~CInvHead()
{
	m_pInvDet = NULL;
	
	//释放链表空间
    DelList(); 
}

INT32 CInvHead::InsertNode(CInvDet* pNew)
{
	DBG_ENTER("CInvHead::InsertNode");
	DBG_PRINT(("进入InsertNode函数"));

	//若为空链表
	if (pHead == NULL) 
	{
		pHead = pNew;
		pNew->pNext = NULL;
		pEnd = pNew;
		DBG_PRINT(("第一个Node"));
		DBG_PRINT(("退出InsertNode函数"));
		DBG_RETURN(SUCCESS);
	}

    //若为非空链表
	pEnd->pNext = pNew;
	pNew->pNext = NULL;
	pEnd = pNew;

	DBG_PRINT(("退出InsertNode函数"));
	DBG_RETURN(SUCCESS);
}

INT32 CInvHead::ShowList()
{
	DBG_ENTER("CInvHead::ShowList");
	CInvDet *p = pHead;

    //若为空链表
	if (pHead == NULL)
	{
		DBG_ASSERT_WARNING(false,("The list is empty!"));
		DBG_RETURN(FAILURE);
	}

    //若为非空链表
	while (p)
	{
		DBG_PRINT(("m_spbm = %s,", p->m_spbm.c_str() ));
		p = p->pNext;
	}
	DBG_RETURN(SUCCESS);	
}

INT32 CInvHead::DelNode(string &pCode)
{
 	DBG_ENTER("CInvHead::DelNode");
 	//若为空链表
 	if (pHead == NULL)
 	{
 		DBG_ASSERT_WARNING(false,("The list is empty!"));
 		DBG_RETURN(0);	
 	}
 	
    CInvDet *p, *s;
 	p = pHead;
 	s = pHead;
 
 	while ((p != NULL)&&((pCode.compare(p->m_spbm))!=0))
 	{
 		s = p;
 		p = p->pNext;
 	}
 	
 	if ((pCode.compare(p->m_spbm))==0) //找到该节点
 	{
 		if (p == pHead)//若是头节点
 		{
 			pHead = p->pNext;
 		}
 		else //若非头节点
 		{
 			s->pNext = p->pNext;
 		}
 
 		if (pEnd == p) //若是尾节点
 		{
 			pEnd = s;
 		}
 //		delete p; //释放该节点所占空间
 		p->FreeInvDet();//释放该节点所占空间
 		p = NULL;
 	}	
     else //未找到该节点
 	{
 		DBG_ASSERT_WARNING(false,("Can not find the node!"));
 		DBG_RETURN(FAILURE);	
 	}
     
 	DBG_RETURN(SUCCESS);
}

//xsr 这个函数是被注掉的，但我不知道为何当时注掉它了。现在把它打开。
//xsr 现在我又注掉了它。
// INT32 CInvHead::DelNode(UINT8 dpNo)
// {
// 	DBG_ENTER("CInvHead::DelNode");
// 	//若为空链表
// 	if (pHead == NULL)
// 	{
// 		DBG_ASSERT_WARNING(false,("The list is empty!"));
// 		DBG_RETURN(0);	
// 	}
// 	
//     CInvDet *p, *s;
// 	p = pHead;
// 	s = pHead;
// 
// 	while ((p != NULL)&&(p->m_dpNo != dpNo))
// 	{
// 		s = p;
// 		p = p->pNext;
// 	}
// 	
// 	if (p->m_dpNo == dpNo) //找到该节点
// 	{
// 		if (p == pHead)//若是头节点
// 		{
// 			pHead = p->pNext;		    
// 		}
// 		else //若非头节点
// 		{
// 			s->pNext = p->pNext;
// 		}
// 
// 		if (pEnd == p) //若是尾节点
// 		{
// 			pEnd = s;
// 		}
// //		delete p; //释放该节点所占空间
// 		p->FreeInvDet();//释放该节点所占空间
// 		p = NULL;
// 	}	
//     else //未找到该节点
// 	{
// 		DBG_ASSERT_WARNING(false,("Can not find the node!"));
// 		DBG_RETURN(FAILURE);	
// 	}
//     
// 	DBG_RETURN(SUCCESS);
// }

INT32 CInvHead::DelLastNode()
{
	DBG_ENTER("CInvHead::DelLastNode");
	//若为空链表
	if (pHead == NULL)
	{
		DBG_ASSERT_WARNING(false,("The list is empty!"));
		DBG_RETURN(FAILURE);	
	}

	if (pEnd == NULL)
	{
		DBG_ASSERT_WARNING(false,("pHead != NULL, pEnd == NULL!"));
		DBG_RETURN(FAILURE);	
	}
	
    CInvDet *p, *s;
	p = pHead;
	//若链表中只有一个节点
	if (p == pEnd)
	{
		pHead =NULL;
		pEnd = NULL;
//		delete p; //释放该节点所占空间
		p->FreeInvDet();//释放该节点所占空间
	    DBG_RETURN(SUCCESS);
	}
    
	//若链表中有多个节点
	while (p != pEnd)
	{
		s = p;
		p = p->pNext;
	}
	pEnd = s;
	s->pNext = NULL;
//	delete p; //释放该节点所占空间
	p->FreeInvDet();//释放该节点所占空间
	DBG_RETURN(SUCCESS);
}

//-------------------------------------------------
//删除链表，释放链表空间
//-------------------------------------------------
INT32 CInvHead::DelList()
{
	DBG_PRINT(("进入DelList函数"));
	DBG_ENTER("CInvHead::DelList");
	CInvDet *p = pHead;
	CInvDet *s;

	while (p)
	{
		DBG_PRINT(("delete CInvDet: DP_NO = %s!", p->m_spbm.c_str()));
		s = p;
		p = p->pNext;
		//delete s;
		s->FreeInvDet();//释放该节点所占空间
	}
	pHead = NULL;
	pEnd = NULL;

	DBG_PRINT(("退出DelList函数"));
    DBG_RETURN(SUCCESS);
}

CInvDet* CInvHead::SearchNode(string &pCode)
{
 	DBG_ENTER("CInvHead::SearchNode");
 	//若为空链表
 	if (pHead == NULL)
 	{
 		DBG_ASSERT_WARNING(false,("The list is empty!"));
 		DBG_RETURN(0);	
 	}
 
     CInvDet *p;
 	p = pHead;
     DBG_PRINT((" begin to search"));
 	while ((p != NULL)&&((pCode.compare(p->m_spbm))!=0))
 	{
 		p = p->pNext;
 		DBG_PRINT((" p = p->pNext"));
 	}
 	if (NULL == p ) 
 	{
 		DBG_PRINT((" not find this node"));
 		DBG_RETURN(NULL);	
 	}
 	
 	if ((pCode.compare(p->m_spbm))==0) //找到该节点
 	{
 		DBG_PRINT((" found this node"));
 		DBG_RETURN(p);
 	}	
 //    else //未找到该节点
 //	{
 //		DBG_PRINT((" not find this node"));
 //		DBG_RETURN(NULL);	
 //	}
}

// CInvDet* CInvHead::SearchNode(UINT8 dpNo)
// {
// 	DBG_ENTER("CInvHead::SearchNode");
// 	
// 	//若为空链表
// 	if (pHead == NULL)
// 	{
// 		DBG_ASSERT_WARNING(false,("The list is empty!"));
// 		DBG_RETURN(0);	
// 	}
// 
//     CInvDet *p;
// 	p = pHead;
// 
// 	while ((p != NULL)&&(p->m_dpNo != dpNo))
// 	{
// 		p = p->pNext;
// 		DBG_PRINT((" next"));
// 	}
// 
// 	if (NULL == p ) 
// 	{
// 		DBG_PRINT((" not find this node"));
// 		DBG_RETURN(NULL);	
// 	}
// 	
// 	if (p->m_dpNo == dpNo) //找到该节点
// 	{
// 		DBG_PRINT((" found this node"));
// 		DBG_RETURN(p);
// 	}	
// //    else //未找到该节点
// //	{
// //		DBG_PRINT((" not find this node"));
// //		DBG_RETURN(NULL);	
// //	}
// }

INT32 CInvHead::Load()
{
 	DBG_ENTER("CInvHead::Load");
	DBG_PRINT(("进入CInvHead::Load函数"));
 	
 	//释放链表所占空间
 	DelList();
     
 	//查询INV_HEAD表
 	CTable::Requery();
 	INT32 returnVal = LoadOneRecord();
 	
 	if (returnVal == SQLITE_OK)
 	{    
        DBG_PRINT(("NO, %d", m_no));
 		DBG_PRINT(("m_fpdm, %s", m_fpdm.c_str()));
 	    DBG_PRINT(("m_fphm, %d", m_fphm));
 	    DBG_PRINT(("m_kprq, %lu", m_kprq));
 	    DBG_PRINT(("m_kpsj, %lu", m_kpsj));
 	    DBG_PRINT(("m_kplx, %d", m_kplx));
 	   
 	    DBG_PRINT(("m_kphjje, %d", m_kphjje));
 	    DBG_PRINT(("m_yfphm, %d", m_yfphm));
 	    
 	    DBG_PRINT(("m_fkdw, %s", m_fkdw.c_str()));
		DBG_PRINT(("m_fkdwsh, %s", m_fkdwsh.c_str()));
 	    DBG_PRINT(("m_sky, %s", m_sky.c_str()));
 //		DBG_PRINT(("m_skyid, %u", m_skyid));
 	    
// 	    DBG_PRINT(("m_ptype, %d", m_ptype));
 	    //DBG_NPRINT_HEX(m_fCode, 8);	    
 	    DBG_PRINT(("m_sphsl, %d", m_sphsl));
 	    DBG_PRINT(("BACKUP1, %s", m_backup1.c_str()));
// 		DBG_PRINT(("BACKUP2, %s", m_backup2.c_str()));
//		DBG_PRINT(("BACKUP3, %s", m_backup3.c_str()));
 	    
 	}
 	else if (returnVal == SQLITE_DONE)
 	{
         DBG_PRINT((" CInvHead has no matched record! errorcode: %d", returnVal));
 		return returnVal;
 	}
 	else
 	{
 		DBG_PRINT(("errorcode: %d", returnVal));
 		return returnVal;
 	}
 
 	//查询INV_DET表
 	INT8 chFilter[256];
 	memset((void *)chFilter, 0x00, sizeof(chFilter));
 	sprintf(chFilter, "where FPDM = '%s' and FPHM = %u", m_fpdm.c_str(), m_fphm);
 	m_pInvDet->m_filter = chFilter;
 	m_pInvDet->Requery();
 
 	INT32 errorcode = m_pInvDet->MoveFirst();	
 	if (errorcode != SQLITE_ROW) 
 	{
 		if ((errorcode == SQLITE_DONE)&&(m_sphsl==0)) //没有明细
 		{
			m_pInvDet->MoveEnd();
 			return SQLITE_OK;
 		}
 		else
 		{
			m_pInvDet->MoveEnd();
 			return errorcode; //可能返回SQLITE_DONE,或其它错误码
 		}	
 	}
 
 	CInvDet *p;
 	while (errorcode == SQLITE_ROW)
 	{
 //		p = new CInvDet;
 		p = p->GetNewInvDet();//从静态数组获取一个可用的CInvDet对象
 		
 		p->m_no = m_pInvDet->m_no;
 		p->m_fpdm = m_pInvDet->m_fpdm;
 		p->m_fphm = m_pInvDet->m_fphm;
		p->m_kplx = m_pInvDet->m_kplx;
		
 		p->m_sphxh = m_pInvDet->m_sphxh;		
 		p->m_kprq = m_pInvDet->m_kprq;
 		p->m_kpsj = m_pInvDet->m_kpsj;
 		p->m_spbm = m_pInvDet->m_spbm;
 		p->m_spmc = m_pInvDet->m_spmc;
 		p->m_spdj = m_pInvDet->m_spdj;
 		p->m_spsl = m_pInvDet->m_spsl;
 		p->m_spje = m_pInvDet->m_spje;
        p->m_spse = m_pInvDet->m_spse;
  		p->m_sl = m_pInvDet->m_sl;
		p->m_dj = m_pInvDet->m_dj;
		p->m_je = m_pInvDet->m_je;
		p->m_property = m_pInvDet->m_property;
		p->m_spdw = m_pInvDet->m_spdw;
		 	
 		InsertNode(p);	//插入节点	
 	    DBG_PRINT(("============================"));
 		DBG_PRINT(("NO, %d", m_pInvDet->m_no));
 		DBG_PRINT(("m_fpdm, %s", m_pInvDet->m_fpdm.c_str()));
 	    DBG_PRINT(("m_fphm, %d", m_pInvDet->m_fphm));
 	    DBG_PRINT(("m_sphxh, %d", m_pInvDet->m_sphxh));
 	    DBG_PRINT(("m_kprq, %lu", m_pInvDet->m_kprq));
 	    DBG_PRINT(("m_kpsj, %lu", m_pInvDet->m_kpsj));
 	   
 	    DBG_PRINT(("m_spbm, %s", m_pInvDet->m_spbm.c_str()));
 	    DBG_PRINT(("m_spmc, %s", m_pInvDet->m_spmc.c_str()));
 	    
 	    DBG_PRINT(("m_spdj, %f", m_pInvDet->m_spdj));
 	    DBG_PRINT(("m_spsl, %f", m_pInvDet->m_spsl));
 	    DBG_PRINT(("m_spje, %lld", m_pInvDet->m_spje));
		DBG_PRINT(("m_spse, %lld", m_pInvDet->m_spse));
		DBG_PRINT(("m_spdw, %s", m_pInvDet->m_spdw.c_str()));
 	    
 	//	DBG_PRINT(("PRICE, %u", m_pInvDet->m_dpNo)); 	    	     
 		errorcode = m_pInvDet->MoveNext();
 	}
 
m_pInvDet->MoveEnd();
//m_db->MutexUnlock();

	
 	if (errorcode==SQLITE_DONE) 
 	{
 		DBG_PRINT(("退出CInvHead::Load函数"));
		return SQLITE_OK;
 	}

	DBG_PRINT(("退出CInvHead::Load函数"));
 	return errorcode;
}

//-----------------------------------------------------
//存储内容到数据库（INV_HEAD和INV_DET表）
//-----------------------------------------------------
INT32 CInvHead::Save()
{
	DBG_PRINT(("进入Save函数"));
	DBG_ENTER("CInvHead::Save");
	INT32 errcode;
	
	//添加INV_HEAD记录
	errcode = AddNew();
	if (errcode != SQLITE_OK)
	{
		DBG_PRINT(("INV_HEAD AddNew() wrong"));
		DBG_RETURN(FAILURE);
	}
	//添加INV_DET记录
    CInvDet *p = pHead;
	if (p==NULL)
	{
		DBG_PRINT(("INV_DET here"));
		DBG_ASSERT_WARNING(false,("List pointer: pHead = NULL!"));
	}
	while (p)
	{
		DBG_PRINT(("INV_DET here"));
		DBG_PRINT(("p->m_fphm = %u", p->m_fphm));
		DBG_PRINT(("p->m_spdj = %f", p->m_spdj));
		errcode = p->AddNew();
		if (errcode != SQLITE_OK)
		{
			DBG_PRINT(("INV_DET AddNew() wrong"));
			DBG_RETURN(FAILURE);
		}
		p = p->pNext;
		DBG_PRINT(("INV_DET AddNew()"));
	}	
	DBG_PRINT(("退出Save函数"));
	DBG_RETURN(SUCCESS);
}

INT32 CInvHead::CheckRoll(INT32 nCount, UINT32 strRefDate, UINT32 &nDelDate)
{
 	DBG_ENTER("CInvHead::Roll");
 	
 	INT32 nNoStart, nNoEnd;
 	char value[128];
 	UINT32 strEndDate;
 	nDelDate = 0;
 
 	if (nCount>=INV_HEAD_MAX) //超过容纳最大记录数
 	{
 		//得到第一条记录的序号
 		Requery();
 		if (LoadOneRecord() == SQLITE_OK)
 		{
 			nNoStart = m_no;
 		}
 		else
 		{
 			DBG_ASSERT_WARNING(false,("Can not load the first record!"));
 			DBG_RETURN(FAILURE);
 		}

         //查询截止条数处的IE_DATE字段
 		nNoEnd = nNoStart + DEL_NUM - 1;
 		sprintf(value, "WHERE NO = %d", nNoEnd);		
 		m_filter.append(value);
        Requery();
 		if (LoadOneRecord() == SQLITE_OK)
 		{
 			strEndDate = m_kprq;
 		}
 		else
 		{
 			DBG_ASSERT_WARNING(false,("Can not load the first record!"));
 			DBG_RETURN(FAILURE);
 		}
         
 		DBG_PRINT(("strRefDate = %u and  strEndDate = %u",strRefDate, strEndDate));
 //		//比较日期字段，确定删除条件	    
 //		if(strRefDate <= strEndDate)
 //		{
 //			nDelDate = strRefDate;
 //		}
 //		else
 //		{
 		nDelDate = strEndDate;  
 //		}
 
 		DBG_RETURN(SUCCESS);
 	}
 
 	DBG_RETURN(SUCCESS);
}


INT32 CInvHead::Roll(UINT32 nDelDate, UINT32 strRefDate)
{
	
#ifdef PART_DEL
#endif

#ifndef PART_DEL

	//整段删除记录
	return (DeleteRecord(nDelDate));
		
#endif
	
}


INT8 CInvHead::DeleteRecord(UINT32 nDelDate)
{
	INT8 value[128];
	UINT8 ret;
	UINT8 nIndex = 0;
	if (nDelDate>0) //要滚动
	{
		sprintf(value,"WHERE KPRQ <= %lu", nDelDate);
		m_filter = value;
		m_pInvDet->m_filter = m_filter;
		ret = Delete(); 
//		FOR_DELAY(0xFFFFFF);//delay for some time
//		FOR_DELAY(0xFFFFFF);//delay for some time
		if (SQLITE_OK != ret)
		{

			Beep_On();
			nIndex = 0;
			do 
			{
				nIndex++;
				m_filter = value;
				ret = Delete(); 
//				FOR_DELAY(0xFFFFFF);//delay for some time
//				FOR_DELAY(0xFFFFFF);//delay for some time
			} while((SQLITE_OK != ret)&&(nIndex<TRY_TIMES));
		}
		if(SQLITE_IOERR==ret)
		{
			return FAILURE;// 滚动失败（disk IO error），返回
		}

        //删除INV_DET的记录
        ret = m_pInvDet->Roll();
//		FOR_DELAY(0xFFFFFF);
//		FOR_DELAY(0xFFFFFF);//delay for some time
		if (SQLITE_OK != ret)
		{

			Beep_On();

			nIndex = 0;
			do 
			{
				nIndex++;
				m_pInvDet->m_filter = value;
				ret = m_pInvDet->Roll(); 
//				FOR_DELAY(0xFFFFFF);//delay for some time
//				FOR_DELAY(0xFFFFFF);//delay for some time
			} while((SQLITE_OK != ret)&&(nIndex<TRY_TIMES));
		}
		if(SQLITE_IOERR==ret)
		{
			return FAILURE;// 滚动失败（disk IO error），返回
		}
	}
	DBG_RETURN(SUCCESS);
}



INT32 CInvHead::GetInvoiceSum(UINT32 &norInv, UINT32 &recInv, UINT32 &canInv)
{
	DBG_ENTER("CInvHead::GetInvoiceSum");

	INT32 nNum;
	string strSql = "select count(*), KPLX from INV_HEAD ";

	strSql.append(m_filter);
    strSql.append(" group by KPLX");
	DBG_PRINT(("strSql = %s ",strSql.c_str()));
	SetSQL(strSql);

	UINT32 value[7][2] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	UINT32 *p[14];
	UINT8 i, j;
	DB_TYPE  nType[2] = {DB_UINT32, DB_UINT8};
	for(i=0; i<7; i++)
	{
	 for(j = 0; j<2; j++)
	 {
		 p[i*2 +j] = &(value[i][j]);
	 }
	}
   
//	nNum = GetMultiResult(nType, (void **)p, 2);
	INT32 errCode = GetMultiResult(nType, (void **)p, 2, nNum);
	if (SQLITE_OK != errCode)
	{
		DBG_PRINT(("GetMultiResult err: errCode = %d ", errCode));
		DBG_RETURN(errCode);
	}
	DBG_PRINT(("nNum = %d ", nNum));
    
	norInv = 0;
	recInv = 0;
	canInv = 0;
	for(i = 0; i<nNum; i++)
	{
	 switch(value[i][1]) 
	 {
		 case NORMAL_INV:
			  norInv = value[i][0];
			  break;
		 case RETURN_INV:
		 case RET_MANUAL_INV:
		 case RET_SPECIAL_INV:
			  recInv += value[i][0];
			  break;
		 case WASTE_INV:
		 case WASTE_NOR:
		 case WASTE_RET:
			  canInv += value[i][0];
			  break;
		 default:;
	 }
	 DBG_PRINT(("value[%u][0] = %u ", i, value[i][0]));
	}
	
//	DBG_RETURN(nNum);
	DBG_RETURN(errCode);
}


INT32 CInvHead::GetInvHeadNum()
{
	INT32 nNoStart = 0;
	INT32 nNoEnd = 0;
	INT32 nCount = 0;
	INT32 errcode;
	string filter, strlimit;

	filter = m_filter;
	strlimit = " limit 1";
	m_filter += strlimit;
	Requery();
	errcode = LoadOneRecord();

	if (errcode == SQLITE_OK)
	{
		nNoStart = m_no; //起始记录的序号
	}
	else if (errcode == SQLITE_DONE) 
	{
		nCount = 0;
		DBG_RETURN(nCount);	//空表，无记录		
	}
	else 
	{
		DBG_PRINT(("Can not load the first record!"));
		DBG_RETURN(-1);
	}

	strlimit = " order by NO desc limit 1";
	m_filter = filter;
	m_filter += strlimit;

	Requery();
	if (LoadOneRecord() == SQLITE_OK)
	{
		nNoEnd = m_no; //末尾记录的序号
	}
	else
	{
		DBG_PRINT(("Can not load the last record!"));
		DBG_RETURN(-1);
	}

	nCount = nNoEnd - nNoStart + 1; //INV_HEAD表的记录数
	if (nCount<0) 
	{
		DBG_PRINT(("INV_HEAD的nCount<0!"));
		DBG_RETURN(-1);
	}

	DBG_RETURN(nCount);


}

UINT32 CInvHead::GetInvNumByDate()
{
	UINT32 nNum;
	string strSql = "select count(*) from INV_HEAD ";
	strSql.append(m_filter);

	DBG_PRINT(("strSql = %s", strSql.c_str()));
	SetSQL(strSql);
	GetOneResult(DB_UINT32, (void*)&nNum); 

	return nNum;
}
