#pragma once

#include "GameCommon.h"
#include "SharedUsableItem.h"

class obj_Repair : public SharedUsableItem
{
	DECLARE_CLASS(obj_Repair, SharedUsableItem)

public:
	obj_Repair();
	virtual ~obj_Repair();

	virtual	BOOL		Load(const char *name);
	virtual	BOOL OnCreate();

	virtual void OnAction();
};