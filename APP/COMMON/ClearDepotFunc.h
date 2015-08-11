/*! \file    ClearDepotFunc.h
   \brief    转换器清库函数
   \author   zfj
   \version  1.0
   \date     2015-7-25
 */


#ifndef CLEAR_DEPOT_FUNC_H
#define CLEAR_DEPOT_FUNC_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string>

#include "comdatatype.h"

using namespace std;

/**
 *@class CClearDepotFunc 
 *@brief 清库函数
 */
class CClearDepotFunc
{

public:

	/*!
	@brief 构造函数	
	*/
	CClearDepotFunc();

	/*!
	@brief 析构函数	
	*/
	~CClearDepotFunc();
    
	/*!
	@brief 具体清库操作
	@param[in] strInfo   显示的消息
	@return  1: SUCCESS; 0: FAILURE
	*/
	UINT8 ClearDepot(string &strErr);

	 
};

#endif
