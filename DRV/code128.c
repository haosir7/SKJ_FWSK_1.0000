
//#include <stdio.h>
#include "code128.h" 

#include "SerialPort.h"
#include "PrintDriver.h"
#include "arithmetic.h"


#include "LOGCTRL.h"
//#define NO_POS_DEBUG
#include "pos_log.h"

const unsigned int code128[106][6]=
{							 
	
	{2,1,2,2,2,2},// sp          00			
	{2,2,2,1,2,2},// !           01
	{2,2,2,2,2,1},// "           02
	{1,2,1,2,2,3},// #           03
	{1,2,1,3,2,2},// $           04
	{1,3,1,2,2,2},// %           05
	{1,2,2,2,1,3},// &           06
	{1,2,2,3,1,2},// ...         07																				
	{1,3,2,2,1,2},// (           08
	{2,2,1,2,1,3},// )           09
	{2,2,1,3,1,2},// *           10
	{2,3,1,2,1,2},// +           11
	{1,1,2,2,3,2},// ,           12
	{1,2,2,1,3,2},// -           13
	{1,2,2,2,3,1},// .           14
	{1,1,3,2,2,2},// /           15
	{1,2,3,1,2,2},// 0           16
	{1,2,3,2,2,1},// 1           17
	{2,2,3,2,1,1},// 2           18
	{2,2,1,1,3,2},// 3           19
	{2,2,1,2,3,1},// 4           20
	{2,1,3,2,1,2},// 5           21
	{2,2,3,1,1,2},// 6           22
	{3,1,2,1,3,1},// 7           23
	{3,1,1,2,2,2},// 8           24
	{3,2,1,1,2,2},// 9           25
	{3,2,1,2,2,1},// :           26
	{3,1,2,2,1,2},// ;           27
	{3,2,2,1,1,2},// <           28
	{3,2,2,2,1,1},// =           29
	{2,1,2,1,2,3},// >           30
	{2,1,2,3,2,1},// ?           31
	{2,3,2,1,2,1},// @           32
	{1,1,1,3,2,3},// A           33
	{1,3,1,1,2,3},// B           34
	{1,3,1,3,2,1},// C           35
	{1,1,2,3,1,3},// D           36
	{1,3,2,1,1,3},// E           37
	{1,3,2,3,1,1},// F           38
	{2,1,1,3,1,3},// G           39
	{2,3,1,1,1,3},// H           40
	{2,3,1,3,1,1},// I           41
	{1,1,2,1,3,3},// J           42
	{1,1,2,3,3,1},// K           43
	{1,3,2,1,3,1},// L           44
	{1,1,3,1,2,3},// M           45
	{1,1,3,3,2,1},// N           46
	{1,3,3,1,2,1},// O           47
	{3,1,3,1,2,1},// P           48
	{2,1,1,3,3,1},// Q           49
	{2,3,1,1,3,1},// R           50
	{2,1,3,1,1,3},// S           51
	{2,1,3,3,1,1},// T           52
	{2,1,3,1,3,1},// U           53
	{3,1,1,1,2,3},// V           54
	{3,1,1,3,2,1},// W           55	 
	{3,3,1,1,2,1},// X           56
	{3,1,2,1,1,3},// Y           57
	{3,1,2,3,1,1},// Z           58
	{3,3,2,1,1,1},// [           59
	{3,1,4,1,1,1},// \           60
	{2,2,1,4,1,1},// ]           61
	{4,3,1,1,1,1},// ^           62
	{1,1,1,2,2,4},// _           63
	{1,1,1,4,2,2},// NUL   '     64
	{1,2,1,1,2,4},// SOH   a     65
	{1,2,1,4,2,1},// STX   b     66
	{1,4,1,1,2,2},// ETX   c     67
	{1,4,1,2,2,1},// EOT   d     68
	{1,1,2,2,1,4},// ENQ   e     69
	{1,1,2,4,1,2},// ACK   f     70
	{1,2,2,1,1,4},// BEL   g     71
	{1,2,2,4,1,1},// BS    h     72
	{1,4,2,1,1,2},// HT    i     73
	{1,4,2,2,1,1},// LF    j     74
	{2,4,1,2,1,1},// VT    k     75
	{2,2,1,1,1,4},// FF    l     76
	{4,1,3,1,1,1},// CR    m     77	  
	{2,4,1,1,1,2},// SO    n     78
	{1,3,4,1,1,1},// SI    o     79
	{1,1,1,2,4,2},// DLE   p     80
	{1,2,1,1,4,2},// DC1   q     81
	{1,2,1,2,4,1},// DC2   r     82
	{1,1,4,2,1,2},// DC3   s     83
	{1,2,4,1,1,2},// DC4   t     84
	{1,2,4,2,1,1},// NAK   u     85
	{4,1,1,2,1,2},// SYN   v     86
	{4,2,1,1,1,2},// ETB   w     87
	{4,2,1,2,1,1},// CAN   x     88
	{2,1,2,1,4,1},// EM    y     89
	{2,1,4,1,2,1},// SUB   z     90
	{4,1,2,1,2,1},// ESC   {     91
	{1,1,1,1,4,3},// FS    |     92
	{1,1,1,3,4,1},// GS    }     93
	{1,3,1,1,4,1},// RS    ~     94
	{1,1,4,1,1,3},// US    DEL   95
	{1,1,4,3,1,1},// FNC3  FNC3  96
	{4,1,1,1,1,3},// FNC2  FNC2  97
	{4,1,1,3,1,1},// SHIFT SHIFT 98
	{1,1,3,1,4,1},// CodeC CodeC 99
	{1,1,4,1,3,1},// CodeB FNC4  CodeB
	{3,1,1,1,4,1},// FNC4  CodeA CodeA
	{4,1,1,1,3,1},// FNC1  FNC1  FNC1
	{2,1,1,4,1,2},//      StartA
	{2,1,1,2,1,4},//      StartB
	{2,1,1,2,3,2}//      StartC
	
}; 

#define _MAX_DOT   210
#define _CenterDot  33
#define _1Dot       1
const unsigned char Startcode[]={2,1,1,2,3,2};
const unsigned char Stopcode[]={2,3,3,1,1,1,2};			

const unsigned char *WillprintNum;  
unsigned char LineCodeTab[91];
unsigned char linecodetab[91];
unsigned char checkcode=0;
							 
unsigned int a[24];

unsigned char CalculateCheckCode()
{
	unsigned  char i,checkcode=0;
	unsigned int sum=0;
	
	for(i=0;i<(strlen(WillprintNum))/2;i++)
	{
		sum+=(i+1)*a[i];						
	}
	sum+=105;
	checkcode=sum%103;
	//	printf("%d\n",(strlen(WillprintNum)));
	return	 checkcode;
	
}
void CreateRightCodeArray(void)
{
	unsigned  char i;
	unsigned int sum=0;
	
	for(i=0;i<strlen(WillprintNum);i++)	 
	{
		a[i]=*(WillprintNum+i);				  
	}
	for(i=0;i<strlen(WillprintNum)/2;i++)	  
	{
		a[i]=((a[2*i]-0x30))*10+(a[2*i+1]-0x30);//注意ascii字符表中字符0对应的hex值时0x30
	}

	for(i=0;i<(strlen(WillprintNum))/2;i++)
	{
		sum+=(i+1)*a[i];
	}
	sum+=105;
	checkcode=sum%103;
}


void CreateRightCode(void)
{
	unsigned char i,k,j=0;

	CreateRightCodeArray();	  //拆位存放在数组里面
	for(i=0;i<6;i++)				 //起始位
	{
		LineCodeTab[i]=Startcode[i];
	}								

   	for(i=0;i<(((strlen(WillprintNum))/2)*6);i+=6)		  //存放数据位
	{
		for(k=0;k<6;k++)
		{		
			LineCodeTab[i+k+6]=code128[(int)a[j]][k];
			LineCodeTab[k+(((strlen(WillprintNum))/2)*6)+6]=code128[checkcode][k];//校验位
		}
		j++; 
		if(j==(strlen(WillprintNum))/2)	
		{
			j=0;
		}
		
	}
				 
	for(i=0;i<7;i++)								//停止位
	{
		LineCodeTab[(((strlen(WillprintNum))/2+2)*6)+i]=Stopcode[i];
	}
	for(i=0;i<(((strlen(WillprintNum))/2+2)*6+7);i++)
	{
		linecodetab[i]=LineCodeTab[i];
	}
	
}


unsigned char  VerticalLineNum(void)//计算打印点数(包括黑条与空白)
{
	unsigned char i=0 ;
	unsigned char sum=0;	
	/*
	for(i=0;i<((((strlen(WillprintNum))/2)+2)*6+7);i++)
	{
		sum+=LineCodeTab[i];
	
	}
	*/
	sum = (strlen(WillprintNum))/2*11+35;
											 
	return sum;
}
void SetPrintLocation(int RightOffset)   //打印区域向右偏移RightOffset 个dot。
{
	unsigned char i=0;
	Code128Return( 1,0x1b);								
	us_delay(1000);
#if (POS_TYPE != POS_APE4000RG)
	Code128Return( 1,0x2B);			
	us_delay(1000);
	Code128Return( 1,0x01);			
			us_delay(1000);
#else
	Code128Return( 1,0x2a);			
	us_delay(1000);
	Code128Return( 1,0x00);			
	us_delay(1000);
#endif
	Code128Return( 1,RightOffset);
	us_delay(1000);
	Code128Return( 1,0x00);					 
	us_delay(1000);
	for(i=0;i<RightOffset;i++)
	{				
		Code128Return( 1,0x00);	//白条数量
		us_delay(2000);					 
	}	
}
void SetPrintCharLocation(unsigned char RightOffset)   //打印区域向右偏移RightOffset 个dot。
{
	unsigned char i;
#if 1
	Code128Return( 1,0x1B);								
	us_delay(1000);
	Code128Return( 1,0x2B);			
	us_delay(1000);
	Code128Return( 1,0x01);			
			us_delay(1000);
	//	 printf("%d\n",PrintWild*(sizeof(LineCodeTab)/sizeof(int)));
	//		Code128Return( 1,(PrintWild*(sizeof(LineCodeTab)/sizeof(int)));	//黑条数量
	Code128Return( 1,RightOffset);
	us_delay(1000);									 
	Code128Return( 1,0x00);					 
	us_delay(1000);
	for(i=0;i<RightOffset;i++)
	{				
		Code128Return( 1,0x00);	//白条数量
		us_delay(2000);				 
	}
	//	 printf("\n");
#endif
	for(i=0;i<strlen(WillprintNum);i++)
	{
	//	printf("%c",*(WillprintNum+i)); //zcy
	}
	//		printf("ok\n");
	//printf("\n");	 //zcy
}
unsigned char CalculateCenter(unsigned char Offset)		//居中起始点计算 函数
{
	unsigned char i=0;

	if(!(Offset%2)) 
	{
		Offset=Offset/2;
	}
	else					  
	{
		Offset=(Offset - _1Dot)/2;
	}
	
	return Offset;
	
}
unsigned char CalculateCharLocation(void)		//居中点计算 函数
{
	unsigned char i;
	i=VerticalLineNum()-strlen(WillprintNum)*4;
	
	if(!(i%2)) i=i/2;
	else
	{
		i=(i-_1Dot * 3)/2;
	}
	
	
	return i;
	
}
#if (POS_TYPE != POS_APE4000RG)
void PrintLineCodeImg(unsigned int LineHigh, unsigned int PrintTimes, int Offset)
{
	unsigned int i,j,n,m;
	unsigned int print_times,line_high,printwild;			
	CreateRightCode();

	for(line_high=0;line_high<LineHigh;line_high++)
	{
		for(print_times=0;print_times<PrintTimes;print_times++)				  
		{			 
			SetPrintLocation(Offset);			
			Code128Return( 1,0x1B);								
			us_delay(1000);
			Code128Return( 1,0x2B);			
			us_delay(1000);
			Code128Return( 1,0x01);			
			us_delay(1000);
			Code128Return( 1,VerticalLineNum());
			//	Code128Return( 1,(VerticalLineNum());
			us_delay(1000);
			Code128Return( 1,0x00);
			us_delay(1000);
			//	for(i=0;i<(sizeof(LineCodeTab)/sizeof(int));i++)
			for(i=0;i<((((strlen(WillprintNum))/2)+2)*6+7);i++)
			{
#if 1										 
				if(i%2==0)	   //												  
				{
					//	for(printwild=0;printwild<linecodetab[i];printwild++)
					for(m=0;m<linecodetab[i];m++)
					{ // CommonSleep(10);
						Code128Return( 1,0xff);	//黑条数量
						//	printf("%x",linecodetab[i]);
						us_delay(2000);	
					}
				}
				else
				{
					
					for(n=0;n<linecodetab[i];n++)
					{ 
						Code128Return( 1,0x00);	//黑条数量
						us_delay(2000);		
					}
				}
#endif
#if 0
				
				if(LineCodeTab[i]==1)
				{
					for(printwild=0;printwild<PrintWild;printwild++)
					{
						Code128Return( 1,0xff);	//黑条数量
						us_delay(1500);	
					}
				}  
				
				else
				{
					
					for(printwild=0;printwild<PrintWild;printwild++)
					{
						Code128Return( 1,0x00);	//黑条数量
						us_delay(1500);	
					} 
					
				}
#endif
			}
			
			Code128Return( 1,0x1B);
		//	Code128Return(1,0x1B);
			us_delay(1000);
			Code128Return( 1,0x4A);
			us_delay(1000);
			Code128Return( 1,0x01);
			us_delay(1000);		
		}
		
		//		us_delay(1000);
		
		while(GetPrinterStatus());
		while(GetPrinterStatus());
		while(GetPrinterStatus());
		Code128Return( 1,0x1B);
		us_delay(1000);
		Code128Return( 1,0x4a);
		us_delay(1000);
		Code128Return( 1,0x0c);
		us_delay(1000);		
	}	
	
}

void Print_Con(unsigned char PrintHigh,unsigned char Printtimes, int PrintOffset)
{
	unsigned char i;
	
	for(i=0;i<PrintHigh;i++)
	{
		while(GetPrinterStatus());
		while(GetPrinterStatus());
		while(GetPrinterStatus());
		//	PrintLineCodeImg(1,Printtimes,CalculateCenter(PrintOffset+(167-VerticalLineNum())/2));
		PrintLineCodeImg(1,Printtimes,PrintOffset);
		//	PrintLineCodeImg(1,Printtimes,0);
		us_delay(20000);
	
		//	S_delay(1);	
	}
	Code128Return( 1,0x1B);
	us_delay(1000);
	Code128Return( 1,0x4a);
	us_delay(1000);
	Code128Return( 1,0x06);
	us_delay(1000);
	
	
/*	PrintOffset = (PrintOffset+(167-VerticalLineNum())/2)/2 + CalculateCharLocation();
	
	Code128Return( 1,0x1B);
	us_delay(1000);
	us_delay(1000);
	Code128Return( 1,0x4a);
	us_delay(1000);
	Code128Return( 1,0x08);
	us_delay(1000);	
	//SetPrintCharLocation(PrintOffset);*/
	
	Code128Return( 1,0x1B);
	us_delay(1000);
	Code128Return( 1,0x4a);
	us_delay(1000);
	Code128Return( 1,0x04);
	us_delay(1000);	
	
}
void PrintLineCode(const unsigned char *chBuf, int prinset)
{
	//unsigned char printOff = 0;
	//unsigned char ywmOffset = 0;
	/*
	if ( (prinset & 0x01)==0x01 )
	{}
	*/
		CodeTwo(2, 0x1B,0x2E);
		CommonSleep(1);
		WillprintNum= chBuf;

		//打印一维条形码,根据模板计算偏移量
		/*
		if ( ((prinset>>2) | 0x00)==0x00 )//居左打印
		{
			printOff = 10;
			ywmOffset = 5;
		}
		else if (((prinset>>2) & 0x01)==0x01)//居中打印
		{
			printOff = CalculateCenter( _MAX_DOT - VerticalLineNum());
			ywmOffset = CalculateCenter(46 - strlen(WillprintNum));//映美打印机一行最多打印42个西文字符
		}
		else if (((prinset>>2) & 0x02)==0x02)//居右打印
		{
			printOff = _MAX_DOT - VerticalLineNum() - 10;
			ywmOffset = 46 - strlen(WillprintNum) - 3;
		}
		else{}
		*/
		Print_Con(3,2,prinset);	   //打印高度，打印次数，偏移量
		/*
		if ( ((prinset>>1) & 0x01)==0x01 )
		{
			us_delay(5000);
			PrintYwtm(chBuf, ywmOffset);//打印数字校验码
		}
		*/
		us_delay(3000);
		CommonSleep(1);
		CodeTwo(2, 0x1B,0x2F);
		//ForwardNPoint(20);
}

#else

void PrintLineCodeImg(unsigned int LineHigh, unsigned int PrintTimes, int Offset)
{
	unsigned int i,j,n,m;
	unsigned int print_times,line_high,printwild;			

	for(line_high=0;line_high<LineHigh;line_high++)
	{
		for(print_times=0;print_times<PrintTimes;print_times++)				  
		{			 
			Code128Return( 1,0x1b);								
			us_delay(1000);
			Code128Return( 1,0x2a);			
			us_delay(1000);
			Code128Return( 1,0x00);			
			us_delay(1000);
			Code128Return( 1,VerticalLineNum()+Offset);
			us_delay(1000);
			Code128Return( 1,0x00);
			us_delay(1000);
			for(i=0;i<Offset;i++)
			{				
				Code128Return( 1,0x00);	//头部静止区对应的条形码,Offset个点距	
				us_delay(2000);					 
			}
			for(i=0;i<((((strlen(WillprintNum))/2)+2)*6+7);i++)
			{
#if 1										 
				if(i%2==0)											  
				{
					for(m=0;m<linecodetab[i];m++)
					{ 
						Code128Return( 1,0xff);	//黑条数量
						us_delay(2000);	
					}
				}
				else
				{
					
					for(n=0;n<linecodetab[i];n++)
					{ 
						Code128Return( 1,0x00);	//空白条数量
						us_delay(2000);		
					}
				}
#endif
			}
			
			Code128Return( 1,0x1b);
			us_delay(1000);
			Code128Return( 1,0x4a);
			us_delay(1000);
			Code128Return( 1,0x01);
			us_delay(1000);		
		}
		//printf("\n**************GetPrinterStatus_check**************\n");		
		//while(GetPrinterStatus());
		//while(GetPrinterStatus());
		//while(GetPrinterStatus());
		Code128Return( 1,0x1b);
		us_delay(1000);
		Code128Return( 1,0x4a);
		us_delay(1000);
		Code128Return( 1,0x0c);
		us_delay(1000);	
	}		
}

void Print_Con(unsigned char PrintHigh,unsigned char Printtimes, int PrintOffset)
{
	unsigned char i=0;

	CreateRightCode();
	//PrintOffset = PrintOffset-(VerticalLineNum()+15);
	
	for(i=0;i<PrintHigh;i++)//打印高度
	{
		while(GetPrinterStatus());
		//while(GetPrinterStatus());
		//while(GetPrinterStatus());
		//PrintLineCodeImg(1,Printtimes,CalculateCenter(PrintOffset+(167-((strlen(WillprintNum)+2)*11+13)/2)/2));
		PrintLineCodeImg(1,Printtimes,PrintOffset);

		us_delay(2000);
	}
	Code128Return( 1,0x1B);
	us_delay(1000);
	Code128Return( 1,0x4a);//打印机进纸指令
	us_delay(1000);
	Code128Return( 1,0x06);
	us_delay(1000);
	
	
	Code128Return( 1,0x1B);
	us_delay(1000);
	Code128Return( 1,0x4a);
	us_delay(1000);
	Code128Return( 1,0x04);
	us_delay(1000);	
	
}
void PrintLineCode(const unsigned char *chBuf, int prinset)
{
	//unsigned char printOff = 0;
	//unsigned char ywmOffset = 0;
	/*
	if ( (prinset & 0x01)==0x01 )
	{}
	*/
		CommonSleep(1);
		
		WillprintNum= chBuf;

		//打印一维条形码
		/*
		if ( ((prinset>>2) | 0x00)==0x00 )//居左打印
		{
			//printOff = 10;
			//ywmOffset = 5;
		}
		else if (((prinset>>2) & 0x01)==0x01)//居中打印
		{
			//printOff = CalculateCenter( _MAX_DOT - VerticalLineNum());
			//ywmOffset = CalculateCenter(42 - strlen(WillprintNum));//映美打印机一行最多打印42个西文字符
		}
		else if (((prinset>>2) & 0x02)==0x02)//居右打印
		{
			//printOff = _MAX_DOT - VerticalLineNum() - 10;
			//ywmOffset = 42 - strlen(WillprintNum) - 3;
		}
		else{}
		*/
		
		Print_Con(3,2,prinset);	   //打印高度，打印次数，偏移量
		/*
		if ( ((prinset>>1) & 0x01)==0x01 )
		{
			us_delay(5000);
			PrintYwtm(chBuf, ywmOffset);//打印数字校验码
		}
		*/
		us_delay(3000);
		CommonSleep(1);
		CarriageReturn();
		LineFormard();	
		//ForwardNPoint(20);

}

#endif

void us_delay(unsigned int us)	//1 us delay
{
	//	DrvSYS_RoughDelay(us);
	if(us ==0)
	{
		return;
	}
	unsigned int i,j;
	for(j=12;j>0;j--)
	{
		for(i=us;i>0;i--);	  
	}
	
}





























