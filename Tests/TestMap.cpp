#include "CppUnitTest.h"
#include "UndoRedo.h"
#include "UndoRedoException.h"
#include "IRecordObject.h"
#include "MMap.h"
#include "TestRecordObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UndoRedo_tests
{
	TEST_CLASS(Tests_map)
	{
	public:
		TEST_CLASS_INITIALIZE(InitClass)
		{
			IRecordObject::initDef();
			TestRecordObject::initDef();
			//TIContainer<size_t>::initDef();
			//MMap<std::string, TestRecordObject>::initDef();
		}

		TEST_CLASS_CLEANUP(CleanupClass)
		{
			//MMap<std::string, TestRecordObject>::releaseDef();
			//TIContainer<size_t>::releaseDef();
			TestRecordObject::releaseDef();
			IRecordObject::releaseDef();
		}

		TEST_METHOD_CLEANUP(Test_cleanup)
		{
			UndoRedo::instance().clear();
		}

		TEST_METHOD(Test_operator)
		{
			MMap<std::string, TestRecordObject> map;
			auto ptr = make_MShared<TestRecordObject>();
			auto ptr1 = make_MShared<TestRecordObject>();
			map["test"] = ptr;
			map["test"] = ptr1;
		}

		TEST_METHOD(Test_undo_add)
		{
			MMap<std::string, TestRecordObject> map;
			auto ptr = make_MShared<TestRecordObject>();
			ptr->setValue(1);
			ptr->setValue(3.14f);
			ptr->setValue("TEST_ADD");

			Assert::AreEqual(0, static_cast<int>(map.size()), L"Wrong size");
			UndoRedo::instance().startSession("Test_Add");
			map["test"] = ptr;
			UndoRedo::instance().endSession();
			Assert::AreEqual(1, static_cast<int>(map.size()), L"Wrong size");
			Assert::IsTrue(UndoRedo::instance().hasUndo(), L"No undo");
			UndoRedo::instance().undo();
			Assert::AreEqual(0, static_cast<int>(map.size()), L"Wrong size");
		}

		TEST_METHOD(Test_undo_redo_add)
		{
			MMap<std::string, TestRecordObject> map;
			auto ptr = make_MShared<TestRecordObject>();
			ptr->setValue(1);
			ptr->setValue(3.14f);
			ptr->setValue("TEST_ADD");

			Assert::AreEqual(0, static_cast<int>(map.size()), L"Wrong size");
			UndoRedo::instance().startSession("Test_Add");
			map["test"] = ptr;
			UndoRedo::instance().endSession();
			Assert::AreEqual(1, static_cast<int>(map.size()), L"Wrong size");
			Assert::IsTrue(UndoRedo::instance().hasUndo(), L"No undo");
			UndoRedo::instance().undo();
			Assert::AreEqual(0, static_cast<int>(map.size()), L"Wrong size");
			UndoRedo::instance().redo();
			Assert::AreEqual(1, static_cast<int>(map.size()), L"Wrong size");

			Assert::AreEqual(1, map.at("test")->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, map.at("test")->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_ADD"), map.at("test")->sVal(), L"Wrong string");

			// test same pointer
			Assert::IsTrue(map.at("test") == ptr, L"Not same pointer");
		}

		TEST_METHOD(Test_undo_change_stack)
		{
			MMap<std::string, TestRecordObject> map;
			auto ptr = make_MShared<TestRecordObject>();
			ptr->setValue(1);
			ptr->setValue(3.14f);
			ptr->setValue("TEST_1");


			auto ptr2 = make_MShared<TestRecordObject>();
			ptr2->setValue(8);
			ptr2->setValue(6.17f);
			ptr2->setValue("TEST_2");


			map["test"] = ptr;
			Assert::IsTrue(map.at("test") == ptr, L"Not same pointer");
			UndoRedo::instance().startSession("Test_change");
			map["test"] = ptr2;
			UndoRedo::instance().endSession();

			Assert::IsTrue(map.at("test") == ptr2, L"Not same pointer");
			Assert::IsTrue(UndoRedo::instance().hasUndo(), L"No undo");

			UndoRedo::instance().undo();
			Assert::IsTrue(map.at("test") == ptr, L"Not same pointer");
		}

		TEST_METHOD(Test_undo_redo_change_stack)
		{
			MMap<std::string, TestRecordObject> map;
			auto ptr = make_MShared<TestRecordObject>();
			ptr->setValue(1);
			ptr->setValue(3.14f);
			ptr->setValue("TEST_1");


			auto ptr2 = make_MShared<TestRecordObject>();
			ptr2->setValue(8);
			ptr2->setValue(6.17f);
			ptr2->setValue("TEST_2");


			map["test"] = ptr;
			Assert::IsTrue(map.at("test") == ptr, L"Not same pointer");
			UndoRedo::instance().startSession("Test_change");
			map["test"] = ptr2;
			UndoRedo::instance().endSession();

			Assert::IsTrue(map.at("test") == ptr2, L"Not same pointer");
			Assert::IsTrue(UndoRedo::instance().hasUndo(), L"No undo");

			UndoRedo::instance().undo();
			Assert::IsTrue(map.at("test") == ptr, L"Not same pointer");

			UndoRedo::instance().redo();
			Assert::IsTrue(map.at("test") == ptr2, L"Not same pointer");
		}

		TEST_METHOD(Test_undo_erase_stack)
		{
			MMap<std::string, TestRecordObject> map;

			{
				auto ptr = make_MShared<TestRecordObject>();
				ptr->setValue(1);
				ptr->setValue(3.14f);
				ptr->setValue("TEST_1");
				map["test"] = ptr;
			}

			Assert::AreEqual(1, static_cast<int>(map.size()), L"Wrong size");
			UndoRedo::instance().startSession("Test_erase");
			map.erase("test");
			UndoRedo::instance().endSession();
			Assert::AreEqual(0, static_cast<int>(map.size()), L"Wrong size");

			UndoRedo::instance().undo();
			Assert::AreEqual(1, static_cast<int>(map.size()), L"Wrong size");

			Assert::AreEqual(1, map.at("test")->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, map.at("test")->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_1"), map.at("test")->sVal(), L"Wrong string");
		}

		TEST_METHOD(Test_undo_redo_erase_stack)
		{
			MMap<std::string, TestRecordObject> map;

			{
				auto ptr = make_MShared<TestRecordObject>();
				ptr->setValue(1);
				ptr->setValue(3.14f);
				ptr->setValue("TEST_1");
				map["test"] = ptr;
			}

			Assert::AreEqual(1, static_cast<int>(map.size()), L"Wrong size");
			UndoRedo::instance().startSession("Test_erase");
			map.erase("test");
			UndoRedo::instance().endSession();
			Assert::AreEqual(0, static_cast<int>(map.size()), L"Wrong size");

			UndoRedo::instance().undo();
			Assert::AreEqual(1, static_cast<int>(map.size()), L"Wrong size");

			Assert::AreEqual(1, map.at("test")->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, map.at("test")->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_1"), map.at("test")->sVal(), L"Wrong string");

			UndoRedo::instance().redo();
			Assert::AreEqual(0, static_cast<int>(map.size()), L"Wrong size");
		}
	};
}
