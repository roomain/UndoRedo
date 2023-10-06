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
class TRecordEmptyRemoved;

template<typename Key>
class TRecordEmptyChangedRev;

template<typename Key>
class TRecordEmptyCreate;

template<typename Key>
class TRecordChanged;

template<typename Key>
class TRecordEmptyChanged;

template<typename Key>
class TIContainer : public MShared_from_this<TIContainer<Key>>, public RefObject
{
    //DECLARE_RTTI_DERIVED(1, TIContainer<Key>, IRecordObject)
	friend class TRecordInsert<Key>;
	friend class TRecordRemoved<Key>;
	friend class TRecordChanged<Key>;

    friend class TRecordEmptyChangedRev<Key>;
    friend class TRecordEmptyRemoved<Key>;
    friend class TRecordEmptyChanged<Key>;
    friend class TRecordEmptyCreate<Key>;

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
            if (a_pItem)
            {
                curSession.addRecord(std::make_shared<TRecordInsert<Key>>(TRecordObjectProxy<TIContainer<Key>>(this), a_index, a_pItem));
            }
            else
            {
                curSession.addRecord(std::make_shared<TRecordEmptyCreate<Key>>(TRecordObjectProxy<TIContainer<Key>>(this), a_index));
            }
        }
    }

    template<typename Type>
    void assert_ItemRemoved(const MShared_ptr<Type>& a_pItem, const Key& a_index)
    {
        if (UndoRedo::instance().sessionStarted())
        {
            RecordSession& curSession = UndoRedo::instance().currentSession();
            if (a_pItem)
            {
                curSession.addRecord(std::make_shared<TRecordRemoved<Key>>(TRecordObjectProxy<TIContainer<Key>>(this), a_index, a_pItem));
            }
            else
            {
                curSession.addRecord(std::make_shared<TRecordEmptyRemoved<Key>>(TRecordObjectProxy<TIContainer<Key>>(this), a_index));
            }
        }
    }
};


