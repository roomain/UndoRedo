#pragma once
#include "UndoRedo_globals.h"

class IRecordObject;

void UNDO_REDO_EXPORT assertModification(IRecordObject* const a_object);
void UNDO_REDO_EXPORT assertDeletion(const IRecordObject* a_object);
