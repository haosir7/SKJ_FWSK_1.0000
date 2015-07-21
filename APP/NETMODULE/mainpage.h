/** @mainpage 


@section intro 说明
网络模块，对应于工程的APP/NETMODULE文件夹。

本模块整合了网络连接与网络数据传输的相关功能。

网络连接支持有：局域网、ADSL、3G、M2M-WMMP、M2M-拨号、VPN。

网络数据传输支持：HTTP协议；SOCKEY方式、WMMP方式；CURL库。

可在netModuleConfig.h文件中对本模块进行裁剪与配置。


@version 1.6



<hr>
@section note 版本修改历史


- 版本: 1.6		日期: 2014-06-13		作者: 张磊
-# 修正M2M模块激活与M2M网络模式切换中的BUG。（2014.5.23）。
-# 增加M2M_WMMP_Check::check()函数。
-# 取消对传输方法的配置项（原本需要在WMMP与SOCKET中选择一个），
	变为网络模式选则M2M_WMMP时直接固定为使用WMMP方式，
	网络模式不为M2M_WMMP并且传输模块选为TRANS_BY_TRANSPROC时使用SOCKET。（2014.5.28）
-# 修改了wmmp方式与apn方式下的initNetDevice函数，
	同时修改了“模块激活”功能的流程：修改后该函数只改数据库状态，不做实际的模块设置。
-# 修改了APN接入点设置界面：APN号码只显示不可修改，并在清库脚本中将默认号码（*99***1#）写死。
-# 改了几个与M2M有关的界面BUG。
-# 修改了CNetManager::NetTransMutexLock()函数的实现方法，原先的实现方法在款机平台不起作用（2014.8.27）。



- 版本: 1.5		日期: 2013-12-10		作者: 张磊
-# 将netHeaderDef.h中用于参数配置的宏定义拿出来另存为netModuleConfig.h头文件，该头文件可位于NETMODULE目录之外。
	这样修改后，配置参数值的改变将不再做为模块内部变动来看待。
-# 增加了一个宏定义SINGLE_TRANSBUFF_MODE，用于设置数据发送与接收共用一个缓存的情况。
-# 在trans_http类中增加了支持使用GET关键字来发送http请求相关方法。（2014.1.8）
-# 完善了几处界面中的错误提示。

  

- 版本: 1.4		日期: 2013-08-07		作者: 张磊
-# 取消1.3版本中第1条的判断方法，改为根据是否获取到IP的方法来判断网络连通状态。
	同时将获取IP与网关的实现方法改为socket方式实现，路由与DNS的获取方式仍用原有方法未改动。
	在proc目录下增加ipINfoProc.c文件，实现了socket方式来获取当前IP与网关的函数，
	相应的CommonCheck.cpp与CommonGetIPinfo.cpp文件也做了修改。
-# 建立3G连接失败时，增加一"killall pppd_modem"的调用。



- 版本: 1.3		日期: 2013-11-11		作者: 张磊
-# 修改了CommonCheck类，将判断方法由原先的"ifconfig XXX"改为"cat /proc/net/dev"
-# 对应上面的改动，变更一些VPN过程调用中的函数层次关系，简化了3G拨号与断开的过程。



- 版本: 1.2		日期: 2013-09-15		作者: 张磊
-# 修改当SOCKET发送数据长度为0时的BUG。
-# 在CNetTransfers中添加接口函数GetRevBuff。
-# 在局域网IP模式设置前先对VPN连接状态做判断。
-# 在3G与ADSL断开操作时，若VPN连接则自动调用VPN断开操作。
-# 对netHeaderDef.h小调整。
-# 修改了VPN配置文件变更中的小BUG（文件系统不支持VPN时执行VPN设置会死机）。



- 版本: 1.1		日期: 2013-08-07		作者: 张磊
-# 修改了VPN拨号时的路由设置方法，使之可适应DHCP下的VPN连接。
-# 增加了切换模式的限定，在未断开连接时禁止切换网络模式。

  

- 版本: 1.0		日期: 2013-06-01		作者: 张磊 


*/

