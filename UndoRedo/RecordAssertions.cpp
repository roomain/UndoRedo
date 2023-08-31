#include "pch.h"
#include "RecordAssertions.h"
#include "IRecordObject.h"
#include "Record.h"
#include "UndoRedo.h"
#include "RecordSession.h"
#include "OutputBinStream.h"

void assertModification(IRecordObject* const a_object)
{
	if (UndoRedo::instance().sessionStarted())
	{
		RecordSession& curSession = UndoRedo::instance().currentSession();
		curSession.addRecord(std::make_shared<RecordModification>(a_object->weak_from_this(), curSession.saveStream()));
	}
}

void assertDeletion(const IRecordObject* a_object)
{
	if (UndoRedo::instance().sessionStarted())
	{
		RecordSession& curSession = UndoRedo::instance().currentSession();
		curSession.addRecord(std::make_shared<RecordDeletion>(a_object, curSession.saveStream()));
	}
}