#include "pch.h"
#include "Record.h"
#include "IInputStream.h"
#include "IOutputStream.h"
#include "IRecordObject.h"
#include "RealocMemory.h"

RecordModification::RecordModification(std::weak_ptr<IRecordObject>& a_pObject, IOutputStream& a_stream) : 
	m_object{ a_pObject }
{
	auto pObj = a_pObject.lock();
	if (pObj)
	{
		m_pObjectDef = pObj->definition();
		m_objectUID = pObj->uid();
		m_recordDataOffset = a_stream.offset();
		pObj->save(a_stream);
	}
	else
	{
		throw; // TODO
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

std::unique_ptr<IRecord> RecordModification::reverse(IOutputStream& a_stream)
{
	return std::make_unique<RecordModification>(m_object, a_stream);
}

//-----------------------------------------------------------------------------------------
RecordDeletion::RecordDeletion(std::weak_ptr<IRecordObject>& a_pObject, IOutputStream& a_stream)
{
	auto pObj = a_pObject.lock();
	if (pObj)
	{
		m_objectUID = pObj->uid();
		m_recordDataOffset = a_stream.offset();
		pObj->save(a_stream);
	}
	else
	{
		throw; // TODO
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

std::unique_ptr<IRecord> RecordDeletion::reverse(IOutputStream& a_stream)
{
	//
	return nullptr;
}
