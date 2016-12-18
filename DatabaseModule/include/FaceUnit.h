////////////////////////////////////////////////////////////////
//人脸库的数据定义
////////////////////////////////////////////////////////////////
#ifndef FACE_UNIT_H
#define FACE_UNIT_H

#include <string>
#include "Poco/Data/LOB.h"

namespace CloudWalk{
	class FaceUnit
	{
public:
	FaceUnit()
	{
	 	 _Id = "";
		 _DownloadTime = Poco::DateTime();
		 _Img = Poco::Data::BLOB();
		 _Feature = Poco::Data::BLOB();
		 _Tag = "";
	}

	const std::string& getId()
	{
		return _Id;
	}

	void setId(const std::string& id)
	{
		_Id = id;
	}

	const Poco::DateTime& getDownloadTime()
	{
		return _DownloadTime;
	}

	void setDownloadTime(const Poco::DateTime& downloadtime)
	{
		_DownloadTime = downloadtime;
	}

	const Poco::Data::BLOB& getFeature()
	{
		return _Feature;
	}

	void setFeature(const Poco::Data::BLOB& feature)
	{
		_Feature = feature;
	}

	const Poco::Data::BLOB& getImg()
	{
		return _Img;
	}

	void setImg(const Poco::Data::BLOB& img)
	{
		_Img = img;
	}

	const std::string& getTag()
	{
		return _Tag;
	}

	void setTag(const std::string& tag)
	{
		_Tag = tag;
	}

	private:
	 		 std::string _Id;
		  Poco::DateTime _DownloadTime;
		Poco::Data::BLOB _Img;
		Poco::Data::BLOB _Feature;
			 std::string _Tag;
	};
}
#endif
