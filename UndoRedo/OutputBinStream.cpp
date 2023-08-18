#include "pch.h"
#include "OutputBinStream.h"
#include "BinaryBuffer.h"

OutputBinStream::OutputBinStream(const BinaryBufferPtr& a_buffer) : m_outputBuffer{ a_buffer }
{
	//
}

void OutputBinStream::setBuffer(const BinaryBufferPtr& a_buffer)
{
	m_outputBuffer = a_buffer;
}

IOutputStream& OutputBinStream::operator << (const unsigned long long& a_value)
{
	if (m_outputBuffer)
		m_outputBuffer->write(a_value);
	return *this;
}

IOutputStream& OutputBinStream::operator << (const long long& a_value)
{
	if (m_outputBuffer)
		m_outputBuffer->write(a_value);
	return *this;
}

IOutputStream& OutputBinStream::operator << (const bool a_value)
{
	if (m_outputBuffer)
		m_outputBuffer->write(a_value);
	return *this;
}

IOutputStream& OutputBinStream::operator << (const char a_value)
{
	if (m_outputBuffer)
		m_outputBuffer->write(a_value);
	return *this;
}

IOutputStream& OutputBinStream::operator << (const unsigned char a_value)
{
	if (m_outputBuffer)
		m_outputBuffer->write(a_value);
	return *this;
}

IOutputStream& OutputBinStream::operator << (const short a_value)
{
	if (m_outputBuffer)
		m_outputBuffer->write(a_value);
	return *this;
}

IOutputStream& OutputBinStream::operator << (const unsigned short a_value)
{
	if (m_outputBuffer)
		m_outputBuffer->write(a_value);
	return *this;
}

IOutputStream& OutputBinStream::operator << (const unsigned int a_value)
{
	if (m_outputBuffer)
		m_outputBuffer->write(a_value);
	return *this;
}

IOutputStream& OutputBinStream::operator << (const int a_value)
{
	if (m_outputBuffer)
		m_outputBuffer->write(a_value);
	return *this;
}

IOutputStream& OutputBinStream::operator << (const double& a_value)
{
	if (m_outputBuffer)
		m_outputBuffer->write(a_value);
	return *this;
}

IOutputStream& OutputBinStream::operator << (const float a_value)
{
	if (m_outputBuffer)
		m_outputBuffer->write(a_value);
	return *this;
}

IOutputStream& OutputBinStream::operator << (const std::string& a_value)
{
	if (m_outputBuffer)
	{
		m_outputBuffer->write(a_value.length());
		m_outputBuffer->write(a_value.c_str(), a_value.length());
	}
	return *this;
}

IOutputStream& OutputBinStream::operator << (const std::wstring& a_value)
{
	if (m_outputBuffer)
	{
		m_outputBuffer->write(a_value.length());
		m_outputBuffer->write(a_value.c_str(), a_value.length());
	}
	return *this;
}