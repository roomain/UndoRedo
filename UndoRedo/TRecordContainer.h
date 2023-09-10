#pragma once
#include <memory>
#include "Record.h"
#include "RealocMemory.h"
#include "TIContainer.h"
#include "UndoRedoException.h"

class RTTIDefinition;

template<typename Key>
class ContainerRecordProxy
{
private:
	ObjectUID m_containerObjectUID;
	TContainerProxy<Key> m_containerProxy;
	std::weak_ptr<RTTIDefinition> m_pObjectDef;

public:
	ContainerRecordProxy(const TContainerProxy<Key>& a_pContainer) : m_containerProxy{ a_pContainer }
	{
		if (m_containerProxy.valid())
		{
			m_pObjectDef = m_containerProxy->isA();
			m_containerObjectUID = m_containerProxy->uid();
		}
		else
		{
			UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_Deleted)
		}
	}

	ContainerRecordProxy(const ContainerRecordProxy<Key>& a_other) : m_containerProxy{ a_other.m_containerProxy }
	{
		if (m_containerProxy.valid())
		{
			m_pObjectDef = m_containerProxy->isA();
			m_containerObjectUID = m_containerProxy->uid();
		}
		else
		{
			UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_Deleted)
		}
	}

	virtual ~ContainerRecordProxy() = default;

	bool containerValid()const
	{
		return m_containerProxy.valid();
	}

	TIContainer<Key>* getContainer(RealocMemory& a_memory)
	{
		auto pObj = m_containerProxy.pointer();
		if (nullptr == pObj && m_containerProxy.isShared())
		{
			m_containerProxy = MStatic_pointer_cast<TIContainer<Key>>(a_memory.realoc(m_containerObjectUID, m_pObjectDef));
			pObj = m_containerProxy.pointer();
		}

		return pObj;
	}
};


template<typename Key>
class TRecordInsert : public IRecord
{
private:
	Key m_objectKey;
	ObjectUID m_newObjectUID;
	std::weak_ptr<IRecordObject> m_pObject;
	ContainerRecordProxy<Key> m_proxy;

public:
	TRecordInsert(const TContainerProxy<Key>& a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pNewObject) :
		m_proxy(a_pContainer), m_objectKey{ a_key }, m_pObject{ a_pNewObject }
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

	TRecordInsert(const TContainerProxy<Key>& a_pContainer, const Key& a_key, const ObjectUID& a_objUID) :
		m_proxy(a_pContainer), m_objectKey{ a_key }, m_newObjectUID{ a_objUID }
	{
		//
	}

	TRecordInsert(const ContainerRecordProxy<Key>& a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pNewObject) :
		m_proxy(a_pContainer), m_objectKey{ a_key }, m_pObject{ a_pNewObject }
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

	TRecordInsert(const ContainerRecordProxy<Key>& a_pContainer, const Key& a_key, const ObjectUID& a_objUID) :
		m_proxy(a_pContainer), m_objectKey{ a_key }, m_newObjectUID{ a_objUID }
	{
		//
	}

	virtual ~TRecordInsert() = default;

	void process(IInputStream& a_stream, RealocMemory& a_memory) final
	{	
		auto pContainer = m_proxy.getContainer(a_memory);
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
		return std::make_shared<TRecordRemoved<Key>>(m_proxy, m_objectKey, m_pObject);
	}

};

template<typename Key>
class TRecordRemoved : public IRecord
{
private:
	Key m_objectKey;
	ObjectUID m_RemovedObjectUID;
	std::weak_ptr<IRecordObject> m_pRemovedObject;
	ContainerRecordProxy<Key> m_proxy;

public:
	TRecordRemoved(const TContainerProxy<Key>& a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pRemovedObject) :
		m_proxy(a_pContainer), m_objectKey{ a_key }, m_pRemovedObject{ a_pRemovedObject }
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

	TRecordRemoved(const ContainerRecordProxy<Key>& a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pRemovedObject) :
		m_proxy(a_pContainer), m_objectKey{ a_key }, m_pRemovedObject{ a_pRemovedObject }
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
		auto pContainer = m_proxy.getContainer(a_memory);
		if (pContainer)
		{
			MShared_ptr<IRecordObject> pObj = m_pRemovedObject.lock();
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
		return std::make_shared<TRecordInsert<Key>>(m_proxy, m_objectKey, m_RemovedObjectUID);
	}
};

template<typename Key>
class TRecordChanged : public IRecord
{
private:
	Key m_objectKey;
	ObjectUID m_oldObjectUID;
	ObjectUID m_newObjectUID;
	std::weak_ptr<IRecordObject> m_pNewObject;
	std::weak_ptr<IRecordObject> m_pOldObject;
	ContainerRecordProxy<Key> m_proxy;

public:
	TRecordChanged(const TContainerProxy<Key>& a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pNewObject,
		const std::weak_ptr<IRecordObject>& a_pOldObject) :
		m_proxy(a_pContainer), m_objectKey{ a_key }, m_pNewObject{ a_pNewObject }, m_pOldObject{ a_pOldObject }
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

	TRecordChanged(const ContainerRecordProxy<Key>& a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pNewObject,
		const std::weak_ptr<IRecordObject>& a_pOldObject) :
		m_proxy(a_pContainer), m_objectKey{ a_key }, m_pNewObject{ a_pNewObject }, m_pOldObject{ a_pOldObject }
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
		auto pContainer = m_proxy.getContainer(a_memory);
		if (pContainer)
		{
			MShared_ptr<IRecordObject> pObj = m_pOldObject.lock();
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
		return std::make_shared<TRecordChanged<Key>>(m_proxy, m_objectKey, m_pOldObject, m_pNewObject);
	}
};
