#pragma once

#include "GameCommon.h"
#include "GameCode/UserProfile.h"
#include "SharedUsableItem.h"
#include "../../EclipseStudio/Sources/ObjectsCode/world/Lamp.h"

class obj_BlockLight : public SharedUsableItem
{
	DECLARE_CLASS(obj_BlockLight, SharedUsableItem)

	class obj_LightHelper* Light;

public:
	obj_BlockLight();
	virtual ~obj_BlockLight();

	bool EnableLights;

	void*	SoundGenerator;

	virtual	BOOL		Load(const char *name);

	virtual	BOOL		OnCreate();
	virtual	BOOL		OnDestroy();

	virtual	BOOL		Update();
};
