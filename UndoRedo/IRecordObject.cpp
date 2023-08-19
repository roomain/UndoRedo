#include "pch.h"
#include "IRecordObject.h"

std::atomic_bool IRecordObject::m_sEnableUIDGen = true;
std::atomic_ullong IRecordObject::m_UIDGen = 0;


IRecordObject::IRecordObject()
{
	m_objectUid = IRecordObject::m_sEnableUIDGen ? ++IRecordObject::m_UIDGen : 0;
}

IRecordObject::IRecordObject(IRecordObject&& a_other)noexcept : m_objectUid{ a_other.m_objectUid }
{
}

IRecordObject::IRecordObject(const IRecordObject& a_other)
{
	m_objectUid = IRecordObject::m_sEnableUIDGen ? ++IRecordObject::m_UIDGen : 0;
}
