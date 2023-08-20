#pragma once
#include "IInputStream.h"
#include "UndoRedo_globals.h"
#include "MShared_ptr.h"
#include "Defines.h"

class RealocMemory;

/*@brief record interface used for undo/redo*/
class IRecord
{
public:
	virtual void process(IInputStream& a_stream, RealocMemory& a_memory) = 0;
};

class UNDO_REDO_EXPORT RecordModification : public IRecord
{
private:
	ObjectUID m_objectUID;
	std::weak_ptr<IRecordObject> m_object;
	size_t m_recordDataOffset;

public:
	RecordModification(std::weak_ptr<IRecordObject>& a_pObject);
	virtual ~RecordModification() = default;
	void process(IInputStream& a_stream, RealocMemory& a_memory) final;
};


class UNDO_REDO_EXPORT RecordDeletion : public IRecord
{
private:
	ObjectUID m_objectUID;
	size_t m_recordDataOffset;

public:
	RecordDeletion(std::weak_ptr<IRecordObject>& a_pObject);
	virtual ~RecordDeletion() = default;
	void process(IInputStream& a_stream, RealocMemory& a_memory) final;
};