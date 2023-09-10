#include "CppUnitTest.h"
#include "UndoRedo.h"
#include "UndoRedoException.h"
#include "IRecordObject.h"
#include "MVector.h"
#include "TestRecordObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UndoRedo_tests
{
	TEST_CLASS(Tests_UndoRedo)
	{
	public:
		TEST_CLASS_INITIALIZE(InitClass)
		{
			IRecordObject::initDef();
			TestRecordObject::initDef();
			TIContainer<size_t>::initDef();
			MVector<TestRecordObject>::initDef();
		}

		TEST_CLASS_CLEANUP(CleanupClass)
		{
			MVector<TestRecordObject>::releaseDef();
			TIContainer<size_t>::releaseDef();
			TestRecordObject::releaseDef();
			IRecordObject::releaseDef();
		}

		TEST_METHOD_CLEANUP(Test_cleanup)
		{
			UndoRedo::instance().clear();
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

		TEST_METHOD(Test_undo_add)
		{
			auto pVect = make_MShared<MVector<TestRecordObject>>();
			auto ptr = make_MShared<TestRecordObject>();
			ptr->setValue(1);
			ptr->setValue(3.14f);
			ptr->setValue("TEST_ADD");

			Assert::AreEqual(0, static_cast<int>(pVect->size()), L"Wrong size");
			UndoRedo::instance().startSession("Test_Add");
			pVect->push_back(ptr);
			UndoRedo::instance().endSession();
			Assert::AreEqual(1, static_cast<int>(pVect->size()), L"Wrong size");
			Assert::IsTrue(UndoRedo::instance().hasUndo(), L"No undo");
			UndoRedo::instance().undo();
			Assert::AreEqual(0, static_cast<int>(pVect->size()), L"Wrong size");
		}

		TEST_METHOD(Test_undo_redo_add)
		{
			auto pVect = make_MShared<MVector<TestRecordObject>>();
			auto ptr = make_MShared<TestRecordObject>();
			ptr->setValue(1);
			ptr->setValue(3.14f);
			ptr->setValue("TEST_ADD");

			Assert::AreEqual(0, static_cast<int>(pVect->size()), L"Wrong size");
			UndoRedo::instance().startSession("Test_Add");
			pVect->push_back(ptr);
			UndoRedo::instance().endSession();
			Assert::AreEqual(1, static_cast<int>(pVect->size()), L"Wrong size");
			Assert::IsTrue(UndoRedo::instance().hasUndo(), L"No undo");
			UndoRedo::instance().undo();
			Assert::AreEqual(0, static_cast<int>(pVect->size()), L"Wrong size");
			UndoRedo::instance().redo();
			Assert::AreEqual(1, static_cast<int>(pVect->size()), L"Wrong size");

			Assert::AreEqual(1, pVect->at(0)->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, pVect->at(0)->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_ADD"), pVect->at(0)->sVal(), L"Wrong string");

			// test same pointer
			Assert::IsTrue(pVect->at(0) == ptr, L"Not same pointer");
		}

		//-------------------------------------------------------------------------------------------------
		TEST_METHOD(Test_undo_add_stack)
		{
			MVector<TestRecordObject> pVect;
			auto ptr = make_MShared<TestRecordObject>();
			ptr->setValue(1);
			ptr->setValue(3.14f);
			ptr->setValue("TEST_ADD");

			Assert::AreEqual(0, static_cast<int>(pVect.size()), L"Wrong size");
			UndoRedo::instance().startSession("Test_Add");
			pVect.push_back(ptr);
			UndoRedo::instance().endSession();
			Assert::AreEqual(1, static_cast<int>(pVect.size()), L"Wrong size");
			Assert::IsTrue(UndoRedo::instance().hasUndo(), L"No undo");
			UndoRedo::instance().undo();
			Assert::AreEqual(0, static_cast<int>(pVect.size()), L"Wrong size");
		}

		TEST_METHOD(Test_undo_redo_add_stack)
		{
			MVector<TestRecordObject> pVect;
			auto ptr = make_MShared<TestRecordObject>();
			ptr->setValue(1);
			ptr->setValue(3.14f);
			ptr->setValue("TEST_ADD");

			Assert::AreEqual(0, static_cast<int>(pVect.size()), L"Wrong size");
			UndoRedo::instance().startSession("Test_Add");
			pVect.push_back(ptr);
			UndoRedo::instance().endSession();
			Assert::AreEqual(1, static_cast<int>(pVect.size()), L"Wrong size");
			Assert::IsTrue(UndoRedo::instance().hasUndo(), L"No undo");
			UndoRedo::instance().undo();
			Assert::AreEqual(0, static_cast<int>(pVect.size()), L"Wrong size");
			UndoRedo::instance().redo();
			Assert::AreEqual(1, static_cast<int>(pVect.size()), L"Wrong size");

			Assert::AreEqual(1, pVect.at(0)->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, pVect.at(0)->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_ADD"), pVect.at(0)->sVal(), L"Wrong string");

			// test same pointer
			Assert::IsTrue(pVect.at(0) == ptr, L"Not same pointer");
		}
		//-------------------------------------------------------------------------------------------------

		TEST_METHOD(Test_undo_change)
		{
			auto pVect = make_MShared<MVector<TestRecordObject>>();
			auto ptr = make_MShared<TestRecordObject>();
			ptr->setValue(1);
			ptr->setValue(3.14f);
			ptr->setValue("TEST_1");


			auto ptr2 = make_MShared<TestRecordObject>();
			ptr2->setValue(8);
			ptr2->setValue(6.17f);
			ptr2->setValue("TEST_2");


			pVect->push_back(ptr);
			Assert::IsTrue(pVect->at(0) == ptr, L"Not same pointer");
			UndoRedo::instance().startSession("Test_change");
			pVect->operator[](0) = ptr2;
			UndoRedo::instance().endSession();

			Assert::IsTrue(pVect->at(0) == ptr2, L"Not same pointer");
			Assert::IsTrue(UndoRedo::instance().hasUndo(), L"No undo");

			UndoRedo::instance().undo();
			Assert::IsTrue(pVect->at(0) == ptr, L"Not same pointer");
		}

		TEST_METHOD(Test_undo_redo_change)
		{
			auto pVect = make_MShared<MVector<TestRecordObject>>();
			auto ptr = make_MShared<TestRecordObject>();
			ptr->setValue(1);
			ptr->setValue(3.14f);
			ptr->setValue("TEST_1");


			auto ptr2 = make_MShared<TestRecordObject>();
			ptr2->setValue(8);
			ptr2->setValue(6.17f);
			ptr2->setValue("TEST_2");


			pVect->push_back(ptr);
			Assert::IsTrue(pVect->at(0) == ptr, L"Not same pointer");
			UndoRedo::instance().startSession("Test_change");
			pVect->operator[](0) = ptr2;
			UndoRedo::instance().endSession();

			Assert::IsTrue(pVect->at(0) == ptr2, L"Not same pointer");
			Assert::IsTrue(UndoRedo::instance().hasUndo(), L"No undo");

			UndoRedo::instance().undo();
			Assert::IsTrue(pVect->at(0) == ptr, L"Not same pointer");

			UndoRedo::instance().redo();
			Assert::IsTrue(pVect->at(0) == ptr2, L"Not same pointer");
		}
		//-------------------------------------------------------------------------------------------------
		TEST_METHOD(Test_undo_change_stack)
		{
			MVector<TestRecordObject> pVect;
			auto ptr = make_MShared<TestRecordObject>();
			ptr->setValue(1);
			ptr->setValue(3.14f);
			ptr->setValue("TEST_1");


			auto ptr2 = make_MShared<TestRecordObject>();
			ptr2->setValue(8);
			ptr2->setValue(6.17f);
			ptr2->setValue("TEST_2");


			pVect.push_back(ptr);
			Assert::IsTrue(pVect.at(0) == ptr, L"Not same pointer");
			UndoRedo::instance().startSession("Test_change");
			pVect.operator[](0) = ptr2;
			UndoRedo::instance().endSession();

			Assert::IsTrue(pVect.at(0) == ptr2, L"Not same pointer");
			Assert::IsTrue(UndoRedo::instance().hasUndo(), L"No undo");

			UndoRedo::instance().undo();
			Assert::IsTrue(pVect.at(0) == ptr, L"Not same pointer");
		}

		TEST_METHOD(Test_undo_redo_change_stack)
		{
			MVector<TestRecordObject> pVect;
			auto ptr = make_MShared<TestRecordObject>();
			ptr->setValue(1);
			ptr->setValue(3.14f);
			ptr->setValue("TEST_1");


			auto ptr2 = make_MShared<TestRecordObject>();
			ptr2->setValue(8);
			ptr2->setValue(6.17f);
			ptr2->setValue("TEST_2");


			pVect.push_back(ptr);
			Assert::IsTrue(pVect.at(0) == ptr, L"Not same pointer");
			UndoRedo::instance().startSession("Test_change");
			pVect.operator[](0) = ptr2;
			UndoRedo::instance().endSession();

			Assert::IsTrue(pVect.at(0) == ptr2, L"Not same pointer");
			Assert::IsTrue(UndoRedo::instance().hasUndo(), L"No undo");

			UndoRedo::instance().undo();
			Assert::IsTrue(pVect.at(0) == ptr, L"Not same pointer");

			UndoRedo::instance().redo();
			Assert::IsTrue(pVect.at(0) == ptr2, L"Not same pointer");
		}
		//-------------------------------------------------------------------------------------------------
		TEST_METHOD(Test_undo_erase)
		{
			auto pVect = make_MShared<MVector<TestRecordObject>>();

			{
				auto ptr = make_MShared<TestRecordObject>();
				ptr->setValue(1);
				ptr->setValue(3.14f);
				ptr->setValue("TEST_1");
				pVect->push_back(ptr);
			}

			Assert::AreEqual(1, static_cast<int>(pVect->size()), L"Wrong size");
			UndoRedo::instance().startSession("Test_erase");
			pVect->pop_back();
			UndoRedo::instance().endSession();
			Assert::AreEqual(0, static_cast<int>(pVect->size()), L"Wrong size");

			UndoRedo::instance().undo();
			Assert::AreEqual(1, static_cast<int>(pVect->size()), L"Wrong size");

			Assert::AreEqual(1, pVect->at(0)->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, pVect->at(0)->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_1"), pVect->at(0)->sVal(), L"Wrong string");
		}

		TEST_METHOD(Test_undo_redo_erase)
		{
			auto pVect = make_MShared<MVector<TestRecordObject>>();

			{
				auto ptr = make_MShared<TestRecordObject>();
				ptr->setValue(1);
				ptr->setValue(3.14f);
				ptr->setValue("TEST_1");
				pVect->push_back(ptr);
			}

			Assert::AreEqual(1, static_cast<int>(pVect->size()), L"Wrong size");
			UndoRedo::instance().startSession("Test_erase");
			pVect->pop_back();
			UndoRedo::instance().endSession();
			Assert::AreEqual(0, static_cast<int>(pVect->size()), L"Wrong size");

			UndoRedo::instance().undo();
			Assert::AreEqual(1, static_cast<int>(pVect->size()), L"Wrong size");

			Assert::AreEqual(1, pVect->at(0)->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, pVect->at(0)->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_1"), pVect->at(0)->sVal(), L"Wrong string");

			UndoRedo::instance().redo();
			Assert::AreEqual(0, static_cast<int>(pVect->size()), L"Wrong size");
		}

		//-------------------------------------------------------------------------------------------------
		TEST_METHOD(Test_undo_erase_stack)
		{
			MVector<TestRecordObject> pVect;

			{
				auto ptr = make_MShared<TestRecordObject>();
				ptr->setValue(1);
				ptr->setValue(3.14f);
				ptr->setValue("TEST_1");
				pVect.push_back(ptr);
			}

			Assert::AreEqual(1, static_cast<int>(pVect.size()), L"Wrong size");
			UndoRedo::instance().startSession("Test_erase");
			pVect.pop_back();
			UndoRedo::instance().endSession();
			Assert::AreEqual(0, static_cast<int>(pVect.size()), L"Wrong size");

			UndoRedo::instance().undo();
			Assert::AreEqual(1, static_cast<int>(pVect.size()), L"Wrong size");

			Assert::AreEqual(1, pVect.at(0)->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, pVect.at(0)->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_1"), pVect.at(0)->sVal(), L"Wrong string");
		}

		TEST_METHOD(Test_undo_redo_erase_stack)
		{
			MVector<TestRecordObject> pVect;

			{
				auto ptr = make_MShared<TestRecordObject>();
				ptr->setValue(1);
				ptr->setValue(3.14f);
				ptr->setValue("TEST_1");
				pVect.push_back(ptr);
			}

			Assert::AreEqual(1, static_cast<int>(pVect.size()), L"Wrong size");
			UndoRedo::instance().startSession("Test_erase");
			pVect.pop_back();
			UndoRedo::instance().endSession();
			Assert::AreEqual(0, static_cast<int>(pVect.size()), L"Wrong size");

			UndoRedo::instance().undo();
			Assert::AreEqual(1, static_cast<int>(pVect.size()), L"Wrong size");

			Assert::AreEqual(1, pVect.at(0)->iVal(), L"Wrong int");
			Assert::AreEqual(3.14f, pVect.at(0)->fVal(), L"Wrong float");
			Assert::AreEqual(std::string("TEST_1"), pVect.at(0)->sVal(), L"Wrong string");

			UndoRedo::instance().redo();
			Assert::AreEqual(0, static_cast<int>(pVect.size()), L"Wrong size");
		}

	};
}
