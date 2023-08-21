#include "pch.h"
#include "Record.h"
#include "IInputStream.h"
#include "IOutputStream.h"

RecordModification::RecordModification(std::weak_ptr<IRecordObject>& a_pObject, IOutputStream& a_stream)
{
	//
}

void RecordModification::process(IInputStream& a_stream, RealocMemory& a_memory)
{
	//
}

std::unique_ptr<IRecord> RecordModification::reverse(IOutputStream& a_stream)
{
	//
	return nullptr;
}

//-----------------------------------------------------------------------------------------
RecordDeletion::RecordDeletion(std::weak_ptr<IRecordObject>& a_pObject, IOutputStream& a_stream)
{
	//
}

void RecordDeletion::process(IInputStream& a_stream, RealocMemory& a_memory)
{
	//
}

std::unique_ptr<IRecord> RecordDeletion::reverse(IOutputStream& a_stream)
{
	//
	return nullptr;
}
