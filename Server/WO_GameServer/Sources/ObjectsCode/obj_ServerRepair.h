#pragma once

#include "GameCommon.h"
#include "NetworkHelper.h"

class obj_ServerRepair: public GameObject, INetworkHelper
{
	DECLARE_CLASS(obj_ServerRepair, GameObject)

public:
	obj_ServerRepair();
	~obj_ServerRepair();

	virtual BOOL	OnCreate();
	virtual BOOL	OnDestroy();
	
	virtual BOOL	Update();

	INetworkHelper*	GetNetworkHelper() { return dynamic_cast<INetworkHelper*>(this); }
	DefaultPacket*	INetworkHelper::NetGetCreatePacket(int* out_size);
};