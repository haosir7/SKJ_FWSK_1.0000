/****************************************************************************************
*文件说明: 中文输入实现文件
*使用说明:
     该文件是拼音输入法码表数据文件，输入字符集: abcdefghijklmnopqrstuvwxyz(小写)
     对于每个合法的字符串_pInput，函数unsigned char *GetPyIndex(unsigned char *_pInput)
     将返回一个汉字字符串或空串NULL;
     _pInput指向的空间要求最少6个字节空间，当输入不满6字节时，用空格' '补齐
     当输入只有一个字符时，函数返回5个以该字符声母的常用汉字(你也可以任意添加
     你想要的字符，包括其它项亦如此)；符号的输入为"zz",也可改变{_PyMa_zz,"z}   "}中的'z'
     使其满足的要求。
     _cByte可以重新定义。
作  者：  程智锋
日  期：  2004年9月27日
版  本：  0.9
-----------------------------------------------------------------------------------------
移植说明：
	为在APE系列上使用T9输入法，故将本段代码移植到ADS编译环境并做以适当删减。
修改内容：	
	删除了所有与查找汉字有关的代码，结构体定义中与汉字查找有关的数据成员也全部予以删除。
	修改了对外接口定义，只保留T9_Input一个接口函数。
作者：	张磊
日  期：  2006年7月7日
****************************************************************************************/
//#define POS_DEBUG_TO_TTY

#include "T9_pinyin.h"
#include "VersionConfig.h"

#if (T9PINYIN_SWITCH == 1)

//#define T9_DEBUG	1

//韵母数组
_cByte _YunMu_none[]="    ";
_cByte _YunMu_a   []="a   ";
_cByte _YunMu_o   []="o   ";
_cByte _YunMu_e   []="e   ";
_cByte _YunMu_ai  []="ai  ";
_cByte _YunMu_ou  []="ou  ";
_cByte _YunMu_ei  []="ei  ";
_cByte _YunMu_ao  []="ao  ";
_cByte _YunMu_an  []="an  ";
_cByte _YunMu_en  []="en  ";
_cByte _YunMu_ang []="ang ";
_cByte _YunMu_eng []="eng ";
_cByte _YunMu_ong []="ong ";
_cByte _YunMu_i   []="i   ";
_cByte _YunMu_ia  []="ia  ";
_cByte _YunMu_ie  []="ie  ";
_cByte _YunMu_iao []="iao "; 
_cByte _YunMu_iu  []="iu  ";
_cByte _YunMu_ian []="ian ";
_cByte _YunMu_in  []="in  ";
_cByte _YunMu_iang[]="iang";
_cByte _YunMu_ing []="ing ";
_cByte _YunMu_iong[]="iong";
_cByte _YunMu_u   []="u   ";
_cByte _YunMu_ua  []="ua  ";
_cByte _YunMu_ue  []="ue  ";
_cByte _YunMu_uo  []="uo  ";
_cByte _YunMu_uai []="uai ";
_cByte _YunMu_ui  []="ui  ";
_cByte _YunMu_uan []="uan ";
_cByte _YunMu_uang[]="uang";
_cByte _YunMu_un  []="un  ";
_cByte _YunMu_r   []="r   ";
_cByte _YunMu_v   []="v   ";
_cByte _YunMu_n   []="n   ";
_cByte _YunMu_ng  []="ng  ";
_cByte _YunMu_z   []="z   ";



//a开头
_PyIndexType  PyIndex_a[]=
{
	{_YunMu_none},
	{_YunMu_i   },
	{_YunMu_n   },
	{_YunMu_ng },
	{_YunMu_o  },
	{NULL}
};

//b开头
_PyIndexType  PyIndex_b[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_ei  },
	{_YunMu_en  },
	{_YunMu_eng },
	{_YunMu_i   },
	{_YunMu_ian },
	{_YunMu_iao },
	{_YunMu_ie  },
	{_YunMu_in  },
	{_YunMu_ing },
	{_YunMu_o   },
	{_YunMu_u   },
	{NULL}
};

//c开头
_PyIndexType  PyIndex_c[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_eng },
	{_YunMu_i   },
	{_YunMu_ong },
	{_YunMu_ou  },
	{_YunMu_u   },
	{_YunMu_uan },
	{_YunMu_ui  },
	{_YunMu_un  },
	{_YunMu_uo  },
	{NULL}
};

//d开头
_PyIndexType  PyIndex_d[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_eng },
	{_YunMu_i   },
	{_YunMu_ian },
	{_YunMu_iao },
	{_YunMu_ie  },
	{_YunMu_ing },
	{_YunMu_iu  },
	{_YunMu_ong },
	{_YunMu_ou  },
	{_YunMu_u   },
	{_YunMu_uan },
	{_YunMu_ui  },
	{_YunMu_un  },
	{_YunMu_uo  },
	{NULL}
};

//e开头
_PyIndexType  PyIndex_e[]=
{
	{_YunMu_none},
	{_YunMu_n   },
	{_YunMu_r   },
	{NULL}
};

//f开头
_PyIndexType  PyIndex_f[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ei  },
	{_YunMu_en  },
	{_YunMu_eng },
	{_YunMu_o   },
	{_YunMu_ou  },
	{_YunMu_u   },
	{NULL}
};

//g开头的拼音
_PyIndexType  PyIndex_g[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_ei  },
	{_YunMu_en  },
	{_YunMu_eng },
	{_YunMu_ong },
	{_YunMu_ou  },
	{_YunMu_u   },
	{_YunMu_ua  },
	{_YunMu_uai },
	{_YunMu_uan },
	{_YunMu_uang},
	{_YunMu_ui  },
	{_YunMu_un  },
	{_YunMu_uo  },
	{NULL}
};

//h开头的拼音
_PyIndexType  PyIndex_h[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_ei  },
	{_YunMu_en  },
	{_YunMu_eng },
	{_YunMu_ong },
	{_YunMu_ou  },
	{_YunMu_u   },
	{_YunMu_ua  },
	{_YunMu_uai },
	{_YunMu_uan },
	{_YunMu_uang},
	{_YunMu_ui  },
	{_YunMu_un  },
	{_YunMu_uo  },
	{NULL}
};


//j开头
//分别用数组指针初始化_PyMa和_YunMu
_PyIndexType  PyIndex_j[]=
{
	{_YunMu_none},
	{_YunMu_i   },
	{_YunMu_ia  },
	{_YunMu_ian },
	{_YunMu_iang},
	{_YunMu_iao },
	{_YunMu_ie  },
	{_YunMu_in  },
	{_YunMu_ing },
	{_YunMu_iong},
	{_YunMu_iu  },
	{_YunMu_u   },
	{_YunMu_uan },
	{_YunMu_ue  },
	{_YunMu_un  },
	{NULL}
};

//k开头
_PyIndexType  PyIndex_k[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_en  },
	{_YunMu_eng },
	{_YunMu_ong },
	{_YunMu_ou  },
	{_YunMu_u   },
	{_YunMu_ua  },
	{_YunMu_uai },
	{_YunMu_uan },
	{_YunMu_uang},
	{_YunMu_ui  },
	{_YunMu_un  },
	{_YunMu_uo  },
	{NULL}
};

//l开头
_PyIndexType  PyIndex_l[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_ei  },
	{_YunMu_eng },
	{_YunMu_i   },
	{_YunMu_ian },
	{_YunMu_iang},
	{_YunMu_iao },
	{_YunMu_ie  },
	{_YunMu_in  },
	{_YunMu_ing },
	{_YunMu_iu  },
	{_YunMu_ong },
	{_YunMu_ou  },
	{_YunMu_u   },
	{_YunMu_uan },
	{_YunMu_ue  },
	{_YunMu_un  },
	{_YunMu_uo  },
	{_YunMu_v   },
	{NULL}
};

//m开头
_PyIndexType  PyIndex_m[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_ei  },
	{_YunMu_en  },
	{_YunMu_eng },
	{_YunMu_i   },
	{_YunMu_ian },
	{_YunMu_iao },
	{_YunMu_ie  },
	{_YunMu_in  },
	{_YunMu_ing },
	{_YunMu_iu  },
	{_YunMu_o   },
	{_YunMu_ou  },
	{_YunMu_u   },
	{NULL}
};

//n开头
_PyIndexType  PyIndex_n[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_ei  },
	{_YunMu_en  },
	{_YunMu_eng },
	{_YunMu_i   },
	{_YunMu_ian },
	{_YunMu_iang},
	{_YunMu_iao },
	{_YunMu_ie  },
	{_YunMu_in  },
	{_YunMu_ing },
	{_YunMu_iu  },
	{_YunMu_ong },
	{_YunMu_u   },
	{_YunMu_uan },
	{_YunMu_ue  },
	{_YunMu_uo  },
	{_YunMu_v   },
	{NULL}
};

//o开头
_PyIndexType  PyIndex_o[]=
{
	{_YunMu_none},
	{_YunMu_u   },
	{NULL}
};

//p开头
_PyIndexType  PyIndex_p[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_ei  },
	{_YunMu_en  },
	{_YunMu_eng },
	{_YunMu_i   },
	{_YunMu_ian },
	{_YunMu_iao },
	{_YunMu_ie  },
	{_YunMu_in  },
	{_YunMu_ing },
	{_YunMu_o   },
	{_YunMu_ou  },
	{_YunMu_u   },
	{NULL}
};

//q开头的拼音
_PyIndexType  PyIndex_q[]=
{
	{_YunMu_none},
	{_YunMu_i   },
	{_YunMu_ia  },
	{_YunMu_ian },
	{_YunMu_iang},
	{_YunMu_iao },
	{_YunMu_ie  },
	{_YunMu_in  },
	{_YunMu_ing },
	{_YunMu_iong},
	{_YunMu_iu  },
	{_YunMu_u   },
	{_YunMu_uan },
	{_YunMu_ue  },
	{_YunMu_un  },
	{NULL}
};

//r开头的拼音
_PyIndexType  PyIndex_r[]=
{
	{_YunMu_none},
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_en  },
	{_YunMu_eng },
	{_YunMu_i   },
	{_YunMu_ong },
	{_YunMu_ou  },
	{_YunMu_u   },
	{_YunMu_uan },
	{_YunMu_ui  },
	{_YunMu_un  },
	{_YunMu_uo  },
	{NULL}
};

//s开头的拼音
_PyIndexType  PyIndex_s[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_en  },
	{_YunMu_eng },
	{_YunMu_i   },
	{_YunMu_ong },
	{_YunMu_ou  },
	{_YunMu_u   },
	{_YunMu_uan },
	{_YunMu_ui  },
	{_YunMu_un  },
	{_YunMu_uo  },
	{NULL}
};

//t开头的拼音
_PyIndexType  PyIndex_t[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_eng },
	{_YunMu_i   },
	{_YunMu_ian },
	{_YunMu_iao },
	{_YunMu_ie  },
	{_YunMu_ing },
	{_YunMu_ong },
	{_YunMu_ou  },
	{_YunMu_u   },
	{_YunMu_uan },
	{_YunMu_ui  },
	{_YunMu_un  },
	{_YunMu_uo  },
	{NULL}
};

//w开头的拼音
_PyIndexType  PyIndex_w[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ei  },
	{_YunMu_en  },
	{_YunMu_eng },
	{_YunMu_o   },
	{_YunMu_u   },
	{NULL}
};

//x开头的拼音
_PyIndexType  PyIndex_x[]=
{
	{_YunMu_none},
	{_YunMu_i   },
	{_YunMu_ia  },
	{_YunMu_ian },
	{_YunMu_iang},
	{_YunMu_iao },
	{_YunMu_ie  },
	{_YunMu_in  },
	{_YunMu_ing },
	{_YunMu_iong},
	{_YunMu_iu  },
	{_YunMu_u   },
	{_YunMu_uan },
	{_YunMu_ue  },
	{_YunMu_un  },
	{NULL}
};

//y开头的拼音
_PyIndexType  PyIndex_y[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_i   },
	{_YunMu_in  },
	{_YunMu_ing },
	{_YunMu_o   },
	{_YunMu_ong },
	{_YunMu_ou  },
	{_YunMu_u   },
	{_YunMu_uan },
	{_YunMu_ue  },
	{_YunMu_un  },
	{NULL}
};

//z开头的拼音
_PyIndexType  PyIndex_z[]=
{
	{_YunMu_none},
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_ei  },
	{_YunMu_en  },
	{_YunMu_eng },
	{_YunMu_i   },
	{_YunMu_ong },
	{_YunMu_ou  },
	{_YunMu_u   },
	{_YunMu_uan },
	{_YunMu_ui  },
	{_YunMu_un  },
	{_YunMu_uo  },
	{_YunMu_z   },
	{NULL}
};

//ch开头的拼音
_PyIndexType  PyIndex_ch[]=
{
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_en  },
	{_YunMu_eng },
	{_YunMu_i   },
	{_YunMu_ong },
	{_YunMu_ou  },
	{_YunMu_u   },
	{_YunMu_uai },
	{_YunMu_uan },
	{_YunMu_uang},
	{_YunMu_ui  },
	{_YunMu_un  },
	{_YunMu_uo  },
	{NULL}
};

//sh开头的拼音
_PyIndexType  PyIndex_sh[]=
{
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_en  },
	{_YunMu_eng },
	{_YunMu_i   },
	{_YunMu_ou  },
	{_YunMu_u   },
	{_YunMu_ua  },
	{_YunMu_uai },
	{_YunMu_uan },
	{_YunMu_uang},
	{_YunMu_ui  },
	{_YunMu_un  },
	{_YunMu_uo  },
	{NULL}
};

//zh开头的拼音
_PyIndexType  PyIndex_zh[]=
{
	{_YunMu_a   },
	{_YunMu_ai  },
	{_YunMu_an  },
	{_YunMu_ang },
	{_YunMu_ao  },
	{_YunMu_e   },
	{_YunMu_en  },
	{_YunMu_eng },
	{_YunMu_i   },
	{_YunMu_ong },
	{_YunMu_ou  },
	{_YunMu_u   },
	{_YunMu_ua  },
	{_YunMu_uai },
	{_YunMu_uan },
	{_YunMu_uang},
	{_YunMu_ui  },
	{_YunMu_un  },
	{_YunMu_uo  },
	{NULL}
};


//上述数组名的指针数组
_PyIndexType *PyIndexPointer[]=
{
	PyIndex_a,
	PyIndex_b,
	PyIndex_c,
	PyIndex_d,
	PyIndex_e,
	PyIndex_f,
	PyIndex_g,
	PyIndex_h,
	PyIndex_i,
	PyIndex_j,
	PyIndex_k,
	PyIndex_l,
	PyIndex_m,
	PyIndex_n,
	PyIndex_o,
	PyIndex_p,
	PyIndex_q,
	PyIndex_r,
	PyIndex_s,
	PyIndex_t,
	PyIndex_u,
	PyIndex_v,
	PyIndex_w,
	PyIndex_x,
	PyIndex_y,
	PyIndex_z
};



//---------------------------------------------------------------------------
static unsigned char KeyNum[PY_MAX_LENGTH];            //按健数码缓冲区
static unsigned char PYBuf_bak[12][PY_MAX_LENGTH];        //拼音组合缓冲区

static char* MatchPinyin;    //匹配出的合法拼音列表
static int PinyinLength=0;   //拼音字符串长度

#if (POS_TYPE == POS_APE3000R)
static struct _KeyBuf KeyCombination[9]=         //按键与字符对应
{
	{1,""},{2,"abc"}, {3,"def"},{4,"ghi"},{5,"jkl"},
	{6,"mno"},{7,"pqrs"},{8,"tuv"}, {9,"wxyz"}    
};
#else //POS_TYPE

#if (KEYPAD_TYPE == 1)
static struct _KeyBuf KeyCombination[9]=         //按键与字符对应
{
	{1,""},{2,"abc"}, {3,"def"},{4,"ghi"},{5,"jkl"},
	{6,"mno"},{7,"pqrs"},{8,"tuv"}, {9,"wxyz"}    
};
#else
static struct _KeyBuf KeyCombination[9]=         //按键与字符对应
{
	{1,"pqrs"},{2,"tuv"}, {3,"wxyz"},{4,"ghi"},
	{5,"jkl"},{6,"mno"},{7,""},{8,"abc"}, {9,"def"}    
};
#endif
#endif //POS_TYPE
//**********************************************************************************
void PYBuf_clear(unsigned char (*PYBuf)[8])	//拼音组合缓冲区清零
{
	unsigned char i,j;
	
	for(i=0;i<12;i++)
	{
		for(j=0;j<8;j++)
		{
			PYBuf_bak[i][j] = PYBuf[i][j];
			PYBuf[i][j] = 0;
		}
	}
}


//判断输入的字符串是否为有效的拼音字符，若是则返回1，否则返回0
//参数char* _pInput为待检测的字符串
char IsPyStr(unsigned char* _pInput)
{
       //
       // p1指向PyIndex_a~zh[]数组中具体的条目，为已经检索到的拼音及对应韵母
       // p2指向_pInput中声母所对应的汉字类别
       // 在比较过程中，先比较声母，然后比较韵母，若都相同则返回1
       //

       _PyIndexType  *p1=0,*p2=0;
       size_t complen=0;
       if(strlen((const char*)_pInput)==0)     //判断合法性
       {
                return 0;
       }
       if (_pInput[0]=='i' || _pInput[0]=='u' || _pInput[0]=='v')
       {    
                return 0;    //不合法的拼音
       }
           
       if(strlen((const char*)_pInput)==1)
       {
                if(_pInput[0]!='i' || _pInput[0]!='u' || _pInput[0]!='v')
                        return 1;
       }
       //顺序排列，得到索引，P2此时指向
       p2= PyIndexPointer[_pInput[0]-'a'];
                                                                                                                                               
       if(_pInput[1]=='h')
       {
                switch(_pInput[0])
                {
                case 'c':
                        p2= PyIndex_ch;
                        _pInput++;
                        break;
                case 's':
                        p2= PyIndex_sh;
                        _pInput++;
                        break;
                case 'z':
                        p2= PyIndex_zh;
                        _pInput++;
                       break;
                }
        }
        //以上为声母比较，以下为韵母比较
        //获得拼音为单个声母时对应的数组
        if(p2==PyIndex_ch||p2==PyIndex_sh||p2==PyIndex_zh)
        {
                complen=strlen((const char*)_pInput)-1;
        }
        else
        {
                complen=strlen((const char*)_pInput)-1;
        }
        p1=p2;
        //获得第一个韵母

        while(p1->_YunMu!=NULL)
       {
                if(p1->_YunMu[0]==_pInput[1])
                {
                        break;
                }
                else
                        p1++;
        }
        while(p1->_YunMu!=NULL)
        {
                if( strncmp((const char*)p1->_YunMu, (const char *)(_pInput+1), complen)==0)
                {
	                        return 1;       //满足要求
                }
                else
                        p1++;
        }
        if(p1->_YunMu == NULL)
                p1=p2;
        return 0;
}
 

//---------------------------------------------------------------------------
//T9输入法核心算法
unsigned char T9_Input(unsigned char *key, unsigned char *num, unsigned char (*PYBuf)[PY_MAX_LENGTH]) 	//按键对应的字母经检索组合成有效的拼音 
{
	int PinyinNum;	//表示输入了几个拼音字符

	//返回：指向PYBuf的二维数组，PYBuf[i][7]=1代表该拼音组合有效
	unsigned char  i,j,k;
	unsigned char  c0=0,c1=0,c2=0,c3=0,c4=0,c5=0;  //可能的字符，ci为第i次的字符
	unsigned char  ABC_Array[6][5];    // 分配缓冲,存放按键对应的字符串,如1对应"abc"

	PYBuf_clear(PYBuf);		//拼音组合缓冲区清零
	memcpy(KeyNum, key, sizeof(KeyNum));

	for(i=0;i<6;i++)	//键盘分配缓冲清零
	{
		for(j=0;j<5;j++)
		{
			ABC_Array[i][j]=0;
		}
	}

    //按照按键缓冲区顺序，将对应的字符串赋给ABC[k]
	PinyinNum=strlen((char *)KeyNum);
	if(PinyinNum==0)    //拼音长度为0
	{
		return FAILURE;
	}

    for( k=0; k<PinyinNum; k++ )
    {
		//if ( (KeyCombination[KeyNum[k]-1]).KeyStr != NULL )
			strcpy((char*)ABC_Array[k],(KeyCombination[KeyNum[k]-1]).KeyStr);
		//printf("the string is %s   \n",(char*)ABC_Array[k]);
    }
	i=0;   //i表示匹配产生的有效拼音数目
    k--;
                                  

    //匹配产生有效拼音
	for(c0=0; ABC_Array[0][c0]!=0; c0++)
	{		
		if(ABC_Array[0][c0]=='i'||ABC_Array[0][c0]=='u'||ABC_Array[0][c0]=='v') //第一个拼音上不可能的字母
			continue;
		PYBuf[i][0]=ABC_Array[0][c0];		//第一位拼音字母
                
		if(k>=1)			//有2个拼音时
		{
		 	for(c1=0; ABC_Array[1][c1]!=0; c1++)
		 	{			
		  		PYBuf[i][1]=ABC_Array[1][c1];	//第二位拼音字母
		  		if(k>=2)			//有3个拼音时
		  		{
		   			for(c2=0;ABC_Array[2][c2]!=0;c2++)
		   			{			
		    			PYBuf[i][2]=ABC_Array[2][c2];
		    			if(k>=3)			//有4个拼音时
		    			{
		     				for(c3=0;ABC_Array[3][c3]!=0;c3++)
		     				{			
		      					PYBuf[i][3]=ABC_Array[3][c3];
		      					if(k>=4)			//有5个拼音时
		      					{
		       						for(c4=0;ABC_Array[4][c4]!=0;c4++)
		       						{			
		        						PYBuf[i][4]=ABC_Array[4][c4];
		        						if(k==5)		//有6个拼音时
		        						{
		         							for(c5=0;ABC_Array[5][c5]!=0;c5++)
		         							{			
		          								PYBuf[i][5]=ABC_Array[5][c5];
		          								if(IsPyStr((unsigned char*)PYBuf[i]))   //如果为有效拼音，则返回1
		          								{
                                                    for(j=0;j<8;j++)
														PYBuf[i+1][j]=PYBuf[i][j];  //将产生的有效拼音赋值给后一个PYBuf行
                                                    PYBuf[i][7]=1;   //将PyBuf最后一个元素置为1
                                                    i++;
                                                    PinyinLength+=strlen((char*)PYBuf[j])+3;
                                                }
                                            }
                                        }
		        						else if(IsPyStr((unsigned char*)PYBuf[i]))	//有5个拼音时
		        						{
	                                        for(j=0;j<8;j++)
	                                                PYBuf[i+1][j]=PYBuf[i][j];
	                                        PYBuf[i][7]=1;
	                                        i++;
	                                        PinyinLength+=strlen((char*)PYBuf[j])+3;
                                        }
		       						}
		      					}
		      					else if(IsPyStr((unsigned char*)PYBuf[i]))	//有4个拼音时
		      					{
                                    for(j=0;j<8;j++)
                                            PYBuf[i+1][j]=PYBuf[i][j];
                                    PYBuf[i][7]=1;
                                    i++;
                                    PinyinLength+=strlen((char*)PYBuf[j])+3;
                                }
                        	}
		    			}
		    		    else if(IsPyStr((unsigned char*)PYBuf[i]))	//有3个拼音时
						{
                            for(j=0;j<8;j++)
                                    PYBuf[i+1][j]=PYBuf[i][j];
                            PYBuf[i][7]=1;
                            i++;
                            PinyinLength+=strlen((char*)PYBuf[j])+3;
                        }
		   			}
		  		}
		  		else if(IsPyStr((unsigned char*)PYBuf[i]))	//有2个拼音时
				{
					if (PYBuf[i][0] != PYBuf[i][1])
					{
						for(j=0;j<8;j++)
								PYBuf[i+1][j]=PYBuf[i][j];
						PYBuf[i][7]=1;
						i++;
						PinyinLength+=strlen((char*)PYBuf[j])+3;
					}
            	}
		 	}
		}
		else if(IsPyStr((unsigned char*)PYBuf[i]))	 //有1个拼音时
		{
            for(j=0;j<8;j++)
                    PYBuf[i+1][j]=PYBuf[i][j];
            PYBuf[i][7]=1;
            i++;
            PinyinLength+=strlen((char*)PYBuf[j])+3;
        }
	}
	
	for(j=0;j<8;j++)      //清空余下的拼音
	{
        PYBuf[i][j]=0;
    }

#ifdef T9_DEBUG 
	j = 0;
	while(1)
	{
		if (PYBuf[j][7] == 0)
			break;
		j++;
	}
#endif

	if (i != 0)			//有符合的拼音，令首字母大写
	{
		j = 0;			
		while(1)
		{
			if (PYBuf[j][7] == 0)
				break;
			PYBuf[j][0] = PYBuf[j][0] - 0x20;
			j++;
		}
		*num = i;
	}
	else				//无符合的拼音，恢复原拼音
	{
		for(j=0; j<8; j++)
		{
			memset(PYBuf[j], 0, sizeof(PYBuf[j]));
		}	
		*num = 0;
	}


		
	return SUCCESS;
}

#endif //T9PINYIN_SWITCH

