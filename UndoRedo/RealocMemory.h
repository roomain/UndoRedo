#pragma once
#include <unordered_map>
#include "MMemory.h"
#include "Defines.h"
#include "RTTIDefinition.h"
#include "UndoRedo_globals.h"

#pragma warning(push)
#pragma warning(disable : 4251)
using RealocMap = std::unordered_map<ObjectUID, MShared_ptr<IRecordObject>>;
using RealocWMap = std::unordered_map<ObjectUID, std::weak_ptr<IRecordObject>>;

/*@brief realocation memory*/
class UNDO_REDO_EXPORT RealocMemory
{
private:
	RealocWMap m_undoRedoRealoc;	/*!< realocated pointer for all sessions*/
	RealocMap m_sessionRealoc;		/*!< realocated pointer for session*/
	
public:
	RealocMemory() = default;
	MShared_ptr<IRecordObject> realoc(const ObjectUID& a_uid);
	MShared_ptr<IRecordObject> realoc(const ObjectUID& a_uid, const std::weak_ptr<RTTIDefinition>& a_objectDef);
	void endSession();
};
#pragma warning(pop)