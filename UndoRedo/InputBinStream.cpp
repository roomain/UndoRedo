#include "pch.h"
#include "InputBinStream.h"
#include "BinaryBuffer.h"

InputBinStream::InputBinStream(const BinaryBufferPtr& a_buffer) : m_inputBuffer{ a_buffer }
{
	//
}

void InputBinStream::setBuffer(const BinaryBufferPtr& a_buffer)
{
	m_inputBuffer = a_buffer;
}

IInputStream& InputBinStream::setPos(const size_t& a_offset)
{
	m_offset = a_offset;
	return *this;
}

IInputStream& InputBinStream::shift(const size_t& a_offset)
{
	m_offset += a_offset;
	return *this;
}

void InputBinStream::atStart()
{
	m_offset = 0;
}

//-----------------------------------------------------------------
IInputStream& InputBinStream::operator >> (unsigned long long& a_value)
{
	if (m_inputBuffer)
		m_offset = m_inputBuffer->read(a_value, m_offset);
	
	return *this;
}

IInputStream& InputBinStream::operator >> (long long& a_value)
{
	if (m_inputBuffer)
		m_offset = m_inputBuffer->read(a_value, m_offset);

	return *this;
}

IInputStream& InputBinStream::operator >> (bool& a_value)
{
	if (m_inputBuffer)
		m_offset = m_inputBuffer->read(a_value, m_offset);

	return *this;
}

IInputStream& InputBinStream::operator >> (char& a_value)
{
	if (m_inputBuffer)
		m_offset = m_inputBuffer->read(a_value, m_offset);

	return *this;
}

IInputStream& InputBinStream::operator >> (unsigned char& a_value)
{
	if (m_inputBuffer)
		m_offset = m_inputBuffer->read(a_value, m_offset);

	return *this;
}

IInputStream& InputBinStream::operator >> (short& a_value)
{
	if (m_inputBuffer)
		m_offset = m_inputBuffer->read(a_value, m_offset);

	return *this;
}

IInputStream& InputBinStream::operator >> (unsigned short& a_value)
{
	if (m_inputBuffer)
		m_offset = m_inputBuffer->read(a_value, m_offset);

	return *this;
}

IInputStream& InputBinStream::operator >> (unsigned int& a_value)
{
	if (m_inputBuffer)
		m_offset = m_inputBuffer->read(a_value, m_offset);

	return *this;
}

IInputStream& InputBinStream::operator >> (int& a_value)
{
	if (m_inputBuffer)
		m_offset = m_inputBuffer->read(a_value, m_offset);

	return *this;
}

IInputStream& InputBinStream::operator >> (double& a_value)
{
	if (m_inputBuffer)
		m_offset = m_inputBuffer->read(a_value, m_offset);

	return *this;
}

IInputStream& InputBinStream::operator >> (float& a_value)
{
	if (m_inputBuffer)
		m_offset = m_inputBuffer->read(a_value, m_offset);

	return *this;
}

IInputStream& InputBinStream::operator >> (std::string& a_value)
{
	if (m_inputBuffer)
	{
		size_t stringLen = 0;
		m_offset = m_inputBuffer->read(stringLen, m_offset);
		if (stringLen > 0)
		{
			m_offset = m_inputBuffer->read<char>(stringLen, std::back_inserter(a_value), m_offset);
		}
	}

	return *this;
}


IInputStream& InputBinStream::operator >> (std::wstring& a_value)
{
	if (m_inputBuffer)
	{
		size_t stringLen = 0;
		m_offset = m_inputBuffer->read(stringLen, m_offset);
		if (stringLen > 0)
		{
			m_offset = m_inputBuffer->read<wchar_t>(stringLen, std::back_inserter(a_value), m_offset);
		}
	}

	return *this;
}

