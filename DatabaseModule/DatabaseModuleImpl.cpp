#include "DatabaseModuleImpl.h"
#include "cwSCSqlite.h"
#include "SysPara.h"

DatabaseModuleImpl::DatabaseModuleImpl()
{

}

DatabaseModuleImpl::~DatabaseModuleImpl()
{

}

void DatabaseModuleImpl::start()
{
	//_LogTrace<<"--------数据库模块启动"<<__DATE__ <<" "<<__TIME__<<_LogTraceEnd;
	
	SysPara::get().parse();
	
}

void DatabaseModuleImpl::stop()
{
	
}

DatabaseModuleImpl&DatabaseModuleImpl::instance()
{
	static  CORE::SingletonHolder<DatabaseModuleImpl> sh;
	return *sh.get();
}

int DatabaseModuleImpl::connect(std::string pDbPath)
{
	
	return 0;
}

int DatabaseModuleImpl::disconnect()
{
	return 0;
}


CORE::CData DatabaseModuleImpl::getParam(const char *Section,const  char *Name )
{
	//CORE::Core_FastMutex::ScopedLock lock(m_paramMutex);
	//return m_CDbParam.getParam(Section, Name);
	return "";
}

bool DatabaseModuleImpl::setParam(const char *Section,const  char *Name , const char *value)
{
	//CORE::Core_FastMutex::ScopedLock lock(m_TorQueMutex);
	//return m_CDbParam.setParam( Section, Name , value);
	return "";
}