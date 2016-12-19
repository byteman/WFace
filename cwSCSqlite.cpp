#include "cwSCSqlite.h"
#include "Poco/File.h"
#include "Poco/Path.h"
//#include "CWLogger.h"

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
		
       

		
	}
   

}

void cwSqlite::InitFaceDB()
{
	
    Session tmp (Poco::Data::SQLite::Connector::KEY, WEIGHTDB);
	std::vector<std::string> vtb;
	Transaction trans(tmp);
	tmp<<"SELECT name FROM sqlite_master WHERE type='table'",into(vtb), now;
    if(!IsExsist("weightTable",vtb))
	{
		tmp<<"DROP TABLE IF EXISTS [faceverify]",now;
        tmp<<"CREATE TABLE [weightTable] ([vtime] TIMESTAMP NOT NULL ON CONFLICT REPLACE DEFAULT (datetime(CURRENT_TIMESTAMP,'localtime')), [weight] Integer, "
        <<"[gps] TEXT, [plate] TEXT, [driver] TEXT [stime] TIMESTAMP NOT NULL ON CONFLICT REPLACE DEFAULT (datetime(CURRENT_TIMESTAMP,'localtime'))", now;
	}

	trans.commit();
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

}

//std::vector<CloudWalk::FaceVerify> cwSqlite::GetFaceVerifyAll()
//{
//	Poco::FastMutex::ScopedLock lock(m_Mutex);
//	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
//	std::vector<int> vRecNo;
//	std::vector<Poco::DateTime> vtime;
//	std::vector<int> vage;
//	std::vector<bool> vmale;
//	std::vector<bool> vglasses;
//	std::vector<int> vxangle;
//	std::vector<int> vyangle;
//	std::vector<int> vzangle;
//	std::vector<int> vsmile;
//	std::vector<float> vquality;
//	std::vector<std::string> vscore;
//	std::vector<Poco::Data::BLOB> vimg;
//	std::vector<bool> vsendFlag;
//	std::vector<int> vx;
//	std::vector<int> vy;
//	std::vector<int> vwidth;
//	std::vector<int> vheight;
//	std::vector<Poco::Data::BLOB> vimgf;
//	std::vector<CloudWalk::FaceVerify> v;
//	tmp<<"SELECT rowid,vtime,age,male,glasses,xangle,yangle,zangle,smile,quality,score,img,send_flag,x,y,width,height,imgf FROM faceverify",
//			into(vRecNo),into(vtime), into(vage), into(vmale), into(vglasses), into(vxangle), into(vyangle), into(vzangle), into(vsmile),
//			into(vquality), into(vscore), into(vimg), into(vsendFlag), into(vx), into(vy), into(vwidth), into(vheight), into(vimgf),now;
//	for(int i = 0; i<vtime.size(); i++)
//	{
//		CloudWalk::FaceVerify fv;
//		fv.setRecNo(vRecNo[i]);
//		fv.setVerifyTime(vtime[i]);
//		fv.setAge(vage[i]);
//		fv.setMale(vmale[i]);
//		fv.setGlasses(vglasses[i]);
//		fv.setXangle(vxangle[i]);
//		fv.setYangle(vyangle[i]);
//		fv.setZangle(vzangle[i]);
//		fv.setSmile(vsmile[i]);
//		fv.setQuality(vquality[i]);
//		fv.setScore(vscore[i]);
//		fv.setImg(vimg[i]);
//		fv.setSendFlag(vsendFlag[i]);
//		fv.setX(vx[i]);
//		fv.setY(vy[i]);
//		fv.setWidth(vwidth[i]);
//		fv.setHeight(vheight[i]);
//		fv.setImgf(vimgf[i]);
//		v.push_back(fv);
//	}
//	return v;
//}
//bool cwSqlite::GetFaceVerifyByID(std::string id, CloudWalk::FaceVerify& fv)
//{
//	Poco::FastMutex::ScopedLock lock(m_Mutex);
//	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
//	std::vector<int> vRecNo;
//	std::vector<Poco::DateTime> vtime;
//	std::vector<int> vage;
//	std::vector<bool> vmale;
//	std::vector<bool> vglasses;
//	std::vector<int> vxangle;
//	std::vector<int> vyangle;
//	std::vector<int> vzangle;
//	std::vector<int> vsmile;
//	std::vector<float> vquality;
//	std::vector<std::string> vscore;
//	std::vector<Poco::Data::BLOB> vimg;
//	std::vector<bool> vsendFlag;
//	std::vector<int> vx;
//	std::vector<int> vy;
//	std::vector<int> vwidth;
//	std::vector<int> vheight;
//	std::vector<Poco::Data::BLOB> vimgf;
//	std::vector<CloudWalk::FaceVerify> v;
//	try
//	{
//		tmp<<"SELECT rowid,vtime,age,male,glasses,xangle,yangle,zangle,smile,quality,score,img,send_flag,x,y,width,height,imgf FROM faceverify where rowid=?",
//			use(id),into(vRecNo),into(vtime), into(vage), into(vmale), into(vglasses), into(vxangle), into(vyangle), into(vzangle), into(vsmile),
//			into(vquality), into(vscore), into(vimg), into(vsendFlag), into(vx), into(vy), into(vwidth), into(vheight), into(vimgf),now;
//	}
//	catch(Poco::Exception& e)
//	{
//		printf("GetFaceVerifyByID error: %s\r\n",e.displayText().c_str());
//		return false;
//	}
	
//	for(int i = 0; i<vtime.size(); i++)
//	{
		
//		fv.setRecNo(vRecNo[i]);
//		fv.setVerifyTime(vtime[i]);
//		fv.setAge(vage[i]);
//		fv.setMale(vmale[i]);
//		fv.setGlasses(vglasses[i]);
//		fv.setXangle(vxangle[i]);
//		fv.setYangle(vyangle[i]);
//		fv.setZangle(vzangle[i]);
//		fv.setSmile(vsmile[i]);
//		fv.setQuality(vquality[i]);
//		fv.setScore(vscore[i]);
//		fv.setImg(vimg[i]);
//		fv.setSendFlag(vsendFlag[i]);
//		fv.setX(vx[i]);
//		fv.setY(vy[i]);
//		fv.setWidth(vwidth[i]);
//		fv.setHeight(vheight[i]);
//		fv.setImgf(vimgf[i]);
//		v.push_back(fv);
//		return true;
//	}
//	return false;
//}

//std::vector<CloudWalk::FaceVerify> cwSqlite::GetFaceVerifyPage(int &count,int pagenum, int pagesize,std::string starttime, std::string  endtime)
//{
//	Poco::FastMutex::ScopedLock lock(m_Mutex);
//	int offset = (pagenum - 1) * pagesize;
//	int limit = pagesize;
//	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
//	std::vector<int> vRecNo;
//	std::vector<Poco::DateTime> vtime;
//	std::vector<int> vage;
//	std::vector<bool> vmale;
//	std::vector<bool> vglasses;
//	std::vector<int> vxangle;
//	std::vector<int> vyangle;
//	std::vector<int> vzangle;
//	std::vector<int> vsmile;
//	std::vector<float> vquality;
//	std::vector<std::string> vscore;
//	std::vector<Poco::Data::BLOB> vimg;
//	std::vector<bool> vsendFlag;
//	std::vector<int> vx;
//	std::vector<int> vy;
//	std::vector<int> vwidth;
//	std::vector<int> vheight;
//	std::vector<Poco::Data::BLOB> vimgf;
//	std::vector<CloudWalk::FaceVerify> v;
//	int size = 0;
//	if(starttime.empty() || endtime.empty())
//	{
//		tmp<<"SELECT COUNT(*) FROM faceverify ",into(size), now;

//		tmp<<"SELECT rowid,vtime,age,male,glasses,xangle,yangle,zangle,smile,quality,score,img,send_flag,x,y,width,height,imgf FROM faceverify   order by rowid desc limit ? offset ?", use(limit), use(offset),
//			into(vRecNo), into(vtime), into(vage), into(vmale), into(vglasses), into(vxangle), into(vyangle), into(vzangle), into(vsmile),
//			into(vquality), into(vscore), into(vimg), into(vsendFlag), into(vx), into(vy), into(vwidth), into(vheight), into(vimgf), now;
//	}
//	else
//	{
//		tmp<<"SELECT COUNT(*) FROM faceverify where vtime between ? and ?", use(starttime),use(endtime),into(size), now;

//		tmp<<"SELECT rowid,vtime,age,male,glasses,xangle,yangle,zangle,smile,quality,score,img,send_flag,x,y,width,height,imgf FROM faceverify where vtime between ? and ? order by rowid desc  limit ? offset ?",  use(starttime),use(endtime),use(limit), use(offset),
//			into(vRecNo), into(vtime), into(vage), into(vmale), into(vglasses), into(vxangle), into(vyangle), into(vzangle), into(vsmile),
//			into(vquality), into(vscore), into(vimg), into(vsendFlag), into(vx), into(vy), into(vwidth), into(vheight), into(vimgf), now;
//	}
//	count = size;

	
//	for(int i = 0; i<vtime.size(); i++)
//	{
//		CloudWalk::FaceVerify fv;
//		fv.setRecNo(vRecNo[i]);
//		fv.setVerifyTime(vtime[i]);
//		fv.setAge(vage[i]);
//		fv.setMale(vmale[i]);
//		fv.setGlasses(vglasses[i]);
//		fv.setXangle(vxangle[i]);
//		fv.setYangle(vyangle[i]);
//		fv.setZangle(vzangle[i]);
//		fv.setSmile(vsmile[i]);
//		fv.setQuality(vquality[i]);
//		fv.setScore(vscore[i]);
//		fv.setImg(vimg[i]);
//		fv.setSendFlag(vsendFlag[i]);
//		fv.setX(vx[i]);
//		fv.setY(vy[i]);
//		fv.setWidth(vwidth[i]);
//		fv.setHeight(vheight[i]);
//		fv.setImgf(vimgf[i]);
//		v.push_back(fv);
//	}
//	return v;
//}

//void cwSqlite::InsertFaceVerify(std::vector<CloudWalk::FaceVerify>& v)
//{
//	Poco::FastMutex::ScopedLock lock(m_Mutex);
//	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
//	Transaction trans(tmp);
//	std::vector<Tuple<Poco::DateTime,int,bool,bool,int,int,int,int,float,std::string,Poco::Data::BLOB,bool,int,int,int,int,Poco::Data::BLOB>> v1;
//	std::vector<CloudWalk::FaceVerify>::iterator it = v.begin();
//	for(;it!=v.end();it++)
//	{
		
//		Tuple<Poco::DateTime,int,bool,bool,int,int,int,int,float,std::string,Poco::Data::BLOB,bool,int,int,int,int,Poco::Data::BLOB> tmp(it->getVerifyTime(), it->getAge(),
//			it->getMale(), it->getGlasses(), it->getXangle(), it->getYangle(), it->getZangle(), it->getSmile(), it->getQuality(), it->getScore(),
//			it->getImg(), it->getSendFlag(), it->getX(), it->getY(), it->getWidth(), it->getHeight(), it->getImgf());
//		v1.push_back(tmp);
//	}
//    tmp << "INSERT INTO faceverify VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)", use(v1), now;
//	trans.commit();
//	ClearFaceVerify();
//	return;
//}



//std::string cwSqlite::GetConfigPath()
//{
//	return cwSqlite::CONFIGDB;
//}
//DBConfiguration& cwSqlite::getParamConfig()
//{
//	return _dbConfig;
//}
//std::vector<std::string> cwSqlite::GetFaceUnitId()
//{
//	Poco::FastMutex::ScopedLock lock(m_Mutex);
//	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
//	std::vector<std::string> vid;
//	tmp<<"SELECT id FROM facefeature", into(vid), now;
//	return vid;
//}

//std::vector<CloudWalk::FaceVerify> cwSqlite::GetFaceVerifyByFlag(bool flag, int limit)
//{
//	Poco::FastMutex::ScopedLock lock(m_Mutex);
//	Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
//	std::vector<int> vRecNo;
//	std::vector<Poco::DateTime> vtime;
//	std::vector<int> vage;
//	std::vector<bool> vmale;
//	std::vector<bool> vglasses;
//	std::vector<int> vxangle;
//	std::vector<int> vyangle;
//	std::vector<int> vzangle;
//	std::vector<int> vsmile;
//	std::vector<float> vquality;
//	std::vector<std::string> vscore;
//	std::vector<Poco::Data::BLOB> vimg;
//	std::vector<bool> vsendFlag;
//	std::vector<int> vx;
//	std::vector<int> vy;
//	std::vector<int> vwidth;
//	std::vector<int> vheight;
//	std::vector<Poco::Data::BLOB> vimgf;
//	std::vector<CloudWalk::FaceVerify> v;
//	std::string condstr;
//	if(flag)
//	{
//		condstr = "=1";
//	}
//	else
//	{
//		condstr = "=0";
//	}
//	tmp<<"SELECT rowid,vtime,age,male,glasses,xangle,yangle,zangle,smile,quality,score,img,send_flag,x,y,width,height,imgf FROM faceverify WHERE send_flag"<<condstr<<" limit ? offset 0", use(limit),
//			into(vRecNo),into(vtime), into(vage), into(vmale), into(vglasses), into(vxangle), into(vyangle), into(vzangle), into(vsmile),
//			into(vquality), into(vscore), into(vimg), into(vsendFlag), into(vx), into(vy), into(vwidth), into(vheight), into(vimgf),now;
//	for(int i = 0; i<vtime.size(); i++)
//	{
//		CloudWalk::FaceVerify fv;
//		fv.setRecNo(vRecNo[i]);
//		fv.setVerifyTime(vtime[i]);
//		fv.setAge(vage[i]);
//		fv.setMale(vmale[i]);
//		fv.setGlasses(vglasses[i]);
//		fv.setXangle(vxangle[i]);
//		fv.setYangle(vyangle[i]);
//		fv.setZangle(vzangle[i]);
//		fv.setSmile(vsmile[i]);
//		fv.setQuality(vquality[i]);
//		fv.setScore(vscore[i]);
//		fv.setImg(vimg[i]);
//		fv.setSendFlag(vsendFlag[i]);
//		fv.setX(vx[i]);
//		fv.setY(vy[i]);
//		fv.setWidth(vwidth[i]);
//		fv.setHeight(vheight[i]);
//		fv.setImgf(vimgf[i]);
//		v.push_back(fv);
//	}
//	return v;
//}

//void cwSqlite::RemoveFaceVerifys()
//{
//	Poco::FastMutex::ScopedLock lock(m_Mutex);
//	try
//	{
//		Session tmp (Poco::Data::SQLite::Connector::KEY, FACEDB);
//		Transaction trans(tmp);
//		tmp << "DELETE FROM faceverify",now;
//		trans.commit();
//	}
//	catch(Poco::Exception& e)
//	{
//		cw_error("RemoveFaceVerifys failed %s",e.displayText().c_str());
//	}

//	return;
//}

