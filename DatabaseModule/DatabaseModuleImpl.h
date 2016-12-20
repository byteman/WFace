//////////////////////////////////////////////////////////////////////////  
///     COPYRIGHT NOTICE  
///     Copyright (c) 2012,  
///     All rights reserved.  
///  
/// @file   DatabaseModuleImpl.h  
/// @brief 	ʵ�����ݿ�ӿ��ļ�
///  
/// 
///  
/// @version 1.0	
/// @author	 houchaoneng
/// @date    2012��9��21��22:39:01
///  
///  
///     �޶�˵��������汾  
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
		///�������ݿ�
    int connect(std::string pDbPath);
		///@brief
		///�Ͽ����ݿ�
    int disconnect();

		//--------------------------------����------------------------------//
public:
		/** 
		 *	@brief	
		 *	��ȡ�ַ�������
		 *	@param Section 	��ȡ��������
		 *	@param name 	��ȡ����Key
		 *	@return    		������
		 */ 
	CORE::CData getParam(const char *Section,const  char *Name );

		/** 
		 *	@brief	
		 *	�����ַ�������
		 *	@param Section 	���ò�������
		 *	@param name 	���ò���Key
		 *	@param value 	���ò���ֵ
		 *	@return    		������
		 */ 
	bool setParam(const char *Section,const  char *Name , const char *value);



		

		//--------------------------------ҵ�����ݿ�--------------------------//
public:		
		/** 
		 *	@brief	
		 *	������Ӧ��IDд���ݿ��ļ� 
		 *	@param nType 	Ҫд������
		 *	@param pInBuf 	���������ָ��
		 *	@param nInLen 	��������ݳ���
		 *	@return    		д���ݽ�� 
		 *	@note    		���õ���ͬ����ʽ��ӳ�Ƚ���
		 */ 
    int setRecord(int nType,char *pInBuf,int nInLen);
		
		/** 
		 *	@brief	
		 *	�����ݿ��л������
		 *	@param nType 	Ҫ��ȡ�����ݱ�����
		 *	@param pOutBuf 	���������ָ��
		 *	@param nInLen 	��������ݳ���
		 *	@return    		��ȡ���ݽ�� 
		 */ 
    int getRecord(int nType,char *pOutBuf,int *nOutLen );
		
public:
private:
	
	CORE::Core_FastMutex		m_mutex;
};

#endif
