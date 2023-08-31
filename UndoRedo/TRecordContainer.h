#pragma once
#include <memory>
#include "Record.h"
#include "RealocMemory.h"
#include "TIContainer.h"
#include "UndoRedoException.h"

class RTTIDefinition;

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
			m_pObjectDef = pObj->isA();
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
			pObj = MStatic_pointer_cast<TIContainer<Key>>(a_memory.realoc(m_containerObjectUID, m_pObjectDef));
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
			m_newObjectUID = pObj->uid();
		}
		else
		{
			UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_Deleted)
		}
	}

	TRecordInsert(const std::weak_ptr<TIContainer<Key>>& a_pContainer, const Key& a_key, const ObjectUID& a_pNewObjectUID) :
		TRecordContainer<Key>(a_pContainer), m_objectKey{ a_key }, m_newObjectUID{ a_pNewObjectUID }
	{
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

	std::shared_ptr<IRecord> reverse(RealocMemory& a_memory, IOutputStream& a_stream) final
	{
		return std::make_shared<TRecordRemoved<Key>>(this->getContainer(a_memory), m_objectKey, m_pObject);
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
			m_RemovedObjectUID = pObj->uid();
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
				pObj = a_memory.realoc(m_RemovedObjectUID);

			if (pObj)
			{
				pContainer->record_insert(m_objectKey, pObj);
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

	std::shared_ptr<IRecord> reverse(RealocMemory& a_memory, IOutputStream& a_stream) final
	{
		return std::make_shared<TRecordInsert<Key>>(this->getContainer(a_memory), m_objectKey, m_RemovedObjectUID);
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
		auto pContainer = this->getContainer(a_memory);
		if (pContainer)
		{
			auto pObj = m_pOldObject.lock();
			if (!pObj)
				pObj = a_memory.realoc(m_oldObjectUID, std::weak_ptr<RTTIDefinition>());

			if (pObj)
			{
				pContainer->record_replace(m_objectKey, pObj);
				m_pOldObject = pObj;
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


	std::shared_ptr<IRecord> reverse(RealocMemory& a_memory, IOutputStream& a_stream) final
	{
		return std::make_shared<TRecordChanged<Key>>(this->getContainer(a_memory), m_objectKey, m_pOldObject, m_pNewObject);
	}
};
