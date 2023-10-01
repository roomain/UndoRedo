#include "CppUnitTest.h"
#include "Reference.h"
#include "IRecordObject.h"
#include <variant>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

class ObjectReferenced : public RefObject
{
private:
	int m_value = 0;

public:
	ObjectReferenced() = default;
	ObjectReferenced(const ObjectReferenced& a_other) : m_value{ a_other.m_value}
	{

	}
	ObjectReferenced(ObjectReferenced&& a_other) noexcept : m_value{ std::move(a_other.m_value) }
	{

	}
	virtual ~ObjectReferenced() = default;

	void operator = (const ObjectReferenced& a_other)
	{
		m_value = a_other.m_value;
	}

	void operator = (ObjectReferenced&& a_other)
	{
		m_value = a_other.m_value;
	}

	void setValue(const int a_val) { m_value = a_val; }
	int value()const { return m_value; }

};

class ObjectInheritReferenced : public ObjectReferenced
{
private:
	double m_dvalue = 5.6;

public:
	ObjectInheritReferenced() = default;
	ObjectInheritReferenced(const ObjectInheritReferenced& a_other) : 
		ObjectReferenced(a_other), m_dvalue{ a_other.m_dvalue }
	{

	}
	ObjectInheritReferenced(ObjectInheritReferenced&& a_other) noexcept : 
		ObjectReferenced(a_other), m_dvalue{ std::move(a_other.m_dvalue) }
	{

	}
	virtual ~ObjectInheritReferenced() = default;

	void operator = (const ObjectInheritReferenced& a_other)
	{
		ObjectReferenced::operator = (a_other);
		m_dvalue = a_other.m_dvalue;
	}

	void operator = (ObjectInheritReferenced&& a_other)
	{
		ObjectReferenced::operator = (a_other);
		m_dvalue = a_other.m_dvalue;
	}

	void setValueDouble(const double& a_val) { m_dvalue = a_val; }
	double valueDouble()const { return m_dvalue; }

};

class Base : public RefObject
{
public:
	virtual void test() = 0;
};

class BaseTest : public Base
{
public:
	virtual void test()override {}
};

class TestRecord : public IRecordObject
{
public:
	virtual unsigned short load(IInputStream& a_stream) { return 1; }
	virtual bool save(IOutputStream& a_stream)const { return false; }
};

namespace Reference_tests
{
	TEST_CLASS(Tests_Ref)
	{
	public:
		TEST_CLASS_INITIALIZE(InitClass)
		{
			//
		}

		TEST_CLASS_CLEANUP(CleanupClass)
		{
			//
		}

		TEST_METHOD_CLEANUP(Test_cleanup)
		{
			//
		}

		TEST_METHOD(Test_validity_at_creation)
		{
			Ref<ObjectReferenced> ref;
			Assert::IsFalse(ref.valid(), L"Reference must be invalid");
		}

		TEST_METHOD(Test_validity_after_affectation)
		{
			ObjectReferenced obj;
			Ref<ObjectReferenced> ref = make_ref(obj);
			Assert::IsTrue(ref.valid(), L"Reference must be valid");
		}

		TEST_METHOD(Test_validity_at_creation_with_rvalue)
		{
			Ref<ObjectReferenced> ref = make_ref(ObjectReferenced());
			Assert::IsFalse(ref.valid(), L"Reference must be invalid");
		}

		TEST_METHOD(Test_validity_after_destruction)
		{
			Ref<ObjectReferenced> ref;
			{
				ObjectReferenced obj;
				ref = make_ref(obj);
			}
			Assert::IsFalse(ref.valid(), L"Reference must be invalid");
		}

		TEST_METHOD(Test_GetRef)
		{
			ObjectReferenced obj;
			Ref<ObjectReferenced> ref = make_ref(obj);
			Assert::IsTrue(ref.pointer() == &obj, L"Reference must point to object");
		}

		TEST_METHOD(Test_SetValue)
		{
			ObjectReferenced obj;
			Ref<ObjectReferenced> ref = make_ref(obj);
			Assert::AreEqual(0, obj.value(), L"Wrong init");
			ref->setValue(5);
			Assert::AreEqual(5, obj.value(), L"Reference must call object functions");
			Assert::AreEqual(5, ref->value(), L"Reference must call object functions");
		}

		TEST_METHOD(Test_virtual)
		{
			Base* obj = new BaseTest;
			Ref<Base> ref = make_ref(*obj);
			Assert::IsTrue(ref.valid(), L"Reference must be valid");
			delete obj;
		}

		TEST_METHOD(Test_variant)
		{
			using Proxy = std::variant<std::shared_ptr< IRecordObject>, Ref< IRecordObject>>;
			IRecordObject* obj = new TestRecord;
			Proxy ref = make_ref(*obj);
			delete obj;
		}

	};
}
