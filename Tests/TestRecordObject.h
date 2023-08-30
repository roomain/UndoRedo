#pragma once
#include <string>
#include "IRecordObject.h"
#include "RTTIDefinition_macros.h"
#include "IInputStream.h"
#include "IOutputStream.h"

class TestRecordObject : public IRecordObject
{
	DECLARE_RTTI_DERIVED(1, TestRecordObject, IRecordObject)

private:
	int m_iValue = 1;
	float m_fValue = 2.5f;
	std::string m_sValue;

public:
	TestRecordObject();
	~TestRecordObject() = default;
	void setValue(const int a_val);
	void setValue(const float a_val);
	void setValue(const std::string& a_val);
	int iVal()const noexcept;
	float fVal()const noexcept;
	std::string sVal()const noexcept;
	unsigned short load(IInputStream& a_stream) final;
	bool save(IOutputStream& a_stream)const final;
};
