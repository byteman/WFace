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
	double 			Longitude;// ����
	double 			Latitude;// γ��
	unsigned char 	ns;	// �ϱ�
	unsigned char 	ew;		// ����
}GpsDef;

typedef struct {
	unsigned char 	year;	// ��ǰ���ȥ2000,��2016�꣬yearʵ�ʱ���16��
	unsigned char	month;
	unsigned char	day;
	unsigned char	hour;
	unsigned char	min;
	unsigned char	sec;
}DateDef;

// A������-��������
typedef struct {	
	int 	wet;				// ����������
	DateDef	wet_date;			// ���������Ļ�ȡ����ʱ�䡢
	GpsDef 	gps;				// GPS��Ϣ��
	char 	license_plate[12];	// ����������Ϣ���򱾻���Ϣ����
	char	duty[16];			// ֵ��Ա����˾����Ϣ����
	DateDef	up_date;			// ���͵�ʵʱ����ʱ��.
}PointWet;

// B������-������
typedef struct {	
	int 	wet;				// ������
	char 	license_plate[12];	// ����������Ϣ���򱾻���Ϣ����	
	GpsDef 	gps;				// GPS��Ϣ��
	DateDef	up_date;			// ���͵�ʵʱ����ʱ��.
}TotalWet;

// C������-��ˮ����
typedef struct {	
	int 	wet;				// ��ˮ������
	char 	license_plate[12];	// ����������Ϣ���򱾻���Ϣ����	
	GpsDef 	gps;				// GPS��Ϣ��
	DateDef	up_date;			// ���͵�ʵʱ����ʱ��.
}DrainageWet;


// �豸״̬
typedef struct {
	unsigned short	snsr_line;		// ����������״̬��
									// ÿ��bit��ʾ��Ӧ�������Ƿ����ߡ�0bit��ʾ��һ�����������������ơ�
									// 0��ʾ�����������ߣ�1��ʾ���ߡ�
	
	unsigned char	snsr_exc;		// �������������,
									// 0û���쳣,1����,2��·,3�����ѹ��Χ����
	
	unsigned char	gpr_exc;		// gpsģ��
	unsigned char	gprs_exc;		// gprsģ��
	unsigned char	wet_exc;		// �������,
									// û�г���ֵΪ0������ʱֵΪ��ǰ����
}DevInf;

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif

