#pragma once
#include <variant>
#include <memory>
#include "Reference.h"

class IRecordObject;
class RealocMemory;

using RecordDataVar = std::variant<std::weak_ptr<IRecordObject>, Reference<IRecordObject>>;

class RecordData
{
private:
	RecordDataVar m_data;
	std::weak_ptr<RTTIDefinition> m_pDefinition;
	ObjectUID m_objectUid;	/*!< object unique identifier*/

	enum VariantType : int
	{
		weakpointer = 0,
		reference = 1
	};

public:
	RecordData(IRecordObject* a_object);
	~RecordData();
	IRecordObject* const get()const;
	IRecordObject* const get(RealocMemory& a_memory);
};