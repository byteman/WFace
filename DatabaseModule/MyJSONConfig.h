#pragma once
#include "Poco/Util/JSONConfiguration.h"
#include "DatabaseModuleConfig.h"


class DATABASEMODULE_API MyJSONConfig:public Poco::Util::JSONConfiguration
{
public:
	MyJSONConfig(void);
	virtual ~MyJSONConfig(void);
	virtual void setInt(const std::string& key, int value);
	virtual void setBool(const std::string& key, bool value);
	virtual void setDouble(const std::string& key, double value);
	virtual void setString(const std::string& key, const std::string& value);
};

