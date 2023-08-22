#pragma once
/***********************************************
* @headerfile InputBinStream.h
* @date 29 / 07 / 2023
* @author Roomain
************************************************/
#include "UndoRedo_globals.h"
#include "IInputStream.h"
#include <memory>
#include "BinaryBuffer.h"

using BinaryBufferPtr = std::shared_ptr<BinaryBuffer>;

#pragma warning(push)
#pragma warning(disable : 4251)

class UNDO_REDO_EXPORT InputBinStream : public IInputStream
{
private:
	BinaryBufferPtr m_inputBuffer;	/*!< input buffer*/
	size_t m_offset = 0;				/*!< buffer offset*/

public:
	InputBinStream() = default;
	explicit InputBinStream(const BinaryBufferPtr& a_buffer);
	virtual ~InputBinStream() = default;
	size_t offset()const final{ return m_inputBuffer ? m_inputBuffer->size() : 0; }
	IInputStream& setPos(const size_t& offset) final;
	IInputStream& shift(const size_t& offset) final;
	void atStart()final;
	void setBuffer(const BinaryBufferPtr& a_buffer);
	[[nodiscard]] bool hasBuffer()const noexcept { return m_inputBuffer.get() != nullptr; }
	//-----------------------------------------------------------------
	IInputStream& operator >> (unsigned long long& a_value) final;
	IInputStream& operator >> (long long& a_value) final;
	IInputStream& operator >> (bool& a_value) final;
	IInputStream& operator >> (char& a_value) final;
	IInputStream& operator >> (unsigned char& a_value) final;
	IInputStream& operator >> (short& a_value) final;
	IInputStream& operator >> (unsigned short& a_value) final;
	IInputStream& operator >> (unsigned int& a_value) final;
	IInputStream& operator >> (int& a_value) final;
	IInputStream& operator >> (double& a_value) final;
	IInputStream& operator >> (float& a_value) final;
	IInputStream& operator >> (std::string& a_value) final;
	IInputStream& operator >> (std::wstring& a_value) final;
};

#pragma warning(pop)
