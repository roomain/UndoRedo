#pragma once
#include "Record.h"
#include "RealocMemory.h"

template<typename Key>
class TRecordInsert
{
private:
	Key m_objectKey;
	ObjectUID m_newObjectUID;
	std::weak_ptr<IRecordObject> m_pNewObject;

public:
	TRecordInsert(const Key& a_key, std::weak_ptr<IRecordObject>& a_pNewObject)
	{
		//
	}
	virtual ~TRecordInsert() = default;
	void process(IInputStream& a_stream, RealocMemory& a_memory) final
	{
		// TODO
	}
};

template<typename Key>
class TRecordRemoved
{
private:
	Key m_objectKey;
	ObjectUID m_RemovedObjectUID;
	std::weak_ptr<IRecordObject> m_pRemovedObject;

public:
	TRecordRemoved(const Key& a_key, std::weak_ptr<IRecordObject>& a_pRemovedObject, IOutputStream& a_stream)
	{
		//
	}
	virtual ~TRecordRemoved() = default;

	void process(IInputStream& a_stream, RealocMemory& a_memory) final
	{
		// TODO
	}

	std::unique_ptr<IRecord> reverse(IOutputStream& a_stream) final
	{
		//
		return nullptr;
	}
};

template<typename Key>
class TRecordChanged
{
private:
	Key m_objectKey;
	ObjectUID m_oldObjectUID;
	ObjectUID m_newObjectUID;
	std::weak_ptr<IRecordObject> m_pNewObject;
	std::weak_ptr<IRecordObject> m_pOldObject;

public:
	TRecordChanged(const Key& a_key, std::weak_ptr<IRecordObject>& a_pNewObject,
		std::weak_ptr<IRecordObject>& a_pOldObject)
	{
		//
	}
	virtual ~TRecordChanged() = default;
	void process(IInputStream& a_stream, RealocMemory& a_memory) final
	{
		// TODO
	}


	std::unique_ptr<IRecord> reverse(IOutputStream& a_stream) final
	{
		//
		return nullptr;
	}
};
