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
	//���ø��ļ�·�������������ļ����ǻ��ڸ�Ŀ¼·��.
	void setRootDir(std::string root);
	std::string getRootDir();

	std::string getDBRootDir();
	std::string getDownloadDir();
	//JSON�ļ�·��
	std::string getJSONPath(std::string file);
	//�㷨DetModel·��
	std::string getFaceDetModelPath();
	//�㷨KeyPtModel·��
	std::string getFaceKeyPtModelPath();
	//�㷨QualityModel·��
	std::string getFaceQualityModelPath();
	//����ʶ���㷨·��
	std::string getFaceRegModelPath();
	std::string getLogDBPath();
	//Web��Ŀ¼·��
	std::string getWebRootDir();
	//web mimes�ļ�·��
	std::string getWebMimeFilePath();
	//�㷨�������ʶ����
	std::string getFaceVerifyDBPath();
    //liveness model
    std::string getLivenessModelPath();
	//�����ļ����ݿ�·��
	std::string getConfigDBPath();
	//��ȡ��־�����ļ�·��
	std::string getLogConfigPath();
	//�������е�·��.
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

