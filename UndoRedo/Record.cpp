#include "pch.h"
#include "Record.h"
#include "IInputStream.h"
#include "IOutputStream.h"
#include "IRecordObject.h"
#include "RealocMemory.h"

RecordModification::RecordModification(const std::weak_ptr<IRecordObject>& a_pObject, IOutputStream& a_stream) : 
	m_object{ a_pObject }
{
	auto pObj = a_pObject.lock();
	if (pObj)
	{
		m_pObjectDef = pObj->isA();
		m_objectUID = pObj->uid();
		m_recordDataOffset = a_stream.offset();
		pObj->save(a_stream);
	}
	else
	{
		UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_Deleted)
	}
}

void RecordModification::process(IInputStream& a_stream, RealocMemory& a_memory)
{
	auto pObj = m_object.lock();
	if (!pObj)
	{
		pObj = a_memory.realoc(m_objectUID, m_pObjectDef);
		m_object = pObj;
	}

	if (pObj)
	{
		a_stream.setPos(m_recordDataOffset);
		pObj->load(a_stream);
	}
	else
	{
		// log
	}
}

std::shared_ptr<IRecord> RecordModification::reverse(RealocMemory& a_memory, IOutputStream& a_stream)
{
	return std::make_shared<RecordModification>(m_object, a_stream);
}

//-----------------------------------------------------------------------------------------
RecordDeletion::RecordDeletion(const IRecordObject* a_pObject, IOutputStream& a_stream)
{
	if (a_pObject)
	{
		m_pObjectDef = a_pObject->isA();
		m_objectUID = a_pObject->uid();
		m_recordDataOffset = a_stream.offset();
		a_pObject->save(a_stream);
	}
	else
	{
		UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_Deleted)
	}
}

void RecordDeletion::process(IInputStream& a_stream, RealocMemory& a_memory)
{
	auto pObject = a_memory.realoc(m_objectUID, m_pObjectDef);
	if (pObject)
	{
		a_stream.setPos(m_recordDataOffset);
		pObject->load(a_stream);
	}
	else
	{
		// log
	}
}

