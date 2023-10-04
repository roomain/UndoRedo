#include "CppUnitTest.h"
#include "UndoRedo.h"
#include "UndoRedoException.h"
#include "IRecordObject.h"
#include "MVector.h"
#include "TestRecordObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
class Test{};

namespace UndoRedo_tests
{
	TEST_CLASS(Tests_UndoRedo)
	{
	public:
		TEST_CLASS_INITIALIZE(InitClass)
		{
			IRecordObject::initDef();
			TestRecordObject::initDef();
		}

		TEST_CLASS_CLEANUP(CleanupClass)
		{
			TestRecordObject::releaseDef();
			IRecordObject::releaseDef();
		}

		TEST_METHOD_CLEANUP(Test_cleanup)
		{
			UndoRedo::instance().clear();
		}
		TEST_METHOD(Test_MShared)
		{
			auto pTest = make_MShared<Test>();
			Assert::IsNotNull(pTest.get(), L"Null");
		}
		TEST_METHOD(Test_scopedActivation)
		{
			Assert::IsFalse(UndoRedo::instance().hasActiveSession(), L"Active session");

			{
				TScoped<bool> scoped = UndoRedo::instance().scopedActivation();
				scoped = true;
				Assert::IsTrue(UndoRedo::instance().hasActiveSession(), L"No Active session");
			}
			Assert::IsFalse(UndoRedo::instance().hasActiveSession(), L"Active session");
		}

		TEST_METHOD(Test_undo_modification)
		{
			auto ptr = make_MShared<TestRecordObject>();
			ptr->setValue(1);
			ptr->setValue(3.14f);
			ptr->setValue("TEST_CHANGE");
			Assert::AreEqual(1, ptr->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, ptr->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_CHANGE"), ptr->sVal(), L"Wrong string");
			UndoRedo::instance().startSession("Test_change");
			ptr->setValue(5);
			UndoRedo::instance().endSession();
			Assert::AreEqual(5, ptr->iVal(), L"Wrong int");
			Assert::IsTrue(UndoRedo::instance().hasUndo(), L"No undo");
			UndoRedo::instance().undo();
			Assert::AreEqual(1, ptr->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, ptr->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_CHANGE"), ptr->sVal(), L"Wrong string");
		}

		TEST_METHOD(Test_redo_modification)
		{
			auto ptr = make_MShared<TestRecordObject>();
			ptr->setValue(1);
			ptr->setValue(3.14f);
			ptr->setValue("TEST_CHANGE");
			Assert::AreEqual(1, ptr->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, ptr->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_CHANGE"), ptr->sVal(), L"Wrong string");
			UndoRedo::instance().startSession("Test_change");
			ptr->setValue(5);
			UndoRedo::instance().endSession();
			Assert::AreEqual(5, ptr->iVal(), L"Wrong int");
			Assert::IsTrue(UndoRedo::instance().hasUndo(), L"No undo");
			UndoRedo::instance().undo();
			Assert::AreEqual(1, ptr->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, ptr->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_CHANGE"), ptr->sVal(), L"Wrong string");
			UndoRedo::instance().redo();
			Assert::AreEqual(5, ptr->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, ptr->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_CHANGE"), ptr->sVal(), L"Wrong string");
		}

		TEST_METHOD(Test_redo_undo_modification)
		{
			auto ptr = make_MShared<TestRecordObject>();
			ptr->setValue(1);
			ptr->setValue(3.14f);
			ptr->setValue("TEST_CHANGE");
			Assert::AreEqual(1, ptr->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, ptr->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_CHANGE"), ptr->sVal(), L"Wrong string");
			UndoRedo::instance().startSession("Test_change");
			ptr->setValue(5);
			UndoRedo::instance().endSession();
			Assert::AreEqual(5, ptr->iVal(), L"Wrong int");
			Assert::IsTrue(UndoRedo::instance().hasUndo(), L"No undo");
			UndoRedo::instance().undo();
			Assert::AreEqual(1, ptr->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, ptr->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_CHANGE"), ptr->sVal(), L"Wrong string");
			UndoRedo::instance().redo();
			Assert::AreEqual(5, ptr->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, ptr->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_CHANGE"), ptr->sVal(), L"Wrong string");
			UndoRedo::instance().undo();
			Assert::AreEqual(1, ptr->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, ptr->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_CHANGE"), ptr->sVal(), L"Wrong string");
		}


	};
}
