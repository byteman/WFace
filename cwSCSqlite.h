#ifndef SQLiteTest_INCLUDED
#define SQLiteTest_INCLUDED

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



class  cwSqlite
{
public:

private:
		//Init Memdb
		Poco::Data::Session* m_memory;// = Session(Poco::Data::SQLite::Connector::KEY, ":memory:");
		bool m_isInit;// = false;
		void RefreshFile();

public:
		cwSqlite();
		static cwSqlite& instance();
		static void InitDBPath(std::string path);

		bool IsExsist(std::string s, std::vector<std::string>& v);
		void InitConfigDB();
		void InitFaceDB();
		void InitLogDB();
		std::string GetConfigPath();
	

		//face db :: faceverify table
//		bool GetFaceVerifyByID(std::string id, CloudWalk::FaceVerify& fv);
//		std::vector<CloudWalk::FaceVerify> GetFaceVerifyAll();
//		std::vector<CloudWalk::FaceVerify> GetFaceVerifyPage(int &count,int pagenum, int pagesize,std::string starttime="", std::string  endtime="");
//		void InsertFaceVerify(std::vector<CloudWalk::FaceVerify>& v);
//		void SetSendFlag(std::vector<int>& vrowid);
//		void RemoveFaceVerifys();
//		std::vector<CloudWalk::FaceVerify> GetFaceVerifyByFlag(bool flag, int limit);



		std::string CONFIGDB;
		std::string FACEDB;
		std::string LOGDB;
private:

		Poco::FastMutex		m_Mutex;
};

//Poco::Data::Session cwSqlite::m_memory = Poco::Data::Session(Poco::Data::SQLite::Connector::KEY, ":memory:");


#endif
