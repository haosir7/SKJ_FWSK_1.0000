#include "CInvDet.h"

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"


//static CInvDet s_DetArray[DET_ARRAY_LEN];
CInvDet *s_DetArray[DET_ARRAY_LEN];

CInvDet::CInvDet()
{
	m_roll = 1;
	
	m_fpdm = "";
	m_fphm = 0;
	m_kprq = 0;
	m_kpsj = 0;
	m_kplx = 0;
	m_sky = "";
	m_sphxh = 0;
	m_spbm = "";
	m_spmc = "";
	m_spdj = 0.0;
	m_spsl = 0.0;
	m_spje = 0;
	m_sl = 0;
	m_zsl = 0;
	m_property =0;
	m_spdw = "件";
	m_ggxh = "";
	m_spse = 0;
	m_dj=0.0;
	m_je=0;
	m_dotNum = 0;
	pNext = NULL;
	m_useFlag = 0;
	
	m_backup = "";
	
	SET_TABLE_NAME("INV_DET");
	SET_FIELD_NUM(20);

	SET_FIELD_MEMBER(DB_UINT32, &m_no, "NO", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_fpdm, "FPDM", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_fphm, "FPHM", 0);
	SET_FIELD_MEMBER(DB_UINT8, &m_kplx, "KPLX", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_sky, "SKY", 0);

	SET_FIELD_MEMBER(DB_UINT32, &m_sphxh, "SPHXH", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_kprq, "KPRQ", 0);
	SET_FIELD_MEMBER(DB_UINT32, &m_kpsj, "KPSJ", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_spbm, "SPBM", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_spmc, "SPMC", 0);
	SET_FIELD_MEMBER(DB_DOUBLE, &m_spdj, "SPDJ", 0);
	SET_FIELD_MEMBER(DB_DOUBLE, &m_spsl, "SPSL", 0);
	SET_FIELD_MEMBER(DB_INT64, &m_spje, "SPJE", 0);
	SET_FIELD_MEMBER(DB_INT64, &m_spse, "SPSE", 0);
	SET_FIELD_MEMBER(DB_FLOAT, &m_sl, "SL", 0);
	SET_FIELD_MEMBER(DB_DOUBLE, &m_dj, "DJ", 0);
	SET_FIELD_MEMBER(DB_INT64, &m_je, "JE", 0);
	SET_FIELD_MEMBER(DB_UINT8, &m_property, "PROP", 0);
	SET_FIELD_MEMBER(DB_TEXT, &m_spdw, "SP_DW", 0);
    SET_FIELD_MEMBER(DB_TEXT, &m_backup, "BACKUP", 0);

}

CInvDet::~CInvDet()
{
}


INT32 CInvDet::Roll()
{
	DBG_ENTER("CInvHead::Roll");
	
	//删除记录
	UINT8 ret = Delete();
	
	DBG_RETURN(ret); //返回sqlite错误号	
}
/*

UINT32 CInvDet::GetMoneySum(UINT32 *Money, UINT8 *taxID, UINT8 taxIDNum)
{
	DBG_ENTER("CInvDet::GetMoneySum");
	UINT32 nNum = 0;
    string strSql = "select sum(SPJE), TAX_ID from INV_DET ";
    
	//组装sql语句
    strSql.append(m_filter);
	strSql.append(" group by TAX_ID");
	DBG_PRINT(("strSql = %s ",strSql.c_str()));
	SetSQL(strSql);
    
	//组装传递的参数
	INT64 value[6][2] = {0,0,0,0,0,0,0,0,0,0,0,0};
	INT64 *p[12];
	UINT8 i, j;
	DB_TYPE  nType[2] = {DB_INT64, DB_UINT8};
	for(i=0; i<6; i++)
	{
	 for(j = 0; j<2; j++)
	 {
		 p[i*2 +j] = &(value[i][j]);
	 }
	}
   
	//查询统计结果
//	nNum = GetMultiResult(nType, (void **)p, 2);
	INT32 errCode = GetMultiResult(nType, (void **)p, 2, nNum);
	if (SQLITE_OK != errCode)
	{
		DBG_PRINT(("GetMultiResult err: errCode = %d ", errCode));
		DBG_RETURN(errCode);
	}
	DBG_PRINT(("nNum = %d ", nNum));
    
	//清零
	for(j=0; j<taxIDNum; j++)
	{
		Money[j] = 0; 
	}

    //金额赋值到数组中，位置与税种税目对应
	for(i = 0; i<nNum; i++)
	{
		for(j=0; j<taxIDNum; j++)
		{
			if(value[i][1]==taxID[j])
			{
				Money[j] = (UINT32)(value[i][0]);	
				break;
			}
		}	 			
	 DBG_PRINT(("Money[%u] = %u ", j, Money[j]));
	}

//	DBG_RETURN(nNum);
	DBG_RETURN(errCode);
}
*/
INT64 CInvDet::PartMoneySum()
{
	DBG_ENTER("CInvDet::PartMoneySum");

	string strSql = "select sum(SPJE) from INV_DET ";//计算商品金额的聚合，filter中应该有分组group by语句
	strSql += m_filter;

	DBG_PRINT(("strSql = %s ",strSql.c_str()));
	SetSQL(strSql);

	INT64 nSum = 0;
	GetOneResult(DB_INT64, (void*)&nSum);

	DBG_RETURN(nSum);
}

CInvDet* CInvDet::GetNewInvDet(void)
{
	DBG_ENTER("CInvDet::GetNewInvDet");

//	UINT8 index=0;
//	for(; index<DET_ARRAY_LEN; index++)
//	{
//		if (0 == s_DetArray[index].m_useFlag) 
//		{
//			s_DetArray[index].m_useFlag = 1;
//			return &(s_DetArray[index]);
//		}
//	}
//
//	DBG_PRINT(("s_DetArray数组元素全部被占用 index = %u ",index));

	UINT8 index=0;
	for(; index<DET_ARRAY_LEN; index++)
	{
		if (0 == s_DetArray[index]->m_useFlag) 
		{
			s_DetArray[index]->m_useFlag = 1;
			return s_DetArray[index];
		}
	}

	DBG_PRINT(("s_DetArray数组元素全部被占用 index = %u ",index));
	return NULL;//s_DetArray数组元素全部被占用
}

UINT8 CInvDet::FreeInvDet()
{
//	DBG_ENTER("CInvDet::FreeInvDet");
	DBG_PRINT(("#########CInvDet::FreeInvDet"));

//	m_roll = 1;
	m_dotNum = 0;
	pNext = NULL;
	m_useFlag = 0; 

	return SUCCESS;
}
