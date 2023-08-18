#pragma once

/*@brief record interface used for undo/redo*/
class IRecord
{
public:
	virtual void process(/**/) = 0;
};