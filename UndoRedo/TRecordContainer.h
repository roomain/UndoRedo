#pragma once
#include <memory>
#include "Record.h"
#include "RealocMemory.h"
#include "TIContainer.h"
#include "UndoRedoException.h"

template<typename Key>
class TRecordContainer : public IRecord
{
private:
	ObjectUID m_containerObjectUID;
	std::weak_ptr<TIContainer<Key>> m_pContainerObject;
	std::weak_ptr<RTTIDefinition> m_pObjectDef;

public:
	TRecordContainer(const std::weak_ptr<TIContainer<Key>>& a_pContainer) : m_pContainerObject{ a_pContainer }
	{
		auto pObj = m_pContainerObject.lock();
		if (pObj)
		{
			m_pObjectDef = pObj->definition();
			m_containerObjectUID = pObj->uid();
		}
		else
		{
			UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_Deleted)
		}
	}

	virtual ~TRecordContainer() = default;
	std::shared_ptr<TIContainer<Key>> getContainer(RealocMemory& a_memory)
	{
		auto pObj = m_pContainerObject.lock();
		if (!pObj)
		{
			pObj = a_memory.realoc(m_containerObjectUID, m_pObjectDef);
			m_pContainerObject = pObj;
		}

		return pObj;
	}
};


template<typename Key>
class TRecordInsert : public TRecordContainer<Key>
{
private:
	Key m_objectKey;
	ObjectUID m_newObjectUID;
	std::weak_ptr<IRecordObject> m_pObject;

public:
	TRecordInsert(const std::weak_ptr<TIContainer<Key>>& a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pNewObject) :
		TRecordContainer<Key>(a_pContainer), m_objectKey{ a_key }, m_pObject{ a_pNewObject }
	{
		auto pObj = m_pObject.lock();
		if (pObj)
		{
			IRecordObject = pObj->uid();
		}
		else
		{
			UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_Deleted)
		}
	}

	virtual ~TRecordInsert() = default;

	void process(IInputStream& a_stream, RealocMemory& a_memory) final
	{	
		auto pContainer = this->getContainer(a_memory);
		if (pContainer)
		{
			pContainer->record_eraseAt(m_objectKey);
		}
		else
		{
			UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_Deleted)
		}
	}

	std::shared_ptr<IRecord> reverse(IOutputStream& a_stream) final
	{
		return std::make_shared<TRecordRemoved>(this->getContainer(), m_objectKey, m_pObject);
	}

};

template<typename Key>
class TRecordRemoved : public TRecordContainer<Key>
{
private:
	Key m_objectKey;
	ObjectUID m_RemovedObjectUID;
	std::weak_ptr<IRecordObject> m_pRemovedObject;

public:
	TRecordRemoved(const std::weak_ptr<TIContainer<Key>>& a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pRemovedObject) :
		TRecordContainer<Key>(a_pContainer), m_objectKey{ a_key }, m_pRemovedObject{ a_pRemovedObject }
	{
		auto pObj = m_pRemovedObject.lock();
		if (pObj)
		{
			IRecordObject = pObj->uid();
		}
		else
		{
			UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_Deleted)
		}
	}
	virtual ~TRecordRemoved() = default;

	void process(IInputStream& a_stream, RealocMemory& a_memory) final
	{
		auto pContainer = this->getContainer(a_memory);
		if (pContainer)
		{
			auto pObj = m_pRemovedObject.lock();
			if(!pObj)
				pObj = a_memory.realoc(m_RemovedObjectUID, nullptr);

			if (pObj)
			{
				pContainer->record_insertAt(m_objectKey, pObj);
			}
			else
			{
				// LOG
			}
		}
		else
		{
			UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_Deleted)
		}
	}

	std::shared_ptr<IRecord> reverse(IOutputStream& a_stream) final
	{
		return std::make_shared<TRecordInsert>(this->getContainer(), m_objectKey, m_pRemovedObject);
	}
};

template<typename Key>
class TRecordChanged : public TRecordContainer<Key>
{
private:
	Key m_objectKey;
	ObjectUID m_oldObjectUID;
	ObjectUID m_newObjectUID;
	std::weak_ptr<IRecordObject> m_pNewObject;
	std::weak_ptr<IRecordObject> m_pOldObject;

public:
	TRecordChanged(const std::weak_ptr<TIContainer<Key>>& a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pNewObject,
		const std::weak_ptr<IRecordObject>& a_pOldObject) :
		TRecordContainer<Key>(a_pContainer), m_objectKey{ a_key }, m_pNewObject{ a_pNewObject }, m_pOldObject{ a_pOldObject }
	{
		auto pObj = m_pNewObject.lock();
		if (pObj)
		{
			m_newObjectUID = pObj->uid();
		}
		else
		{
			UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_Deleted)
		}

		pObj = m_pOldObject.lock();
		if (pObj)
		{
			m_oldObjectUID = pObj->uid();
		}
		else
		{
			UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_Deleted)
		}
	}
	virtual ~TRecordChanged() = default;
	void process(IInputStream& a_stream, RealocMemory& a_memory) final
	{
		// TODO
	}


	std::shared_ptr<IRecord> reverse(IOutputStream& a_stream) final
	{
		//
		return nullptr;
	}
};
