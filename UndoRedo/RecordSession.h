#pragma once
#include <list>
#include "RealocMemory.h"
#include "Record.h"
#include "InputBinStream.h"
#include "OutputBinStream.h"

class RecordSession
{
private:
	std::list<std::shared_ptr<IRecord>> m_undoRecord;
	std::list<std::shared_ptr<IRecord>> m_redoRecord;
	InputBinStream m_loadStream;
	OutputBinStream m_saveStream;

public:
	void undo();
	void redo();
};