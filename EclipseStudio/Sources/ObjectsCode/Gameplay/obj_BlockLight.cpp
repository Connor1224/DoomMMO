#include "r3dPCH.h"
#include "r3d.h"

#include "obj_BlockLight.h"
#include "GameCommon.h"

#include "ObjectsCode/weapons/WeaponArmory.h"

extern bool g_bEditMode;

IMPLEMENT_CLASS(obj_BlockLight, "obj_BlockLight", "Object");
AUTOREGISTER_CLASS(obj_BlockLight);

obj_BlockLight::obj_BlockLight()
{

}

obj_BlockLight::~obj_BlockLight()
{
}

BOOL obj_BlockLight::Load(const char *fname)
{
	const char* cpMeshName = "Data\\ObjectsDepot\\Weapons\\Block_Light_01.sco";
	if(!parent::Load(cpMeshName))
		return FALSE;

	return TRUE;
}

BOOL obj_BlockLight::OnCreate()
{
	m_spawnPos = GetPosition();
if ( !g_bEditMode )
{
	Light = (obj_LightHelper*)srv_CreateGameObject("obj_LightHelper", "Omni", GetPosition()+r3dPoint3D(2,2,2));
	//Light->SetRotationVector(GetRotationVector());
	Light->SetPosition(GetBBoxWorld().Center());
	Light->Color = r3dColor(255,241,196);
	Light->LT.Intensity = 0.0f;
	Light->bOn = true;
	EnableLights=false;
	Light->innerRadius = 0.0f;
	Light->outerRadius = 0.0f;
	Light->bKilled = false;
}

	return parent::OnCreate();
}

BOOL obj_BlockLight::OnDestroy()
{
	if (Light != NULL)
	{
		Light = NULL;
	}
	return parent::OnDestroy();
}

BOOL obj_BlockLight::Update()
{
if ( !g_bEditMode )
{
	EnableLights = false;
	for( GameObject* obj = GameWorld().GetFirstObject(); obj; obj = GameWorld().GetNextObject(obj) )
	{
		if (obj->Class->Name == "obj_IndustGen")
		{
			float dist = (GetPosition() - obj->GetPosition()).Length();
			if (dist<100)
			{
				EnableLights = true;
				break;
			}
		}
	}
	if (EnableLights)
	{
		Light->LT.Intensity = 2.36f;
		Light->innerRadius = 0.0f;
		Light->outerRadius = 11.58f;
	}
	else {
		Light->LT.Intensity = 0.0f;
		Light->innerRadius = 0.0f;
		Light->outerRadius = 0.0f;
	}
	Light->SetPosition(GetPosition()+r3dPoint3D(2,2,2));
}
	return parent::Update();
}
