/*! \file pos_log.h
   \brief 本文件定义了调试信息输出接口
   \author zyk
 */

#ifndef POS_LOG_ZYK_H
#define POS_LOG_ZYK_H
#ifdef __cplusplus

extern "C"
{  
#endif
	
#ifdef NO_POS_DEBUG
#undef POS_DEBUG_TO_FILE
#undef POS_DEBUG_TO_SERIAL
#undef POS_DEBUG_TO_TTY
#endif	

#if !defined(POS_DEBUG_TO_FILE) && !defined(POS_DEBUG_TO_SERIAL) && !defined(POS_DEBUG_TO_TTY)
#define NO_POS_DEBUG
#endif


#ifdef  NO_POS_DEBUG

        #define prn_log(x)
        #define prn_log1(x)
        #define prn_log2(x,y) 
        #define prn_log3(x,y,z) 
        #define prn_log4(x,y,z,a) 
        #define prn_log5(x,y,z,a,b) 
        #define prn_log6(x,y,z,a,b,c) 
        #define prn_log7(x,y,z,a,b,c,d) 
        #define prn_log8(x,y,z,a,b,c,d,e) 
        #define prn_log9(x,y,z,a,b,c,d,e,f) 
        #define prn_log10(x,y,z,a,b,c,d,e,f,g) 
		#define prn_log11(x,y,z,a,b,c,d,e,f,g,h) 
		#define prn_logx(x)
#else
extern    const char *GetFileName(const char *fullfilename);
#endif // POS_DEBUG_TO_TTY

#ifdef POS_DEBUG_TO_FILE 
    #include<stdio.h>
#ifdef WIN32    
    extern  char g_log_file_path_and_file[512];
#endif
    void write_log(const char * file , int line ,const char * fmt , ...);
	void write_log2(const char * info);
    #define prn_log(x) write_log(GetFileName(__FILE__),__LINE__,x)
    #define prn_log1(x) write_log(GetFileName(__FILE__),__LINE__,x)
   
    #define prn_log2(x,y) write_log(GetFileName(__FILE__),__LINE__,x,y) 
   
    #define prn_log3(x,y,z) write_log(GetFileName(__FILE__),__LINE__,x,y,z) 
   
    #define prn_log4(x,y,z,a) write_log(GetFileName(__FILE__),__LINE__,x,y,z,a)
   
    #define prn_log5(x,y,z,a,b) write_log(GetFileName(__FILE__),__LINE__,x,y,z,a,b) 
   
    #define prn_log6(x,y,z,a,b,c) write_log(GetFileName(__FILE__),__LINE__,x,y,z,a,b,c)
   
    #define prn_log7(x,y,z,a,b,c,d) write_log(GetFileName(__FILE__),__LINE__,x,y,z,a,b,c,d)
   
    #define prn_log8(x,y,z,a,b,c,d,e) write_log(GetFileName(__FILE__),__LINE__,x,y,z,a,b,c,d,e)
   
    #define prn_log9(x,y,z,a,b,c,d,e,f) write_log(GetFileName(__FILE__),__LINE__,x,y,z,a,b,c,d,e,f)
   
    #define prn_log10(x,y,z,a,b,c,d,e,f,g) write_log(GetFileName(__FILE__),__LINE__,x,y,z,a,b,c,d,e,f,g)

	#define prn_log11(x,y,z,a,b,c,d,e,f,g,h) write_log(GetFileName(__FILE__),__LINE__,x,y,z,a,b,c,d,e,f,g,h)

	#define prn_logx(x) write_log2(x)
	
#endif // #ifdef POS_DEBUG_TO_FILE 

#ifdef POS_DEBUG_TO_TTY

#include<stdio.h>

#define prn_log(x) {\
		 printf("\n");\
		 printf("%s %d %s \n" ,GetFileName(__FILE__),__LINE__,x); \
		 }
		 //#define prn_log(x) printf("%s %d %s \n" ,__FILE__,__LINE__,x)
         #define prn_log1(x) printf("%s %d %s \n" ,__FILE__,__LINE__,x)
        
         #define prn_log2(x,y) {\
         printf("%s %d ",GetFileName(__FILE__),__LINE__); \
         printf(x,y);\
         printf("\n");\
          }
        
         #define prn_log3(x,y,z) {\
         printf("%s %d ",GetFileName(__FILE__),__LINE__); \
         printf(x,y,z);\
         printf("\n");\
          }
        
         #define prn_log4(x,y,z,a) {\
         printf("%s %d ",GetFileName(__FILE__),__LINE__); \
         printf(x,y,z,a);\
         printf("\n");\
          }
        
         #define prn_log5(x,y,z,a,b) {\
         printf("%s %d ",GetFileName(__FILE__),__LINE__); \
         printf(x,y,z,a,b);\
         printf("\n");\
          }
        
         #define prn_log6(x,y,z,a,b,c) {\
         printf("%s %d ",GetFileName(__FILE__),__LINE__); \
         printf(x,y,z,a,b);\
         printf("\n");\
          }
        
         #define prn_log7(x,y,z,a,b,c,d) {\
         printf("%s %d ",GetFileName(__FILE__),__LINE__); \
         printf(x,y,z,a,b,c,d);\
         printf("\n");\
          }
        
         #define prn_log8(x,y,z,a,b,c,d,e) {\
         printf("%s %d ",GetFileName(__FILE__),__LINE__); \
         printf(x,y,z,a,b,c,d,e);\
         printf("\n");\
          }
        
         #define prn_log9(x,y,z,a,b,c,d,e,f) {\
         printf("%s %d ",GetFileName(__FILE__),__LINE__); \
         printf(x,y,z,a,b,c,d,e,f);\
         printf("\n");\
          }
        
         #define prn_log10(x,y,z,a,b,c,d,e,f,g) {\
         printf("%s %d ",GetFileName(__FILE__),__LINE__); \
         printf(x,y,z,a,b,c,d,e,f,g);\
         printf("\n");\
          }

		 #define prn_log11(x,y,z,a,b,c,d,e,f,g,h) {\
         printf("%s %d ",GetFileName(__FILE__),__LINE__); \
         printf(x,y,z,a,b,c,d,e,f,g,h);\
         printf("\n");\
          }

		 #define prn_logx(x) printf(x);

#endif



#ifdef POS_DEBUG_TO_SERIAL
		 
#include<stdio.h>

extern int   open_debug_com(void);

extern void  SendStrToSerial(int port, unsigned char *str,int len ); 

extern void log2serial(const char * file, int line, const char * fmt , ...);

extern void log2serial2(const char * fmt , ...);

extern void log2serial3(const char * info);

#define prn_log(x) log2serial(GetFileName(__FILE__), __LINE__, x)
#define prn_log1(x) log2serial(GetFileName(__FILE__), __LINE__, x)
#define prn_log2(x,y) log2serial(GetFileName(__FILE__), __LINE__, x,y)
#define prn_log3(x,y,z) log2serial(GetFileName(__FILE__), __LINE__, x,y,z)
#define prn_log4(x,y,z,a) log2serial(GetFileName(__FILE__), __LINE__, x,y,z,a)
#define prn_log5(x,y,z,a,b) log2serial(GetFileName(__FILE__), __LINE__, x,y,z,a,b)
#define prn_log6(x,y,z,a,b,c) log2serial(GetFileName(__FILE__), __LINE__, x,y,z,a,b,c)
#define prn_log7(x,y,z,a,b,c,d) log2serial(GetFileName(__FILE__), __LINE__, x,y,z,a,b,c,d)
#define prn_log8(x,y,z,a,b,c,d,e) log2serial(GetFileName(__FILE__), __LINE__, x,y,z,a,b,c,d,e)
#define prn_log9(x,y,z,a,b,c,d,e,f) log2serial(GetFileName(__FILE__), __LINE__, x,y,z,a,b,c,d,e,f)
#define prn_log10(x,y,z,a,b,c,d,e,f,g) log2serial(GetFileName(__FILE__), __LINE__, x,y,z,a,b,c,d,e,f,g)
#define prn_log11(x,y,z,a,b,c,d,e,f,g,h) log2serial(GetFileName(__FILE__), __LINE__, x,y,z,a,b,c,d,e,f,g,h)

#define prn_logx(x) log2serial3(x)

#endif // POS_DEBUG_TO_SERIAL



#ifdef __cplusplus
}
#endif

#endif // POS_LOG_ZYK_H

