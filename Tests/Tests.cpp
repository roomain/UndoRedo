#include "CppUnitTest.h"
#include "UndoRedo.h"
#include "UndoRedoException.h"
#include "MVector.h"
#include "IRecordObject.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(Tests_UndoRedo)
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

		TEST_METHOD(TestMethod1)
		{
		}
	};
}
