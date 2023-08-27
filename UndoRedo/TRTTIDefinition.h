#pragma once
#include "MMemory.h"
#include "RTTIDefinition.h"

/*@brief template RTTI definition*/
template<typename DefinedClass>
class TRTTIDefinition : public RTTIDefinition
{
public:
	TRTTIDefinition() = delete;
	TRTTIDefinition(const std::string_view& a_name, const unsigned short a_version) :
		RTTIDefinition(a_name, a_version) {}
	TRTTIDefinition(const std::string_view& a_name, const unsigned short a_version,
		const std::vector<std::shared_ptr<RTTIDefinition>>& a_vParents) :
		RTTIDefinition(a_name, a_version, a_vParents) {}

	virtual ~TRTTIDefinition() = default;

	/*@brief size of described class*/
	[[nodiscard]] virtual size_t classSize()const noexcept override { return sizeof(DefinedClass); }

	/*@brief create class*/
	[[nodiscard]] MShared_ptr<IRecordObject> create(const ObjectUID& a_objectUID)const final
	{
		if constexpr(std::is_base_of_v<IRecordObject, DefinedClass>  && !std::is_abstract_v<DefinedClass>)
		{
			IRecordObject::enableUIDGenerator(false);
			MShared_ptr<DefinedClass> ptrObj = make_MShared<DefinedClass>();
			ptrObj->setUID(a_objectUID);
			IRecordObject::enableUIDGenerator(true);
			//RTTIDefinition::registerToDocument(ptrObj);
			return ptrObj;
		}
		return nullptr;
	}

	/*@brief create class*/
	[[nodiscard]] MShared_ptr<DefinedClass> create()const
	{
		if constexpr (std::is_base_of_v<IRecordObject, DefinedClass> && !std::is_abstract_v<DefinedClass>)
		{
			auto ptrObj = make_MShared<DefinedClass>();
			//RTTIDefinition::registerToDocument(ptrObj);
			return ptrObj;
		}
		return nullptr;
	}
};


template<typename DefinedClass>
using TRTTIDefinitionPtr = std::shared_ptr<TRTTIDefinition<DefinedClass>>;