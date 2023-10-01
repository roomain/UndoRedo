#pragma once
#include <variant>
#include "IRecordObject.h"
#include "Reference.h"
#include "UndoRedo.h"
#include "RecordSession.h"
#include "InputBinStream.h"
#include "Reference.h"
#include "ContainerCell.h"
#include "TRecordObjectProxy.h"

template<typename Key>
class TContainerProxy;

template<typename Key>
class TRecordInsert;

template<typename Key>
class TRecordRemoved;


template<typename Key>
class TRecordChanged;

template<typename Key>
class TIContainer : public MShared_from_this<TIContainer<Key>>, public RefObject
{
    //DECLARE_RTTI_DERIVED(1, TIContainer<Key>, IRecordObject)
	friend class TRecordInsert<Key>;
	friend class TRecordRemoved<Key>;
	friend class TRecordChanged<Key>;

protected:

	virtual void record_replace(const Key& a_key, const IRecordObjectPtr& a_object) = 0;
	virtual void record_insert(const Key& a_key, const IRecordObjectPtr& a_value) = 0;
	virtual void record_eraseAt(const Key& a_key) = 0;


    template<typename Type>
    void assert_ItemAdd(const MShared_ptr<Type>& a_pItem, const Key& a_index)
    {
        if (UndoRedo::instance().sessionStarted())
        {
            RecordSession& curSession = UndoRedo::instance().currentSession();
            curSession.addRecord(std::make_shared<TRecordInsert<Key>>(TRecordObjectProxy<TIContainer<Key>>(this), a_index, a_pItem));
        }
    }

    template<typename Type>
    void assert_ItemRemoved(const MShared_ptr<Type>& a_pItem, const size_t& a_index)
    {
        if (UndoRedo::instance().sessionStarted())
        {
            RecordSession& curSession = UndoRedo::instance().currentSession();
            curSession.addRecord(std::make_shared<TRecordRemoved<Key>>(TRecordObjectProxy<TIContainer<Key>>(this), a_index, a_pItem));
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


