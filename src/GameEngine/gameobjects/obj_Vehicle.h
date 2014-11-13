//=========================================================================
//	Module: obj_Vehicle.hpp
//	Copyright (C) 2011. Online Warmongers Group Inc. All rights reserved
//=========================================================================

#pragma once

#include "GameObj.h"
#include "../../EclipseStudio/Sources/ObjectsCode/Gameplay/SharedUsableItem.h"
#include "../../EclipseStudio/Sources/ObjectsCode/EFFECTS/obj_ParticleSystem.h"
#include "vehicle/PxVehicleDrive.h"
#include "../../EclipseStudio/Sources/ObjectsCode/AI/AI_Player.h"
#include "../../EclipseStudio/Sources/multiplayer/NetCellMover.h"
#include "../../EclipseStudio/Sources/ObjectsCode/world/Lamp.H"
//////////////////////////////////////////////////////////////////////////

#if VEHICLES_ENABLED

class obj_VehicleSpawn;
struct VehicleDescriptor;

class obj_Vehicle: public SharedUsableItem
{
	DECLARE_CLASS(obj_Vehicle, SharedUsableItem)

	VehicleDescriptor *vd;

	CNetCellMover	netMover;
	r3dPoint3D	netVelocity;
	
	void SyncPhysicsPoseWithObjectPose();
	void SetBoneMatrices();

public:
	obj_Vehicle();
	~obj_Vehicle();
	bool oldInAir;
	virtual BOOL Update();
	virtual BOOL OnCreate();
	virtual BOOL OnDestroy();
	virtual	BOOL		Load(const char *name);
	virtual void SetPosition(const r3dPoint3D& pos);
	virtual	void SetRotationVector(const r3dVector& Angles);
	virtual void OnPreRender() { SetBoneMatrices(); }

	//virtual r3dMesh*	GetObjectMesh();
	//virtual r3dMesh*	GetObjectLodMesh() OVERRIDE;
	r3dMesh* m_PrivateModel;
	obj_Player* owner;
	float falltime;
	float extime;
	float dura;
	float oldSpeed;
	float fuel;
	float RemoteSpeed;
	float rpm;
	bool status;
	bool bOn;
	void LightOnOff();
	void UpdateTraction();
	void		*footStepsSnd;
	void		*EngineSnd;
	void		*BreakSnd;
	void UpdateEngineSound();
	void SendCrashSnd(int id);
	int currentFootStepsSoundID;
	DWORD FireNetID;
	void UpdateBrake();
	class obj_ParticleSystem* m_ParticleTracer;
	class obj_ParticleSystem* m_ParticleBoostSmoke;
	class obj_LightHelper* Light;
	void SwitchToDrivable(bool doDrive);
	const VehicleDescriptor* getVehicleDescriptor() { return vd; }
#ifndef FINAL_BUILD
	float DrawPropertyEditor(float scrx, float scry, float scrw, float scrh, const AClass* startClass, const GameObjects& selected);
#endif
	const bool getExitSpace( r3dVector& outVector, int exitIndex );
	void setVehicleSpawner( obj_VehicleSpawn* targetSpawner) { spawner = targetSpawner;}
	void UpdatePositionFromRemote();
	void UpdatePositionFromPhysx();
	void OnNetPacket(const PKT_C2S_CarSound_s& n);
	//void OnNetPacket(const PKT_C2C_CarStatus_s& n);
	void OnNetPacket(const PKT_C2C_CarStatusSv_s& n);
	void OnNetPacket(const PKT_C2S_CarMove_s& n);
	void OnNetPacket(const PKT_C2C_MoveSetCell_s& n);
	void OnNetPacket(const PKT_C2C_CarSpeed_s& n);
	void OnNetPacket(const PKT_C2C_MoveRel_s& n);
	void OnNetPacket(const PKT_C2C_CarFlashLight_s& n);
	BOOL OnNetReceive(DWORD EventID, const void* packetData, int packetSize);
private:
	obj_VehicleSpawn* spawner;

};

#endif // VEHICLES_ENABLED