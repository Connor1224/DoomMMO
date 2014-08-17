#include "r3dPCH.h"
#include "r3d.h"

#include "obj_IndustGen.h"
#include "GameCommon.h"

#include "ObjectsCode/weapons/WeaponArmory.h"

IMPLEMENT_CLASS(obj_IndustGen, "obj_IndustGen", "Object");
AUTOREGISTER_CLASS(obj_IndustGen);

obj_IndustGen::obj_IndustGen()
{

}

obj_IndustGen::~obj_IndustGen()
{
}

BOOL obj_IndustGen::Load(const char *fname)
{
	const char* cpMeshName = "Data\\ObjectsDepot\\Weapons\\block_powergen_01_industrial.sco";
	if(!parent::Load(cpMeshName))
		return FALSE;

	return TRUE;
}

BOOL obj_IndustGen::OnCreate()
{
	m_spawnPos = GetPosition();
	SoundGenerator = 0;
	return parent::OnCreate();
}

BOOL obj_IndustGen::OnDestroy()
{
	SoundSys.Stop(SoundGenerator);
	SoundSys.Release(SoundGenerator);
	SoundGenerator=0;
	return parent::OnDestroy();
}

BOOL obj_IndustGen::Update()
{
	if (!SoundGenerator)
	{
		SoundGenerator = SoundSys.Play(SoundSys.GetEventIDByPath("Sounds/Effects/Generator/Generator_Running"), GetPosition());
		SoundSys.SetSoundPos(SoundGenerator, GetPosition());
	}
	return parent::Update();
}
