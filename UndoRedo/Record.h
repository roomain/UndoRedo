#pragma once
#include <memory>
#include "MShared_ptr.h"
#include "Defines.h"

class RealocMemory;
class IInputStream;
class IOutputStream;

/*@brief record interface used for undo/redo*/
class IRecord
{
private:
	bool m_ignore = false;
public:
	void setIgnore(const bool a_ignore) { m_ignore = a_ignore; }
	bool ignored()const noexcept { return m_ignore; }
	virtual void process(IInputStream& a_stream, RealocMemory& a_memory) = 0; 
	virtual std::unique_ptr<IRecord> reverse(IOutputStream& a_stream) = 0;
};

class RecordModification : public IRecord
{
private:
	ObjectUID m_objectUID;
	std::weak_ptr<IRecordObject> m_object;
	size_t m_recordDataOffset;

public:
	RecordModification(std::weak_ptr<IRecordObject>& a_pObject, IOutputStream& a_stream);
	virtual ~RecordModification() = default;
	void process(IInputStream& a_stream, RealocMemory& a_memory) final;
	std::unique_ptr<IRecord> reverse(IOutputStream& a_stream) final;
};


class RecordDeletion : public IRecord
{
private:
	ObjectUID m_objectUID;
	size_t m_recordDataOffset;

public:
	RecordDeletion(std::weak_ptr<IRecordObject>& a_pObject, IOutputStream& a_stream);
	virtual ~RecordDeletion() = default;
	void process(IInputStream& a_stream, RealocMemory& a_memory) final;
	std::unique_ptr<IRecord> reverse(IOutputStream& a_stream) final;
};