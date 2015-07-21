/*! \file    TDateTime.cpp
   \brief    日期时间接口封装
   \author   zyk
   \date     2005-04-04
 */

#include "TDateTime.h"
#include "rtc.h"
#include "arithmetic.h"

#define EPOCH_YEAR 1900
#define EPOCH_OFFSET 2
#define EPOCH_WEEK 1    
#define MAXSEC (24*60*60)
#define INVALID_DATE_VALUE 0xffffff

#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_debug.h"

const int TDateTime::AggregatedDaysInMonths[2][13]=
{
	{0,31,59,90,120,151,181,212,243,273,304,334,365},
	{0,31,60,91,121,152,182,213,244,274,305,335,366}
};
const int TDateTime::DaysInMonths[2][13]=
{
	{0,31,28,31,30,31,30,31,31,30,31,30,31},
	{0,31,29,31,30,31,30,31,31,30,31,30,31}
};

const char * TDateTime::Days[2][7]=
{
	{"日","一","二","三","四","五","六"},
	{"星期日","星期一","星期二","星期三","星期四","星期五","星期六"}
};

const char * TDateTime::Months[2][12]=
{
	{"一","二","三","四","五","六","七","八","九","十" ,"十一","十二"},
	{"一月","二月","三月","四月","五月","六月","七月","八月","九月","十月" ,"十一月","十二月"}
};


int TDateTime::DateIsValid(unsigned int   year, unsigned int   month,
                           unsigned int   day)
{
	if (month < 1 || month > 12 || day < 1 || day > 31)
		return 0;
	if ((month == 4 || month == 6 || month == 9 || month == 11)
	    && day > 30)
		return 0;
	if (month == 2) {
		if (IsLeapYear(year)) {
			if (day > 29)
				return 0;
		} else if (day > 28)
			return 0;
	}

	return 1;
}

int TDateTime::TimeIsValid(unsigned int   hour, unsigned int   min,
                           unsigned int   sec)
{
	if (hour > 23 || min > 59 || sec > 59)
		return 0;
	else
		return 1;
}

int TDateTime::IsLeapYear(unsigned int   year) 
{
	if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		return 1;
	else
		return 0;
}

double TDateTime::ComposeDate(unsigned int   year, unsigned int   month,
                              unsigned int   day)
{
	double result = 0;
	int y;

	if (year >= EPOCH_YEAR) {
		for (y = EPOCH_YEAR; y < year; y++) {
			result +=
				365 +
				(((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) ? 1 : 0);
		}
	} else {
		for (y = year; y < EPOCH_YEAR; y++)
			result -=
				(365 +
				 (((y % 4 == 0 && y % 100 != 0) || y % 400 == 0) ? 1 : 0));

	}

	result +=
		TDateTime::AggregatedDaysInMonths[IsLeapYear(year)][month - 1] + day +
		EPOCH_OFFSET - 1;

	return result;
}

double TDateTime::ComposeTime(unsigned int   hour, unsigned int   min,
			      unsigned int   sec)
{
	double result;
	result =((double) ((hour * 60) + min) * 60 + sec) / MAXSEC;
	return result;
}



int TDateTime::DecodeDateString(const char *src,
				unsigned  int &year,
				unsigned  int &month,
				unsigned  int &day)
{

	char buf[64];
	char srcbuf[64];
	char *psrc;
	int n=0;
	int j=0;
	int len=0;
//	if( src == NULL || strlen(src) < 8  || strlen(src) > 10 )
	if( src == NULL || strlen(src) < 8 )
	{
		return 0;
	}
	len=strlen(src);
	memset(buf,0,sizeof(buf));
	memset(srcbuf,0,sizeof(srcbuf));
	n = len > sizeof(srcbuf) ? sizeof(srcbuf)-1:len;
	memcpy(srcbuf,src,n);
	n=0;
	psrc=srcbuf;

	for(;;)
	{
		if( *psrc == '\0' )
		{
			return 0;
		}

		if(*psrc >= '0' && *psrc <= '9' )
		{
			break;
		}
		psrc++;
	}
  
	for( j=0;j<4;j++)
	{
		if( *psrc < '0' || *psrc > '9' )
		{
			return 0;
		}
		buf[n++]=*psrc;
		psrc++;
	}

	buf[n++]=' ';

	for(;;)
	{
		if( *psrc == '\0' )
		{
			return 0;
		}

		if(*psrc >= '0' && *psrc <= '9' )
		{
			break;
		}
		psrc++;
	}

	for(j=0;j<2;j++)
	{
		if( *psrc == '\0' )
		{
			return 0;
		}

		if(*psrc < '0' || *psrc > '9' )
		{
			break;
		}

		buf[n++]=*psrc;
		psrc++;	  
	}

	buf[n++]=' ';

	for(;;)
	{
		if( *psrc == '\0' )
		{
			return 0;
		}

		if(*psrc >= '0' && *psrc <= '9' )
		{
			break;
		}
		psrc++;
	}

	for(j=0;j<2;j++)
	{
		if( *psrc == '\0' )
		{
			break;
		}

		if(*psrc < '0' || *psrc > '9' )
		{
			break;
		}

		buf[n++]=*psrc;
		psrc++;	  
	}
//	if(*psrc != '\0' )
//	{
//	  return 0;
//	}
	sscanf(buf,"%4u %2u %2u",&year,&month,&day); 
	if(DateIsValid(year,month,day) == 0 )
	{
		return 0;
	}
	return psrc - srcbuf ;
}

int TDateTime::DecodeTimeString(const char *src,
				unsigned  int &hour,
				unsigned  int &min,
				unsigned  int &sec)
{
	char buf[64];
	char srcbuf[64];
	char *psrc;
	int n=0;
	int j=0;
	int len=0;
//	if( src == NULL || strlen(src) < 6 ||strlen(src) > 8 )
	if( src == NULL || strlen(src) < 6 )
	{
		return 0;
	}
	len=strlen(src);
	memset(buf,0,sizeof(buf));
	memset(srcbuf,0,sizeof(srcbuf));
	n = len > sizeof(srcbuf) ? sizeof(srcbuf)-1:len;
	memcpy(srcbuf,src,n);
	n=0;
	psrc=srcbuf;

	for(;;)
	{
		if( *psrc == '\0' )
		{
			return 0;
		}

		if(*psrc >= '0' && *psrc <= '9' )
		{
			break;
		}
		psrc++;
	}
	//hour
	for( j=0;j<2;j++)
	{
		if( *psrc == '\0' )
		{
			return 0;
		}

		if(*psrc < '0' || *psrc > '9' )
		{
			break;
		}

		buf[n++]=*psrc;
		psrc++;	  
	}

	buf[n++]=' ';

	for(;;)
	{
		if( *psrc == '\0' )
		{
			return 0;
		}

		if(*psrc >= '0' && *psrc <= '9' )
		{
			break;
		}
		psrc++;
	}
	//minute
	for(j=0;j<2;j++)
	{
		if( *psrc == '\0' )
		{
			return 0;
		}

		if(*psrc < '0' || *psrc > '9' )
		{
			break;
		}
		buf[n++]=*psrc;
		psrc++;	  
	}

	buf[n++]=' ';

	for(;;)
	{
		if( *psrc == '\0' )
		{
			return 0;
		}

		if(*psrc >= '0' && *psrc <= '9' )
		{
			break;
		}
		psrc++;
	}
	//sec
	for(j=0;j<2;j++)
	{
		if( *psrc == '\0' )
		{
			return 0;
		}

		if(*psrc < '0' || *psrc > '9' )
		{
			break;
		}

		buf[n++]=*psrc;
		psrc++;	  
	}
//  if(*psrc != '\0' )
//  {
//    return 0;
//  }
	sscanf(buf,"%u %u %u",&hour,&min,&sec); 

	if(TimeIsValid(hour,min,sec) == 0 )
	{
		return 0;
	}
	return psrc - srcbuf ;
}

int TDateTime::DecodeDateTimeString(const char *src,
				    unsigned  int &year,
				    unsigned  int &month,
				    unsigned  int &day,
				    unsigned  int &hour,
				    unsigned  int &min,
				    unsigned  int &sec)
{

	if(strlen(src) < 14 )
	{
		return 0;
	}
	int offset=DecodeDateString(src,year,month,day);
	if(offset == 0 )
	{
		return 0;
	}
	return DecodeTimeString(src+offset,hour,min,sec);

}

////////////////////////////////////////////////////////////////////////
TDateTime::TDateTime()
{
	Val = INVALID_DATE_VALUE;
}

TDateTime::TDateTime(const TDateTime& src)
{
	Val = src.Val;
}

TDateTime::TDateTime(const char* src, TDateTimeFlag flag)
{
	unsigned  int year = 0;
	unsigned  int month = 0;
	unsigned  int day = 0;
	unsigned  int hour = 0;
	unsigned  int min = 0;
	unsigned  int sec = 0;
 
	int result=0;
	Val = 0;

	if(flag == DATE_POS)
	{
		result=DecodeDateString(src,year,month,day);
		if(result == 0 )
		{
			Val = INVALID_DATE_VALUE;
		}
		else
		{
			Val = ComposeDate(year, month, day);
		}
	}
	else if(flag == TIME_POS )
	{
		result=DecodeTimeString(src,hour,min,sec);
		if(result == 0 )
		{
			Val = INVALID_DATE_VALUE;
		}
		else
		{
			Val = ComposeTime(hour, min, sec);
		}
	}
	else if(flag == DATETIME_POS )
	{
		result=DecodeDateTimeString(src,year,month,day,hour,min,sec);
		if(result == 0 )
		{
			Val = INVALID_DATE_VALUE;
		}
		else
		{
			Val = ComposeDate(year, month, day);
			Val += ComposeTime(hour, min, sec);
		}
	}
 

}

TDateTime::TDateTime(unsigned int   year, unsigned int   month,
                     unsigned int   day)
{
	if (DateIsValid(year, month, day))
	{
		Val = ComposeDate(year, month, day);
	}
	else {
		Val = INVALID_DATE_VALUE;
	}
}

TDateTime::TDateTime(unsigned int   hour, unsigned int   min,
                     unsigned int   sec,unsigned int   timeflag)
{
	if (TimeIsValid(hour, min, sec))
	{
		Val = ComposeTime(hour, min, sec);
	}
	else {
		Val = INVALID_DATE_VALUE;
	}
}

TDateTime::TDateTime(unsigned int   year, unsigned int   month, unsigned int   day,
		     unsigned int   hour, unsigned int   min, unsigned int   sec)
{
	if (DateIsValid(year, month, day))
	{
		Val = ComposeDate(year, month, day);
		if (TimeIsValid(hour, min, sec))
		{
			Val += ComposeTime(hour, min, sec);
		}
		else 
		{
			Val = INVALID_DATE_VALUE;
		}
	}
	else 
	{
		Val = INVALID_DATE_VALUE;
	}	
}


 TDateTime::TDateTime(unsigned int date, unsigned int time)
 {
	 unsigned int year, month, day, hour, min, sec;
     year = date/10000;
     month = (date - year*10000)/100;
	 day = date%100;

	 hour = time/10000;
     min = (time - hour*10000)/100;
	 sec = time%100;
     
     if (DateIsValid(year, month, day))
	{
		Val = ComposeDate(year, month, day);
		if (TimeIsValid(hour, min, sec))
		{
			Val += ComposeTime(hour, min, sec);
		}
		else 
		{
			Val = INVALID_DATE_VALUE;
		}
	}
	else 
	{
		Val = INVALID_DATE_VALUE;
	}	
 }


void TDateTime::DecodeDate(unsigned int   &year, unsigned int   &month,
                           unsigned int   &day)const
{
	int days = (int) Val;
	days -= EPOCH_OFFSET;
  if((int)Val >= INVALID_DATE_VALUE)
		return;

	if (days >= 0) {
		year = EPOCH_YEAR;
		for (; days > 364;) {
			if (IsLeapYear(year)) {
				if (days > 365) {
					days -= 366;
					year++;
				} else
					break;
			} else {
				days -= 365;
				year++;
			}
		}

		int mtemp = IsLeapYear(year);
		int i;
		for (i = 1; i <= 12; i++) {
			if (AggregatedDaysInMonths[mtemp][i] > days)
				break;
		}
		month = i;
		day = days - AggregatedDaysInMonths[mtemp][i - 1] + 1;
	} else {
		year = EPOCH_YEAR - 1;
		for (; days < -364;) {
			if (IsLeapYear(year)) {
				if (days < -365) {
					days += 366;
					(year)--;
				} else
					break;
			} else {
				days += 365;
				(year)--;
			}
		}

		int ntemp = IsLeapYear(year);
		int j;
		days += (365 + ntemp);
		for (j = 1; j <= 12; j++) {
			if (AggregatedDaysInMonths[ntemp][j] > days)
				break;
		}
		month = j;
		day = days - AggregatedDaysInMonths[ntemp][j - 1] + 1;
	}
  
}
void TDateTime::DecodeTime(unsigned int   &hour, unsigned int   &min,
                           unsigned int   &sec) const
{
	int secs=0;
	int days=0;
	double afterdot=0;

	days = (int) Val;
	afterdot = (Val - days) * MAXSEC;
	secs = (int) afterdot;
	if((afterdot - (int)afterdot) > 0.1/MAXSEC )
	{
		secs++;
	}
	if (secs < 0)
		secs = -secs;

	sec = secs % 60;
	secs /= 60;
	min = secs % 60;
	secs /= 60;
	hour = secs;
}
bool TDateTime::operator ==(const TDateTime& rhs)const 
{
 
        if((int )Val != (int)(rhs.Val))
		return false;
        if( (Val-(int)Val )* MAXSEC != (rhs.Val-(int)(rhs.Val))* MAXSEC ) 
		return false;

        return true;
}
bool TDateTime::operator !=(const TDateTime& rhs)const
{
        if((int )Val != (int)(rhs.Val))
		return true;
        if( (Val-(int)Val )* MAXSEC != (rhs.Val-(int)(rhs.Val))* MAXSEC ) 
		return true;

        return false;

}
bool TDateTime::operator >(const TDateTime& rhs)const
{
        if((int )Val > (int)(rhs.Val))
        {
		return true;
        }
        else if((int )Val < (int)(rhs.Val))
        {
		return false;
        }

        if( (Val-(int)Val )* MAXSEC > (rhs.Val-(int)(rhs.Val))* MAXSEC ) 
		return true;

	return false;
}
bool TDateTime::operator <(const TDateTime& rhs)const
{
        if((int )Val < (int)(rhs.Val))
        {
		return true;
        }
        else if((int )Val > (int)(rhs.Val))
        {
		return false;
        }

        if( (Val-(int)Val )* MAXSEC < (rhs.Val-(int)(rhs.Val))* MAXSEC ) 
		return true;

	return false;
}
bool TDateTime::operator >= (const TDateTime& rhs)const
{
        if((int )Val < (int)(rhs.Val))
        {
		return false;
        }
        else if((int )Val > (int)(rhs.Val))
        {
		return true;
        }

        if( (Val-(int)Val )* MAXSEC >= (rhs.Val-(int)(rhs.Val))* MAXSEC ) 
		return true;

	return false;
}
bool TDateTime::operator <= (const TDateTime& rhs)const
{
        if((int )Val > (int)(rhs.Val))
        {
		return false;
        }
        else if((int )Val < (int)(rhs.Val))
        {
		return true;
        }

        if( (Val-(int)Val )* MAXSEC <= (rhs.Val-(int)(rhs.Val))* MAXSEC ) 
		return true;

	return false;
}
TDateTime TDateTime::MonthBegin(const TDateTime & rhs)
{
	unsigned int year=0;
	unsigned int  month=0;
	unsigned int day=0;

	TDateTime tmp(rhs);
	tmp.DecodeDate(year, month, day);
	tmp -= (int)(day - 1);
	return tmp;
}
TDateTime TDateTime::MonthEnd(const TDateTime & rhs)
{
	unsigned  int year=0;
	unsigned  int month=0;
	unsigned  int day=0;
	unsigned  int lastday=0;

	TDateTime tmp(rhs);
	tmp.DecodeDate(year, month, day);
	lastday = DaysInMonths[IsLeapYear(year)][month];
	tmp += (int)(lastday - day);
	return tmp;
}
TDateTime TDateTime::MonthAdd(const TDateTime & rhs, int Months)
{

	unsigned  int year=0;
	unsigned  int month=0;
	unsigned  int day=0;
	unsigned  int lastday=0;
	int _Months=0;

	TDateTime tmp(rhs);
	tmp.DecodeDate(year, month, day);
	_Months = month + Months;
	if (_Months >= 0) {
		year += _Months / 12;
		month = _Months % 12;
		if (month == 0) {
			year--;
			month = 12;
		}
	} else {
		_Months = -_Months;
		year -= (_Months / 12 + 1);
		month = 12 - _Months % 12;
	}
	lastday = DaysInMonths[IsLeapYear(year)][month];
	if (day > lastday)
		day = lastday;
	tmp = ComposeDate(year, month, day);
	return tmp;
}

TDateTime TDateTime::HourAdd(const TDateTime& rhs, int addHours)
{
	unsigned  int year=0;
	unsigned  int month=0;
	unsigned  int day=0;
	unsigned  int hour=0;
	unsigned  int min=0;
	unsigned  int sec=0;
	
	TDateTime tmp(rhs);
	tmp.DecodeDate(year, month, day);
	tmp.DecodeTime(hour, min, sec);
	
	int tmpHours = hour + addHours;
	int tmpDays = tmpHours/24;
	hour = tmpHours%24;
	
	TDateTime midDate(year, month, day, hour, min, sec);
	
	tmp = midDate + tmpDays;
	return tmp;
}

string TDateTime::FormatString(TDateTimeFormat format,const char *formatstr)
{
	unsigned  int year=0;
	unsigned  int yy=0;
	unsigned  int month=0;
	unsigned  int day=0;
	unsigned  int hour=0;
	unsigned  int min =0;
	unsigned  int sec=0;


	char result[64];
  
	DecodeDate(year, month, day);
	DecodeTime(hour, min, sec);


	memset(result,0,sizeof(result));
	switch(format) {
	case YYYYMMDD:
	{
		if(formatstr == NULL || strlen(formatstr) > 64 )
		{
			sprintf(result,"%u-%02u-%02u",year,month,day);
			
		}
		else
		{
			sprintf(result,formatstr,year,month,day);
		
		}
	}
  	break;
	case HHMMSS:
	{
		if(formatstr == NULL || strlen(formatstr) > 64 )
		{
			sprintf(result,"%02u:%02u:%02u",hour,min,sec);
		}
		else
		{
			sprintf(result,formatstr,hour,min,sec);
		}
	}
  	break;
	case YYYYMMDDHHMMSS:
	{
		if(formatstr == NULL || strlen(formatstr) > 64 )
		{
			sprintf(result,"%u-%02u-%02u %02u:%02u:%02u",
				year,month,day,hour,min,sec);
		}
		else
		{
			sprintf(result,formatstr,year,month,day,hour,min,sec);
		}
	}
	break;
	case YYMMDD:
	{
		yy=year-(year/100)*100;
		if(formatstr == NULL || strlen(formatstr) > 64 )
		{
			sprintf(result,"%02u-%02u-%02u",yy,month,day);
		}
		else
		{
			sprintf(result,formatstr,yy,month,day);
		}
	}
	break;
	case YYMMDDHHMMSS:
	{
		yy=year-(year/100)*100;
		if(formatstr == NULL || strlen(formatstr) > 64 )
		{
			sprintf(result,"%02u-%02u-%02u %02u:%02u:%02u",
				yy,month,day,hour,min,sec);
		}
		else
		{
			sprintf(result,formatstr,yy,month,day,hour,min,sec);
		}
	}
	break;
	case YYYYMMDDHHMM:
		{
			if(formatstr == NULL || strlen(formatstr) > 64 )
			{
				sprintf(result,"%u-%02u-%02u %02u:%02u",
					year,month,day,hour,min);
			}
			else
			{
				sprintf(result,formatstr,year,month,day,hour,min);
			}
		}
		break;
	default:
		sprintf(result,"%u-%02u-%02u",year,month,day);
		break;
	}
	return result;
}

//得到UINT32类型的时间,只限于YYYYMMDD和HHMMSS两种类型
unsigned int TDateTime::FormatInt(TDateTimeFormat format,const char *formatstr)			
{
	unsigned int nTmpTime;
	string strTmpTime;
    strTmpTime = FormatString(format,formatstr);
    switch(format)
	{
    case YYYYMMDD:
        strTmpTime.erase(4,1);
        strTmpTime.erase(6,1);
    	break;
    case HHMMSS:
		strTmpTime.erase(2,1);
        strTmpTime.erase(4,1);
    	break;
    default:;
    } 	
    nTmpTime = atoi(strTmpTime.c_str());

	return nTmpTime;
}


TDateTime TDateTime::CurrentDateTime()
{

	time_t ti;
	struct tm *ptm;
	time(&ti);
	ptm=localtime(&ti);
	return TDateTime(ptm->tm_year+EPOCH_YEAR,ptm->tm_mon+1,ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
}

TDateTime TDateTime::CurrentDate()
{

	time_t ti;
	struct tm *ptm;
	time(&ti);
	ptm=localtime(&ti);
	return TDateTime(ptm->tm_year+EPOCH_YEAR,ptm->tm_mon+1,ptm->tm_mday);
}

bool TDateTime::SetCurrentDateTime(TDateTime &rhs)
{
#ifndef WIN32
 	unsigned int year=0, month=0, day=0, hour=0, min=0, sec=0;
	rhs.DecodeDate( year, month, day );
	rhs.DecodeTime( hour, min, sec );

	//更新操作系统时间
	struct tm tmSys;
	time_t tSys;

	tmSys.tm_year = year - 1900;
	tmSys.tm_mon = month - 1;
	tmSys.tm_mday = day;
	tmSys.tm_sec = sec;
	tmSys.tm_min = min;
	tmSys.tm_hour = hour;
	tmSys.tm_isdst = -1;

	if ( ( tSys = mktime( &tmSys ) ) == -1 )
	{
		DBG_PRN("info", ("mktime error!!"));		
		return false;
	}
	if ( stime( &tSys ) == -1)
	{
		DBG_PRN("info", ("stime error!!"));		
		return false;
	}
	
	//更RTC时间
	unsigned char tmp[3];
	int datetime;
	datetime = rhs.FormatInt(YYYYMMDD);
	int2bin(&tmp[0], (datetime / 10000) % 100, 1);
	int2bin(&tmp[1], (datetime / 100) % 100, 1);
	int2bin(&tmp[2], datetime % 100, 1);
	SetRTCData(tmp);

	datetime = rhs.FormatInt(HHMMSS);
	int2bin(&tmp[0], (datetime / 10000) % 100, 1);
	int2bin(&tmp[1], (datetime / 100) % 100, 1);
	int2bin(&tmp[2], datetime % 100, 1);
	SetRTCTime(tmp);
               	

#endif
	return true;
}

bool TDateTime::SetCurrentDate(TDateTime &rhs)
{
#ifndef WIN32
	unsigned int year=0, month=0, day=0, hour=0, min=0, sec=0;

	rhs.DecodeDate(year, month, day);
	TDateTime curDateTime = CurrentDateTime();
	curDateTime.DecodeTime( hour, min, sec );
	
	TDateTime tmpDateTime(year, month, day, hour, min, sec);
	return SetCurrentDateTime(tmpDateTime);

#endif
	return true;
}

int TDateTime::DayOfWeek() const
{
	int days = (int) Val;
	days -= EPOCH_OFFSET;
	if (days >= 0) {
		return ((EPOCH_WEEK + days) % 7);
	} else {
		return (7 + (EPOCH_WEEK + days) % 7);
	}
}
unsigned int TDateTime::DayOfMonth(void)const
{
	unsigned  int year=0;
	unsigned  int month=0;
	unsigned  int day=0;
	DecodeDate(year, month, day);
	return day;
}
unsigned int TDateTime::Year(void)const
{
	unsigned  int year=0;
	unsigned  int month=0;
	unsigned  int day=0;
	DecodeDate(year, month, day);
	return year;
}
unsigned int TDateTime::Month(void)const
{
	unsigned  int year=0;
	unsigned  int month=0;
	unsigned  int day=0;
	DecodeDate(year, month, day);
	return month;
}
unsigned int TDateTime::Hour(void)const
{
	unsigned  int hour=0;
	unsigned  int min=0;
	unsigned  int sec=0;
	DecodeTime(hour, min, sec);
	return hour;
}
unsigned int TDateTime::Minute(void)const
{
	unsigned  int hour=0;
	unsigned  int min=0;
	unsigned  int sec=0;
	DecodeTime(hour, min, sec);
	return min;
}
unsigned int TDateTime::Second(void)const
{
	unsigned  int hour=0;
	unsigned  int min=0;
	unsigned  int sec=0;
	DecodeTime(hour, min, sec);
	return sec;
}

int TDateTime::DateIsValid(void)
{

	unsigned  int year=0;
	unsigned  int month=0;
	unsigned  int day=0;
	if((unsigned int)Val >= INVALID_DATE_VALUE)
	{
		return 0;
	}
	DecodeDate(year, month, day);	
	return DateIsValid(year,month,day);
}
int TDateTime::TimeIsValid(void)
{
	unsigned  int hour=0;
	unsigned  int min=0;
	unsigned  int sec=0;
	if((unsigned int)Val >= INVALID_DATE_VALUE)
	{
		return 0;
	}
	DecodeTime(hour, min, sec);
	return TimeIsValid(hour,min,sec);
}

int TDateTime::DateTimeIsValid(void)
{
	return (DateIsValid() && TimeIsValid());
}

int TDateTime::IsLeapYear()
{
	unsigned  int year=0;
	unsigned  int month=0;
	unsigned  int day=0;
	DecodeDate(year, month, day);	
	return IsLeapYear(year);
}
