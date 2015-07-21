#ifndef CAIMEWIN_H
#define CAIMEWIN_H

#include "IncludeMe.h"
#include "g_def.h"
#include "CaInput.h"
#include "CaLabel.h"
#include "CaLableSelect.h"
#include "CaWindow.h"

extern "C"
{
#include "phrase.h"
#include "T9_pinyin.h"
};

#define IMEWIN_X 0
#define IMEWIN_Y (SCREEN_H - LINE_H*2 - 4)
#define IMEWIN_H (LINE_H*2 + 2)
#define IMEWIN_W (SCREEN_W - 1)

#define PINYIN_BUF_LEN	12
#define PINYIN_X		0
#define PINYIN_Y		16


#define PINYIN_CHN_BUF_LEN  31
#define PINYIN_PAGE_SIZE  6
#define WB_MAX_DIS_LEN 24

#define PINYIN_OUT_LEN  512
#define PINYIN_IN_LEN  32
#define MAX_PINYIN_LEN 6

#define T9ASC_DELAY_TIME	700

#define HW_REV_EVENT		0xee

#define	LENOVO_MAX_DIS_LEN	24
#define	T9_MAX_DIS_LEN		19

extern UINT8 g_nHWType;

/*! @brief 按键与西文字符对照表
*/
typedef struct				   
{
	unsigned char key;
	char *list;
}ASC_Key_List;
	

/*! @brief 浮动的输入法窗口
	输入法名字长度固定为2个汉字长
*/
class CaIMEWin :public CaWindow
{
public:
	/** 
     * @enum IME_TYPE
     * @brief 输入法类别定义
     */
	typedef enum{//IME_DEFAULT=0, /**< 缺省值		*/  
				IME_PIN_YIN=0,	/**< 拼音		*/
				IME_LOWER_CASE,	/**< 小写字母	*/
				IME_UPPER_CASE,	/**< 大写字母	*/
				IME_BI_HUA,		/**< 笔画		*/	 
				IME_QU_WEI,		/**< 区位		*/
				IME_HAND_WRITE, /**< 手写		*/
				//IME_WU_BI,		/**< 五笔		*/
				//IME_T9_PINYIN,	/**< 五笔拼音	*/
				//IME_T9_BIHUA,	/**< 五笔笔画	*/
				IME_DISABLE		/**< 不能打开输入法，只能用缺省输入*/
	}IME_TYPE; 
	static char *s_ImeNames[9];
	 
	static ASC_Key_List s_ascKeyList[9];

private:
	int m_IMEType;
	CaLabel *m_pIMELabel;
	CaInput *m_pPYDisInput;
	CaLabelSelect *m_T9Label;
	bool m_bIMEInputFocused;

	int m_iCurHZIndex;
	int m_iHZMaxLen;
	int m_iPinyinLen;
	unsigned char m_caPinyin_ChnBuf[PINYIN_CHN_BUF_LEN];//显示带数字的候选汉字
	wchar_t m_caPinyin_out[PINYIN_OUT_LEN];//存储根据拼音查出来的汉字
	unsigned char m_caPinyin_in[PINYIN_IN_LEN];//记录输入的拼音
	unsigned char m_RandLenBuf[/*PHRASE_BUFFER_SIZE*/64];//用于记录不定长每行的单词数
	int m_iLineIndex;//用于记录翻页时的页数即行数
	unsigned char m_chHwBuf[64];//带屏手写板数据缓存

	int m_iMaxWordsSize;//每页最多的字或词的个数  五笔

	int m_iQwLen;
	unsigned char m_caQwBuf[4];//记录输入的区位

	int m_caT9PYLen;							//记录T9码输入个数
	unsigned char m_caT9PYBuf[PINYIN_BUF_LEN];	//记录t9码
	unsigned char PYBuf[12][PY_MAX_LENGTH];
	unsigned char PY_num;						//符合当前输入的拼音数
	int PY_CurPos;								//选中拼音的所在位置
	int m_T9state;	
	int m_T9LenovoState;	//T9联想状态，1-选择联想汉字 0-继续输入拼音

	int m_T9ASC_lastkey;		//记录上次按键键值
	int m_T9ASC_clickCount;		//记录连续快速按键次数
	UINT64 m_T9ASC_lastTime;	//记录上次按键时间
	
	UINT8 T9BH_Pageup_Could;
	UINT8 T9BH_Pagedown_Could;
	UINT8 T9BH_state;		//T9笔画状态，1-选汉字 0-输入笔画

	int m_handwrite_flag;		//记录手写状态：1－有待选汉字；0－无

	//拼音输入法
	void BeginChn();
	int RecPinyin(int iEvent, unsigned char *pEventData, int iDataLen);

	//T9拼音
	int RecT9py(int iEvent, unsigned char *pEventData, int iDataLen);
	void BeginT9py();
	unsigned char InputT9PY(unsigned char *key, unsigned char enterNum, 
							unsigned char *outNum, unsigned char (*PYBuf)[PY_MAX_LENGTH]);
	
	//T9字符
	void BeginT9ASC();
	int ProcDirectInput(int iEvent, unsigned char *pEventData, int iDataLen);

	//T9笔画
	void BeginT9Bihua();
	int RecT9Bihua(int iEvent, unsigned char *pEventData, int iDataLen);

	//五笔输入法
	void BeginWB();
	int RecWB(int iEvent, unsigned char *pEventData, int iDataLen);
	
	//区位输入法
	void BeginQW();
	int RecQW(int iEvent, unsigned char *pEventData, int iDataLen);
	
	//手写输入
	/*!
	@brief 处理手写板事件（不带屏手写板）	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	@return   1  已处理； 0 无法处理
	*/
	int RecHandWrite(int iEvent, unsigned char *pEventData, int iDataLen);
		
	/*!
	@brief 处理手写板事件（胜创通带屏手写板）	
	@param[in] iEvent 事件编号
	@param[in] pEventData 事件内容
	@param[in] iDataLen 事件内容长度
	@return   1  已处理； 0 无法处理
	*/
	int RecScreenHandWrite(int iEvent, unsigned char *pEventData, int iDataLen);
		
	//处理联想输入 (因为拼音的接口没有和wb统一，所以只能单独出来)
	bool m_bLenovoOn;//是否启动了联想
	int LenovoProc(int iEvent, unsigned char *pEventData, int iDataLen);

	//显示T9拼音选择条
	void DisT9py(void);
	void ClearT9py(void);

	
	void ChangeFocus(int iEvent, unsigned char *pEventData, int iDataLen);
	
public:
	CaInput *m_pIMEInput;
	bool m_bRecurSpecChar;//防止特殊字符win和ime的递归调用
public:
	CaIMEWin();
	~CaIMEWin();
    virtual	int ReFresh();
    virtual int ProcEvent(int iEvent,unsigned char *pEventData, int iDataLen);
	void Show();
// 	IME_TYPE ImeType(){return m_IMEType;}
//  void ImeType(IME_TYPE itype){m_IMEType=itype;}
	virtual bool IsCurWin();

	//结束输入法窗口时处理一些事情
	void EndProcess();
	//变换输入法
	void ChangeInputMethod();


};
#endif

