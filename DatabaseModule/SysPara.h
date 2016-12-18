#pragma once

#include "DatabaseModuleConfig.h"
#include "DBConfiguration.h";
//��Ƶ������
struct VideoPara
{
	VideoPara()
	{
		width = 1280;
		height = 720;
		fps = 10;
		bitRate = 1024000;
		gop = 10;
		bitRateType = 0;
	}
	int width,height; //��Ƶ�ߴ�.
	int fps;	//֡��
	int bitRate; //����
	int gop;//I֡���
	int bitRateType; //��������, 0 ������ 1 ������,������

};
//ͼ������������Χ 0 - 100
struct ImagePara{
	int brightness; //����
	int saturation; //���Ͷ�
	int contrast; //�Աȶ�
	int sharpness; //���
	int aperturetype; //��Ȧ����
	int autoaperture; //�Զ���Ȧ������
	int exposuretime; //�ع�ʱ��
	int backlit;//	���ⲹ������ ...
};
//�����㷨����
struct FaceAlgoPara{
	int expos;//	�����ع�		��		1������ -1���ر�
	int roi;//	��������ROI	��		1������ -1���ر�
	float	minfacesize;//	��С�����ߴ�		��		
	float	maxfacesize;//	��������ߴ�		��		
	int	drawrect;//	����������		��		1������ -1������
	int	age;//	����		��		1������ -1���ر�
	int	sex;//	�Ա�		��		1������ -1���ر�
	int	glass;//	�۾�		��		1������ -1���ر�
	int	smile;//	΢Ц		��		1������ -1���ر�
	float	threshold;//	ʶ����ֵ		��		
	float	qualityscore;//	�ﵽʶ����������		��		
	int	version;//	�������С		��		
};
class DATABASEMODULE_API SysPara
{
public:
	static SysPara& get();
	SysPara(void);
	~SysPara(void);
	bool parse(void);
	VideoPara& getVideoPara()
	{
		return _videoPara;
	}
	ImagePara& getImagePara()
	{
		return _imagePara;
	}
    /**
     * @brief loadDefaultPara �������ļ��м���ָ�����͵�Ĭ�ϲ�����JSON������.
     * @param name
     * @param para
     * @return
     */

private:
	
	VideoPara _videoPara;
	ImagePara _imagePara;
	FaceAlgoPara _facePara;
private:
	void parseVideoSize(int index);
	void parseVideoFps(int index);
	void parseVideoBitrate(int index);

    bool load(DBConfiguration &config,std::string name);

};

