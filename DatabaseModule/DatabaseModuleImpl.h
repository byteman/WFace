//////////////////////////////////////////////////////////////////////////  
///     COPYRIGHT NOTICE  
///     Copyright (c) 2012,  
///     All rights reserved.  
///  
/// @file   DatabaseModuleImpl.h  
/// @brief 	实现数据库接口文件
///  
/// 
///  
/// @version 1.0	
/// @author	 houchaoneng
/// @date    2012年9月21日22:39:01
///  
///  
///     修订说明：最初版本  
//////////////////////////////////////////////////////////////////////////

#ifndef DatabaseModuleImpl_H
#define DatabaseModuleImpl_H

#include "DatabaseModuleInc.h"
#include "DatabaseModuleConfig.h"

struct RS232_Para
{
	int comNo;
	int baud;
	int stopbits;
	int databits;
	int parity;
	int flowctrl;
	int mode;
};
class DATABASEMODULE_API DatabaseModuleImpl
{
public:
	DatabaseModuleImpl();

	~DatabaseModuleImpl();

public:
	void start();

	void stop();

	static DatabaseModuleImpl&instance();


public:
		///@brief
		///连接数据库
    int connect(std::string pDbPath);
		///@brief
		///断开数据库
    int disconnect();

		//--------------------------------参数------------------------------//
public:
		/** 
		 *	@brief	
		 *	读取字符串参数
		 *	@param Section 	读取参数分类
		 *	@param name 	读取参数Key
		 *	@return    		布尔型
		 */ 
	CORE::CData getParam(const char *Section,const  char *Name );

		/** 
		 *	@brief	
		 *	设置字符串参数
		 *	@param Section 	设置参数分类
		 *	@param name 	设置参数Key
		 *	@param value 	设置参数值
		 *	@return    		布尔型
		 */ 
	bool setParam(const char *Section,const  char *Name , const char *value);



		

		//--------------------------------业务数据库--------------------------//
public:		
		/** 
		 *	@brief	
		 *	根据相应的ID写数据库文件 
		 *	@param nType 	要写的数据
		 *	@param pInBuf 	输入的数据指针
		 *	@param nInLen 	输入的数据长度
		 *	@return    		写数据结果 
		 *	@note    		采用的是同步方式反映比较慢
		 */ 
    int setRecord(int nType,char *pInBuf,int nInLen);
		
		/** 
		 *	@brief	
		 *	从数据库中获得数据
		 *	@param nType 	要读取的数据表类型
		 *	@param pOutBuf 	输出的数据指针
		 *	@param nInLen 	输出的数据长度
		 *	@return    		读取数据结果 
		 */ 
    int getRecord(int nType,char *pOutBuf,int *nOutLen );
		
public:
private:
	
	CORE::Core_FastMutex		m_mutex;
};

#endif
