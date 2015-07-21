#include "pinyin.h"
#include "stdio.h"
#include <fcntl.h>

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define GB18030_LIB

#ifdef WIN32
FILE *pinyin_fd = NULL;
#else
static int pinyin_fd = 0;
#endif

//const gen_inp_conf_t mycf={ic1,icidx};

#define N_KEYS  47
#define N_KEY_IN_CODE 5		/* Number of keys in code. */

static char keymap[128];
static wch_t mcch[500];		//需大于最大同音字数


unsigned char Readicidx_2(unsigned char *pDes,unsigned int offset)
{
	unsigned char temp[2];

#ifdef WIN32
	if (pinyin_fd == NULL)
	{
		pinyin_fd = fopen("./LIB/pinyin_gb18030.bin", "rb");
	}
	fseek(pinyin_fd, offset*2, SEEK_SET);
	fread(temp, 1, 2, pinyin_fd);
#else
	if (pinyin_fd <= 0)
	{
		pinyin_fd = open("/usr/ziku/pinyin_gb18030.bin", O_RDONLY);
	}
	
	lseek(pinyin_fd, offset*2, SEEK_SET);
	read(pinyin_fd, temp, 2);
#endif
    *pDes = temp[1];
    *(pDes+1) = temp[0];

	return 0;
}

unsigned char Readic1_num_4(unsigned char *pDes,unsigned int offset)
{
	unsigned char temp[4];
#ifdef WIN32
	if (pinyin_fd == NULL)
	{
		pinyin_fd = fopen("./LIB/pinyin_gb18030.bin", "rb");
	}
	fseek(pinyin_fd, 0xe400+offset*4, SEEK_SET);
	fread(temp, 1, 4, pinyin_fd);
#else
	if (pinyin_fd <= 0)
	{
		pinyin_fd = open("/usr/ziku/pinyin_gb18030.bin", O_RDONLY);
	}
	
	lseek(pinyin_fd, 0xe400+offset*4, SEEK_SET);
	read(pinyin_fd, temp, 4);
#endif

	*pDes = temp[3];
	*(pDes+1) = temp[2];     
	*(pDes+2) = temp[1];
	*(pDes+3) = temp[0];

	return 0;
}
unsigned char Readic1_4(unsigned char *pDes,unsigned int offset)
{
	unsigned char temp[4];
#ifdef WIN32
	if (pinyin_fd == NULL)
	{
		pinyin_fd = fopen("./LIB/pinyin_gb18030.bin", "rb");
	}
	fseek(pinyin_fd, 0xdc00+offset*4, SEEK_SET);
	fread(temp, 1, 4, pinyin_fd);
#else
	if (pinyin_fd <= 0)
	{
		pinyin_fd = open("/usr/ziku/pinyin_gb18030.bin", O_RDONLY);
	}
	
	lseek(pinyin_fd, 0xdc00+offset*4, SEEK_SET);
	read(pinyin_fd, temp, 4);
#endif

	*pDes = temp[3];
	*(pDes+1) = temp[2];     
	*(pDes+2) = temp[1];
	*(pDes+3) = temp[0];

	return 0;
}

static void keymap_init(void)
{
    keymap['1'] = 1;
    keymap['2'] = 2;
    keymap['3'] = 3;
    keymap['4'] = 4;
    keymap['5'] = 5;
    keymap['6'] = 6;
    keymap['7'] = 7;
    keymap['8'] = 8;
    keymap['9'] = 9;
    keymap['0'] = 10;

    keymap['!'] = 1;
    keymap['@'] = 2;
    keymap['#'] = 3;
    keymap['$'] = 4;
    keymap['%'] = 5;
    keymap['^'] = 6;
    keymap['&'] = 7;
    keymap['*'] = 8;
    keymap['('] = 9;
    keymap[')'] = 10;

    keymap['a'] = 11;
    keymap['b'] = 12;
    keymap['c'] = 13;
    keymap['d'] = 14;
    keymap['e'] = 15;
    keymap['f'] = 16;
    keymap['g'] = 17;
    keymap['h'] = 18;
    keymap['i'] = 19;
    keymap['j'] = 20;
    keymap['k'] = 21;
    keymap['l'] = 22;
    keymap['m'] = 23;
    keymap['n'] = 24;
    keymap['o'] = 25;
    keymap['p'] = 26;
    keymap['q'] = 27;
    keymap['r'] = 28;
    keymap['s'] = 29;
    keymap['t'] = 30;
    keymap['u'] = 31;
    keymap['v'] = 32;
    keymap['w'] = 33;
    keymap['x'] = 34;
    keymap['y'] = 35;
    keymap['z'] = 36;

    keymap['A'] = 11;
    keymap['B'] = 12;
    keymap['C'] = 13;
    keymap['D'] = 14;
    keymap['E'] = 15;
    keymap['F'] = 16;
    keymap['G'] = 17;
    keymap['H'] = 18;
    keymap['I'] = 19;
    keymap['J'] = 20;
    keymap['K'] = 21;
    keymap['L'] = 22;
    keymap['M'] = 23;
    keymap['N'] = 24;
    keymap['O'] = 25;
    keymap['P'] = 26;
    keymap['Q'] = 27;
    keymap['R'] = 28;
    keymap['S'] = 29;
    keymap['T'] = 30;
    keymap['U'] = 31;
    keymap['V'] = 32;
    keymap['W'] = 33;
    keymap['X'] = 34;
    keymap['Y'] = 35;
    keymap['Z'] = 36;

    keymap['`'] = 37;
    keymap['-'] = 38;
    keymap['='] = 39;
    keymap['['] = 40;
    keymap[']'] = 41;
    keymap[';'] = 42;
    keymap['\''] = 43;
    keymap[','] = 44;
    keymap['.'] = 45;
    keymap['/'] = 46;
    keymap['\\'] = 47;

    keymap['~'] = 37;
    keymap['_'] = 38;
    keymap['+'] = 39;
    keymap['{'] = 40;
    keymap['}'] = 41;
    keymap[':'] = 42;
    keymap['\"'] = 43;
    keymap['<'] = 44;
    keymap['>'] = 45;
    keymap['?'] = 46;
    keymap['|'] = 47;
}


int keys2codes(unsigned int *klist, int klist_size, char *keystroke)
{
    int i, j;
    unsigned int k, *kk=klist;
    char *kc = keystroke;

    if (! keymap['1'])
	keymap_init();

    *kk = 0;
    *(kk+1) = 0;
    for (i=0, j=0; *kc; i++, kc++) 
    {
		k = keymap[(int) *kc];
		*kk |= (k << (24 - (i - j*N_KEY_IN_CODE) * 6));
		if (i % N_KEY_IN_CODE == N_KEY_IN_CODE-1) 
		{
			if (++j >= klist_size)
			break;
			kk++;
			*kk = 0;
		}        
    }
    return j;
}

static int cmp_icvalue(unsigned int idx, icode_t icode1)
{
	unsigned char ic1_temp[4];
	int ic1_value;
	
	Readic1_4(ic1_temp, idx);
	ic1_value = (int)ic1_temp[0]<<24 | (int)ic1_temp[1]<<16
				 | (int)ic1_temp[2]<<8 | (int)ic1_temp[3];
    if (ic1_value > icode1)
	{	
		return 1;
	}
    else if (ic1_value < icode1)
	{
		return -1;
	}
    else 
	{
		return 0;
	}
}

static int bsearch_char(icode_t icode1, icode_t icode2, int size, int *homophone_flag)
{
    int head, middle, end, ret;
	unsigned char homophone_temp[4], middle_temp[4];
	
    head   = 0;
    middle = size / 2;
    end    = size;
    while ( ( ret = cmp_icvalue(middle, icode1) ) ) 
	{
        if (ret > 0)
            end = middle;
        else
            head = middle + 1;
        middle = (end + head) / 2;
        if (middle == head && middle == end)
            break;
    }
    if (ret == 0) 
	{

/*		while(middle > 0 && ! cmp_icvalue(ic1, middle-1, icode1))          remark by zhanglei
			middle --;
		return middle;
*/
		Readic1_num_4(homophone_temp, middle+1);
		*homophone_flag = (int)homophone_temp[2]<<8 | (int)homophone_temp[3];
		Readic1_num_4(middle_temp, middle);
		middle = (int)middle_temp[2]<<8 | (int)middle_temp[3];
		
/**（临时）补丁代码：使六拼音的字可正常查找，该疏漏是早期代码考虑的不周。add by zhanglei**/
		if (icode2==0x11000000)
		{
#ifdef GB18030_LIB
			if (icode1==0x2449f2d8)					//zhuang
			{
				middle = middle+54;
			}
			else if (icode1==0x0d49f2d8)			//chuang
			{
				middle = middle+38;
			}
			else if (icode1==0x1d49f2d8)			//shuang
			{
				middle = middle+11;
			}
			else if (icode1==0x178cf658)			//myeong
			{
				middle = middle+1;
			}
			else if (icode1==0x1a48e3d8)			//phdeng
			{
				middle = middle;
			}
			else 
			{
				*homophone_flag = 0;
				return -1;
			}
#endif

#ifdef GB2312_LIB
			if (icode1==0x2449f2d8)					//zhuang
			{
				middle = middle+11;
			}
			else if (icode1==0x0d49f2d8)			//chuang
			{
				middle = middle+13;
			}
			else if (icode1==0x1d49f2d8)			//shuang
			{
				middle = middle+4;
			}
			else 
			{
				*homophone_flag = 0;
				return -1;
			}
#endif
		}
		else if (icode2!=0x0)
		{
			*homophone_flag = 0;
			return -1;
		}
/***********************（临时）补丁代码结束***************************************/

		return middle;
    }
    else
		return -1;	
}

int py_init(void)
{

	return 0;
}


int match_keystroke_normal(unsigned char *inpinfo, unsigned int inpinfo_size, wchar_t *outbuff, unsigned int *outsize)
{
//    const gen_inp_conf_t *cf=&mycf;
    unsigned char inbuf[MAX_KEYSTROKE_LEN];
    icode_t icode[4];
    unsigned int size, md, n_ich, mcch_size;
    int idx;
    unsigned int i;
    int end_pos;//,start_pos;
    char tmp;
    int keylen;
    int homophone_flag;
    
	//mcch=&outbuff;
    for (i=0;(i<inpinfo_size /*& i<(MAX_KEYSTROKE_LEN-1)*/);i++)
    {
    	inbuf[i]=inpinfo[i];
    }
    //md = (cf->header.icode_mode == ICODE_MODE2) ? 1 : 0;//md=1
	md=0;

    n_ich = 0;

    mcch_size =0xa; 			//inpinfo->n_selkey;
    //mcch = (wch_t *)malloc(mcch_size * sizeof(wch_t));
		
    for (end_pos=min(MAX_USER_KEYSTROKE_LEN,inpinfo_size); end_pos>=0; end_pos=-1/*end_pos--*/) 
    {	
		keylen = end_pos;
		if (keylen > MAX_KEYSTROKE_LEN) continue; /*not for sys char and phr, jimc*/
			icode[0] = icode[1] = icode[2] = icode[3] = 0;
		/* the following's indents are wrong, I just leave it, jimc */
		tmp = inbuf[end_pos];
		inbuf[end_pos] = '\0';
		
  
		keys2codes(icode, 2, ((char *)(&inbuf[0])) );
	//size = cf->header.n_icode;  
//		size  = 0x1df0;
		size = PINYIN_SIZE;					//chang by zhanglei

		if ((idx = bsearch_char(icode[0], icode[1], size, &homophone_flag)) != -1)
		{
//       		d_printf("icode1= %x,%x,%x  %d mode=%d\r\n",icode[0],icode[1],icode[2],idx,md);
        	do 
        	{   
		    	Readicidx_2(mcch[n_ich].s, idx);

		    	n_ich ++;
	            idx ++;
       		} while (idx < HANZI_SIZE && idx < homophone_flag);
// 		    d_printf("iiiiixxxxx %d , %x, %d\r\n",n_ich,mcch[n_ich].wch,mcch_size);
   		}
    	inbuf[end_pos] = tmp;
    }
     /*
     *  Prepare mcch for display.
     */
    for (i=0;i<n_ich;i++)
    {
    	outbuff[i]=mcch[i].wch;    		
    }
	*outsize=n_ich;

    return 1;
}







