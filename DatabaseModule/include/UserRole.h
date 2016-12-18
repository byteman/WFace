////////////////////////////////////////////////////////////////
//用户表的数据定义
////////////////////////////////////////////////////////////////
#ifndef USER_ROLE_H
#define USER_ROLE_H
#include <string>
namespace CloudWalk{
class UserRole
{
public:
	UserRole()
	{
		_UserName = "";
		_Password = "";
		_Role = 0;
	}

	UserRole(const std::string& username, const std::string& password, int role):_UserName(username), _Password(password), _Role(role)
	{

	}

	const std::string& getUserName()
	{
		return _UserName;
	}
	void setUserName(const std::string& username)
	{
		_UserName = username;
	}

	const std::string& getPassword()
	{
		return _Password;
	}
	void setPassword(const std::string& password)
	{
		_Password = password;
	}

	int getRole()
	{
		return _Role;
	}
	void setRole(int role)
	{
		_Role = role;
	}
	

public:
	std::string _UserName;
	std::string _Password;
			int _Role;
};
}
#endif
