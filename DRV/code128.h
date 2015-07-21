#ifndef CODE_128_H_
#define CODE_128_H_

#ifdef __cplusplus
extern "C"
{
#endif

	unsigned char CalculateCenter(unsigned char Offset)	;
unsigned char CalculateCharLocation(void);
unsigned char CalculateCheckCode();

void CreateRightCode(void);

void CreateRightCodeArray(void);


void Print_Con(unsigned char PrintHigh,unsigned char Printtimes, unsigned char PrintOffset);


void PrintLineCode(const unsigned char *chBuf);

void PrintLineCodeImg(unsigned int LineHigh , unsigned int PrintTimes,unsigned char Offset);

void SetPrintCharLocation(unsigned char RightOffset);
void SetPrintLocation(unsigned char RightOffset) ;
unsigned char  VerticalLineNum(void);
void us_delay(unsigned int us);




extern const unsigned int code128[106][6];
extern const unsigned char *WillprintNum;
extern unsigned int a[24];
extern unsigned char LineCodeTab[91];
extern unsigned char linecodetab[91];

#ifdef __cplusplus
}
#endif

#endif