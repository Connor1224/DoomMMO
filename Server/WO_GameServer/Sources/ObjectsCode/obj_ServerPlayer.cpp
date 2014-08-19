#include "r3dpch.h"
#include "r3d.h"

#include "obj_ServerPlayer.h"
#include "ObjectsCode/Vehicles/obj_ServerVehicle.h"

#include "ServerWeapons/ServerWeapon.h"
#include "ServerWeapons/ServerGear.h"
#include "../EclipseStudio/Sources/ObjectsCode/weapons/WeaponArmory.h"

#include "multiplayer/P2PMessages.h"
#include "ServerGameLogic.h"
#include "obj_ServerRadioactiveArea.h"

#include "ObjectsCode/obj_ServerPostBox.h"
#include "ObjectsCode/sobj_DroppedItem.h"
#include "ObjectsCode/sobj_Note.h"
#include "ObjectsCode/sobj_Grave.h"
#include "ObjectsCode/obj_ServerBarricade.h"
#include "ObjectsCode/Zombies/sobj_Zombie.h"

#include "AsyncFuncs.h"
#include "Async_Notes.h"
#include "../EclipseStudio/Sources/backend/WOBackendAPI.h"
#include "../EclipseStudio/Sources/Gameplay_Params.h"
extern CGamePlayParams		GPP_Data;

IMPLEMENT_CLASS(obj_ServerPlayer, "obj_ServerPlayer", "Object");
AUTOREGISTER_CLASS(obj_ServerPlayer);

CVAR_COMMENT("_ai_", "AI variables");

obj_ServerPlayer::obj_ServerPlayer() :
	netMover(this, 0.2f, (float)PKT_C2C_MoveSetCell_s::PLAYER_CELL_RADIUS)
{
	ObjTypeFlags = OBJTYPE_Human;

	peerId_ = -1;
	startPlayTime_ = r3dGetTime();
	PlayerOnVehicle=false;

	m_PlayerFlyingAntiCheatTimer = 0.0f;
	m_PlayerUndergroundAntiCheatTimer = 0.0f;

	wasDisconnected_   = false;

	moveInited = false;

	r3dscpy(userName, "unknown");
	m_SelectedWeapon   = 0;
	m_ForcedEmptyHands = false;

	for(int i=0; i<NUM_WEAPONS_ON_PLAYER; i++)
		m_WeaponArray[i] = NULL;

	for(int i=0; i<SLOT_Max; i++)
		gears_[i] = NULL;

	lastTimeHit     = 0;
	lastHitBodyPart = 0;

	isTargetDummy_ = false;

	lastChatTime_    = -1;
	numChatMessages_ = 0;

	m_PlayerRotation = 0;

	lastCharUpdateTime_  = r3dGetTime();
	lastWorldUpdateTime_ = -1;
	lastWorldFlags_      = -1;
	lastVisUpdateTime_   = -1;

	haveBadBackpack_ = 0;

	return;
}

BOOL obj_ServerPlayer::OnCreate()
{
	parent::OnCreate();
	DrawOrder	= OBJ_DRAWORDER_FIRST;

	SetVelocity(r3dPoint3D(0, 0, 0));

	r3d_assert(!NetworkLocal);

	myPacketSequence = 0;
	clientPacketSequence = 0;
	packetBarrierReason = "";
	PlayerOnVehicle=false;
	GroupTime=0;

	enableCrosshairServer=true;
	enableSNP=true;
	CheckDatas = r3dGetTime() + 20; 

	FireHitCount = 0;

	lastWeapDataRep   = r3dGetTime();
	weapCheatReported = false;
	dieForExplosion = false;

	lastPlayerAction_ = r3dGetTime();
	m_PlayerState = 0;

	m_Stamina = GPP_Data.c_fSprintMaxEnergy;

	if(loadout_->Stats.skillid1 == 1){
		m_Stamina +=  25.5f;
		if(loadout_->Stats.skillid4 == 1)
			m_Stamina += 30.5f;
	}

	// set that character is alive
	loadout_->Alive   = 1;
	loadout_->GamePos = GetPosition();
	loadout_->GameDir = m_PlayerRotation;

	// invalidate last sended vitals
	lastVitals_.Health = 0xFF;
	lastVitals_.Hunger = 0xFF;

	gServerLogic.NetRegisterObjectToPeers(this);
	// detect what objects is visible right now
	gServerLogic.UpdateNetObjVisData(this);
	lastVisUpdateTime_ = r3dGetTime();
	LastHackShieldLog = r3dGetTime();

	// for initing cellMover there
	TeleportPlayer(GetPosition());

	return TRUE;
}


BOOL obj_ServerPlayer::OnDestroy(){
    for (int i=0;i<72;i++)
    {
        TradeSlot[i] = 0;
        TradeItems[i].Reset();    
    }
    Tradetargetid = 0;
    //CloseHandle(tThread);
  //  isDestroy = true;
    isTradeAccept = false;
    //TerminateThread(tThread,-1);
    //gServerLogic.ResetNetObjVisData(this);
    return parent::OnDestroy();
}

BOOL obj_ServerPlayer::Load(const char *fname)
{
	if(!parent::Load(fname))
		return FALSE;

	// Object won't be saved when level saved
	bPersistent = 0;

	Height      = SRV_WORLD_SCALE (1.8f);

	RecalcBoundBox();

	return TRUE;
}

void obj_ServerPlayer::DoRemoveItems(int slotid)
{
	PKT_S2C_BackpackModify_s n;
	n.SlotTo     = slotid;
	n.Quantity   = 0;
	n.dbg_ItemID = loadout_->Items[slotid].itemID;
	gServerLogic.p2pSendToPeer(peerId_, this, &n, sizeof(n));
	loadout_->Items[slotid].Reset();
	OnBackpackChanged(slotid);
}
void obj_ServerPlayer::DoRemoveSafeLockItems(int slotid, int quantity, int RemQuantity)
{
	PKT_S2C_BackpackModify_s n;
	n.SlotTo     = slotid;
	n.Quantity   = quantity;
	n.dbg_ItemID = loadout_->Items[slotid].itemID;
	gServerLogic.p2pSendToPeer(peerId_, this, &n, sizeof(n));

    loadout_->Items[slotid].quantity -= RemQuantity;
	if(loadout_->Items[slotid].quantity <= 0)
	{
	   loadout_->Items[slotid].Reset();
	}
	OnBackpackChanged(slotid);
}
void obj_ServerPlayer::DoRemoveAllItems(obj_ServerPlayer* plr)
{
	for (int i =0;i<72;i++)
	{
		PKT_S2C_BackpackModify_s n;
		n.SlotTo     = i;
		n.Quantity   = 0;
		n.dbg_ItemID = plr->loadout_->Items[i].itemID;
		gServerLogic.p2pSendToPeer(plr->peerId_, plr, &n, sizeof(n));
		plr->loadout_->Items[i].Reset();
		plr->OnBackpackChanged(i);
	}
}

void obj_ServerPlayer::DoTrade(obj_ServerPlayer* plr , obj_ServerPlayer* plr2)
{
    if (!plr || !plr2) return;
    r3dOutToLog("DoTrade(%s,%s)\n",plr->userName,plr2->userName);

	// check slot and weight
    float totalTradeweight = 0;
    bool freeWeight = false;
    float totalTradeweight2 = 0;
    bool freeWeight2 = false;

    for (int i =0;i<72;i++)
    {
        if (plr->TradeItems[i].itemID == 0)
            continue;
        wiInventoryItem item = plr->TradeItems[i];
        const BaseItemConfig* itemCfg = g_pWeaponArmory->getConfig(item.itemID);
        if (itemCfg)
        {
            totalTradeweight += itemCfg->m_Weight;
        }
    }

    for (int i =0;i<72;i++)
    {
        if (plr2->TradeItems[i].itemID == 0)
            continue;
        wiInventoryItem item = plr2->TradeItems[i];
        const BaseItemConfig* itemCfg = g_pWeaponArmory->getConfig(item.itemID);
        if (itemCfg)
        {
            totalTradeweight2 += itemCfg->m_Weight;
        }
    }

	// before trade. we need to save old Items of players.
    wiInventoryItem Items1[72];
    wiInventoryItem Items2[72];

    for (int i=0; i<72;i++)
    {
        Items1[i].Reset();
        Items2[i].Reset();
        Items1[i] = plr->loadout_->Items[i];
        Items2[i] = plr2->loadout_->Items[i];
    }

    bool Failed1 = false;
    bool Failed2 = false;
    for (int i=0; i<72;i++)
    {
        if (plr->TradeItems[i].itemID == 0 || plr->TradeSlot[i] == -1)
            continue;


        if (plr->loadout_->Items[plr->TradeSlot[i]].itemID != plr->TradeItems[i].itemID)
            continue;


        if (plr2->BackpackAddItem(plr->TradeItems[i]))
        {
            g_AsyncApiMgr->AddJob(new CJobTradeLog(plr->profile_.CustomerID,plr2->profile_.CustomerID,plr->loadout_->LoadoutID,plr2->loadout_->LoadoutID,plr->userName,plr2->userName,gServerLogic.ginfo_.gameServerId,plr->TradeItems[i]));
            plr->DoRemoveItems(plr->TradeSlot[i]);
        }
        else
            Failed2 = true;


        plr->TradeItems[i].Reset();
    }

    for (int i=0; i<72;i++)
    {
        if (plr2->TradeItems[i].itemID == 0 || plr2->TradeSlot[i] == -1)
            continue;


        if (plr2->loadout_->Items[plr2->TradeSlot[i]].itemID != plr2->TradeItems[i].itemID)
            continue;


        if (plr->BackpackAddItem(plr2->TradeItems[i]))
        {
            g_AsyncApiMgr->AddJob(new CJobTradeLog(plr2->profile_.CustomerID,plr->profile_.CustomerID,plr2->loadout_->LoadoutID,plr->loadout_->LoadoutID,plr2->userName,plr->userName,gServerLogic.ginfo_.gameServerId,plr2->TradeItems[i]));
            plr2->DoRemoveItems(plr2->TradeSlot[i]);
        }
        else
            Failed1 = true;


        plr2->TradeItems[i].Reset();
    }

    if (Failed1 || Failed2)
    {
        DoRemoveAllItems(plr);
        DoRemoveAllItems(plr2);


        for (int i=0;i<72;i++)
        {
            if (Items1[i].itemID != 0)
                plr->BackpackAddItem(Items1[i]);


            if (Items2[i].itemID != 0)
                plr2->BackpackAddItem(Items2[i]);
        }


        char chatmessage[128] = {0};
        PKT_C2C_ChatMessage_s n;
        sprintf(chatmessage, "Other player does not have enough free backpack slots");
        r3dscpy(n.gamertag, "<Trade>");
        r3dscpy(n.msg, chatmessage);
        n.msgChannel = 0;
        n.userFlag = 0;
        gServerLogic.p2pSendRawToPeer(plr->peerId_,&n,sizeof(n));
        gServerLogic.p2pSendRawToPeer(plr2->peerId_,&n,sizeof(n));
    }

    gServerLogic.ApiPlayerUpdateChar(plr2);
    gServerLogic.ApiPlayerUpdateChar(plr);
}
void obj_ServerPlayer::SetProfile(const CServerUserProfile& in_profile)
{
	profile_ = in_profile;
	loadout_ = &profile_.ProfileData.ArmorySlots[0];
	savedLoadout_ = *loadout_;

	// those was already checked in GetProfileData, but make sure about that
	r3d_assert(profile_.ProfileData.ArmorySlots[0].LoadoutID);
	r3d_assert(profile_.ProfileData.NumSlots == 1);
	r3d_assert(loadout_->LoadoutID > 0);
	r3d_assert(loadout_->Alive > 0);

	r3dscpy(userName, loadout_->Gamertag);

	boostXPBonus_          = 0.0f; // % to add
	boostWPBonus_          = 0.0f; // % to add

	//r3dOutToLog("SetProfile %s\n", userName); CLOG_INDENT;

	ValidateBackpack();
	ValidateAttachments();
	SetLoadoutData();

	return;
}

void obj_ServerPlayer::ValidateBackpack()
{
	for(int i=0; i<loadout_->CHAR_MAX_BACKPACK_SIZE; i++)
	{
		wiInventoryItem& wi = loadout_->Items[i];
		if(wi.itemID == 0)
			continue;

		if(g_pWeaponArmory->getConfig(wi.itemID) == NULL)
		{
			gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Data, false, "ValidateBackpack",
				"%d", wi.itemID);
			wi.Reset();

			haveBadBackpack_ = 1;
		}
	}
}

void obj_ServerPlayer::ValidateAttachments()
{
	for(int i=0; i<2; i++)
	{
		for(int j=0; j<WPN_ATTM_MAX; j++)
		{
			uint32_t itm = loadout_->Attachment[i].attachments[j];
			if(itm == 0)
				continue;

			if(g_pWeaponArmory->getAttachmentConfig(itm) == NULL)
			{
				gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Data, false, "ValidateAttachments",
					"%d", itm);

				loadout_->Attachment[i].attachments[j] = 0;

				haveBadBackpack_ = 1;
			}
		}
	}
}

void obj_ServerPlayer::DoDeath()
{
	gServerLogic.LogInfo(peerId_, "Death", ""); CLOG_INDENT;

	deathTime     = r3dGetTime();

	// drop all items
	for(int i=0; i<loadout_->BackpackSize; i++)
	{
		const wiInventoryItem& wi = loadout_->Items[i];
		if(wi.itemID > 0) {
			BackpackDropItem(i);
		}
	}

	// drop not-default backpack as well
	if(loadout_->BackpackID != 20176)
	{
		// create network object
		obj_DroppedItem* obj = (obj_DroppedItem*)srv_CreateGameObject("obj_DroppedItem", "obj_DroppedItem",GetRandomPosForItemDrop() );
		obj->SetNetworkID(gServerLogic.GetFreeNetId());
		obj->NetworkLocal = true;
		// vars
		obj->m_Item.itemID   = loadout_->BackpackID;
		obj->m_Item.quantity = 1;
		//obj->m_Item.Durability = 100;
	}

	// set that character is dead
	loadout_->Alive   = 0;
	loadout_->GamePos = GetPosition();
	loadout_->Health  = 0;
	// clear attachments
	loadout_->Attachment[0].Reset();
	loadout_->Attachment[1].Reset();
	//NOTE: server WZ_Char_SRV_SetStatus will clear player backpack, so make that CJobUpdateChar::Exec() won't update it
	savedLoadout_ = *loadout_;

	gServerLogic.ApiPlayerUpdateChar(this);

	SetLatePacketsBarrier("death");

	return;
}

wiStatsTracking obj_ServerPlayer::AddReward(const wiStatsTracking& in_rwd)
{
  float XPBonus = boostXPBonus_;
  float WPBonus = boostWPBonus_;

  wiStatsTracking rwd = in_rwd;
  // round up. basically if we award only 2 points, with +25% it would give us 0, so, let's make users more happy by round up
  // in case if that will be a balancing problem - we can always round it down with floorf
  rwd.XP += int(ceilf(R3D_ABS(rwd.XP)*XPBonus));
  rwd.GP += int(ceilf(rwd.GP*WPBonus));
  rwd.GD += int(ceilf(rwd.GD*WPBonus));

  // adjust player stats
  profile_.ProfileData.GamePoints  += rwd.GP;
  profile_.ProfileData.GameDollars += rwd.GD;
  loadout_->Stats.XP += rwd.XP;

  return rwd;
}

wiNetWeaponAttm	obj_ServerPlayer::GetWeaponNetAttachment(int wid)
{
	wiNetWeaponAttm atm;

	const ServerWeapon* wpn = m_WeaponArray[wid];
	if(!wpn)
		return atm;

	if(wpn->m_Attachments[WPN_ATTM_LEFT_RAIL])
		atm.LeftRailID = wpn->m_Attachments[WPN_ATTM_LEFT_RAIL]->m_itemID;
	if(wpn->m_Attachments[WPN_ATTM_MUZZLE])
		atm.MuzzleID = wpn->m_Attachments[WPN_ATTM_MUZZLE]->m_itemID;

	return atm;
}

float obj_ServerPlayer::CalcWeaponDamage(const r3dPoint3D& shootPos)
{
  // calc damaged based on weapon
  // decay damage based from distance from player to target
  float dist   = (GetPosition() - shootPos).Length();
  float damage = 0;
  if(m_WeaponArray[m_SelectedWeapon])
	  damage = m_WeaponArray[m_SelectedWeapon]->calcDamage(dist);

  return damage;
}

bool obj_ServerPlayer::FireWeapon(int wid, bool wasAiming, int executeFire, DWORD targetId, const char* pktName/*, int Durability*/)
{
	r3d_assert(loadout_->Alive);

	lastPlayerAction_ = r3dGetTime();

	if(targetId)
	{
		GameObject* targetObj = GameWorld().GetNetworkObject(targetId);
		if(targetObj == NULL)
		{
			// target already disconnected (100% cases right now) or invalid.
			return false;
		}
	}

	if(wid < 0 || wid >= NUM_WEAPONS_ON_PLAYER)
	{
		gServerLogic.LogInfo(peerId_, "wid invalid", "%s %d", pktName, wid);
		return false;
	}

	if(wid != m_SelectedWeapon)
	{
		// just log for now... we'll see how much mismatches we'll get
		gServerLogic.LogInfo(peerId_, "wid mismatch", "%s %d vs %d", pktName, wid, m_SelectedWeapon);
	}

	if(m_ForcedEmptyHands)
	{
		gServerLogic.LogInfo(peerId_, "empty hands", "%s %d vs %d", pktName, wid, m_SelectedWeapon);
		return false;
	}

	ServerWeapon* wpn = m_WeaponArray[wid];
	if(wpn == NULL)
	{
		gServerLogic.LogInfo(peerId_, "no wid", "%s %d", pktName, wid);
		return false;
	}

	if (wpn->getConfig()->m_itemID == WeaponConfig::ITEMID_ClayMore)
		return false;

	// can't fire in safe zones
    if((loadout_->GameFlags & wiCharDataFull::GAMEFLAG_NearPostBox) || (loadout_->GameFlags & wiCharDataFull::GAMEFLAG_SpawnProtection))
    {
		return false;
	}

	// melee - infinite ammo
	if(wpn->getCategory() == storecat_MELEE)
	{
		gServerLogic.TrackWeaponUsage(wpn->getConfig()->m_itemID, 1, 0, 0);

		FireHitCount++;
		return true;
	}

	if(wpn->getCategory() == storecat_GRENADE) // grenades are treated as items
	{
		gServerLogic.TrackWeaponUsage(wpn->getConfig()->m_itemID, 1, 0, 0);

		wiInventoryItem& wi = wpn->getPlayerItem();
		//wi.Durability = Durability;

		// remove used item
		wi.quantity--;
		if(wi.quantity <= 0) {
			wi.Reset();
		}

		return true;
	}

	if(wpn->getClipConfig() == NULL)
	{
		gServerLogic.LogInfo(peerId_, "no clip", "%s %d", pktName, wid);
		return false;
	}

	// incr fire count, decremented on hit event
	//r3dOutToLog("FireHitCount++\n");
	FireHitCount++;

	// track ShotsFired
	loadout_->Stats.ShotsFired++;

	gServerLogic.TrackWeaponUsage(wpn->getConfig()->m_itemID, 1, 0, 0);

	if(executeFire && gServerLogic.weaponDataUpdates_ < 2)
	{
		// check if we fired more that we was able
		wiInventoryItem& wi = wpn->getPlayerItem();
		if(wi.Var1 <= 0)
		{
			gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_NumShots, true, "bullethack",
				"%d/%d clip:%d(%s)",
				wi.Var1,
				wpn->getClipConfig()->m_Clipsize,
				wpn->getClipConfig()->m_itemID,
				wpn->getClipConfig()->m_StoreName
				);
			return false;
		}

		wpn->getPlayerItem().Var1--;
		//wpn->getPlayerItem().Durability = Durability;
	}

	return true;
}

float obj_ServerPlayer::ReduceDamageByGear(int gslot, float damage)
{
  if(gears_[gslot] == NULL)
    return damage;

  float armorProtectionMul = 1.0f;
  float armorProtectionAdd = 0.0f;

  float new_damage = gears_[gslot]->GetDamage(damage, armorProtectionMul, armorProtectionAdd, isTargetDummy_);
  return new_damage;
}

// params should be in [0..360] range
float getMinimumAngleDistance(float from, float to)
{
	float d = to - from;
	if(d <-180.0f)	d += 360.0f;
	if(d > 180.0f)	d -= 360.0f;
	return d;
}


float obj_ServerPlayer::ApplyDamage(float damage, GameObject* fromObj, int bodyPart, STORE_CATEGORIES damageSource)
{
  lastTimeHit     = r3dGetTime();
  lastHitBodyPart = bodyPart;

  //r3dOutToLog("Player(%s) received gamage\n", userName); CLOG_INDENT;
  //r3dOutToLog("raw damage(%.2f) at part (%d)\n", damage, bodyPart);

  // adjust damage based on hit part
  if(damageSource != storecat_MELEE)
  {
	  switch(bodyPart)
	  {
	  case 1: // head
		  damage *= 2;
		  break;

		  // case 2: // hands
	  case 3: // legs
		  damage *= 0.75f;
		  break;
	  }
  }

  // reduce damage by armor
  switch(bodyPart)
  {
    case 1: // head
      damage = ReduceDamageByGear(SLOT_Headgear, damage);
      break;

    default:
      //damage = ReduceDamageByGear(SLOT_Char,  damage);
      damage = ReduceDamageByGear(SLOT_Armor, damage);
      break;
  }


 	if(loadout_->Stats.skillid0 == 1)
  	{
		//r3dOutToLog("Player %s has learned Skill: Alive and Well 1 - removing 0.1% from Damage!\n", loadout_->Gamertag);
		damage *= 0.9f;
		if(loadout_->Stats.skillid7 == 1)
			damage *= 0.8f;
 	}
 // r3dOutToLog("gear adjusted damage(%.2f)\n", damage);

  if(damage < 0)
    damage = 0;

  // reduce health, now works for target dummy as well!
  if(!isTargetDummy_ || 1)
	loadout_->Health -= damage;

	loadout_->Health = R3D_CLAMP(loadout_->Health, 0.0f, 100.0f);

	//r3dOutToLog("########## %s damaged by %s by %.1f points\n", userName, fromObj->Name.c_str(), damage);

  return damage;
}

void obj_ServerPlayer::SetWeaponSlot(int wslot, uint32_t weapId, const wiWeaponAttachment& attm)
{
	r3d_assert(wslot < NUM_WEAPONS_ON_PLAYER);
	SAFE_DELETE(m_WeaponArray[wslot]);

	if(weapId == 0)
		return;

	const WeaponConfig* weapCfg = g_pWeaponArmory->getWeaponConfig(weapId);
	if(weapCfg == NULL) {
		r3dOutToLog("!!! %s does not have weapon id %d\n", userName, weapId);
    		return;
	}

	//r3dOutToLog("Creating wpn %s\n", weapCfg->m_StoreName); CLOG_INDENT;
	m_WeaponArray[wslot] = new ServerWeapon(weapCfg, this, wslot, attm);

	if(weapCfg->category != storecat_MELEE)
	{
		if(m_WeaponArray[wslot]->getClipConfig() == NULL) {
			r3dOutToLog("!!! weapon id %d does not have default clip attachment\n", weapId);
		}
	}

	return;
}

void obj_ServerPlayer::SetGearSlot(int gslot, uint32_t gearId)
{
	r3d_assert(gslot >= 0 && gslot < SLOT_Max);
	SAFE_DELETE(gears_[gslot]);

	if(gearId == 0)
		return;

	if(g_pWeaponArmory->getGearConfig(gearId) == NULL) {
		r3dOutToLog("!!! %s does not have gear id %d\n", userName, gearId);
    		return;
	}

	gears_[gslot] = g_pWeaponArmory->createGear(gearId);
	return;
}

void obj_ServerPlayer::SetLoadoutData()
{
	wiCharDataFull& slot = profile_.ProfileData.ArmorySlots[0];
	slot.GroupID = 0;

	//@ FOR NOW, attachment are RESET on entry. need to detect if some of them was dropped
	// (SERVER CODE SYNC POINT)
	slot.Attachment[0] = wiWeaponAttachment();
	if(slot.Items[0].Var2 > 0)
		slot.Attachment[0].attachments[WPN_ATTM_CLIP] = slot.Items[0].Var2;

	slot.Attachment[1] = wiWeaponAttachment();
	if(slot.Items[1].Var2 > 0)
		slot.Attachment[1].attachments[WPN_ATTM_CLIP] = slot.Items[1].Var2;

	SetWeaponSlot(0, slot.Items[wiCharDataFull::CHAR_LOADOUT_WEAPON1].itemID, slot.Attachment[0]);
	SetWeaponSlot(1, slot.Items[wiCharDataFull::CHAR_LOADOUT_WEAPON2].itemID, slot.Attachment[1]);

	//SetGearSlot(SLOT_Char,     slot.HeroItemID);
	SetGearSlot(SLOT_Armor,    slot.Items[wiCharDataFull::CHAR_LOADOUT_ARMOR].itemID);
	SetGearSlot(SLOT_Headgear, slot.Items[wiCharDataFull::CHAR_LOADOUT_HEADGEAR].itemID);

	return;
}

BOOL obj_ServerPlayer::Update()
{
	parent::Update();
	CheckData();
	for( GameObject* obj = GameWorld().GetFirstObject(); obj; obj = GameWorld().GetNextObject(obj) ) // Server Vehicles
	{
		if(obj->isObjType(OBJTYPE_Vehicle))
		{
				obj_Vehicle* Vehicles = (obj_Vehicle*)obj;
				Vehicles->Update();
		} 
		if (obj->Class->Name == "obj_ServerBarricade") //safelock
		{
			obj_ServerBarricade* shield = (obj_ServerBarricade*)obj;
			for( GameObject* pl = GameWorld().GetFirstObject(); pl; pl = GameWorld().GetNextObject(pl) )
			{
				if(pl->Class->Name == "obj_Vehicle")
				{
					if (shield->m_ItemID == 101416)
					{
						obj_Vehicle* Vehicle = (obj_Vehicle*)pl;
						float dist   = (Vehicle->GetPosition() - shield->GetPosition()).Length();
						if (dist<3)
						{
							if (Vehicle && Vehicle->OccupantsVehicle>0 && !shield->killed)
							{
								shield->killed=true;
								PKT_C2S_ClayMoreDestroy_s n;
								n.Pos = shield->GetPosition();
								n.ID = shield->GetNetworkID();
								gServerLogic.p2pSendToPeer(this->peerId_,this, &n, sizeof(n), true);
	
								PKT_C2S_DamageCar_s n2;
								n2.CarID = Vehicle->GetNetworkID();
								n2.WeaponID =100999;
								gServerLogic.p2pSendToPeer(this->peerId_,this, &n2, sizeof(n2), true);									
								break;
							}
						}
					}
				}
		if (isTradeAccept)
		{
			GameObject* obj = GameWorld().GetNetworkObject(Tradetargetid);
			if (IsServerPlayer(obj))
			{
				obj_ServerPlayer* plr = (obj_ServerPlayer*)obj;
				if (plr)
				{
					if (plr->isTradeAccept && isTradeAccept && plr->Tradetargetid == GetNetworkID() && Tradetargetid == plr->GetNetworkID() && plr != this && plr->Tradetargetid != 0 && Tradetargetid != 0)
					{
						// start tradeing..
						this->DoTrade(this,plr);


						// trade finished 
						for (int i=0;i<72;i++)
						{
							this->TradeSlot[i] = 0;
							this->TradeItems[i].Reset();
							plr->TradeSlot[i] = 0;
							plr->TradeItems[i].Reset();    
						}


						this->Tradetargetid = 0;
						plr->Tradetargetid = 0;
						this->isTradeAccept = false;
						plr->isTradeAccept = false;
						PKT_C2S_TradeSuccess_s n;
						gServerLogic.p2pSendToPeer(plr->peerId_,plr,&n,sizeof(n));
						gServerLogic.p2pSendToPeer(peerId_,this,&n,sizeof(n));
					}
				}
			}
		}
				if(pl->isObjType(OBJTYPE_Human))
				{
					obj_ServerPlayer* Player= static_cast< obj_ServerPlayer* > ( pl );
					if (shield->m_ItemID == 101416)
					{
						float dist   = (Player->GetPosition() - shield->GetPosition()).Length();
						if (dist<2)
						{
							if (!shield->killed && shield->TimeToOut != 0 && (r3dGetTime() - shield->TimeToOut) > 5)
							{
								PKT_C2S_ClayMoreDestroy_s n;
								n.Pos = shield->GetPosition();
								n.ID = shield->GetNetworkID();
								gServerLogic.p2pSendToPeer(this->peerId_,this, &n, sizeof(n), true);
								shield->killed=true;
							}
						}
						if (dist<8)
						{
							if (shield->killed)
							{
								for( GameObject* pl2 = GameWorld().GetFirstObject(); pl2; pl2 = GameWorld().GetNextObject(pl2) )
								{
									if(pl2->isObjType(OBJTYPE_Human))
									{
										obj_ServerPlayer* Player2= static_cast< obj_ServerPlayer* > ( pl2 );
										if (Player2)
										{
											float dist   = (Player2->GetPosition() - shield->GetPosition()).Length();
											if (dist<8)
											{
												if (Player->GetNetworkID() == this->GetNetworkID())
												{
													if (loadout_->GameMapId != GBGameInfo::MAPID_WZ_PVE_Colorado && !profile_.ProfileData.isGod && !(loadout_->GameFlags & wiCharDataFull::GAMEFLAG_SpawnProtection) && !(loadout_->GameFlags & wiCharDataFull::GAMEFLAG_NearPostBox))
														gServerLogic.ApplyDamage(this, this, GetPosition(), 10.0f, true, storecat_Vehicle);
												}
											}
										}
									}
								}
							}
						}
					}
				}

				if(pl->isObjType(OBJTYPE_Zombie))
				{
					obj_Zombie* Zombie= static_cast< obj_Zombie* > ( pl );
					if (shield->m_ItemID == 101416)
					{
						float dist   = (Zombie->GetPosition() - shield->GetPosition()).Length();
						if (dist<2 && Zombie->ZombieHealth >0)
						{
							if (!shield->killed && shield->TimeToOut != 0 && (r3dGetTime() - shield->TimeToOut) > 5)
							{
								PKT_C2S_ClayMoreDestroy_s n;
								n.Pos = shield->GetPosition();
								n.ID = shield->GetNetworkID();
								gServerLogic.p2pSendToPeer(this->peerId_,this, &n, sizeof(n), true);
								shield->killed=true;

							}
						}
						if (dist<8)
						{
							if (shield->killed)
							{
								for( GameObject* pl2 = GameWorld().GetFirstObject(); pl2; pl2 = GameWorld().GetNextObject(pl2) )
								{
									if(pl2->isObjType(OBJTYPE_Zombie))
									{
										obj_Zombie* Zombie2= static_cast< obj_Zombie* > ( pl2 );
										if (Zombie2)
										{
											float dist   = (Zombie2->GetPosition() - shield->GetPosition()).Length();
											if (dist<8)
												gServerLogic.ApplyDamageToZombie(this,Zombie2,GetPosition()+r3dPoint3D(0,1,0),100, 1, 1, false, storecat_ShootVehicle);
										}
									}
								}
							}
						}
					}
				}
			}
			if (shield->killed == true && shield->m_ItemID == 101416)
			{
				shield->DoDamage(1000);
			}
			if (shield->m_ItemID == 101348)
			{
				__int64 secs1 = _time64(&secs1);
				if (secs1 > shield->ExpireTime && strcmp(shield->Password,"") != 0)
				{
					strcpy(shield->Password,"");
					gServerLogic.RemoveDBSafelock(this,shield->SafeLockID,0,0,0,0,shield->GameServerID);
					r3dOutToLog("#### The SafeLockID %i is expired\n",shield->SafeLockID);
					PKT_C2S_SafelockData_s n;
					n.Status = 7;
					n.IDNetwork=toP2pNetId(shield->GetNetworkID());
					gServerLogic.p2pSendToPeer(this->peerId_,this, &n, sizeof(n), true);
					shield->DoDamage(10000000000);
				}
			}
		}
	}
	if (loadout_->GroupID != 0)
	{
		if ((r3dGetTime() - GroupTime) > 10)
		{
			GroupTime = r3dGetTime();
			for( GameObject* obj = GameWorld().GetFirstObject(); obj; obj = GameWorld().GetNextObject(obj) ) // Server Vehicles
			{
				if(obj->isObjType(OBJTYPE_Human))
				{
					obj_ServerPlayer* Player= static_cast< obj_ServerPlayer* > ( obj );
					if (Player->loadout_->GroupID == loadout_->GroupID)
					{
						PKT_S2C_GroupData_s n;
						n.State = 7;
						n.IDPlayer=Player->GetNetworkID();
						n.Position=Player->GetPosition();
						gServerLogic.p2pSendToPeer(this->peerId_,this, &n, sizeof(n), true);
					}
				}
			}
		}
	}
	else {
		GroupTime = r3dGetTime();
	}
	const float timePassed = r3dGetFrameTime();
	const float curTime = r3dGetTime();

	//We introduce the 1 minute time limit for our repellent
	//Zombie Repellent Expires after one minute
	const float RepellentExpires = 60; //Repellent expires in one minute
	if(loadout_->Repellent==1){
		if(curTime > loadout_->UsedRepellentAt + RepellentExpires)
		{
			loadout_->Repellent = 0;
		}
	}

	if (r3dGetTime() >  LastHackShieldLog + 20.0f && (((r3dGetTime() - startPlayTime_) >= 90.0f)) && loadout_->Alive != 0)
	{
		PKT_S2C_CheatMsg_s n2;
		r3dscpy(n2.cheatreason,"HackShield Kicked from server. : HackShield Service not responding.");
		gServerLogic.p2pSendToPeer(peerId_, this, &n2, sizeof(n2));

		gServerLogic.DisconnectPeer(peerId_, false, "HackShield Late.");
	}

	// pereodically update network objects visibility
	if(curTime > lastVisUpdateTime_ + 0.3f)
	{
		lastVisUpdateTime_ = r3dGetTime();
		gServerLogic.UpdateNetObjVisData(this);
	}

	if(loadout_->Alive == 0)
	{
		return TRUE;
	}

	if(wasDisconnected_)
		return TRUE;

	// disconnect player after few ticks if he had bad items in inventory
	if(haveBadBackpack_)
	{
		if(++haveBadBackpack_ > 5)
		{
			haveBadBackpack_ = 0;
			gServerLogic.DisconnectPeer(peerId_, false, "haveBadBackpack");
			return TRUE;
		}
	}

	// give x4 time for weapon packet to arrive (make sure it's bigger that r3dNetwork peers disconnect)
	if(!isTargetDummy_ && curTime > (lastWeapDataRep + PKT_C2S_PlayerWeapDataRep_s::REPORT_PERIOD * 4))
	{
		gServerLogic.DisconnectPeer(peerId_, true, "no weapdatarep");
		return TRUE;
	}

	// STAT LOGIC
	{
		if(loadout_->Toxic < 100)
		{
			if(loadout_->Toxic > GPP_Data.c_fBloodToxicIncLevel2)
				loadout_->Toxic+= timePassed*GPP_Data.c_fBloodToxicIncLevel2Value;
			else if(loadout_->Toxic > GPP_Data.c_fBloodToxicIncLevel1)
				loadout_->Toxic+= timePassed*GPP_Data.c_fBloodToxicIncLevel1Value;
		}

		if(loadout_->Thirst < 100)
		{
			if(m_PlayerState == PLAYER_MOVE_SPRINT)
				loadout_->Thirst += timePassed*GPP_Data.c_fThirstSprintInc;
			else
				loadout_->Thirst += timePassed*GPP_Data.c_fThirstInc;
			if(loadout_->Toxic > GPP_Data.c_fThirstHighToxicLevel)
				loadout_->Thirst += timePassed*GPP_Data.c_fThirstHighToxicLevelInc;
		}
		if(loadout_->Hunger < 100)
		{

			float HungerSprint = timePassed*GPP_Data.c_fHungerSprintInc; //2,5

			float HungerRun		= timePassed*GPP_Data.c_fHungerRunInc;  // 1.5
			float HungerNorm	= timePassed*GPP_Data.c_fHungerInc;    // 1.5
			float HungerToxici  = timePassed*GPP_Data.c_fHungerHighToxicLevelInc; // 3.0

			if(loadout_->Stats.skillid19 == 1){
				HungerSprint = HungerSprint - 0.000161f;
				HungerRun = HungerRun - 0.000161f;
				HungerNorm = HungerNorm - 0.000161f;
				HungerToxici = HungerToxici - 0.000161f;
				if(loadout_->Stats.skillid26 == 1){
					HungerSprint = HungerSprint - 0.000161f;
					HungerRun = HungerRun + 0.000161f;
					HungerNorm = HungerNorm + 0.000161f;
					HungerToxici = HungerToxici + 0.000161f;
				}
				// Skillsystem 19
			}

			if(m_PlayerState == PLAYER_MOVE_SPRINT)
				loadout_->Hunger += timePassed*GPP_Data.c_fHungerSprintInc;
			else if(m_PlayerState == PLAYER_MOVE_RUN)
				loadout_->Hunger += timePassed*GPP_Data.c_fHungerRunInc;
			else
				loadout_->Hunger += timePassed*GPP_Data.c_fHungerInc;
			if(loadout_->Toxic > GPP_Data.c_fHungerHighToxicLevel)
				loadout_->Hunger += timePassed*GPP_Data.c_fHungerHighToxicLevelInc;
		}

		if(loadout_->Toxic > GPP_Data.c_fBloodToxicLevel3)
			loadout_->Health -= timePassed*GPP_Data.c_fBloodToxicLevel3_HPDamage;
		else if(loadout_->Toxic > GPP_Data.c_fBloodToxicLevel2)
			loadout_->Health -= timePassed*GPP_Data.c_fBloodToxicLevel2_HPDamage;
		else if(loadout_->Toxic > GPP_Data.c_fBloodToxicLevel1)
			loadout_->Health -= timePassed*GPP_Data.c_fBloodToxicLevel1_HPDamage;
		if(loadout_->Hunger > GPP_Data.c_fHungerLevel1)
			loadout_->Health -= timePassed*GPP_Data.c_fHungerLevel_HPDamage;
		if(loadout_->Thirst > GPP_Data.c_fThirstLevel1)
			loadout_->Health -= timePassed*GPP_Data.c_fThirstLevel_HPDamage;

		if(loadout_->Health <= 0.0f || loadout_->Health != loadout_->Health)
		{
			gServerLogic.DoKillPlayer(this, this, storecat_INVALID, true);
			return TRUE;
		}
	}

	// STAMINA LOGIC SHOULD BE SYNCED WITH CLIENT CODE!
	// (stamina penalty and bOnGround is not synced with server, as it will not cause desync for non cheating client)
	{
		const float TimePassed = R3D_MIN(r3dGetFrameTime(), 0.1f);
		if(m_PlayerState == PLAYER_MOVE_SPRINT)
		{
			m_Stamina -= TimePassed;
			if(m_Stamina < -60.0f) // allow one minute of stamina cheating
			{
				gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Stamina, true, "stamina");
			}
		}
		else
		{
			float regen_rate = loadout_->Health<50?GPP_Data.c_fSprintRegenRateLowHealth:GPP_Data.c_fSprintRegenRateNormal;
				if(loadout_->Stats.skillid3 == 1){
					regen_rate *= 2.0f;
					if(loadout_->Stats.skillid5 == 1){
						regen_rate *= 3.0f;
				}
			}
			m_Stamina += TimePassed*regen_rate; // regeneration rate
		}
		m_Stamina = R3D_CLAMP((float)m_Stamina, 0.0f, GPP_Data.c_fSprintMaxEnergy);

	  if (loadout_->GameFlags & wiCharDataFull::GAMEFLAG_RadioactiveArea) // Radiactive
	  {
		  loadout_->Toxic += 10.0f;
		  if (loadout_->Toxic>100.0f)
		  {
			  loadout_->Toxic=100.0f;
			  loadout_->Health=0.0f;
		  }
		  loadout_->GameFlags = 0;
	  }
	}

	// send vitals if they're changed
	PKT_S2C_SetPlayerVitals_s vitals;
	vitals.FromChar(loadout_);

	vitals.GroupID = loadout_->GroupID;
	if(vitals != lastVitals_ || vitals.GroupID != lastVitals_.GroupID)
	{
		gServerLogic.p2pBroadcastToActive(this, &vitals, sizeof(vitals));
		lastVitals_.FromChar(loadout_);
	}

	const float CHAR_UPDATE_INTERVAL = 60;
	if(curTime > lastCharUpdateTime_ + CHAR_UPDATE_INTERVAL)
	{
		lastCharUpdateTime_ = curTime;
		gServerLogic.ApiPlayerUpdateChar(this);
	}

	const float WORLD_UPDATE_INTERVAL = 0.5f;
	if(curTime > lastWorldUpdateTime_ + WORLD_UPDATE_INTERVAL)
	{
		lastWorldUpdateTime_ = curTime;
		UpdateGameWorldFlags();
	}

	//if(CheckForFastMove()) // DESACTIVADO TEMPORALMENTE
	//	return TRUE;

	// anti cheat: player is under the ground, or player is flying above the ground
	/*{
		//PxRaycastHit hit;
		PxSweepHit hit;
		PxSceneQueryFilterData filter(PxFilterData(COLLIDABLE_PLAYER_COLLIDABLE_MASK, 0, 0, 0), PxSceneQueryFilterFlag::eSTATIC);
		r3dVector pos = GetPosition();
		PxBoxGeometry boxg(0.5f, 0.1f, 0.5f);
		PxTransform pose(PxVec3(pos.x, pos.y+0.5f, pos.z));
		if(!g_pPhysicsWorld->PhysXScene->sweepSingle(boxg, pose, PxVec3(0,-1,0), 2000.0f, PxSceneQueryFlag::eDISTANCE|PxSceneQueryFlag::eINITIAL_OVERLAP|PxSceneQueryFlag::eINITIAL_OVERLAP_KEEP, hit, filter))
		{
			m_PlayerUndergroundAntiCheatTimer += r3dGetFrameTime();
			if(m_PlayerUndergroundAntiCheatTimer > 2.0f)
			{
				gServerLogic.DoKillPlayer(this, this, storecat_INVALID, true);
				gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Flying, false, "player underground - killing", "%.2f, %.2f, %.2f", pos.x, pos.y, pos.z);
			}
		}
		else
		{
			if(m_PlayerUndergroundAntiCheatTimer > 0)
				m_PlayerUndergroundAntiCheatTimer -= r3dGetFrameTime();

			float dist = hit.distance;
			//r3dOutToLog("@@@@ dist=%.2f\n", dist);
			if(dist > 5.1f) // higher than 1.6 meter above ground
			{
				// check if he is not falling, with some safe margin in case if he is walking down the hill
				if( (oldstate.Position.y - GetPosition().y) < 0.1f )
				{
					m_PlayerFlyingAntiCheatTimer += r3dGetFrameTime();
					if(m_PlayerFlyingAntiCheatTimer > 5.0f)
					{
						gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Flying, true, "player flying", "dist=%.2f, pos=%.2f, %.2f, %.2f", dist, pos.x, pos.y, pos.z);
					}
				}
				else if(m_PlayerFlyingAntiCheatTimer > 0.0f)
					m_PlayerFlyingAntiCheatTimer-=r3dGetFrameTime(); // slowly decrease timer
			}
		}
	}*/

// 	// afk kick
// 	const float AFK_KICK_TIME_IN_SEC = 90.0f;
// 	if(!isTargetDummy_ && curTime > lastPlayerAction_ + AFK_KICK_TIME_IN_SEC)
// 	{
// 		if(profile_.ProfileData.isDevAccount)
// 		{
// 			// do nothing for admin accs
// 		}
// 		else
// 		{
// 			PKT_S2C_CheatWarning_s n;
// 			n.cheatId = PKT_S2C_CheatWarning_s::CHEAT_AFK;
// 			gServerLogic.p2pSendToPeer(peerId_, this, &n, sizeof(n), true);
//
// 			gServerLogic.DisconnectPeer(peerId_, false, "afk_kick");
// 			return TRUE;
// 		}
// 	}


	return TRUE;
}

void obj_ServerPlayer::CheckData()
{
	if ((r3dGetTime() - CheckDatas) > 5)
	{
		CheckDatas=r3dGetTime();
		CWOBackendReq req("api_GetRentServers.aspx");
		req.AddParam("411", "1");

		if(!req.Issue())
		{
			r3dOutToLog("Read RentServers FAILED, code: %d\n", req.resultCode_);
			return;
		}

		pugi::xml_document xmlFile;
		req.ParseXML(xmlFile);
		pugi::xml_node xmlSafelock = xmlFile.child("Rent_Servers");
		
		while(!xmlSafelock.empty())
		{		
			uint32_t gameServerId = xmlSafelock.attribute("gameServerId").as_uint();
			uint32_t ExpireTime = xmlSafelock.attribute("ExpireTime").as_uint();
			uint32_t Crosshair = xmlSafelock.attribute("Crosshair").as_uint();
			uint32_t SNP = xmlSafelock.attribute("SNP").as_uint();

			if (gServerLogic.ginfo_.gameServerId == gameServerId && ExpireTime>0)
			{
				if ((int)enableCrosshairServer != (int)Crosshair || (int)enableSNP != (int)SNP)
				{
					enableCrosshairServer = (Crosshair ==1)?true:false;
					enableSNP = (SNP == 1)?true:false;

					PKT_S2C_SendDataRent_s n;
					n.Crosshair = enableCrosshairServer;
					n.SNP   = enableSNP;
					gServerLogic.p2pSendToPeer(this->peerId_, this, &n, sizeof(n));
				}
			}

			xmlSafelock = xmlSafelock.next_sibling();
		}
	}
}

void obj_ServerPlayer::RecalcBoundBox()
{
  float	x_size = 0.8f;
  float	z_size = x_size;
  float	y_size = Height;

  r3dPoint3D pos = GetPosition();
  r3dBoundBox bboxlocal;
  bboxlocal.Org.Assign(pos.X - x_size / 2, pos.Y, pos.Z - z_size / 2);
  bboxlocal.Size.Assign(x_size, y_size, z_size);
  SetBBoxLocal(bboxlocal);

  return;
}

BOOL obj_ServerPlayer::OnCollide(GameObject *tobj, CollisionInfo &trace)
{
  return TRUE;
}

void obj_ServerPlayer::UpdateGameWorldFlags()
{
	loadout_->GameFlags = 0;

    if((((r3dGetTime() - startPlayTime_) <= 30.0f))) // 30 second spawn protection
        loadout_->GameFlags |= wiCharDataFull::GAMEFLAG_SpawnProtection;

	// scan for near postboxes
	for(int i=0; i<gPostBoxesMngr.numPostBoxes_; i++)
	{
		obj_ServerPostBox* pbox = gPostBoxesMngr.postBoxes_[i];
		float dist = (GetPosition() - pbox->GetPosition()).Length();
		if(dist < pbox->useRadius)
		{
			loadout_->GameFlags |= wiCharDataFull::GAMEFLAG_NearPostBox;
			break;
		}
	}

	if (loadout_->GameFlags & wiCharDataFull::GAMEFLAG_SpawnProtection && loadout_->GameFlags & wiCharDataFull::GAMEFLAG_NearPostBox || loadout_->GameFlags & wiCharDataFull::GAMEFLAG_SpawnProtection || loadout_->GameFlags & wiCharDataFull::GAMEFLAG_NearPostBox)
	{
		// If you in SafeZone, or have Spawn Protection, you not Have Radioactive
	}
	else if(this->PlayerOnVehicle == true || this->profile_.ProfileData.isGod || loadout_->Items[wiCharDataFull::CHAR_LOADOUT_HEADGEAR].itemID == 20199|| loadout_->Items[wiCharDataFull::CHAR_LOADOUT_HEADGEAR].itemID == 20177 || loadout_->Items[wiCharDataFull::CHAR_LOADOUT_HEADGEAR].itemID == 20178 || loadout_->Items[wiCharDataFull::CHAR_LOADOUT_HEADGEAR].itemID == 20211 || loadout_->Items[wiCharDataFull::CHAR_LOADOUT_HEADGEAR].itemID == 20199)
	{
		    // If you Have GasMask You nat Have Radioactive
	}
	else{
		    // You not have GasMask. Scan for Radioactive Area
			for(int i=0; i<gRadioactiveAreaMngr.numRadioactiveArea_; i++)
			{
					obj_ServerRadioactiveArea* rbox = gRadioactiveAreaMngr.RadioactiveArea_[i];
					float dist = (GetPosition() - rbox->GetPosition()).Length();
					if(dist < rbox->useRadius)
					{
						loadout_->GameFlags |= wiCharDataFull::GAMEFLAG_RadioactiveArea;
						break;
					}
			}
	}
	if(lastWorldFlags_ != loadout_->GameFlags)
	{
		lastWorldFlags_ = loadout_->GameFlags;

		PKT_S2C_SetPlayerWorldFlags_s n;
		n.flags = loadout_->GameFlags;
		gServerLogic.p2pSendToPeer(peerId_, this, &n, sizeof(n));
	}

	return;
}

bool storecat_CanPlaceItemToSlot(const BaseItemConfig* itemCfg, int idx)
{
	bool canPlace = true;
	int cat = itemCfg->category;
	switch(idx)
	{
		case wiCharDataFull::CHAR_LOADOUT_WEAPON1:
			if(cat != storecat_ASR && cat != storecat_SNP && cat != storecat_SHTG && cat != storecat_MG && cat != storecat_SMG)// pt: temp, ui doesn't support that && cat != storecat_HG)
				canPlace = false;
			break;
		case wiCharDataFull::CHAR_LOADOUT_WEAPON2:
			if(cat != storecat_MELEE && cat != storecat_HG)
				canPlace = false;
			break;
		case wiCharDataFull::CHAR_LOADOUT_ITEM1:
		case wiCharDataFull::CHAR_LOADOUT_ITEM2:
		case wiCharDataFull::CHAR_LOADOUT_ITEM3:
		case wiCharDataFull::CHAR_LOADOUT_ITEM4:
			//if(cat != storecat_UsableItem)
			//	canPlace = false;
			canPlace = true;
			break;
		case wiCharDataFull::CHAR_LOADOUT_ARMOR:
			if(cat != storecat_Armor)
				canPlace = false;
			break;
		case wiCharDataFull::CHAR_LOADOUT_HEADGEAR:
			if(cat != storecat_Helmet)
				canPlace = false;
			break;
	}

	return canPlace;
}
bool obj_ServerPlayer::IsItemCanAddToInventory(const wiInventoryItem& wi1)
{
	const BaseItemConfig* itemCfg = g_pWeaponArmory->getConfig(wi1.itemID);
	if(!itemCfg) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Data, false, "BackpackAddItem",
			"%d", wi1.itemID);
		return false;
	}

	int slot_exist = -1;
	int slot_free  = -1;

	extern bool storecat_IsItemStackable(uint32_t ItemID);
	bool isStackable = storecat_IsItemStackable(wi1.itemID);
	for(int i=0; i<loadout_->BackpackSize; i++)
	{
		const wiInventoryItem& wi2 = loadout_->Items[i];

		// can stack only non-modified items
		if(isStackable && wi2.itemID == wi1.itemID && wi1.Var1 < 0 && wi2.Var1 < 0) {
			slot_exist = i;
			break;
		}

		// check if we can place that item to loadout slot
		bool canPlace = storecat_CanPlaceItemToSlot(itemCfg, i);
		if(canPlace && wi2.itemID == 0 && slot_free == -1) {
			slot_free = i;
			break;
		}
	}

	// check weight
	if (slot_exist >= 0 || slot_free >= 0)
	{
		float totalWeight = loadout_->getTotalWeight();
		if(loadout_->Stats.skillid2 == 1){
			totalWeight *= 0.9f;
			if(loadout_->Stats.skillid6 == 1)
				totalWeight *= 0.7f;
		}
		totalWeight += itemCfg->m_Weight*wi1.quantity;

		const BackpackConfig* bc = g_pWeaponArmory->getBackpackConfig(loadout_->BackpackID);
		if(totalWeight > bc->m_maxWeight)
		{
			PKT_S2C_BackpackModify_s n;
			n.SlotTo = 0xFE;

			gServerLogic.p2pSendToPeer(peerId_, this, &n, sizeof(n));

			return false;
		}
	}

	return (slot_exist >= 0 || slot_free >= 0);
}
bool obj_ServerPlayer::BackpackAddItem(const wiInventoryItem& wi1)
{
	// SPECIAL case - GOLD item
	if(wi1.itemID == 'GOLD')
	{
		//r3dOutToLog("%s BackpackAddItem %d GameDollars\n", userName, wi1.quantity); CLOG_INDENT;

		profile_.ProfileData.GameDollars += wi1.quantity;

		// report to client
		PKT_S2C_BackpackAddNew_s n;
		n.SlotTo = 0;
		n.Item   = wi1;
		gServerLogic.p2pSendToPeer(peerId_, this, &n, sizeof(n));
		return true;
	}

	//r3dOutToLog("%s BackpackAddItem %dx%d\n", userName, wi1.itemID, wi1.quantity); CLOG_INDENT;
	r3d_assert(wi1.itemID > 0);
	r3d_assert(wi1.quantity > 0);

	const BaseItemConfig* itemCfg = g_pWeaponArmory->getConfig(wi1.itemID);
	if(!itemCfg) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Data, false, "BackpackAddItem",
			"%d", wi1.itemID);
		return false;
	}

	int slot_exist = -1;
	int slot_free  = -1;

	extern bool storecat_IsItemStackable(uint32_t ItemID);
	bool isStackable = storecat_IsItemStackable(wi1.itemID);
	for(int i=0; i<loadout_->BackpackSize; i++)
	{
		const wiInventoryItem& wi2 = loadout_->Items[i];

		// can stack only non-modified items
		if(isStackable && wi2.itemID == wi1.itemID && wi1.Var1 < 0 && wi2.Var1 < 0) {
			slot_exist = i;
			break;
		}

		// check if we can place that item to loadout slot
		bool canPlace = storecat_CanPlaceItemToSlot(itemCfg, i);
		if(canPlace && wi2.itemID == 0 && slot_free == -1) {
			slot_free = i;
		}
	}

	if(slot_exist == -1 && slot_free == -1)
	{
		PKT_S2C_BackpackModify_s n;
		n.SlotTo = 0xFF;

		gServerLogic.p2pSendToPeer(peerId_, this, &n, sizeof(n));
		return false;
	}

	// check weight
	float totalWeight = loadout_->getTotalWeight();
	if(loadout_->Stats.skillid2 == 1){
		totalWeight *= 0.9f;
		if(loadout_->Stats.skillid6 == 1)
			totalWeight *= 0.7f;
	}
	totalWeight += itemCfg->m_Weight*wi1.quantity;

	const BackpackConfig* bc = g_pWeaponArmory->getBackpackConfig(loadout_->BackpackID);
	r3d_assert(bc);
	if(totalWeight > bc->m_maxWeight)
	{
		PKT_S2C_BackpackModify_s n;
		n.SlotTo = 0xFE;

		gServerLogic.p2pSendToPeer(peerId_, this, &n, sizeof(n));

		return false;
	}


	if(slot_exist >= 0)
	{
		// modify backpack
		r3d_assert(loadout_->Items[slot_exist].itemID == wi1.itemID);
		loadout_->Items[slot_exist].quantity += wi1.quantity;

		// report to client
		PKT_S2C_BackpackModify_s n;
		n.SlotTo     = (BYTE)slot_exist;
		n.Quantity   = (WORD)loadout_->Items[slot_exist].quantity;
		n.dbg_ItemID = loadout_->Items[slot_exist].itemID;
		gServerLogic.p2pSendToPeer(peerId_, this, &n, sizeof(n));

		OnBackpackChanged(slot_exist);

		return true;
	}

	if(slot_free >= 0)
	{
		// modify backpack
		r3d_assert(loadout_->Items[slot_free].itemID == 0);
		loadout_->Items[slot_free] = wi1;

		// report to client
		PKT_S2C_BackpackAddNew_s n;
		n.SlotTo = (BYTE)slot_free;
		n.Item   = wi1;
		gServerLogic.p2pSendToPeer(peerId_, this, &n, sizeof(n));

		OnBackpackChanged(slot_free);

		return true;
	}

	r3d_assert(false);
	return false;
}

r3dPoint3D obj_ServerPlayer::GetRandomPosForItemDrop()
{
	// create random position around player
	r3dPoint3D pos = GetPosition();
	if (this->PlayerOnVehicle==true)
	{
		pos+=r3dPoint3D(u_GetRandom(-4,4),-8,u_GetRandom(-4,4));
	}
	pos.y += 0.4f;
	pos.x += u_GetRandom(-1, 1);
	pos.z += u_GetRandom(-1, 1);

	return pos;
}

void obj_ServerPlayer::BackpackDropItem(int idx)
{
	wiInventoryItem& wi = loadout_->Items[idx];
	r3d_assert(wi.itemID);

	// create network object
	obj_DroppedItem* obj = (obj_DroppedItem*)srv_CreateGameObject("obj_DroppedItem", "obj_DroppedItem", GetRandomPosForItemDrop());
	obj->SetNetworkID(gServerLogic.GetFreeNetId());
	obj->NetworkLocal = true;
	// vars
	obj->m_Item       = wi;

	// remove from remote inventory
	PKT_S2C_BackpackModify_s n;
	n.SlotTo     = idx;
	n.Quantity   = 0;
	n.dbg_ItemID = wi.itemID;
	gServerLogic.p2pSendToPeer(peerId_, this, &n, sizeof(n));

	// remove from local inventory
	wi.Reset();
}

void obj_ServerPlayer::OnBackpackChanged(int idx)
{
	// if slot changed is related to loadout - relay to other players
	switch(idx)
	{
		case wiCharDataFull::CHAR_LOADOUT_WEAPON1:
		case wiCharDataFull::CHAR_LOADOUT_WEAPON2:
			// attachments are reset on item change (SERVER CODE SYNC POINT)
			loadout_->Attachment[idx].Reset();
			if(loadout_->Items[idx].Var2 > 0)
				loadout_->Attachment[idx].attachments[WPN_ATTM_CLIP] = loadout_->Items[idx].Var2;

			SetWeaponSlot(idx, loadout_->Items[idx].itemID, loadout_->Attachment[idx]);
			OnLoadoutChanged();
			break;

		case wiCharDataFull::CHAR_LOADOUT_ARMOR:
			SetGearSlot(SLOT_Armor, loadout_->Items[idx].itemID);
			OnLoadoutChanged();
			break;
		case wiCharDataFull::CHAR_LOADOUT_HEADGEAR:
			SetGearSlot(SLOT_Headgear, loadout_->Items[idx].itemID);
			OnLoadoutChanged();
			break;

		case wiCharDataFull::CHAR_LOADOUT_ITEM1:
		case wiCharDataFull::CHAR_LOADOUT_ITEM2:
		case wiCharDataFull::CHAR_LOADOUT_ITEM3:
		case wiCharDataFull::CHAR_LOADOUT_ITEM4:
			OnLoadoutChanged();
			break;
	}
}

void obj_ServerPlayer::OnLoadoutChanged()
{
	PKT_S2C_SetPlayerLoadout_s n;
	n.WeaponID0  = loadout_->Items[wiCharDataFull::CHAR_LOADOUT_WEAPON1].itemID;
	n.WeaponID1  = loadout_->Items[wiCharDataFull::CHAR_LOADOUT_WEAPON2].itemID;
	n.QuickSlot1 = loadout_->Items[wiCharDataFull::CHAR_LOADOUT_ITEM1].itemID;
	n.QuickSlot2 = loadout_->Items[wiCharDataFull::CHAR_LOADOUT_ITEM2].itemID;
	n.QuickSlot3 = loadout_->Items[wiCharDataFull::CHAR_LOADOUT_ITEM3].itemID;
	n.QuickSlot4 = loadout_->Items[wiCharDataFull::CHAR_LOADOUT_ITEM4].itemID;
	n.ArmorID    = loadout_->Items[wiCharDataFull::CHAR_LOADOUT_ARMOR].itemID;
	n.HeadGearID = loadout_->Items[wiCharDataFull::CHAR_LOADOUT_HEADGEAR].itemID;
	n.BackpackID = loadout_->BackpackID;

	//TODO: for network traffic optimization (do not send to us) - change to RelayPacket (and add preparePacket there)
	gServerLogic.p2pBroadcastToActive(this, &n, sizeof(n), true);
}

void obj_ServerPlayer::OnAttachmentChanged(int wid, int atype)
{
	// send packet only if attachments specified in wiNetWeaponAttm was changed
	if(atype != WPN_ATTM_LEFT_RAIL && atype != WPN_ATTM_MUZZLE)
		return;

	PKT_S2C_SetPlayerAttachments_s n;
	n.wid  = (BYTE)wid;
	n.Attm = GetWeaponNetAttachment(wid);

	//TODO: for network traffic optimization (do not send to us) - change to RelayPacket (and add preparePacket there)
	gServerLogic.p2pBroadcastToActive(this, &n, sizeof(n), true);
}

void obj_ServerPlayer::OnChangeBackpackSuccess(const std::vector<wiInventoryItem>& droppedItems)
{
	// backpack change was successful, drop items to the ground
	for(size_t i=0; i<droppedItems.size(); i++)
	{
		// create network object
		obj_DroppedItem* obj = (obj_DroppedItem*)srv_CreateGameObject("obj_DroppedItem", "obj_DroppedItem", GetRandomPosForItemDrop());
		obj->SetNetworkID(gServerLogic.GetFreeNetId());
		obj->NetworkLocal = true;
		// vars
		obj->m_Item       = droppedItems[i];
	}
}

void obj_ServerPlayer::UseItem_CreateNote(const PKT_C2S_CreateNote_s& n)
{
	if(n.SlotFrom >= loadout_->BackpackSize) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "PKT_C2S_CreateNote_s",
			"slot: %d", n.SlotFrom);
		return;
	}
	wiInventoryItem& wi = loadout_->Items[n.SlotFrom];
	uint32_t usedItemId = wi.itemID;

	if(wi.itemID != WeaponConfig::ITEMID_PieceOfPaper) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "PKT_C2S_CreateNote_s",
			"itemid: %d vs %d", wi.itemID, WeaponConfig::ITEMID_PieceOfPaper);
		return;
	}
	if(wi.quantity <= 0) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_UseItem, true, "PKT_C2S_CreateNote_s",
			"%d", wi.quantity);
		return;
	}

	// remove used item
	wi.quantity--;
	if(wi.quantity <= 0) {
		wi.Reset();
	}

	// create network object
	obj_Note* obj = (obj_Note*)srv_CreateGameObject("obj_Note", "obj_Note", n.pos);
	obj->SetNetworkID(gServerLogic.GetFreeNetId());
	obj->NetworkLocal = true;
	// vars
	obj->m_Note.NoteID     = 0;
	_time64(&obj->m_Note.CreateDate);
	obj->m_Note.TextFrom   = n.TextFrom;
	obj->m_Note.TextSubj   = n.TextSubj;
	obj->m_Note.ExpireMins = n.ExpMins;
	obj->OnCreate();

	CJobAddNote* job = new CJobAddNote(this);
	job->GameServerId= gServerLogic.ginfo_.gameServerId;
	job->GamePos     = obj->GetPosition();
	job->TextFrom    = obj->m_Note.TextFrom;
	job->TextSubj    = obj->m_Note.TextSubj;
	job->ExpMins     = n.ExpMins;
	job->NoteGameObj = obj->GetSafeID();
	g_AsyncApiMgr->AddJob(job);

	return;
}

void obj_ServerPlayer::TeleportPlayer(const r3dPoint3D& pos)
{
	SetPosition(pos);
	netMover.SrvSetCell(GetPosition());
	loadout_->GamePos = GetPosition();

	moveInited = false;

	gServerLogic.UpdateNetObjVisData(this);
}

bool obj_ServerPlayer::CheckForFastMove()
{
	if(!moveInited)
		return false;

	// check every 5 sec and check against sprint speed with bigger delta
	moveAccumTime += r3dGetFrameTime();
	if(moveAccumTime < 5.0f)
		return false;

	float avgSpeed = moveAccumDist / moveAccumTime;
	bool isCheat   = false;
	//r3dOutToLog("avgSpeed: %f vs %f\n", avgSpeed, GPP_Data.AI_SPRINT_SPEED);
	if(loadout_->Alive && avgSpeed > GPP_Data.AI_SPRINT_SPEED * 1.4f)
	{
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_FastMove, true, "CheatFastMove",
			"dist: %f for %f, speed:%f\n",
			moveAccumDist,
			moveAccumTime,
			avgSpeed
			);
		isCheat = true;
	}

	// reset accomulated vars
	moveAccumTime = 0;
	moveAccumDist = 0;

	return isCheat;
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_PacketBarrier_s& n)
{
	// client switched to next sequence
	clientPacketSequence++;
	r3dOutToLog("peer%02d PKT_C2C_PacketBarrier_s %s %d vs %d\n", peerId_, packetBarrierReason, myPacketSequence, clientPacketSequence);
	packetBarrierReason = "";

	// reset move cheat detection
	moveInited = false;
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_MoveSetCell_s& n) // Server Vehicles
{
	// if by some fucking unknown method you appeared at 0,0,0 - don't do that!
	/*if(gServerLogic.ginfo_.mapId != GBGameInfo::MAPID_ServerTest && n.pos.Length() < 10)
	{
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Data, true, "ZeroTeleport",
			"%f %f %f",
			n.pos.x, n.pos.y, n.pos.z);
		return;
	}*/

	if(moveInited)
	{
		// for now we will check ONLY ZX, because somehow players is able to fall down
		r3dPoint3D p1 = netMover.SrvGetCell();
		r3dPoint3D p2 = n.pos;
		p1.y = 0;
		p2.y = 0;
		float dist = (p1 - p2).Length();

		// check for teleport - more that 3 sec of sprint speed. MAKE sure that max dist is more that current netMover.cellSize
		/*if(loadout_->Alive && dist > GPP_Data.AI_SPRINT_SPEED * 3.0f)
		{
			gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_FastMove, true, (dist > 500.0f ? "huge_teleport" : "teleport"),
				"%f, srvGetCell: %.2f, %.2f, %.2f; n.pos: %.2f, %.2f, %.2f",
				dist,
				netMover.SrvGetCell().x, netMover.SrvGetCell().y, netMover.SrvGetCell().z,
				n.pos.x, n.pos.y, n.pos.z
				);
			return;
		}*/
	}

	netMover.SetCell(n);
	SetPosition(n.pos);

	// keep them guaranteed
	RelayPacket(&n, sizeof(n), true);
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_MoveRel_s& n)
{
	// decode move
	const CNetCellMover::moveData_s& md = netMover.DecodeMove(n);

	if(moveInited)
	{
		// for now we will check ONLY ZX, because somehow players is able to fall down
		r3dPoint3D p1 = GetPosition();
		r3dPoint3D p2 = md.pos;
		p1.y = 0;
		p2.y = 0;
		float dist = (p1 - p2).Length();
		moveAccumDist += dist;
	}

	// check if we need to reset accomulated speed
	if(!moveInited)
	{
		moveInited    = true;
		moveAccumTime = 0.0f;
		moveAccumDist = 0.0f;
	}

	// update last action if we moved or rotated
	if((GetPosition() - md.pos).Length() > 0.01f || m_PlayerRotation != md.turnAngle)
	{
		lastPlayerAction_ = r3dGetTime();
	}

	SetPosition(md.pos);
	m_PlayerRotation = md.turnAngle;
	m_PlayerState = md.state&0xF; // PlayerState&0xF

	loadout_->GamePos = GetPosition();
	loadout_->GameDir = m_PlayerRotation;

	RelayPacket(&n, sizeof(n), false);
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_PlayerJump_s& n)
{
	RelayPacket(&n, sizeof(n), true);
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_PlayerEquipAttachment_s& n)
{
	if(n.wid >= 2) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "attachment",
			"wid: %d", n.wid);
		return;
	}
	if(m_WeaponArray[n.wid] == NULL) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "attachment",
			"nowpn: %d", n.wid);
		return;
	}
	if(n.AttmSlot >= loadout_->BackpackSize) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "attachment",
			"slot: %d", n.AttmSlot);
		return;
	}
	if(n.dbg_WeaponID != m_WeaponArray[n.wid]->getConfig()->m_itemID) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "attachment",
			"wid: %d %d", n.dbg_WeaponID, m_WeaponArray[n.wid]->getConfig()->m_itemID);
		return;
	}
	if(n.dbg_AttmID != loadout_->Items[n.AttmSlot].itemID) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "attachment",
			"aid: %d %d", n.dbg_AttmID, loadout_->Items[n.AttmSlot].itemID);
		return;
	}

	// get attachment config
	wiInventoryItem& wi = loadout_->Items[n.AttmSlot];
	const WeaponAttachmentConfig* attachCfg = g_pWeaponArmory->getAttachmentConfig(wi.itemID);
	if(!attachCfg) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "attachment",
			"bad itemid: %d", wi.itemID);
		return;
	}

	ServerWeapon* wpn = m_WeaponArray[n.wid];
	// verify that attachment is legit and can go into this weapon
	if(!wpn->m_pConfig->isAttachmentValid(attachCfg))
	{
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, false, "attachment",
			"not valid attm itemid: %d", wi.itemID);
		return;
	}

	r3dOutToLog("%s: equip attachment %s for %s\n", userName, attachCfg->m_StoreName, wpn->getConfig()->m_StoreName); CLOG_INDENT;

	// set wpn attachment
	wpn->m_Attachments[attachCfg->m_type] = attachCfg;
	wpn->recalcAttachmentsStats();

	loadout_->Attachment[n.wid].attachments[attachCfg->m_type] = attachCfg->m_itemID;

	// report new loadout in case if flashlight/laser was changed
	OnLoadoutChanged();

	// report to other players
	OnAttachmentChanged(n.wid, attachCfg->m_type);
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_PlayerRemoveAttachment_s& n)
{
	if(n.wid >= 2) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "attachment",
			"wid: %d", n.wid);
		return;
	}
	if(m_WeaponArray[n.wid] == NULL) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "attachment",
			"nowpn: %d", n.wid);
		return;
	}
	if(n.WpnAttmType >= WPN_ATTM_MAX) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "attachment",
			"WpnAttmType: %d", n.WpnAttmType);
		return;
	}

	ServerWeapon* wpn = m_WeaponArray[n.wid];

	// remove wpn attachment, equip default if have
	wpn->m_Attachments[n.WpnAttmType] = g_pWeaponArmory->getAttachmentConfig(wpn->m_pConfig->FPSDefaultID[n.WpnAttmType]);
	wpn->recalcAttachmentsStats();

	loadout_->Attachment[n.wid].attachments[n.WpnAttmType] = 0;

	// report new loadout in case if flashlight/laser was changed
	OnLoadoutChanged();

	// report to other players
	OnAttachmentChanged(n.wid, n.WpnAttmType);
}
/*void obj_ServerPlayer::OnNetPacket(const PKT_S2C_RepairALLWeapon_s& n)
{
		for (int a = 0; a < loadout_->BackpackSize; a++)
		{
			if (loadout_->Items[a].itemID != 0)
			{
				//loadout_->Items[a].Durability = 100;

				char* g_ServerApiKey = "bvx425698dg6GsnxwedszF";
				CWOBackendReq req("api_GetInventoryData.aspx");
				req.AddSessionInfo(profile_.CustomerID, profile_.SessionID);
				req.AddParam("skey1",  g_ServerApiKey);
				req.AddParam("Data", 3);
				req.AddParam("Inventory", (int)loadout_->Items[a].InventoryID);//(int)m_inventoryID);
				req.AddParam("CustomerID", profile_.CustomerID);
				req.AddParam("CharID", 0);
				req.AddParam("Slot", 0);
				req.AddParam("ItemID", 0);
				req.AddParam("Quantity", 0);
				req.AddParam("Var1", 0);
				req.AddParam("Var2", 0);
				req.AddParam("Category",  0);

				//req.AddParam("Durability",  loadout_->Items[a].Durability);

				if(!req.Issue())
				{
					r3dOutToLog("GetInventoryData FAILED, code: %d\n", req.resultCode_);
					return;
				}

			}
		}
		profile_.ProfileData.GameDollars=n.price;
		gServerLogic.ApiPlayerUpdateChar(this);
}
void obj_ServerPlayer::OnNetPacket(const PKT_S2C_RepairWeapon_s& n)
{
		ServerWeapon* wpn = m_WeaponArray[n.slot];

		if (wpn == NULL)
			return;

		//wpn->getPlayerItem().Durability = 100;

		char* g_ServerApiKey = "bvx425698dg6GsnxwedszF";
		CWOBackendReq req("api_GetInventoryData.aspx");
		req.AddSessionInfo(profile_.CustomerID, profile_.SessionID);
		req.AddParam("skey1",  g_ServerApiKey);
		req.AddParam("Data", 3);
		req.AddParam("Inventory", (int)wpn->getPlayerItem().InventoryID);//(int)m_inventoryID);
		req.AddParam("CustomerID", profile_.CustomerID);
		req.AddParam("CharID", 0);
		req.AddParam("Slot", 0);
		req.AddParam("ItemID", 0);
		req.AddParam("Quantity", 0);
		req.AddParam("Var1", 0);
		req.AddParam("Var2", 0);
		req.AddParam("Category",  0);

		//req.AddParam("Durability",  wpn->getPlayerItem().Durability);

		if(!req.Issue())
		{
			r3dOutToLog("GetInventoryData FAILED, code: %d\n", req.resultCode_);
			return;
		}
		profile_.ProfileData.GameDollars=n.price;
		gServerLogic.ApiPlayerUpdateChar(this);
		//r3dOutToLog("### ACTUALIZANDO DB %i Durability %i\n",loadout_->Items[n.slot].itemID,loadout_->Items[n.slot].Durability);
}*/
void obj_ServerPlayer::OnNetPacket(const PKT_C2C_PlayerSwitchWeapon_s& n)
{
	if(n.wid == 255) // signal that player has empty hands, just relay it
	{
		m_ForcedEmptyHands = true;
		RelayPacket(&n, sizeof(n));
		return;
	}
	m_ForcedEmptyHands = false;

	if(n.wid >= NUM_WEAPONS_ON_PLAYER) {
		gServerLogic.LogInfo(peerId_, "SwitchWeapon", "wrong weaponslot %d", n.wid);
		return;
	}

	// ptumik: because server creating weapons only for 1 and 2 slots, user can switch to usable items.
	// so, having NULL here is totally legitimate.
	// also, because of this, before using m_WeaponArray[m_SelectedWeapon] we need to check that it is not NULL
// 	if(m_WeaponArray[n.wid] == NULL) {
// 		gServerLogic.LogInfo(peerId_, "SwitchWeapon", "empty weaponslot %d", n.wid);
// 		return;
// 	}

	m_SelectedWeapon = n.wid;

	RelayPacket(&n, sizeof(n));
}

void obj_ServerPlayer::UseItem_Barricade(const r3dPoint3D& pos, float rotX, uint32_t itemID)
{
	const WeaponConfig* wc = g_pWeaponArmory->getWeaponConfig(itemID);
	if(!wc)
		return;

	if((GetPosition() - pos).Length() > 5.0f)
	{
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_UseItem, true, "distance",
			"%d",
			itemID
			);
		return;
	}

	// spawn
	obj_ServerBarricade* shield= (obj_ServerBarricade*)srv_CreateGameObject("obj_ServerBarricade", "barricade", pos);
	shield->m_ItemID = itemID;
	shield->Health = wc->m_AmmoDamage;
	shield->SetRotationVector(r3dPoint3D(rotX,0,0));
	shield->SetNetworkID(gServerLogic.GetFreeNetId());
	shield->NetworkLocal = true;
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_PlayerCraftItem_s& n)
{
	r3dOutToLog("Plr %s craft slot %d\n",loadout_->Gamertag,n.slotid1);
	r3dOutToLog("Plr %s craft slot %d\n",loadout_->Gamertag,n.slotid2);
	r3dOutToLog("Plr %s craft slot %d\n",loadout_->Gamertag,n.slotid3);
	r3dOutToLog("Plr %s craft slot %d\n",loadout_->Gamertag,n.slotid4);
	r3dOutToLog("Plr %s craft %d\n",loadout_->Gamertag,n.itemid);
	wiInventoryItem& wi1 = loadout_->Items[n.slotid1];
	wiInventoryItem& wi2 = loadout_->Items[n.slotid2];

if (n.slotid1 != 99999)
{
	wiInventoryItem& wi1 = loadout_->Items[n.slotid1];
	int q1 = wi1.quantity;
	q1 -= n.slotid1q;
	PKT_S2C_BackpackModify_s n1;
	n1.SlotTo     = n.slotid1;
	n1.Quantity   = q1;
	n1.dbg_ItemID = wi1.itemID;
	gServerLogic.p2pSendToPeer(peerId_, this, &n1, sizeof(n1));
	wi1.Reset();
}

if (n.slotid2 != 99999)
{
	wiInventoryItem& wi2 = loadout_->Items[n.slotid2];
	int q2 = wi2.quantity;
	q2 -= n.slotid2q;
	PKT_S2C_BackpackModify_s n1;
	n1.SlotTo     = n.slotid2;
	n1.Quantity   = q2;
	n1.dbg_ItemID = wi2.itemID;
	gServerLogic.p2pSendToPeer(peerId_, this, &n1, sizeof(n1));
	wi2.Reset();
}

if (n.slotid3 != 99999)
{
	wiInventoryItem& wi3 = loadout_->Items[n.slotid3];
	int q3 = wi3.quantity;
	q3 -= n.slotid3q;
	PKT_S2C_BackpackModify_s n1;
	n1.SlotTo     = n.slotid3;
	n1.Quantity   = q3;
	n1.dbg_ItemID = wi3.itemID;
	gServerLogic.p2pSendToPeer(peerId_, this, &n1, sizeof(n1));
	wi3.Reset();
}
if (n.slotid4 != 99999)
{
	wiInventoryItem& wi4 = loadout_->Items[n.slotid4];
	int q4 = wi4.quantity;
	q4 -= n.slotid4q;
	PKT_S2C_BackpackModify_s n1;
	n1.SlotTo     = n.slotid4;
	n1.Quantity   = q4;
	n1.dbg_ItemID = wi4.itemID;
	gServerLogic.p2pSendToPeer(peerId_, this, &n1, sizeof(n1));
	wi4.Reset();

}
if (n.slotid5 != 99999)
{
	wiInventoryItem& wi5 = loadout_->Items[n.slotid5];
	int q5 = wi5.quantity;
	q5 -= n.slotid5q;
	PKT_S2C_BackpackModify_s n1;
	n1.SlotTo     = n.slotid5;
	n1.Quantity   = q5;
	n1.dbg_ItemID = wi5.itemID;
	gServerLogic.p2pSendToPeer(peerId_, this, &n1, sizeof(n1));
	wi5.Reset();
}

	// Add Crafted item to player
    int itemids = n.itemid;
	wiInventoryItem wi;
	wi.itemID   = itemids;
	wi.quantity = 1;	
	BackpackAddItem(wi);
	gServerLogic.ApiPlayerUpdateChar(this);
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_PlayerUseItem_s& n)
{
	//gServerLogic.LogInfo(peerId_, "UseItem", "%d", n.dbg_ItemID); CLOG_INDENT;

	if(n.SlotFrom >= loadout_->BackpackSize) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "useitem",
			"slot: %d", n.SlotFrom);
		return;
	}
	wiInventoryItem& wi = loadout_->Items[n.SlotFrom];
	uint32_t usedItemId = wi.itemID;

	if(wi.itemID != n.dbg_ItemID) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "useitem",
			"itemid: %d vs %d", wi.itemID, n.dbg_ItemID);
		return;
	}

	if(wi.quantity <= 0) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_UseItem, true, "quantity",
			"%d", wi.quantity);
		return;
	}

	// remove used item
	wi.quantity--;
	if(wi.quantity <= 0) {
		wi.Reset();
	}

	RelayPacket(&n, sizeof(n));

	const FoodConfig* foodConfig = g_pWeaponArmory->getFoodConfig(usedItemId);
	if(foodConfig)
	{
		loadout_->Health += foodConfig->Health;   loadout_->Health = R3D_CLAMP(loadout_->Health, 0.0f, 100.0f);
		loadout_->Toxic  += foodConfig->Toxicity; loadout_->Toxic  = R3D_CLAMP(loadout_->Toxic,  0.0f, 100.0f);
		loadout_->Hunger -= foodConfig->Food;     loadout_->Hunger = R3D_CLAMP(loadout_->Hunger, 0.0f, 100.0f);
		loadout_->Thirst -= foodConfig->Water;    loadout_->Thirst = R3D_CLAMP(loadout_->Thirst, 0.0f, 100.0f);

		m_Stamina += GPP_Data.c_fSprintMaxEnergy*foodConfig->Stamina;
		return;
	}

	bool useOnOtherPlayer = usedItemId==WeaponConfig::ITEMID_Antibiotics||usedItemId==WeaponConfig::ITEMID_Bandages||usedItemId==WeaponConfig::ITEMID_CraftedBandages||usedItemId==WeaponConfig::ITEMID_Bandages2||
		usedItemId==WeaponConfig::ITEMID_Painkillers||usedItemId==WeaponConfig::ITEMID_ZombieRepellent||usedItemId==WeaponConfig::ITEMID_C01Vaccine||
		usedItemId==WeaponConfig::ITEMID_C04Vaccine||usedItemId==WeaponConfig::ITEMID_Medkit;
	if(useOnOtherPlayer && n.var4!=0)
	{
		GameObject* obj = GameWorld().GetNetworkObject(n.var4);
		if(obj && obj->isObjType(OBJTYPE_Human))
		{
			obj_ServerPlayer* otherPlayer = (obj_ServerPlayer*)obj;
			otherPlayer->UseItem_ApplyEffect(n, usedItemId);
		}
		else
		{
			gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_UseItem, false, "otherplayer", "%d", n.var4);
		}
	}
	else
		UseItem_ApplyEffect(n, usedItemId);

	return;
}

void obj_ServerPlayer::UseItem_ApplyEffect(const PKT_C2C_PlayerUseItem_s& n, uint32_t itemID)
{
	switch(itemID)
	{
		case WeaponConfig::ITEMID_Medkit:
		case WeaponConfig::ITEMID_Bandages:
		case WeaponConfig::ITEMID_Bandages2:
		case WeaponConfig::ITEMID_CraftedBandages:
		case WeaponConfig::ITEMID_Antibiotics:
		case WeaponConfig::ITEMID_Painkillers:
			{
				const WeaponConfig* wc = g_pWeaponArmory->getWeaponConfig(itemID);
				if(!wc) {
					r3d_assert(false && "bandages must be a weapon");
					return;
				}

				float bandageEffect = wc->m_AmmoDamage;
					if(loadout_->Stats.skillid20 == 1){
					bandageEffect *= 1.25f;
					if(loadout_->Stats.skillid25 == 1){
						bandageEffect *= 1.50f;
						if(loadout_->Stats.skillid28 == 1)
							bandageEffect *= 1.75f;
					}
				}

				// AomBESkill : Alive And Well
				if(loadout_->Stats.skillid7 == 1)
				{
					loadout_->Health += bandageEffect;
					loadout_->Health = R3D_MIN(loadout_->Health, 200.0f);
					r3dOutToLog("bandage used with skill 7, %f\n", bandageEffect);
					return;
				}
				else if (loadout_->Stats.skillid0 == 1)
				{
					loadout_->Health += bandageEffect;
					loadout_->Health = R3D_MIN(loadout_->Health, 150.0f);
				    r3dOutToLog("bandage used with skill 0, %f\n", bandageEffect);
				}
				else
				{
					loadout_->Health += bandageEffect;
					loadout_->Health = R3D_MIN(loadout_->Health, 100.0f);
					r3dOutToLog("bandage used, %f\n", bandageEffect);
				}
			}
			break;
		case WeaponConfig::ITEMID_C01Vaccine:
		case WeaponConfig::ITEMID_C04Vaccine:
			{
				const WeaponConfig* wc = g_pWeaponArmory->getWeaponConfig(itemID);
				if(!wc) {
					r3d_assert(false && "vaccine must be a weapon");
					return;
				}

				float vaccineEffect = wc->m_AmmoDamage;
				loadout_->Toxic -= vaccineEffect;
				loadout_->Toxic = R3D_CLAMP(loadout_->Toxic, 0.0f, 100.0f);
				r3dOutToLog("vaccine used, %f\n", vaccineEffect);
			}
			break;
		case WeaponConfig::ITEMID_BarbWireBarricade:
		case WeaponConfig::ITEMID_WoodShieldBarricade:
		case WeaponConfig::ITEMID_RiotShieldBarricade:
		case WeaponConfig::ITEMID_SandbagBarricade:
		case WeaponConfig::ITEMID_BlockDoorWood:
        case WeaponConfig::ITEMID_BlockLight:
        case WeaponConfig::ITEMID_BlockSolarWater:
        case WeaponConfig::ITEMID_BlockWallBrickShort:
        case WeaponConfig::ITEMID_BlockWallBrick:
		case WeaponConfig::ITEMID_BlockWallMetal:
        case WeaponConfig::ITEMID_BlockWallWood:
        case WeaponConfig::ITEMID_BlockDoorWood2M01:
        case WeaponConfig::ITEMID_BlockWallMetal2M01:
        case WeaponConfig::ITEMID_BlockWallBrickTall:
        case WeaponConfig::ITEMID_BlockWallWood2M01:
        case WeaponConfig::ITEMID_BlockWallBrickShort2:
        case WeaponConfig::ITEMID_BlockFarm:
        case WeaponConfig::ITEMID_BlockSolarWater2:
        case WeaponConfig::ITEMID_BlockLight2:
        case WeaponConfig::ITEMID_BlockFarmCrate:
		case WeaponConfig::ITEMID_PersonalLocker:
		case WeaponConfig::ITEMID_IndustGenerator:
		case WeaponConfig::ITEMID_ClayMore:
		case WeaponConfig::ITEMID_BlockEnergy:
			UseItem_Barricade(n.pos, n.var1, itemID);
			break;
		case 101398:
		case 101397:
			break;
		case WeaponConfig::ITEMID_ZombieRepellent:
			//When the item is used in-game make it so the server knows we used it
			loadout_->Repellent = 1;
			loadout_->UsedRepellentAt = r3dGetTime();
			break;

		default:
			gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_UseItem, true, "baditemid",
				"%d",
				itemID
				);
			break;
	}

	return;
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_PlayerReload_s& n)
{
	if(n.WeaponSlot >= loadout_->BackpackSize || n.AmmoSlot >= loadout_->BackpackSize) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "reload",
			"slot: %d %d", n.WeaponSlot, n.AmmoSlot);
		return;
	}
	if(n.WeaponSlot != wiCharDataFull::CHAR_LOADOUT_WEAPON1 && n.WeaponSlot != wiCharDataFull::CHAR_LOADOUT_WEAPON2) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "reload",
			"wslot: %d", n.WeaponSlot);
		return;
	}

	// validate weapon
	ServerWeapon* wpn = m_WeaponArray[n.WeaponSlot];
	if(wpn == NULL) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Data, true, "reload",
			"wempty: %d", n.WeaponSlot);
		return;
	}
	if(wpn->getClipConfig() == NULL) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Data, true, "reload",
			"noclip: %d", n.WeaponSlot);
		return;
	}

	// validate ammo slot
	wiInventoryItem& wi = loadout_->Items[n.AmmoSlot];
	if(wi.itemID == 0 || wi.quantity == 0) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "reload",
			"aempty: %d %d", wi.itemID, wi.quantity);
		return;
	}

	// validate if we reloaded correct amount
	int ammoReloaded = wi.Var1 < 0 ? wpn->getClipConfig()->m_Clipsize : wi.Var1;
	if(n.dbg_Amount != ammoReloaded) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "reload",
			"amount:%d var1:%d clip:%d", n.dbg_Amount, wi.Var1, wpn->getClipConfig()->m_Clipsize);
		return;
	}

	r3dOutToLog("reloaded using slot %d - %d left\n", n.AmmoSlot, wi.quantity);

	//@TODO: check if we can use that ammo

	// remove single clip
	wi.quantity--;
	if(wi.quantity <= 0)
		wi.Reset();

	// drop current ammo clip (if have clip speficied and have ammo)
	if(wpn->getPlayerItem().Var1 > 0 && wpn->getPlayerItem().Var2 > 0)
	{
		wiInventoryItem wi4;
		wi4.itemID   = wpn->getPlayerItem().Var2;
		wi4.quantity = 1;	
		wi4.Var1 = wpn->getPlayerItem().Var1;
		wi4.Var2 = -1;
		//wi4.Durability = 100;

		if (IsItemCanAddToInventory(wi4))
		{
			BackpackAddItem(wi4);
		}
		else
		{
			obj_DroppedItem* obj = (obj_DroppedItem*)srv_CreateGameObject("obj_DroppedItem", "obj_DroppedItem", GetRandomPosForItemDrop());
			obj->SetNetworkID(gServerLogic.GetFreeNetId());
			obj->NetworkLocal = true;
			// vars
			obj->m_Item.itemID   = wpn->getPlayerItem().Var2;
			obj->m_Item.quantity = 1;
			obj->m_Item.Var1     = wpn->getPlayerItem().Var1;
			//obj->m_Item.Durability = 100;
		}
	}

	// reload weapon
	wpn->getPlayerItem().Var1 = ammoReloaded;
	wpn->getPlayerItem().Var2 = wpn->getClipConfig()->m_itemID;
	//wpn->getPlayerItem().Durability = n.Durability;

	gServerLogic.ApiPlayerUpdateChar(this);

	RelayPacket(&n, sizeof(n));
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_PlayerFired_s& n)
{
	if(!FireWeapon(n.debug_wid, n.wasAiming != 0, n.execWeaponFire, 0, "PKT_C2C_PlayerFired_s"/*,n.Durability*/))
	{
		return;
	}

	if(n.execWeaponFire)
	{
		gServerLogic.InformZombiesAboutSound(this, m_WeaponArray[n.debug_wid]);
	}

	RelayPacket(&n, sizeof(n));
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_PlayerHitStatic_s& n)
{
	//r3dOutToLog("FireHitCount--: PKT_C2C_PlayerHitStatic_s\n");
	FireHitCount--;
	if(FireHitCount < -10) // -10 - to allow some buffer
	{
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_NumShots, true, "bullethack3");
		return;
	}

	RelayPacket(&n, sizeof(n), false);
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_PlayerHitStaticPierced_s& n)
{
	// just relay packet. not a real hit, just idintication that we pierced some static geometry, will be followed up by real HIT packet
	RelayPacket(&n, sizeof(n), false);
}


void obj_ServerPlayer::OnNetPacket(const PKT_C2C_PlayerHitNothing_s& n)
{
	//r3dOutToLog("FireHitCount--: PKT_C2C_PlayerHitNothing_s\n");
	FireHitCount--;
	if(FireHitCount < -10) // -10 - to allow some buffer
	{
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_NumShots, true, "bullethack2");
		return;
	}

	//RelayPacket(&n, sizeof(n), false);
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_PlayerHitDynamic_s& n)
{
	//r3dOutToLog("FireHitCount--: PKT_C2C_PlayerHitDynamic_s\n");
	FireHitCount--;
	if(FireHitCount < -10) // -10 - to allow some buffer
	{
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_NumShots, true, "bullethack1");
		return;
	}

	// make sure we're shooting to another player
	GameObject* targetObj = GameWorld().GetNetworkObject(n.targetId);
	if(!targetObj)
	{
		gServerLogic.LogInfo(peerId_, "HitBody0", "not valid targetId");
		return;
	}

	//r3dOutToLog("hit from %s to %s\n", fromObj->Name.c_str(), targetObj->Name.c_str()); CLOG_INDENT;
	//r3dOutToLog("hit from player!!!!\n");

	if(!gServerLogic.CanDamageThisObject(targetObj))
	{
		gServerLogic.LogInfo(peerId_, "HitBody1", "hit object that is not damageable!");
		return;
	}

	// validate hit_pos is close to the targetObj, if not, that it is a hack
	if(n.damageFromPiercing == 0) // 0 - bullet didn't pierce anything
	{
		const float dist  = (n.hit_pos - targetObj->GetPosition()).Length();
		const float allow = GPP_Data.AI_SPRINT_SPEED*2.0f;
		if(dist > allow) // if more than Xsec of sprint
		{
			// ptumik: disabled cheat report, as we might receive packet for a player that is dead for client, but respawned on server -> distance difference
			//gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_ShootDistance, false, "HitBodyBigDistance",
			//	"hit %s, dist %f vs %f",
			//	targetObj->Name.c_str(), dist, allow
			//	);
			return;
		}
	}


	// validate that we currently have valid weapon
	if(m_WeaponArray[m_SelectedWeapon] == NULL)
	{
		gServerLogic.LogInfo(peerId_, "HitBody2", "empty weapon");
		return;
	}

	// validate melee range
	if(m_WeaponArray[m_SelectedWeapon]->getCategory()==storecat_MELEE)
	{
		float dist = (GetPosition() - targetObj->GetPosition()).Length();
		if(dist > 5.0f)
		{
			gServerLogic.LogInfo(peerId_, "HitBody0", "knife cheat %f", dist);
			return;
		}
	}

	// validate muzzle position
	{
		if((GetPosition() - n.muzzler_pos).Length() > 5.0f)
		{
			gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_NoGeomtryFiring, true, "muzzle pos cheat");
			return;
		}
	}

	// validate ray cast (should work for ballistic bullets too)
	{
		PxRaycastHit hit;
		PxSceneQueryFilterData filter(PxFilterData(COLLIDABLE_STATIC_MASK, 0, 0, 0), PxSceneQueryFilterFlag::eSTATIC);
		r3dVector pos = n.muzzler_pos;
		r3dVector dir = n.hit_pos - n.muzzler_pos;
		float dirl = dir.Length();
		dir.Normalize();
		if(g_pPhysicsWorld->raycastSingle(PxVec3(pos.x, pos.y, pos.z), PxVec3(dir.x,dir.y,dir.z), dirl, PxSceneQueryFlag::eIMPACT, hit, filter))
		{
			/*
			// we shouldn't hit any static geometry, if we did, than probably user is cheating.
			// so let's discard this packet for now
			gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_NoGeomtryFiring, false,
				"raycast failed", "player pos: %.2f, %.2f, %.2f, muzzler: %.2f, %.2f, %.2f, distance: %.2f, hitpos: %.2f, %.2f, %.2f",
				GetPosition().x, GetPosition().y ,GetPosition().z, pos.x, pos.y, pos.z, dirl, n.hit_pos.x, n.hit_pos.y, n.hit_pos.z);
			*/
			return;
		}

		// and now validate raycast in opposite direction to check for ppl shooting from inside buildings and what not
		pos = n.hit_pos;
		dir = n.muzzler_pos - n.hit_pos;
		dirl = dir.Length();
		dir.Normalize();
		if(g_pPhysicsWorld->raycastSingle(PxVec3(pos.x, pos.y, pos.z), PxVec3(dir.x,dir.y,dir.z), dirl, PxSceneQueryFlag::eIMPACT, hit, filter))
		{
			/*
			// we shouldn't hit any static geometry, if we did, than probably user is cheating.
			// so let's discard this packet for now
			gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_NoGeomtryFiring, false,
				"reverse raycast failed", "player pos: %.2f, %.2f, %.2f, muzzler: %.2f, %.2f, %.2f, distance: %.2f, hitpos: %.2f, %.2f, %.2f",
				GetPosition().x, GetPosition().y ,GetPosition().z, pos.x, pos.y, pos.z, dirl, n.hit_pos.x, n.hit_pos.y, n.hit_pos.z);
			*/
			return;
		}

	}

	RelayPacket(&n, sizeof(n));

	// calc damage
	float damage = CalcWeaponDamage(targetObj->GetPosition());
	if(n.damageFromPiercing > 0)
	{
		float dmod = float(n.damageFromPiercing)/100.0f;
		damage *= dmod;
	}

	if(loadout_->Stats.skillid8 == 1){
		if(m_WeaponArray[m_SelectedWeapon]->getCategory()==storecat_MELEE){
			damage *= 1.25f;
		}
		if(loadout_->Stats.skillid10 == 1){
			if(m_WeaponArray[m_SelectedWeapon]->getCategory()==storecat_MELEE){
				damage *= 1.25f;
			}
		}
	}

	// track ShotsHits
	loadout_->Stats.ShotsHits++;

	if(obj_ServerPlayer* targetPlr = IsServerPlayer(targetObj))
	{
		if(gServerLogic.ApplyDamageToPlayer(this, targetPlr, GetPosition()+r3dPoint3D(0,1,0), damage, n.hit_body_bone, n.hit_body_part, false, m_WeaponArray[m_SelectedWeapon]->getCategory()))
		{
			//HACK: track Kill here, because we can't pass weapon ItemID to ApplyDamageToPlayer yet
			int isKill = ((obj_ServerPlayer*)targetObj)->loadout_->Alive == 0 ? 1 : 0;
			gServerLogic.TrackWeaponUsage(m_WeaponArray[m_SelectedWeapon]->getConfig()->m_itemID, 0, 1, isKill);
		}
	}
	else if(targetObj->isObjType(OBJTYPE_Zombie))
	{
		gServerLogic.ApplyDamageToZombie(this, targetObj, GetPosition()+r3dPoint3D(0,1,0), damage, n.hit_body_bone, n.hit_body_part, false, m_WeaponArray[m_SelectedWeapon]->getCategory());
	}
	else
	{
		gServerLogic.TrackWeaponUsage(m_WeaponArray[m_SelectedWeapon]->getConfig()->m_itemID, 0, 1, 0);
		gServerLogic.ApplyDamage(this, targetObj, this->GetPosition()+r3dPoint3D(0,1,0), damage, false, m_WeaponArray[m_SelectedWeapon]->getCategory() );
	}
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_PlayerReadyGrenade_s& n)
{
	RelayPacket(&n, sizeof(n));
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_PlayerThrewGrenade_s& n)
{
	r3d_assert(loadout_->Alive);

	lastPlayerAction_ = r3dGetTime();

	if(n.debug_wid < 0 || n.debug_wid>= NUM_WEAPONS_ON_PLAYER)
	{
		gServerLogic.LogInfo(peerId_, "wid invalid", "%s %d", "PKT_C2C_PlayerThrewGrenade_s", n.debug_wid);
		return;
	}

	if(m_ForcedEmptyHands)
	{
		gServerLogic.LogInfo(peerId_, "empty hands", "%s %d vs %d", "PKT_C2C_PlayerThrewGrenade_s", n.debug_wid, m_SelectedWeapon);
		return;
	}

	if(n.slotFrom >= loadout_->BackpackSize) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "ThrewGrenade",
			"slot: %d", n.slotFrom);
		return;
	}
	wiInventoryItem& wi = loadout_->Items[n.slotFrom];
	uint32_t usedItemId = wi.itemID;

	if(wi.quantity <= 0) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_UseItem, true, "ThrewGrenade quantity",
			"%d", wi.quantity);
		return;
	}

	gServerLogic.TrackWeaponUsage(usedItemId, 1, 0, 0);

	// remove used item
	wi.quantity--;
	if(wi.quantity <= 0) {
		wi.Reset();
	}

	RelayPacket(&n, sizeof(n));
}


void obj_ServerPlayer::OnNetPacket(const PKT_C2S_PlayerChangeBackpack_s& n)
{
	if(n.SlotFrom >= loadout_->BackpackSize) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "backpack",
			"chbp slot: %d", n.SlotFrom);
		return;
	}

	const BackpackConfig* cfg = g_pWeaponArmory->getBackpackConfig(loadout_->Items[n.SlotFrom].itemID);
	if(cfg == NULL) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "backpack",
			"chbp item: %d", loadout_->Items[n.SlotFrom].itemID);
		return;
	}
	if(cfg->m_maxSlots != n.BackpackSize) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "backpack",
			"chbp slots: %d %d vs %d", loadout_->Items[n.SlotFrom].itemID, cfg->m_maxSlots, n.BackpackSize);
		return;
	}

	gServerLogic.LogInfo(peerId_, "PKT_C2S_PlayerChangeBackpack_s", "%d->%d", loadout_->BackpackSize, cfg->m_maxSlots); CLOG_INDENT;

	// check for same backpack
	if(loadout_->BackpackID == loadout_->Items[n.SlotFrom].itemID) {
		return;
	}

	// replace backpack in used slot with current one (SERVER CODE SYNC POINT)
	loadout_->Items[n.SlotFrom].itemID = loadout_->BackpackID;

	// remove items that won't fit into backpack and build list of dropped items
	std::vector<wiInventoryItem> droppedItems;
	if(cfg->m_maxSlots < loadout_->BackpackSize)
	{
		for(int i=cfg->m_maxSlots; i<loadout_->BackpackSize; i++)
		{
			wiInventoryItem& wi = loadout_->Items[i];
			if(wi.itemID > 0)
			{
				droppedItems.push_back(wi);

				// remove from remote inventory
				PKT_S2C_BackpackModify_s n;
				n.SlotTo     = i;
				n.Quantity   = 0;
				n.dbg_ItemID = wi.itemID;
				gServerLogic.p2pSendToPeer(peerId_, this, &n, sizeof(n));

				// remove from local inventory
				wi.Reset();
			}
		}
	}

	// update backpack, safe to do here as those params will be updated in api job and if it fails, player will be disconnected
	loadout_->BackpackSize = cfg->m_maxSlots;
	loadout_->BackpackID   = cfg->m_itemID;

	// force player inventory update, so items will be deleted
	gServerLogic.ApiPlayerUpdateChar(this);

	// create api job for backpack change
	CJobChangeBackpack* job = new CJobChangeBackpack(this);
	job->BackpackID   = cfg->m_itemID;
	job->BackpackSize = cfg->m_maxSlots;
	job->DroppedItems = droppedItems;
	g_AsyncApiMgr->AddJob(job);

	OnLoadoutChanged();

	return;
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_BackpackDrop_s& n)
{
	if(n.SlotFrom >= loadout_->BackpackSize) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "backpack",
			"slot: %d", n.SlotFrom);
		return;
	}

	float dropLength = (GetPosition() - n.pos).Length();
	if(dropLength > 20.0f)
	{
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "backpack",
			"dlen: %f", dropLength);
		return;
	}

	wiInventoryItem& wi = loadout_->Items[n.SlotFrom];
	if(wi.itemID == 0 || wi.quantity < 1)
	{
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "backpack",
			"id: %d %d %d", n.SlotFrom, wi.itemID, wi.quantity);
		return;
	}
	//wi.Durability = n.Durability;
	// create network object
	obj_DroppedItem* obj = (obj_DroppedItem*)srv_CreateGameObject("obj_DroppedItem", "obj_DroppedItem", n.pos);
	obj->SetNetworkID(gServerLogic.GetFreeNetId());
	obj->NetworkLocal = true;
	// vars
	obj->m_Item          = wi;
	obj->m_Item.quantity = 1;

	// modify backpack (need after item creation)
	wi.quantity--;
	if(wi.quantity <= 0)
		wi.Reset();

	OnBackpackChanged(n.SlotFrom);
	gServerLogic.ApiPlayerUpdateChar(this);
	return;
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_BackpackSwap_s& n)
{
	if(n.SlotFrom >= loadout_->BackpackSize || n.SlotTo >= loadout_->BackpackSize)	{
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "backpack",
			"move: %d->%d %d", n.SlotFrom, n.SlotTo, loadout_->BackpackSize);
		return;
	}

	// check if we can place item to slot by type
	const BaseItemConfig* itmFrom = g_pWeaponArmory->getConfig(loadout_->Items[n.SlotFrom].itemID);
	const BaseItemConfig* itmTo   = g_pWeaponArmory->getConfig(loadout_->Items[n.SlotTo].itemID);
	if(itmFrom && !storecat_CanPlaceItemToSlot(itmFrom, n.SlotTo))
	{
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "backpack",
			"bad swap: %d->%d", itmFrom->m_itemID, n.SlotTo);
			return;
	}
	if(itmTo && !storecat_CanPlaceItemToSlot(itmTo, n.SlotFrom))
	{
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "backpack",
			"bad swap: %d->%d", itmTo->m_itemID, n.SlotFrom);
			return;
	}


	R3D_SWAP(loadout_->Items[n.SlotFrom], loadout_->Items[n.SlotTo]);

	OnBackpackChanged(n.SlotFrom);
	OnBackpackChanged(n.SlotTo);
	gServerLogic.ApiPlayerUpdateChar(this); // vault
	return;
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_BackpackJoin_s& n)
{
	if(n.SlotFrom >= loadout_->BackpackSize || n.SlotTo >= loadout_->BackpackSize)	{
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "backpack",
			"join: %d->%d %d", n.SlotFrom, n.SlotTo, loadout_->BackpackSize);
		return;
	}

	wiInventoryItem& wi1 = loadout_->Items[n.SlotFrom];
	wiInventoryItem& wi2 = loadout_->Items[n.SlotTo];
	if(wi1.itemID == 0 || wi1.itemID != wi2.itemID) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "backpack",
			"join: itm %d %d", wi1.itemID, wi2.itemID);
		return;
	}
	if(wi1.Var1 >= 0 || wi2.Var1 >= 0) {
		gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, true, "backpack",
			"join: var %d %d", wi1.Var1, wi2.Var1);
		return;
	}

	wi2.quantity += wi1.quantity;
	wi1.Reset();

	OnBackpackChanged(n.SlotFrom);
	OnBackpackChanged(n.SlotTo);
	gServerLogic.ApiPlayerUpdateChar(this); // vault
	return;
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_HackShieldLog_s& n)
{
	bool isLagN = false;
	if (!isHighPing && (r3dGetTime() - LastHackShieldLog) >= 5.8f && ((r3dGetTime() - startPlayTime_) >= 30.0f)) // this player is send slower than 0.8 sec. because player is high ping.
	{
		isHighPing = true;
		isLagN = true;
		r3dOutToLog("HackShieldLog || !! plr %d %d name %s pName %s , IS LAG!\n",peerId_,profile_.CustomerID,userName,n.pName);
		// send message to player.
		char chatmessage[128] = {0};
		PKT_C2C_ChatMessage_s n1;
		sprintf(chatmessage, "WARNING !! YOUR PING IS HIGH.");
		r3dscpy(n1.gamertag, "<SYSTEM>");
		r3dscpy(n1.msg, chatmessage);
		n1.msgChannel = 1;
		n1.userFlag = 2;
		gServerLogic.net_->SendToPeer(&n1,sizeof(n1),peerId_);
	}

	// clear high ping status.
	if (isHighPing && !isLagN)
	{
		r3dOutToLog("HackShieldLog || plr %d %d name %s pName %s clear isHighPing... \n",peerId_,profile_.CustomerID,userName,n.pName);
		isHighPing = false;
	}

	// for this callback not care it.
	// NOTE: FOR HOOKFUNCTION WE NOT USED. WILL HAVE PROBLEM WITH WARGUARD ANTIHACK.
	if (n.lCode == AHNHS_ENGINE_DETECT_WINDOWED_HACK || n.lCode == AHNHS_ACTAPC_DETECT_CODEMISMATCH || n.lCode == AHNHS_ACTAPC_DETECT_HOOKFUNCTION || n.lCode == AHNHS_ACTAPC_DETECT_ALREADYHOOKED)
	{
		LastHackShieldLog = r3dGetTime();
		return;
	}

	// step 3 check callback value.
	if (n.lCode != 0)
	{
		if (n.lCode != 0x010001) // hackshield running.
		{
			r3dOutToLog("HackShieldLog || !! plr %d %d name %s diffirent callback!. disconnect him!! pName %s \n",peerId_,profile_.CustomerID,userName,n.pName);

			char msg[128] = {0};
			switch(n.lCode)
			{
				//Engine Callback
			case AHNHS_ENGINE_DETECT_GAME_HACK:
				{
					sprintf(msg,"Game Hack Found. - \n %s",n.pName);
					break;
				}
			case AHNHS_ACTAPC_DETECT_CODEMISMATCH:
				{
					sprintf(msg,"HackShield Service not running correctly.");
					break;
				}
			case AHNHS_ENGINE_DETECT_WINDOWED_HACK:
				{
					//sprintf(msg,"Windowed Hack Found.");
					break;
				}
			case AHNHS_ACTAPC_DETECT_PROTECTSCREENFAILED:
				{
					sprintf(msg,"Protect Screen Failed.");
					break;
				}
			case AHNHS_ACTAPC_DETECT_SPEEDHACK:
			case AHNHS_ACTAPC_DETECT_SPEEDHACK_RATIO:
				{
					sprintf(msg,"Speed Hack Found.");
					break;
				}
				/*case AHNHS_ACTAPC_DETECT_AUTOMACRO:
				{
				sprintf(msg,"Automacro Found.");
				break;
				}*/
			case AHNHS_ACTAPC_DETECT_ABNORMAL_FUNCTION_CALL:
				{
					sprintf(msg,"Undefined Function Found.");
					break;
				}
			case AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS:
				{
					sprintf(msg,"ABNORMAL Memory Access.");
					break;
				}
			case AHNHS_ACTAPC_DETECT_AUTOMOUSE:
				{
					sprintf(msg,"AutoMouse Found.");
					break;
				}
			case AHNHS_CHKOPT_DETECT_VIRTUAL_MACHINE:
				{
					sprintf(msg,"Vitiual Machine Found.");
					break;
				}
			case AHNHS_ACTAPC_DETECT_ALREADYHOOKED:
			case AHNHS_ACTAPC_DETECT_HOOKFUNCTION:
				{
					sprintf(msg,"Direct3DX Hack Found.");
					break;
				}
			case AHNHS_ACTAPC_DETECT_KDTRACE:
			case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:
				{
					sprintf(msg,"Hack Found.");
					break;
				}
			default:
				{
					sprintf(msg,"Undefined. %x",n.lCode);
					r3dOutToLog("HackShieldLog || plr %d %s : %s\n",peerId_,userName,msg);
					LastHackShieldLog = r3dGetTime();
					return;
					break;
				}
			}
			r3dOutToLog("HackShieldLog || plr %d %s : %s\n",peerId_,userName,msg);
			gServerLogic.DisconnectPeer(peerId_, true, msg);
		}
	}
	LastHackShieldLog = r3dGetTime();
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_VaultBackpackToInv_s& n) // de mi al inventario
{
	wiInventoryItem item = loadout_->Items[n.m_gridFrom];

	if (n.Quantity!=0 && n.Inventory != 0)
	{
		int RemoveQnt = 0;	
		//int Durability = item.Durability;
		item.quantity -= n.Quantity;
		RemoveQnt = item.quantity;
		r3dOutToLog("#### Quantity deleted: %i\n",n.Quantity);
		DoRemoveSafeLockItems(n.m_gridFrom,RemoveQnt,n.Quantity);
		gServerLogic.ApiPlayerUpdateChar(this);

		char* g_ServerApiKey = "bvx425698dg6GsnxwedszF";
		CWOBackendReq req("api_GetInventoryData.aspx");
		req.AddSessionInfo(profile_.CustomerID, profile_.SessionID);
		req.AddParam("skey1",  g_ServerApiKey);
		req.AddParam("Data", 2);
		req.AddParam("Inventory", n.Inventory+1);//(int)m_inventoryID);
		req.AddParam("CustomerID", profile_.CustomerID);
		req.AddParam("CharID", 0);
		req.AddParam("Slot", -1);
		req.AddParam("ItemID", item.itemID);
		req.AddParam("Quantity", n.Quantity);
		req.AddParam("Var1", n.var1);
		req.AddParam("Var2", n.var2);
		const WeaponConfig* wc = g_pWeaponArmory->getWeaponConfig(item.itemID);
		if (wc)
		req.AddParam("Category",  wc->category);
		else
		req.AddParam("Category",  0);

		//req.AddParam("Durability",  Durability);

		if(!req.Issue())
		{
			r3dOutToLog("GetInventoryData FAILED, code: %d\n", req.resultCode_);
			return;
		}
	}
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_VaultBackpackFromInv_s& n) // del inventario a mi
{

	r3dOutToLog("#### ItemID %i InventoryID %i Var1 %i Var2 %i Quantity %i\n",n.itemID,n.m_inventoryID,n.var1,n.var2,n.Quantity);

	wiInventoryItem wi;
	wi.InventoryID = n.m_inventoryID;
	wi.itemID   = n.itemID;
	wi.quantity = n.Quantity;	
	wi.Var1 = n.var1;
	wi.Var2 = n.var2;
	//wi.Durability = n.Durability;
	BackpackAddItem(wi);
	r3dOutToLog("#### HUDVault add item from inventory: %d added!\n", n.itemID);
	gServerLogic.ApiPlayerUpdateChar(this);

}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_SendHelpCall_s& n)
{
//RelayPacket(&n, sizeof(n), true);
	PKT_S2C_SendHelpCallData_s n1;
	sprintf(n1.DistressText,n.DistressText);
	n1.peerid = peerId_;
	n1.pName = n.pName;
	n1.pos = n.pos;
	sprintf(n1.rewardText,n.rewardText);
//gServerLogic.p2pBroadcastToActive(this, &n1, sizeof(n1));
	 gServerLogic.p2pBroadcastToAll(NULL, &n1, sizeof(n1), true);
//r3dOutToLog("SendToHelp '%s' '%s '%s'\n",n.pName,n.DistressText,n.rewardText);
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_SafelockData_s& n)
{
	switch(n.Status)
	{
	default:
		break;
	case 0:
		{
			wiInventoryItem item = loadout_->Items[n.itemIDorGrid];

			char* g_ServerApiKey = "bvx425698dg6GsnxwedszF";
			CWOBackendReq req("api_SrvSafeLock.aspx");
			req.AddSessionInfo(profile_.CustomerID, profile_.SessionID);
			req.AddParam("skey1",  g_ServerApiKey);
			req.AddParam("SafeLockID", n.SafelockID);
			req.AddParam("Password", n.Password);
			req.AddParam("itemID", item.itemID);
			req.AddParam("ExpireTime", n.ExpireTime);
			req.AddParam("GamePos", 0);
			req.AddParam("GameRot", 0);
			req.AddParam("MapID",  n.MapID);
			req.AddParam("Quantity",  n.Quantity);
			req.AddParam("Var1",  n.Var1);
			req.AddParam("Var2",  n.Var2);
			req.AddParam("GameServerID", n.GameServerID);
			const WeaponConfig* wc = g_pWeaponArmory->getWeaponConfig(item.itemID);
			if (wc)
				req.AddParam("Category",  wc->category);
			else
				req.AddParam("Category",  0);
			req.AddParam("CustomerID", 0);
			//req.AddParam("Durability",n.Durability);

			// issue
			if(!req.Issue())
			{
				r3dOutToLog("!!!! SaveSafeLock failed, code: %d\n", req.resultCode_);
			}

			// remove used item
			r3dOutToLog("#### Found items for delete ItemID: %i, quantity have: %i, quantity for delete: %i\n",item.itemID,item.quantity,n.Quantity);
			if(n.Quantity != 0)
			{	
				int RemoveQnt = 0;
				RemoveQnt = item.quantity -= n.Quantity;
				r3dOutToLog("#### Quantity deleted: %i\n",RemoveQnt);
				DoRemoveSafeLockItems(n.itemIDorGrid,RemoveQnt,n.Quantity);
			}
			break;
		}
	case 1:
		{
			r3dOutToLog("#### ItemID for move: %i\n", n.itemIDorGrid);

			char* g_ServerApiKey = "bvx425698dg6GsnxwedszF";
			CWOBackendReq req("api_RemoveSafelockDATA.aspx");
			req.AddSessionInfo(profile_.CustomerID, profile_.SessionID);
			req.AddParam("skey1",  g_ServerApiKey);
			req.AddParam("SafeLockID", n.SafelockID);
			req.AddParam("ItemID", n.itemIDorGrid);
			req.AddParam("Quantity", n.Quantity);
			req.AddParam("Var1", n.Var1);
			req.AddParam("Var2", n.Var2);
			req.AddParam("GameServerID", n.GameServerID);
			//req.AddParam("Durability", n.Durability);

			if(!req.Issue())
			{
				r3dOutToLog("!!!! SaveSafeLock Remove failed, code: %d\n", req.resultCode_);
			}

			int itemID = n.itemIDorGrid;
			wiInventoryItem wi;
			wi.itemID   = itemID;
			wi.quantity = n.Quantity;	
			wi.Var1 = n.Var1;
			wi.Var2 = n.Var2;
			//wi.Durability = n.Durability;
			BackpackAddItem(wi);

			r3dOutToLog("#### BackpackFromInv ItemID: %d added!\n", itemID);
			break;
		}
	case 2:
		{
			__int64 secs1 = _time64(&secs1);

			int RegSeconds = (int)secs1 + 608400;

			if (!gServerLogic.WriteDBSafelock(n.myID, // ID of player
					n.SafeID, // SafeID of Safelock
					0, // ItemID for add to Table
					RegSeconds, // Expire in seconds
					n.Password, // password of Safelock
					n.pos, // Position of Safelock
					n.rot, // rotation of Safelock
					n.MapID, // Map ID for safelock
					n.Quantity,// Quantity of itemsID
					n.Var1,
					n.Var2,
					n.GameServerID))
			return;



			GameObject* obj = GameWorld().GetNetworkObject(n.SafelockID);
			obj_ServerBarricade* shield = (obj_ServerBarricade*)obj;
			strcpy(shield->Password,n.Password);
			shield->SafeLockID=n.SafeID;
			shield->MapID=n.MapID;
			shield->Health = 10000000000;
			shield->ExpireTime=RegSeconds;
			shield->GameServerID = n.GameServerID;
			break;
		}
	case 3:
		{
			for( GameObject* obj = GameWorld().GetFirstObject(); obj; obj = GameWorld().GetNextObject(obj) ) // Server Vehicles
			{
				if (obj->Class->Name == "obj_ServerBarricade") //safelock
				{
					obj_ServerBarricade* shield = (obj_ServerBarricade*)obj;
					if (shield->m_ItemID == 101348)
					{
						if (shield->SafeLockID == n.SafelockID && strcmp(shield->Password,"") != 0)
						{
							gServerLogic.RemoveDBSafelock(this,shield->SafeLockID,0,0,0,0,shield->GameServerID);
							int SafeID = shield->GetNetworkID();
							PKT_C2S_SafelockData_s n;
							n.Status = 7;
							n.IDNetwork=toP2pNetId(SafeID);
							gServerLogic.p2pBroadcastToActive(this, &n, sizeof(n));

							shield->DoDamage(10000000000);

							int itemID = n.itemIDorGrid;
							wiInventoryItem wi;
							wi.itemID   = 101348;
							wi.quantity = 1;	
							wi.Var1 = -1;
							wi.Var2 = -1;
							//wi.Durability = 100;
							BackpackAddItem(wi);

							r3dOutToLog("#### BackpackFromInv ItemID: 101348 added!\n");
							break;
						}
					}
				}
			}
			break;
		}
	case 4:
		{
			break;
		}
	case 5:
		{
 			for( GameObject* obj = GameWorld().GetFirstObject(); obj; obj = GameWorld().GetNextObject(obj) ) // Server Vehicles
			{
				if (obj->Class->Name == "obj_ServerBarricade") //safelock
				{
					obj_ServerBarricade* shield = (obj_ServerBarricade*)obj;
					if (shield->m_ItemID == 101348)
					{
						if (strcmp(shield->Password,"") != 0 && shield->MapID == n.MapID && shield->SafeLockID == n.iDSafeLock && shield->GameServerID == n.GameServerID)
						{
							if (n.StateSesion == 1)
							{
								if (shield->SesionID == 0)
								{
									GameObject* targetObj = GameWorld().GetNetworkObject(n.CustomerID);
									if (targetObj)
									{
										if (targetObj->Class->Name == "obj_ServerPlayer")
										{
											obj_ServerPlayer* plr = (obj_ServerPlayer*)targetObj;
											__int64 secs1 = _time64(&secs1);
											shield->SesionID = n.CustomerID;
											PKT_C2S_SafelockData_s n;
											n.Status = 5;
											n.State = 0;
											n.MapID = shield->MapID;
											n.GameServerID = shield->GameServerID;
											n.CustomerID = shield->SesionID;
											n.iDSafeLock = shield->SafeLockID;
											strcpy(n.Password,shield->Password);
											char dateforexpire[512];
											if (((shield->ExpireTime-(int)secs1)/60) <=60)
												sprintf(dateforexpire,"%i $MIN",((shield->ExpireTime-(int)secs1)/60));
											else if ((((shield->ExpireTime-(int)secs1)/60)/60) <=24)
												sprintf(dateforexpire,"%i $HRS",(((shield->ExpireTime-(int)secs1)/60)/60));
											else
												sprintf(dateforexpire,"%i $DAYS",((((shield->ExpireTime-(int)secs1)/60)/60)/24));
											strcpy(n.DateExpire,dateforexpire);
											gServerLogic.p2pSendToPeer(plr->peerId_, plr, &n, sizeof(n));
										}
									}
									break;
								}
								else {
									GameObject* targetObj = GameWorld().GetNetworkObject(n.CustomerID);
									if (targetObj)
									{
										obj_ServerPlayer* plr = (obj_ServerPlayer*)targetObj;
										PKT_C2S_SafelockData_s n;
										n.Status = 5;
										n.State = 1;
										n.CustomerID = 0;
										strcpy(n.DateExpire,"");
										n.GameServerID = 0;
										n.iDSafeLock = 0;
										n.MapID = 0;
										strcpy(n.Password,"");
										n.StateSesion = 0;
										gServerLogic.p2pSendToPeer(plr->peerId_, plr, &n, sizeof(n));
									}
								}
							}
							else {
									shield->SesionID = 0;
									break;
							}
						}
					}
				}
			}
			break;
		}
	case 6:
		{
			GameObject* obj = GameWorld().GetNetworkObject(n.SafelockID);
			if (!obj)
				return;
			GameObject* target = GameWorld().GetNetworkObject(n.ID);

			if (!target)
				return;

			if (obj->GetNetworkID() != n.SafelockID)
				return;

			if (target->Class->Name == "obj_ServerPlayer")
			{
				obj_ServerPlayer* plr = (obj_ServerPlayer*)target;
				obj_ServerBarricade* shield = (obj_ServerBarricade*)obj;

				if (!plr)
					return;

				if (strcmp(shield->Password,"") != 0)
				{
					PKT_C2S_SafelockData_s n2;
					n2.Status = 4;
					n2.ExpSeconds = shield->ExpireTime;
					strcpy(n2.Password,shield->Password);
					n2.pos		 = obj->GetPosition();
					n2.rot		 = obj->GetRotationVector().x;
					n2.SafeID	 = shield->SafeLockID;
					n2.SafelockID = obj->GetNetworkID();
					n2.MapID     = shield->MapID;
					n2.GameServerID  = shield->GameServerID;
					gServerLogic.p2pSendToPeer(plr->peerId_,plr, &n2, sizeof(n2));
					//r3dOutToLog("#### PASSWORD %s\n",shield->Password);
				}
			}
			break;
		}
	case 7:
		{
			break;
		}
	case 8:
		{
 			for( GameObject* obj = GameWorld().GetFirstObject(); obj; obj = GameWorld().GetNextObject(obj) ) // Server Vehicles
			{
				if (obj->Class->Name == "obj_ServerBarricade") //safelock
				{
					obj_ServerBarricade* shield = (obj_ServerBarricade*)obj;
					if (shield->m_ItemID == 101348)
					{
						if (shield->SafeLockID == n.IDofSafeLock)
						{
							GameObject* target = GameWorld().GetNetworkObject(n.IDPlayer);

							if (!target)
							return;

							obj_ServerPlayer* plr = (obj_ServerPlayer*)target;

							char* g_ServerApiKey = "bvx425698dg6GsnxwedszF";
							CWOBackendReq req("api_ChangePassSafeLock.aspx");
							req.AddSessionInfo(plr->profile_.CustomerID, plr->profile_.SessionID);
							req.AddParam("skey1",  g_ServerApiKey);
							req.AddParam("SafeLockID", shield->SafeLockID);
							req.AddParam("Password", n.NewPassword);
							req.AddParam("GameServerID", shield->GameServerID);
							// issue
							if(!req.Issue())
							{
								r3dOutToLog("!!!! Change password failed of SafeLock, code: %d\n", req.resultCode_);
								//return req.resultCode_;
								return;
							}

							strcpy(shield->Password,n.NewPassword);

							PKT_C2S_SafelockData_s n2;
							n2.Status = 4;
							n2.ExpSeconds = shield->ExpireTime;
							strcpy(n2.Password,n.NewPassword);
							n2.pos		 = obj->GetPosition();
							n2.rot		 = obj->GetRotationVector().x;
							n2.SafeID	 = shield->SafeLockID;
							n2.SafelockID = obj->GetNetworkID();
							n2.MapID     = shield->MapID;
							n2.GameServerID  = shield->GameServerID;
							gServerLogic.p2pBroadcastToActive(plr, &n2, sizeof(n2));
							//r3dOutToLog("#### Changing password to %s - %s , %i Safelock\n",shield->Password,n.NewPassword,shield->SafeLockID);
							break;
						}
					}
				}
			}
			break;
		}
	}
	gServerLogic.ApiPlayerUpdateChar(this);
	
}
void obj_ServerPlayer::OnNetPacket(const PKT_C2S_UnloadClipReq_s& n)
{
	if(loadout_->Alive == 0)
		return;

	wiInventoryItem item = loadout_->Items[n.slotID];

	const WeaponConfig* wc = g_pWeaponArmory->getWeaponConfig(item.itemID);
	//m_WeaponArray[n.slotID]
	if(wc)
	{
		if(wc->category >= storecat_ASR && wc->category <= storecat_SMG)
		{
			if(item.Var1 > 0 && item.Var2 > 0)
			{
				wiInventoryItem wi;
				wi.itemID   = item.Var2;
				wi.quantity = 1;
				wi.Var1 = item.Var1;

				item.Var1 = 0;
				item.Var2 = wi.itemID;

				if(BackpackAddItem(wi))
					loadout_->Items[n.slotID] = item;

				OnBackpackChanged(n.slotID);
				gServerLogic.ApiPlayerUpdateChar(this); // Vault

			}else{
				gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, false, "Clip is serverside empty",
				"slot%d %d - %d", n.slotID, item.itemID, item.Var1);
			}
		}
	}
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_TradeCancel_s& n)
{
    GameObject* targetObj = GameWorld().GetNetworkObject((DWORD)n.netId);
    if (targetObj)
    {
        if (!IsServerPlayer(targetObj))
        {
            gServerLogic.LogCheat(peerId_,99,true,"Trade","not player!");
            return;
        }
        obj_ServerPlayer* tplr = (obj_ServerPlayer*)targetObj;
        for (int i=0;i<72;i++)
        {
            tplr->TradeItems[i].Reset();
            tplr->isTradeAccept = false;
            tplr->TradeSlot[i] = -1;
        }
        PKT_C2S_TradeCancel_s n1;
        gServerLogic.p2pSendToPeer(tplr->peerId_,tplr,&n1,sizeof(n1));
    }
    for (int i=0;i<72;i++)
    {
        this->TradeItems[i].Reset();
        this->isTradeAccept = false;
        this->TradeSlot[i] = -1;
    }
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_TradeOptoBack_s& n)
{
    if (isTradeAccept) 
    {
        gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Data, true, "isTradeAccept");
        return;
    }


    GameObject* targetObj = GameWorld().GetNetworkObject((DWORD)n.netId);
    if (targetObj)
    {
        if (!IsServerPlayer(targetObj))
        {
            gServerLogic.LogCheat(peerId_,99,true,"Trade","not player!");
            return;
        }


        r3dOutToLog("Trade: OptoBack %s ItemID %d\n",userName,n.Item.itemID);
        //TradeItems[TradeNums].Reset();
        //TradeSlot[TradeNums] = -1;


        //if (TradeNums > 0)
        // TradeNums--;


        GameObject* obj = GameWorld().GetNetworkObject(Tradetargetid);
        if (IsServerPlayer(obj))
        {
            obj_ServerPlayer* plr = (obj_ServerPlayer*)obj;
            if (plr && plr->isTradeAccept)
            {
                plr->isTradeAccept = false;
                for (int i=0;i<72;i++)
                {
                    plr->TradeItems[i].Reset();
                    plr->TradeSlot[i] = -1;
                }
            }
        }


        isTradeAccept = false;
        for (int i=0;i<72;i++)
        {
            TradeItems[i].Reset();
            TradeSlot[i] = -1;
        }

        obj_ServerPlayer* tplr = (obj_ServerPlayer*)targetObj;
        PKT_C2S_TradeOptoBack_s n1;
        n1.Item = n.Item;
        n1.slotfrom = n.slotfrom;
        gServerLogic.p2pSendToPeer(tplr->peerId_,tplr,&n1,sizeof(n1));
    }
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_TradeBacktoOp_s& n)
{
    if (isTradeAccept) 
    {
        gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Data, true, "isTradeAccept");
        return;
    }


    GameObject* targetObj = GameWorld().GetNetworkObject((DWORD)n.netId);
    if (targetObj)
    {
        if (!IsServerPlayer(targetObj))
        {
            gServerLogic.LogCheat(peerId_,99,true,"Trade","not player!");
            return;
        }


        r3dOutToLog("Trade: BackToOp %s ItemID %d\n",userName,n.Item.itemID);


        //TradeItems[TradeNums].Reset();
        //TradeItems[TradeNums] = n.Item;
        //TradeSlot[TradeNums] = n.slotto;
        //TradeNums++;


        GameObject* obj = GameWorld().GetNetworkObject(Tradetargetid);
        if (IsServerPlayer(obj))
        {
            obj_ServerPlayer* plr = (obj_ServerPlayer*)obj;
            if (plr && plr->isTradeAccept)
            {
                plr->isTradeAccept = false;
                for (int i=0;i<72;i++)
                {
                    plr->TradeItems[i].Reset();
                    plr->TradeSlot[i] = -1;
                }
            }
        }


        isTradeAccept = false;
        for (int i=0;i<72;i++)
        {
            TradeItems[i].Reset();
            TradeSlot[i] = -1;
        }


        obj_ServerPlayer* tplr = (obj_ServerPlayer*)targetObj;
        PKT_C2S_TradeBacktoOp_s n1;
        n1.Item = n.Item;
        n1.slotto = n.slotto;
        gServerLogic.p2pSendToPeer(tplr->peerId_,tplr,&n1,sizeof(n1));
    }
}
void obj_ServerPlayer::OnNetPacket(const PKT_C2S_TradeRequest_s& n)
{
    GameObject* targetObj = GameWorld().GetNetworkObject((DWORD)n.netId);
    if (targetObj)
    {
        if (!IsServerPlayer(targetObj))
        {
            gServerLogic.LogCheat(peerId_,99,true,"Trade","not player!");
            return;
        }
        obj_ServerPlayer* tplr = (obj_ServerPlayer*)targetObj;
        PKT_C2S_TradeRequest_s n1;
        n1.netId = (int)this->GetNetworkID();
        gServerLogic.p2pSendToPeer(tplr->peerId_,tplr,&n1,sizeof(n1));
    }
}
void obj_ServerPlayer::OnNetPacket(const PKT_C2S_TradeAccept2_s& n)
{
    // stored data


    if (isTradeAccept) 
    {
        gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Data, true, "isTradeAccept");
        return;
    }


    for (int i=0;i<72; i++)
    {
        TradeItems[i] = n.Item[i];
        TradeSlot[i] = n.slot[i];
    }


    this->Tradetargetid = (DWORD)n.netId;
    this->isTradeAccept = true;
    // the trading will start in Update() now we will wait a other player


    GameObject* targetObj = GameWorld().GetNetworkObject((DWORD)n.netId);
    if (targetObj && IsServerPlayer(targetObj))
    {
        obj_ServerPlayer * plr = (obj_ServerPlayer*)targetObj;
        PKT_C2S_TradeAccept2_s n1;
        gServerLogic.p2pSendToPeer(plr->peerId_,plr,&n1,sizeof(n1));
    }
}
void obj_ServerPlayer::OnNetPacket(const PKT_C2S_StackClip_s& n)
{
	wiInventoryItem wi = loadout_->Items[n.slotId];
	const WeaponAttachmentConfig* wac = g_pWeaponArmory->getAttachmentConfig(wi.itemID);
	if (wi.itemID == 0 || !wac || wac->category != storecat_FPSAttachment) 
		return;

	// save old loadout. use if additem failed.
	wiInventoryItem OldItems[72];
	for (int i=0;i<72;i++)
	{
		OldItems[i].Reset();

		OldItems[i] = loadout_->Items[i];
	}

	// find the same clips itemid. and not full clip.
	int slotid[72];

	for (int i=0;i<72;i++)
	{
		slotid[i] = -1;
	}

	for (int i=0;i<loadout_->BackpackSize;i++)
	{
		wiInventoryItem wi2 = loadout_->Items[i];
		if (wi2.itemID != 0 && wi2.itemID == wi.itemID /*&& wi2.Var1 != wac->m_Clipsize*/)
			slotid[i] = i;
	}

	int NumBulletToStack = 0;
	for (int i=0;i<72;i++)
	{
		if (slotid[i] != -1)
		{
			wiInventoryItem mag = loadout_->Items[slotid[i]];
			NumBulletToStack += (mag.Var1==-1 ? wac->m_Clipsize : mag.Var1)*mag.quantity;
			//r3dOutToLog("Var1 %d\n",mag.Var1);
			DoRemoveItems(slotid[i]);
		}
	}

	int NumLeftBulletToStack = NumBulletToStack;
	//int NumAttmStack = NumBulletToStack/wac->m_Clipsize;

	//if (NumAttmStack < 1) NumAttmStack = 1;
	//r3dOutToLog("%d\n",NumLeftBulletToStack);

	while (NumLeftBulletToStack > 0)
	{
		wiInventoryItem clips;
		clips.itemID = wi.itemID;
		clips.quantity = 1;
		if (NumLeftBulletToStack >= wac->m_Clipsize)
		{
			clips.Var1 = -1; // -1 is full bullets. this is for stackable Clips.
			NumLeftBulletToStack -= wac->m_Clipsize;
		}
		else
		{
			clips.Var1 = NumLeftBulletToStack;
			NumLeftBulletToStack = -1;
		}
		if (!BackpackAddItem(clips)) // this is happened , your backpack is full!
		{
			// clear all items and fill the old items loadout.
			DoRemoveAllItems(this);
			for (int i=0;i<72;i++)
			{
				if (OldItems[i].itemID > 0)
				{
					BackpackAddItem(OldItems[i]);
				}
			}
			break; // stop this loop.
		}
		//r3dOutToLog("%d\n",NumLeftBulletToStack);
	}

	for (int i=0;i<72;i++)
	{
		OldItems[i].Reset();

		OldItems[i] = loadout_->Items[i];
	}
	r3dOutToLog("%s Stack Clips %s %d %d %d\n",userName,NumLeftBulletToStack > 0 ? "Failed" : "Success",wi.itemID,NumBulletToStack,wac->m_Clipsize);
	gServerLogic.ApiPlayerUpdateChar(this);
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_DisconnectReq_s& n)
{
	r3dOutToLog("PKT_C2S_DisconnectReq for %s\n", userName);
	if(loadout_->Alive == 0)
	{
		PKT_C2S_DisconnectReq_s n2;
		gServerLogic.p2pSendToPeer(peerId_, this, &n2, sizeof(n2));

		gServerLogic.DisconnectPeer(peerId_, false, "disconnect request while dead, we already updated profile");
		return;
	}

	// start update thread, player will disconnect itself when thread is finished
	if(!wasDisconnected_)
	{
		gServerLogic.ApiPlayerUpdateChar(this, true);
		wasDisconnected_ = true;
	}
}

/*void obj_ServerPlayer::OnNetPacket(const PKT_C2S_FallingDamage_s& n)
{
	r3dOutToLog("Falling damage to %s, damage=%.2f\n", Name.c_str(), n.damage); CLOG_INDENT;
	gServerLogic.ApplyDamage(this, this, GetPosition(), n.damage, true, storecat_INVALID);
}*/

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_FallingDamage_s& n)
{
	float nulldamage = 100.0f;
	if (loadout_->Stats.skillid23)
	        nulldamage = 75.0f;
	if (loadout_->Stats.skillid27)
			nulldamage = 50.0f;

	/*r3dOutToLog("Falling damage to %s, damage=%.2f\n", Name.c_str(), nulldamage); CLOG_INDENT;
    if (r3d_float_isFinite(nulldamage))  //add this line
    gServerLogic.ApplyDamage(this, this, GetPosition(), nulldamage, true, storecat_INVALID);*/
}
void obj_ServerPlayer::OnNetPacket(const PKT_C2S_BuyItemReq_s& n)
{

	char* g_ServerApiKey = "bvx425698dg6GsnxwedszF";
	CWOBackendReq req("api_GetItemQuantityBuy.aspx");
	req.AddSessionInfo(profile_.CustomerID, profile_.SessionID);
	req.AddParam("skey1",  g_ServerApiKey);
	req.AddParam("ItemID", n.ItemID);

	if(!req.Issue())
	{
		r3dOutToLog("!!!! BuyItemReq FAILLLLL, code: %d\n", req.resultCode_);
		PKT_C2S_BuyItemReq_s n1;
		n1.ItemID = 0;
		n1.ansCode = 1;
		n1.GP=0;
		n1.GD=0;
		gServerLogic.p2pSendToPeer(peerId_, this, &n1, sizeof(n1));
		return;
	}

	pugi::xml_document xmlFile;
	req.ParseXML(xmlFile);
	pugi::xml_node xmlBuyItem = xmlFile.child("Items_Weapons");

	int Quantity = 1;

	while(!xmlBuyItem.empty())
	{		
        Quantity = xmlBuyItem.attribute("Clipsize").as_int();

		xmlBuyItem = xmlBuyItem.next_sibling();
	}

			wiInventoryItem wi;
			wi.itemID = n.ItemID;
			wi.quantity = Quantity;
			wi.Var1 = -1;
			wi.Var2 = -1;
			//wi.Durability = 100;
			if (IsItemCanAddToInventory(wi))
			{
				BackpackAddItem(wi);

				profile_.ProfileData.GameDollars-=n.GD;
				profile_.ProfileData.GamePoints-=n.GP;
				gServerLogic.ApiPlayerUpdateChar(this);

				PKT_C2S_BuyItemReq_s n1;
				n1.ItemID = 0;
				n1.ansCode = 0;
				n1.GP=0;
				n1.GD=0;
				gServerLogic.p2pSendToPeer(peerId_, this, &n1, sizeof(n1));
				return;
			}
			else
			{
				PKT_C2S_BuyItemReq_s n1;
				n1.ItemID = 0;
				n1.ansCode = 1;
				n1.GP=0;
				n1.GD=0;
				gServerLogic.p2pSendToPeer(peerId_, this, &n1, sizeof(n1));
			}
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_TradeAccept_s& n)
{
    GameObject* targetObj = GameWorld().GetNetworkObject((DWORD)n.netId);


    for (int i=0;i<72;i++)
    {
        this->TradeItems[i].Reset();
        this->isTradeAccept = false;
        this->TradeSlot[i] = -1;
    }

    Tradetargetid = (DWORD)n.netId;
    TradeNums = 0;

    if (targetObj && IsServerPlayer(targetObj))
    {
        obj_ServerPlayer * plr = (obj_ServerPlayer*)targetObj;


        plr->TradeNums = 0;
        plr->Tradetargetid = GetNetworkID();


        for (int i=0;i<72;i++)
        {
            plr->TradeItems[i].Reset();
            plr->isTradeAccept = false;
            plr->TradeSlot[i] = -1;
        }
        PKT_C2S_TradeAccept_s n1;
        n1.netId = this->GetNetworkID();
        gServerLogic.p2pSendToPeer(plr->peerId_,plr,&n1,sizeof(n));
    }
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_PlayerWeapDataRep_s& n)
{
	lastWeapDataRep = r3dGetTime();

	// if weapon data was updated more that once it mean that updated happened in middle of the game
	// so skip validation
	if(gServerLogic.weaponDataUpdates_ >= 2)
		return;

	for(int i=0; i<2; i++)
	{
		if(m_WeaponArray[i] == NULL)
			continue;
		DWORD hash = m_WeaponArray[i]->getConfig()->GetClientParametersHash();

		if(hash == n.weaponsDataHash[i])
			continue;

		const WeaponConfig& wc1 = *m_WeaponArray[i]->getConfig();
		WeaponConfig wc2(n.debug_wid[i]);
		wc2.copyParametersFrom(n.debug_winfo[i]);

		if(wc1.m_itemID != wc2.m_itemID)
		{
			gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_Protocol, false, "weapDataRep different",
				"slot%d %d - %d", i, wc1.m_itemID, wc2.m_itemID);
		}
		else
		{
			// create diffs string for logging
			char diffs[4096] = "";
			if(fabs((float)wc1.m_spread - (float)wc2.m_spread) > 0.01f)
				sprintf(diffs + strlen(diffs), "s:%.2f/%.2f ", (float)wc1.m_spread, (float)wc2.m_spread);
			if(fabs((float)wc1.m_recoil - (float)wc2.m_recoil) > 0.01f)
				sprintf(diffs + strlen(diffs), "r:%.2f/%.2f ", (float)wc1.m_recoil, (float)wc2.m_recoil);
			if(fabs((float)wc1.m_reloadTime - (float)wc2.m_reloadTime) > 0.01f)
				sprintf(diffs + strlen(diffs), "t:%.2f/%.2f ", (float)wc1.m_reloadTime, (float)wc2.m_reloadTime);
			if(fabs((float)wc1.m_fireDelay - (float)wc2.m_fireDelay) > 0.01f)
				sprintf(diffs + strlen(diffs), "d:%.2f/%.2f ", (float)wc1.m_fireDelay, (float)wc2.m_fireDelay);
			if(fabs((float)wc1.m_AmmoSpeed - (float)wc2.m_AmmoSpeed) > 0.01f)
				sprintf(diffs + strlen(diffs), "a:%.2f/%.2f ", (float)wc1.m_AmmoSpeed, (float)wc2.m_AmmoSpeed);

			// report it only once per session (for now, because there is no disconnect yet)
			if(diffs[0] && !weapCheatReported)
			{
				weapCheatReported = true;
				gServerLogic.LogCheat(peerId_, PKT_S2C_CheatWarning_s::CHEAT_BadWeapData, false, "weapDataRep different",
					"id:%d, diff:%s", wc1.m_itemID, diffs);
			}
		}
	}
}

void obj_ServerPlayer::SetLatePacketsBarrier(const char* reason)
{
	if(isTargetDummy_)
		return;

	r3dOutToLog("peer%02d, SetLatePacketsBarrier: %s\n", peerId_, reason);

	packetBarrierReason = reason;
	myPacketSequence++;
	lastWeapDataRep = r3dGetTime();

	PKT_C2C_PacketBarrier_s n;
	gServerLogic.p2pSendToPeer(peerId_, this, &n, sizeof(n));

	// NOTE:
	// from now on, we'll ignore received packets until client ack us with same barrier packet.
	// so, any fire/move/etc requests that will invalidate logical state of player will be automatically ignored
}


#undef DEFINE_GAMEOBJ_PACKET_HANDLER
#define DEFINE_GAMEOBJ_PACKET_HANDLER(xxx) \
	case xxx: { \
		const xxx##_s&n = *(xxx##_s*)packetData; \
		if(packetSize != sizeof(n)) { \
			r3dOutToLog("!!!!errror!!!! %s packetSize %d != %d\n", #xxx, packetSize, sizeof(n)); \
			return TRUE; \
		} \
		OnNetPacket(n); \
		return TRUE; \
	}

BOOL obj_ServerPlayer::OnNetReceive(DWORD EventID, const void* packetData, int packetSize)
{
	//@TODO somehow check that originator of that packet have playerIdx that match peer

	// packets that ignore packet sequence
	switch(EventID)
	{
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_PacketBarrier);
	}

	if(myPacketSequence != clientPacketSequence)
	{
		// we get late packet after late packet barrier, skip it
		r3dOutToLog("peer%02d, CustomerID:%d LatePacket %d %s\n", peerId_, profile_.CustomerID, EventID, packetBarrierReason);
		return TRUE;
	}

	// packets while dead
	switch(EventID)
	{
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_MoveSetCell);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_MoveRel);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_DisconnectReq);
	}

	if(wasDisconnected_)
		return TRUE;

	if(loadout_->Alive == 0)
	{
		//r3dOutToLog("peer%02d, CustomerID:%d packet while dead %d\n", peerId_, profile_.CustomerID, EventID);
		return TRUE;
	}

	switch(EventID)
	{
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_PlayerWeapDataRep);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_PlayerJump);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_TradeAccept);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_TradeCancel);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_TradeOptoBack);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_TradeRequest);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_TradeAccept2);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_TradeBacktoOp);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_PlayerReload);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_PlayerFired);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_PlayerHitNothing);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_PlayerHitStatic);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_PlayerHitStaticPierced);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_PlayerHitDynamic);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_PlayerReadyGrenade);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_PlayerThrewGrenade);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_PlayerChangeBackpack);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_BackpackDrop);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_BackpackSwap);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_BackpackJoin);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_VaultBackpackToInv);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_VaultBackpackFromInv);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_FallingDamage);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_PlayerEquipAttachment);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_PlayerRemoveAttachment);
		//DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_S2C_RepairALLWeapon);
		//DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_S2C_RepairWeapon);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_PlayerSwitchWeapon);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_PlayerUseItem);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_HackShieldLog);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_PlayerCraftItem);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_UnloadClipReq);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_SafelockData);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_StackClip);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_PlayerOnVehicle);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_flashlightToggle); // flashlight
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2C_UnarmedCombat); // Unarmed Combat
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_SendHelpCall);
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_CarKill); // Server Vehicles
		DEFINE_GAMEOBJ_PACKET_HANDLER(PKT_C2S_BuyItemReq);
	}

	return FALSE;
}
#undef DEFINE_GAMEOBJ_PACKET_HANDLER

DefaultPacket* obj_ServerPlayer::NetGetCreatePacket(int* out_size)
{
	static PKT_S2C_CreatePlayer_s n;

	r3d_assert(GetNetworkID());

	n.CustomerID= profile_.CustomerID ^ 0x54281162; // encode CustomerID so it won't look linear on client side
	n.playerIdx = BYTE(GetNetworkID() - NETID_PLAYERS_START);
	n.spawnPos  = GetPosition();
	n.moveCell  = netMover.SrvGetCell();
	n.spawnDir  = m_PlayerRotation;
	n.weapIndex = m_SelectedWeapon;
	n.isFreeHands = m_ForcedEmptyHands?1:0;

	// loadout part
	const wiCharDataFull& slot = *loadout_;
	n.HeroItemID = slot.HeroItemID;
	n.HeadIdx    = (BYTE)slot.HeadIdx;
	n.BodyIdx    = (BYTE)slot.BodyIdx;
	n.LegsIdx    = (BYTE)slot.LegsIdx;
	n.WeaponID0  = slot.Items[wiCharDataFull::CHAR_LOADOUT_WEAPON1].itemID;
	n.WeaponID1  = slot.Items[wiCharDataFull::CHAR_LOADOUT_WEAPON2].itemID;
	n.ArmorID    = slot.Items[wiCharDataFull::CHAR_LOADOUT_ARMOR].itemID;
	n.HeadGearID = slot.Items[wiCharDataFull::CHAR_LOADOUT_HEADGEAR].itemID;
	n.Item0      = slot.Items[wiCharDataFull::CHAR_LOADOUT_ITEM1].itemID;
	n.Item1      = slot.Items[wiCharDataFull::CHAR_LOADOUT_ITEM2].itemID;
	n.Item2      = slot.Items[wiCharDataFull::CHAR_LOADOUT_ITEM3].itemID;
	n.Item3      = slot.Items[wiCharDataFull::CHAR_LOADOUT_ITEM4].itemID;
	n.BackpackID = slot.BackpackID;
	n.Attm0      = GetWeaponNetAttachment(0);
	n.Attm1      = GetWeaponNetAttachment(1);

	r3dscpy(n.gamertag, slot.Gamertag);
	n.ClanID = slot.ClanID;
	n.GroupID = slot.GroupID;
	r3dscpy(n.ClanTag, slot.ClanTag);
	n.ClanTagColor = slot.ClanTagColor;

	n.Reputation = slot.Stats.Reputation;

	*out_size = sizeof(n);
	return &n;
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_PlayerOnVehicle_s& n)  // Server Vehicles
{
	PlayerOnVehicle=n.PlayerOnVehicle;
	IDOFMyVehicle=n.VehicleID;

	if (!PlayerOnVehicle)
	{
		GameObject* targetObj = GameWorld().GetNetworkObject(IDOFMyVehicle);

		if (targetObj)
		{
			obj_Vehicle* Vehicle = (obj_Vehicle*)targetObj;
			
			for (int i=0;i<=8;i++)
			{
				if (Vehicle->PlayersOnVehicle[i]==this->GetNetworkID())
				{
					Vehicle->PlayersOnVehicle[i]=0;
					Vehicle->OccupantsVehicle--;
					if (Vehicle->HaveDriver==this->GetNetworkID())
						Vehicle->HaveDriver=0;

					if (Vehicle->OccupantsVehicle<=0)
					{
						PKT_S2C_PositionVehicle_s n2; // Server Vehicles
						n2.spawnPos=Vehicle->GetPosition(); 
						n2.RotationPos = Vehicle->GetRotationVector();
						n2.OccupantsVehicle=0;
						n2.GasolineCar=Vehicle->Gasolinecar;
						n2.DamageCar=Vehicle->DamageCar;
						n2.RPMPlayer=0;
						n2.RotationSpeed=0;
						n2.RespawnCar=false;
						n2.bOn = false;
						n2.spawnID=Vehicle->GetNetworkID();
						n2.timeStep=0;
						gServerLogic.p2pBroadcastToAll(this, &n2, sizeof(n2), true);
					}

					r3dOutToLog("######## VehicleID: %i Have %i Passengers\n",IDOFMyVehicle,Vehicle->OccupantsVehicle);
					break;
				}
			}
		}
	}
	RelayPacket(&n, sizeof(n), false);
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_flashlightToggle_s& n)  // flashlight
{
	RelayPacket(&n, sizeof(n), false);
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2S_CarKill_s& n) // Server Vehicles
{
	//r3dOutToLog("ENTRA %i\n",n.targetId);
	GameObject* target = GameWorld().GetNetworkObject(n.targetId);
	if (!target) return;

	if (n.weaponID==101399)
	{
		int isHead = (n.extra_info == 1);
		if (target->isObjType(OBJTYPE_Zombie))
			gServerLogic.ApplyDamageToZombie(this,target,GetPosition()+r3dPoint3D(0,1,0),16, isHead, isHead, false, storecat_punch);
		else if (target->isObjType(OBJTYPE_Human))
		{
			obj_ServerPlayer* targetPlr = IsServerPlayer(target);
			if ( targetPlr->loadout_->GameMapId == GBGameInfo::MAPID_WZ_PVE_Colorado || targetPlr->PlayerOnVehicle == true || targetPlr->loadout_->Alive <=0 || targetPlr->profile_.ProfileData.isGod || (targetPlr->loadout_->GameFlags & wiCharDataFull::GAMEFLAG_SpawnProtection) || (targetPlr->loadout_->GameFlags & wiCharDataFull::GAMEFLAG_NearPostBox))
				return;
			else
			gServerLogic.ApplyDamage(this, target, GetPosition(), 16.0f, true, storecat_Vehicle);
		}
		return;
	}
	if (target->Class->Name == "obj_ServerBarricade")
	{
		obj_ServerBarricade* shield = (obj_ServerBarricade*)target;

		if (shield->m_ItemID == 101358)
		{
			if (n.weaponID != 100999 && n.weaponID != 101310)
				shield->DoDamage(10);
			else
				shield->DoDamage(50);
		}
	}
	if (target->isObjType(OBJTYPE_Zombie))
	{
		if (n.weaponID==6)
			gServerLogic.ApplyDamageToZombie(this,target,GetPosition()+r3dPoint3D(0,1,0),100, 1, 1, false, storecat_Vehicle);
		else
			gServerLogic.ApplyDamageToZombie(this,target,GetPosition()+r3dPoint3D(0,1,0),100, 1, 1, false, storecat_ShootVehicle);
	}
	else if (target->Class->Name == "obj_Vehicle")
	{
		obj_Vehicle* Vehicle = (obj_Vehicle*)target;

		if (Vehicle && Vehicle->OccupantsVehicle>0)
		{
			for (int i=0;i<=8;i++)
			{
				if (Vehicle->PlayersOnVehicle[i]!=0)
				{
					GameObject* from = GameWorld().GetNetworkObject(Vehicle->PlayersOnVehicle[i]);
					if (from)
					{
						obj_ServerPlayer* Player = (obj_ServerPlayer*)from;
						if (Player->loadout_->GameMapId == GBGameInfo::MAPID_WZ_PVE_Colorado || Player->profile_.ProfileData.isGod || (Player->loadout_->GameFlags & wiCharDataFull::GAMEFLAG_SpawnProtection) || (Player->loadout_->GameFlags & wiCharDataFull::GAMEFLAG_NearPostBox))
							return;

						PKT_C2S_DamageCar_s n2;
						n2.CarID = Vehicle->GetNetworkID();
						n2.WeaponID =n.weaponID;
						gServerLogic.p2pSendToPeer(Player->peerId_, this, &n2, sizeof(n2));
					}
				}
							
			}
		}
		
	}
	else if (target->isObjType(OBJTYPE_Human))
	{
		obj_ServerPlayer* targetPlr = IsServerPlayer(target);
		if (n.DieForExplosion == true && !targetPlr->profile_.ProfileData.isGod && !(targetPlr->loadout_->GameFlags & wiCharDataFull::GAMEFLAG_SpawnProtection) && !(targetPlr->loadout_->GameFlags & wiCharDataFull::GAMEFLAG_NearPostBox))
		{
			targetPlr->dieForExplosion=true;
			gServerLogic.DoKillPlayer(this,targetPlr,storecat_Vehicle);
		}
		else if ( targetPlr->loadout_->GameMapId == GBGameInfo::MAPID_WZ_PVE_Colorado || targetPlr->PlayerOnVehicle == true || targetPlr->loadout_->Alive <=0 || targetPlr->profile_.ProfileData.isGod || (targetPlr->loadout_->GameFlags & wiCharDataFull::GAMEFLAG_SpawnProtection) || (targetPlr->loadout_->GameFlags & wiCharDataFull::GAMEFLAG_NearPostBox))
		{
			return;
		}
		else {
			if (targetPlr->loadout_->GroupID == this->loadout_->GroupID)
			{
				if (targetPlr->loadout_->GroupID != 0)
					return;
			}
			gServerLogic.DoKillPlayer(this,targetPlr,storecat_Vehicle);
		}
	}
}

void obj_ServerPlayer::OnNetPacket(const PKT_C2C_UnarmedCombat_s& n)  // Unarmed Combat
{
	RelayPacket(&n, sizeof(n), false);
}

void obj_ServerPlayer::RelayPacket(const DefaultPacket* packetData, int packetSize, bool guaranteedAndOrdered)
{
	gServerLogic.RelayPacket(peerId_, this, packetData, packetSize, guaranteedAndOrdered);
}
