#pragma once
#include <string>
//#include "Poco/Path.h"
#include "DatabaseModuleConfig.h"

class DATABASEMODULE_API PathUtil
{
public:
	PathUtil(void);
	~PathUtil(void);
	static PathUtil& get();
	//设置根文件路径，下面所有文件都是基于根目录路径.
	void setRootDir(std::string root);
	std::string getRootDir();

	std::string getDBRootDir();
	std::string getDownloadDir();
	//JSON文件路径
	std::string getJSONPath(std::string file);
	//算法DetModel路径
	std::string getFaceDetModelPath();
	//算法KeyPtModel路径
	std::string getFaceKeyPtModelPath();
	//算法QualityModel路径
	std::string getFaceQualityModelPath();
	//人脸识别算法路径
	std::string getFaceRegModelPath();
	std::string getLogDBPath();
	//Web根目录路径
	std::string getWebRootDir();
	//web mimes文件路径
	std::string getWebMimeFilePath();
	//算法人脸库和识别结果
	std::string getFaceVerifyDBPath();
    //liveness model
    std::string getLivenessModelPath();
	//配置文件数据库路径
	std::string getConfigDBPath();
	//获取日志配置文件路径
	std::string getLogConfigPath();
	//测试所有的路径.
	void testPath();
private:
	std::string setPath(std::string subdir);
	//Poco::Path _root;
	std::string _root;
	std::string _getDbPath;
	std::string _webPath;
	std::string _mimePath;
	std::string _configPath;
	std::string _faceQualityModelPath;
	std::string _faceKeyModelPath;
	std::string _faceDetModelPath;
	std::string _faceRegModelPath;
    std::string _livenessModelPath;
	std::string _LogDBPath;
	std::string _faceDBPath;
	std::string _downloadPath;
	std::string _logConfigPath;
};

