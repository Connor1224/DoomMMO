#include "r3dPCH.h"
#include "r3dProtect.h"

#include "HUDDisplay.h"

#include "ObjectsCode/Gameplay/BasePlayerSpawnPoint.h"
#include "ObjectsCode/Gameplay/obj_Grave.h"
#include "ObjectsCode/Gameplay/obj_SafeLock.h"
#include "../multiplayer/clientgamelogic.h"
#include "../ObjectsCode/ai/AI_Player.H"
#include "../ObjectsCode/weapons/Weapon.h"
#include "../ObjectsCode/weapons/WeaponArmory.h"
#include "../multiplayer/MasterServerLogic.h"

#include "HUDPause.h"
//#include "HUDSafeLock.h"

extern HUDPause*	hudPause;
extern HUDDisplay* hudMain;
//extern HUDSafeLock* hudSafeLock;

struct NameHashFunc_T
{
	inline int operator () ( const char * szKey )
	{
		return r3dHash::MakeHash( szKey );
	}
};
static HashTableDynamic<const char*, FixedString256, NameHashFunc_T, 1024> dictionaryHash_;


HUDDisplay :: HUDDisplay()
{
	Inited = false;
	chatVisible = false;
	chatInputActive = false;
	chatVisibleUntilTime = 0;
	lastChatMessageSent = 0;
	playersListVisible = false;
	bloodAlpha = 0.0f;
	writeNoteSavedSlotIDFrom = 0;
	timeoutForNotes = 0;
	timeoutNoteReadAbuseReportedHideUI = 0;
	RangeFinderUIVisible = false;

	if(dictionaryHash_.Size() == 0)
	{
		r3dFile* f = r3d_open( "Data/LangPack/dictionary.txt", "rb" );
		if (!f->IsValid())
		{
			f = r3d_open( "Data/LangPack/dictionary.dat", "rb" );
		}

		if (f->IsValid())
		{
			char tmpStr[256];
			while(fgets(tmpStr, 256, f) != NULL)
			{
				size_t len = strlen(tmpStr);
				for(size_t i=0; i<len; ++i)
				{
					if(tmpStr[i]==13 || tmpStr[i]==10)
						tmpStr[i]=0;
				}
				dictionaryHash_.Add(tmpStr, tmpStr);
			}
			fclose(f);
		}
	}
}

HUDDisplay :: ~HUDDisplay()
{
	dictionaryHash_.Clear();
}

bool HUDDisplay::Init()
{
	isGroupLeaderActive = false;
	strcpy(GametagName,"");
	TimeToWaitGroup=0.0f;
	TimeLeaveGroup=1.0f; // Seconds for show Leave Button, 300 = 5 Minutes

	if(!gfxHUD.Load("Data\\Menu\\WarZ_HUD.swf", true))
		return false;
	if(!gfxBloodStreak.Load("Data\\Menu\\WarZ_BloodStreak.swf", false))
		return false;
	if(!gfxRangeFinder.Load("Data\\Menu\\WarZ_HUD_RangeFinder.swf", false))
		return false;

	gfxHUD.SetCurentRTViewport( Scaleform::GFx::Movie::SM_ExactFit );
	gfxBloodStreak.SetCurentRTViewport(Scaleform::GFx::Movie::SM_ExactFit);
	gfxRangeFinder.SetCurentRTViewport(Scaleform::GFx::Movie::SM_ExactFit);

#define MAKE_CALLBACK(FUNC) new r3dScaleformMovie::TGFxEICallback<HUDDisplay>(this, &HUDDisplay::FUNC)
	gfxHUD.RegisterEventHandler("eventChatMessage", MAKE_CALLBACK(eventChatMessage));
	gfxHUD.RegisterEventHandler("eventNoteWritePost", MAKE_CALLBACK(eventNoteWritePost));
	gfxHUD.RegisterEventHandler("eventNoteClosed", MAKE_CALLBACK(eventNoteClosed));
	gfxHUD.RegisterEventHandler("eventGraveNoteClosed", MAKE_CALLBACK(eventGraveNoteClosed));
	gfxHUD.RegisterEventHandler("eventSafelockPass", MAKE_CALLBACK(eventSafelockPass));
	gfxHUD.RegisterEventHandler("eventNoteReportAbuse", MAKE_CALLBACK(eventNoteReportAbuse));
	gfxHUD.RegisterEventHandler("eventShowPlayerListContextMenu", MAKE_CALLBACK(eventShowPlayerListContextMenu));
	gfxHUD.RegisterEventHandler("eventPlayerListAction", MAKE_CALLBACK(eventPlayerListAction));

	{
		Scaleform::GFx::Value var[4];
		var[0].SetInt(0);
		var[1].SetString("$HUD_Chat_Channel1");
		var[2].SetBoolean(true);
		var[3].SetBoolean(true);
		gfxHUD.Invoke("_root.api.setChatTab", var, 4);
 		var[0].SetInt(1);
		var[1].SetString("$HUD_Chat_Channel2");
		var[2].SetBoolean(true);
		var[3].SetBoolean(true);
		gfxHUD.Invoke("_root.api.setChatTab", var, 4);
		var[0].SetInt(2);
		var[1].SetString("$HUD_Chat_Channel3");
		var[2].SetBoolean(true);
		var[3].SetBoolean(true);
		gfxHUD.Invoke("_root.api.setChatTab", var, 4);
		var[0].SetInt(3);
		var[1].SetString("$HUD_Chat_Channel4");
		var[2].SetBoolean(true);
		var[3].SetBoolean(true);
		gfxHUD.Invoke("_root.api.setChatTab", var, 4);

		currentChatChannel = 0;
		var[0].SetInt(0);
		gfxHUD.Invoke("_root.api.setChatTabActive", var, 1);
	}

	setChatTransparency(R3D_CLAMP(g_ui_chat_alpha->GetFloat()/100.0f, 0.0f, 1.0f));

	Inited = true;

	weaponInfoVisible = -1;
	SafeZoneWarningVisible = false;

	int numGames = (int)gMasterServerLogic.games_.size();
	for(int i=0; i<numGames; i++) 
    {
        const GBPKT_M2C_GameData_s& gd = gMasterServerLogic.games_[i];
        const GBGameInfo& ginfo = gd.info;

		if (gClientLogic().m_gameInfo.gameServerId == ginfo.gameServerId)
		{
		
		//r3dOutToLog("######## ginfo.MapSettings %d\n",ginfo.MapSettings);
	if (strcmp(ginfo.MapSettings,"6") == 0)
    {
        addChatMessage(0,"<SYSTEM>","You have joined private server",0);
		if (ginfo.enableCrosshair)
			addChatMessage(0,"<SYSTEM>","The Crosshair is Enabled on this server",0);
		else
			addChatMessage(0,"<SYSTEM>","The Crosshair is disabled on this server",0);
		if (ginfo.enableSnipers)
			addChatMessage(0,"<SYSTEM>","The Snipers is Enabled on this server",0);
		else
			addChatMessage(0,"<SYSTEM>","The Snipers is disabled on this server",0);
		return true;
    }
    else if (strcmp(ginfo.MapSettings,"0") == 0)
    {
        addChatMessage(0,"<SYSTEM>","You have joined officials server",0);
		if (ginfo.enableCrosshair)
			addChatMessage(0,"<SYSTEM>","The Crosshair is Enabled on this server",0);
		else
			addChatMessage(0,"<SYSTEM>","The Crosshair is disabled on this server",0);
		if (ginfo.enableSnipers)
			addChatMessage(0,"<SYSTEM>","The Snipers is Enabled on this server",0);
		else
			addChatMessage(0,"<SYSTEM>","The Snipers is disabled on this server",0);
		return true;
    }
    else if (strcmp(ginfo.MapSettings,"2") == 0)
    {
        addChatMessage(0,"<SYSTEM>","You have joined Premium server",0);
		if (ginfo.enableCrosshair)
			addChatMessage(0,"<SYSTEM>","The Crosshair is Enabled on this server",0);
		else
			addChatMessage(0,"<SYSTEM>","The Crosshair is disabled on this server",0);
		if (ginfo.enableSnipers)
			addChatMessage(0,"<SYSTEM>","The Snipers is Enabled on this server",0);
		else
			addChatMessage(0,"<SYSTEM>","The Snipers is disabled on this server",0);
		return true;
    }
    else if (strcmp(ginfo.MapSettings,"1") == 0)
    {
        addChatMessage(0,"<SYSTEM>","You have joined Public server",0);
		if (ginfo.enableCrosshair)
			addChatMessage(0,"<SYSTEM>","The Crosshair is Enabled on this server",0);
		else
			addChatMessage(0,"<SYSTEM>","The Crosshair is disabled on this server",0);
		if (ginfo.enableSnipers)
			addChatMessage(0,"<SYSTEM>","The Snipers is Enabled on this server",0);
		else
			addChatMessage(0,"<SYSTEM>","The Snipers is disabled on this server",0);
		return true;
    }
    else if (strcmp(ginfo.MapSettings,"4") == 0)
    {
        addChatMessage(0,"<SYSTEM>","You have joined StrongHolds server",0);
		if (ginfo.enableCrosshair)
			addChatMessage(0,"<SYSTEM>","The Crosshair is Enabled on this server",0);
		else
			addChatMessage(0,"<SYSTEM>","The Crosshair is disabled on this server",0);
		if (ginfo.enableSnipers)
			addChatMessage(0,"<SYSTEM>","The Snipers is Enabled on this server",0);
		else
			addChatMessage(0,"<SYSTEM>","The Snipers is disabled on this server",0);
		return true;
    }
	else if (strcmp(ginfo.MapSettings,"5") == 0)
    {
        addChatMessage(0,"<SYSTEM>","You have joined Veteran server",0);
		if (ginfo.enableCrosshair)
			addChatMessage(0,"<SYSTEM>","The Crosshair is Enabled on this server",0);
		else
			addChatMessage(0,"<SYSTEM>","The Crosshair is disabled on this server",0);
		if (ginfo.enableSnipers)
			addChatMessage(0,"<SYSTEM>","The Snipers is Enabled on this server",0);
		else
			addChatMessage(0,"<SYSTEM>","The Snipers is disabled on this server",0);
		return true;
    }
	else if (strcmp(ginfo.MapSettings,"3") == 0)
    {
        addChatMessage(0,"<SYSTEM>","You have joined Trial server",0);
		if (ginfo.enableCrosshair)
			addChatMessage(0,"<SYSTEM>","The Crosshair is Enabled on this server",0);
		else
			addChatMessage(0,"<SYSTEM>","The Crosshair is disabled on this server",0);
		if (ginfo.enableSnipers)
			addChatMessage(0,"<SYSTEM>","The Snipers is Enabled on this server",0);
		else
			addChatMessage(0,"<SYSTEM>","The Snipers is disabled on this server",0);
		return true;
    }
		}
	}
	return true;
}

bool HUDDisplay::Unload()
{
	gfxHUD.Unload();
	gfxBloodStreak.Unload();
	gfxRangeFinder.Unload();

	Inited = false;
	return true;
}

void HUDDisplay::setCarInfo(int var1 , int var2 , int var3 ,int var4, int var5,int var6 , bool show)
{
	if(!Inited) return;
	Scaleform::GFx::Value var[5];
	var[0].SetInt(var1); // Damage
	var[1].SetInt(100-var2); // RPM
	var[2].SetInt(var3); // SPEED
	var[3].SetInt(var4); // Gasoline
	var[4].SetInt(100-var5); // RotationSpeed
	Scaleform::GFx::Value seat[2];
	if (gClientLogic().localPlayer_->isInVehicle())
		seat[0].SetInt(0); // position where you feel
	else
		seat[0].SetInt(1); // position where you feel
	seat[1].SetString("");
	Scaleform::GFx::Value car[1];
	switch(var6)
	{
	case 1: 	
		  car[0].SetString("BUGGY");
		  break;
	case 2:
		  car[0].SetString("TRUCK");
		  break;
	case 3:
		  car[0].SetString("STRYKER");
		  break;

	}
	gfxHUD.Invoke("_root.api.setCarInfo", var , 5);
	gfxHUD.Invoke("_root.api.setCarTypeInfo",car,1);
	gfxHUD.Invoke("_root.api.setCarSeatInfo",seat,2);
	gfxHUD.Invoke("_root.api.setCarInfoVisibility", show);
}

void HUDDisplay::enableClanChannel()
{
	Scaleform::GFx::Value var[4];
	var[0].SetInt(2);
	var[1].SetString("CLAN");
	var[2].SetBoolean(false);
	var[3].SetBoolean(true);
	gfxHUD.Invoke("_root.api.setChatTab", var, 4);
}

void HUDDisplay::enableGroupChannel()
{
	Scaleform::GFx::Value var[4];
	var[0].SetInt(3);
	var[1].SetString("GROUP");
	var[2].SetBoolean(false);
	var[3].SetBoolean(true);
	gfxHUD.Invoke("_root.api.setChatTab", var, 4);
}

void HUDDisplay::removePlayerFromGroup(const char* gamertag,bool legend)
{
		gClientLogic().localPlayer_->MaxGroupPlayers--;
		if (gClientLogic().localPlayer_->MaxGroupPlayers<1)
			gClientLogic().localPlayer_->MaxGroupPlayers=0;

		Scaleform::GFx::Value var[2];
		var[0].SetString(gamertag);
		gfxHUD.Invoke("_root.api.removePlayerFromGroup", var , 1);
}

void HUDDisplay::aboutToLeavePlayerFromGroup(const char* gamertag)
{
		Scaleform::GFx::Value var[2];
		var[0].SetString(gamertag);
		gfxHUD.Invoke("_root.api.aboutToLeavePlayerFromGroup", var , 1);
}

void HUDDisplay::updateLeaderGroup(const char* gamertag)
{
		Scaleform::GFx::Value var[2];
		var[0].SetString(gamertag);
		gfxHUD.Invoke("_root.api.updateLeaderGroup", var , 1);
}

void HUDDisplay::WantLeaveFromGroup()
{
	PKT_S2C_GroupData_s Groups;
	Groups.State = 9;
	strcpy(Groups.fromgamertag,gClientLogic().localPlayer_->CurLoadout.Gamertag);
	Groups.MyID = gClientLogic().localPlayer_->CurLoadout.GroupID;
	Groups.IDPlayerOutGroup = gClientLogic().localPlayer_->GetNetworkID();
	p2pSendToHost(gClientLogic().localPlayer_, &Groups, sizeof(Groups));
}

void HUDDisplay::LeaveFromGroup()
{
	obj_Player* plr = gClientLogic().localPlayer_;
	PKT_S2C_GroupData_s n2;
	n2.State = 6;
	strcpy(n2.MyNickName,gClientLogic().localPlayer_->CurLoadout.Gamertag);
	n2.MyID=gClientLogic().localPlayer_->CurLoadout.GroupID;
	n2.IDPlayerOutGroup=gClientLogic().localPlayer_->GetNetworkID();
	p2pSendToHost(plr, &n2, sizeof(n2));
}

void HUDDisplay::addPlayerToGroup(const char* gamertag,bool isLeader)
{
		Scaleform::GFx::Value var[3];
		var[0].SetString(gamertag);
		var[1].SetBoolean(isLeader); // Group Leader Active=true, Disable=false
		var[2].SetBoolean(false);
		gfxHUD.Invoke("_root.api.addPlayerToGroup", var , 3);
}

void HUDDisplay::showMissionInfo(bool var1)
{
	Scaleform::GFx::Value var[1];
	var[0].SetBoolean(var1);
	gfxHUD.Invoke("_root.api.showMissionInfo", var , 1);
}
void HUDDisplay::removeMissionInfo(int var1)
{
	Scaleform::GFx::Value var[1];
	var[0].SetUInt(var1);
	gfxHUD.Invoke("_root.api.removeMissionInfo", var , 1);
}
void HUDDisplay::setMissionObjectiveCompleted(int var1 , int var2)
{
	Scaleform::GFx::Value var[2];
	var[0].SetUInt(var1);
	var[1].SetUInt(var2);
	gfxHUD.Invoke("_root.api.setMissionObjectiveCompleted", var , 2);
}
void HUDDisplay::addMissionObjective(int var1 , const char* var2 , bool var3 , const char* var4 , bool var5)
{
	Scaleform::GFx::Value var[5];
	var[0].SetUInt(var1);
	var[1].SetString(var2);
	var[2].SetBoolean(var3);
	var[3].SetString(var4);
	var[4].SetBoolean(var5);
	gfxHUD.Invoke("_root.api.addMissionObjective", var , 5);
}
void HUDDisplay::setMissionObjectiveNumbers(int var1, int var2 , const char* var3)
{
	Scaleform::GFx::Value var[3];
	var[0].SetUInt(var1);
	var[1].SetUInt(var2);
	var[2].SetString(var3);
	gfxHUD.Invoke("_root.api.setMissionObjectiveNumbers", var , 3);
}
void HUDDisplay::addMissionInfo(const char* var1)
{
	Scaleform::GFx::Value var[2];
	var[0].SetString(var1);
	gfxHUD.Invoke("_root.api.addMissionInfo", var , 1);
}

int HUDDisplay::Update()
{
	if(!Inited)
		return 1;

	const ClientGameLogic& CGL = gClientLogic();

	if(r3dGetTime() > chatVisibleUntilTime && chatVisible && !chatInputActive)
	{
		showChat(false);
	}

	if(r3dGetTime() > timeoutNoteReadAbuseReportedHideUI && timeoutNoteReadAbuseReportedHideUI != 0)
	{
		r3dMouse::Hide();
		writeNoteSavedSlotIDFrom = 0;
		timeoutNoteReadAbuseReportedHideUI = 0;
		timeoutForNotes = r3dGetTime() + 0.5f;
		Scaleform::GFx::Value var[2];
		var[0].SetBoolean(false);
		var[1].SetString("");
		gfxHUD.Invoke("_root.api.showNoteRead", var, 2);
	}

	if(RangeFinderUIVisible)
	{
		r3dPoint3D dir;
		r3dScreenTo3D(r3dRenderer->ScreenW2, r3dRenderer->ScreenH2, &dir);

		PxRaycastHit hit;
		PhysicsCallbackObject* target = NULL;
		PxSceneQueryFilterData filter(PxFilterData(COLLIDABLE_STATIC_MASK|(1<<PHYSCOLL_NETWORKPLAYER), 0, 0, 0), PxSceneQueryFilterFlag::eSTATIC|PxSceneQueryFilterFlag::eDYNAMIC);
		g_pPhysicsWorld->raycastSingle(PxVec3(gCam.x, gCam.y, gCam.z), PxVec3(dir.x, dir.y, dir.z), 2000.0f, PxSceneQueryFlag::eDISTANCE, hit, filter);

		float distance = -1;
		if(hit.shape)
		{
			// sergey's design (range finder shows not real distance... have no idea what it actually shows)
			distance = hit.distance * (1.0f + R3D_MIN(1.0f, (R3D_MAX(0.0f, (hit.distance-200.0f)/1800.0f)))*0.35f);
		}
		gfxRangeFinder.Invoke("_root.Main.Distance.gotoAndStop", distance!=-1?"on":"off");
		char tmpStr[16];
		sprintf(tmpStr, "%.1f", distance);
		gfxRangeFinder.SetVariable("_root.Main.Distance.Distance.Distance.text", tmpStr);

		const ClientGameLogic& CGL = gClientLogic();
		float compass = atan2f(CGL.localPlayer_->m_vVision.z, CGL.localPlayer_->m_vVision.x)/R3D_PI;
		compass = R3D_CLAMP(compass, -1.0f, 1.0f);

		float cmpVal = -(compass * 820);
		gfxRangeFinder.SetVariable("_root.Main.compass.right.x", cmpVal);
		gfxRangeFinder.SetVariable("_root.Main.compass.left.x", cmpVal-1632);

		if(!CGL.localPlayer_->m_isAiming)
			showRangeFinderUI(false); // in case if player switched weapon or anything happened
	}

	return 1;
}


int HUDDisplay::Draw()
{
	if(!Inited)
		return 1;
	{
		R3DPROFILE_FUNCTION("gfxBloodStreak.UpdateAndDraw");
		if(bloodAlpha > 0.0f)
			gfxBloodStreak.UpdateAndDraw();
	}
	{
		R3DPROFILE_FUNCTION("gfxRangeFinder.UpdateAndDraw");
		if(RangeFinderUIVisible)
			gfxRangeFinder.UpdateAndDraw();
	}
	{
		R3DPROFILE_FUNCTION("gfxHUD.UpdateAndDraw");
#ifndef FINAL_BUILD
		gfxHUD.UpdateAndDraw(d_disable_render_hud->GetBool());
#else
		gfxHUD.UpdateAndDraw();
#endif
	}

	return 1;
}

void HUDDisplay::setBloodAlpha(float alpha)
{
	if(!Inited) return;
	if(R3D_ABS(bloodAlpha-alpha)<0.01f) return;

	bloodAlpha = alpha;
	gfxBloodStreak.SetVariable("_root.blood.alpha", alpha);
}

void HUDDisplay::eventShowPlayerListContextMenu(r3dScaleformMovie* pMove, const Scaleform::GFx::Value* args, unsigned argCount)
{
	/*gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", 2, "");
	gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", 3, "");
	gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", 4, "$HUD_PlayerAction_Kick");
	gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", 5, "$HUD_PlayerAction_Ban");*/

	const char* pName = args[0].GetString(); // Player Selected Gametag


	Scaleform::GFx::Value var[3];

	if(gUserProfile.ProfileData.isDevAccount)// || gClientLogic().localPlayer_->OwnerOfMap == gClientLogic().localPlayer_->CustomerID)
	{

	var[0].SetInt(1);
	var[1].SetString("$HUD_TELEPORT_TO");
	var[2].SetInt(1);
	gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

	var[0].SetInt(2);
	var[1].SetString("$HUD_TELEPORT_TO_ME");
	var[2].SetInt(2);
	gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

	var[0].SetInt(3);
	var[1].SetString("$HUD_PlayerAction_Kick");
	var[2].SetInt(3);
	gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

	var[0].SetInt(4);
	var[1].SetString("$HUD_PlayerAction_Ban");
	var[2].SetInt(4);
	gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

if (gClientLogic().localPlayer_)
{
	int NumPlayer=0;
	for(int i=0; i<R3D_ARRAYSIZE(gClientLogic().playerNames); i++)
	{
				if (gClientLogic().playerNames[i].isInvitePending && strcmp(gClientLogic().playerNames[i].Gamertag,pName) == 0)
				{
				NumPlayer=i;
				break;
				}
	}
				if (!gClientLogic().playerNames[NumPlayer].isInvitePending)
				{
						if (strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName)!= 0 && !gClientLogic().localPlayer_->CheckNameOnGroup(pName))
						{
							if (gClientLogic().localPlayer_->MaxGroupPlayers<11)
							{
								if (strcmp(GametagName,"") == 0)
								{
										var[0].SetInt(5);
										var[1].SetString("$HUD_PlayerAction_InviteGroup");
										var[2].SetInt(5);
										gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

										if (gClientLogic().localPlayer_->imOnGruop) 
										{
											if ((r3dGetTime() - TimeToWaitGroup) < TimeLeaveGroup && TimeToWaitGroup!=0)
											{
												var[0].SetInt(6);
												var[1].SetString("");
												var[2].SetInt(6);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
											}
											else {
												if (strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName)==0)
												{
												var[0].SetInt(6);
												var[1].SetString("$InfoMsg_LeaveGroup");
												var[2].SetInt(6);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
												else {
												var[0].SetInt(6);
												var[1].SetString("");
												var[2].SetInt(6);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
											}
										}
										else {
											var[0].SetInt(6);
											var[1].SetString("");
											var[2].SetInt(6);
											gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
										}
								}
								else {
										if (gClientLogic().localPlayer_->imOnGruop) 
										{
											if ((r3dGetTime() - TimeToWaitGroup) < TimeLeaveGroup && TimeToWaitGroup!=0)
											{
													var[0].SetInt(5);
													var[1].SetString("");
													var[2].SetInt(5);
													gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
											}
											else  {
												if (strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName)==0)
												{
												var[0].SetInt(5);
												var[1].SetString("$InfoMsg_LeaveGroup");
												var[2].SetInt(5);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
												else {
														var[0].SetInt(5);
														var[1].SetString("");
														var[2].SetInt(5);
														gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
											}
										}
										else {
											var[0].SetInt(5);
											var[1].SetString("");
											var[2].SetInt(5);
											gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
										}

										var[0].SetInt(6);
										var[1].SetString("");
										var[2].SetInt(6);
										gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

								}
							}
							else {
							if (gClientLogic().localPlayer_->imOnGruop) 
							{
											if ((r3dGetTime() - TimeToWaitGroup) < TimeLeaveGroup && TimeToWaitGroup!=0)
											{
												var[0].SetInt(5);
												var[1].SetString("");
												var[2].SetInt(5);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
											}
											else {
												if (strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName)==0)
												{
												var[0].SetInt(5);
												var[1].SetString("$InfoMsg_LeaveGroup");
												var[2].SetInt(5);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
												else {
												var[0].SetInt(5);
												var[1].SetString("");
												var[2].SetInt(5);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
											}
							}
							else {
								var[0].SetInt(5);
								var[1].SetString("");
								var[2].SetInt(5);
								gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
							}

								var[0].SetInt(6);
								var[1].SetString("");
								var[2].SetInt(6);
								gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
							}
						}
						else {
										if (gClientLogic().localPlayer_->imOnGruop) 
										{
											if ((r3dGetTime() - TimeToWaitGroup) < TimeLeaveGroup && TimeToWaitGroup!=0)
											{
												var[0].SetInt(5);
												var[1].SetString("");
												var[2].SetInt(5);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
											}
											else {
												if (strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName)==0)
												{
												var[0].SetInt(5);
												var[1].SetString("$InfoMsg_LeaveGroup");
												var[2].SetInt(5);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
												else {
													if (strcmp(GametagName,"") == 0)
													{
														if (gClientLogic().localPlayer_->KickGroupRemove(pName) && gClientLogic().localPlayer_->CheckNameOnGroup(pName) && strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName) != 0)
														{
															var[0].SetInt(5);
															var[1].SetString("$HUD_PlayerAction_KickFromGroup");
															var[2].SetInt(5);
															gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
														}
														else {
															var[0].SetInt(5);
															var[1].SetString("");
															var[2].SetInt(5);
															gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
														}
													}
													else {
														var[0].SetInt(5);
														var[1].SetString("");
														var[2].SetInt(5);
														gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
													}
												}
											}
										}
										else {
											var[0].SetInt(5);
											var[1].SetString("");
											var[2].SetInt(5);
											gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
										}

							var[0].SetInt(6);
							var[1].SetString("");
							var[2].SetInt(6);
							gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

						}
				}
				else {
					   if (strcmp(gClientLogic().playerNames[NumPlayer].Gamertag,pName) == 0)
					   {
						var[0].SetInt(5);
						var[1].SetString("$HUD_PlayerAction_AcceptGroupInvite");
						var[2].SetInt(5);
						gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

						var[0].SetInt(6);
						var[1].SetString("$HUD_PlayerAction_DeclineGroup");
						var[2].SetInt(6);
						gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
					   }
					   else {
							var[0].SetInt(5);
							var[1].SetString("");
							var[2].SetInt(5);
							gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

							if (gClientLogic().localPlayer_->imOnGruop) 
							{
											if ((r3dGetTime() - TimeToWaitGroup) < TimeLeaveGroup && TimeToWaitGroup!=0)
											{
												var[0].SetInt(6);
												var[1].SetString("");
												var[2].SetInt(6);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
											}
											else {
												if (strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName)==0)
												{
												var[0].SetInt(6);
												var[1].SetString("$InfoMsg_LeaveGroup");
												var[2].SetInt(6);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
												else {
												var[0].SetInt(6);
												var[1].SetString("");
												var[2].SetInt(6);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
											}
							}
							else {
								var[0].SetInt(6);
								var[1].SetString("");
								var[2].SetInt(6);
								gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
							}
					   }
				}
}
						var[0].SetInt(7);
						var[1].SetString("");
						var[2].SetInt(7);
						gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
	}
	else {

	var[0].SetInt(1);
	var[1].SetString("$HUD_PlayerAction_Report");
	var[2].SetInt(1);
	gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

if (gClientLogic().localPlayer_)
{
	int NumPlayer=0;
	for(int i=0; i<R3D_ARRAYSIZE(gClientLogic().playerNames); i++)
	{
				if (gClientLogic().playerNames[i].isInvitePending && strcmp(gClientLogic().playerNames[i].Gamertag,pName) == 0)
				{
				NumPlayer=i;
				break;
				}
	}
				if (!gClientLogic().playerNames[NumPlayer].isInvitePending)
				{
						if (strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName)!= 0 && !gClientLogic().localPlayer_->CheckNameOnGroup(pName))
						{
							if (gClientLogic().localPlayer_->MaxGroupPlayers<11)
							{
								if (strcmp(GametagName,"") == 0)
								{
										var[0].SetInt(2);
										var[1].SetString("$HUD_PlayerAction_InviteGroup");
										var[2].SetInt(2);
										gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

										if (gClientLogic().localPlayer_->imOnGruop) 
										{
											if ((r3dGetTime() - TimeToWaitGroup) < TimeLeaveGroup && TimeToWaitGroup!=0)
											{
													var[0].SetInt(3);
													var[1].SetString("");
													var[2].SetInt(3);
													gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
											}
											else {
												if (strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName)==0)
												{
												var[0].SetInt(3);
												var[1].SetString("$InfoMsg_LeaveGroup");
												var[2].SetInt(3);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
												else {
														var[0].SetInt(3);
														var[1].SetString("");
														var[2].SetInt(3);
														gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
											}
										}
										else {
											var[0].SetInt(3);
											var[1].SetString("");
											var[2].SetInt(3);
											gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
										}
								}
								else {
										if (gClientLogic().localPlayer_->imOnGruop) 
										{
											if ((r3dGetTime() - TimeToWaitGroup) < TimeLeaveGroup && TimeToWaitGroup!=0)
											{
												var[0].SetInt(2);
												var[1].SetString("");
												var[2].SetInt(2);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
											}
											else {
												if (strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName)==0)
												{
												var[0].SetInt(2);
												var[1].SetString("$InfoMsg_LeaveGroup");
												var[2].SetInt(2);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
												else {
												var[0].SetInt(2);
												var[1].SetString("");
												var[2].SetInt(2);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
											}
										}
										else {
											var[0].SetInt(2);
											var[1].SetString("");
											var[2].SetInt(2);
											gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
										}

										var[0].SetInt(3);
										var[1].SetString("");
										var[2].SetInt(3);
										gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
								}
							}
							else {
										if (gClientLogic().localPlayer_->imOnGruop) 
										{
											if ((r3dGetTime() - TimeToWaitGroup) < TimeLeaveGroup && TimeToWaitGroup!=0)
											{
												var[0].SetInt(2);
												var[1].SetString("");
												var[2].SetInt(2);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
											}
											else {
												if (strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName)==0)
												{
												var[0].SetInt(2);
												var[1].SetString("$InfoMsg_LeaveGroup");
												var[2].SetInt(2);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
												else {
												var[0].SetInt(2);
												var[1].SetString("");
												var[2].SetInt(2);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
											}
										}
										else {
											var[0].SetInt(2);
											var[1].SetString("");
											var[2].SetInt(2);
											gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
										}

								var[0].SetInt(3);
								var[1].SetString("");
								var[2].SetInt(3);
								gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
							}
						}
						else {
							if (gClientLogic().localPlayer_->imOnGruop) 
							{
											if ((r3dGetTime() - TimeToWaitGroup) < TimeLeaveGroup && TimeToWaitGroup!=0)
											{
												var[0].SetInt(2);
												var[1].SetString("");
												var[2].SetInt(2);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
											}
											else {
												if (strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName)==0)
												{
												var[0].SetInt(2);
												var[1].SetString("$InfoMsg_LeaveGroup");
												var[2].SetInt(2);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
												else {
													if (strcmp(GametagName,"")==0)
													{
														if (gClientLogic().localPlayer_->KickGroupRemove(pName) && gClientLogic().localPlayer_->CheckNameOnGroup(pName) && strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName) != 0)
														{
															var[0].SetInt(2);
															var[1].SetString("$HUD_PlayerAction_KickFromGroup");
															var[2].SetInt(2);
															gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
														}
														else {
															var[0].SetInt(2);
															var[1].SetString("");
															var[2].SetInt(2);
															gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
														}
													}
													else {
														var[0].SetInt(2);
														var[1].SetString("");
														var[2].SetInt(2);
														gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
													}
												}
											}
							}
							else {
											var[0].SetInt(2);
											var[1].SetString("");
											var[2].SetInt(2);
											gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
							}

							var[0].SetInt(3);
							var[1].SetString("");
							var[2].SetInt(3);
							gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

						}
				}
				else {
					   if (strcmp(gClientLogic().playerNames[NumPlayer].Gamertag,pName)== 0)
					   {
						var[0].SetInt(2);
						var[1].SetString("$HUD_PlayerAction_AcceptGroupInvite");
						var[2].SetInt(2);
						gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

						var[0].SetInt(3);
						var[1].SetString("$HUD_PlayerAction_DeclineGroup");
						var[2].SetInt(3);
						gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
					   }
					   else {
							if (gClientLogic().localPlayer_->imOnGruop) 
							{
											if ((r3dGetTime() - TimeToWaitGroup) < TimeLeaveGroup && TimeToWaitGroup!=0)
											{
												var[0].SetInt(2);
												var[1].SetString("");
												var[2].SetInt(2);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
											}
											else {
												if (strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName)==0)
												{
												var[0].SetInt(2);
												var[1].SetString("$InfoMsg_LeaveGroup");
												var[2].SetInt(2);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
												else {
												var[0].SetInt(2);
												var[1].SetString("");
												var[2].SetInt(2);
												gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
												}
											}
							}
							else {
											var[0].SetInt(2);
											var[1].SetString("");
											var[2].SetInt(2);
											gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
							}

						var[0].SetInt(3);
						var[1].SetString("");
						var[2].SetInt(3);
						gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
					   }
				}
}
if (gClientLogic().localPlayer_->OwnerOfMap == gClientLogic().localPlayer_->CustomerID)
{
						var[0].SetInt(4);
						var[1].SetString("$HUD_TELEPORT_TO");
						var[2].SetInt(4);
						gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

						var[0].SetInt(5);
						var[1].SetString("$HUD_TELEPORT_TO_ME");
						var[2].SetInt(5);
						gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

						var[0].SetInt(6);
						var[1].SetString("$HUD_PlayerAction_Kick");
						var[2].SetInt(6);
						gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
}
else {
						var[0].SetInt(4);
						var[1].SetString("");
						var[2].SetInt(4);
						gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

						var[0].SetInt(5);
						var[1].SetString("");
						var[2].SetInt(5);
						gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

						var[0].SetInt(6);
						var[1].SetString("");
						var[2].SetInt(6);
						gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);
}
						var[0].SetInt(7);
						var[1].SetString("");
						var[2].SetInt(7);
						gfxHUD.Invoke("_root.api.setPlayerListContextMenuButton", var, 3);

    }

	gfxHUD.Invoke("_root.api.showPlayerListContextMenu", "");
}

void HUDDisplay::eventPlayerListAction(r3dScaleformMovie* pMove, const Scaleform::GFx::Value* args, unsigned argCount)
{
	// REPORT
	// ""
	// ""
	// KICK
	// BAN
	int action = args[0].GetInt();
	const char* pName = args[1].GetString();

if(gUserProfile.ProfileData.isDevAccount)
{
	if(action == 1)  // Teleport to player
	{
		showChatInput();
		hudMain->showPlayersList(0);

		const ClientGameLogic& CGL = gClientLogic();
		obj_Player* plr = CGL.localPlayer_;

		if (plr)
		{
			PKT_C2C_ChatMessage_s n;
			char tpt[128];
			sprintf(tpt, "/goto %s", pName);

			r3dscpy(n.gamertag, "system");
			r3dscpy(n.msg, tpt);
			n.msgChannel = 1;
			n.userFlag = 0;
			p2pSendToHost(gClientLogic().localPlayer_, &n, sizeof(n));
			return;
		}
	}

	if(action == 2)  // Teleport a player to you
	{
		showChatInput();
		hudMain->showPlayersList(0);

		const ClientGameLogic& CGL = gClientLogic();
		obj_Player* plr = CGL.localPlayer_;

		if (plr)
		{
			PKT_C2C_ChatMessage_s n;
			char tptm[128];
			sprintf(tptm, "/gome %s", pName);

			r3dscpy(n.gamertag, "system");
			r3dscpy(n.msg, tptm);
			n.msgChannel = 1;
			n.userFlag = 0;
			p2pSendToHost(gClientLogic().localPlayer_, &n, sizeof(n));
			return;
		}
	}

	if(action == 3)
	{
		showChatInput();
		hudMain->showPlayersList(0);

		const ClientGameLogic& CGL = gClientLogic();
		obj_Player* plr = CGL.localPlayer_;

		if (plr)
		{
			PKT_C2C_ChatMessage_s n;
			char ffkick[128];
			sprintf(ffkick, "/kick %s", pName);

			r3dscpy(n.gamertag, "system");
			r3dscpy(n.msg, ffkick);
			n.msgChannel = 1;
			n.userFlag = 0;
			p2pSendToHost(gClientLogic().localPlayer_, &n, sizeof(n));
			return;
		}
	}

	if(action == 4)
	{
		showChatInput();
		hudMain->showPlayersList(0);

		char ffBan[128];
		sprintf(ffBan, "/ban %s You.are.banned.from.the.server", pName);
		gfxHUD.Invoke("_root.api.setChatActive", pName);

		chatVisible = true;
		Scaleform::GFx::Value var[3];
		var[0].SetBoolean(true);
		var[1].SetBoolean(true);
		var[2].SetString(ffBan);
		gfxHUD.Invoke("_root.api.showChat", var, 3);
		chatVisibleUntilTime = r3dGetTime() + 20.0f;
	}
	if(action == 5)  // Group invite sent
	{
		hudMain->showPlayersList(0);

		if (gClientLogic().localPlayer_->CheckNameOnGroup(pName) && strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName) != 0)
		{
			for (int i=0;i<11;i++) 
			{
				if (strcmp(gClientLogic().localPlayer_->PlayersNames[i],pName) == 0) // Check if player is on Group table
				{
					gClientLogic().localPlayer_->PlayersOnGroup[i]=false;
					break;
				}
			}
			PKT_S2C_GroupData_s Groups;
			Groups.State = 10;
			strcpy(Groups.fromgamertag,pName);
			strcpy(Groups.intogamertag,gClientLogic().localPlayer_->CurLoadout.Gamertag);
			Groups.MyID = gClientLogic().localPlayer_->CurLoadout.GroupID;
			p2pSendToHost(gClientLogic().localPlayer_, &Groups, sizeof(Groups));
			return;
		}
		if (gClientLogic().localPlayer_->imOnGruop && strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName) == 0) // LEAVE GROUP
		{
			if (gClientLogic().localPlayer_->StartToLeaveGroup == true)
				return;
			WantLeaveFromGroup();
			return;
		}
	
		int NumPlayers =0;
		for(int i=0; i<R3D_ARRAYSIZE(gClientLogic().playerNames); i++)
		{
				if (gClientLogic().playerNames[i].isInvitePending && strcmp(gClientLogic().playerNames[i].Gamertag,pName) == 0)
				{
					NumPlayers=i;
					break;
				}
		}
				
		if (gClientLogic().playerNames[NumPlayers].isInvitePending) // ACCEPT INVITE
		{
						for (int i=0;i<10;i++)
						{
							strcpy(gClientLogic().localPlayer_->PlayersNames[i],"");
							gClientLogic().localPlayer_->IDSPlayers[i]=0;
						}
                        obj_Player* plr = gClientLogic().localPlayer_;
                        PKT_S2C_GroupData_s n1;
						n1.State = 2;
                        n1.FromCustomerID = plr->CustomerID;
                        r3dscpy(n1.fromgamertag,plr->CurLoadout.Gamertag); // YO
                        r3dscpy(n1.intogamertag,pName); // EL
						strcpy(GametagName,pName);
	                    p2pSendToHost(plr, &n1, sizeof(n1));
						gClientLogic().playerNames[NumPlayers].isInvitePending = false;
						plr->imOnGruop=true;
						TimeToWaitGroup=r3dGetTime();
						for(int i=0; i<R3D_ARRAYSIZE(gClientLogic().playerNames); i++)
						{
							gClientLogic().playerNames[i].isInvitePending= false;
						}
                        return;
		}	
		else // SEND INVITES
		{
			obj_Player* plr = gClientLogic().localPlayer_;
        
			if (gClientLogic().localPlayer_)
			{
				if (strcmp(plr->CurLoadout.Gamertag,pName)== 0)
				{
					hudMain->showMessage(gLangMngr.getString("$InfoMsg_CannotInviteYourself"));
					return;
				}
			}

					for (int i=0;i<10;i++)
					{
						if (strcmp(gClientLogic().localPlayer_->PlayersNames[i],pName) == 0)
						{
							hudMain->showMessage(gLangMngr.getString("GroupPlayerAlreadyInGroup"));
							return;
						}
					}

					if (gClientLogic().localPlayer_->imOnGruop==false)
					{
						for (int i=0;i<10;i++)
						{
							strcpy(gClientLogic().localPlayer_->PlayersNames[i],"");
							gClientLogic().localPlayer_->IDSPlayers[i]=0;
						}
					}

					PKT_S2C_GroupData_s n;
					n.State = 1;
					n.FromCustomerID = plr->CustomerID;
					r3dscpy(n.fromgamertag,plr->CurLoadout.Gamertag);
					r3dscpy(n.intogamertag,pName);
					strcpy(n.GametagLeader,gClientLogic().localPlayer_->CurLoadout.Gamertag);
					p2pSendToHost(gClientLogic().localPlayer_, &n, sizeof(n));
					strcpy(GametagName,"");
					return;
		}
	}
	if(action == 6) // Decline Invites
	{
		hudMain->showPlayersList(0);

		if (gClientLogic().localPlayer_->imOnGruop) // LEAVE GROUP
		{
			if (gClientLogic().localPlayer_->StartToLeaveGroup == true)
				return;
			WantLeaveFromGroup();
			return;
		}

		if (gClientLogic().localPlayer_)
		{
				int NumPlayer=0;
				for(int i=0; i<R3D_ARRAYSIZE(gClientLogic().playerNames); i++)
				{
					if (gClientLogic().playerNames[i].isInvitePending && strcmp(gClientLogic().playerNames[i].Gamertag,pName) == 0)
					{
						NumPlayer=i;
						break;
					}
				}

				 if (gClientLogic().playerNames[NumPlayer].isInvitePending)
					{
						for (int i=0;i<10;i++)
						{
							strcpy(gClientLogic().localPlayer_->PlayersNames[i],"");
							gClientLogic().localPlayer_->IDSPlayers[i]=0;
						}
                        obj_Player* plr = gClientLogic().localPlayer_;
                        PKT_S2C_GroupData_s n1;
						n1.State = 3;
                        n1.FromCustomerID = plr->CustomerID;
                        r3dscpy(n1.fromgamertag,plr->CurLoadout.Gamertag);
                        r3dscpy(n1.intogamertag,pName);
	                    p2pSendToHost(plr, &n1, sizeof(n1));
	                    gClientLogic().playerNames[NumPlayer].isInvitePending = false;
						plr->imOnGruop=false;
						TimeToWaitGroup=r3dGetTime();
						gClientLogic().localPlayer_->MaxGroupPlayers=0;
	                    return;
					}
		}

	}
	if(action == 7)
	{
	}
}
else {

	if(action == 1)  // Report to player
	{
		showChatInput();
		hudMain->showPlayersList(0);

		char tpt[128];
		sprintf(tpt, "/report %s", pName);
		//gfxHUD.Invoke("_root.api.setChatActive", ffReport);

		chatVisible = true;
		Scaleform::GFx::Value var[3];
		var[0].SetBoolean(true);
		var[1].SetBoolean(true);
		var[2].SetString(tpt);
		gfxHUD.Invoke("_root.api.showChat", var, 3);
		chatVisibleUntilTime = r3dGetTime() + 20.0f;
	}

	if(action == 2)  // Group invite sent
	{
		hudMain->showPlayersList(0);

		if (gClientLogic().localPlayer_->CheckNameOnGroup(pName) && strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName) != 0)
		{
			for (int i=0;i<11;i++) 
			{
				if (strcmp(gClientLogic().localPlayer_->PlayersNames[i],pName) == 0) // Check if player is on Group table
				{
					gClientLogic().localPlayer_->PlayersOnGroup[i]=false;
					break;
				}
			}
			PKT_S2C_GroupData_s Groups;
			Groups.State = 10;
			strcpy(Groups.fromgamertag,pName);
			strcpy(Groups.intogamertag,gClientLogic().localPlayer_->CurLoadout.Gamertag);
			Groups.MyID = gClientLogic().localPlayer_->CurLoadout.GroupID;
			p2pSendToHost(gClientLogic().localPlayer_, &Groups, sizeof(Groups));
			return;
		}

		if (gClientLogic().localPlayer_->imOnGruop && strcmp(gClientLogic().localPlayer_->CurLoadout.Gamertag,pName) == 0) // LEAVE GROUP
		{
			if (gClientLogic().localPlayer_->StartToLeaveGroup == true)
				return;
			WantLeaveFromGroup();
			return;
		}

		int NumPlayers =0;
		for(int i=0; i<R3D_ARRAYSIZE(gClientLogic().playerNames); i++)
		{
				if (gClientLogic().playerNames[i].isInvitePending && strcmp(gClientLogic().playerNames[i].Gamertag,pName) == 0)
				{
					NumPlayers=i;
					break;
				}
		}
				
		if (gClientLogic().playerNames[NumPlayers].isInvitePending) // ACCEPT INVITE
		{
						for (int i=0;i<10;i++)
						{
							strcpy(gClientLogic().localPlayer_->PlayersNames[i],"");
							gClientLogic().localPlayer_->IDSPlayers[i]=0;
						}

                        obj_Player* plr = gClientLogic().localPlayer_;
                        PKT_S2C_GroupData_s n1;
						n1.State = 2;
                        n1.FromCustomerID = plr->CustomerID;
                        r3dscpy(n1.fromgamertag,plr->CurLoadout.Gamertag); // YO
                        r3dscpy(n1.intogamertag,pName); // EL
						strcpy(GametagName,pName);
	                    p2pSendToHost(plr, &n1, sizeof(n1));
						gClientLogic().playerNames[NumPlayers].isInvitePending = false;
						TimeToWaitGroup=r3dGetTime();
						plr->imOnGruop=true;
						for(int i=0; i<R3D_ARRAYSIZE(gClientLogic().playerNames); i++)
						{
							gClientLogic().playerNames[i].isInvitePending= false;
						}
                        return;
		}	
		else // SEND INVITES
		{
			obj_Player* plr = gClientLogic().localPlayer_;
        
			if (gClientLogic().localPlayer_)
			{
				if (strcmp(plr->CurLoadout.Gamertag,pName)== 0)
				{
					hudMain->showMessage(gLangMngr.getString("$InfoMsg_CannotInviteYourself"));
					return;
				}
			}
					for (int i=0;i<10;i++)
					{
						if (strcmp(gClientLogic().localPlayer_->PlayersNames[i],pName) == 0)
						{
							hudMain->showMessage(gLangMngr.getString("GroupPlayerAlreadyInGroup"));
							return;
						}
					}
					if (gClientLogic().localPlayer_->imOnGruop==false)
					{
						for (int i=0;i<10;i++)
						{
							strcpy(gClientLogic().localPlayer_->PlayersNames[i],"");
							gClientLogic().localPlayer_->IDSPlayers[i]=0;
						}
					}
					PKT_S2C_GroupData_s n;
					n.State = 1;
					n.FromCustomerID = plr->CustomerID;
					r3dscpy(n.fromgamertag,plr->CurLoadout.Gamertag);
					r3dscpy(n.intogamertag,pName);
					strcpy(n.GametagLeader,gClientLogic().localPlayer_->CurLoadout.Gamertag);
					p2pSendToHost(gClientLogic().localPlayer_, &n, sizeof(n));
					strcpy(GametagName,"");
					return;
		}
	}
	if(action == 3) // Decline Invites
	{
		hudMain->showPlayersList(0);

		if (gClientLogic().localPlayer_->imOnGruop) // LEAVE GROUP
		{
			if (gClientLogic().localPlayer_->StartToLeaveGroup == true)
				return;
			WantLeaveFromGroup();
			return;
		}

		if (gClientLogic().localPlayer_)
		{
				int NumPlayer=0;
				for(int i=0; i<R3D_ARRAYSIZE(gClientLogic().playerNames); i++)
				{
					if (gClientLogic().playerNames[i].isInvitePending && strcmp(gClientLogic().playerNames[i].Gamertag,pName) == 0)
					{
						NumPlayer=i;
						break;
					}
				}

				 if (gClientLogic().playerNames[NumPlayer].isInvitePending)
					{
						for (int i=0;i<10;i++)
						{
							strcpy(gClientLogic().localPlayer_->PlayersNames[i],"");
							gClientLogic().localPlayer_->IDSPlayers[i]=0;
						}
                        obj_Player* plr = gClientLogic().localPlayer_;
                        PKT_S2C_GroupData_s n1;
						n1.State = 3;
                        n1.FromCustomerID = plr->CustomerID;
                        r3dscpy(n1.fromgamertag,plr->CurLoadout.Gamertag);
                        r3dscpy(n1.intogamertag,pName);
	                    p2pSendToHost(plr, &n1, sizeof(n1));
	                    gClientLogic().playerNames[NumPlayer].isInvitePending = false;
						plr->imOnGruop=false;
						TimeToWaitGroup=r3dGetTime();
						gClientLogic().localPlayer_->MaxGroupPlayers=0;
	                    return;
					}
		}

	}
	if(action == 4)// Admin rent Server TELEPORT_TO
	{
		hudMain->showPlayersList(0);

		PKT_S2C_RentAdminCommands_s n;
		n.Param = 1;
		strcpy(n.KickedBy,gClientLogic().localPlayer_->CurLoadout.Gamertag);
		strcpy(n.PlayerToKick,pName);
		p2pSendToHost(NULL, &n, sizeof(n));
	}
	if(action == 5)// Admin rent Server TELEPORT_TO_ME
	{
		hudMain->showPlayersList(0);

		PKT_S2C_RentAdminCommands_s n;
		n.Param = 2;
		strcpy(n.KickedBy,gClientLogic().localPlayer_->CurLoadout.Gamertag);
		strcpy(n.PlayerToKick,pName);
		p2pSendToHost(NULL, &n, sizeof(n));
	}
	if(action == 6)// Admin rent Server PlayerAction_Kick
	{
		hudMain->showPlayersList(0);

		PKT_S2C_RentAdminCommands_s n;
		n.Param = 3;
		strcpy(n.KickedBy,gClientLogic().localPlayer_->CurLoadout.Gamertag);
		strcpy(n.PlayerToKick,pName);
		p2pSendToHost(NULL, &n, sizeof(n));
	}

}

}

void HUDDisplay::eventChatMessage(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{
	chatInputActive = false;
	lastChatMessageSent = r3dGetTime();

	static char s_chatMsg[2048];
	int currentTabIndex = args[0].GetInt();
	r3dscpy(s_chatMsg, args[1].GetString());

	bool has_anything = false;

	size_t start_text=0;
	size_t argLen = strlen(s_chatMsg);
	if(argLen < 3)
		return;

//#ifndef FINAL_BUILD
	if(gUserProfile.ProfileData.isDevAccount && strncmp(s_chatMsg, "/stime", 6) == NULL)
	{
		char buf[256];
		int hour, min;
		if(3 != sscanf(s_chatMsg, "%s %d %d", buf, &hour, &min))
		{
			addChatMessage(0, "<system>", "/stime {hour} {min}", 0);
			return;
		}

		__int64 gameUtcTime = gClientLogic().GetServerGameTime();
		struct tm* tm = _gmtime64(&gameUtcTime);
		r3d_assert(tm);

		// adjust server time to match supplied hour
		gClientLogic().gameStartUtcTime_ -= tm->tm_sec;
		gClientLogic().gameStartUtcTime_ -= (tm->tm_min) * 60;
		gClientLogic().gameStartUtcTime_ += (hour - tm->tm_hour) * 60 * 60;
		gClientLogic().gameStartUtcTime_ += (min) * 60;
		gClientLogic().lastShadowCacheReset_ = -1;

		addChatMessage(0, "<system>", "time changed", 0);
		return;
	}
//#endif
	if(strncmp(s_chatMsg, "/sp", 6) == NULL)
	{
		obj_Player* plr = gClientLogic().localPlayer_;
	r3d_assert(plr);
	obj_SafeLock* obj = (obj_SafeLock*)srv_CreateGameObject("obj_SafeLock", "obj_SafeLock", plr->GetPosition());
	obj->SetNetworkID(100001);
	obj->OnCreate();
	}

	if(gUserProfile.ProfileData.isDevAccount && strncmp(s_chatMsg, "/spawngrave", 6) == NULL)
	{
		obj_Player* plr = gClientLogic().localPlayer_;
	r3d_assert(plr);
	obj_Grave* obj = (obj_Grave*)srv_CreateGameObject("obj_Grave", "obj_Grave", plr->GetPosition());
	obj->SetNetworkID(10000);
	obj->OnCreate();
	}

	char userName[64];
	gClientLogic().localPlayer_->GetUserName(userName);

	{
		PKT_C2C_ChatMessage_s n;
		n.userFlag = 0; // server will init it for others
		n.msgChannel = currentTabIndex;
		r3dscpy(n.msg, &s_chatMsg[start_text]);
		r3dscpy(n.gamertag, userName);
		p2pSendToHost(gClientLogic().localPlayer_, &n, sizeof(n));
	}

	uint32_t flags = 0;
	if(gUserProfile.ProfileData.AccountType==0)
		flags|=1;
	if(gUserProfile.ProfileData.isDevAccount)
		flags|=2;
	addChatMessage(currentTabIndex, userName, &s_chatMsg[start_text], flags);

	memset(s_chatMsg, 0, sizeof(s_chatMsg));
}

void HUDDisplay::addChatMessage(int tabIndex, const char* user, const char* text, uint32_t flags)
{
	if(!Inited) return;
	Scaleform::GFx::Value var[3];

	char tmpMsg[1024];
	const char* tabNames[] = {"[PROXIMITY]", "[GLOBAL]", "[CLAN]", "[GROUP]"};
	const char* tabNamesColor[] = {"#00A000", "#13bbeb", "#de13eb", "#ffa900"};
	const char* userNameColor[] = {"#ffffff", "#ffa800"};

	bool isUserLegend = (flags&1)?true:false;
	bool isUserDev = (flags&2)?true:false;

	const char* userColor = userNameColor[isUserLegend?1:0];
	const char* textColor = "#d0d0d0";
	const char* namePrefix = "";
	if(isUserDev)
	{
		userColor = "#00E1E1";
		textColor = "#00E1E1";
		namePrefix = "&lt;DEV&gt;";
	}

	// dirty stl :)
	std::string sUser = user;
	int pos = 0;
	while((pos= sUser.find('<'))!=-1)
		sUser.replace(pos, 1, "&lt;");
	while((pos = sUser.find('>'))!=-1)
		sUser.replace(pos, 1, "&gt;");

	std::string sMsg = text;
	while((pos = sMsg.find('<'))!=-1)
		sMsg.replace(pos, 1, "&lt;");
	while((pos = sMsg.find('>'))!=-1)
		sMsg.replace(pos, 1, "&gt;");

	// really simple profanity filter
	{
		int counter = 0;
		char profanityFilter[2048]={0};
		char clearString[2048]={0};
		r3dscpy(profanityFilter, sMsg.c_str());
		char* word = strtok(profanityFilter, " ");
		while(word)
		{
			if(dictionaryHash_.IsExists(word))
			{
				r3dscpy(&clearString[counter], "*** ");
				counter +=4;
			}
			else
			{
				r3dscpy(&clearString[counter], word);
				counter +=strlen(word);
				clearString[counter++] = ' ';
			}
			word = strtok(NULL, " ");
		}
		clearString[counter++] = 0;

		sMsg = clearString;
	}

	sprintf(tmpMsg, "<font color=\"%s\">%s</font> <font color=\"%s\">%s%s:</font> <font color=\"%s\">%s</font>", tabNamesColor[tabIndex], tabNames[tabIndex], userColor, namePrefix, sUser.c_str(), textColor, sMsg.c_str());

	var[0].SetString(tmpMsg);
	gfxHUD.Invoke("_root.api.receiveChat", var, 1);
}

void HUDDisplay::setVisibility(float percent)
{
	if(!Inited) return;
	gfxHUD.Invoke("_root.api.updateVisibility", percent);
}

void HUDDisplay::setHearing(float percent)
{
	if(!Inited) return;
	gfxHUD.Invoke("_root.api.updateHearingRadius", percent);
}

void HUDDisplay::setLifeParams(int food, int water, int health, int toxicity, int stamina)
{
	if(!Inited) return;
	Scaleform::GFx::Value var[5];

	// temp, for testing
#ifndef FINAL_BUILD
	if(d_ui_health->GetInt() >= 0)
		health = d_ui_health->GetInt();
	if(d_ui_toxic->GetInt() >= 0)
		toxicity = d_ui_toxic->GetInt();
	if(d_ui_water->GetInt() >= 0)
		water = d_ui_water->GetInt();
	if(d_ui_food->GetInt() >= 0)
		food = d_ui_food->GetInt();
	if(d_ui_stamina->GetInt() >= 0)
		stamina = d_ui_stamina->GetInt();
#endif

	// UI expects inverse values, so do 100-X (exception is toxicity)
	var[0].SetInt(100-food);
	var[1].SetInt(100-water);
	var[2].SetInt(100-health);
	var[3].SetInt(toxicity);
	var[4].SetInt(100-stamina);
	gfxHUD.Invoke("_root.api.setHeroCondition", var, 5);
}

void HUDDisplay::setWeaponInfo(int ammo, int clips, int firemode/*, int Durability*/)
{
	if(!Inited) return;
	Scaleform::GFx::Value var[4];
	var[0].SetInt(ammo);
	var[1].SetInt(clips);
	if(firemode==1)
		var[2].SetString("one");
	else if(firemode ==2)
		var[2].SetString("three");
	else
		var[2].SetString("auto");
		//var[3].SetInt(Durability);
	gfxHUD.Invoke("_root.api.setWeaponInfo", var, 4);
}

void HUDDisplay::showWeaponInfo(int state)
{
	if(!Inited) return;
	if(state != weaponInfoVisible)
		gfxHUD.Invoke("_root.api.showWeaponInfo", state);
	weaponInfoVisible = state;
}

void HUDDisplay::setSlotInfo(int slotID, const char* name, int quantity, const char* icon)
{
	if(!Inited) return;
	Scaleform::GFx::Value var[4];
	var[0].SetInt(slotID);
	var[1].SetString(name);
	var[2].SetInt(quantity);
	var[3].SetString(icon);
	gfxHUD.Invoke("_root.api.setSlot", var, 4);
}

void HUDDisplay::updateSlotInfo(int slotID, int quantity)
{
	if(!Inited) return;
	Scaleform::GFx::Value var[2];
	var[0].SetInt(slotID);
	var[1].SetInt(quantity);
	gfxHUD.Invoke("_root.api.updateSlot", var, 2);
}

void HUDDisplay::showSlots(bool state)
{
	if(!Inited) return;
	gfxHUD.Invoke("_root.api.showSlots", state);
}

void HUDDisplay::setActiveSlot(int slotID)
{
	if(!Inited) return;
	gfxHUD.Invoke("_root.api.setActiveSlot", slotID);
}

void HUDDisplay::setActivatedSlot(int slotID)
{
	if(!Inited) return;
	gfxHUD.Invoke("_root.api.setActivatedSlot", slotID);
}

void HUDDisplay::showMessage(const wchar_t* text)
{
	if(!Inited) return;
	gfxHUD.Invoke("_root.api.showMsg", text);
}

void HUDDisplay::showChat(bool showChat, bool force)
{
	if(!Inited) return;
	if(chatVisible != showChat || force)
	{
		chatVisible = showChat;
		Scaleform::GFx::Value var[2];
		var[0].SetBoolean(showChat);
		var[1].SetBoolean(chatInputActive);
		gfxHUD.Invoke("_root.api.showChat", var, 2);
	}
	if(showChat)
		chatVisibleUntilTime = r3dGetTime() + 20.0f;
}

void HUDDisplay::showChatInput()
{
	if(!Inited) return;
	chatInputActive = true;
	showChat(true, true);
	gfxHUD.Invoke("_root.api.setChatActive", "");
}

void HUDDisplay::setChatTransparency(float alpha)
{
	if(!Inited) return;
	gfxHUD.Invoke("_root.api.setChatTransparency", alpha);
}

void HUDDisplay::setChatChannel(int index)
{
	if(!Inited) return;
	if(index <0 || index > 3) return;

	if(currentChatChannel != index)
	{
		currentChatChannel = index;
		Scaleform::GFx::Value var[1];
		var[0].SetInt(index);
		gfxHUD.Invoke("_root.api.setChatTabActive", var, 1);

		showChatInput();
	}
}

void HUDDisplay::clearPlayersList()
{
	if(!Inited) return;
	gfxHUD.Invoke("_root.api.clearPlayersList", "");
}

extern const wchar_t* getReputationString(int Reputation);
//void HUDDisplay::addPlayerToList(int num, const char* name, int Reputation, bool isLegend, bool isDev, bool isPunisher, bool isInvitePending, bool IsPremium)
void HUDDisplay::addPlayerToList(int num, const char* name, int Reputation, bool isLegend, bool isDev, bool isPunisher, bool isInvitePending, bool IsPremium, bool local)
{
 if(!Inited) return;
 Scaleform::GFx::Value var[11];
 var[0].SetInt(num);
 var[1].SetInt(num);

 // dirty stl :)
 std::string sUser = name;
 int pos = 0;
 while((pos= sUser.find('<'))!=-1)
  sUser.replace(pos, 1, "&lt;");
 while((pos = sUser.find('>'))!=-1)
  sUser.replace(pos, 1, "&gt;");

 var[2].SetString(sUser.c_str());

 const wchar_t* algnmt = getReputationString(Reputation);
 if(isDev)
  algnmt = L"";
 var[3].SetStringW(algnmt);
 var[4].SetBoolean(isLegend);
 var[5].SetBoolean(isDev);
 var[6].SetBoolean(isPunisher);
 var[7].SetBoolean(isInvitePending);
 var[8].SetBoolean(false);
 var[9].SetBoolean(IsPremium);
 var[10].SetBoolean(local);
 gfxHUD.Invoke("_root.api.addPlayerToList", var, 11);
}

void HUDDisplay::showPlayersList(int flag)
{
	if(!Inited) return;
	playersListVisible = flag;
	gfxHUD.Invoke("_root.api.showPlayersList", flag);
}

void HUDDisplay::showWriteNote(int slotIDFrom)
{
	if(!Inited) return;

	r3dMouse::Show();

	writeNoteSavedSlotIDFrom = slotIDFrom;

	Scaleform::GFx::Value var[1];
	var[0].SetBoolean(true);
	gfxHUD.Invoke("_root.api.showNoteWrite", var, 1);
}

void HUDDisplay::eventNoteWritePost(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{
	r3d_assert(argCount == 1);

	r3dMouse::Hide();

	const char* Message = args[0].GetString();

	obj_Player* plr = gClientLogic().localPlayer_;
	r3d_assert(plr);

	PKT_C2S_CreateNote_s n;
	n.SlotFrom = (BYTE)writeNoteSavedSlotIDFrom;
	n.pos      = plr->GetPosition() + plr->GetvForw()*0.2f;
	n.ExpMins  = PKT_C2S_CreateNote_s::DEFAULT_PLAYER_NOTE_EXPIRE_TIME;
	r3dscpy(n.TextFrom, plr->CurLoadout.Gamertag);
	sprintf(n.TextSubj, Message);
	p2pSendToHost(gClientLogic().localPlayer_, &n, sizeof(n));

	// local logic
	wiInventoryItem& wi = plr->CurLoadout.Items[writeNoteSavedSlotIDFrom];
	r3d_assert(wi.itemID && wi.quantity > 0);
	//local logic
	wi.quantity--;
	if(wi.quantity <= 0) {
		wi.Reset();
	}

	plr->OnBackpackChanged(writeNoteSavedSlotIDFrom);

	writeNoteSavedSlotIDFrom = 0;

	timeoutForNotes = r3dGetTime() + .5f;
}

int HUDDisplay::GetRandSafeID()
{
	int rand = (int)u_GetRandom(0,30000);//rand() % 30000;
	for( GameObject* obj = GameWorld().GetFirstObject(); obj; obj = GameWorld().GetNextObject(obj) ) // Server Vehicles
	{
		if (obj->Class->Name == "obj_SafeLock") //safelock
		{
			obj_SafeLock* Safelock = (obj_SafeLock*)obj;
				if (Safelock->IDSafe == rand)
				{
					GetRandSafeID();
					break;
				}
		}
	}
	return rand;
}
void HUDDisplay::eventSafelockPass(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{
	r3d_assert(argCount == 1);

	r3dMouse::Hide();

	GameObject* obj = GameWorld().GetNetworkObject(SafeLockID);
	if (!obj)
		return;

	obj_SafeLock* Safelock = (obj_SafeLock*)obj;

	const char* Password = args[0].GetString();

	writeNoteSavedSlotIDFrom = 0;
	timeoutForNotes = r3dGetTime() + .5f;

	if (strcmp(Safelock->Password,"") == 0) // if not exist this note create one new
	{
		PKT_C2S_SafelockData_s n;
		n.Status = 2;
		n.ExpSeconds = 0;
		strcpy(n.Password,Password);
		n.pos		 = Safelock->GetPosition();
		n.rot		 = Safelock->GetRotationVector().x;
		n.SafelockID = SafeLockID;
		n.SafeID	 = GetRandSafeID();
		n.MapID	 = gClientLogic().m_gameInfo.mapId;
		Safelock->IDSafe=n.SafeID;
		n.Quantity = 0;
		n.Var1 = 0;
		n.Var2 = 0;
		n.myID = gClientLogic().localPlayer_->GetNetworkID();
		n.GameServerID = gClientLogic().m_gameInfo.gameServerId;
		p2pSendToHost(gClientLogic().localPlayer_, &n, sizeof(n));
		return;
	}

	if (strcmp(Safelock->Password,Password) == 0)
	{
		PKT_C2S_SafelockData_s n;
		n.Status = 5;
		n.State = 0;
		n.StateSesion = 1;
		n.MapID = gClientLogic().m_gameInfo.mapId;
		n.GameServerID = gClientLogic().m_gameInfo.gameServerId;
		n.CustomerID = gClientLogic().localPlayer_->GetNetworkID();
		n.iDSafeLock = Safelock->IDSafe;
		strcpy(n.Password,Safelock->Password); 
		p2pSendToHost(gClientLogic().localPlayer_, &n, sizeof(n));
		//r3dOutToLog("###### A SAFELOCK ID %i PASSWORD %s\n",Safelock->IDSafe,Safelock->Password);
	}
	else {
		showMessage(gLangMngr.getString("WrongPassword"));
	}

}

void HUDDisplay::eventGraveNoteClosed(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{
	r3dMouse::Hide();

	writeNoteSavedSlotIDFrom = 0;
	timeoutForNotes = r3dGetTime() + .5f;
}

void HUDDisplay::eventNoteClosed(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{
	r3dMouse::Hide();

	writeNoteSavedSlotIDFrom = 0;
	timeoutForNotes = r3dGetTime() + .5f;
}

void HUDDisplay::showSL(bool var1,bool var2, int ID )
{
if(!Inited) return;

	SafeLockID=0;
	SafeLockID=ID;

	GameObject* obj = GameWorld().GetNetworkObject(ID);
	if (obj)
	{
		obj_SafeLock* Safelock = (obj_SafeLock*)obj;
		if (strcmp(Safelock->Password,"") != 0)
			var1 = true;
	}

	r3dMouse::Show();
	writeNoteSavedSlotIDFrom = 1; // temp, to prevent mouse from hiding
	Scaleform::GFx::Value var[2];
	var[0].SetBoolean(var1);
    var[1].SetBoolean(var2);
	gfxHUD.Invoke("_root.api.showSafelock", var, 2);
	//gfxHUD.Invoke("_root.api.enableSafelockInput",true);
	
}

void HUDDisplay::showGraveNote(const char* plr,const char* plr2)
{
if(!Inited) return;

	r3dMouse::Show();
	writeNoteSavedSlotIDFrom = 1; // temp, to prevent mouse from hiding
	Scaleform::GFx::Value var[4];
	var[0].SetBoolean(true);
	var[1].SetString("R.I.P");
	var[2].SetString(plr);
	var[3].SetString(plr2);
	gfxHUD.Invoke("_root.api.showGraveNote", var, 4);
}

void HUDDisplay::showCarFull(const char* msg) // Server Vehicles
{
if(!Inited) return;

	r3dMouse::Show();
	writeNoteSavedSlotIDFrom = 1; // temp, to prevent mouse from hiding
	Scaleform::GFx::Value var[4];
	var[0].SetBoolean(true);
	var[1].SetStringW(gLangMngr.getString("$Vehicle"));
	var[2].SetStringW(gLangMngr.getString("$Vehicle_tittle"));
	var[3].SetString(msg);
	gfxHUD.Invoke("_root.api.showGraveNote", var, 4);
}

void HUDDisplay::StatusVehicle(const wchar_t* plr,const wchar_t* plr2) // Server Vehicles
{
if(!Inited) return;

	r3dMouse::Show();
	writeNoteSavedSlotIDFrom = 1; // temp, to prevent mouse from hiding
	Scaleform::GFx::Value var[4];
	var[0].SetBoolean(true);
	var[1].SetStringW(gLangMngr.getString("$Vehicle"));
	var[2].SetStringW(plr);
	var[3].SetStringW(plr2);
	gfxHUD.Invoke("_root.api.showGraveNote", var, 4);
}

void HUDDisplay::VehicleWithoutGasoline() // Server Vehicles
{
if(!Inited) return;

	r3dMouse::Show();
	writeNoteSavedSlotIDFrom = 1; // temp, to prevent mouse from hiding
	Scaleform::GFx::Value var[4];
	var[0].SetBoolean(true);
	var[1].SetStringW(gLangMngr.getString("$Vehicle"));
	var[2].SetStringW(gLangMngr.getString("$NoGasoline1"));
	var[3].SetString("$NoGasoline2");
	gfxHUD.Invoke("_root.api.showGraveNote", var, 4);
}

void HUDDisplay::VehicleDamaged() // Server Vehicles
{
if(!Inited) return;

	r3dMouse::Show();
	writeNoteSavedSlotIDFrom = 1; // temp, to prevent mouse from hiding
	Scaleform::GFx::Value var[4];
	var[0].SetBoolean(true);
	var[1].SetStringW(gLangMngr.getString("$Vehicle"));
	var[2].SetStringW(gLangMngr.getString("$VehicleDamage1"));
	var[3].SetString("$VehicleDamage2");
	gfxHUD.Invoke("_root.api.showGraveNote", var, 4);
}

void HUDDisplay::showReadNote(const char* msg)
{
	if(!Inited) return;

	r3dMouse::Show();
	writeNoteSavedSlotIDFrom = 1; // temp, to prevent mouse from hiding
	Scaleform::GFx::Value var[2];
	var[0].SetBoolean(true);
	var[1].SetString(msg);
	gfxHUD.Invoke("_root.api.showNoteRead", var, 2);
}

void HUDDisplay::eventNoteReportAbuse(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{
	// ptumik: not sure what to do with this yet... need design

	//r3dMouse::Hide();
	//writeNoteSavedSlotIDFrom = 0;
	//timeoutForNotes = r3dGetTime() + 1.0f;

	timeoutNoteReadAbuseReportedHideUI = r3dGetTime() + 0.5f;
}

void HUDDisplay::showYouAreDead(const char* killedBy)
{
	if(!Inited) return;

	gfxHUD.SetVariable("_root.Main.PlayerDead.DeadMsg.Text2.text", killedBy);
	gfxHUD.Invoke("_root.Main.PlayerDead.gotoAndPlay", "in");
}

void HUDDisplay::showSafeZoneWarning(bool flag)
{
	if(!Inited) return;

	if(SafeZoneWarningVisible != flag)
	{
		SafeZoneWarningVisible = flag;
		gfxHUD.Invoke("_root.Main.Condition.gotoAndPlay", flag ? 0 : 8); //"in":"out"
	}
}

namespace
{
	const char* getReputationIconString(int Reputation)
	{
		const char* algnmt = "";
		if(Reputation >= ReputationPoints::Paragon)
			algnmt = "paragon";
		else if(Reputation >= ReputationPoints::Vigilante)
			algnmt = "vigilante";
		else if(Reputation >= ReputationPoints::Guardian)
			algnmt = "guardian";
		else if(Reputation >= ReputationPoints::Lawman)
			algnmt = "lawman";
		else if(Reputation >= ReputationPoints::Deputy)
			algnmt = "deputy";
		else if(Reputation >= ReputationPoints::Constable)
			algnmt = "constable";
		else if(Reputation >= ReputationPoints::Civilian)
			algnmt = "civilian";
		else if(Reputation <= ReputationPoints::Villain)
			algnmt = "villain";
		else if(Reputation <= ReputationPoints::Assassin)
			algnmt = "assassin";
		else if(Reputation <= ReputationPoints::Hitman)
			algnmt = "hitman";
		else if(Reputation <= ReputationPoints::Bandit)
			algnmt = "bandit";
		else if(Reputation <= ReputationPoints::Outlaw)
			algnmt = "outlaw";
		else if(Reputation <= ReputationPoints::Thug)
			algnmt = "thug";

		return algnmt;
	}
}

void HUDDisplay::addCharTag(const char* name, int Reputation, bool isSameClan, Scaleform::GFx::Value& result)
{
	if(!Inited) return;
	r3d_assert(result.IsUndefined());

	Scaleform::GFx::Value var[3];
	var[0].SetString(name);
	var[1].SetBoolean(isSameClan);
	var[2].SetString(getReputationIconString(Reputation));
	gfxHUD.Invoke("_root.api.addCharTag", &result, var, 3);
}

void HUDDisplay::removeUserIcon(Scaleform::GFx::Value& icon)
{
	if(!Inited) return;
	r3d_assert(!icon.IsUndefined());

	Scaleform::GFx::Value var[1];
	var[0] = icon;
	gfxHUD.Invoke("_root.api.removeUserIcon", var, 1);

	icon.SetUndefined();
}

// optimized version
void HUDDisplay::moveUserIcon(Scaleform::GFx::Value& icon, const r3dPoint3D& pos, bool alwaysShow, bool force_invisible /* = false */, bool pos_in_screen_space/* =false */)
{
	if(!Inited)
		return;
	r3d_assert(!icon.IsUndefined());

	r3dPoint3D scrCoord;
	float x, y;
	int isVisible = 1;
	if(!pos_in_screen_space)
	{
		if(alwaysShow)
			isVisible = r3dProjectToScreenAlways(pos, &scrCoord, 20, 20);
		else
			isVisible = r3dProjectToScreen(pos, &scrCoord);
	}
	else
		scrCoord = pos;

	// convert screens into UI space
	float mulX = 1920.0f/r3dRenderer->ScreenW;
	float mulY = 1080.0f/r3dRenderer->ScreenH;
	x = scrCoord.x * mulX;
	y = scrCoord.y * mulY;

	Scaleform::GFx::Value::DisplayInfo displayInfo;
	icon.GetDisplayInfo(&displayInfo);
	displayInfo.SetVisible(isVisible && !force_invisible);
	displayInfo.SetX(x);
	displayInfo.SetY(y);
	icon.SetDisplayInfo(displayInfo);
}

void HUDDisplay::setCharTagTextVisible(Scaleform::GFx::Value& icon, bool isShowName, bool isSameGroup)
{
	if(!Inited) return;
	r3d_assert(!icon.IsUndefined());

	Scaleform::GFx::Value var[4]; // change 3 to 4
	var[0] = icon;
	var[1].SetBoolean(isShowName);
	var[2].SetBoolean(isSameGroup);
	var[3].SetBoolean(true);
	gfxHUD.Invoke("_root.api.setCharTagTextVisible", var, 4); // change 3 to 4
}

void HUDDisplay::setThreatValue(int value)
{
	Scaleform::GFx::Value vars[1];
	vars[0].SetInt(value);
	gfxHUD.Invoke("_root.api.setThreatValue", vars, 1);
}

void HUDDisplay::setCooldown(int slot,int CoolSecond,int value)
{
	Scaleform::GFx::Value vars[3];
	vars[0].SetInt(slot);
	vars[1].SetInt(CoolSecond);
	vars[2].SetInt(value);
	gfxHUD.Invoke("_root.api.setSlotCooldown", vars, 3);
}
