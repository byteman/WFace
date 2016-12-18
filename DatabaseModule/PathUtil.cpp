#include "PathUtil.h"
#include "Poco/SingletonHolder.h"
#include "Poco/Path.h"
#include "Poco/File.h"
#include "CWLogger.h"
PathUtil::PathUtil(void)
{
	_root.clear();
}


PathUtil::~PathUtil(void)
{
}
PathUtil& PathUtil::get()
{
	static Poco::SingletonHolder<PathUtil> sh;
	return *sh.get();
}
std::string PathUtil::setPath(std::string subdir)
{
	Poco::Path path(_root+"/"+subdir);
#if 0
	Poco::File file(path);
	if(!file.exists() && file.isDirectory())
	{
		file.createDirectory();
	}
#endif
	return path.toString();
}
void PathUtil::setRootDir(std::string root)
{
	_root = root;
	Poco::Path path(_root);
	Poco::File file(path);
	if (!file.exists())
	{
		
		file.createDirectory();
	}
	_root = path.toString();
	_getDbPath  = _root;

	
	_webPath    = setPath("/cwsc1_web");

	_mimePath   = setPath("/mime.types");
	_configPath = setPath("/cwConfig.db");
	_faceDBPath = setPath("/cwFaceVerify.db");
	_LogDBPath = setPath("/cwLog.db");
	_logConfigPath= setPath("/logger.config");
    _livenessModelPath=setPath("/Models/face_liveness_model.bin");
    _faceQualityModelPath = setPath("/Models/face_quality_2_1.bin");
    _faceKeyModelPath = setPath("/Models/keypt_model_sdm_9pts.bin");
    _faceDetModelPath = setPath("/Models/faceDetector_2_1.mdl");
	_faceRegModelPath = setPath("/models/CWR_Config.txt");
	_downloadPath = setPath("/download/");
	cw_error("RootPath=%s\n",path.toString().c_str());
}
std::string PathUtil::getJSONPath(std::string file)
{

	Poco::Path path(_root + "/json/" +file);
	return path.toString();

}

std::string PathUtil::getFaceDetModelPath()
{
	return _faceDetModelPath;

}
std::string PathUtil::getFaceKeyPtModelPath()
{
	return _faceKeyModelPath;
}
std::string PathUtil::getFaceQualityModelPath()
{
	return _faceQualityModelPath;
}
std::string PathUtil::getFaceRegModelPath()
{
	return _faceRegModelPath;
}

std::string PathUtil::getWebRootDir()
{
	return _webPath;
}

std::string PathUtil::getFaceVerifyDBPath()
{
    return _faceDBPath;
}

std::string PathUtil::getLivenessModelPath()
{
    return _livenessModelPath;
}

std::string PathUtil::getConfigDBPath()
{
	return _configPath;
}
std::string PathUtil::getWebMimeFilePath()
{
	return _mimePath;
}

void PathUtil::testPath()
{
	cw_info("ConfigDBPath=%s\n",getConfigDBPath().c_str());
	cw_info("getFaceVerifyDBPath=%s\n",getFaceVerifyDBPath().c_str());
	cw_info("getWebRootDir=%s\n",getWebRootDir().c_str());
	cw_info("getFaceQualityModelPath=%s\n",getFaceQualityModelPath().c_str());
	cw_info("getFaceKeyPtModelPath=%s\n",getFaceKeyPtModelPath().c_str());
	cw_info("getFaceDetModelPath=%s\n",getFaceDetModelPath().c_str());
	cw_info("getJSONPath=%s\n",getJSONPath("video").c_str());
	cw_info("getWebMimeFilePath=%s\n",getWebMimeFilePath().c_str());
}

std::string PathUtil::getRootDir()
{
	 cw_error("getRootDir root1=%s",_root.c_str());
	 Poco::Path path(_root);
	 cw_error("getRootDir root2=%s",_root.c_str());
	 return path.toString();
}

std::string PathUtil::getDBRootDir()
{
	return getRootDir();
}

std::string PathUtil::getLogDBPath()
{
	return _LogDBPath;
}

std::string PathUtil::getDownloadDir()
{
	return _downloadPath;
}

std::string PathUtil::getLogConfigPath()
{
	return _logConfigPath;
}

