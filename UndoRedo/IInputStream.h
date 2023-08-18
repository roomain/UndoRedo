#pragma once

/***********************************************
* @headerfile IInputStream.h
* @date 17 / 07 / 2023
* @author Roomain
************************************************/
#include <string>
#include <memory>

/*@brief Base of input stream*/
class IInputStream
{
public:
	virtual IInputStream& setPos(const size_t& a_offset) = 0;
	virtual IInputStream& shift(const size_t& a_offset) = 0;
	virtual void atStart() = 0;
	//-----------------------------------------------------------------
	virtual IInputStream& operator >> (unsigned long long&) = 0;
	virtual IInputStream& operator >> (long long&) = 0;
	virtual IInputStream& operator >> (bool&) = 0;
	virtual IInputStream& operator >> (char&) = 0;
	virtual IInputStream& operator >> (unsigned char&) = 0;
	virtual IInputStream& operator >> (short&) = 0;
	virtual IInputStream& operator >> (unsigned short&) = 0;
	virtual IInputStream& operator >> (unsigned int&) = 0;
	virtual IInputStream& operator >> (int&) = 0;
	virtual IInputStream& operator >> (double&) = 0;
	virtual IInputStream& operator >> (float&) = 0;
	virtual IInputStream& operator >> (std::string&) = 0;
	virtual IInputStream& operator >> (std::wstring&) = 0;
};

using IInputStreamPtr = std::shared_ptr<IInputStream>;