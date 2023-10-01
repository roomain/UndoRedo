#pragma once
#include <algorithm>
#include <map>
#include "MMemory.h"
#include "TIContainer.h"
#include "TRecordContainer.h"
#include "RTTIDefinition_macros.h"

template<typename Key, typename Type,
    class Compare = std::less<Key>> requires std::is_base_of_v<IRecordObject, Type>
class MMap : public TIContainer<Key>, private std::map<Key, ContainerKeyCell<Key,Type>, Compare>
{
    //DECLARE_RTTI_DERIVED(1, MMap<Key, Type>, IRecordObject)
private:
    using MapBase = std::map<Key, ContainerKeyCell<Key, Type>, Compare>;
    ChangeAssertionKey<Key, Type> m_itemCallback;
    bool m_bActiveCallback = true;

protected:
    void record_replace(const Key& a_key, const IRecordObjectPtr& a_object)final
    {
        MapBase::operator[](a_key) = MStatic_pointer_cast<Type>(a_object);
    }

    void record_insert(const Key& a_key, const IRecordObjectPtr& a_object)final
    {
        MapBase::operator[](a_key) = MStatic_pointer_cast<Type>(a_object);
    }

    void record_eraseAt(const Key& a_index) final
    {
        MapBase::erase(a_index);
    }

    void assert_EmptyCreate(const Key& a_index)
    {
        if (UndoRedo::instance().sessionStarted())
        {
            RecordSession& curSession = UndoRedo::instance().currentSession();
            curSession.addRecord(std::make_shared<TRecordEmptyCreate<Key>>(TRecordObjectProxy<TIContainer<Key>>(this), a_index));
        }
    }

    void assert_ItemChanged(const ContainerKeyCell<Key, Type>* a_pItem, const MShared_ptr<Type>& a_pBefore, const MShared_ptr<Type>& a_pAfter)
    {
        if (m_bActiveCallback && UndoRedo::instance().sessionStarted())
        {
            RecordSession& curSession = UndoRedo::instance().currentSession();   
            if (a_pBefore)
            {
                curSession.addRecord(std::make_shared<TRecordChanged<Key>>(TRecordObjectProxy<TIContainer<Key>>(this),
                    a_pItem->key(),
                    a_pAfter, a_pBefore));
            }
            else
            {
                curSession.addRecord(std::make_shared<TRecordEmptyChanged<Key>>(TRecordObjectProxy<TIContainer<Key>>(this),
                    a_pItem->key(),
                    a_pAfter));
            }
        }
    }

    template< class... Args >
    ContainerKeyCell<Key, Type> createWithoutRecord(const Key& a_key, Args&&... args)
    {
        TScoped<bool> scoped = UndoRedo::instance().scopedActivation();
        scoped = false;
        return ContainerKeyCell<Key, Type>(a_key, m_itemCallback, args...);
    }

public:
    MMap()
    {
        m_itemCallback = std::bind_front(&MMap<Key, Type>::assert_ItemChanged, this);
    }


    using iterator = std::map <Key, ContainerKeyCell<Key, Type>, Compare>::iterator;
    using const_iterator = std::map < Key, ContainerKeyCell<Key, Type>, Compare>::const_iterator;

    /*template< class... Args >
    std::pair<iterator, bool> try_emplace(const Key& a_key, Args&&... args)
    {
        return MapBase::try_emplace(a_key, createWithoutRecord(a_key,args...));
    }

    template< class... Args >
    std::pair<iterator, bool> try_emplace(Key&& a_key, Args&&... args)
    {
        return MapBase::try_emplace(a_key, createWithoutRecord(a_key, args...));
    }

    template< class... Args >
    iterator try_emplace(const_iterator hint, const Key& a_key, Args&&... args)
    {
        return MapBase::try_emplace(hint, a_key, createWithoutRecord(a_key, args...));
    }

    template< class... Args >
    iterator try_emplace(const_iterator hint, Key&& a_key, Args&&... args)
    {
        return MapBase::try_emplace(hint, a_key, createWithoutRecord(a_key, args...));
    }*/

    ContainerKeyCell<Key, Type>& operator[](const Key& a_key)
    {
        std::pair<iterator, bool> ret = MapBase::try_emplace(a_key, a_key, m_itemCallback);
        if (ret.second)
        {
            // item empty
            this->assert_EmptyCreate(a_key);
        }
        return ret.first->second;
    }
   
    using std::map<Key, ContainerKeyCell<Key, Type>, Compare>::at;
    using std::map<Key, ContainerKeyCell<Key, Type>, Compare>::begin;
    using std::map<Key, ContainerKeyCell<Key, Type>, Compare>::end;
    using std::map<Key, ContainerKeyCell<Key, Type>, Compare>::cbegin;
    using std::map<Key, ContainerKeyCell<Key, Type>, Compare>::cend;
    using std::map<Key, ContainerKeyCell<Key, Type>, Compare>::rbegin;
    using std::map<Key, ContainerKeyCell<Key, Type>, Compare>::rend;
    using std::map<Key, ContainerKeyCell<Key, Type>, Compare>::crbegin;
    using std::map<Key, ContainerKeyCell<Key, Type>, Compare>::crend;

    using std::map<Key, ContainerKeyCell<Key, Type>, Compare>::size;
    using std::map<Key, ContainerKeyCell<Key, Type>, Compare>::max_size;
    using std::map<Key, ContainerKeyCell<Key, Type>, Compare>::empty;
};

