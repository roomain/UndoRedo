#pragma once
#include "IOutputStream.h"
#include "IInputStream.h"
#include "MShared_ptr.h"
/*@brief interface for recordable object*/
class IRecordObject
{
	friend MShared_ptr<IRecordObject>;
protected:
	void assertDelete();
public:
	virtual void load(IInputStream& a_stream) = 0;
	virtual void save(IOutputStream& a_stream) = 0;
};