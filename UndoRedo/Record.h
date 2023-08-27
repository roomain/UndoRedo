#pragma once
#include <memory>
#include "MShared_ptr.h"
#include "Defines.h"
#include "RTTIDefinition.h"


class RealocMemory;
class IInputStream;
class IOutputStream;
class IRecordObject;
class RTTIDefinition;

/*@brief record interface used for undo/redo*/
class IRecord
{
private:
	bool m_ignore = false;
public:
	void setIgnore(const bool a_ignore) { m_ignore = a_ignore; }
	bool ignored()const noexcept { return m_ignore; }
	/*@brief indicate if record can have a reverse*/
	virtual bool hasReverse()const noexcept { return true; }
	/*@brief process record*/
	virtual void process(IInputStream& a_stream, RealocMemory& a_memory) = 0; 
	/*@brief gen reverse record*/
	virtual std::shared_ptr<IRecord> reverse(IOutputStream& a_stream) { return  nullptr; }
};


class RecordModification : public IRecord
{
private:
	ObjectUID m_objectUID;
	std::weak_ptr<IRecordObject> m_object;
	size_t m_recordDataOffset;
	std::weak_ptr<RTTIDefinition> m_pObjectDef;

public:
	RecordModification(const std::weak_ptr<IRecordObject>& a_pObject, IOutputStream& a_stream);
	virtual ~RecordModification() = default;
	void process(IInputStream& a_stream, RealocMemory& a_memory) final;
	std::shared_ptr<IRecord> reverse(IOutputStream& a_stream) final;
};


class RecordDeletion : public IRecord
{
private:
	ObjectUID m_objectUID;
	size_t m_recordDataOffset;
	std::weak_ptr<RTTIDefinition> m_pObjectDef;

public:
	RecordDeletion(const std::weak_ptr<IRecordObject>& a_pObject, IOutputStream& a_stream);
	virtual ~RecordDeletion() = default;
	bool hasReverse()const noexcept final{ return false; }
	void process(IInputStream& a_stream, RealocMemory& a_memory) final;
};