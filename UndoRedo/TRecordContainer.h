#pragma once
#include <memory>
#include "Record.h"
#include "RealocMemory.h"
#include "TIContainer.h"
#include "UndoRedoException.h"

class RTTIDefinition;

template<typename Key>
using TContainerRecordProxy = TRecordObjectProxy<TIContainer<Key>>;

template<typename Key>
class TRecordInsert : public IRecord
{
private:
	Key m_objectKey;
	ObjectUID m_newObjectUID;
	std::weak_ptr<IRecordObject> m_pObject;
	TContainerRecordProxy<Key> m_proxy;

public:
	TRecordInsert(const TIContainer<Key>* a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pNewObject) :
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

	TRecordInsert(const TIContainer<Key>* a_pContainer, const Key& a_key, const ObjectUID& a_objUID) :
		m_proxy(a_pContainer), m_objectKey{ a_key }, m_newObjectUID{ a_objUID }
	{
		//
	}

	TRecordInsert(const TContainerRecordProxy<Key>& a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pNewObject) :
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

	TRecordInsert(const TContainerRecordProxy<Key>& a_pContainer, const Key& a_key, const ObjectUID& a_objUID) :
		m_proxy(a_pContainer), m_objectKey{ a_key }, m_newObjectUID{ a_objUID }
	{
		//
	}

	virtual ~TRecordInsert() = default;

	void process(IInputStream& a_stream, RealocMemory& a_memory) final
	{	

		if (m_proxy.realocate(a_memory))
		{
			m_proxy->record_eraseAt(m_objectKey);
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
class TRecordEmptyCreate : public IRecord
{
private:
	Key m_objectKey;
	TContainerRecordProxy<Key> m_proxy;

public:
	TRecordEmptyCreate(const TIContainer<Key>* a_pContainer, const Key& a_key) :
		m_proxy(a_pContainer), m_objectKey{ a_key }
	{
	}

	TRecordEmptyCreate(const TContainerRecordProxy<Key>& a_pContainer, const Key& a_key) :
		m_proxy(a_pContainer), m_objectKey{ a_key }
	{
	}


	//bool hasReverse()const noexcept final{ return false; }

	virtual ~TRecordEmptyCreate() = default;

	void process(IInputStream& a_stream, RealocMemory& a_memory) final
	{

		if (m_proxy.realocate(a_memory))
		{
			m_proxy->record_eraseAt(m_objectKey);
		}
		else
		{
			UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_Deleted)
		}
	}

	std::shared_ptr<IRecord> reverse(RealocMemory& a_memory, IOutputStream& a_stream) final
	{
		return std::make_shared<TRecordEmptyRemoved<Key>>(m_proxy, m_objectKey);
	}

};


template<typename Key>
class TRecordRemoved : public IRecord
{
private:
	Key m_objectKey;
	ObjectUID m_RemovedObjectUID;
	std::weak_ptr<IRecordObject> m_pRemovedObject;
	TContainerRecordProxy<Key> m_proxy;

public:
	TRecordRemoved(const TIContainer<Key>* a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pRemovedObject) :
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

	TRecordRemoved(const TContainerRecordProxy<Key>& a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pRemovedObject) :
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
		if (m_proxy.realocate(a_memory))
		{
			MShared_ptr<IRecordObject> pObj = m_pRemovedObject.lock();
			if (!pObj)
				pObj = a_memory.realoc(m_RemovedObjectUID);

			if (pObj)
			{
				m_proxy->record_insert(m_objectKey, pObj);
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
class TRecordEmptyRemoved : public IRecord
{
private:
	Key m_objectKey;
	TContainerRecordProxy<Key> m_proxy;

public:
	TRecordEmptyRemoved(const TIContainer<Key>* a_pContainer, const Key& a_key) :
		m_proxy(a_pContainer), m_objectKey{ a_key }
	{
	}

	TRecordEmptyRemoved(const TContainerRecordProxy<Key>& a_pContainer, const Key& a_key) :
		m_proxy(a_pContainer), m_objectKey{ a_key }
	{
		
	}
	virtual ~TRecordEmptyRemoved() = default;
	//bool hasReverse()const noexcept final { return false; }

	void process(IInputStream& a_stream, RealocMemory& a_memory) final
	{
		if (m_proxy.realocate(a_memory))
		{
			m_proxy->record_insert(m_objectKey, nullptr);
		}
		else
		{
			UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_Deleted)
		}
	}

	std::shared_ptr<IRecord> reverse(RealocMemory& a_memory, IOutputStream& a_stream) final
	{
		return std::make_shared<TRecordEmptyCreate<Key>>(m_proxy, m_objectKey);
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
	TContainerRecordProxy<Key> m_proxy;

public:
	TRecordChanged(const TIContainer<Key>* a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pNewObject,
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

	TRecordChanged(const TContainerRecordProxy<Key>& a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pNewObject,
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
		if (m_proxy.realocate(a_memory))
		{
			MShared_ptr<IRecordObject> pObj = m_pOldObject.lock();
			if (!pObj)
				pObj = a_memory.realoc(m_oldObjectUID, std::weak_ptr<RTTIDefinition>());

			if (pObj)
			{
				m_proxy->record_replace(m_objectKey, pObj);
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

template<typename Key>
class TRecordEmptyChangedRev;

template<typename Key>
class TRecordEmptyChanged : public IRecord
{
private:
	Key m_objectKey;
	ObjectUID m_newObjectUID;
	std::weak_ptr<IRecordObject> m_pNewObject;
	TContainerRecordProxy<Key> m_proxy;

public:
	TRecordEmptyChanged(const TIContainer<Key>* a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pNewObject) :
		m_proxy(a_pContainer), m_objectKey{ a_key }, m_pNewObject{ a_pNewObject }
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
	}

	TRecordEmptyChanged(const TContainerRecordProxy<Key>& a_pContainer, const Key& a_key, const std::weak_ptr<IRecordObject>& a_pNewObject) :
		m_proxy(a_pContainer), m_objectKey{ a_key }, m_pNewObject{ a_pNewObject }
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

	}
	virtual ~TRecordEmptyChanged() = default;
	//bool hasReverse()const noexcept final { return false; }
	void process(IInputStream& a_stream, RealocMemory& a_memory) final
	{
		if (m_proxy.realocate(a_memory))
		{
			m_proxy->record_replace(m_objectKey, nullptr);
		}
		else
		{
			UNDO_REDO_TROW(UndoRedoException::ExceptionType::Except_Deleted)
		}
	}


	std::shared_ptr<IRecord> reverse(RealocMemory& a_memory, IOutputStream& a_stream) final
	{
		return std::make_shared<TRecordEmptyChangedRev<Key>>(m_proxy, m_objectKey, m_pNewObject, m_newObjectUID);
	}
};


template<typename Key>
class TRecordEmptyChangedRev : public IRecord
{
private:
	Key m_objectKey;
	ObjectUID m_oldObjectUID;
	std::weak_ptr<IRecordObject> m_pOldObject;
	TContainerRecordProxy<Key> m_proxy;

public:
	TRecordEmptyChangedRev(const TIContainer<Key>* a_pContainer, const Key& a_key, 
		const std::weak_ptr<IRecordObject>& a_pOldObject, const ObjectUID& oldObjectUID) :
		m_proxy(a_pContainer), m_objectKey{ a_key }, m_pOldObject{ a_pOldObject }, m_oldObjectUID{ oldObjectUID }
	{
	}
	TRecordEmptyChangedRev(const TContainerRecordProxy<Key> a_pContainer, const Key& a_key,
		const std::weak_ptr<IRecordObject>& a_pOldObject, const ObjectUID& oldObjectUID) :
		m_proxy(a_pContainer), m_objectKey{ a_key }, m_pOldObject{ a_pOldObject }, m_oldObjectUID{ oldObjectUID }
	{
	}
	
	virtual ~TRecordEmptyChangedRev() = default;
	bool hasReverse()const noexcept final { return false; }
	void process(IInputStream& a_stream, RealocMemory& a_memory) final
	{
		if (m_proxy.realocate(a_memory))
		{
			MShared_ptr<IRecordObject> pObj = m_pOldObject.lock();
			if (!pObj)
				pObj = a_memory.realoc(m_oldObjectUID, std::weak_ptr<RTTIDefinition>());

			if (pObj)
			{
				m_proxy->record_replace(m_objectKey, pObj);
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
		return nullptr;
		//return std::make_shared<TRecordRemoved<Key>>(m_proxy, m_objectKey, m_pNewObject);
	}
};
