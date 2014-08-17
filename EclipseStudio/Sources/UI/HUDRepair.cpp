#include "r3dPCH.h"
#include "r3d.h"

#include "../../../Eternity/sf/Console/config.h"
#include "HUDRepair.h"
#include "LangMngr.h"

#include "FrontendShared.h"

#include "../multiplayer/clientgamelogic.h"
#include "../ObjectsCode/AI/AI_Player.H"
#include "../ObjectsCode/weapons/WeaponArmory.h"
#include "../ObjectsCode/weapons/Weapon.h"
#include "../GameLevel.h"

extern const wchar_t* getReputationString(int Reputation);

HUDRepair::HUDRepair():
isActive_(false),
isInit(false),
prevKeyboardCaptureMovie_(NULL)
{
}

HUDRepair::~HUDRepair()
{
}

bool HUDRepair::Init()
{
	if(!gfxMovie.Load("Data\\Menu\\WarZ_HUD_RepairBench.swf", false))
		return false;

#define MAKE_CALLBACK(FUNC) new r3dScaleformMovie::TGFxEICallback<HUDRepair>(this, &HUDRepair::FUNC)
	gfxMovie.RegisterEventHandler("eventReturnToGame", MAKE_CALLBACK(eventReturnToGame));
	gfxMovie.RegisterEventHandler("eventRepairAll", MAKE_CALLBACK(eventRepairAll));
	gfxMovie.RegisterEventHandler("eventRepairItemInSlot", MAKE_CALLBACK(eventRepairItemInSlot));

	gfxMovie.SetCurentRTViewport( Scaleform::GFx::Movie::SM_ExactFit );

	itemInventory_.initialize(&gfxMovie);

	isActive_ = false;
	isInit = true;
	return true;
}

void HUDRepair::addClientSurvivor(const wiCharDataFull& slot)
{
    Scaleform::GFx::Value var[22];
    char tmpGamertag[128];
    if(slot.ClanID != 0)
        sprintf(tmpGamertag, "[%s] %s", slot.ClanTag, slot.Gamertag);
    else
        r3dscpy(tmpGamertag, slot.Gamertag);
    var[0].SetString(tmpGamertag);
    var[1].SetNumber(slot.Health);
    var[2].SetNumber(slot.Stats.XP);
    var[3].SetNumber(slot.Stats.TimePlayed);
    var[4].SetNumber(slot.Hardcore);
    var[5].SetNumber(slot.HeroItemID);
    var[6].SetNumber(slot.HeadIdx);
    var[7].SetNumber(slot.BodyIdx);
    var[8].SetNumber(slot.LegsIdx);
    var[9].SetNumber(slot.Alive);
    var[10].SetNumber(slot.Hunger);
    var[11].SetNumber(slot.Thirst);
    var[12].SetNumber(slot.Toxic);
    var[13].SetNumber(slot.BackpackID);
    var[14].SetNumber(slot.BackpackSize);
    var[15].SetNumber(0);        // weight
    var[16].SetNumber(slot.Stats.KilledZombies);        // zombies Killed
    var[17].SetNumber(slot.Stats.KilledBandits);        // bandits killed
    var[18].SetNumber(slot.Stats.KilledSurvivors);        // civilians killed
     var[19].SetStringW(getReputationString(slot.Stats.Reputation));    // alignment
	switch(slot.GameMapId)
		{
			case GBGameInfo::MAPID_WZ_Colorado:
			var[20].SetString("COLORADO PVP");	// last Map
			break;
			case GBGameInfo::MAPID_WZ_PVE_Colorado:
			var[20].SetString("COLORADO PVE");	// last Map
			break;
			case GBGameInfo::MAPID_WZ_Cliffside:
			var[20].SetString("CLIFFSIDE PVP");	// last Map
			break;
			case GBGameInfo::MAPID_CaliWood:
			var[20].SetString("CALIWOOD");	// last Map
			break;
			case GBGameInfo::MAPID_Devmap:
			var[20].SetString("DEVMAP");	// last Map
			break;
			default:
			var[20].SetString("UNKNOWN");	// last Map
			break;
		}
    var[21].SetBoolean(true);

    gfxMovie.Invoke("_root.api.addClientSurvivor", var, 22);    
}
bool HUDRepair::Unload()
{
	gfxMovie.Unload();
	isActive_ = false;
	isInit = false;
	return true;
}

void HUDRepair::Update()
{
	gClientLogic().localPlayer_->RepairTime = r3dGetTime();
}

void HUDRepair::Draw()
{
	gfxMovie.UpdateAndDraw();
}

void HUDRepair::Deactivate()
{
	if(prevKeyboardCaptureMovie_)
	{
		prevKeyboardCaptureMovie_->SetKeyboardCapture();
		prevKeyboardCaptureMovie_ = NULL;
	}

	if(!g_cursor_mode->GetInt())
	{
		r3dMouse::Hide();
	}

	{
		Scaleform::GFx::Value var[1];
		var[0].SetString("menu_close");
		gfxMovie.OnCommandCallback("eventSoundPlay", var, 1);
	}


	isActive_ = false;
}

void HUDRepair::Activate()
{
	prevKeyboardCaptureMovie_ = gfxMovie.SetKeyboardCapture();

	r3d_assert(!isActive_);

	r3dMouse::Show();
	isActive_ = true;
    addClientSurvivor(gClientLogic().localPlayer_->CurLoadout);
    updateSurvivorTotalWeight();
    reloadBackpackInfo();

	{
		Scaleform::GFx::Value var[1];
		var[0].SetString("menu_open");
		gfxMovie.OnCommandCallback("eventSoundPlay", var, 1);
	}
}

/**********************************************************************/

void HUDRepair::eventReturnToGame(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{
	Deactivate();
}

void HUDRepair::eventRepairItemInSlot(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{
	obj_Player* plr = gClientLogic().localPlayer_;
	wiCharDataFull& slot = plr->CurLoadout;

	Scaleform::GFx::Value var[3];
	int SlotID = args[0].GetUInt();

	//int Durability = slot.Items[SlotID].Durability;

	/*if(Durability <= 99.9f)
	{
		int price = (int)((100-Durability) / 0.7f);
		if(gUserProfile.ProfileData.GameDollars >= price)
		{
			
			gUserProfile.ProfileData.GameDollars -= price;
			//slot.Items[SlotID].Durability = 100;
			updateSurvivorTotalWeight();
			reloadBackpackInfo();
			
			PKT_S2C_RepairWeapon_s n;
			n.slot = SlotID;
			n.price = gUserProfile.ProfileData.GameDollars;
			p2pSendToHost(plr, &n, sizeof(n));
		}else{
			var[0].SetStringW(gLangMngr.getString("InGameUI_ErrorMsg_NoMoney"));
			var[1].SetBoolean(true);
			var[2].SetStringW(gLangMngr.getString("InGameUI_ErrorMsg_NoMoney"));
			gfxMovie.Invoke("_root.api.showInfoMsg", var, 3);
		}
	}*/
}

void HUDRepair::eventRepairAll(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{

	obj_Player* plr = gClientLogic().localPlayer_;
	r3d_assert(plr);
	wiCharDataFull& slot = plr->CurLoadout;

	int totalCosts = 0;
	Scaleform::GFx::Value var[3];

	/*for (int a = 0; a < slot.BackpackSize; a++)
	{
		if (slot.Items[a].itemID != 0)
		{
			int Durability = slot.Items[a].Durability;
			if(Durability <= 99)
			{
				totalCosts += (int)floor((100-Durability) / 0.7f);
			}
		}
	}*/

	if(gUserProfile.ProfileData.GameDollars >= totalCosts){
		gUserProfile.ProfileData.GameDollars -= totalCosts;

		/*for (int a = 0; a < slot.BackpackSize; a++)
		{
			if (slot.Items[a].itemID != 0)
			{
				slot.Items[a].Durability = 100;
			}
		}*/
		updateSurvivorTotalWeight();
		reloadBackpackInfo();

		PKT_S2C_RepairALLWeapon_s n;
		n.price = gUserProfile.ProfileData.GameDollars;
		p2pSendToHost(plr, &n, sizeof(n));

	}else{
			var[0].SetStringW(gLangMngr.getString("InGameUI_ErrorMsg_NoMoney"));
			var[1].SetBoolean(true);
			var[2].SetStringW(gLangMngr.getString("InGameUI_ErrorMsg_NoMoney"));
			gfxMovie.Invoke("_root.api.showInfoMsg", var, 3);
			return;
	}
}

/***************************************************************************/

void HUDRepair::updateSurvivorTotalWeight()
{
	obj_Player* plr = gClientLogic().localPlayer_;
	r3d_assert(plr);

	float totalWeight = plr->CurLoadout.getTotalWeight();

	Scaleform::GFx::Value var[2];
	var[0].SetString(plr->CurLoadout.Gamertag);
	var[1].SetNumber(totalWeight);
	gfxMovie.Invoke("_root.api.updateClientSurvivorWeight", var, 2);

	var[0].SetInt(gUserProfile.ProfileData.GameDollars);
	gfxMovie.Invoke("_root.Main.setGD", var, 1);
}

void HUDRepair::reloadBackpackInfo()
{
	// reset backpack
	{
		gfxMovie.Invoke("_root.api.clearBackpack", "");
		gfxMovie.Invoke("_root.api.clearBackpacks", "");
	}
	// reset Hudrepair
	
	gfxMovie.Invoke("_root.api.hideRepairScreen", 0);

	int backpackSlotIDInc = 0;
	// add backpack content info
	{
		obj_Player* plr = gClientLogic().localPlayer_;
		r3d_assert(plr);
		wiCharDataFull& slot = plr->CurLoadout;

		Scaleform::GFx::Value var[7];
		for (int a = 0; a < slot.BackpackSize; a++)
		{
			if (slot.Items[a].itemID != 0 && slot.Items[a].itemID != 101397)
			{
				var[0].SetInt(a);
				var[1].SetUInt(0); //slot.Items[a].InventoryID); // not used for game
				var[2].SetUInt(slot.Items[a].itemID);
				var[3].SetInt(slot.Items[a].quantity);
				var[4].SetInt(slot.Items[a].Var1);
				var[5].SetInt(slot.Items[a].Var2);
				char tmpStr[128] = {0};
				getAdditionalDescForItem(slot.Items[a].itemID, slot.Items[a].Var1, slot.Items[a].Var2, tmpStr);
				var[6].SetString(tmpStr);
				gfxMovie.Invoke("_root.api.addBackpackItem", var, 7);

				//int Durability = slot.Items[a].Durability;
				/*if(Durability <= 99)
				{
					r3dOutToLog("## ItemID %i Durability %i\n",slot.Items[a].itemID,Durability);
					var[0].SetInt(a);
					var[1].SetUInt((int)Durability);
					var[2].SetUInt((int)((100-Durability) / 0.7f));
					gfxMovie.Invoke("_root.api.addBackpackItemRepairInfo", var, 3);
				}*/
			}
		}
	}

	gfxMovie.Invoke("_root.api.showRepairScreen", 0);
}