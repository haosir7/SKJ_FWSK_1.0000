/****************************************************************************
文件名           ：PowOffData.h
功能             ：awe4000r掉电保护所用的结构体。
起始日期         ：2008-03-15
作者             ：Yu Yan     
****************************************************************************/


#ifndef POWER_OFF_DATA_H
#define POWER_OFF_DATA_H

#include <stdlib.h>
#include <signal.h> 
#include <errno.h> 
 
#ifndef WIN32
#include  <stdio.h>
#include  <sys/ioctl.h>
#include  <sys/timeb.h>
#include  <unistd.h>
#include  <fcntl.h>
#include  <termios.h>
#endif
#include "comdatatype.h"
#include "SysMacDef.h"
#include "powoff_protect.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define INIT_PROC 			0x01 //初始化过程
#define FPFF_PROC 			0x02 //发票分发过程
#define FPDR_PROC 			0x03 //发票导入过程
#define FPTK_NORMAL_PROC 	0x04 //正票填开过程
#define FPTK_RETURN_PROC 	0x05 //红票填开过程
#define FPTK_WASTE_PROC 	0x06 //废票填开过程
#define SB_PROC   			0x07 //申报过程 
#define WS_PROC   			0x08 //完税过程
#define INV_ROLL_PROC 		0x09 //发票滚动过程



enum CHS_CHILD_PROC{UPDATE_INT_INFO=1};
enum FPFF_CHILD_PROC{UPDATE_INV_INFO=1, DISTRIBUTE_INV_IC};
enum FPTK_CHILD_PROC{WRITE_ICCARD=1, WRITE_INV,PRINT_INV};
enum SB_CHILD_PROC{UPDATE_DEC_INFO=1};
enum WS_CHILD_PROC{UPDATE_TAX_PAY_INFO=1};



#define MAX_FPJ_LEN 1 			//发票分发掉电保护的最大发票卷数
#define MAX_BLK_INV_LEN 4 		//空白发票作废税控码保护最大份数
#define MAX_INVOICE_DETAILS_NUM        	16   	/**<支持最大商品行数*/



/**
 *@struct TInvHeadInfo 
 *@brief 发票头信息
 */
struct TInvHeadInfo
{
	//UINT32  TypeCodeNo;                       /**<发票代码关联号 */
	INT8	TypeCodeNo[21];						/**<发票代码 *///xsr
	UINT32  InvNo;                             	/**<发票号码     */
	UINT8   MachineNo;                         	/**<开票机号 HEX  */
	UINT8   Type;                              	/**<开票类型      */
	                                           	/**< 01：正票  02：红票   03：废票 */
	UINT32  Date;                              	/**<开票日期 CCYYMMDD  */
	UINT32  Time;                              	/**<开票时间 hhmmss    */
	INT64   TotalSum;                          	/**<开票总金额 单位：分  */
	UINT32  PosInvNo;                          	/**<原发票号码           */
	UINT8   ClientName[FKDW_NAME_LEN+1];       	/**<付款人               */
	UINT8   OperatorName[OPERATOR_NAME_LEN+1]; 	/**<开票员编号HEX        */
	UINT8   OpId; 								/**<开票员编号ID       */
	UINT8   TaxCtrCode[INV_TAX_CTRL_CODE_LEN]; 	/**<税控码 HEX            */
	UINT8   PayMode;                           	/**<付款方式             
	                                           	01－现金，02－信用卡，03－支票 
	                                           	04－外汇，05－签单     */
	UINT8   TableID;                           	/**<台号 HEX 用于餐饮      */
	UINT16  detailCount;					   	/**<商品行个数        	*/ 
	float   sl;									//税率
	float   zsl;								//征收率
	UINT16  pzlb;								//票种类别
//	UINT8   fplb;								//发票类别
	UINT8   scbz;								//上传标志
	UINT16  dycs;								//打印次数                                  	
};


/**
 *@struct TInvDetailInfo 
 *@brief 发票商品明细信息
 */
struct TInvDetailInfo
{
	UINT8   Property;                      		/**<明细行性质 HEX             */
	                                       		/**<1-一般商品行，2-折扣行 5-折让      
	                                       		    3-带折扣的商品行,4-服务费行*/ 
	UINT8   GoodsCode[GOODS_CODE_LEN];     		/**<商品编码                   */
	UINT8   GoodsName[GOODS_NAME_LEN+1];   		/**<商品名称                   */ 
	float   TaxItem;							/**<税率			           *///xsr
	float   zsl;								//征收率
	double  Amount;                        		/**<商品数量                   */
	double  Price;                         		/**<单价                       */
	INT64   Sum;                           		/**<金额                       */
	INT64   TaxSum;                           		/**<税额                       */
//	UINT32  rebate;                             /**<商品折扣                   */  
};  


/**
 *@struct TInvFullInfo 
 *@brief 发票信息
 */
struct TInvFullInfo                            
{			  
	struct TInvHeadInfo	head;								/**< 发票头*/					  
	struct TInvDetailInfo  detail[MAX_INVOICE_DETAILS_NUM];	/**< 明细 */		
												                      
};


/**
 *@struct TInvVolInfo 
 *@brief 发票领用信息
 */
struct TInvVolInfo
{
	//UINT8   TypeCodeNo; 						/**<发票代码关联号 */
	INT8	TypeCodeNo[20];						/**<发票代码		*/
	UINT32  StartNo;                     		/**<起始发票号码  */      
	UINT32  EndNo;                       		/**<中止发票号码  */     
	UINT32  RemainAmount;                		/**<剩余份数      */      
	UINT8   Mac[INV_VOL_MAC_LEN];        		/**<MAC  BCD      */                
	UINT8   UsingFalg;                   		/**<0－导入标志   */      
	UINT8   OverFlag;                       	/**<0-用完标志    */                                  	
};


/*  初始化过程掉电保护数据  */
typedef struct TCSHPwOffdata 
{
//	BaseInfo_f baseInfo_f;
  	unsigned char pin_buf[FCARD_PWD_LEN];             		/*初始化税控卡初始PIN码	*/                                                     
} TCSHPwOffdata; 

/*  发票分发过程掉电保护数据  */
typedef struct TFPFFPwOffData
{
	unsigned int fpjs;                                  	/*发票卷数            	*/
//	InvoiceInfoPer_u fpffmx[MAX_FPJ_LEN];               	/*发票卷信息序列      	*/
} TFPFFPwOffData;

/*  发票导入过程掉电保护数据  */
typedef struct TFPDRPwOffData
{
	struct TInvVolInfo curInvVol;                        /*当前发票卷信息     	*/
//	CInvVol curInvVol;                     		
} TFPDRPwOffData;


/*  发票填开过程掉电保护数据  */
typedef struct TFPTKPwOffData
 {	
	UINT8 wasteCount;
    struct TInvFullInfo posInvInfo;						//正常票信息
 //	struct TReInvInfo reInvInfo; 
 //	struct TCurInvVolInfo curInvVol; 					//当前发票卷信息
 	UINT8  RemainLineCount;                             //掉电剩余打印行个数
	UINT8  BeginPrintFlag;                                   //标记掉电时是否已经开始打印数据
 } TFPTKPwOffData;

/*  申报过程掉电保护数据  */
typedef struct TSBPwOffData
{
	UINT8 reDeclareFlag;
	UINT32 declareDate;
//   	DeclareDutyData decDutyData; 							//申报信息
} TSBPwOffData; 

/*  发票存储区滚动过程保护数据  */
typedef struct
{
	UINT32 invDatelimit;								/**<InvHead表滚动删除的截止日期  */   	
	UINT32 sumDatelimit;								/**<InvSum表滚动删除的截止日期  */   	
	UINT32 rtNolimit;									/**<RtInv表滚动删除的截止号码  */ 
	UINT32 nRefDate;                                    /**<上次申报的截止日期  */ 
}InvRollPwOffData;


/*  业务层掉电掉电保护数据  */
typedef struct TYWPwOffData
{
	unsigned char 	pw_off_flg; 							//0000 0000 最低位为1表示掉电保护标志 为0表示正常
	unsigned char 	main_proc_id; 							//主过程号 
	unsigned char 	child_proc_id; 							//子过程号
	unsigned int 	pwoffdate; 								//掉电日期
	unsigned int 	pwofftime; 								//掉电时间
	unsigned char 	logflag; 								//登录标志
	unsigned char 	invoiceException;						//发票分发异常标志
	unsigned char 	declareException;						//申报异常标志
	unsigned char 	machineCancel;							//停机标志

//	TaxItemIndex_u taxItems[TAX_ITEM_RECORD_NUM];
//	SuperviseData_f spvData_f;
	InvRollPwOffData invRollPwOffData;						//停机标志

// 	struct TOperatorInfo operator;

	struct _ProcData
	{
	  struct TCSHPwOffdata    init_data; 
	  struct TFPFFPwOffData   fpff_data;
	  struct TFPDRPwOffData   fpdr_data;
	  struct TFPTKPwOffData   fptk_data;             // 发票填开过程掉电保护数据
	  struct TSBPwOffData     sb_data;
	  
	  
	}ProcData;
  
}TYWPwOffData; 


//取掉电标志
//#define GetPowerOffFlag() g_YW_PowerOffData->pw_off_flg & 0x01
#define GetPowerOffFlag() g_YW_PowerOffData->pw_off_flg


//清除掉电标志
#define CleanPowerOffFlag() { g_YW_PowerOffData->pw_off_flg = 0; g_YW_PowerOffData->main_proc_id = 0 ; g_YW_PowerOffData->child_proc_id = 0 ;}


//设置掉电标志
#define SetPowerOffFlag() { g_YW_PowerOffData->pw_off_flg = 0x01 ;}


//设置主过程号
#define SetPowerOffProcID_M(x) { g_YW_PowerOffData->main_proc_id = x ;}

//设置子过程号 
#define SetPowerOffProcID_C(x) { g_YW_PowerOffData->child_proc_id = x ; }

//取主过程号
#define GetPowerOffProcID_M(x) { x = g_YW_PowerOffData->main_proc_id  ;}

//取子过程号 
#define GetPowerOffProcID_C(x) { x = g_YW_PowerOffData->child_proc_id  ; }



extern struct TYWPwOffData *g_YW_PowerOffData;


void PowOff_init();
void PowOff_release();


#ifdef __cplusplus
}
#endif

#endif
