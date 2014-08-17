#pragma once

#include "GameCommon.h"
#include "GameCode/UserProfile.h"
#include "SharedUsableItem.h"

class obj_IndustGen : public SharedUsableItem
{
	DECLARE_CLASS(obj_IndustGen, SharedUsableItem)
public:


public:
	obj_IndustGen();
	virtual ~obj_IndustGen();

	void*	SoundGenerator;

	virtual	BOOL		Load(const char *name);

	virtual	BOOL		OnCreate();
	virtual	BOOL		OnDestroy();

	virtual	BOOL		Update();
};
