#include "r3dPCH.h"
#include "r3d.h"

#include "GameCommon.h"

#include "multiplayer/P2PMessages.h"
#include "ServerGameLogic.h"

#include "obj_ServerRepair.h"

IMPLEMENT_CLASS(obj_ServerRepair, "obj_Repair", "Object");
AUTOREGISTER_CLASS(obj_ServerRepair);

obj_ServerRepair::obj_ServerRepair()
{
}

obj_ServerRepair::~obj_ServerRepair()
{
}

BOOL obj_ServerRepair::OnCreate()
{
	r3dOutToLog("obj_ServerRepair %p created.\n", this);

	SetNetworkID(gServerLogic.net_lastFreeId++);

	SetPosition(GetPosition());
	SetRotationVector(GetRotationVector());
	
	gServerLogic.NetRegisterObjectToPeers(this);
	return parent::OnCreate();
}

BOOL obj_ServerRepair::OnDestroy()
{
	r3dOutToLog("obj_ServerRepair %p destroyed\n", this);
	return parent::OnDestroy();
}

BOOL obj_ServerRepair::Update()
{
	return parent::Update();
}

DefaultPacket* obj_ServerRepair::NetGetCreatePacket(int* out_size)
{
	static PKT_S2C_CreateRepairBench_s n;
	n.spawnID = toP2pNetId(GetNetworkID());
	n.pos     = GetPosition();
	n.rot     = GetRotationVector();
	
	*out_size = sizeof(n);
	return &n;
}