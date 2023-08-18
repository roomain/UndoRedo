#pragma once
/***********************************************
* @headerfile IOutputStream.h
* @date 17 / 07 / 2023
* @author Roomain
************************************************/
#include <string>
#include <memory>

/*@brief Base of input stream*/
class IOutputStream
{
public:
	virtual IOutputStream& operator << (const unsigned long long&) = 0;
	virtual IOutputStream& operator << (const long long&) = 0;
	virtual IOutputStream& operator << (const bool) = 0;
	virtual IOutputStream& operator << (const char) = 0;
	virtual IOutputStream& operator << (const unsigned char) = 0;
	virtual IOutputStream& operator << (const short) = 0;
	virtual IOutputStream& operator << (const unsigned short) = 0;
	virtual IOutputStream& operator << (const unsigned int) = 0;
	virtual IOutputStream& operator << (const int) = 0;
	virtual IOutputStream& operator << (const double&) = 0;
	virtual IOutputStream& operator << (const float) = 0;
	virtual IOutputStream& operator << (const std::string&) = 0;
	virtual IOutputStream& operator << (const std::wstring&) = 0;
};

using IOutputStreamPtr = std::shared_ptr<IOutputStream>;