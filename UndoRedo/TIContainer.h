#pragma once
#include "IRecordObject.h"

template<typename Key>
class TRecordInsert;

template<typename Key>
class TRecordRemoved;


template<typename Key>
class TRecordChanged;

template<typename Key>
class TIContainer : public IRecordObject
{
	friend class TRecordInsert<Key>;
	friend class TRecordRemoved<Key>;
	friend class TRecordChanged<Key>;

protected:
	//virtual MShared_ptr<Contained> at(const Key& a_key) = 0;
	virtual void record_insert(const Key& a_key, const IRecordObjectPtr& a_value) = 0;
	virtual void record_eraseAt(const Key& a_key) = 0;
};