#include "cwSCSqlite.h" //����cwSQLite�ӿ���������ֻ�������ͷ�ļ�
#include <iostream>
#include <sstream>
#include <fstream>
//#include <vector>
using namespace std;
void db_test_main()
{
	//��ȷ��ѯ����
	cwSqlite sql;
	sql.InitMem();
	std::cout<<"---------------------------cwSqlite demo begin--------------------------------"<<std::endl;
	std::cout<<"---------------------------GetParamA begin--------------------------------"<<std::endl;
	CloudWalk::ConfigItem cA = sql.GetParamA(cwSqlite::DomainM::SYSTEM, "bs_device_no");
	std::cout<<cA.getKey()<<"|"<<cA.getValue()<<std::endl;

	//��һ�����ѯ������(һ�����Ӧ��configdb�е�3����)
	std::cout<<"---------------------------GetParamB begin--------------------------------"<<std::endl;
	std::vector<CloudWalk::ConfigItem> vB = sql.GetParamB(cwSqlite::DomainM::SYSTEM);
	std::vector<CloudWalk::ConfigItem>::iterator itB = vB.begin();
	for(;itB!=vB.end();itB++)
	{
		std::cout<<itB->getKey()<<"|"<<itB->getValue()<<std::endl;
	}

	//������������ѯ������(һ�����Ӧ��configdb�е�3����, ����������Ӧ�˱��в�ͬ�Ĳ�����ǰ׺)
	std::cout<<"---------------------------GetParamC begin--------------------------------"<<std::endl;
	std::vector<CloudWalk::ConfigItem> vC = sql.GetParamC(cwSqlite::DomainM::SYSTEM, cwSqlite::DomainS::TCP);
	std::vector<CloudWalk::ConfigItem>::iterator itC = vC.begin();
	for(;itC!=vC.end();itC++)
	{
		std::cout<<itC->getKey()<<"|"<<itC->getValue()<<std::endl;
	}

	//�޸Ĳ���ֵ
	CloudWalk::ConfigItem ci;
	ci.setKey("bs_device_no");ci.setValue("99");
	sql.SetParam(cwSqlite::DomainM::SYSTEM, ci);

	//��ѯ�û������Ȩ�ޡ�
	CloudWalk::UserRole ur = sql.GetUser("admin");
	std::cout<<ur.getUserName()<<"|"<<ur.getPassword()<<"|"<<ur.getRole()<<std::endl;

	//�޸��û������Ȩ�ޡ�
	ur.setRole(1516);
	sql.ModifyUser(ur);

	//��һ�����ǲ����û���������ÿ������demoʱ����Ҫ�����û������ע�͵��ˡ�
	//CloudWalk::UserRole urnew;
	//urnew.setUserName("chenkkuan");urnew.setPassword("asdf1234");urnew.setRole(15);
	//InsertUser(urnew);
	sql.DeleteFaceUnit();
	//face db :: faceunit table
	//��һ�����ǲ���FaceUnit, ������ÿ������demoʱ����Ҫ���������⡣���ע�͵��ˡ�
	std::vector<CloudWalk::FaceUnit> vfu;
	for(int i=0; i<100; i++)
	{ 
		CloudWalk::FaceUnit fu;
		//id
		std::stringstream ss;
		ss<<i;
		string strid;
		ss>>strid;
		fu.setId(strid);
		//feature
		std::ifstream ifs1("./ImgTest/1.jpg", ios::binary);
		if(!ifs1)
		{
			return;
		}
		unsigned char s1[1024*100] = {0};
		int ilen1=0;
		while(!ifs1.eof())
		{
			ifs1.read((char*)s1+ilen1, 1);
			ilen1++;
		}
		ifs1.close();
		Poco::Data::BLOB bftr(s1, ilen1);
		fu.setFeature(bftr);
		//img
		std::ifstream ifs2("./ImgTest/2.jpg", ios::binary);
		if(!ifs2)
		{
			return;
		}
		unsigned char s2[1024*100] = {0};
		int ilen2=0;
		while(!ifs2.eof())
		{
			ifs2.read((char*)s2+ilen2, 1);
			ilen2++;
		}
		Poco::Data::BLOB bimg(s2, ilen2);
		fu.setImg(bimg);
		//dtime
		Poco::DateTime dt(2015, 11, 16, 13, 12, 11); //2015��11��16�� 13��12��11��
		fu.setDownloadTime(dt);
		//tag
		fu.setTag("asdf1234");
		vfu.push_back(fu);
	}
	sql.InsertFaceUnit(vfu);
#if 0
	//ȫ���ѯFaceUnit
	std::cout<<"---------------------------GetFaceUnitAll begin---------------------------"<<std::endl;
	std::vector<CloudWalk::FaceUnit> vfuall = sql.GetFaceUnitAll();
	std::vector<CloudWalk::FaceUnit>::iterator itfuall = vfuall.begin();
	for(;itfuall != vfuall.end(); itfuall++)
	{
		cout<<itfuall->getId()<<"|"<<itfuall->getDownloadTime().year()<<"��"<<itfuall->getDownloadTime().month()<<"��"
									<<itfuall->getDownloadTime().day()<<"��"<<itfuall->getDownloadTime().hour()<<"��"
									<<itfuall->getDownloadTime().minute()<<"��"<<itfuall->getDownloadTime().second()<<"��"
			<<"|"<<itfuall->getFeature().size()<<"|"<<itfuall->getImg().size()<<"|"<<itfuall->getTag()<<std::endl;
	}

	//��ҳ��ѯFaceUnit
	std::cout<<"--------------------------GetFaceUnitPage begin---------------------------"<<std::endl;
	std::vector<CloudWalk::FaceUnit> vfupg = sql.GetFaceUnitPage(1, 20);
	std::vector<CloudWalk::FaceUnit>::iterator itfupg = vfupg.begin();
	for(;itfupg != vfupg.end(); itfupg++)
	{
		cout<<itfupg->getId()<<"|"<<itfupg->getDownloadTime().year()<<"��"<<itfupg->getDownloadTime().month()<<"��"
									<<itfupg->getDownloadTime().day()<<"��"<<itfupg->getDownloadTime().hour()<<"��"
									<<itfupg->getDownloadTime().minute()<<"��"<<itfupg->getDownloadTime().second()<<"��"
			<<"|"<<itfupg->getFeature().size()<<"|"<<itfupg->getImg().size()<<"|"<<itfupg->getTag()<<std::endl;
	}
#endif
	////face db :: faceverify table
	//��һ�����ǲ���FaceVerify, ������ÿ������demoʱ����Ҫ����FaceVerify���ݡ����ע�͵��ˡ�
	std::vector<CloudWalk::FaceVerify> vfv;
	for(int j=1; j<=100; j++)
	{
		CloudWalk::FaceVerify fv;
		Poco::DateTime dt; //now
		fv.setVerifyTime(dt);
		fv.setAge(27);
		fv.setMale(false);
		fv.setGlasses(false);
		fv.setXangle(-8);
		fv.setYangle(8);
		fv.setZangle(8);
		fv.setSmile(2);
		fv.setQuality((float)3.1416);
		fv.setScore("{\"id\":\"12345\",\"score\":\"3.1415926\"}");
		std::ifstream ifs2("./ImgTest/2.jpg", ios::binary);
		if(!ifs2)
		{
			return;
		}
		unsigned char s2[1024*100] = {0};
		int ilen2=0;
		while(!ifs2.eof())
		{
			ifs2.read((char*)s2+ilen2, 1);
			ilen2++;
		}
		Poco::Data::BLOB bimg(s2, ilen2);
		fv.setImg(bimg);
		//
		fv.setSendFlag(false);
		fv.setX(4);
		fv.setY(5);
		fv.setWidth(6);
		fv.setHeight(7);
		fv.setImgf(bimg);
		vfv.push_back(fv);
		//vfg.push_back(j);
	}
	sql.InsertFaceVerify(vfv);
	//sql.SetSendFlag(vfg);

	

	//ȫ���ѯFaceVerify����
	std::cout<<"--------------------------GetFaceVerifyAll begin--------------------------"<<std::endl;
	std::vector<CloudWalk::FaceVerify> vfvall = sql.GetFaceVerifyAll();
	std::vector<CloudWalk::FaceVerify>::iterator itfvall = vfvall.begin();
	std::vector<int> vfg;
	for(;itfvall != vfvall.end(); itfvall++)
	{
		vfg.push_back(itfvall->getRecNo());
		cout<<itfvall->getVerifyTime().year()<<"��"<<itfvall->getVerifyTime().month()<<"��"
			<<itfvall->getVerifyTime().day()<<"��"<<itfvall->getVerifyTime().hour()<<"��"
			<<itfvall->getVerifyTime().minute()<<"��"<<itfvall->getVerifyTime().second()<<"��"
			<<"|"<<itfvall->getAge()<<"|"<<itfvall->getMale()<<"|"<<itfvall->getGlasses()
			<<"|"<<itfvall->getXangle()<<"|"<<itfvall->getYangle()<<"|"<<itfvall->getZangle()
			<<"|"<<itfvall->getSmile()<<"|"<<itfvall->getQuality()<<"|"<<itfvall->getScore()
			<<"|"<<itfvall->getImg().size()<<"|"<<itfvall->getSendFlag()<<"|"<<itfvall->getX()
			<<"|"<<itfvall->getY()<<"|"<<itfvall->getWidth()<<"|"<<itfvall->getHeight()<<std::endl;
	}
	sql.SetSendFlag(vfg);
	//��ҳ��ѯFaceVerify����
#if 0
	std::cout<<"-------------------------GetFaceVerifyPage begin--------------------------"<<std::endl;
	std::vector<CloudWalk::FaceVerify> vfvpg = sql.GetFaceVerifyPage(1, 30);
	std::vector<CloudWalk::FaceVerify>::iterator itfvpg = vfvpg.begin();
	for(;itfvpg != vfvpg.end(); itfvpg++)
	{
		cout<<itfvpg->getVerifyTime().year()<<"��"<<itfvpg->getVerifyTime().month()<<"��"
			<<itfvpg->getVerifyTime().day()<<"��"<<itfvpg->getVerifyTime().hour()<<"��"
			<<itfvpg->getVerifyTime().minute()<<"��"<<itfvpg->getVerifyTime().second()<<"��"
			<<"|"<<itfvpg->getAge()<<"|"<<itfvpg->getMale()<<"|"<<itfvpg->getGlasses()<<std::endl;
		cout<<"|"<<itfvpg->getXangle()<<"|"<<itfvpg->getYangle()<<"|"<<itfvpg->getZangle()
			<<"|"<<itfvpg->getSmile()<<"|"<<itfvpg->getQuality()<<"|"<<itfvpg->getScore()<<std::endl;
		cout<<"|"<<itfvpg->getImg().size()<<"|"<<itfvpg->getSendFlag()<<"|"<<itfvpg->getX()
			<<"|"<<itfvpg->getY()<<"|"<<itfvpg->getWidth()<<"|"<<itfvpg->getHeight()<<std::endl;
	}
#endif
	//log db :: log table
	//��һ�����ǲ���LogRecord, ������ÿ������demoʱ����Ҫ����LogRecord���ݡ����ע�͵��ˡ�
	//std::vector<CloudWalk::LogRecord> vlr;
	//for(int k =0; k<100; k++)
	//{
	//	CloudWalk::LogRecord lr;
	//	Poco::DateTime dt(2015, 10, 15, 12, 12, 12);
	//	lr.setOperTime(dt);
	//	lr.setType(9);
	//	lr.setMsg("log record insert");
	//	vlr.push_back(lr);
	//}
	//CloudWalk::InsertLogRecord(vlr); 

	//ȫ���ѯsyslog
	std::cout<<"-------------------------GetLogRecordAll begin--------------------------"<<std::endl;
	std::vector<CloudWalk::LogRecord> vlrall = sql.GetLogRecordAll();
	std::vector<CloudWalk::LogRecord>::iterator itlrall = vlrall.begin();
	for(;itlrall != vlrall.end();itlrall++)
	{
		cout<<itlrall->getOperTime().year()<<"��"<<itlrall->getOperTime().month()<<"��"
			<<itlrall->getOperTime().day()<<"��"<<itlrall->getOperTime().hour()<<"��"
			<<itlrall->getOperTime().minute()<<"��"<<itlrall->getOperTime().second()<<"��"
			<<"|"<<itlrall->getType()<<"|"<<itlrall->getMsg()<<std::endl;
	}
#if 0
	//��ҳ��ѯsyslog
	std::cout<<"-------------------------GetLogRecordPage begin-------------------------"<<std::endl;
	std::vector<CloudWalk::LogRecord> vlrpg = sql.GetLogRecordPage(4, 30);
	std::vector<CloudWalk::LogRecord>::iterator itlrpg = vlrpg.begin();
	for(;itlrpg != vlrpg.end();itlrpg++)
	{
		cout<<itlrpg->getOperTime().year()<<"��"<<itlrpg->getOperTime().month()<<"��"
			<<itlrpg->getOperTime().day()<<"��"<<itlrpg->getOperTime().hour()<<"��"
			<<itlrpg->getOperTime().minute()<<"��"<<itlrpg->getOperTime().second()<<"��"
			<<"|"<<itlrpg->getType()<<"|"<<itlrpg->getMsg()<<std::endl;
	}
#endif
}