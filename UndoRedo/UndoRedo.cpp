#include "pch.h"
#include "UndoRedo.h"

UndoRedo& UndoRedo::instance()
{
	static UndoRedo s_instance;
	return s_instance;
}


void UndoRedo::startSession(const std::string_view& a_title)
{
	if (!m_lRecordSessions.empty() && m_curUndoSession != std::prev(m_lRecordSessions.end()))
	{
		if (m_curUndoSession.has_value())
			m_lRecordSessions.erase(m_curUndoSession.value(), m_lRecordSessions.end());
	}
	m_lRecordSessions.emplace_back(a_title);
	m_curUndoSession = std::prev(m_lRecordSessions.end());
	m_curRedoSession.reset();
	m_HasActiveSession = true;
}

void UndoRedo::endSession()
{
	m_HasActiveSession = false;
}

bool UndoRedo::hasUndo()const
{
	return m_curUndoSession.has_value();
}

bool UndoRedo::hasRedo()const
{
	return m_curRedoSession.has_value();
}

RecordSession& UndoRedo::currentSession()
{
	if (m_curUndoSession.has_value())
		return *(m_curUndoSession.value());
	UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_No_session)
}

void UndoRedo::undo()
{
	if (hasUndo() && !m_HasActiveSession)
	{
		m_curUndoSession.value()->undo(m_realocMemory);
		m_realocMemory.endSession();
		if (m_curUndoSession.value() == m_lRecordSessions.begin())
		{
			m_curUndoSession.reset();
			m_curRedoSession = m_lRecordSessions.begin();
		}
		else
		{
			m_curRedoSession = m_curUndoSession.value();
			m_curUndoSession.value()--;
		}
	}
}

void UndoRedo::redo()
{
	if (hasRedo() && !m_HasActiveSession)
	{
		m_curRedoSession.value()->redo(m_realocMemory);
		m_realocMemory.endSession();
		if (m_curRedoSession.value() == std::prev(m_lRecordSessions.end()))
		{
			m_curRedoSession.reset();
			m_curUndoSession = std::prev(m_lRecordSessions.end());
		}
		else
		{
			m_curUndoSession = m_curRedoSession.value();
			m_curRedoSession.value()++;
		}
	}
}