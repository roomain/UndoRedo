#pragma once
#include "IRecordObject.h"


template<typename Key, typename Contained>
class TIContainer : public IRecordObject
{
public:
	virtual MShared_ptr<Contained> at(const Key& a_key) = 0;
	virtual void insert(const Key& a_key, const MShared_ptr<Contained>& a_value) = 0;
	virtual void eraseAt(const Key& a_key) = 0;
};