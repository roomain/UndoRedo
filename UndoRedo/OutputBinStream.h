#pragma once
#pragma once
/***********************************************
* @headerfile OutputBinStream.h
* @date 29 / 07 / 2023
* @author Roomain
************************************************/
#include "UndoRedo_globals.h"
#include "IOutputStream.h"
#include <memory>
#include "BinaryBuffer.h"

using BinaryBufferPtr = std::shared_ptr<BinaryBuffer>;

#pragma warning(push)
#pragma warning(disable : 4251)

class UNDO_REDO_EXPORT OutputBinStream : public IOutputStream
{
private:
	BinaryBufferPtr m_outputBuffer;	/*!< input buffer*/
	size_t m_offset = 0;				/*!< buffer offset*/

public:
	OutputBinStream() = default;
	explicit OutputBinStream(const BinaryBufferPtr& a_buffer);
	virtual ~OutputBinStream() = default;
	size_t offset()const final{ return m_outputBuffer ? m_outputBuffer->size() : 0; }
	void setBuffer(const BinaryBufferPtr& a_buffer);
	[[nodiscard]] bool hasBuffer()const noexcept { return m_outputBuffer.get() != nullptr; }
	IOutputStream& operator << (const unsigned long long& a_value)final;
	IOutputStream& operator << (const long long& a_value)final;
	IOutputStream& operator << (const bool a_value)final;
	IOutputStream& operator << (const char a_value)final;
	IOutputStream& operator << (const unsigned char a_value)final;
	IOutputStream& operator << (const short a_value)final;
	IOutputStream& operator << (const unsigned short a_value)final;
	IOutputStream& operator << (const unsigned int a_value)final;
	IOutputStream& operator << (const int a_value)final;
	IOutputStream& operator << (const double& a_value)final;
	IOutputStream& operator << (const float a_value)final;
	IOutputStream& operator << (const std::string& a_value)final;
	IOutputStream& operator << (const std::wstring& a_value)final;
};

#pragma warning(pop)
