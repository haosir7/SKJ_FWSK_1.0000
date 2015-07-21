
/*! \file   USBMount.c
   \brief   USB½Ó¿ÚÇý¶¯
   \author  zl
*/
//#include "IncludeMe.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "USBMount.h"
#include "comdatatype.h"


unsigned char UsbDiskMount(char* dev)
{
	system("umount /mnt");
	system("mkdir /mnt");
	char str[64];
	int k=0,m=0;
	
	if (strlen(dev) < 1)
	{
		for( k=0; k<4; k++)
		{
			for( m=0; m<2; m++)
			{
				memset(str, 0, sizeof(str));
				sprintf(str, "mount -t vfat /dev/sd%c%c /mnt", "abcd"[k],"123"[m]);
				if(system(str) == 0)
				{
					return SUCCESS;
				}
			}
			
			memset(str, 0, sizeof(str));
			sprintf(str, "mount -t vfat /dev/sd%c /mnt","abcd"[k]);
			if(system(str) == 0)
			{
				return SUCCESS;
			}
		}	
	}
	else
	{
		
		sprintf(str, "mount %s /mnt", dev);
		if(system(str) == 0)
		{
			return SUCCESS;
		}
	}

	return FAILURE;
}


unsigned char UsbDiskUnMount()
{
	int res;
	res = system("umount /mnt");
	if (res)
		return FAILURE;
	else
		return SUCCESS;
}


unsigned char UsbDiskMount2(char* dev)
{
//	char *strMounta = "mount -t vfat /dev/sd%c%c /mnt";
//	char *strMountb = "mount -t vfat /dev/sd%c /mnt";
// 	char *strMountc = "mount %s /mnt";

	char strMounta[64] = "mount -t vfat /dev/sd%c%c /mnt";
	char strMountb[64] = "mount -t vfat /dev/sd%c /mnt";
	char strMountc[64] = "mount %s /mnt";
	
	int nUsbNum = 0;

	system("umount /mnt");
	system("mkdir /mnt");

	system("/rw");
	system("umount /mnt1");
	system("mkdir /mnt1");
	char str[64];
	int k=0,m=0;
	
	if (strlen(dev) < 1)
	{
		for(k=0; k<4; k++)
		{
			for(m=0; m<2; m++)
			{
				memset(str, 0, sizeof(str));
			//	sprintf(str, "mount -t vfat /dev/sd%c%c /mnt1", "abcd"[k],"123"[m]);
				sprintf(str, strMounta, "abcd"[k],"123"[m]);
				if(system(str) == 0)
				{
//					strMounta += "1";
//					strMountb += "1";
//					strMountc += "1";
					strcat(strMounta,"1");
					strcat(strMountb,"1");
					strcat(strMountc,"1");
					nUsbNum++;
					if (2 == nUsbNum)
					{
						return SUCCESS;
					}
				//	break;					
				}
			}
			
			memset(str, 0, sizeof(str));
		//	sprintf(str, "mount -t vfat /dev/sd%c /mnt1","bcd"[k]);
			sprintf(str, strMountb,"abcd"[k]);
		
			if(system(str) == 0)
			{
//					strMounta += "1";
//					strMountb += "1";
//					strMountc += "1";
					strcat(strMounta,"1");
					strcat(strMountb,"1");
					strcat(strMountc,"1");
				nUsbNum++;
				if (2 == nUsbNum)
				{
					return SUCCESS;
				}
			//	break;	
			}
		}	
	}
	else
	{
		
	//	sprintf(str, "mount %s /mnt1", dev);
		sprintf(str, strMountc, dev);
	
		if(system(str) == 0)
		{
//					strMounta += "1";
//					strMountb += "1";
//					strMountc += "1";
					strcat(strMounta,"1");
					strcat(strMountb,"1");
					strcat(strMountc,"1");
			nUsbNum++;
			if (2 == nUsbNum)
			{
				return SUCCESS;
			}	
		}
	}

	return FAILURE;
}


unsigned char UsbDiskUnMount2()
{
	int res;

	res = system("umount /mnt");
	if (res)
	{
		return FAILURE;
	}

	res = system("umount /mnt1");
	if (res)
	{
		return FAILURE;
	}
	
	return SUCCESS;
}
