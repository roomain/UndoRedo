#include "pch.h"
#include "TestRecordObject.h"
#include "RecordAssertions.h"

TestRecordObject::TestRecordObject() : IRecordObject()
{
	m_iValue = 1;
	m_fValue = 2.5f;
	m_sValue = "TEST";
}

//TestRecordObject::TestRecordObject(const std::shared_ptr<TestRecordObject>& a_test)
//{
//	//
//}

void TestRecordObject::setValue(const int a_val) 
{
	assertModification(this);
	m_iValue = a_val; 
}

void TestRecordObject::setValue(const float a_val) 
{
	assertModification(this);
	m_fValue = a_val; 
}

void TestRecordObject::setValue(const std::string& a_val) 
{
	assertModification(this);
	m_sValue = a_val; 
}

int TestRecordObject::iVal()const noexcept { return m_iValue; }
float TestRecordObject::fVal()const noexcept { return m_fValue; }
std::string TestRecordObject::sVal()const noexcept { return m_sValue; }

unsigned short TestRecordObject::load(IInputStream& a_stream)
{
	a_stream >> m_iValue >> m_fValue >> m_sValue;
	return 0;
}

bool TestRecordObject::save(IOutputStream& a_stream)const
{
	a_stream << m_iValue << m_fValue << m_sValue;
	return true;
}