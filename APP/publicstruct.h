#ifndef             PUBLICSTRUCT_H
#define             PUBLICSTRUCT_H

#include "datatype.h"

#define    OUT
#define    IN



typedef          void         *HDISK;



typedef struct  DEVINFO_T
{
	PVOID				m_pDev;                
	int				m_Handle;              
	CHAR				m_cVidName[8+1];
	CHAR				m_cPidName[16+1];
	CHAR				m_cFirmwareVer[4+1];
	CHAR				m_cDriveLetter;       
	UINT32				m_wVid;
	UINT32				m_wPid;
	UINT32				m_dwDeviceError;       
	UINT8				m_byDiskMode;	       
	UINT32               m_FlashNo;             
	UINT32               m_BlockOfFlash;            
	UINT32               m_SectorOfBlock;            
	UINT32               m_DiskMode;              
	UINT8                SenseBuf[32];          
	UINT8                m_Lib_Ver;               
	UINT8                reserved[128];
	BOOL                bActive;
	
} DEVINFO,*PDEVINFO;


struct ScsiInquiryResult
{
    UINT8  deviceType    :5;
    UINT8  reserved0     :3;
	
    UINT8  reserved1     :7;
    UINT8  rmb           :1;
	
    UINT8  ansiVersion   :3;
    UINT8  ecmaVersion   :3;
    UINT8  isoVesion     :2;
	
    UINT8  repDataFormat :4;
    UINT8  reserved2     :4;
	
    UINT8  additionalLen;
    UINT8  reserved3[3];
	
    UINT8  vendorInfo[8];        
    UINT8  productID[16];        
    UINT8  productRevisionlevel[4]; 
	
    UINT8  vendorSpec[20];
    UINT8  reserved4;
    UINT8  vendorSpecPara[64];
};


typedef struct  PHYDEVINFO_T
{
	PVOID				m_pDev;                
	int				m_Handle;              
	CHAR				m_cVidName[8+1];
	CHAR				m_cPidName[16+1];
	CHAR				m_cFirmwareVer[4+1];
	CHAR				m_cDriveLetter;       
	UINT32				m_wVid;
	UINT32				m_wPid;
	UINT32				m_dwDeviceError;        
	UINT8				m_byDiskMode;	       
	UINT32               m_FlashNo;             
	UINT32               m_BlockOfFlash;            
	UINT32               m_ByteOfBlock;
	UINT32               m_DiskMode;               
	UINT8                SenseBuf[32];
	BOOL                bActive;
	UINT8                reserved[127];	
} PHYDEVINFO,*PPHYDEVINFO;

#endif 
