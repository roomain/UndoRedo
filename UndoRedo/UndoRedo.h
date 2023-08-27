#pragma once
#include <list>
#include <optional>
#include <string_view>
#include "RecordSession.h"
#include "RealocMemory.h"
#include "UndoRedo_globals.h"

#pragma warning(push)
#pragma warning(disable : 4251)

using SessionList = std::list<RecordSession>;
using SessionIter = std::optional<SessionList::iterator>;

class UNDO_REDO_EXPORT UndoRedo
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
	bool sessionStarted()const;
	void undo();
	void redo();
	void clear();
};
#pragma warning(pop)