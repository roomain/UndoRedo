#include "RecordData.h"
#include "IRecordObject.h"
#include "RealocMemory.h"


RecordData::RecordData(IRecordObject* a_object)
{
	m_pDefinition = a_object->isA();
	if (a_object->isShared())
	{
		m_data = a_object->weak_from_this();
	}
	else
	{
		m_data = make_ref(*a_object);
	}
}

RecordData::~RecordData()
{
	//
}

IRecordObject* const RecordData::get()const
{
	IRecordObject* pointer = nullptr;
	switch (m_data.index())
	{
	case VariantType::weakpointer:
		pointer = std::get<std::weak_ptr<IRecordObject>>(m_data).lock().get();
		break;

	case VariantType::reference:
		pointer = std::get<Reference<IRecordObject>>(m_data).pointer();
		break;

	default:
		break;
	}
	return pointer;
}

IRecordObject* const RecordData::get(RealocMemory& a_memory)
{
	IRecordObject* pointer = nullptr;
	switch (m_data.index())
	{
	case VariantType::weakpointer:
	{
		pointer = std::get<std::weak_ptr<IRecordObject>>(m_data).lock().get();
		if (pointer == nullptr)
		{
			auto pShared = a_memory.realoc(m_objectUid, m_pDefinition);
			pointer = pShared.get();
		}
	}
		break;

	case VariantType::reference:
	{
		pointer = std::get<Reference<IRecordObject>>(m_data).pointer();
	}
		break;

	default:
		break;
	}
	return pointer;
}