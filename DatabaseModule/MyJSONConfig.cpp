#include "MyJSONConfig.h"


MyJSONConfig::MyJSONConfig(void)
{
}


MyJSONConfig::~MyJSONConfig(void)
{
}

void MyJSONConfig::setInt(const std::string& key, int value)
{
	if(has(key))
	{
		remove(key);
	}
	JSONConfiguration::setInt(key,value);
}
void MyJSONConfig::setBool(const std::string& key, bool value)
{
	if(has(key))
	{
		remove(key);
	}
	JSONConfiguration::setBool(key,value);
}
void MyJSONConfig::setDouble(const std::string& key, double value)
{
	if(has(key))
	{
		remove(key);
	}
	JSONConfiguration::setDouble(key,value);
}
void MyJSONConfig::setString(const std::string& key, const std::string& value)
{
	if(has(key))
	{
		remove(key);
	}
	JSONConfiguration::setString(key,value);
}