#include "SysPara.h"
#include "cwSCSqlite.h"
#include "Poco/SingletonHolder.h"
#include "MyJSONConfig.h"
#include "PathUtil.h"
#include "CWLogger.h"
struct Rect
{
	int width;
	int height;
};
static Rect ImageRect[] = {
	{
	640,480
	},
	{
	720,576
	},
	{
	1280,720
	},
	{
	1920,1080
	},
};
static int fpsList[] = {
	25,15,10,5
};
static int bitRateList[] = {
	8192000,4096000,2048000,1024000
};
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

SysPara::SysPara(void)
{
}

SysPara& SysPara::get()
{
	static Poco::SingletonHolder<SysPara> sh;
	return *sh.get();
}
SysPara::~SysPara(void)
{
}
void SysPara::parseVideoFps(int index)
{
	index--;
	if(index < 0 || index >= ARRAY_SIZE(fpsList))
	{
		index = 0;
	}
	_videoPara.fps = fpsList[index];
	
}
void SysPara::parseVideoBitrate(int index)
{
	index--;
	if(index < 0 || index >= ARRAY_SIZE(bitRateList))
	{
		index = 0;
	}
	_videoPara.bitRate = bitRateList[index];
}
void SysPara::parseVideoSize(int index)
{
	index--;
	if(index < 0 || index >= ARRAY_SIZE(ImageRect))
	{
		index = 0;
	}
	_videoPara.width  = ImageRect[index].width;
	_videoPara.height = ImageRect[index].height;
}
//static std::string params_list[] = {"roi","ports","rs232","rs485",}
/**
    json�ļ��д����Ĭ�ϲ���
    ��ѯ���ݿ����Ƿ��ж�Ӧ�Ĳ��������û�У�����json�е�Ĭ�ϲ��������ݿ��С��Ա�������ģ�����
*/
bool SysPara::load(DBConfiguration &config,std::string name)
{
	try
	{
		MyJSONConfig jconf;
		jconf.load(PathUtil::get().getJSONPath(name+".json"));
		if(!config.exist(name))
		{
			MyJSONConfig::Keys keys;
			jconf.keys("data",keys); //��ȡjson��Ϣ�����е�keys

			for(int i = 0 ;i < keys.size();i++)
			{
				//printf("key[%d]=%s\r\n",i+1,keys[i].c_str());
				//����JSON��Ϣ�е�key-value�����ݿ�.
				std::string key = keys[i];
				std::size_t n = key.rfind("list");
				if(n != std::string::npos)
				{
					continue;
				}
				std::string t_key = name + "." + keys[i];
				std::string t_value=jconf.getString("data."+keys[i]);
				config.setString( t_key , t_value);
			}
		}
	}
	catch(Poco::Exception& e)
	{
		cw_error("init default param error %s\n",e.displayText().c_str());
		return false;
	}
	return true;
	
}

bool SysPara::parse(void)
{
		
	cwSqlite &sql = cwSqlite::instance();
	DBConfiguration &config = sql.getParamConfig();
	load(config,"ports");
	load(config,"roi");
    load(config,"times");
	load(config,"rs232");
	load(config,"rs485");

	return true;
}
