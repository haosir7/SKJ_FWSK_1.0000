

drop table IF EXISTS MACHINE;
drop table IF EXISTS USERINFO;
drop table IF EXISTS USER_MORE;
drop table IF EXISTS DEPT;
drop table IF EXISTS CLIENT;
drop table IF EXISTS INV_VOL;
drop table IF EXISTS INV_HEAD;
drop table IF EXISTS INV_DET;
drop table IF EXISTS RT_INV;
drop table IF EXISTS CB_HZ;
drop table IF EXISTS OPERATOR;
drop table IF EXISTS SYSLOG;
drop table IF EXISTS SYSARG;
drop table IF EXISTS INV_SERV;
drop table IF EXISTS INV_KIND;
drop table IF EXISTS PRN_INFO;
drop table IF EXISTS INV_SUM;
drop table IF EXISTS INV_SORT; 
drop table IF EXISTS TAX;
drop table IF EXISTS NETPARA;

--#########创建开票机信息表###################
--#     机器编号                varchar(16)          
--#     生产日期                date
--#     硬件版本号              varchar(18)
--#     软件版本号              varchar(18)
--############################################
--
-- Table structure for table `MACHINE`
--

CREATE TABLE MACHINE (
	M_NO		varchar(16) 	NOT NULL,
	P_DATE		date        	NOT NULL,
	H_VER		varchar(18) 	NOT NULL,
	S_VER		varchar(18) 	NOT NULL
);

--
-- Dumping data for table `MACHINE`
--
--for_initProc_20150506linzihao
--#########创建企业信息表#####################
--#     纳税人识别号					varchar(30)
--#     纳税人名称            varchar(200) 
--#     开票机号码							varchar(50)
--#     主分机标志					varchar(50)
--#     纳税人企业性质							varchar(6)
--#     行业类型			varchar(40)
--#     金税盘设备号							char
--#     工商登记号						char
--#     开户银行			char
--#     开户账号				char
--#     登记注册地址					varchar(2)
--#     主管税务机关代码				char
--#     主管税务机关名称						varchar(50)
--#     普票授权税率个数     				varchar(200)
--#     开票机数量						int
--#     备用						int
--############################################
--
-- Table structure for table `USERINFO`
--

CREATE TABLE USERINFO (
  NSRSBH		varchar(25)		NOT NULL,
  NSRMC			varchar(200)		NOT NULL,
  KPJHM                 int                     NOT NULL,
  ZFFLAG                char			NOT NULL,
  QY_LX                 varchar(3)			NOT NULL,
  JSPSBH                varchar(12)		NOT NULL,
  GSDJH                 varchar(50),
  KHYH                  varchar(100),
  KHZH                  varchar(50),
  DJZCDZ                varchar(100),
  SWJGDM		varchar(20),		
  SWJGMC		varchar(100),
  SLGS                  int			NOT NULL,
  KPJSL			int			NOT NULL,
  QYSJ			varchar(14),
  BACKUP	        varchar(20)
);

--
-- Dumping data for table `USERINFO`
--

--#########创建额外用户信息表#################
--#		备用1		varchar(20)
--#		备用2		varchar(20)
--#		备用3		varchar(20)
--#		备用4		varchar(20)
--#		备用5		varchar(20)
--#		备用6		varchar(20)
--#		备用7		varchar(20)

--############################################
--
-- Table structure for table `USER_MORE`
--

CREATE TABLE USER_MORE (
	BACKUP1	 varchar(20),
	BACKUP2	 varchar(20),
	BACKUP3	 varchar(20),
	BACKUP4	 varchar(20),
	BACKUP5	 varchar(20),
	BACKUP6	 varchar(20),
	BACKUP7	 varchar(20)	
) ;
--
-- Dumping data for table `USER_MORE`
--

--#########创建所属期设置表###################
--#     所属期起          int   
--#     所属期止         	int
--#     开票截止日期			int
--############################################
--
-- Table structure for table `SSQ_SZ`
--

--CREATE TABLE SSQ_SZ (
--	SSQ		int		NOT NULL,
--	SSQ_Q		int		NOT NULL,
--	SSQ_Z		int		NOT NULL,
--	KPJZRQ		int		NOT NULL,
--	PRIMARY KEY(SSQ)
--);

--
-- Dumping data for table `SSQ_SZ`
--

--#########创建商品信息表#####################
--#     编码           				varchar(16)          
--#     名称           				varchar(100)
--#     单价      					double
--#		税率						float
--#		单位						varchar(50)
--#		部类号						int
--#		简称						varchar(6)
--#		规格						varchar(50)
--#		征收品目代码（税目）		varchar(4)
--#     征收率           			float
--#     折扣率						float
--############################################
--
-- Table structure for table `DEPT`
--

CREATE TABLE DEPT (
	SP_BM			varchar(16)  		NOT NULL,
	SP_MC			varchar(100)		NOT NULL,
	SP_DJ			double			NOT NULL,
	SP_SL			float			NOT NULL,
	SP_DW			varchar(10),
	BLH				int        NOT NULL,
	SP_JM			varchar(6),
	SP_GG			varchar(50),
	ZSPM_DM			varchar(4),
	SP_ZSL			float,
	D_RATE			float			NOT NULL,
	PRIMARY KEY(SP_BM)
);

--
-- Dumping data for table `DEPT`
--

--#########创建客户信息表#################          
--#     本地编号			varchar(16)
--#     代码				varchar(20)
--#     代码简称			varchar(4)
--#     名称				varchar(100)
--#     税号				varchar(30)
--############################################
--
-- Table structure for table `CLIENT`
--

CREATE TABLE CLIENT (
	KH_BH			varchar(16)			NOT NULL,
	KH_DM			varchar(20)			NOT NULL,
	KH_JM		varchar(4)			NOT NULL,
	KH_MC		varchar(100)			NOT NULL,
	KH_SH		varchar(30),
	PRIMARY KEY(KH_BH)
);

--
-- Dumping data for table `CLIENT`
--

--#########创建纳税人发票领用信息表#################
--#     自增       					INTEGER
--#     发票代码         		varchar(20)
--#     发票起始号码        varchar(10)	
--#     发票终止号码				varchar(10)
--#     剩余份数						int
--#     使用标志						char
--#     用完标志						char
--#     日期						int
--############################################
--
-- Table structure for table `INV_VOL`
--

CREATE TABLE INV_VOL (
	NO				INTEGER 		PRIMARY KEY,
	INV_TYPE	        char		        NOT NULL,
	CODE			varchar(20)		NOT NULL,
	IS_NO			int				NOT NULL,
	IE_NO			int				NOT NULL,
	REMAIN			int				NOT NULL,
	FPZFS			int				NOT NULL,
	USED_FLAG		char			NOT NULL,
	OVER_FLAG		char			NOT NULL,
	RQ              int,
	FPLXDM			char(3),
	BACKUP1         int,
	BACKUP2         varchar(10),
	UNIQUE  (CODE,IS_NO)
);

--
-- Dumping data for table `INV_VOL`
--

--#########创建发票开具表#################	
--#     自增					INTEGER
--#     发票代码				varchar(20)    
--#     发票号码				int
--#		开票日期				int              
--#     开票时间				int
--#     开票类型				char
--#     开票合计金额			Big int  
--#     税额					Big int       
--#     原发票代码				varchar(20)
--#     原发票号码				int			
--#     付款单位      			varchar(200)	
--#     付款单位税务识别号      varchar(20)
--#		收款员					varchar(30)
--#		商品行数量				int		
--#		防伪税控码				varchar(32)	
--#		票种类别				varchar(5)
--#		上传标志				char
--#		票样代码				varchar(5)
--#		备注扩展				varchar(4)//暂时注掉20150507linzihao
--#		是否可以开具红票		varchar(4)//暂时注掉20150507linzihao
--#		备用字段1				varchar(20)
--#		CA签名					varchar(200)
--############################################
--
-- Table structure for table `INV_HEAD`
--

CREATE TABLE INV_HEAD (
	NO				INTEGER			PRIMARY KEY,
	FPDM			varchar(20)		NOT NULL,
	FPHM			int				NOT NULL,
	KPRQ			int				NOT NULL,
	KPSJ			int				NOT NULL,
	KPLX			char			NOT NULL,
	KPHJJE			Big int			NOT NULL,
	KPSE			Big int			NOT NULL,
	YFPDM			varchar(20),
	YFPHM			int				NOT NULL,
	FKDW			varchar(200)	NOT NULL,
	FKDWSH		varchar(20)		NOT NULL,
	SKY				varchar(30),
	SPHSL			int				NOT NULL,
	FWM				varchar(32)		NOT NULL,
	FPLB			char,
	SCBZ			char,
	BACKUP1			varchar(20),

	UNIQUE  (FPDM,FPHM)
);

--
-- Dumping data for table `INV_HEAD`
--

--#########创建发票开具明细表#################
--#     自增					INTEGER          
--#     发票代码			varchar(20)
--#			发票号码			int		
--#     开票类型				char
--#     操作员ID      char
--#			商品行序号		int
--#			开票日期			int					
--#			开票时间			int						
--#		商品编码			varchar(16)
--#			商品名称			varchar(100)
--#			商品单价			double
--#			商品数量			double
--#			商品金额			Big int
--#			商品税额			Big int
--#			税率					float
--#		单价(不含税)		double
--#		金额(不含税)		Big int
--#     商品行属性    char
--#     商品单位			varchar(10)
--#		备用字段			varchar(20)	
--############################################
--
-- Table structure for table `INV_DET `
--

CREATE TABLE INV_DET (
	NO		INTEGER				PRIMARY KEY,
	FPDM		varchar(20)			NOT NULL,
	FPHM		int				NOT NULL,
	KPLX		char			NOT NULL,
	SKY		    varchar(30), 
	SPHXH		int				NOT NULL,
	KPRQ		int				NOT NULL,
	KPSJ		int				NOT NULL,
	SPBM		varchar(16)  			NOT NULL,
	SPMC		varchar(100)			NOT NULL,
	SPDJ		double,
	SPSL		double,
	SPJE		Big int				NOT NULL,
	SPSE		Big int				NOT NULL,
	SL		float,
	DJ			double,
	JE			Big int,
	PROP		char				NOT NULL,
	SP_DW			varchar(10),
	BACKUP		varchar(20),
	UNIQUE(FPDM,FPHM,SPHXH)
);

--
-- Dumping data for table `INV_DET `
--

--#########创建退票表#########################
--#     自增						INTEGER
--#     发票代码				varchar(20)
--#     发票号码			int(8)
--############################################
--
-- Table structure for table `RT_INV`
--

CREATE TABLE RT_INV (
     NO     		INTEGER 		PRIMARY KEY,
     FPDM		varchar(20)		NOT NULL,
     FPHM		int			NOT NULL,
     UNIQUE  (FPDM,FPHM)
);

--
-- Dumping data for table `RT_INV`
--

--#########创建抄报汇总表####################
--#			编号				INTEGER	
--#			所属期				int
--#			所属期起			int
--#			所属期止			int
--#			正票份数			int
--#			正票开票金额			Big int
--#			红票份数			int
--#			红票开票金额			Big int
--#			废票份数			int
--#			废票开票金额			Big int
--#			红废份数			int
--#			红废开票金额			Big int
--#			空白发票份数			int
--#			抄报日期			int
--#			抄报成功日期			int
--#			失败原因			varchar(100)
--#			抄报类型			char
--#			报税成功标志			char
--#			报送介质标志			char
--#			本期未上传的发票份数		int
--############################################
--
-- Table structure for table `CB_HZ`
--

CREATE TABLE CB_HZ (
	NO			INTEGER				PRIMARY KEY,
	SSQ			int				NOT NULL,
	SSQ_Q			int				NOT NULL,
	SSQ_Z			int				NOT NULL,
	ZPFS			int				NOT NULL,
	ZPKPJE			Big int				NOT NULL,
	HPFS			int				NOT NULL,
	HPKPJE			Big int				NOT NULL,
	ZFFS			int				NOT NULL,
	ZFKPJE			Big int				NOT NULL,
	HFFS			int				NOT NULL,
	HFKPJE			Big int				NOT NULL,
	KBFPFS			int				NOT NULL,
	BSRQ			int				NOT NULL,
	BSCGRQ			int				NOT NULL,
	BSSBYY			varchar(100),
	BSLX			char				NOT NULL,
	BSCGBZ			char				NOT NULL,
	BSJZBZ			char				NOT NULL,
	WSCFPS			int				NOT NULL,
	UNIQUE (SSQ)
);

--
-- Dumping data for table `CB_HZ`
--
--#########创建操作员信息表###################
--#     编码			char
--#     名称			varchar(10)
--#     密码			varchar(10)     
--#     角色			char
--############################################
--
-- Table structure for table `OPERATOR`
--

CREATE TABLE OPERATOR (
	OP_ID			char			NOT NULL,
	NAME			varchar(10)		NOT NULL,
	PASSWD			varchar(10)		NOT NULL,
	ROLE			char			NOT NULL,
	PRIMARY KEY(OP_ID)
);

--
-- Dumping data for table `OPERATOR`
--

--#########创建系统操作日志表#################	
--#   自增							INTEGER
--#   日期							int
--#		时间							int     			
--#		操作员名称				varchar(10)	
--#		操作类型					char
--#		结果代码					int	
--#											varchar(20)
--############################################
--
-- Table structure for table `SYSLOG`
--

CREATE TABLE SYSLOG (
	NO       	INTEGER 		NOT NULL,
	I_DATE		int			NOT NULL,
	I_TIME		int			NOT NULL, 
	OPERATOR	varchar(10)		NOT NULL,
	TYPE		char			NOT NULL,
	RESULT		int,
	BACKUP		varchar(20),
	PRIMARY KEY(NO)
);

--
-- Dumping data for table `SYSLOG`
--


--#########创建系统参数表#################
--#		参数代码			char			
--#		参数名称			varchar(20)		
--#		参数整型值		int			
--#		参数文本值		varchar(64)		
--#		参数数组值		blob			
--############################################
--
-- Table structure for table `SYSARG`
--
CREATE TABLE SYSARG (
	SA_ID			char			NOT NULL,
	NAME			varchar(20) 		NOT NULL,
	V_INT			int,
	V_TEXT			varchar(64),
	V_BLOB			BLOB,
	PRIMARY KEY(SA_ID)
);

--
-- Dumping data for table `SYSARG`
--
--#########创建发票服务表#################	
--#     发票代码		varchar(20)
--#     发票号码			int    
--#     开票日期			int
--#			开票时间			int              
--#     开票类型			tinyint
--#     开票总金额		big int
--#     上传标志      tinyint
--#     作废标志      tinyint
--#			CA签名			varchar(160)
--#			备用字段			varchar(20)	
--############################################
--
-- Table structure for table `INV_SERV`
--

CREATE TABLE INV_SERV ( 
	NO        INTEGER PRIMARY KEY,
--	CODE		  varchar(20)				NOT NULL,
--	INV_NO		int(11)		NOT NULL, 
	FPDM		  varchar(20)				NOT NULL,
	FPHM		int(11)		NOT NULL,     
	I_DATE		int				NOT NULL,
	I_TIME		int				NOT NULL, 
	I_TYPE		char			NOT NULL,
	M_SUM		  big int		NOT NULL,
	UP_FLAG   char      NOT NULL,
	WST_FLAG  char      NOT NULL,
	CA_SIGN   varchar(160),
	BACKUP		varchar(20)
--	UNIQUE  (CODE_NO, INV_NO)
) ;
--
-- Dumping data for table `INV_SERV`
--

--for_initProc_20150506linzihao
--#########票种信息表#################	
--#     发票类型代码		varchar(10)
--#     离线锁死日（每月的第几天）	varchar(20)
--#     离线开具时间（单位小时）	varchar(20)
--#     单张发票限额		big int
--#     离线正数发票累计限额	big int
--#	备用字段		varchar(20)	
--############################################
--
-- Table structure for table `INV_KIND`
--

CREATE TABLE INV_KIND ( 
	NO        INTEGER PRIMARY KEY,
	FPLXDM		varchar(10)		NOT NULL,	
	LX_SSR      int			NOT NULL,
	LX_KJSJ     int			NOT NULL,
	MAX_SING	big int		NOT NULL,	
	MAX_SUM	    big int		NOT NULL,	
	BACKUP		varchar(20),
	UNIQUE  (FPLXDM)
) ;

--
-- Dumping data for table `INV_KIND`
--
--#########发票打印附加信息#################	
--#     自定义名称1		  varchar(20)
--#     自定义内容1			varchar(20)   
--#     自定义名称2		  varchar(20)
--#     自定义内容2		  varchar(20)
--#			备注						varchar(30)              
--#			备用字段			varchar(20)	
--############################################
--
-- Table structure for table `PRN_INFO`
--

CREATE TABLE PRN_INFO( 
	ZDY_TAB1		varchar(30)				,
	ZDY_CONT1		varchar(30)				,
	ZDY_TAB2		varchar(30)				,
	ZDY_CONT2		varchar(30)				,
  REMARKS     varchar(50)				,
	BACKUP		  varchar(20)
) ;
--
-- Dumping data for table `PRN_INFO`
--
--#########创建月统计信息表#############
--#     起始日期       		int          
--#     截止日期			int
--#     期初库存份数        int
--#     领购发票份数		int
--#     退回发票份数		int          
--#     期末库存份数		int
--#     正数发票份数        int
--#     正废发票份数        int
--#     负数发票份数        int
--#     负废发票份数        int
--#     空废发票份数        int
--#     正数发票累计金额        int
--#     正数发票累计税额        int
--#     正废发票累计金额        int
--#     正废发票累计税额        int
--#     负数发票累计金额        int
--#     负数发票累计税额        int
--#     负废发票累计金额        int
--#     负废发票累计税额        int
--############################################
--
-- Table structure for table `INV_SUM`
--

CREATE TABLE INV_SUM (
	NO				INTEGER PRIMARY KEY,
	QSRQ			int		NOT NULL,
	JZRQ			int		NOT NULL,
	QCKC			int		NOT NULL,
	BQXG		int		NOT NULL,
	BQTH		int		NOT NULL,
	QMKC		int		NOT NULL,
	ZSFS		int		NOT NULL,
	ZFFS		int		NOT NULL,
	FSFS		int		NOT NULL,
	FFFS		int		NOT NULL,
	KFFS		int		NOT NULL,

	ZSJE		big int		NOT NULL,
	ZSSE		big int		NOT NULL,
	ZFJE		big int		NOT NULL,
	ZFSE		big int		NOT NULL,
	FSJE		big int		NOT NULL,
	FSSE		big int		NOT NULL,
	FFJE		big int		NOT NULL,
	FFSE		big int		NOT NULL
);
--
-- Dumping data for table `INV_SUM`
--

--for_initProc_20150506linzihao
--#########税率表###################
--#    序号          INTEGER
--#     税率         int
--#     备用		varchar(20)
--############################################
--
-- Table structure for table `TAX`
--
CREATE TABLE TAX(
	NO			INTEGER 		PRIMARY KEY,
	SL       	int,
	BACKUP		varchar(20)
);

--#########网络参数表###################
--#	是否自动分配IP	char,
--#	本机IP			varchar(20),
--#	本机网关		varchar(20),
--#	子网掩码		varchar(20),
--#	DNS				varchar(20),
--#	服务器IP地址	varchar(20),
--#	服务器端口号	varchar(6),
--#	服务器部署路径	varchar(50),
--#	ftp服务器地址	varchar(20),
--#	ftp端口号		varchar(6),
--#	用户名			varchar(16),
--#	密码			varchar(16)
--############################################
CREATE TABLE NETPARA(
	ISDHCP		char,
	LOCIP		varchar(20),
	LOCGATE		varchar(20),
	LOCMASK		varchar(20),
	LOCDNS		varchar(20),
	SERVIP		varchar(20),
	SERVPORT	varchar(6),
	SERVADDR	varchar(50),
	FTPIP		varchar(20),
	FTPPORT		varchar(6),
	FTPUSER		varchar(16),
	FTPPWD		varchar(16)
);


--建立表索引
CREATE index HEAD_NO		on 		INV_HEAD(FPDM, FPHM);
CREATE index HEAD_DATE		on 		INV_HEAD(KPRQ);
CREATE index DET_NO		on 		INV_DET(FPHM);
CREATE index DET_DATE		on 		INV_DET(KPRQ);
CREATE index DEPT_PLU	on DEPT(SP_BM);

--
-- Table structure for table `dept` 
--
INSERT INTO dept VALUES ('0000000000000001', '部类商品1', 10.0, 0.03, '个', 1, '001', '001', '1', 1.11, 100.0);
INSERT INTO dept VALUES ('0000000000000002', '部类商品2', 20.0, 0.17, '条', 2, '002', '002', '1', 1.22, 100.0);
INSERT INTO dept VALUES ('0000000000000003', '部类商品3', 30.0, 0.00, '扎', 3, '003', '003', '1', 1.33, 100.0);


--
-- init sysarg
--
insert into sysarg values(0,'',1,'',0);
insert into sysarg values(1,'',0,'个人',0);
insert into sysarg values(2,'',9600,'',0);
insert into sysarg values(3,'',9600,'',0);
insert into sysarg values(4,'',0,'',0);
insert into sysarg values(5,'',1,'',0);
insert into sysarg values(6,'',3,'',0);
insert into sysarg values(11,'',9600,'',0);

insert into sysarg values(50,'',0,'',0);
insert into sysarg values(51,'',0,'',0);
insert into sysarg values(52,'',0,'',0);
insert into sysarg values(53,'',0,'',0);
insert into sysarg values(54,'',0,'',0);
insert into sysarg values(55,'',0,'',0);
insert into sysarg values(56,'',0,'',0);
insert into sysarg values(57,'',0,'',0);
insert into sysarg values(58,'',0,'20140101020010',0);
insert into sysarg values(59,'',0,'',0);

insert into sysarg values(100,'',0,'88888888',0);
insert into sysarg values(101,'',0,'23456789',0);

--insert into sysarg values(20,'',0,'192.168.5.128',0);
--insert into sysarg values(21,'',0,'7001',0);
--insert into sysarg values(22,'',2,'',0);
--insert into sysarg values(26,'',0,'',0);
--insert into sysarg values(27,'',0,'',0);
--insert into sysarg values(32,'',1,'',0);


insert into sysarg values(201,'',1,'',0);
insert into sysarg values(202,'',10,'',0);
insert into sysarg values(203,'',0,'',0);
insert into sysarg values(204,'',0,'192.168.2.153',0);
insert into sysarg values(205,'',0,'255.255.255.0',0);
insert into sysarg values(206,'',0,'192.168.2.1',0);
insert into sysarg values(207,'',0,'192.168.204.2',0);
insert into sysarg values(208,'',0,'',0);
insert into sysarg values(209,'',0,'',0);
insert into sysarg values(210,'',1,'',0);
insert into sysarg values(211,'',0,'card',0);
insert into sysarg values(212,'',0,'card',0);
insert into sysarg values(213,'',0,'',0);
insert into sysarg values(214,'',0,'CMNET',0);
insert into sysarg values(215,'',0,'*99***1#',0);
insert into sysarg values(216,'',0,'',0);
insert into sysarg values(217,'',0,'',0);
insert into sysarg values(218,'',0,'',0);
insert into sysarg values(219,'',2,'',0);

insert into sysarg values(231,'',0,'192.168.0.104',0);
insert into sysarg values(232,'',0,'8002',0);
insert into sysarg values(233,'',0,'',0);

--
-- init Operator
--
INSERT INTO operator values(200, '系统', '198402', '1');
INSERT INTO operator values(0, '主管', '123456', '2');
INSERT INTO operator values(99, '学习', '123456', '4');

--
-- init Machine
--
INSERT INTO machine values('222222222222', '20110101', 'hver1.0', 'sver1.0');

--
-- init TAX
-- for test 20150508_linzihao
-- INSERT INTO TAX values(1, 17, NULL);
-- INSERT INTO TAX values(2, 3, NULL);

--
-- init Client
--
--INSERT INTO client values('9', '航天信息股份有限公司航天信息股份有限公司航天信息股份有限公司航天信息股份有限公司航天信息股份有限公司航天信息股份有限公司航天信息股份有限公司航天信息股份有限公司航天信息股份有限公司1234567890!@#sefeted');
insert into NETPARA values(1,'192.168.2.134','192.168.2.1','255.255.255.0','','192.168.0.104','8008','','','','','');
