#pragma once
#include "IRecordObject.h"
#include "Reference.h"
#include <variant>

template<typename Key>
class TContainerProxy;

template<typename Key>
class TRecordInsert;

template<typename Key>
class TRecordRemoved;


template<typename Key>
class TRecordChanged;

template<typename Key, typename Type>
class TIContainer : public IRecordObject, public Referenced<TIContainer<Key, Type>>
{

	friend class TRecordInsert<Key>;
	friend class TRecordRemoved<Key>;
	friend class TRecordChanged<Key>;

protected:
    bool m_bActiveCallback = true;

	//virtual MShared_ptr<Contained> at(const Key& a_key) = 0;
	virtual void record_replace(const size_t& a_key, const IRecordObjectPtr& a_object) = 0;
	virtual void record_insert(const Key& a_key, const IRecordObjectPtr& a_value) = 0;
	virtual void record_eraseAt(const Key& a_key) = 0;


    void assert_ItemAdd(const MShared_ptr<Type>& a_pItem, const size_t a_index)
    {
        if (UndoRedo::instance().sessionStarted())
        {
            RecordSession& curSession = UndoRedo::instance().currentSession();
            if (isShared())
            {
                //
            }
            else
            {
                //
            }
            std::weak_ptr<TIContainer<size_t>> ptr = std::dynamic_pointer_cast<TIContainer<size_t>>(shared_from_this());
            curSession.addRecord(std::make_shared<TRecordInsert<size_t>>(ptr, a_index, a_pItem));
        }
    }

    void assert_ItemRemoved(const MShared_ptr<Type>& a_pItem, const size_t a_index)
    {
        if (UndoRedo::instance().sessionStarted())
        {
            RecordSession& curSession = UndoRedo::instance().currentSession();
            if (isShared())
            {
                //
            }
            else
            {
                //
            }
            std::weak_ptr<TIContainer<size_t>> ptr = std::dynamic_pointer_cast<TIContainer<size_t>>(shared_from_this());
            curSession.addRecord(std::make_shared<TRecordRemoved<size_t>>(ptr, a_index, a_pItem));
        }
    }

    void assert_ItemChanged(const ContainerCell<Type>* a_pItem, const MShared_ptr<Type>& a_pBefore, const MShared_ptr<Type>& a_pAfter)
    {        
        if (m_bActiveCallback && UndoRedo::instance().sessionStarted())
        {
            RecordSession& curSession = UndoRedo::instance().currentSession();
            if (isShared())
            {
                //
            }
            else
            {
                //
            }
            const size_t index = a_pItem - VectorBase::data();
            std::weak_ptr<TIContainer<size_t>> ptr = std::dynamic_pointer_cast<TIContainer<size_t>>(shared_from_this());
            curSession.addRecord(std::make_shared<TRecordChanged<size_t>>(ptr, index, a_pAfter, a_pBefore));
        }
    }

};

template<typename Key>
class TContainerProxy
{
    using ContainerProxy = std::variant<std::weak_ptr< TIContainer<Key>>, Reference<  TIContainer<Key>>>;

private:
    ContainerProxy m_proxy;

public:
    TContainerProxy(TIContainer<Key>* const a_ptr)
    {
        if (a_ptr->isShared())
        {
            m_proxy = a_ptr->weak_from_this();
        }
        else
        {
            m_proxy = make_ref(*a_ptr);
        }
    }

    bool valid()const
    {
        // TODO
    }

    TIContainer<Key>* operator ->()
    {
        // TODO
    }
};