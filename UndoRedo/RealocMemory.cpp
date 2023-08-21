#include "pch.h"
#include "RealocMemory.h"

MShared_ptr<IRecordObject> RealocMemory::realoc(const ObjectUID& a_uid, std::weak_ptr<RTTIDefinition>& a_objectDef)
{
	MShared_ptr<IRecordObject> pRealocObject;
	auto iter = m_undoRedoRealoc.find(a_uid);
	if (iter != m_undoRedoRealoc.end())
	{
		pRealocObject = iter->second.lock();
		if (!pRealocObject)
			m_undoRedoRealoc.erase(a_uid);
		else
			return pRealocObject;
	}
	
	if (!pRealocObject && a_objectDef.lock())
	{
		pRealocObject = a_objectDef.lock()->create(a_uid);
		m_undoRedoRealoc.try_emplace(a_uid, pRealocObject);
		m_sessionRealoc.try_emplace(a_uid, pRealocObject);
	}
	else
	{
		// log
	}
	return pRealocObject;
}

void RealocMemory::endSession()
{
	m_sessionRealoc.clear();
}