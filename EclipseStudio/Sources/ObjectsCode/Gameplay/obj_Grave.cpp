#include "r3dPCH.h"
#include "r3d.h"

#include "obj_Grave.h"
#include "GameCommon.h"

#include "ObjectsCode/weapons/WeaponArmory.h"

IMPLEMENT_CLASS(obj_Grave, "obj_Grave", "Object");
AUTOREGISTER_CLASS(obj_Grave);

obj_Grave::obj_Grave()
{
	m_GotData = false;
}

obj_Grave::~obj_Grave()
{
}

BOOL obj_Grave::Load(const char *fname)
{
	int Gravestone = (int)u_GetRandom(1.0f,4.0f);
	char FileGrave[512]="Data\\ObjectsDepot\\Weapons\\item_gravestone_01.sco";

	switch(Gravestone)
		{
		case 1:
			    strcpy(FileGrave,"Data\\ObjectsDepot\\Weapons\\item_gravestone_01.sco");
				break;
		case 2:
			    strcpy(FileGrave,"Data\\ObjectsDepot\\Weapons\\item_gravestone_02.sco");
				break;
		case 3:
			    strcpy(FileGrave,"Data\\ObjectsDepot\\Weapons\\item_gravestone_03.sco");
				break;
		case 4:
			    strcpy(FileGrave,"Data\\ObjectsDepot\\Weapons\\item_gravestone_03.sco");
				break;
		}
	const char* cpMeshName =FileGrave;
	if(!parent::Load(cpMeshName)) 
		return FALSE;

	return TRUE;
}

BOOL obj_Grave::OnCreate()
{
	m_ActionUI_Title = gLangMngr.getString("$FR_Gravestone");
	m_ActionUI_Msg = gLangMngr.getString("HoldEToReadGravestone");

	m_spawnPos = GetPosition();

	if(!DisablePhysX)
	{
		ReadPhysicsConfig();
		PhysicsConfig.group = PHYSCOLL_TINY_GEOMETRY; // skip collision with players
		PhysicsConfig.requireNoBounceMaterial = true;
		PhysicsConfig.isFastMoving = true;
		m_bEnablePhysics = false;
	}

	return parent::OnCreate();
}

BOOL obj_Grave::OnDestroy()
{
	return parent::OnDestroy();
}

BOOL obj_Grave::Update()
{
	return parent::Update();
}
