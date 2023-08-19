#pragma once
#include "IInputStream.h"
#include "UndoRedo_globals.h"


/*@brief record interface used for undo/redo*/
class UNDO_REDO_EXPORT IRecord
{
public:
	virtual void process(IInputStream& a_stream) = 0;
};