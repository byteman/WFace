
typedef struct {
	double 			Longitude;// 经度
	double 			Latitude;// 纬度
	unsigned char 	ns;	// 南北
	unsigned char 	ew;		// 东西
}GpsDef;

typedef struct {
	unsigned char 	year;	// 当前年减去2000,如2016年，year实际保存16。
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
}DateDef;

// A组内容-单点重量
typedef struct {	
	int 	wet;				// 单点重量、
	DateDef	wet_date;			// 单点重量的获取日期时间、
	GpsDef 	gps;				// GPS信息、
	char 	license_plate[12];	// 车辆号牌信息（或本机信息）、
	char	duty[16];			// 值班员（或司机信息）、
	DateDef	up_date;			// 发送的实时日期时间.
}PointWet;

// B组内容-总重量
typedef struct {	
	int 	wet;				// 总重量
	char 	license_plate[12];	// 车辆号牌信息（或本机信息）、	
	GpsDef 	gps;				// GPS信息、
	DateDef	up_date;			// 发送的实时日期时间.
}TotalWet;

// C组内容-排水重量
typedef struct {	
	int 	wet;				// 排水重量、
	char 	license_plate[12];	// 车辆号牌信息（或本机信息）、	
	GpsDef 	gps;				// GPS信息、
	DateDef	up_date;			// 发送的实时日期时间.
}DrainageWet;


// 设备状态
typedef struct {
	unsigned short	snsr_line;		// 传感器在线状态，
									// 每个bit表示对应传感器是否在线。0bit表示第一个传感器，依次类推。
									// 0表示传感器不在线，1表示在线。
	
	unsigned char	snsr_exc;		// 传感器故障情况,
									// 0没有异常,1断线,2短路,3输出电压范围错误
	
	unsigned char	gpr_exc;		// gps模块
	unsigned char	gprs_exc;		// gprs模块
	unsigned char	wet_exc;		// 超载情况,
									// 没有超载值为0，超载时值为当前重量
}DevInf;


