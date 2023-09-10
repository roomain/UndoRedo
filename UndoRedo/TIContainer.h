#pragma once
#include <variant>
#include "IRecordObject.h"
#include "Reference.h"
#include "UndoRedo.h"
#include "RecordSession.h"
#include "InputBinStream.h"
#include "Reference.h"
#include "ContainerCell.h"

template<typename Key>
class TContainerProxy;

template<typename Key>
class TRecordInsert;

template<typename Key>
class TRecordRemoved;


template<typename Key>
class TRecordChanged;

template<typename Key>
class TIContainer : public IRecordObject
{
    DECLARE_RTTI_DERIVED(1, TIContainer<Key>, IRecordObject)
	friend class TRecordInsert<Key>;
	friend class TRecordRemoved<Key>;
	friend class TRecordChanged<Key>;

protected:

	virtual void record_replace(const size_t& a_key, const IRecordObjectPtr& a_object) = 0;
	virtual void record_insert(const Key& a_key, const IRecordObjectPtr& a_value) = 0;
	virtual void record_eraseAt(const Key& a_key) = 0;


    template<typename Type>
    void assert_ItemAdd(const MShared_ptr<Type>& a_pItem, const Key& a_index)
    {
        if (UndoRedo::instance().sessionStarted())
        {
            RecordSession& curSession = UndoRedo::instance().currentSession();
            curSession.addRecord(std::make_shared<TRecordInsert<Key>>(TContainerProxy<Key>(this), a_index, a_pItem));
        }
    }

    template<typename Type>
    void assert_ItemRemoved(const MShared_ptr<Type>& a_pItem, const size_t& a_index)
    {
        if (UndoRedo::instance().sessionStarted())
        {
            RecordSession& curSession = UndoRedo::instance().currentSession();
            curSession.addRecord(std::make_shared<TRecordRemoved<Key>>(TContainerProxy<size_t>(this), a_index, a_pItem));
        }
    }

    //void assert_ItemChanged(const ContainerCell<Type>* a_pItem, const MShared_ptr<Type>& a_pBefore, const MShared_ptr<Type>& a_pAfter)
    //{
    //    if (m_bActiveCallback && UndoRedo::instance().sessionStarted())
    //    {
    //        RecordSession& curSession = UndoRedo::instance().currentSession();
    //        curSession.addRecord(std::make_shared<TRecordChanged<Key>>(TContainerProxy<Key>(this), a_pItem - VectorBase::data(), a_pAfter, a_pBefore));
    //    }
    //}


};

template<typename Key>
class TContainerProxy
{
    using ContainerProxy = std::variant<std::weak_ptr<TIContainer<Key>>, Ref<TIContainer<Key>>>;

private:
    ContainerProxy m_proxy;
    enum class AcessType : int
    {
        Weak_ptr = 0,
        Reference = 1
    }m_accessType;

public:
   
    TContainerProxy(TIContainer<Key>* const a_ptr)
    {
        if (a_ptr->isShared())
        {
            m_proxy = std::dynamic_pointer_cast<TIContainer<Key>>(a_ptr->shared_from_this());
            m_accessType = AcessType::Weak_ptr;
        }
        else
        {
            m_proxy = make_ref<TIContainer<Key>>(*a_ptr);
            m_accessType = AcessType::Reference;
        }
    }

    explicit TContainerProxy(const TContainerProxy<Key>& a_other) : m_proxy{a_other.m_proxy }, m_accessType{ a_other.m_accessType }
    {

    }

    TContainerProxy(TContainerProxy<Key>&& a_other)noexcept : m_proxy{ std::move(a_other.m_proxy) }, m_accessType{ a_other.m_accessType }
    {

    }

    bool valid()const
    {
        bool bRet = false;
        switch (m_accessType)
        {
        case AcessType::Weak_ptr:
            bRet = !std::get<std::weak_ptr<TIContainer<Key>>>(m_proxy).expired();
            break;
        case AcessType::Reference:
            bRet = std::get<Ref<TIContainer<Key>>>(m_proxy).valid();
            break;
        default:
            break;
        }
        return bRet;
    }

    TIContainer<Key>*const  operator ->()
    {
        switch (m_accessType)
        {
        case AcessType::Weak_ptr:
            return std::get<std::weak_ptr<TIContainer<Key>>>(m_proxy).lock().get();
        case AcessType::Reference:
            return std::get<Ref<TIContainer<Key>>>(m_proxy).operator->();
        default:
            break;
        }
        return nullptr;
    }

    const TIContainer<Key>* operator ->()const
    {
        switch (m_accessType)
        {
        case AcessType::Weak_ptr:
            return std::get<std::weak_ptr<TIContainer<Key>>>(m_proxy).lock().get();
        case AcessType::Reference:
            return std::get<Ref<TIContainer<Key>>>(m_proxy).pointer();
        default:
            break;
        }
        return nullptr;
    }

    TIContainer<Key>*const pointer()
    {
        switch (m_accessType)
        {
        case AcessType::Weak_ptr:
            return std::get<std::weak_ptr<TIContainer<Key>>>(m_proxy).lock().get();
        case AcessType::Reference:
            return std::get<Ref<TIContainer<Key>>>(m_proxy).pointer();
        default:
            break;
        }
        return nullptr;
    }

    bool isRef()const
    {
        return m_accessType == AcessType::Reference;
    }

    bool isShared()const
    {
        return m_accessType == AcessType::Weak_ptr;
    }

    void operator = (const std::weak_ptr<TIContainer<Key>>& a_pWeak)
    {
        m_proxy = a_pWeak;
        m_accessType = AcessType::Weak_ptr;
    }
};