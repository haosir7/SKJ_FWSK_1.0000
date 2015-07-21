//时间和字符串函数。
#ifndef CKWSHARE_H
#define CKWSHARE_H

#include "string.h"
#include "stdlib.h"
#include "time.h"

#ifndef IN
#define IN
#endif // IN

#ifndef OUT
#define OUT
#endif // OUT

#define  TIME_FMT  "%Y%m%d %H:%M:%S"

#if (defined(_WIN32) || defined(_WIN64))
	#define SLEEPMS(ms)	Sleep(ms)
	#define	SLEEPS(s)	Sleep((s*1000))
#else
	#define SLEEPMS(ms)	usleep(ms*1000)
	#define	SLEEPS(s)	sleep(s)
	#define stricmp(s1, s2) strcasecmp(s1, s2)
	#define strnicmp(s1, s2, maxlen) strncasecmp(s1, s2, maxlen)
#endif

#ifdef __cplusplus
extern "C" {
#endif

char* _strupr (char* str);
char* _strlwr (char* str);
void safestrcpy(char *str1, const char *str2, int maxlen);
void safefree(void *memblock );
void GetCurTime(char * sTime);
void GetThatTime(char * sTime,time_t ltime);
time_t get_time_milisec(time_t *);//得到当前毫秒值。参数可NULL，如非空则结果保存在参数变量中。
int FindData1InDATA2(unsigned char* data1,int data1len,unsigned char* data2,int data2len);
char* trim(IN char* s);


//判断某个数字num是否属于某一个范围 str_scope，如" 2 -  6 ; 192 ; 80.2 - 0 0. 6 ;"，要求后者仅含有空格、数字和".;-"。
//,"ALL","all","*"代表所有的范围;
//返回值：0--不属于，1--属于，-1--参数中有错误或非法字符。
//1.首先去掉字符串中的空格，判断字符串中仅含有数字和".;-"，否则返回-1。
//2.分析范围字串，遇到";"为一个段，段中有无"-"，将这个段的上下限转换为数字形式，判断上限是否大于下限（也可以不判断，不影响效果）。
//3.看num是否属于此段，如果是，返回值1；如果不是，继续分析范围字串；
//4.范围字串分析完了，返回0；
//////////
int  IsThisNumInTheScope(IN int num,IN char* str_scope);//ckw add 20141102.

#ifdef __cplusplus
}
#endif

#endif
