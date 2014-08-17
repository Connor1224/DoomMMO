#include "r3dPCH.h"
#include "r3d.h"

#include "GameCommon.h"
#include "obj_Repair.h"
#include "UI\HUDRepair.h"

IMPLEMENT_CLASS(obj_Repair, "obj_Repair", "Object");
AUTOREGISTER_CLASS(obj_Repair);

extern HUDRepair* hudRepair;

obj_Repair::obj_Repair()
{
}

obj_Repair::~obj_Repair()
{
}

BOOL obj_Repair::Load(const char *fname)
{
	const char* cpMeshName = "Data\\ObjectsDepot\\Weapons\\item_repairkit_bench_01.sco";
	if(!parent::Load(cpMeshName)) 
		return FALSE;
	return TRUE;
}

BOOL obj_Repair::OnCreate()
{
	m_ActionUI_Title = gLangMngr.getString("RepairBench");
	m_ActionUI_Msg = gLangMngr.getString("HoldEToAccessRepairBench");

	m_spawnPos = GetPosition();

	return parent::OnCreate();
}

void obj_Repair::OnAction()
{
	hudRepair->Activate();
}
