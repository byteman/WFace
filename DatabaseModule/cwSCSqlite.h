#ifndef SQLiteTest_INCLUDED
#define SQLiteTest_INCLUDED

#include "ConfigItem.h"
#include "UserRole.h"
#include "FaceUnit.h"
#include "FaceVerify.h"
#include "LogRecord.h"
#include <vector>
#include "Poco/Data/Date.h"
#include "Poco/Data/Time.h"
#include "Poco/Data/LOB.h"
#include "Poco/Data/Statement.h"
#include "Poco/Data/RecordSet.h"
#include "Poco/Data/SQLChannel.h"
#include "Poco/Data/SessionFactory.h"
#include "Poco/Data/SQLite/Connector.h"
#include "Poco/Data/SQLite/Utility.h"
#include "Poco/Data/SQLite/Notifier.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/Data/TypeHandler.h"
#include "Poco/Nullable.h"
#include "Poco/Data/Transaction.h"
#include "Poco/Data/DataException.h"
#include "Poco/Data/SQLite/SQLiteException.h"
#include "Poco/Tuple.h"
#include "Poco/Any.h"
#include "Poco/SharedPtr.h"
#include "Poco/DynamicAny.h"
#include "Poco/DateTime.h"
#include "Poco/Logger.h"
#include "Poco/Message.h"
#include "Poco/Thread.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "Poco/RefCountedObject.h"
#include "Poco/Stopwatch.h"
#include "Poco/Delegate.h"
#include <iostream>
#include <sstream>

using namespace Poco::Data::Keywords;
using Poco::Data::Session;
using Poco::Data::Statement;
using Poco::Data::RecordSet;
using Poco::Data::Column;
using Poco::Data::Row;
using Poco::Data::SQLChannel;
using Poco::Data::LimitException;
using Poco::Data::CLOB;
using Poco::Data::Date;
using Poco::Data::Time;
using Poco::Data::Transaction;
using Poco::Data::AbstractExtractionVec;
using Poco::Data::AbstractExtractionVecVec;
using Poco::Data::AbstractBindingVec;
using Poco::Data::NotConnectedException;
using Poco::Data::SQLite::Notifier;
using Poco::Nullable;
using Poco::Tuple;
using Poco::Any;
using Poco::AnyCast;
using Poco::DynamicAny;
using Poco::DateTime;
using Poco::Logger;
using Poco::Message;
using Poco::AutoPtr;
using Poco::Thread;
using Poco::format;
using Poco::InvalidAccessException;
using Poco::RangeException;
using Poco::BadCastException;
using Poco::NotFoundException;
using Poco::NullPointerException;
using Poco::TimeoutException;
using Poco::NotImplementedException;
using Poco::Data::SQLite::ConstraintViolationException;
using Poco::Data::SQLite::ParameterCountMismatchException;
using Poco::Int32;
using Poco::Dynamic::Var;
using Poco::Data::SQLite::Utility;
using Poco::delegate;

#define USING_CPP11

#include "DatabaseModuleConfig.h"
#include "DBConfiguration.h"

class DATABASEMODULE_API cwSqlite
{
public:
#ifdef USING_CPP11	//C++11
		enum class DomainM
		{
			SYSTEM,
			IPC,
			FACE
		};
		enum class DomainS
		{
			BASE,
			TIME,
			RS232,
			RS485,
			TCP,
			PORT,
			VIDEO,
			ROI,
			IMG,
			FACE
		};
#else	//C++0x
		struct DomainM
		{
			enum t
			{
				SYSTEM,
				IPC,
				FACE
			};
		};

		struct DomainS
		{
			enum t
			{
				BASE, //bs_
				TIME, //tc_
				RS232, //rs232_
				RS485, //rs485_
				TCP,   //tcp_
				PORT,  //port_
				VIDEO, //video_
				ROI,  //roi_
				IMG, //img_
				FACE //face_
			};
		};
#endif
private:
		//Init Memdb
		Poco::Data::Session* m_memory;// = Session(Poco::Data::SQLite::Connector::KEY, ":memory:");
		bool m_isInit;// = false;
		void RefreshFile();
		void ClearFaceVerify();
		void ClearLogRecord();
public:
		cwSqlite();
		static cwSqlite& instance();
		static void InitDBPath(std::string path);
		void InitMem();
		bool IsExsist(std::string s, std::vector<std::string>& v);
		void InitConfigDB();
		void InitFaceDB();
		void InitLogDB();
		std::string GetConfigPath();
	
		//config db :: config table
		CloudWalk::ConfigItem GetParamA(DomainM domain, std::string strkey);
		DBConfiguration& getParamConfig();
		void setParamConfig(DBConfiguration& config);
		std::vector<CloudWalk::ConfigItem> GetParamB(DomainM strdomain);
		std::vector<CloudWalk::ConfigItem> GetParamC(DomainM strdomain1, DomainS strdomain2);
		void SetParam(DomainM domain, CloudWalk::ConfigItem ci);
		//config db :: userrole table
		CloudWalk::UserRole GetUser(std::string struser);
		void ModifyUser(CloudWalk::UserRole ur);
		void InsertUser(CloudWalk::UserRole ur);

		//face db :: faceunit table
		bool GetFaceUnitByID(std::string id,CloudWalk::FaceUnit& unit);
		std::vector<CloudWalk::FaceUnit> GetFaceUnitAll();
		std::vector<CloudWalk::FaceUnit> GetFaceUnitPage(int &count,int pagenum, int pagesize,std::string id,std::string starttime="", std::string  endtime="");
		void InsertFaceUnit(std::vector<CloudWalk::FaceUnit>& v);
		void DeleteFaceUnit();
		void DeleteFaceUnit(std::vector<std::string> vid);
		std::vector<std::string> GetFaceUnitId();

		//face db :: faceverify table
		bool GetFaceVerifyByID(std::string id, CloudWalk::FaceVerify& fv);
		std::vector<CloudWalk::FaceVerify> GetFaceVerifyAll();
		std::vector<CloudWalk::FaceVerify> GetFaceVerifyPage(int &count,int pagenum, int pagesize,std::string starttime="", std::string  endtime="");
		void InsertFaceVerify(std::vector<CloudWalk::FaceVerify>& v); 
		void SetSendFlag(std::vector<int>& vrowid);
		void RemoveFaceVerifys();
		std::vector<CloudWalk::FaceVerify> GetFaceVerifyByFlag(bool flag, int limit);

		//log db :: log table
		std::vector<CloudWalk::LogRecord> GetLogRecordAll();
		std::vector<CloudWalk::LogRecord> GetLogRecordPage(int &count,int pagenum, int pagesize,std::string starttime="", std::string  endtime="");
		void InsertLogRecord (std::vector<CloudWalk::LogRecord>& v); 
		void ReInitFaceDB();

		std::string CONFIGDB;
		std::string FACEDB;
		std::string LOGDB;
private:
		DBConfiguration _dbConfig;
		Poco::FastMutex		m_Mutex;
};

//Poco::Data::Session cwSqlite::m_memory = Poco::Data::Session(Poco::Data::SQLite::Connector::KEY, ":memory:");


#endif
