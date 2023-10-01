#pragma once
#include <algorithm>
#include <map>
#include "MMemory.h"
#include "TIContainer.h"
#include "TRecordContainer.h"
#include "RTTIDefinition_macros.h"

template<typename Key, typename Type,
    class Compare = std::less<Key>> requires std::is_base_of_v<IRecordObject, Type>
class MMap : public TIContainer<Key>, private std::map<Key,ContainerCell<Type>, Compare>
{
    //DECLARE_RTTI_DERIVED(1, MMap<Key, Type>, IRecordObject)
private:
    using MapBase = std::map<Key, ContainerCell<Type>>;
    ChangeAssertion<Type> m_itemCallback;
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

    void assert_ItemChanged(const ContainerCell<Type>* a_pItem, const MShared_ptr<Type>& a_pBefore, const MShared_ptr<Type>& a_pAfter)
    {
        if (m_bActiveCallback && UndoRedo::instance().sessionStarted())
        {
            RecordSession& curSession = UndoRedo::instance().currentSession();
            curSession.addRecord(std::make_shared<TRecordChanged<Key>>(TRecordObjectProxy<TIContainer<Key>>(this), a_pItem - MapBase::data(), a_pAfter, a_pBefore));
        }
    }

    template< class... Args >
    ContainerCell<Type> createWithoutRecord(Args&&... args)
    {
        TScoped<bool> scoped = UndoRedo::instance().scopedActivation();
        scoped = false;
        return ContainerCell<Type>(args...);
    }

public:
    MMap()
    {
        // TODO
    }


    using iterator = std::map < Key, ContainerCell<Type>>::iterator;
    using const_iterator = std::map < Key, ContainerCell<Type>>::const_iterator;

    template< class... Args >
    std::pair<iterator, bool> try_emplace(const Key& k, Args&&... args)
    {
        return MapBase::try_emplace(k, createWithoutRecord(args...));
    }

    template< class... Args >
    std::pair<iterator, bool> try_emplace(Key&& k, Args&&... args)
    {
        return MapBase::try_emplace(k, createWithoutRecord(args...));
    }

    template< class... Args >
    iterator try_emplace(const_iterator hint, const Key& k, Args&&... args)
    {
        return MapBase::try_emplace(hint, k, createWithoutRecord(args...));
    }

    template< class... Args >
    iterator try_emplace(const_iterator hint, Key&& k, Args&&... args)
    {
        return MapBase::try_emplace(hint, k, createWithoutRecord(args...));
    }

    ContainerCell<Type>& operator[](const Key& a_index)
    {
        std::pair<iterator, bool> ret = try_emplace(a_index, createWithoutRecord());
        if (ret.second)
        {
            // item empty
            TIContainer<Key>::assert_ItemAdd(ret.first->second, a_index);
        }
        return ret.first->second;
    }
   
    using std::map<Key, ContainerCell<Type>>::at;
    using std::map<Key, ContainerCell<Type>>::begin;
    using std::map<Key, ContainerCell<Type>>::end;
    using std::map<Key, ContainerCell<Type>>::cbegin;
    using std::map<Key, ContainerCell<Type>>::cend;
    using std::map<Key, ContainerCell<Type>>::rbegin;
    using std::map<Key, ContainerCell<Type>>::rend;
    using std::map<Key, ContainerCell<Type>>::crbegin;
    using std::map<Key, ContainerCell<Type>>::crend;

    using std::map<Key, ContainerCell<Type>>::size;
    using std::map<Key, ContainerCell<Type>>::max_size;
    using std::map<Key, ContainerCell<Type>>::empty;
};

