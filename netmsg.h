#ifndef NET_MSG_H
#define NET_MSG_H

#ifdef __cplusplus
extern "C"{
#endif

#pragma pack(push) // ����ǰpack����ѹջ����
#pragma pack(1)// �����ڽṹ�嶨��֮ǰʹ��

typedef struct
{
    unsigned short dev_id;
    unsigned char  dir;
    unsigned char  cmd;
    unsigned char  oper;
    unsigned short len;
}Msg_Head;



typedef struct {
        double 			Longitude;	// ����
        double 			Latitude;	// γ��
        unsigned char 	ns;			// �ϱ�ֵΪ,'n'��'s'
        unsigned char 	ew;			// ����,'e'��'w'
}GpsDef;


typedef struct {
        unsigned char 	year;		// ��ǰ���ȥ2000,��2016�꣬yearʵ�ʱ���16��
        unsigned char	month;
        unsigned char	day;
        unsigned char	hour;
        unsigned char	min;
        unsigned char	sec;
}DateDef;

#define		LICENSE_LEN		10
#define		DUTY_LEN		16

// A������-��������
typedef struct {
        int 	wet;						// ����������
        DateDef	wet_date;					// ���������Ļ�ȡ����ʱ�䡢
        GpsDef 	gps;						// GPS��Ϣ��
        char 	license_plate[LICENSE_LEN];	// ����������Ϣ���򱾻���Ϣ����
        char	duty[DUTY_LEN];				// ֵ��Ա����˾����Ϣ����
        DateDef	up_date;					// ���͵�ʵʱ����ʱ��.
}PointWet;

// B������-������
typedef struct {
        int 	wet;						// ������
        char 	license_plate[LICENSE_LEN];	// ����������Ϣ���򱾻���Ϣ����
        GpsDef 	gps;						// GPS��Ϣ��
        DateDef	up_date;					// ���͵�ʵʱ����ʱ��.
}TotalWet;

// C������-��ˮ����
typedef struct {
        int 	wet;						// ��ˮ������
        char 	license_plate[LICENSE_LEN];	// ����������Ϣ���򱾻���Ϣ����
        GpsDef 	gps;						// GPS��Ϣ��
        DateDef	up_date;					// ���͵�ʵʱ����ʱ��.
}DrainageWet;


// �豸״̬
typedef struct {
        unsigned short	snsr_line;		// ����������״̬��
                                                                        // ÿ��bit��ʾ��Ӧ�������Ƿ����ߡ�0bit��ʾ��һ�����������������ơ�
                                                                        // 0��ʾ�����������ߣ�1��ʾ���ߡ�

        unsigned char	snsr_exc;		// �������������,
                                                                        // 0û���쳣,1����,2��·,3�����ѹ��Χ����

                         char	gpr_exc;		// gpsģ��,0����;>1��������;-1���߶�·;-2���߶�·;-3ģ���쳣
        unsigned char	gprs_exc;		// gprsģ��,0����;1�쳣
        unsigned char	wet_exc;		// �������,
                                                                        // û�г���ֵΪ0������ʱֵΪ��ǰ����

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
        cws_cmd_wet_one = 1,	// ����
        cws_cmd_wet_dump,		// ж��
        cws_cmd_wet_drain,		// ��ˮ
        cws_cmd_gps,			// gps��Ϣ
        cws_cmd_dev_inf,		// �豸��Ϣ
        cws_cmd_heart,			// ����
        cws_cmd_file,			// �����ļ�
        cws_cmd_dev_rst,		// �豸��λ

        cws_cmd_dev_rev,		// �ն˰汾
        cws_cmd_wet_rt,			// ʵʱ����
        cws_cmd_gps_mode,		// gpsģ��
        cws_cmd_gps_uptime,		// Gps�ϴ�ʱ��������
        cws_cmd_dev_uptime,		// �豸״̬�ϴ�ʱ����
}cws_cmd_def;




#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif

