#ifndef G_DEF_H
#define G_DEF_H

#ifdef WIN32
#define WIN_MMI
#endif
#define LINE_H (CHAR_H + 1) //第一行文字上边框到第二行文字上边框的距离
#define SCREEN_H 64
#define SCREEN_W 192
#define SCREEN_LEFT_OFFSET 4
#define SCREEN_TOP_OFFSET 0

#define PAGE_NUM_W (CHAR_W*6)
#define PAGE_NUM_X (SCREEN_W - PAGE_NUM_W - 2)
#define PAGE_NUM_Y (SCREEN_H - CHAR_H - 2)

//备用的页码
#define PAGE_NUM_X_2 PAGE_NUM_X
#define PAGE_NUM_Y_2 SCREEN_TOP_OFFSET

#define MAX_TITLE_LEN 40

#define INPUT_MAX_TITLE_LEN  64

#define PAGE_UP_KEY 0x49
#define PAGE_DOWN_KEY 0x51
#define PRE_SCREEN_KEY 0x42   //F8

#define PRE_WIN 0x27

#define INPUT_FIRST_ITEM 0X3B		//F1
#define INPUT_SECOND_ITEM 0X3C		//F2
#define INPUT_THIRD_ITEM 0X3D		//F3
#define INPUT_FOURTH_ITEM 0X3E		//F4
#define INPUT_FIFTH_ITEM 0X3F		//F5

/*#define RETURN_MAIN_MENU 0x40		//F6*/

#define PRINT_KEY 'p'



//操作员权限
#define SYSTEMMANAGER				2      //系统管理员
#define SYSTEMDIRECTOR				0      //管理员
#define SYSTEMOPERATOR				1      //开票员

#define ONE_QUERY_UPPER_LIMIT_NUM 200

//一次最多开辟的查询结果空间(256*1024 / sizeOf(invtaxhead) )
#define MAX_RESULT_LINES  64 /*一定要是4的倍数，不然页码显示不正确 最大为256*1024/116 */

#define FAILURE_PRN		0						//打印失败
#define SUCCESS_PRN		1                       //打印成功

//#define GOODS_LINE_MAX_PRN_COUNT          8     //发票商品行最大个数  
#define GOODSDETAIL_LINE_MAX_PRN_COUNT    25    //明细列表中一页商品行数

#define MAX_INV_VOLUME  50 /*当前金税卡中的发票卷数>= MAX_INV_VOLUME时就不允许再导入发票*/

//用来测试
//#define  _TEST_DEBUG 

//单价、数量等浮点数的最大小数位长度
#define MAX_DECIMAL_LEN_IN_APP 14

//税额允许的最大误差
#define  MAX_TAX_ERROR_RANGE 1.27
//数据库保留几个月的发票数据
#define  BACK_MONTHS_OF_INV_DATA  12
//特殊字符支持的最大长度
#define SPECIAL_CH_MAX_LEN 2
//特殊字符数限制
#define SPECIAL_CH_LIMITED 40
//每行显示特殊字符个数
#define CH_PER_LINE 8
//最少保留的特殊字符个数
#define MIN_SPECIAL_CH_LEFT 1

//for debug on the linux platform 
#define RETURN_MAIN_MENU	ESC_KEY	

#endif

