#pragma once
#include <list>
#include <optional>
#include <string_view>
#include "RecordSession.h"
#include "RealocMemory.h"

using SessionList = std::list<RecordSession>;
using SessionIter = std::optional<SessionList::iterator>;

class UndoRedo
{
private:
	UndoRedo() = default;

	RealocMemory m_realocMemory;
	SessionList m_lRecordSessions;
	SessionIter m_curUndoSession;
	SessionIter m_curRedoSession;
	bool m_HasActiveSession = false;

public:
	static UndoRedo& instance();
	void startSession(const std::string_view& a_title);
	void endSession();
	[[nodiscard]] bool hasUndo()const;
	[[nodiscard]] bool hasRedo()const;
	RecordSession& currentSession();
	void undo();
	void redo();
};