#include "cwLog.h"
#include "cwSCSqlite.h"

cwLog::LogLevel cwLog::m_level = cwLog::LogLevel::L_ERROR;

cwLog::cwLog(void)
{
}


cwLog::~cwLog(void)
{
}

void cwLog::setLevel(LogLevel level)
{
	m_level = level;
}
	
void cwLog::WriteLog(LogLevel level, std::string text)
{
	if(level >= m_level)
	{
		CloudWalk::LogRecord lr;
		Poco::DateTime now;
		lr.setOperTime(now);
		lr.setType(level);
		lr.setMsg(text);
		std::vector<CloudWalk::LogRecord> vlr;
		vlr.push_back(lr);
		cwSqlite cq;
		cq.InsertLogRecord(vlr);
	}
	else
	{
		return;
	}
}
