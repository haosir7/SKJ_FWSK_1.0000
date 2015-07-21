#ifndef __CDECLARE_H
#define __CDECLARE_H

#include "IncludeMe.h"

//#include "CTable.h"
//#include "CDB.h"


/**
 *@class CDeclare
 *@brief 数据抄报信息汇总表
 *@brief 
 */
class CDeclare
{
public:
    
	/*!
	@brief 构造函数	
	*/ 
	CDeclare();

    	/*!
	@brief 析构函数	
	*/
	~CDeclare();
		
public:
	string m_sq;			//属期
	string m_fpmx;			//发票明细数据
	string m_fpdxx;			//发票段信息
	string m_fphz;			//税控设备汇总数据
	string m_szfphz;		//安全芯片汇总数据
	string m_qtxx;			//其它信息
	
};

#endif
