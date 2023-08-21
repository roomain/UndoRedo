#pragma once
#include <list>
#include <string_view>
#include <string>
#include <chrono>
#include "RealocMemory.h"
#include "Record.h"
#include "InputBinStream.h"
#include "OutputBinStream.h"

using SessionTimePoint = std::chrono::time_point<std::chrono::system_clock>;
class RecordSession
{
private:
	SessionTimePoint m_timePoint;
	std::list<std::unique_ptr<IRecord>> m_undoRecord;
	std::list<std::unique_ptr<IRecord>> m_redoRecord;
	InputBinStream m_loadStream;
	OutputBinStream m_saveStream;
	std::string m_title;

public:
	RecordSession() = delete;
	RecordSession(const std::string_view& a_title);
	const std::string& title()const noexcept { return m_title; }
	void undo(RealocMemory& a_memory);
	void redo(RealocMemory& a_memory);
	OutputBinStream& saveStream()noexcept { return m_saveStream; }
	RecordSession& operator = (const RecordSession&) = delete;
};