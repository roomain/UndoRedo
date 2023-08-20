#pragma once
#include <unordered_map>
#include "MMemory.h"
#include "Defines.h"
#include "RTTIDefinition.h"

using RealocMap = std::unordered_map<ObjectUID, MShared_ptr<IRecordObject>>;

/*@brief realocation memory*/
class RealocMemory
{
private:
	RealocMap m_undoRedoRealoc;	/*!< realocated pointer for all sessions*/
	RealocMap m_sessionRealoc;	/*!< realocated pointer for session*/
	
public:
	RealocMemory() = default;
	MShared_ptr<IRecordObject> realoc(const ObjectUID& a_uid, std::weak_ptr<RTTIDefinition>& a_objectDef);
	void endSession();
};