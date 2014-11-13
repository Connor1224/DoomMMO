//=========================================================================
//	Module: obj_Vehicle.cpp
//	Copyright (C) 2011. Online Warmongers Group Inc. All rights reserved
//=========================================================================

#include "r3dPCH.h"

#if VEHICLES_ENABLED

#include "r3d.h"
#include "PhysXWorld.h"
#include "ObjManag.h"
#include "GameCommon.h"

#include "obj_Vehicle.h"
#include "VehicleDescriptor.h"
#include "../../EclipseStudio/Sources/ObjectsCode/world/DecalChief.h"
#include "../../EclipseStudio/Sources/ObjectsCode/world/MaterialTypes.h"
#include "../../EclipseStudio/Sources/ObjectsCode/EFFECTS/obj_ParticleSystem.h"
#include "../../EclipseStudio/Sources/Editors/ObjectManipulator3d.h"
#include "../../EclipseStudio/Sources/ObjectsCode/Gameplay/obj_VehicleSpawn.h"
#include "../../EclipseStudio/Sources/ObjectsCode/Gameplay/obj_Zombie.h"
#include "../../EclipseStudio/Sources/ObjectsCode/AI/AI_Player.h"
#include "../../EclipseStudio/Sources/ObjectsCode/world/Lamp.H"
#include "../../EclipseStudio/Sources/multiplayer/ClientGameLogic.h"
#include "..\..\EclipseStudio\Sources\ui\HUDActionUI.h"
#include "..\..\EclipseStudio\Sources\ui\HUDDisplay.h"
//#include "..\..\EclipseStudio\Sources\ui\HUDRepair.h"

extern bool g_bEditMode;
extern ObjectManipulator3d g_Manipulator3d;
extern int CurHUDID;
extern HUDActionUI*	hudActionUI;
extern HUDDisplay*	hudMain;
//extern HUDRepair*	hudRepair;
//////////////////////////////////////////////////////////////////////////

namespace
{
	void QuaternionToEulerAngles(PxQuat &q, float &xRot, float &yRot, float &zRot)
	{
		q.normalize();

		void MatrixGetYawPitchRoll ( const D3DXMATRIX & mat, float & fYaw, float & fPitch, float & fRoll );
		PxMat33 mat(q);
		D3DXMATRIX res;
		D3DXMatrixIdentity(&res);
		res._11 = mat.column0.x;
		res._12 = mat.column0.y;
		res._13 = mat.column0.z;

		res._21 = mat.column1.x;
		res._22 = mat.column1.y;
		res._23 = mat.column1.z;

		res._31 = mat.column2.x;
		res._32 = mat.column2.y;
		res._33 = mat.column2.z;

		MatrixGetYawPitchRoll(res, xRot, yRot, zRot);
	}

	//////////////////////////////////////////////////////////////////////////

	/** Helper constant transformation factors */
	struct UsefulTransforms 
	{
		PxQuat rotY_quat;
		D3DXMATRIX rotY_mat;

		UsefulTransforms()
		{
			D3DXQUATERNION rotY_D3D;
			D3DXQuaternionRotationYawPitchRoll(&rotY_D3D, D3DX_PI / 2, 0, 0);
			rotY_quat = PxQuat(rotY_D3D.x, rotY_D3D.y, rotY_D3D.z, rotY_D3D.w);
			D3DXMatrixRotationQuaternion(&rotY_mat, &rotY_D3D);
		}
	};
}

//////////////////////////////////////////////////////////////////////////

obj_Vehicle::obj_Vehicle()
: vd(0)
, footStepsSnd(0)
,netMover(this, 0.1f, (float)PKT_C2C_MoveSetCell_s::VEHICLE_CELL_RADIUS)
{
	m_bEnablePhysics = false;
	ObjTypeFlags |= OBJTYPE_Vehicle;
	//ObjType |= OBJTYPE_Vehicle;
	spawner = NULL;
	m_isSerializable = false;
}

//////////////////////////////////////////////////////////////////////////

obj_Vehicle::~obj_Vehicle()
{
	g_pPhysicsWorld->m_VehicleManager->DeleteCar(vd);
	if( spawner )
	{
		spawner->clearVehicle();
		spawner = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////

void obj_Vehicle::UpdatePositionFromPhysx()
{
	if (!vd)
		return;

	PxRigidDynamicFlags f = vd->vehicle->getRigidDynamicActor()->getRigidDynamicFlags();
	if ( !(f & PxRigidDynamicFlag::eKINEMATIC) )
	{

		PxTransform t = vd->vehicle->getRigidDynamicActor()->getGlobalPose();
		SetPosition(r3dVector(t.p.x, t.p.y, t.p.z));

		r3dVector angles;
		QuaternionToEulerAngles(t.q, angles.x, angles.y, angles.z);
		//	To degrees
		angles = D3DXToDegree(angles);

		SetRotationVector(angles);
	}
}

//////////////////////////////////////////////////////////////////////////

BOOL obj_Vehicle::Update()
{
	if (!gClientLogic().localPlayer_) return parent::Update();
	//r3dOutToLog("Vehicle Update Networklocal : %d\n",(int)NetworkLocal);
#ifndef FINAL_BUILD

	if ( g_bEditMode )
	{
		if( CurHUDID == 0 && d_drive_vehicles->GetBool() == false ) {
			if ( spawner != NULL && g_Manipulator3d.IsSelected( this ) )
			{
				spawner->SetPosition( GetPosition() );
				spawner->SetRotationVector( GetRotationVector() );
			}
		}
	}
#endif 

	/*r3dPoint3D scrCoord;
	r3dProjectToScreen(GetPosition() + r3dPoint3D(0, 1.8f, 0), &scrCoord);
	Font_Editor->PrintF(scrCoord.x, scrCoord.y,    r3dColor(255,255,255,100), "Speed : %f", getVehicleDescriptor()->vehicle->computeForwardSpeed());
	Font_Editor->PrintF(scrCoord.x, scrCoord.y-12,    r3dColor(255,255,255,100), "isInAir : %s", (getVehicleDescriptor()->vehicle->isInAir() ? "true" : "false"));*/
	dura = 100.0f;
	//UpdateTraction();
	//UpdateBrake();
	SyncPhysicsPoseWithObjectPose();
	//UpdateTransform();
	UpdateEngineSound();
	Light->SetRotationVector(GetRotationVector());
	Light->SetPosition(GetBBoxWorld().Center());
	//Light->setActiveFlag((int)bOn);
	//SoundSys.Stop(gClientLogic().localPlayer_->m_sndVehicleDrive);
	LightOnOff();
	float speeda = getVehicleDescriptor()->vehicle->computeForwardSpeed()*1.25f;
	/*if (speeda < 15 && m_ParticleBoostSmoke)
	{
	m_ParticleBoostSmoke->bKillDelayed = 1;
	m_ParticleBoostSmoke = NULL;
	}
	else if (!m_ParticleBoostSmoke)
	m_ParticleBoostSmoke = (obj_ParticleSystem*)srv_CreateGameObject("obj_ParticleSystem", "vehicle_damage_01", GetPosition() );
	*/
//	if (m_ParticleBoostSmoke)
//		m_ParticleBoostSmoke->SetPosition(GetPosition());
	if (gClientLogic().localPlayer_->isInVehicle() && !d_drive_vehicles->GetBool() && gClientLogic().localPlayer_->curcar == this && !gClientLogic().localPlayer_->isDriving())
	{
		/*int value = (int)dura/20;
		if (dura < 20.0f && dura > 0.0f)
		{
		value = 1;
		}
		float speedshow = R3D_MIN(RemoteSpeed,100.0f);*/
		//hudMain->setCarInfo(value,(int)speedshow,40,20,(int)fuel,true);
		float RemoteSpeed2 = RemoteSpeed*1.25f;
		float RemoteSpeed3 = RemoteSpeed*2.0f;
		hudMain->setCarInfo((int)dura,(int)100-(int)RemoteSpeed2,(int)RemoteSpeed3,(int)fuel,100-(int)rpm/15,true);
	}

	if (NetworkLocal)
	{
		if (InputMappingMngr->wasPressed(r3dInputMappingMngr::KS_TOGGLE_FLASHLIGHT))
		{
			bOn = !bOn;
			//LightOnOff();
			SoundSys.PlayAndForget(SoundSys.GetEventIDByPath("Sounds/NewWeapons/Melee/flashlight"), GetPosition());
			PKT_C2C_CarFlashLight_s n;
			n.bOn = bOn;
			p2pSendToHost(this, &n, sizeof(n));
			SendCrashSnd(SoundSys.GetEventIDByPath("Sounds/NewWeapons/Melee/flashlight"));
		}
	}
	/*	if(!hudActionUI->isActive() )
	}

	if (d_drive_vehicles->GetBool())
	{
	/*if (!Light)
	{
	Light = (obj_LightHelper*)srv_CreateGameObject("obj_LightHelper", "Spot", GetPosition ());
	Light->SetRotationVector(GetRotationVector());
	//Light->m_pPreset = m_pPreset;
	Light->SetPosition(GetPosition());
	Light->Color = r3dColor::white;;
	/*Light->LT.bDiffuseOnly = 0;
	Light->LT.bCastShadows = 0;
	Light->LT.bDiffuseOnly = 0;
	Light->LT.bDiffuseOnly = 0;
	Light->LT.Intensity = 15.0f;
	Light->bOn = true;
	Light->innerRadius = 0.0f;
	Light->outerRadius = 15.0f;
	Light->bKilled = false;

	Light->bSerialized = true;
	}*/
	float rpm = R3D_MIN(getVehicleDescriptor()->vehicle->computeForwardSpeed()*55 , 8000.0f);

	//SoundSys.SetParamValue(gClientLogic().localPlayer_->m_sndVehicleDrive,"rpm",rpm);

	if (NetworkLocal)
	{

		if (fuel < 2.0f && d_drive_vehiclescon->GetBool())
		{
			//SoundSys.Stop(gClientLogic().localPlayer_->m_sndVehicleDrive);
			SoundSys.Play(SoundSys.GetEventIDByPath("Sounds/Vehicles/DuneBuggyEngine_Stop"), GetPosition());
			SendCrashSnd(SoundSys.GetEventIDByPath("Sounds/Vehicles/DuneBuggyEngine_Stop"));
			d_drive_vehiclescon->SetBool(false);
			bOn = false;
		}
		int value = (int)dura/20;
		if (dura < 20.0f && dura > 0.0f)
		{
			value = 1;
		}
		//float speedshow = R3D_MIN(getVehicleDescriptor()->vehicle->computeForwardSpeed()*2,100.0f);
		float speeda = getVehicleDescriptor()->vehicle->computeForwardSpeed()*1.25f;
		hudMain->setCarInfo((int)dura,(int)100-(int)speeda,(int)getVehicleDescriptor()->vehicle->computeForwardSpeed()*2,(int)fuel,100-(int)getVehicleDescriptor()->vehicle->mDriveDynData.getEngineRotationSpeed()/15,true);
		/*	if(!hudActionUI->isActive() )
		hudActionUI->Activate();

		if(hudActionUI->isActive())
		{
		//hudActionUI->setText(L"", L"", InputMappingMngr->getKeyName(r3dInputMappingMngr::KS_INTERACT));
		int value = (int)dura/20;
		if (dura < 20.0f && dura > 0.0f)
		{
		value = 1;
		}
		hudActionUI->setCarInfo(value,(int)getVehicleDescriptor()->vehicle->computeForwardSpeed()*2,40,20,(int)fuel,true);
		hudActionUI->setScreenPos((int)r3dRenderer->ScreenW-180, (int)r3dRenderer->ScreenH-160);
		}*/
	}

	if (dura <= 0)
	{
		d_drive_vehiclescon->SetBool(false);
		if (!m_ParticleTracer)
			m_ParticleTracer = (obj_ParticleSystem*)srv_CreateGameObject("obj_ParticleSystem", "Fire_Large_01", GetPosition() );
		bOn = false;
		//m_ParticleTracer->SetNetworkID(500000);
		//FireNetID = m_ParticleTracer->GetNetworkID();
		//extime = r3dGetTime() + 5.0f;
	}
	//else
	//	d_drive_vehiclescon->SetBool(true);

	if (NetworkLocal)
	{
		if (fuel < 2)
			d_drive_vehiclescon->SetBool(false);
	}

	if (m_ParticleTracer)
	{
		m_ParticleTracer->SetPosition(GetPosition());
	}

	if (dura <= 0)
	{
		//SoundSys.Stop(gClientLogic().localPlayer_->m_sndVehicleDrive);
		int timeLeft = int(ceilf(extime - r3dGetTime()));
		if (timeLeft <= 0)
		{
			SoundSys.PlayAndForget(SoundSys.GetEventIDByPath("Sounds/Effects/Explosions/Bomb01"),GetPosition());
			/*	obj_Player* plr = gClientLogic().localPlayer_;
			PKT_C2S_CarKill_s n;
			n.targetId = plr->GetNetworkID();
			p2pSendToHost(plr, &n, sizeof(n));*/
			//plr->exitVehicle();
		}
	}

	if (getVehicleDescriptor()->vehicle->isInAir())
		falltime = r3dGetTime() + 1.0f;

	if (oldInAir && !getVehicleDescriptor()->vehicle->isInAir() && falltime > 3.0f)
	{
		SoundSys.PlayAndForget(SoundSys.GetEventIDByPath("Sounds/Vehicles/Crashes/Crash_Rock"),GetPosition());
		SendCrashSnd(SoundSys.GetEventIDByPath("Sounds/Vehicles/Crashes/Crash_Rock"));
		falltime = 0.0f;
	}
	float Speed = getVehicleDescriptor()->vehicle->computeForwardSpeed()*2;
	//float xSpeed = getVehicleDescriptor()->vehicle->computeForwardSpeed()*2-oldSpeed;
	if (oldSpeed > 10)
	{
		if (Speed < oldSpeed-10)
		{
			SoundSys.PlayAndForget(SoundSys.GetEventIDByPath("Sounds/Vehicles/Crashes/Crash_Metal"),GetPosition());
			SendCrashSnd(SoundSys.GetEventIDByPath("Sounds/Vehicles/Crashes/Crash_Metal"));
			dura -= oldSpeed/3.5f;
			//if (dura <= 0)
			//(obj_ParticleSystem*)srv_CreateGameObject("obj_ParticleSystem", "Fire_Large_01", GetPosition() );
		}
	}
	oldInAir = getVehicleDescriptor()->vehicle->isInAir();

	oldSpeed = getVehicleDescriptor()->vehicle->computeForwardSpeed()*2;
	//Font_Label->PrintF(r3dRenderer->ScreenW-80, 0, r3dColor(243,43,37), "FPS: %.1f", getVehicleDescriptor()->vehicle->computeForwardSpeed());

	// Zombie Kill
	ObjectManager& GW = GameWorld();
	float Maxdist=50;
	for (GameObject *targetObj = GW.GetFirstObject(); targetObj; targetObj = GW.GetNextObject(targetObj))
	{

		if(targetObj->isObjType(OBJTYPE_Zombie))
		{
			obj_Zombie* zombie = (obj_Zombie*)targetObj;
			if (!zombie->bDead)
			{
				float dist = (GetPosition() - targetObj->GetPosition()).Length();
				if(dist < 6 && Speed > 20)
				{
					zombie->DoDeath();
					SoundSys.PlayAndForget(SoundSys.GetEventIDByPath("Sounds/Vehicles/Crashes/Crash_Zombie"),GetPosition());
					SendCrashSnd(SoundSys.GetEventIDByPath("Sounds/Vehicles/Crashes/Crash_Zombie"));
					obj_Player* plr = gClientLogic().localPlayer_;
					PKT_C2S_CarKill_s n;
					n.targetId = targetObj->GetNetworkID();
					p2pSendToHost(plr, &n, sizeof(n));

				}
			}
		}
	}
	// Car Move Packet

	/*	CNetCellMover::moveData_s md;
	md.pos       = GetPosition();
	md.turnAngle = GetRotationVector().x;
	md.bendAngle = 0;
	md.state     = 0;
	PKT_C2C_MoveSetCell_s n1;
	PKT_C2C_MoveRel_s     n2;
	DWORD pktFlags = netMover.SendPosUpdate(md,&n1,&n2);
	if(pktFlags & 0x1)
	p2pSendToHost(this, &n1, sizeof(n1), true);
	if(pktFlags & 0x2)
	p2pSendToHost(this, &n2, sizeof(n2), false);
	*/
	if(NetworkLocal)
	{
		{
			PKT_C2C_CarSpeed_s n;
			n.speed = getVehicleDescriptor()->vehicle->computeForwardSpeed();
			n.rpm = getVehicleDescriptor()->vehicle->mDriveDynData.getEngineRotationSpeed();
			n.fuel = fuel;
			p2pSendToHost(this, &n, sizeof(n));
		}
		gClientLogic().localPlayer_->SetRotationVector(GetRotationVector());
		PKT_C2S_CarMove_s n;
		n.pos = GetPosition();
		n.angle = GetRotationVector();
		p2pSendToHost(this, &n, sizeof(n));
		/*PKT_C2C_CarStatus_s n1;
		n1.status = true;
		n1.fuel = fuel;
		p2pSendToHost(this,&n1,sizeof(n1));*/
	}
	else 
	{
		//UpdatePositionFromRemote();
	}

	return TRUE;
}

void obj_Vehicle::SendCrashSnd(int id)
{
	if (!NetworkLocal) return;

	PKT_C2S_CarSound_s n;
	n.Id = id;
	p2pSendToHost(this, &n, sizeof(n));
}
// Engine Sound
void obj_Vehicle::UpdateEngineSound()
{
	// ONLY SAME CAR!!!!!!!!!!!!!!!!
	//if (!EngineSnd) return;
	if (!gClientLogic().localPlayer_) return;
	//if (gClientLogic().localPlayer_->curcar == NULL) return;
	//if (gClientLogic().localPlayer_->curcar != this) return;
	//if (!status) return;

	if (!status)
	{
		if (SoundSys.isPlaying(EngineSnd))
			SoundSys.Stop(EngineSnd);
		return;
	}
	if (!SoundSys.IsHandleValid(EngineSnd))
	{
		if (EngineSnd)
			SoundSys.Release(EngineSnd);
		EngineSnd = NULL;
		EngineSnd = SoundSys.Play(SoundSys.GetEventIDByPath("Sounds/Vehicles/DuneBuggyEngineLoop"), GetPosition());
		SoundSys.Stop(EngineSnd);
		if (!SoundSys.IsHandleValid(EngineSnd)) return;
	}

	//else if(!SoundSys.isPlaying(EngineSnd))
	if (fuel < 2)
		SoundSys.Stop(EngineSnd);
	else
		if(!SoundSys.isPlaying(EngineSnd))
			SoundSys.Start(EngineSnd);


	float rpm1;

	if (gClientLogic().localPlayer_->isDriving() && gClientLogic().localPlayer_->curcar == this && d_drive_vehicles->GetBool())
		rpm1 = R3D_MIN(vd->vehicle->mDriveDynData.getEngineRotationSpeed()*5.33f, 8000.0f);
	else
		rpm1 = R3D_MIN(rpm*5.33f , 8000.0f);

	if (SoundSys.isPlaying(EngineSnd) && SoundSys.IsHandleValid(EngineSnd))
		SoundSys.SetParamValue(EngineSnd,"rpm",rpm1);
	//SoundSys.SetSoundPos(EngineSnd,GetPosition());
	SoundSys.Set3DAttributes(EngineSnd, &GetPosition(),0,0);
	//}
}
void obj_Vehicle::UpdateBrake()
{
	// for local car
	if (!gClientLogic().localPlayer_) return;
	if (gClientLogic().localPlayer_->curcar == NULL) return;
	if (gClientLogic().localPlayer_->curcar != this) return;
	if (!NetworkLocal) return;

    if (!SoundSys.IsHandleValid(BreakSnd))
	{
		if (BreakSnd)
			SoundSys.Release(BreakSnd);
		BreakSnd = NULL;
		BreakSnd = SoundSys.Play(SoundSys.GetEventIDByPath("Sounds/Vehicles/Braking/SkidLoop_AllSurfaces"), GetPosition());
		SoundSys.Stop(BreakSnd);
		if (!SoundSys.IsHandleValid(BreakSnd)) return;
	}

    if (!SoundSys.isPlaying(BreakSnd))
    SoundSys.Start(BreakSnd);

	// work only driver
	/*if (g_pPhysicsWorld->m_VehicleManager->carControlData.getDigitalBrake())
		SoundSys.SetParamValue(BreakSnd,"Skid_Amount",R3D_MIN(vd->vehicle->computeForwardSpeed()/40,1.0f));
	else // if not brake set Skid_Amount to 0.0f for stop sound.!
	SoundSys.SetParamValue(BreakSnd,"Skid_Amount",0.0f);*/

	SoundSys.SetParamValue(BreakSnd,"Skid_Amount",100.0f);
	// not forgot set Concrete param.
	SoundSys.SetParamValue(BreakSnd,"Concrete_Surface_Amount",100.0f);
	// and not forgot set 3d position it very important!
	SoundSys.Set3DAttributes(BreakSnd, &GetPosition(),0,0);
}
void obj_Vehicle::UpdateTraction()
{
	// ONLY LOCAL CAR!!!!!!!!!!!!!!!!
	if (!gClientLogic().localPlayer_) return;
	if (gClientLogic().localPlayer_->curcar == NULL) return;
	if (gClientLogic().localPlayer_->curcar != this) return;
	if (!NetworkLocal) return;

	PhysicsCallbackObject* target = NULL;
	const MaterialType *mt = 0;

	PxRaycastHit hit;
	PxSceneQueryFilterData filter(PxFilterData(COLLIDABLE_STATIC_MASK,0,0,0), PxSceneQueryFilterFlags(PxSceneQueryFilterFlag::eSTATIC|PxSceneQueryFilterFlag::eDYNAMIC));
	bool hitResult = g_pPhysicsWorld->raycastSingle(PxVec3(GetPosition().x, GetPosition().y + 0.5f, GetPosition().z), PxVec3(0, -1, 0), 1.0f, PxSceneQueryFlags(PxSceneQueryFlag::eIMPACT), hit, filter);
	if( hitResult )
	{
		if( hit.shape && (target = static_cast<PhysicsCallbackObject*>(hit.shape->getActor().userData)))
		{
			r3dMaterial* material = 0;
			GameObject *gameObj = target->isGameObject();
			if(gameObj)
			{
				if( gameObj->isObjType( OBJTYPE_Mesh ) )
					material = static_cast< MeshGameObject* > ( target )->MeshLOD[ 0 ]->GetFaceMaterial( hit.faceIndex );
				if(gameObj->isObjType(OBJTYPE_Terrain))
				{
					mt = Terrain->GetMaterialType(GetPosition());
				}
			}
			else if(target->hasMesh())
			{
				material = target->hasMesh()->GetFaceMaterial( hit.faceIndex );
			}
			if (!material)
				material = target->GetMaterial(hit.faceIndex);

			if(material && !mt) // get material type from mesh material
			{
				mt = g_pMaterialTypes->Get(r3dHash::MakeHash(material->TypeName));
			}

		}
		if (!mt || mt->VehiclesSound<0) // if no mt stop it.
		{
			if (SoundSys.isPlaying(footStepsSnd))
				SoundSys.Stop(footStepsSnd);
			return;
		}
		int fts = currentFootStepsSoundID; 
		if(mt )
		{
			if(mt->VehiclesSound>=0)
				fts = mt->VehiclesSound;
		}
		else if (SoundSys.isPlaying(footStepsSnd))
			SoundSys.Stop(footStepsSnd);

		// for currentFootStepsSoundID != fts but sound is playing i will stop and start sound.

		if (!SoundSys.IsHandleValid(footStepsSnd) || fts != currentFootStepsSoundID && mt)
		{
			SoundSys.Stop(footStepsSnd);
			footStepsSnd = NULL;
			if(footStepsSnd == NULL)
			{
				footStepsSnd = SoundSys.Play(fts, GetPosition());
				if (!SoundSys.IsHandleValid(footStepsSnd))
					return;
			}
			currentFootStepsSoundID = fts;
		}

		if (mt)
		{
			if (SoundSys.GetState(footStepsSnd) != FMOD_EVENT_STATE_PLAYING && !SoundSys.isPlaying(footStepsSnd))
				SoundSys.Start(footStepsSnd);
		}
		else if (SoundSys.isPlaying(footStepsSnd)) // for playing sound but no mt at now.
			SoundSys.Stop(footStepsSnd);

		if (SoundSys.isPlaying(footStepsSnd) && mt && SoundSys.IsHandleValid(footStepsSnd))
			SoundSys.SetParamValue(footStepsSnd,"speed",R3D_MIN(getVehicleDescriptor()->vehicle->computeForwardSpeed()*2.0f,65.0f));
	}
	else if (SoundSys.isPlaying(footStepsSnd))
		SoundSys.Stop(footStepsSnd);

	if (SoundSys.isPlaying(footStepsSnd) && mt && SoundSys.IsHandleValid(footStepsSnd))
		SoundSys.Set3DAttributes(footStepsSnd, &GetPosition(),0,0);
}

BOOL obj_Vehicle::Load(const char *fname)
{
	r3dOutToLog("LoadVehicles\n");
	const char* cpMeshName = "Data\\ObjectsDepot\\Vehicles\\drivable_buggy_02.sco";

	if(!parent::Load(cpMeshName)) 
		return FALSE;

	return TRUE;
}
void obj_Vehicle::LightOnOff()
{
	if (bOn)
	{
		Light->LT.Intensity = 4.0f;
		Light->innerRadius = 30.0f;
		Light->outerRadius = 40.0f;
	}
	else
	{
		Light->LT.Intensity = 0.0f;
		Light->innerRadius = 0.0f;
		Light->outerRadius = 0.0f;
	}
}
/*r3dMesh* obj_Vehicle::GetObjectMesh() 
{
	return m_PrivateModel;
}

/*virtual
r3dMesh* obj_Vehicle::GetObjectLodMesh()
{
	return m_PrivateModel;
}*/
BOOL obj_Vehicle::OnCreate()
{
	m_ActionUI_Title = gLangMngr.getString("Vehicles");
	m_ActionUI_Msg = gLangMngr.getString("Hold E To Enter Vehicles");

	DisablePhysX = false;
	ReadPhysicsConfig();
	PhysicsConfig.group = PHYSCOLL_PLAYER_ONLY_GEOMETRY;

	// perform our own movement to sync over network properly
	//PhysicsConfig.isKinematic = true; 
	//PhysicsConfig.isDynamic = true;
	//PhysicsConfig.requireNoBounceMaterial = true;
	//PhysicsConfig.isFastMoving = false;
	if (!parent::OnCreate())
		//PhysicsConfig.group = PHYSCOLL_TINY_GEOMETRY; // skip collision with players
		//PhysicsConfig.requireNoBounceMaterial = true;
		//PhysicsConfig.isFastMoving = false;
	if (!parent::OnCreate())
		return FALSE;

	r3dMesh *m = MeshLOD[0];
	//m_PrivateModel = r3dGOBAddMesh("Data\\ObjectsDepot\\Vehicles\\drivable_buggy_02.sco", true, false, true, true );
	if (!m)
		return FALSE;

	vd = g_pPhysicsWorld->m_VehicleManager->CreateVehicle(m);
	if (vd)
	{
		//	Set position and orientation for car
		SwitchToDrivable(d_drive_vehicles->GetBool());
		SyncPhysicsPoseWithObjectPose();
		r3dBoundBox bb = GetBBoxLocal();
		std::swap(bb.Size.x, bb.Size.z);
		std::swap(bb.Org.x, bb.Org.z);
		SetBBoxLocal(bb);
		vd->owner = this;
	}

	//SetBBoxLocal(GetObjectMesh()->localBBox);

	netMover.Teleport(GetPosition());

	dura = 100.0f;
	RemoteSpeed = 0.0f;
	//extime = 0.0f;
	//(obj_ParticleSystem*)srv_CreateGameObject("obj_ParticleSystem", "Fire_Large_01", GetPosition() );
	Light = (obj_LightHelper*)srv_CreateGameObject("obj_LightHelper", "Spot", GetPosition ());
	Light->SetRotationVector(GetRotationVector());
	//Light->m_pPreset = m_pPreset;
	Light->SetPosition(GetBBoxWorld().Center());
	Light->Color = r3dColor::white;;
	/*Light->LT.bDiffuseOnly = 0;
	Light->LT.bCastShadows = 0;
	Light->LT.bDiffuseOnly = 0;
	Light->LT.bDiffuseOnly = 0;*/
	/*	Light->LT.Intensity = 4.0f;
	Light->bOn = true;
	Light->innerRadius = 30.0f;
	Light->outerRadius = 40.0f;*/
	Light->LT.Intensity = 0.0f;
	Light->bOn = true;
	Light->innerRadius = 0.0f;
	Light->outerRadius = 0.0f;
	Light->bKilled = false;

	//Light->bSerialized = true;

	//bOn = false;

	m_ParticleTracer = NULL;
	m_ParticleBoostSmoke = NULL;
	//fuel = 100;

	// Sound

	//EngineSnd = NULL;


	//m_ParticleBoostSmoke = (obj_ParticleSystem*)srv_CreateGameObject("obj_ParticleSystem", "vehicle_damage_01", GetPosition() );

	EngineSnd = SoundSys.Play(SoundSys.GetEventIDByPath("Sounds/Vehicles/DuneBuggyEngineLoop"), GetPosition());
	SoundSys.Stop(EngineSnd);

	BreakSnd = SoundSys.Play(SoundSys.GetEventIDByPath("Sounds/Vehicles/Braking/SkidLoop_AllSurfaces"), GetPosition());
	SoundSys.Stop(BreakSnd);

	UpdateTransform();
	//SoundSys.Start(BreakSnd);
	//r3d_assert(BreakSnd);

	return vd != 0;
}

//////////////////////////////////////////////////////////////////////////

BOOL obj_Vehicle::OnDestroy()
{
	if (m_ParticleTracer)
		m_ParticleTracer = NULL;
	if (m_ParticleBoostSmoke)
		m_ParticleBoostSmoke = NULL;
	if (Light)
		Light = NULL;

	if (vd)
		vd->owner = 0;
	return parent::OnDestroy();
}

//////////////////////////////////////////////////////////////////////////

void obj_Vehicle::SetPosition(const r3dPoint3D& pos)
{
	parent::SetPosition(pos);
	SyncPhysicsPoseWithObjectPose();
}

//////////////////////////////////////////////////////////////////////////

void obj_Vehicle::SetRotationVector(const r3dVector& Angles)
{
	parent::SetRotationVector(Angles);
	SyncPhysicsPoseWithObjectPose();
}

//////////////////////////////////////////////////////////////////////////

void obj_Vehicle::SwitchToDrivable(bool doDrive)
{	if (vd && vd->vehicle->getRigidDynamicActor())
{
	vd->vehicle->getRigidDynamicActor()->setRigidDynamicFlag(PxRigidDynamicFlag::eKINEMATIC, !doDrive);
	if (doDrive)
		g_pPhysicsWorld->m_VehicleManager->DriveCar(vd);
}
}

//////////////////////////////////////////////////////////////////////////
void obj_Vehicle::SyncPhysicsPoseWithObjectPose()
{
	if (!vd)
		return;

	PxRigidDynamicFlags f = vd->vehicle->getRigidDynamicActor()->getRigidDynamicFlags();
	if (!(f & PxRigidDynamicFlag::eKINEMATIC))
		return;

	r3dPoint3D pos(GetPosition());
	D3DXMATRIX rotM(GetRotationMatrix());
	D3DXQUATERNION q;
	D3DXQuaternionRotationMatrix(&q, &rotM);
	PxQuat quat(q.x, q.y, q.z, q.w);

	PxTransform carPose(PxVec3(pos.x, pos.y, pos.z), quat);
	vd->vehicle->getRigidDynamicActor()->setGlobalPose(carPose);
}

//////////////////////////////////////////////////////////////////////////

void obj_Vehicle::SetBoneMatrices()
{
	if (!vd)
		return;

	PxRigidDynamic *a = vd->vehicle->getRigidDynamicActor();
	D3DXMATRIX boneTransform;

	static const UsefulTransforms T;

	//	Init with identities
	for (int i = 0; i < vd->skl->NumBones; ++i)
	{
		r3dBone &b = vd->skl->Bones[i];
		b.CurrentTM = T.rotY_mat;
	}

	//	Retrieve vehicle wheels positions
	PxShape *shapes[VEHICLE_PARTS_COUNT] = {0};
	PxU32 sn = a->getShapes(shapes, VEHICLE_PARTS_COUNT);
	for (PxU32 i = 0; i < sn; ++i)
	{
		PxShape *s = shapes[i];
		PxU32 boneIndex = reinterpret_cast<PxU32>(s->userData);
		r3dBone &b = vd->skl->Bones[boneIndex];
		PxTransform pose = s->getLocalPose();

		PxVec3 bonePos = PxVec3(b.vRelPlacement.x, b.vRelPlacement.y, b.vRelPlacement.z);
		D3DXMATRIX toOrigin, fromOrigin, suspensionOffset;
		D3DXMatrixTranslation(&toOrigin, -bonePos.x, -bonePos.y, -bonePos.z);

		PxVec3 bonePosNew = T.rotY_quat.rotate(bonePos);
		D3DXMatrixTranslation(&fromOrigin, pose.p.x, pose.p.y, pose.p.z);

		pose.q = pose.q * T.rotY_quat;

		D3DXQUATERNION q(pose.q.x, pose.q.y, pose.q.z, pose.q.w);
		D3DXMatrixRotationQuaternion(&boneTransform, &q);

		D3DXMatrixMultiply(&boneTransform, &toOrigin, &boneTransform);
		D3DXMatrixMultiply(&boneTransform, &boneTransform, &fromOrigin);

		b.CurrentTM = boneTransform;
	}
	vd->skl->SetShaderConstants();
}

void obj_Vehicle::UpdatePositionFromRemote()
{
	r3d_assert(!NetworkLocal);

	const float fTimePassed = r3dGetFrameTime();

	r3dVector currentRotation = GetRotationVector();
	float rotX      = currentRotation.x;
	float turnAngle = netMover.NetData().turnAngle;

	if(fabs(rotX - turnAngle) > 0.01f) 
	{
		extern float getMinimumAngleDistance(float from, float to);
		float f1 = getMinimumAngleDistance(rotX, turnAngle);
		rotX += ((f1 < 0) ? -1 : 1) * fTimePassed * 360;
		float f2 = getMinimumAngleDistance(rotX, turnAngle);
		if((f1 > 0 && f2 <= 0) || (f1 < 0 && f2 >= 0))
			rotX = turnAngle;

		currentRotation.x = rotX;
		SetRotationVector( currentRotation );
	}


	if(netVelocity.LengthSq() > 0.0001f)
	{
		SetPosition(GetPosition() + netVelocity * fTimePassed);

		// check if we overmoved to target position
		r3dPoint3D v = netMover.GetNetPos() - GetPosition();
		float d = netVelocity.Dot(v);
		if(d < 0) {
			SetPosition(netMover.GetNetPos());
			netVelocity = r3dPoint3D(0, 0, 0);
		}
	}

}
//////////////////////////////////////////////////////////////////////////

extern PxF32 &gVehicleTireGroundFriction;

#ifndef FINAL_BUILD
//#define EXTENDED_VEHICLE_CONFIG
float obj_Vehicle::DrawPropertyEditor(float scrx, float scry, float scrw, float scrh, const AClass* startClass, const GameObjects& selected)
{
	float y = scry + parent::DrawPropertyEditor(scrx, scry, scrw, scrh, startClass, selected );

	if( !IsParentOrEqual( &ClassData, startClass ) || !vd)
		return y;

	y += 10.0f;
	y += imgui_Static(scrx, y, "Vehicle physics configuration");
#ifdef EXTENDED_VEHICLE_CONFIG
	y += imgui_Value_Slider(scrx, y, "Friction", &gVehicleTireGroundFriction, 0.5f, 8.0f, "%-02.2f");

	PxVehicleAckermannGeometryData &ad = vd->ackermannData;
	y += 5.0f;
	y += imgui_Static(scrx, y, "Chassis:");
	y += imgui_Value_Slider(scrx, y, "Mass", &vd->chassisData.mMass, 100.0f, 5000.0f, "%-02.2f");
	y += imgui_Value_Slider(scrx, y, "Ackerman accuracy", &ad.mAccuracy, 0.0f, 1.0f, "%-02.2f");
#endif

	PxVehicleEngineData &ed = vd->engineData;
	y += 5.0f;
	y += imgui_Static(scrx, y, "Engine:");
	y += imgui_Value_Slider(scrx, y, "Peak torque", &ed.mPeakTorque, 100.0f, 5000.0f, "%-02.f");
	float x = ed.mMaxOmega / 0.104719755f;
	y += imgui_Value_Slider(scrx, y, "Max RPM", &x, 0.0f, 15000.0f, "%-02.0f");
	ed.mMaxOmega = x * 0.104719755f;

#ifdef EXTENDED_VEHICLE_CONFIG
	y += 5.0f;
	y += imgui_Static(scrx, y, "Gears:");
	PxVehicleGearsData &gd = vd->gearsData;
	y += imgui_Value_Slider(scrx, y, "Switch time", &gd.mSwitchTime, 0.0f, 3.0f, "%-02.2f");

	PxVehicleDifferential4WData &dd = vd->diffData;
	y += 5.0f;
	y += imgui_Static(scrx, y, "Differential:");
	y += imgui_Value_Slider(scrx, y, "Front-rear split", &dd.mFrontRearSplit, 0.0f, 1.0f, "%-02.3f");
#endif
	y += 10.0f;
	y += imgui_Static(scrx, y, "Select wheel:");

	static int currentWheel = 2;
	if (imgui_Button(scrx, y, 80.0f, 30.0f, "Front-Left", currentWheel == 2))
		currentWheel = 2;

	if (imgui_Button(scrx + 80, y, 80.0f, 30.0f, "Front-Right", currentWheel == 3))
		currentWheel = 3;

	if (imgui_Button(scrx + 160, y, 80.0f, 30.0f, "Rear-Left", currentWheel == 0))
		currentWheel = 0;

	if (imgui_Button(scrx + 240, y, 80.0f, 30.0f, "Rear-Right", currentWheel == 1))
		currentWheel = 1;

	y += 30.0f;

	VehicleDescriptor::WheelData &wd = vd->wheelsData[currentWheel];

	y += 5.0f;
#ifdef EXTENDED_VEHICLE_CONFIG
	y += imgui_Value_Slider(scrx, y, "Mass", &wd.wheelData.mMass, 1.0f, 100.0f, "%-02.3f");
	y += imgui_Value_Slider(scrx, y, "Spring max compression", &wd.suspensionData.mMaxCompression, 0.0f, 2.0f, "%-02.3f");
	y += imgui_Value_Slider(scrx, y, "Spring max droop", &wd.suspensionData.mMaxDroop, 0.0f, 2.0f, "%-02.3f");
#endif
	y += imgui_Value_Slider(scrx, y, "Break torque", &wd.wheelData.mMaxBrakeTorque, 0.0f, 25000.0f, "%-02.0f");
	float f = R3D_RAD2DEG(wd.wheelData.mMaxSteer);
	y += imgui_Value_Slider(scrx, y, "Steer angle", &f, 0.0f, 90.0f, "%-02.2f");
	wd.wheelData.mMaxSteer = R3D_DEG2RAD(f);
	if (currentWheel >= 2)
	{
		vd->wheelsData[(currentWheel + 1) % 2 + 2].wheelData.mMaxSteer = wd.wheelData.mMaxSteer;
	}
	float z = R3D_RAD2DEG(wd.wheelData.mToeAngle);
	wd.wheelData.mToeAngle = R3D_DEG2RAD(z);
#ifdef EXTENDED_VEHICLE_CONFIG
	y += imgui_Value_Slider(scrx, y, "Spring strength", &wd.suspensionData.mSpringStrength, 10000.0f, 50000.0f, "%-05.0f");
#endif
	y += imgui_Value_Slider(scrx, y, "Spring damper", &wd.suspensionData.mSpringDamperRate, 500.0f, 9000.0f, "%-02.0f");

	y += 10.0f;
	if (imgui_Button(scrx, y, 360.0f, 22.0f, "Save Vehicle Data"))
	{
		vd->Save(0);
	}
	y += 22.0f;

	vd->ConfigureVehicleSimulationData();
	return y - scry;
}
#endif

//////////////////////////////////////////////////////////////////////////

const bool obj_Vehicle::getExitSpace( r3dVector& outVector, int exitIndex )
{
	return vd->GetExitIndex( outVector,exitIndex);
}



BOOL obj_Vehicle::OnNetReceive(DWORD EventID, const void* packetData, int packetSize)
{
	r3d_assert(!(ObjFlags & OBJFLAG_JustCreated)); // make sure that object was actually created before processing net commands

	switch(EventID)
	{
	default: return FALSE;

	case PKT_C2C_MoveSetCell:
		{
			const PKT_C2C_MoveSetCell_s& n = *(PKT_C2C_MoveSetCell_s*)packetData;
			r3d_assert(packetSize == sizeof(n));

			OnNetPacket(n);
			break;
		}
	case PKT_C2C_MoveRel:
		{
			const PKT_C2C_MoveRel_s& n = *(PKT_C2C_MoveRel_s*)packetData;
			r3d_assert(packetSize == sizeof(n));

			OnNetPacket(n);
			break;
		}
	case PKT_C2S_CarMove:
		{
			const PKT_C2S_CarMove_s& n = *(PKT_C2S_CarMove_s*)packetData;
			r3d_assert(packetSize == sizeof(n));

			OnNetPacket(n);
			break;
		}
	case PKT_C2C_CarStatusSv:
		{
			const PKT_C2C_CarStatusSv_s& n = *(PKT_C2C_CarStatusSv_s*)packetData;
			r3d_assert(packetSize == sizeof(n));

			OnNetPacket(n);
			break;
		}
	case PKT_C2C_CarFlashLight:
		{
			const PKT_C2C_CarFlashLight_s& n = *(PKT_C2C_CarFlashLight_s*)packetData;
			r3d_assert(packetSize == sizeof(n));

			OnNetPacket(n);
			break;
		}
	case PKT_C2C_CarSpeed:
		{
			const PKT_C2C_CarSpeed_s& n = *(PKT_C2C_CarSpeed_s*)packetData;
			r3d_assert(packetSize == sizeof(n));

			OnNetPacket(n);
			break;
		}
		case PKT_C2S_CarSound:
		{
		const PKT_C2S_CarSound_s& n = *(PKT_C2S_CarSound_s*)packetData;
		r3d_assert(packetSize == sizeof(n));

		OnNetPacket(n);
		break;
		}
	}

	return TRUE;
}
void obj_Vehicle::OnNetPacket(const PKT_C2C_CarStatusSv_s& n)
{
	status = n.status;
}
void obj_Vehicle::OnNetPacket(const PKT_C2S_CarSound_s& n)
{
	//if (NetworkLocal) return;
	if (gClientLogic().localPlayer_->isDriving() && gClientLogic().localPlayer_->curcar == this) return;

	SoundSys.PlayAndForget(n.Id,GetPosition());
}
void obj_Vehicle::OnNetPacket(const PKT_C2C_CarFlashLight_s& n)
{
	if (NetworkLocal) return;
	bOn = n.bOn;
	LightOnOff();
}
/*void obj_Vehicle::OnNetPacket(const PKT_C2C_CarStatus_s& n)
{
status = n.status;
if (!NetworkLocal)
fuel = n.fuel;
}*/
void obj_Vehicle::OnNetPacket(const PKT_C2S_CarMove_s& n)
{
	if (NetworkLocal) return;

	//r3dOutToLog ("CarMoveReq\n");
	SetPosition(n.pos);
	SetRotationVector(n.angle);
}
void obj_Vehicle::OnNetPacket(const PKT_C2C_CarSpeed_s& n)
{
	if (NetworkLocal) return;
	RemoteSpeed = n.speed;
	rpm = n.rpm;
	fuel = n.fuel;
}
void obj_Vehicle::OnNetPacket(const PKT_C2C_MoveSetCell_s& n)
{

	// HACK, we have no knowledge if this is network local on the server yet. 
	if ( !NetworkLocal) 
	{
		netMover.SetCell(n);
	}
}

void obj_Vehicle::OnNetPacket(const PKT_C2C_MoveRel_s& n)
{

	// HACK, we have no knowledge if this is network local on the server yet. 
	if ( !NetworkLocal) 
	{
		const CNetCellMover::moveData_s& md = netMover.DecodeMove(n);

		// calc velocity to reach position on time for next update
		r3dPoint3D vel = netMover.GetVelocityToNetTarget(
			GetPosition(),
			/*GPP->UAV_FLY_SPEED_V **/ 1.5f,
			1.0f);

		netVelocity = vel;
	}
}


#endif // VEHICLES_ENABLED