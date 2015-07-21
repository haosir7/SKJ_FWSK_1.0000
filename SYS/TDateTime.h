/*! \file    TDateTime.h
   \brief    日期时间接口封装
   \author   zyk
   \date     2005-04-04
 */

#ifndef _POS_DATETIME_ZYK_H_
#define _POS_DATETIME_ZYK_H_
#include <string>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#include <errno.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>

#ifndef WIN32
#include <sys/ioctl.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>

#include <sys/types.h>
#include <sys/timeb.h>
#include <unistd.h>
#endif

using namespace std;

/** @brief 枚举类型，定义日期时间格式 */
enum TDateTimeFormat {  
			YYYYMMDDHHMMSS,		/**< 年(4位)月日时分秒 */
			YYMMDDHHMMSS,		/**< 年月日时分秒 */
			YYYYMMDD,			/**< 年(4位)月日 */
			YYMMDD,				/**< 年月日 */
			HHMMSS,				/**< 时分秒 */
			YYYYMMDDHHMM		/**< 年(4位)月日时分 */
};

/** @brief 枚举类型，日期时间格式标志  */
enum TDateTimeFlag {DATE_POS, TIME_POS, DATETIME_POS};


/*!
@class TDateTime
@brief 日期时间类
*/
class TDateTime
{
public:
	
	/*!
	@brief 构造函数，初始一个无效时间。
	*/
	TDateTime();

	/*!
	@brief 构造函数，拷贝给定TDateTime的时间。
	*/
	TDateTime(const TDateTime& src);

	/*!
	@brief 构造函数，初始化为给定的字符串时间。
	*/
	TDateTime(const char * src, TDateTimeFlag flag = DATE_POS);

	/*!
	@brief 构造函数，初始为给定的年月日数值。
	*/
	TDateTime(unsigned int   year, unsigned int   month, unsigned int   day);

	/*!
	@brief 构造函数，初始为给定的时分秒数值。
	*/
	TDateTime(unsigned int   hour, unsigned int   min, unsigned int   sec,unsigned int   timeflag);

	/*!
	@brief 构造函数，初始为给定的年月日时分秒数值。
	*/
	TDateTime(unsigned int   year, unsigned int   month, unsigned int   day,
		  unsigned int   hour, unsigned int   min, unsigned int   sec);
	
	/*!
	@brief 构造函数，初始为给定的日期与时间两个整型数值。
	*/
    TDateTime(unsigned int date, unsigned int time);


	/*!
	@brief 取当前日期时间	
	@return  当前日期时间（TDateTime类型）
	*/
	static TDateTime CurrentDateTime();

	/*!
	@brief 取当前日期	
	@return  当前日期（TDateTime类型）
	*/
	static TDateTime CurrentDate();

	/*!
	@brief 将给定日期时间设置为系统当前时间	
	@param[in] rhs 当前日期时间（TDateTime类型）
	@return  true-成功；false-失败
	*/
	static bool SetCurrentDateTime(TDateTime& rhs);
	
	/*!
	@brief 将给定日期设置为系统当前日期	（时间默认为0:0:0）
	@param[in] rhs 当前日期（TDateTime类型）
	@return  true-成功；false-失败
	*/
	static bool SetCurrentDate(TDateTime& rhs);



	/*!
	@brief 将自身日期时间转化为string形式输出。
	@param[in] format 输出格式的类型，使用TDateTimeFormat枚举值给定。
	@param[in] formatstr 指定特殊的输出格式。
	@note 例如：
	  formatstr="%04u-%02u-%02u %02u:%02u:%02u",format=YYYYMMDDHHMMSS；
	  formatstr="%u年%u月%u日", format=YYYYMMDD；
	  formatstr="%02u:%02u:%02u",format=HHMMSS；
	*/
	string FormatString(TDateTimeFormat format=YYYYMMDD,
			    const char *formatstr=NULL);

	/*!
	@brief 将自身日期时间转化为UINT32类型输出。
	@param[in] format 输出格式的类型，使用TDateTimeFormat枚举值给定。
	@param[in] formatstr 此参数无用
	@warning 参数format只限于YYYYMMDD和HHMMSS两种类型
	*/
    unsigned int FormatInt(TDateTimeFormat format=YYYYMMDD,
			    const char *formatstr=NULL);


	/*!
	@brief 判断日期是否合法
	@return 0-不合法; 
			1-合法
	*/
	static int DateIsValid(unsigned int   year, unsigned int   month, unsigned int   day);

	/*!
	@brief 判断时间是否合法
	@return 0-不合法
			1-合法
	*/
	static int TimeIsValid(unsigned int   hour, unsigned int   min, unsigned int   sec);

	/*!
	@brief 判断是否是平年 
	@return 
	  0-不是
	  1-是
	*/
	static int IsLeapYear(unsigned int   year);

	static double ComposeDate(unsigned int   year, unsigned int   month, unsigned int   day);

	static double ComposeTime(unsigned int   hour, unsigned int   min, unsigned int   sec);	 

	static int DecodeDateString(const char *src,
				    unsigned  int &year,
				    unsigned  int &month,
				    unsigned  int &day);

	static int DecodeTimeString(const char *src,
				    unsigned  int &hour,
				    unsigned  int &min,
				    unsigned  int &sec);

	static int DecodeDateTimeString(const char *src,
					unsigned  int &year,
					unsigned  int &month,
					unsigned  int &day,
					unsigned  int &hour,
					unsigned  int &min,
					unsigned  int &sec);

	int DateIsValid(void);

	int TimeIsValid(void);

	int DateTimeIsValid(void);

	int IsLeapYear();

	TDateTime MonthBegin(const TDateTime& rhs);

	TDateTime MonthEnd(const TDateTime& rhs);

	TDateTime MonthAdd(const TDateTime& rhs, int Months);

	TDateTime HourAdd(const TDateTime& rhs, int addHours);


	int DayOfWeek()const;

	unsigned int DayOfMonth(void)const;

	unsigned int Year(void)const;

	unsigned int Month(void)const;

	unsigned int Hour(void)const;

	unsigned int Minute(void)const;

	unsigned int Second(void)const;

	TDateTime& operator =(const TDateTime& rhs)
		{
			Val = rhs.Val;
			return *this;
		};

 	TDateTime& operator +=(const int days)
 		{
 			Val += days;
 			return *this;
 		};

	TDateTime& operator -=(const int days)
		{
			Val -= days;
			return *this;
		};
	
	TDateTime& operator ++()
		{
			Val++;
			return *this;
		};
	
	TDateTime operator ++(int)
		{
			TDateTime tmp(*this);
			Val++;
			return tmp;
		};
	
	TDateTime& operator --()
		{
			Val--;
			return *this;
		};

	TDateTime operator --(int)
		{
			TDateTime tmp(*this);
			Val--;
			return tmp;
		};
	

	TDateTime operator +(const int days)const
		{
			return Val + days;
		};
	
	int  operator -(const TDateTime& rhs)const
		{
			return int(Val - rhs.Val);
		};
	

	bool operator ==(const TDateTime& rhs)const ;
        /*
	  {
	  return Val == rhs.Val;
	  };
        */
	
/*
  bool operator !=(const TDateTime& rhs)const
  {
  return Val != rhs.Val;
  };
  bool operator >(const TDateTime& rhs)const
  {
  return Val > rhs.Val;
  };
  bool operator <(const TDateTime& rhs)const
  {
  return Val < rhs.Val;
  };
  bool operator >= (const TDateTime& rhs)const
  {
  return Val >= rhs.Val;
  };
  bool operator <= (const TDateTime& rhs)const
  {
  return Val <= rhs.Val;
  };
*/
	bool operator !=(const TDateTime& rhs)const;
	bool operator >(const TDateTime& rhs)const;
	bool operator <(const TDateTime& rhs)const;
	bool operator >= (const TDateTime& rhs)const;
	bool operator <= (const TDateTime& rhs)const;

	///////////////////////////////////////////////////////////////////
 public:
	static const int AggregatedDaysInMonths[2][13];
	static const int DaysInMonths[2][13];
	static const char *Days[2][7];
	static const char *Months[2][12];
 public:
        double Val;
	void DecodeDate(unsigned int  & year, unsigned int  & month, unsigned int  & day)const;
	void DecodeTime(unsigned int  & hour, unsigned int  & min, unsigned int  & sec)const;
	TDateTime(const double src)
		{
			Val = src;
		
		};
	TDateTime(const int src)
		{
			Val = src;
		};	
	TDateTime& operator =(const double rhs)
		{
			Val = rhs;
			return *this;
		};
	
	TDateTime& operator = (const int rhs)
		{
			Val = rhs;
			return *this;
		};
	

	TDateTime& operator +=(const double rhs)
		{
			Val += rhs;
			return *this;
		};
	


	TDateTime& operator -=(const TDateTime& rhs)
		{
			Val -= rhs.Val;
			return *this;
		};

	TDateTime& operator -=(const double rhs)
		{
			Val -= rhs;
			return *this;
		}; 
	
	TDateTime operator +(const TDateTime& rhs)const
		{
			return Val + rhs.Val;
		};
	TDateTime operator +(const double rhs)const
		{
			return Val + rhs;
		};
	TDateTime operator -(const double rhs)
		{
			return Val - rhs;
		};
	
	TDateTime operator -(const int rhs)
		{
			return Val -rhs;
		};
	operator double()const
		{
			return Val;
		};

	operator int()const
		{
			return (int)Val;
		};
};

#endif
