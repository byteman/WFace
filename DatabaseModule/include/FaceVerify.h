////////////////////////////////////////////////////////////////
//人脸识别的数据定义
////////////////////////////////////////////////////////////////
#ifndef FACE_VERIFY_H
#define FACE_VERIFY_H
#include <string>
#include "Poco/Data/LOB.h"
namespace CloudWalk{
class FaceVerify
{
public:
	FaceVerify()
	{
		_VerifyTime = Poco::DateTime();
			   _Age = 0;
		      _Male = true;
		   _Glasses = true; 
			_Xangle = 0;
			_Yangle = 0; 
			_Zangle = 0; 
			 _Smile = 0; 
		   _Quality = 0.0;
	         _Score = "";
			   _Img = Poco::Data::BLOB();
		  _SendFlag = false; 
                                 this->_XX = 0;
			     _Y = 0; 
			 _Width = 0; 
			_Height = 0;
			 _RecNo = 0;
			_Imgf = Poco::Data::BLOB();
	}
	int getRecNo()
	{
		return _RecNo;
	}
	void setRecNo(int recno)
	{
		_RecNo = recno;
	}

	const Poco::DateTime& getVerifyTime()
	{
		return _VerifyTime;
	}
	void setVerifyTime(const Poco::DateTime& verifytime)
	{
		_VerifyTime = verifytime;
	}

	int getAge()
	{
		return _Age;
	}
	void setAge(int age)
	{
		_Age = age;
	}

	bool getMale()
	{
		return _Male;
	}
	void setMale(bool male)
	{
		_Male = male;
	}

	bool getGlasses()
	{
		return _Glasses;
	}
	void setGlasses(bool glasses)
	{
		_Glasses = glasses;
	}

	int getXangle()
	{
		return _Xangle;
	}
	void setXangle(int xangle)
	{
		_Xangle = xangle;
	}

	int getYangle()
	{
		return _Yangle;
	}
	void setYangle(int yangle)
	{
		_Yangle = yangle;
	}

	int getZangle()
	{
		return _Zangle;
	}
	void setZangle(int zangle)
	{
		_Zangle = zangle;
	}

	int getSmile()
	{
		return _Smile;
	}
	void setSmile(int smile)
	{
		_Smile = smile;
	}

	float getQuality()
	{
		return _Quality;
	}
	void setQuality(float quality)
	{
		
		_Quality = quality;
	}

	std::string getScore()
	{
		return _Score;
	}
	void setScore(std::string score)
	{
		_Score = score;
	}

	const Poco::Data::BLOB& getImg()
	{
		return _Img;
	}
	void setImg(const Poco::Data::BLOB& img)
	{
		_Img = img;
	}

	bool getSendFlag()
	{
		return _SendFlag;
	}
	void setSendFlag(bool sendflag)
	{
		_SendFlag = sendflag;
	}

	int getX()
	{
                return _XX;
	}
	void setX(int x)
	{
                _XX = x;
	}

	int getY()
	{
		return _Y;
	}
	void setY(int y)
	{
		_Y = y;
	}

	int getWidth()
	{
		return _Width;
	}
	void setWidth(int width)
	{
		_Width = width;
	}

	int getHeight()
	{
		return _Height;
	}
	void setHeight(int height)
	{
		_Height = height;
	}

	//add 2015-11-25
	const Poco::Data::BLOB& getImgf()
	{
		return _Imgf;
	}
	void setImgf(const Poco::Data::BLOB& img)
	{
		_Imgf = img;
	}

private:
			int _RecNo;
	Poco::DateTime _VerifyTime;
			int _Age; 
		   bool _Male;
		   bool _Glasses; 
			int _Xangle;
			int _Yangle; 
			int _Zangle; 
			int _Smile; 
		  float _Quality;
	std::string _Score;
	Poco::Data::BLOB _Img;
		   bool _SendFlag; 
                        int _XX;
			int _Y; 
			int _Width; 
			int _Height;
			//add 2015-11-25
	Poco::Data::BLOB _Imgf;
};
}
#endif
