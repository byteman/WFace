#ifndef NET_MSG_H
#define NET_MSG_H

#ifdef __cplusplus
extern "C"{
#endif

#pragma pack(push) // 将当前pack设置压栈保存
#pragma pack(1)// 必须在结构体定义之前使用

typedef struct
{
    unsigned short dev_id;
    unsigned char  dir;
    unsigned char  cmd;
    unsigned char  oper;
    unsigned short len;
}Msg_Head;



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


typedef struct {
        unsigned short	dev_addr;
        unsigned char	dir;
        unsigned char	cmd;
        unsigned char	type;
        unsigned short	len_data;
        void 			*data;
        unsigned short	crc;
}CommuDef;


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




#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif

