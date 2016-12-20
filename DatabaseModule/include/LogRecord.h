////////////////////////////////////////////////////////////////
//系统日志表的数据定义
////////////////////////////////////////////////////////////////
#ifndef LOG_RECORD_H
#define LOG_RECORD_H
#include <string>
#include "Poco/Data/LOB.h"
namespace CloudWalk{
class LogRecord
{
public:
	LogRecord()
	{
		_OperTime = Poco::DateTime();
			_Type = 0;
			 _Msg = "";
	}

//	LogRecord(const std::string& username, const std::string& password, int role):_UserName(username), _Password(password), _Role(role)
//	{
//
//	}

	const Poco::DateTime& getOperTime()
	{
		return _OperTime;
	}
	void setOperTime(const Poco::DateTime& opertime)
	{
		_OperTime = opertime;
	}

	int getType()
	{
		return _Type;
	}
	void setType(int type)
	{
		_Type = type;
	}

	std::string getMsg()
	{
		return _Msg;
	}
	void setMsg(std::string msg)
	{
		_Msg = msg;
	}
public:
	Poco::DateTime _OperTime;
	int _Type;
	std::string _Msg;
};
}
#endif
