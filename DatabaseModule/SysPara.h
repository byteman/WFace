#pragma once

#include "DatabaseModuleConfig.h"
#include "DBConfiguration.h";
//视频流参数
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
	int width,height; //视频尺寸.
	int fps;	//帧率
	int bitRate; //码率
	int gop;//I帧间隔
	int bitRateType; //码率类型, 0 定码率 1 变码率,定质量

};
//图像质量参数范围 0 - 100
struct ImagePara{
	int brightness; //亮度
	int saturation; //饱和度
	int contrast; //对比度
	int sharpness; //锐度
	int aperturetype; //光圈类型
	int autoaperture; //自动光圈灵敏度
	int exposuretime; //曝光时间
	int backlit;//	背光补偿区域 ...
};
//人脸算法参数
struct FaceAlgoPara{
	int expos;//	人脸曝光		是		1：启用 -1：关闭
	int roi;//	启用人脸ROI	是		1：启用 -1：关闭
	float	minfacesize;//	最小人脸尺寸		是		
	float	maxfacesize;//	最大人脸尺寸		是		
	int	drawrect;//	绘制人脸框		是		1：绘制 -1：不画
	int	age;//	年龄		是		1：启用 -1：关闭
	int	sex;//	性别		是		1：启用 -1：关闭
	int	glass;//	眼镜		是		1：启用 -1：关闭
	int	smile;//	微笑		是		1：启用 -1：关闭
	float	threshold;//	识别阈值		是		
	float	qualityscore;//	达到识别质量分数		是		
	int	version;//	人脸库大小		是		
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
     * @brief loadDefaultPara 从配置文件中加载指定类型的默认参数到JSON对象中.
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

