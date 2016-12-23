
#define		DATA_UP		0xA5
#define		DATA_DOWN	0x5A

#pragma pack (1) 
typedef struct {
	double 			Longitude;	// 经度
	double 			Latitude;	// 纬度
	unsigned char 	ns;			// 南北值为,'n'或's'
	unsigned char 	ew;			// 东西,'e'或'w'
}GpsDef;


typedef struct {
	unsigned char 	year;		// 当前年减去2000,如2016年，year实际保存16。
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
}DateDef;

#define		LICENSE_LEN		10
#define		DUTY_LEN		16

// A组内容-单点重量
typedef struct {	
	int 	wet;						// 单点重量、
	DateDef	wet_date;					// 单点重量的获取日期时间、
	GpsDef 	gps;						// GPS信息、
	char 	license_plate[LICENSE_LEN];	// 车辆号牌信息（或本机信息）、
	char	duty[DUTY_LEN];				// 值班员（或司机信息）、
	DateDef	up_date;					// 发送的实时日期时间.
}PointWet;

// B组内容-总重量
typedef struct {	
	int 	wet;						// 总重量
	char 	license_plate[LICENSE_LEN];	// 车辆号牌信息（或本机信息）、	
	GpsDef 	gps;						// GPS信息、
	DateDef	up_date;					// 发送的实时日期时间.
}TotalWet;

// C组内容-排水重量
typedef struct {	
	int 	wet;						// 排水重量、
	char 	license_plate[LICENSE_LEN];	// 车辆号牌信息（或本机信息）、	
	GpsDef 	gps;						// GPS信息、
	DateDef	up_date;					// 发送的实时日期时间.
}DrainageWet;


// 设备状态
typedef struct {
	unsigned short	snsr_line;		// 传感器在线状态，
									// 每个bit表示对应传感器是否在线。0bit表示第一个传感器，依次类推。
									// 0表示传感器不在线，1表示在线。
	
	unsigned char	snsr_exc;		// 传感器故障情况,
									// 0没有异常,1断线,2短路,3输出电压范围错误
	
			 char	gpr_exc;		// gps模块,0正常;>1搜星数量;-1天线断路;-2天线短路;-3模块异常
	unsigned char	gprs_exc;		// gprs模块,0正常;1异常
	unsigned char	wet_exc;		// 超载情况,
									// 没有超载值为0，超载时值为当前重量
	
}DevInf;


// 文件传输结构
typedef struct {
	int 			mode;			// -1表示开始文件传输
	unsigned int	file_type;		// 0普通文件，1升级文件
	unsigned int 	file_size;		// 传输的文件大小
	char			file_name[20];	// 使用8.3文件名规则,字符串保存，空余填零
}fileStartDef;

typedef struct {
	int 			pack;			// 当前传输第几包数据
	unsigned short	pack_size;		// 当前文件数据包的大小,可以选128，512，默认128
	char			*data;			// 具体文件数据长度由 pack_size决定
}fileRecvDef;

typedef struct {
	int 			mode;			// -2表示文件传输结束
}fileEndDef;

enum {
	FILE_START = -1,
	FILE_END = -2,
};





typedef struct {
	unsigned short	dev_addr;
	unsigned char	dir;
	unsigned char	cmd;
	unsigned char	type;		
	unsigned short	len_data;
	char 			*data;
	unsigned short	crc;
}CommuDef;

#pragma pack () 


typedef enum {
	cws_type_up = 1,
	cws_type_rd,
	cws_type_wr,
	cws_type_back,			

}cws_cmd_type;

typedef enum _cws_cmd{
	cws_cmd_wet_one = 1,	// 单次
	cws_cmd_wet_dump,		// 卸载
	cws_cmd_wet_drain,		// 排水
	cws_cmd_gps,			// gps信息
	cws_cmd_dev_inf,		// 设备信息
	cws_cmd_heart,			// 心跳
	cws_cmd_file,			// 升级文件
	cws_cmd_dev_rst,		// 设备复位
	
	cws_cmd_dev_rev,		// 终端版本
	cws_cmd_wet_rt,			// 实时重量
	cws_cmd_gps_mode,		// gps模块
	cws_cmd_gps_uptime,		// Gps上传时间间隔设置
	cws_cmd_dev_uptime,		// 设备状态上传时间间隔
}cws_cmd_def;




extern GpsDef	gpsInf;


extern int cws_protocol_up(unsigned char cmd, unsigned char type, unsigned short len, char *ptr, void *cb);
