#include "pch.h"
#include "RecordSession.h"
#include "BinaryBuffer.h"


RecordSession::RecordSession(const std::string_view& a_title) : m_title{a_title}
{
	m_timePoint = std::chrono::system_clock::now();
	auto pBinBuffer = std::make_shared<BinaryBuffer>();
	m_loadStream.setBuffer(pBinBuffer);
	m_saveStream.setBuffer(pBinBuffer);
}

void RecordSession::undo(RealocMemory& a_memory)
{
	bool bGenRedoRecord = m_redoRecord.empty();
	for (auto&& pRecord : m_undoRecord)
	{
		if (bGenRedoRecord)
		{
			//
		}
		pRecord->process(m_loadStream, a_memory);
	}
}

void RecordSession::redo(RealocMemory& a_memory)
{
	for (auto&& pRecord : m_redoRecord)
	{
		pRecord->process(m_loadStream, a_memory);
	}
}