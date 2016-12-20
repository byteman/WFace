////////////////////////////////////////////////////////////////
//config项的数据定义
////////////////////////////////////////////////////////////////
#ifndef CONFIG_ITEM_H
#define CONFIG_ITEM_H
#include <string>
#include "Poco/NumberParser.h"

namespace CloudWalk{
class  ConfigItem
{
public:
	ConfigItem()
	{
		_Key = "";
		_Value = "";
	}

	ConfigItem(const std::string& key, const std::string& value):_Key(key), _Value(value)
	{

	}

	const std::string& getKey() const
	{
		return _Key;
	}

	void setKey(const std::string& key)
	{
		_Key = key;
	}

	const std::string& getValue() const
	{
		return _Value;
	}
	const int getIntValue() const
	{
	
		return Poco::NumberParser::parse(getValue());
	}
	const int getIntDefValue(int value) 
	{

		int v;
		if(Poco::NumberParser::tryParse(getValue(),v))
		{
			return v;

		}
		return value;
	}
	void setValue(const std::string& value)
	{
		_Value = value;
	}

private:
	std::string _Key;
	std::string _Value;
};
}
#endif
