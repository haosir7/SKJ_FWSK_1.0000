#ifndef BUSINESS_SERIAL_PROC_H
#define BUSINESS_SERIAL_PROC_H

#include <string>
#include <string.h>
#include <stdio.h>
#include "comdatatype.h"
#include "DataDesign.h"
#include "SerialProtocol.h"
#include "CUserInfo.h"
#include "CInvKind.h"
#include "CInvVol.h"
#include "CInvHead.h"
#include "CInvDet.h"
#include "CTax.h"
#include "CTjxxhz.h"
#include "CNetPara.h"

#include "YWXMLGY.h"

using namespace std;

/**
 *@class CBusinessSerialProc
 *@brief 串口数据发起
 */
class CBusinessSerialProc {
private:
	UINT8 ret;
	string sksbbhstr;
	string bspbhstr;
	string qtxx;
	string strErr;

	SerialProtocol* m_serialProtocol;

public:
	CBusinessSerialProc();
	~CBusinessSerialProc();
	

/*!
@brief  联机函数
@param[in] 
@param[out] strErr	错误信息
*/
UINT8 onLine_Serial(string &strErr);


/*!
@brief  税控盘编号查询
@param[in] ywxml_gy	公共参数
@param[out] sksbbh	税控设备编号
@param[out] strErr	错误信息
*/
UINT8 skpbhcx_Serial(CYWXML_GY &ywxml_gy, string &sksbbh, string &strErr);


/*!
@brief  报税盘编号查询
@param[in] ywxml_gy	公共参数
@param[out] bspbh	税控设备编号
@param[out] strErr	错误信息
*/
UINT8 bspbhcx_Serial(CYWXML_GY &ywxml_gy, string &bspbh, string &strErr);


/*!
@brief  税控盘信息查询
@param[in] ywxml_gy	公共参数
@param[out] UserInfo	纳税户信息
@param[out] strCurTime	当前时间
@param[out] strErr	错误信息
*/
UINT8 skpxxcx_Serial(CYWXML_GY &ywxml_gy, CUserInfo &UserInfo, string &strCurTime, string &strErr);

/*!
@brief  报税盘信息查询
@param[in] ywxml_gy	公共参数
@param[out] UserInf	纳税户信息
@param[out] strErr	错误信息
*/
UINT8 bspxxcx_Serial(CYWXML_GY &ywxml_gy, CUserInfo &UserInfo, string &strErr);


/*!
@brief  监控数据查询
@param[in] ywxml_gy	公共参数
@param[out] InvKind	票种信息
@param[out] strErr	错误信息
*/
UINT8 jksjcx_Serial(CYWXML_GY &ywxml_gy, CInvKind &InvKind, string &strErr);


/*!
@brief  授权税率查询
@param[in] ywxml_gy	公共参数
@param[out] pTax	税率信息
@param[out] taxNum	税率个数
@param[out] strErr	错误信息
*/
UINT8 sqslcx_Serial(CYWXML_GY &ywxml_gy, CTax *pTax, UINT8 &taxNum, string &strErr);


/*!
@brief  购票信息查询
@param[in] ywxml_gy	公共参数
@param[out] pInvVol	发票领用信息
@param[out] strErr	错误信息
*/
UINT8 gpxxcx_Serial(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr);


/*!
@brief  税控设备口令更改
@param[in] ywxml_gy	公共参数
@param[in] ykl	原口令
@param[in] xkl	新口令
@param[out] strErr	错误信息
*/
UINT8 sksbklgg_Serial(CYWXML_GY &ywxml_gy, string ykl, string xkl, string &strErr);


/*!
@brief  发票开具
@param[in] ywxml_gy	公共参数
@param[in & out] pInvHead	发票信息
@param[out] strErr	错误信息
*/
UINT8 fpkj_Serial(CYWXML_GY &ywxml_gy, CInvHead *pInvHead, string &strErr);

/*!
@brief  发票作废
@param[in] ywxml_gy	公共参数
@param[in & out] pInvHead	发票信息
@param[in] zflx	作废类型
@param[out] strErr	错误信息
*/
UINT8 fpzf_Serial(CYWXML_GY &ywxml_gy, CInvHead *pInvHead, UINT8 zflx, string &strErr);


/*!
@brief  发票查询
@param[in] ywxml_gy	公共参数
@param[in] cxfs	查询方式
@param[in] cxtj	查询条件
@param[out] invNum	发票张数
@param[out] pInvHead	发票信息
@param[out] strErr	错误信息
*/
UINT8 fpcx_Serial(CYWXML_GY &ywxml_gy, UINT8 cxfs, string cxtj, UINT32 &invNum, CInvHead *pInvHead, string &strErr);


/*!
@brief  税控盘报税盘组合操作
@param[in] ywxml_gy	公共参数
@param[in] czlx	操作类型
@param[out] strErr	错误信息
*/
UINT8 skpbspzhcz_Serial(CYWXML_GY &ywxml_gy, UINT8 czlx, string &strErr);


/*!
@brief  报税盘发票查询
@param[in] ywxml_gy	公共参数
@param[out] InvCount	发票数目
@param[out] pInvVol	发票卷信息
@param[out] strErr	错误信息
*/
UINT8 bspfpcx_Serial(CYWXML_GY &ywxml_gy,  UINT32 &InvCount, CInvVol *pInvVol, string &strErr);


/*!
@brief  发票分发
@param[in] ywxml_gy	公共参数
@param[in] jzlx	介质类型
@param[out] pInvVol	发票卷信息
@param[out] strErr	错误信息
*/
UINT8 fpff_Serial(CYWXML_GY &ywxml_gy, UINT8 jzlx, CInvVol *pInvVol, string &strErr);


/*!
@brief  发票回收
@param[in] ywxml_gy	公共参数
@param[out] pInvVol	发票卷信息
@param[in] strJzlx	介质类型
@param[out] strErr	错误信息
*/
UINT8 fphs_Serial(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, UINT8 strJzlx, string &strErr);


/*!
@brief  数据抄报
@param[in] ywxml_gy	公共参数
@param[in] jzlx	介质类型
@param[out] strErr	错误信息
*/
UINT8 sjcb_Serial(CYWXML_GY &ywxml_gy, UINT8 jzlx, string &strErr);


/*!
@brief  网络监控回传
@param[in] ywxml_gy	公共参数
@param[out] strErr	错误信息
*/
UINT8 wljkhc_Serial(CYWXML_GY &ywxml_gy, string &strErr);


/*!
@brief  发票统计信息查询
@param[in] ywxml_gy	公共参数
@param[out] pTjxxhz	统计信息
@param[out] strErr	错误信息
*/
UINT8 fptjcx_Serial(CYWXML_GY &ywxml_gy, CTjxxhz *pTjxxhz, string &strErr);


/*!
@brief  企业信息更新
@param[in] ywxml_gy	公共参数
@param[out] strErr	错误信息
*/
UINT8 qyxxgx_Serial(CYWXML_GY &ywxml_gy, string &strErr);


/*!
@brief  离线权限更新
@param[in] ywxml_gy	公共参数
@param[in] lxxxmw	离线消息密文
@param[out] strErr	错误信息
*/
UINT8 lxqxgx_Serial(CYWXML_GY &ywxml_gy, string &strErr);


/*!
@brief  发票实时上传
@param[in] ywxml_gy	公共参数
@param[in] czlx	操作类型
@param[out] fpzs	发票张数
@param[out] strErr	错误信息
*/
UINT8 fpsssc_Serial(CYWXML_GY &ywxml_gy, UINT8 czlx, UINT32 &fpzs, string &strErr);


/*!
@brief  网络参数维护
@param[in] ywxml_gy	公共参数
@param[out] strErr	错误信息
*/
UINT8 wlcswh_Serial(CNetPara *pNetPara, string &strErr);


/*!
@brief  设置串口波特率
@param[in] btl	波特率
@param[out] strErr	错误信息
*/
UINT8 setBaudRate_Serial(UINT32 btl, string &strErr);


/*!
@brief  发票读入
@param[in] ywxml_gy	公共参数
@param[in] jzlx	介质类型
@param[out] strErr	错误信息
*/
UINT8 fpdr_Serial(CYWXML_GY &ywxml_gy, UINT8 jzlx, string &strErr);


//分机退回
/*!
@brief  报税盘信息查询
@param[in] ywxml_gy	公共参数
@param[in] jzlx	介质类型
@param[out] strErr	错误信息
*/
UINT8 fjth_Serial(CYWXML_GY &ywxml_gy, UINT8 jzlx, string &strErr);


/*!
@brief  网络购票
@param[in]  ywxml_gy 公共参数
@param[out] strErr	错误信息
*/
UINT8 wlgp_Serial(CYWXML_GY &ywxml_gy, CInvVol *pInvVol, string &strErr);

/*!
@brief  证书口令更改
@param[in] yzskl	原口令
@param[in] xzskl	新口令
@param[out] strErr	错误信息
*/
UINT8 zsklgg_Serial(CYWXML_GY &ywxml_gy, string yzskl, string xzskl, string &strErr);

/*!
@brief  发票补录
@param[in] ywxml_gy	公共参数
@param[in] bsqsrq	报税起始日期
@param[in] bszhrq	报税终止日期
@param[out] strErr	错误信息
*/
UINT8 fpbl_Serial(CYWXML_GY &ywxml_gy, UINT32 bsqsrq, UINT32 bszzrq, string &strErr);

/*!
@brief  获取离线相关数据
@param[out] wscfpzs		未上传发票张数
@param[out] wscfpsj		未上传发票时间
@param[out] wscfpljje	未上传发票累计金额
@param[out] sczs		上传张数
@param[out] scsjjg		上传时间间隔
@param[out] strErr		错误信息
*/
UINT8 hqlxsj_Serial(string &wscfpzs, string &wscfpsj, string &wscfpljje, string &sczs, string &scsjjg, string &strErr);


/*!
@brief 根据转换器返回的错误号填充错误信息
@param[in] ErrBuf		转换器返回的错误
@param[out] strErr		错误信息
*/
UINT8 FindErrInfo(INT8 *ErrBuf, string &srrErr);

};

#endif
