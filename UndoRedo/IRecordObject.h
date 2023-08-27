#pragma once
#include <atomic>
#include "MMemory.h"
#include "IOutputStream.h"
#include "IInputStream.h"
#include "MShared_ptr.h"
#include "RTTIDefinition_macros.h"
#include "UndoRedo_globals.h"

#pragma warning(push)
#pragma warning(disable : 4275)
#pragma warning(disable : 4251)

/*@brief interface for recordable object*/
class UNDO_REDO_EXPORT IRecordObject : public MShared_from_this<IRecordObject>
{
	DECLARE_RTTI(1, IRecordObject)
private:
	ObjectUID m_objectUid;	/*!< object unique identifier*/

protected:
	static std::atomic_bool m_sEnableUIDGen;	/*!< enable identifier generator*/
	static std::atomic_ullong m_UIDGen;			/*!< unique identifier generator*/

	/*@brief is uid generator enabled*/
	static bool isUIDGeneratorEnabled() { return m_sEnableUIDGen; }
	/*@brief enabling uid genrator*/
	static void enableUIDGenerator(bool a_bEnable) { m_sEnableUIDGen = a_bEnable; }

	void setUID(const ObjectUID& a_uid) { m_objectUid = a_uid; }

public:
	IRecordObject();
	IRecordObject(const IRecordObject& a_other);
	IRecordObject(IRecordObject&& a_other)noexcept;
	IRecordObject& operator = (IRecordObject&&) = delete;
	[[nodiscard]] constexpr ObjectUID uid()const noexcept { return m_objectUid; }
	virtual void load(IInputStream& a_stream) = 0;
	virtual void save(IOutputStream& a_stream) = 0;
};

using IRecordObjectPtr = MShared_ptr<IRecordObject>;

#pragma warning(pop)