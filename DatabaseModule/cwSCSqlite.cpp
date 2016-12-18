#include "cwSCSqlite.h"
#include "Poco/File.h"
#include "Poco/Path.h"
#include "CWLogger.h"
#include "PathUtil.h"



bool cwSqlite::IsExsist(std::string s, std::vector<std::string>& v)
{
	bool brt = false;
	std::vector<std::string>::iterator it = v.begin();
	for(;it!=v.end();it++)
	{
		if(0 == s.compare(*it))
		{
			brt = true;
			break;
		}
	}
	return brt;
}

void cwSqlite::InitConfigDB()
{
	
	//Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp (Poco::Data::SQLite::Connector::KEY, CONFIGDB);
	std::vector<std::string> vtb;
    
    try
    {
        tmp<<"SELECT name FROM sqlite_master WHERE type='table'",into(vtb), now;
    }
    catch(...)
    {
       
    }
   
	if(!IsExsist("systemconfig",vtb))
	{
		
       
		tmp<<"DROP TABLE IF EXISTS [systemconfig]",now;
		tmp<<"CREATE TABLE [systemconfig] ([config_name] varchar(32), [config_value] varchar(64), CONSTRAINT [sqlite_autoindex_systemconfig_1] PRIMARY KEY ([config_name]))", now;
		tmp<<"insert into systemconfig values('bs_device_name', 'IP CAMERA')",now;
		tmp<<"insert into systemconfig values('bs_device_no', '88')",now;
		tmp<<"insert into systemconfig values('bs_device_type', 'iDS-2CD6024FWD-A/F')",now;
		tmp<<"insert into systemconfig values('bs_device_serial', 'iDS-2CD6024FWD-A/F20150909CCCH540444513')",now;
		tmp<<"insert into systemconfig values('bs_main_control_ver', 'V5.3.3 build 150720')",now;
		tmp<<"insert into systemconfig values('bs_coding_ver', 'V7.0 build 150617')",now;
		tmp<<"insert into systemconfig values('bs_web_ver', 'V4.0.1 build 150629')",now;
		tmp<<"insert into systemconfig values('bs_plugin_ver', 'V3.0.5.41')",now;
		tmp<<"insert into systemconfig values('bs_channel_num', '1')",now;
		tmp<<"insert into systemconfig values('bs_alart_input', '1')",now;
		tmp<<"insert into systemconfig values('bs_alart_output', '1')",now;
		tmp<<"insert into systemconfig values('tc_alg_ver', 'V4.0.1 build 150629')",now;
		tmp<<"insert into systemconfig values('tc_mod_ver', 'V4.0.1 build 150629')",now;
		tmp<<"insert into systemconfig values('tc_ntp_server', 'time.windows.com')",now;
		tmp<<"insert into systemconfig values('tc_ntp_port', '123')",now;
		tmp<<"insert into systemconfig values('tc_ntp_intvl', '1440')",now;
		tmp<<"insert into systemconfig values('rs232_baud_rate', '0')",now;
		tmp<<"insert into systemconfig values('rs232_data_bit', '0')",now;
		tmp<<"insert into systemconfig values('rs232_stop_bit', '0')",now;
		tmp<<"insert into systemconfig values('rs232_ischeck', '0')",now;
		tmp<<"insert into systemconfig values('rs232_isstreamcontrol', '0')",now;
		tmp<<"insert into systemconfig values('rs232_control_mode', '0')",now;
		tmp<<"insert into systemconfig values('rs485_baud_rate', '0')",now;
		tmp<<"insert into systemconfig values('rs485_data_bit', '0')",now;
		tmp<<"insert into systemconfig values('rs485_stop_bit', '0')",now;
		tmp<<"insert into systemconfig values('rs485_ischeck', '0')",now;
		tmp<<"insert into systemconfig values('rs485_isstreamcontrol', '0')",now;
		tmp<<"insert into systemconfig values('rs485_control_mode', '0')",now;
		tmp<<"insert into systemconfig values('tcp_netcard_type', '0')",now;
		tmp<<"insert into systemconfig values('tcp_ip_addr', '192.168.10.119')",now;
		tmp<<"insert into systemconfig values('tcp_net_mask', '255.255.255.0')",now;
		tmp<<"insert into systemconfig values('tcp_net_gate', '192.168.10.1')",now;
		tmp<<"insert into systemconfig values('tcp_mc', 'c4:2f:90:c9:6b:19')",now;
		tmp<<"insert into systemconfig values('tcp_mtu', '1500')",now;
		tmp<<"insert into systemconfig values('tcp_abroad', '')",now;
		tmp<<"insert into systemconfig values('tcp_isabroad', '0')",now;
		tmp<<"insert into systemconfig values('tcp_dns_m', '8.8.8.8')",now;
		tmp<<"insert into systemconfig values('tcp_dns_s', '')",now;
		tmp<<"insert into systemconfig values('port_http_port', '80')",now;
		tmp<<"insert into systemconfig values('port_rtsp_port', '554')",now;
		tmp<<"insert into systemconfig values('port_https_port', '443')",now;
		tmp<<"insert into systemconfig values('port_server_addr', '192.168.10.115:51000')",now;

		
	}
   
	if(!IsExsist("ipcconfig",vtb))
	{
		Transaction trans(tmp);
		tmp<<"DROP TABLE IF EXISTS [ipcconfig];",now;
		tmp<<"CREATE TABLE [ipcconfig] ([config_name] varchar(32), [config_value] varchar(64), CONSTRAINT [sqlite_autoindex_ipcconfig_1] PRIMARY KEY ([config_name]))",now;
		tmp<<"insert into ipcconfig values('video_codestream_type', '0')",now;
		tmp<<"insert into ipcconfig values('video_video_type', '0')",now;
		tmp<<"insert into ipcconfig values('video_resolution', '0')",now;
		tmp<<"insert into ipcconfig values('video_coderate_type', '0')",now;
		tmp<<"insert into ipcconfig values('video_quality', '0')",now;
		tmp<<"insert into ipcconfig values('video_frame_rate', '0')",now;
		tmp<<"insert into ipcconfig values('video_coderate_ceiling', '4096')",now;
		tmp<<"insert into ipcconfig values('video_code', '0')",now;
		tmp<<"insert into ipcconfig values('video_code_complex', '0')",now;
		tmp<<"insert into ipcconfig values('video_frame_intvl', '50')",now;
		tmp<<"insert into ipcconfig values('video_svc', '0')",now;
		tmp<<"insert into ipcconfig values('video_codestream_smooth', '50')",now;
		tmp<<"insert into ipcconfig values('roi_x', '10')",now;
		tmp<<"insert into ipcconfig values('roi_y', '10')",now;
		tmp<<"insert into ipcconfig values('roi_h', '100')",now;
		tmp<<"insert into ipcconfig values('roi_w', '100')",now;
		tmp<<"insert into ipcconfig values('img_adjust_lightness', '10')",now;
		tmp<<"insert into ipcconfig values('img_adjust_contrast', '10')",now;
		tmp<<"insert into ipcconfig values('img_adjust_saturation', '10')",now;
		tmp<<"insert into ipcconfig values('img_adjust_sharpness', '10')",now;
		tmp<<"insert into ipcconfig values('img_aperture_type', '0')",now;
		tmp<<"insert into ipcconfig values('img_aperture_sensitivity', '50')",now;
		tmp<<"insert into ipcconfig values('img_aperture_exposure', '0')",now;
		tmp<<"insert into ipcconfig values('img_poorlight_area', '0')",now;
		tmp<<"insert into ipcconfig values('img_poorlight_dyn', '0')",now;
		tmp<<"insert into ipcconfig values('img_balance', '0')",now;
		tmp<<"insert into ipcconfig values('img_advance_digflt', '0')",now;
		tmp<<"insert into ipcconfig values('img_advance_fltlevel', '0')",now;
		tmp<<"insert into ipcconfig values('img_advance_elcstable', '0')",now;
		tmp<<"insert into ipcconfig values('img_advance_range', '500')",now;
		tmp<<"insert into ipcconfig values('img_video_mirror', '0')",now;
		tmp<<"insert into ipcconfig values('img_video_rotation', '0')",now;
		tmp<<"insert into ipcconfig values('img_video_mode', '0')",now;
		tmp<<"insert into ipcconfig values('img_video_format', '0')",now;
		tmp<<"insert into ipcconfig values('img_video_isinput', '0')",now;
	}
  
	if(!IsExsist("faceconfig",vtb))
	{
		tmp<<"DROP TABLE IF EXISTS [faceconfig]",now;
		tmp<<"CREATE TABLE [faceconfig] ([config_name] varchar(32), [config_value] varchar(64), CONSTRAINT [sqlite_autoindex_faceconfig_1] PRIMARY KEY ([config_name]))",now;
		tmp<<"insert into faceconfig values('face_smart_exposure', '0')",now;
		tmp<<"insert into faceconfig values('face_smart_isroi', '0')",now;
		tmp<<"insert into faceconfig values('face_detect_min', '70')",now;
		tmp<<"insert into faceconfig values('face_detect_max', '100')",now;
		tmp<<"insert into faceconfig values('face_detect_box', '0')",now;
		tmp<<"insert into faceconfig values('face_attr_age', '0')",now;
		tmp<<"insert into faceconfig values('face_attr_male', '0')",now;
		tmp<<"insert into faceconfig values('face_attr_glass', '0')",now;
		tmp<<"insert into faceconfig values('face_attr_smile', '0')",now;
		tmp<<"insert into faceconfig values('face_recog_threshold', '0.7')",now;
		tmp<<"insert into faceconfig values('face_recog_quality', '0.7')",now;
		tmp<<"insert into faceconfig values('face_recog_lib', '1000')",now;
	}

	if(!IsExsist("user",vtb))
	{
		tmp<<"DROP TABLE IF EXISTS [user]",now;
		tmp<<"CREATE TABLE [user] ([username] VARCHAR(16), [pwd] CHAR(32), [role] BIGINT, CONSTRAINT [sqlite_autoindex_user_1] PRIMARY KEY ([username]))",now;
		tmp<<"insert into user values('admin', '21232F297A57A5A743894A0E4A801FC3', '9')",now;
	}
   
}

void cwSqlite::InitFaceDB()
{
	
	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
	std::vector<std::string> vtb;
	Transaction trans(tmp);
	tmp<<"SELECT name FROM sqlite_master WHERE type='table'",into(vtb), now;
	if(!IsExsist("facefeature",vtb))
	{
		tmp<<"DROP TABLE IF EXISTS [faceverify]",now;
		tmp<<"CREATE TABLE [faceverify] ([vtime] TIMESTAMP NOT NULL ON CONFLICT REPLACE DEFAULT (datetime(CURRENT_TIMESTAMP,'localtime')), [age] Integer, "
		<<"[male] BOOLEAN, [glasses] BOOLEAN, [xangle] Integer, [yangle] Integer, [zangle] Integer, [smile] Integer, [quality] Float, [score] TEXT, "
		<<"[img] blob, [send_flag] BOOLEAN, [x] INTEGER, [y] INTEGER, [width] INTEGER, [height] INTEGER, [imgf] BLOB)", now;
	}
	if(!IsExsist("faceverify",vtb))
	{
		tmp<<"DROP TABLE IF EXISTS [facefeature]", now;
		tmp<<"CREATE TABLE [facefeature] ([id] char(36), [feature] blob, [img] blob, [time] TIMESTAMP, "
		"[tag] VARCHAR(64), CONSTRAINT [sqlite_autoindex_facefeature_1] PRIMARY KEY ([id]))", now;
	}
	trans.commit();
}
void cwSqlite::ReInitFaceDB()
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	InitFaceDB();
}
void cwSqlite::InitLogDB()
{
	//Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp (Poco::Data::SQLite::Connector::KEY, LOGDB);
	std::vector<std::string> vtb;
	tmp<<"SELECT name FROM sqlite_master WHERE type='table'",into(vtb), now;
	if(!IsExsist("syslog",vtb))
	{
		Transaction trans(tmp);
		tmp<<"DROP TABLE IF EXISTS [syslog]",now;
		tmp<<"CREATE TABLE [syslog] ([opertime] TIMESTAMP NOT NULL ON CONFLICT REPLACE DEFAULT (datetime(CURRENT_TIMESTAMP,'localtime')), "
		<<"[type] TINYINT NOT NULL ON CONFLICT REPLACE DEFAULT 0, [msg] TEXT)",now; 
		trans.commit();
	}
}
cwSqlite& cwSqlite::instance()
{
	static Poco::SingletonHolder<cwSqlite> sh;
	
	return *sh.get();
}
cwSqlite::cwSqlite():
	m_isInit(false),
	m_memory(NULL)
{
	if(!m_isInit)
	{
		InitMem();
	}
}

void cwSqlite::InitMem()
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	Poco::Data::SQLite::Connector::registerConnector();

	if(m_isInit) return;
	if(m_memory == NULL)
	{
		CONFIGDB = PathUtil::get().getConfigDBPath();
		FACEDB = PathUtil::get().getFaceVerifyDBPath();
		LOGDB = PathUtil::get().getLogDBPath();
		Poco::File configdb(CONFIGDB);
		if(!configdb.exists()) 
		{
			Session tmp (Poco::Data::SQLite::Connector::KEY, CONFIGDB);
		}
      
		Poco::File facedb(FACEDB);
		if(!facedb.exists()) 
		{
          
			Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
		}
     
		Poco::File logdb(LOGDB);
		if(!logdb.exists()) 
		{
           
			Session tmp (Poco::Data::SQLite::Connector::KEY, LOGDB);
		}
     
		InitConfigDB();
      
		InitFaceDB();
      
		InitLogDB();

		m_memory = new Session(Poco::Data::SQLite::Connector::KEY, ":memory:");
		Poco::Data::SQLite::Utility::fileToMemory(*m_memory, CONFIGDB);
		_dbConfig.setDBPath(CONFIGDB);
		m_isInit = true;
	}
	
}

void cwSqlite::RefreshFile()
{
	//Poco::FastMutex::ScopedLock lock(m_Mutex);
	//Poco::Data::SQLite::Utility::memoryToFile(CONFIGDB, *m_memory);
}

CloudWalk::ConfigItem cwSqlite::GetParamA(cwSqlite::DomainM strdomain, std::string strkey)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);

	Session tmp = *m_memory;
	std::string key;
	std::string value;
	CloudWalk::ConfigItem ci;
	std::string tbname;
	switch(strdomain)
	{//DomainM::SYS
	case cwSqlite::DomainM::SYSTEM:
		tbname = "systemconfig";
		break;
	case cwSqlite::DomainM::IPC:
		tbname = "ipcconfig";
		break;
	case cwSqlite::DomainM::FACE:
		tbname = "faceconfig";
		break;
	}
    tmp << "SELECT config_name,config_value FROM "<< tbname <<" WHERE config_name = ?", use(strkey), into(key), into(value), now;
    ci.setKey(key);
	ci.setValue(value);
	return ci;
}

std::vector<CloudWalk::ConfigItem> cwSqlite::GetParamB(cwSqlite::DomainM strdomain)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	std::vector<std::string> vkey;
	std::vector<std::string> vvalue;
	std::vector<CloudWalk::ConfigItem> v;
	std::string tbname;
	switch(strdomain)
	{//DomainM::SYS
	case cwSqlite::DomainM::SYSTEM:
		tbname = "systemconfig";
		break;
	case cwSqlite::DomainM::IPC:
		tbname = "ipcconfig";
		break;
	case cwSqlite::DomainM::FACE:
		tbname = "faceconfig";
		break;
	}
    //int count = 0;
    *m_memory << "SELECT config_name,config_value FROM "<< tbname, into(vkey), into(vvalue), now;
	for(int i = 0; i<vkey.size(); i++)
	{
		CloudWalk::ConfigItem ci;
		ci.setKey(vkey[i]);
		ci.setValue(vvalue[i]);
		v.push_back(ci);
	}
	return v;
}

std::vector<CloudWalk::ConfigItem> cwSqlite::GetParamC(cwSqlite::DomainM strdomain1, cwSqlite::DomainS strdomain2)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);

	Session tmp = *m_memory;
	std::vector<std::string> vkey;
	std::vector<std::string> vvalue;
	std::vector<CloudWalk::ConfigItem> v;

	std::string tbname;
	switch(strdomain1)
	{//DomainM::SYS
	case cwSqlite::DomainM::SYSTEM:
		tbname = "systemconfig";
		break;
	case cwSqlite::DomainM::IPC:
		tbname = "ipcconfig";
		break;
	case cwSqlite::DomainM::FACE:
		tbname = "faceconfig";
		break;
	}
    
	std::string prex_name;
	switch(strdomain2)
	{//DomainM::SYS
	case cwSqlite::DomainS::BASE:
		prex_name = "bs_";
		break;
	case cwSqlite::DomainS::TIME:
		prex_name = "tc_";
		break;
	case cwSqlite::DomainS::RS232:
		prex_name = "rs232_";
		break;
	case cwSqlite::DomainS::RS485:
		prex_name = "rs485_";
		break;
	case cwSqlite::DomainS::TCP:
		prex_name = "tcp_";
		break;
	case cwSqlite::DomainS::PORT:
		prex_name = "port_";
		break;
	case cwSqlite::DomainS::VIDEO:
		prex_name = "video_";
		break;
	case cwSqlite::DomainS::ROI:
		prex_name = "roi_";
		break;
	case cwSqlite::DomainS::IMG:
		prex_name = "img_";
		break;
	case cwSqlite::DomainS::FACE:
		prex_name = "face_";
		break;
	}
	std::string strsql = "SELECT config_name,config_value FROM "+ tbname + " WHERE config_name like(\""+ prex_name +"%\")";
    tmp << strsql, into(vkey), into(vvalue), now;
	//std::map<std::string,CloudWalk::ConfigItem>::iterator it = m.begin();
	for(int i = 0; i<vkey.size(); i++)
	{
		CloudWalk::ConfigItem ci;
		ci.setKey(vkey[i]);
		ci.setValue(vvalue[i]);
		v.push_back(ci);
	}
	return v;
}

void cwSqlite::SetParam(cwSqlite::DomainM strdomain, CloudWalk::ConfigItem ci)
{
	//Poco::FastMutex::ScopedLock lock(m_Mutex);

	Session tmp = *m_memory;
	std::string key = ci.getKey();
	std::string value = ci.getValue();
	std::string tbname;
	switch(strdomain)
	{//DomainM::SYS
	case cwSqlite::DomainM::SYSTEM:
		tbname = "systemconfig";
		break;
	case cwSqlite::DomainM::IPC:
		tbname = "ipcconfig";
		break;
	case cwSqlite::DomainM::FACE:
		tbname = "faceconfig";
		break;
	}
    tmp << "UPDATE "<<tbname<<" SET config_name=?, config_value=? WHERE config_name = ?", use(key), use(value), use(key), now;
	RefreshFile();
	return;
}

CloudWalk::UserRole cwSqlite::GetUser(std::string struser)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp = *m_memory;
	std::string uname;
	std::string pwd;
	int role;
	CloudWalk::UserRole ur;
   
    tmp << "SELECT username,pwd,role FROM user WHERE username = ?", use(struser), into(uname), into(pwd), into(role), now;
  
	ur.setUserName(uname);
	ur.setPassword(pwd);
	ur.setRole(role);
	return ur;
}

void cwSqlite::ModifyUser(CloudWalk::UserRole ur)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);

	Session tmp = *m_memory;
	Transaction trans(tmp);
	std::string username = ur.getUserName();
    tmp << "UPDATE user SET username=\""
		<< ur.getUserName() 
		<<"\", pwd=\""
		<<ur.getPassword()
		<<"\", role="
		<<ur.getRole()
		<<" WHERE username = ?", use(username),now;
	RefreshFile();
	trans.commit();
	return;
}

void cwSqlite::InsertUser(CloudWalk::UserRole ur)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp = *m_memory;
	Transaction trans(tmp);
    tmp << "INSERT INTO user VALUES(\""
		<< ur.getUserName() 
		<<"\", \""
		<<ur.getPassword()
		<<"\", "
		<<ur.getRole()
		<<")",now;
	RefreshFile();
	trans.commit();
	return;
}

std::vector<CloudWalk::FaceUnit> cwSqlite::GetFaceUnitAll()
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	std::vector<CloudWalk::FaceUnit> v;
	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
	std::vector<std::string> vid;
	std::vector<Poco::Data::BLOB> vfeature;
	std::vector<Poco::Data::BLOB> vimg;
	std::vector<Poco::DateTime> vtime;
	std::vector<std::string> vtag;
	tmp<<"SELECT id,feature,img,time,tag FROM facefeature", into(vid), into(vfeature), into(vimg), into(vtime), into(vtag), now;
	for(int i = 0; i<vid.size(); i++)
	{
		CloudWalk::FaceUnit fu;
		fu.setId(vid[i]);
		fu.setFeature(vfeature[i]);
		fu.setImg(vimg[i]);
		fu.setDownloadTime(vtime[i]);
		fu.setTag(vtag[i]);
		v.push_back(fu);
	}
	return v;
}
	
std::vector<CloudWalk::FaceUnit> cwSqlite::GetFaceUnitPage(int &count,int pagenum, int pagesize,std::string id,std::string starttime, std::string  endtime)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);

	int offset = (pagenum - 1) * pagesize;
	int limit = pagesize;
	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
	std::vector<std::string> vid;
	std::vector<Poco::Data::BLOB> vfeature;
	std::vector<Poco::Data::BLOB> vimg;
	std::vector<Poco::DateTime> vtime;
	std::vector<std::string> vtag;
	std::vector<CloudWalk::FaceUnit> v;

	int size = 0;
	try
	{
		//Poco::FastMutex::ScopedLock lock(m_Mutex);
		if(id == "")
		{
			if(starttime.empty() || endtime.empty())
			{
				tmp<<"SELECT COUNT(*) FROM facefeature ",into(size), now;

				tmp<<"SELECT id,feature,img,time,tag FROM facefeature limit ? offset ?",use(limit), use(offset), into(vid), into(vfeature), into(vimg), into(vtime), into(vtag),now;
			}
			else
			{	
				tmp<<"SELECT COUNT(*) FROM facefeature where time between ? and ?", use(starttime),use(endtime),into(size), now;

				tmp<<"SELECT id,feature,img,time,tag FROM facefeature where time between ? and ? limit ? offset ?",use(starttime),use(endtime),use(limit), use(offset), into(vid), into(vfeature), into(vimg), into(vtime), into(vtag),now;
			}
		}
		else
		{
			tmp<<"SELECT COUNT(*) FROM facefeature where id=?", use(id),into(size), now;

			tmp<<"SELECT id,feature,img,time,tag FROM facefeature where id=? limit ? offset ?",use(id),use(limit), use(offset), into(vid), into(vfeature), into(vimg), into(vtime), into(vtag),now;

		}
	}
	catch(Poco::Exception& e)
	{
		cw_error("GetFaceUnitPage error %s\n",e.displayText().c_str());
		return v;
	}
	
	
	count = size;

	for(int i = 0; i<vid.size(); i++)
	{
		CloudWalk::FaceUnit fu;
		fu.setId(vid[i]);
		fu.setFeature(vfeature[i]);
		fu.setImg(vimg[i]);
		fu.setDownloadTime(vtime[i]);
		fu.setTag(vtag[i]);
		v.push_back(fu);
	}
	return v;
}
	
void cwSqlite::InsertFaceUnit(std::vector<CloudWalk::FaceUnit>& v)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
	Transaction trans(tmp);
	
	std::vector<Tuple<std::string,Poco::Data::BLOB,Poco::Data::BLOB,Poco::DateTime,std::string>> v1;
	std::vector<std::string> v2;
	std::vector<CloudWalk::FaceUnit>::iterator it = v.begin();
	for(;it!=v.end();it++)
	{
		Tuple<std::string,Poco::Data::BLOB,Poco::Data::BLOB,Poco::DateTime,std::string> tmp(it->getId(), it->getFeature(), it->getImg(), it->getDownloadTime(), it->getTag());
		v1.push_back(tmp);
		v2.push_back(it->getId());
	}
	tmp << "DELETE FROM facefeature WHERE id = ?", use(v2), now;
    tmp << "INSERT INTO facefeature VALUES(?,?,?,?,?)", use(v1), now;
	trans.commit();
	return;
}

void cwSqlite::DeleteFaceUnit()
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
	Transaction trans(tmp);
    tmp << "DELETE FROM facefeature",now;
	trans.commit();
	return;
}

void cwSqlite::DeleteFaceUnit(std::vector<std::string> vid)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
	Transaction trans(tmp);
    tmp << "DELETE FROM facefeature WHERE id = ?", use(vid), now;
	trans.commit();
	return;
}
bool cwSqlite::GetFaceUnitByID(std::string id,CloudWalk::FaceUnit& fu)
{
	
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
	std::vector<std::string> vid;
	std::vector<Poco::Data::BLOB> vfeature;
	std::vector<Poco::Data::BLOB> vimg;
	std::vector<Poco::DateTime> vtime;
	std::vector<std::string> vtag;
	std::vector<CloudWalk::FaceUnit> v;
	try
	{
		tmp<<"SELECT id,feature,img,time,tag FROM facefeature where id=?",use(id),into(vid), into(vfeature), into(vimg), into(vtime), into(vtag),now;

	}
	catch(Poco::Exception& e)
	{
		printf("GetFaceUnitByID error: %s\r\n",e.displayText().c_str());
		return false;
	}

	for(int i = 0; i<vid.size(); i++)
	{
		
		fu.setId(vid[i]);
		fu.setFeature(vfeature[i]);
		fu.setImg(vimg[i]);
		fu.setDownloadTime(vtime[i]);
		fu.setTag(vtag[i]);
		return true;
	}
	return false;
}
std::vector<CloudWalk::FaceVerify> cwSqlite::GetFaceVerifyAll()
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
	std::vector<int> vRecNo;
	std::vector<Poco::DateTime> vtime;
	std::vector<int> vage; 
	std::vector<bool> vmale;
	std::vector<bool> vglasses; 
	std::vector<int> vxangle;
	std::vector<int> vyangle; 
	std::vector<int> vzangle; 
	std::vector<int> vsmile; 
	std::vector<float> vquality;
	std::vector<std::string> vscore;
	std::vector<Poco::Data::BLOB> vimg;
	std::vector<bool> vsendFlag; 
	std::vector<int> vx; 
	std::vector<int> vy; 
	std::vector<int> vwidth; 
	std::vector<int> vheight;
	std::vector<Poco::Data::BLOB> vimgf;
	std::vector<CloudWalk::FaceVerify> v;
	tmp<<"SELECT rowid,vtime,age,male,glasses,xangle,yangle,zangle,smile,quality,score,img,send_flag,x,y,width,height,imgf FROM faceverify",
			into(vRecNo),into(vtime), into(vage), into(vmale), into(vglasses), into(vxangle), into(vyangle), into(vzangle), into(vsmile),
			into(vquality), into(vscore), into(vimg), into(vsendFlag), into(vx), into(vy), into(vwidth), into(vheight), into(vimgf),now;
	for(int i = 0; i<vtime.size(); i++)
	{
		CloudWalk::FaceVerify fv;
		fv.setRecNo(vRecNo[i]);
		fv.setVerifyTime(vtime[i]);
		fv.setAge(vage[i]);
		fv.setMale(vmale[i]);
		fv.setGlasses(vglasses[i]);
		fv.setXangle(vxangle[i]);
		fv.setYangle(vyangle[i]);
		fv.setZangle(vzangle[i]);
		fv.setSmile(vsmile[i]);
		fv.setQuality(vquality[i]);
		fv.setScore(vscore[i]);
		fv.setImg(vimg[i]);
		fv.setSendFlag(vsendFlag[i]);
		fv.setX(vx[i]);
		fv.setY(vy[i]);
		fv.setWidth(vwidth[i]);
		fv.setHeight(vheight[i]);
		fv.setImgf(vimgf[i]);
		v.push_back(fv);
	}
	return v;
}
bool cwSqlite::GetFaceVerifyByID(std::string id, CloudWalk::FaceVerify& fv)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
	std::vector<int> vRecNo;
	std::vector<Poco::DateTime> vtime;
	std::vector<int> vage; 
	std::vector<bool> vmale;
	std::vector<bool> vglasses; 
	std::vector<int> vxangle;
	std::vector<int> vyangle; 
	std::vector<int> vzangle; 
	std::vector<int> vsmile; 
	std::vector<float> vquality;
	std::vector<std::string> vscore;
	std::vector<Poco::Data::BLOB> vimg;
	std::vector<bool> vsendFlag; 
	std::vector<int> vx; 
	std::vector<int> vy; 
	std::vector<int> vwidth; 
	std::vector<int> vheight;
	std::vector<Poco::Data::BLOB> vimgf;
	std::vector<CloudWalk::FaceVerify> v;
	try
	{
		tmp<<"SELECT rowid,vtime,age,male,glasses,xangle,yangle,zangle,smile,quality,score,img,send_flag,x,y,width,height,imgf FROM faceverify where rowid=?",
			use(id),into(vRecNo),into(vtime), into(vage), into(vmale), into(vglasses), into(vxangle), into(vyangle), into(vzangle), into(vsmile),
			into(vquality), into(vscore), into(vimg), into(vsendFlag), into(vx), into(vy), into(vwidth), into(vheight), into(vimgf),now;
	}
	catch(Poco::Exception& e)
	{
		printf("GetFaceVerifyByID error: %s\r\n",e.displayText().c_str());
		return false;
	}
	
	for(int i = 0; i<vtime.size(); i++)
	{
		
		fv.setRecNo(vRecNo[i]);
		fv.setVerifyTime(vtime[i]);
		fv.setAge(vage[i]);
		fv.setMale(vmale[i]);
		fv.setGlasses(vglasses[i]);
		fv.setXangle(vxangle[i]);
		fv.setYangle(vyangle[i]);
		fv.setZangle(vzangle[i]);
		fv.setSmile(vsmile[i]);
		fv.setQuality(vquality[i]);
		fv.setScore(vscore[i]);
		fv.setImg(vimg[i]);
		fv.setSendFlag(vsendFlag[i]);
		fv.setX(vx[i]);
		fv.setY(vy[i]);
		fv.setWidth(vwidth[i]);
		fv.setHeight(vheight[i]);
		fv.setImgf(vimgf[i]);
		v.push_back(fv);
		return true;
	}
	return false;
}

std::vector<CloudWalk::FaceVerify> cwSqlite::GetFaceVerifyPage(int &count,int pagenum, int pagesize,std::string starttime, std::string  endtime)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	int offset = (pagenum - 1) * pagesize;
	int limit = pagesize;
	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
	std::vector<int> vRecNo;
	std::vector<Poco::DateTime> vtime;
	std::vector<int> vage; 
	std::vector<bool> vmale;
	std::vector<bool> vglasses; 
	std::vector<int> vxangle;
	std::vector<int> vyangle; 
	std::vector<int> vzangle; 
	std::vector<int> vsmile; 
	std::vector<float> vquality;
	std::vector<std::string> vscore;
	std::vector<Poco::Data::BLOB> vimg;
	std::vector<bool> vsendFlag; 
	std::vector<int> vx; 
	std::vector<int> vy; 
	std::vector<int> vwidth; 
	std::vector<int> vheight;
	std::vector<Poco::Data::BLOB> vimgf;
	std::vector<CloudWalk::FaceVerify> v;
	int size = 0;
	if(starttime.empty() || endtime.empty())
	{
		tmp<<"SELECT COUNT(*) FROM faceverify ",into(size), now;

		tmp<<"SELECT rowid,vtime,age,male,glasses,xangle,yangle,zangle,smile,quality,score,img,send_flag,x,y,width,height,imgf FROM faceverify   order by rowid desc limit ? offset ?", use(limit), use(offset), 
			into(vRecNo), into(vtime), into(vage), into(vmale), into(vglasses), into(vxangle), into(vyangle), into(vzangle), into(vsmile),
			into(vquality), into(vscore), into(vimg), into(vsendFlag), into(vx), into(vy), into(vwidth), into(vheight), into(vimgf), now;
	}
	else
	{	 
		tmp<<"SELECT COUNT(*) FROM faceverify where vtime between ? and ?", use(starttime),use(endtime),into(size), now;

		tmp<<"SELECT rowid,vtime,age,male,glasses,xangle,yangle,zangle,smile,quality,score,img,send_flag,x,y,width,height,imgf FROM faceverify where vtime between ? and ? order by rowid desc  limit ? offset ?",  use(starttime),use(endtime),use(limit), use(offset), 
			into(vRecNo), into(vtime), into(vage), into(vmale), into(vglasses), into(vxangle), into(vyangle), into(vzangle), into(vsmile),
			into(vquality), into(vscore), into(vimg), into(vsendFlag), into(vx), into(vy), into(vwidth), into(vheight), into(vimgf), now;
	}
	count = size;

	
	for(int i = 0; i<vtime.size(); i++)
	{
		CloudWalk::FaceVerify fv;
		fv.setRecNo(vRecNo[i]);
		fv.setVerifyTime(vtime[i]);
		fv.setAge(vage[i]);
		fv.setMale(vmale[i]);
		fv.setGlasses(vglasses[i]);
		fv.setXangle(vxangle[i]);
		fv.setYangle(vyangle[i]);
		fv.setZangle(vzangle[i]);
		fv.setSmile(vsmile[i]);
		fv.setQuality(vquality[i]);
		fv.setScore(vscore[i]);
		fv.setImg(vimg[i]);
		fv.setSendFlag(vsendFlag[i]);
		fv.setX(vx[i]);
		fv.setY(vy[i]);
		fv.setWidth(vwidth[i]);
		fv.setHeight(vheight[i]);
		fv.setImgf(vimgf[i]);
		v.push_back(fv);
	}
	return v;
}

void cwSqlite::InsertFaceVerify(std::vector<CloudWalk::FaceVerify>& v)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
	Transaction trans(tmp);
	std::vector<Tuple<Poco::DateTime,int,bool,bool,int,int,int,int,float,std::string,Poco::Data::BLOB,bool,int,int,int,int,Poco::Data::BLOB>> v1;
	std::vector<CloudWalk::FaceVerify>::iterator it = v.begin();
	for(;it!=v.end();it++)
	{
		
		Tuple<Poco::DateTime,int,bool,bool,int,int,int,int,float,std::string,Poco::Data::BLOB,bool,int,int,int,int,Poco::Data::BLOB> tmp(it->getVerifyTime(), it->getAge(), 
			it->getMale(), it->getGlasses(), it->getXangle(), it->getYangle(), it->getZangle(), it->getSmile(), it->getQuality(), it->getScore(),
			it->getImg(), it->getSendFlag(), it->getX(), it->getY(), it->getWidth(), it->getHeight(), it->getImgf());
		v1.push_back(tmp);
	}
    tmp << "INSERT INTO faceverify VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)", use(v1), now;
	trans.commit();
	ClearFaceVerify();
	return;
}

void cwSqlite::SetSendFlag(std::vector<int>& vrowid)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);

	if(vrowid.size() == 0)
	{
		return;
	}
	std::vector<int>::iterator it = vrowid.begin();
	std::string id_list;
	std::stringstream ss;
	for(;it!=vrowid.end();it++)
	{
		ss<<*it<<","; 
	}
	ss>>id_list;
	id_list = id_list.substr(0, id_list.length()-1);
	
	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
	Transaction trans(tmp);
    tmp << "UPDATE faceverify SET send_flag='true' WHERE rowid IN ("<<id_list<<")", now;
	trans.commit();
	return;
}

std::vector<CloudWalk::LogRecord> cwSqlite::GetLogRecordAll()
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp (Poco::Data::SQLite::Connector::KEY, LOGDB);
	std::vector<Poco::DateTime> vtime;
	std::vector<int> vtype;
	std::vector<std::string> vmsg;
	std::vector<CloudWalk::LogRecord> v;
	tmp<<"SELECT opertime,type,msg FROM syslog", into(vtime), into(vtype), into(vmsg), now;
	for(int i = 0; i<vtime.size(); i++)
	{
		CloudWalk::LogRecord lr;
		lr.setOperTime(vtime[i]);
		lr.setType(vtype[i]);
		lr.setMsg(vmsg[i]);
		v.push_back(lr);
	}
	return v;
}
  
std::vector<CloudWalk::LogRecord> cwSqlite::GetLogRecordPage(int &count,int pagenum, int pagesize,std::string starttime, std::string  endtime)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	int offset = (pagenum - 1) * pagesize;
	int limit = pagesize;
	int size = 0;
	Session tmp (Poco::Data::SQLite::Connector::KEY, LOGDB);
	std::vector<Poco::DateTime> vtime;
	std::vector<int> vtype;
	std::vector<std::string> vmsg;
	std::vector<CloudWalk::LogRecord> v;
	if(starttime.empty() || endtime.empty())
	{
		tmp<<"SELECT COUNT(*) FROM syslog ",into(size), now;

		tmp<<"SELECT opertime,type,msg FROM syslog  limit ? offset ?",use(limit), use(offset), into(vtime), into(vtype), into(vmsg), now;

	}
	else
	{	
		tmp<<"SELECT COUNT(*) FROM syslog where opertime between ? and ?",into(size), use(starttime),use(endtime), now;
		tmp<<"SELECT opertime,type,msg FROM syslog where opertime between ? and ? limit ? offset ?", use(starttime),use(endtime),use(limit), use(offset), into(vtime), into(vtype), into(vmsg), now;
	}
	count = size;
	for(int i = 0; i<vtime.size(); i++)
	{
		CloudWalk::LogRecord lr;
		lr.setOperTime(vtime[i]);
		lr.setType(vtype[i]);
		lr.setMsg(vmsg[i]);
		v.push_back(lr);
	}
	return v;
}
	
void cwSqlite::InsertLogRecord (std::vector<CloudWalk::LogRecord>& v)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp (Poco::Data::SQLite::Connector::KEY, LOGDB);
	Transaction trans(tmp);
	std::vector<Tuple<Poco::DateTime,int,std::string>> v1;
	std::vector<CloudWalk::LogRecord>::iterator it = v.begin();
	for(;it!=v.end();it++)
	{
		Tuple<Poco::DateTime,int,std::string> tmp(it->getOperTime(), it->getType(), it->getMsg());
		v1.push_back(tmp);
	}
    tmp << "INSERT INTO syslog VALUES(?,?,?)", use(v1), now;
	trans.commit();
	ClearLogRecord();
	return;
}

void cwSqlite::ClearFaceVerify()
{
	//Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
//	Transaction trans(tmp);
	int count = 0;
	int minrowid = 0;
	tmp<<"SELECT COUNT(*),MIN(rowid) FROM faceverify", into(count),into(minrowid),now;
	minrowid = minrowid + 1000 -1;
	if(count > 10000)
	{
		tmp<<"DELETE FROM faceverify WHERE rowid <= ? AND send_flag ='true'",use(minrowid),now;
		tmp<<"VACUUM",now;
		//delete send_flag = true front 1000
	}
//	trans.commit();
	return;
}

void cwSqlite::ClearLogRecord()
{
	//Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp (Poco::Data::SQLite::Connector::KEY, LOGDB);
//	Transaction trans(tmp);
	int count = 0;
	int minrowid = 0;
	tmp<<"SELECT COUNT(*),MIN(rowid) FROM syslog", into(count),into(minrowid),now;
	minrowid = minrowid + 1000 -1;
	if(count > 10000)
	{
		tmp<<"DELETE FROM syslog WHERE rowid <= ?",use(minrowid),now;
		tmp<<"VACUUM",now;
		//delete send_flag = true front 1000
	}
//	trans.commit();
	return;
}

std::string cwSqlite::GetConfigPath()
{
	return cwSqlite::CONFIGDB;
}
DBConfiguration& cwSqlite::getParamConfig()
{
	return _dbConfig;
}
std::vector<std::string> cwSqlite::GetFaceUnitId()
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
	std::vector<std::string> vid;
	tmp<<"SELECT id FROM facefeature", into(vid), now;
	return vid;
}

std::vector<CloudWalk::FaceVerify> cwSqlite::GetFaceVerifyByFlag(bool flag, int limit)
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
	std::vector<int> vRecNo;
	std::vector<Poco::DateTime> vtime;
	std::vector<int> vage; 
	std::vector<bool> vmale;
	std::vector<bool> vglasses; 
	std::vector<int> vxangle;
	std::vector<int> vyangle; 
	std::vector<int> vzangle; 
	std::vector<int> vsmile; 
	std::vector<float> vquality;
	std::vector<std::string> vscore;
	std::vector<Poco::Data::BLOB> vimg;
	std::vector<bool> vsendFlag; 
	std::vector<int> vx; 
	std::vector<int> vy; 
	std::vector<int> vwidth; 
	std::vector<int> vheight;
	std::vector<Poco::Data::BLOB> vimgf;
	std::vector<CloudWalk::FaceVerify> v;
	std::string condstr;
	if(flag)
	{
		condstr = "=1";
	}
	else
	{
		condstr = "=0";
	}
	tmp<<"SELECT rowid,vtime,age,male,glasses,xangle,yangle,zangle,smile,quality,score,img,send_flag,x,y,width,height,imgf FROM faceverify WHERE send_flag"<<condstr<<" limit ? offset 0", use(limit),
			into(vRecNo),into(vtime), into(vage), into(vmale), into(vglasses), into(vxangle), into(vyangle), into(vzangle), into(vsmile),
			into(vquality), into(vscore), into(vimg), into(vsendFlag), into(vx), into(vy), into(vwidth), into(vheight), into(vimgf),now;
	for(int i = 0; i<vtime.size(); i++)
	{
		CloudWalk::FaceVerify fv;
		fv.setRecNo(vRecNo[i]);
		fv.setVerifyTime(vtime[i]);
		fv.setAge(vage[i]);
		fv.setMale(vmale[i]);
		fv.setGlasses(vglasses[i]);
		fv.setXangle(vxangle[i]);
		fv.setYangle(vyangle[i]);
		fv.setZangle(vzangle[i]);
		fv.setSmile(vsmile[i]);
		fv.setQuality(vquality[i]);
		fv.setScore(vscore[i]);
		fv.setImg(vimg[i]);
		fv.setSendFlag(vsendFlag[i]);
		fv.setX(vx[i]);
		fv.setY(vy[i]);
		fv.setWidth(vwidth[i]);
		fv.setHeight(vheight[i]);
		fv.setImgf(vimgf[i]);
		v.push_back(fv);
	}
	return v;
}

void cwSqlite::RemoveFaceVerifys()
{
	Poco::FastMutex::ScopedLock lock(m_Mutex);
	try
	{
		Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
		Transaction trans(tmp);
		tmp << "DELETE FROM faceverify",now;
		trans.commit();
	}
	catch(Poco::Exception& e)
	{
		cw_error("RemoveFaceVerifys failed %s",e.displayText().c_str());
	}

	return;
}

