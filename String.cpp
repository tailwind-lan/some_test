#include "stdafx.h"
#include "String.h"
#include <string>
#include <time.h>

// 普通构造函数
String::String(const char *str)
{
	if (str == nullptr)
	{
		m_data = new char[1]; // 对空字符串自动申请存放结束标志'\0'的
		*m_data = '\0'; // 加分点：对m_data加NULL判断
	}
	else
	{
		m_data = new char[strlen(str) + 1]; // 申请内存
		strcpy_s(m_data, strlen(m_data) - 1, str); // 加分点：对m_data加NULL判断
	}
}

// 普通构造函数
String::String(int i)
{
	char temp[100];
	sprintf_s(temp, "Test for_each %u", i);

	m_data = new char[strlen(temp) + 1]; // 申请内存
	strcpy_s(m_data, strlen(m_data) - 1, temp); // 加分点：对m_data加NULL判断
}

// String的析构函数
String::~String(void)
{
	delete []m_data; // 由于m_data是内部数据类型，也可以写成"delete m_data;"
}

// 拷贝构造函数
String::String(const String &other) // 输入参数为const型
{
	m_data = new char[strlen(other.m_data) + 1]; // 申请内存
	strcpy_s(m_data, strlen(m_data) - 1, other.m_data); // 加分点：对m_data加NULL判断
}

// 赋值函数
String &String::operator = (const String &other) // 输入参数为const型
{
	// 检查自赋值
	if ( this == &other ) return *this;
	delete []m_data; // 释放原有的内存资源
	m_data = new char[strlen(other.m_data) + 1]; // 申请内存
	strcpy_s(m_data, strlen(m_data) - 1, other.m_data); // 加分点：对m_data加NULL判断
	return *this; // 返回本对象的引用
}

void String::WriteToFile()
{
	if (m_data == nullptr)
		return;

	// 写日志
	time_t tCurrTime;
	time(&tCurrTime);
	tm *pTmFile = new tm();
	::localtime_s(pTmFile, &tCurrTime);
	char szLogFile[256] = "";
	_snprintf_s(szLogFile, sizeof(szLogFile) - 1, "syslog%04d-%02d-%02d.log", pTmFile->tm_year + 1900, pTmFile->tm_mon + 1, pTmFile->tm_mday);

	// Open log file
	FILE* fp;
	::fopen_s(&fp, szLogFile, "a+");
	if (!fp)
		return;;

	char szTime[128] = { 0 };
	_snprintf_s(szTime, sizeof(szTime) - 1, "%04d-%02d-%02d %d:%d:%d", pTmFile->tm_year + 1900, pTmFile->tm_mon + 1, pTmFile->tm_mday, pTmFile->tm_hour, pTmFile->tm_min, pTmFile->tm_sec);
	::fprintf(fp, "%s -- %s\n", szTime, m_data);

	long dwLogSize = ::ftell(fp);
	::fclose(fp);

	// Log file is too large
	const int MAX_LOGFILESIZE = 10 * 1024 * 1024;
	if (dwLogSize >= MAX_LOGFILESIZE)
	{
		time_t long_time = ::time(nullptr);
		struct tm *pTime = new tm();
		::localtime_s(pTime, &long_time); /* Convert to local time. */;

		long dwTime = pTime->tm_year % 100 * 10000 + (pTime->tm_mon + 1) * 100 + pTime->tm_mday;
		char szBackupFile[256] = "";
		_snprintf_s(szBackupFile, sizeof(szBackupFile) - 1, "syslog%04d-%02d-%02d.%u.log", pTmFile->tm_year + 1900, pTmFile->tm_mon + 1, pTmFile->tm_mday, dwTime);
		::rename(szLogFile, szBackupFile);
	}
}