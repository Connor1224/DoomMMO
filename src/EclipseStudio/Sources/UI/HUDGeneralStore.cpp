#include "r3dPCH.h"
#include "r3d.h"

#include "HUDGeneralStore.h"

#include "FrontendShared.h"
#include "multiplayer/ClientGameLogic.h"
#include "LangMngr.h"
#include "ObjectsCode/AI/AI_Player.H"

HUDGeneralStore::HUDGeneralStore() :
isActive_(false),
isInit_(false),
prevKeyboardCaptureMovie_(NULL)
{
}

HUDGeneralStore::~HUDGeneralStore()
{
}

void HUDGeneralStore::eventReturnToGame(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{
	Deactivate();
}

void HUDGeneralStore::eventBuyItem(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{
	const unsigned int storeBuyItemID = args[0].GetUInt();
	const int storeBuyPrice = args[1].GetInt();
	const int storeBuyPriceGD = args[2].GetInt();

	if(gUserProfile.ProfileData.GameDollars < storeBuyPriceGD || gUserProfile.ProfileData.GamePoints < storeBuyPrice)
	{
		Scaleform::GFx::Value var[2];
		var[0].SetStringW(gLangMngr.getString("NotEnougMoneyToBuyItem"));
		var[1].SetBoolean(true);
		gfxMovie_.Invoke("_root.api.showInfoMsg", var, 2);
		return;
	}

	const int buyIdx = StoreDetectBuyIdx(storeBuyPrice, storeBuyPriceGD);
	if(buyIdx == 0)
	{
		Scaleform::GFx::Value args[2];
		args[0].SetStringW(gLangMngr.getString("BuyItemFailNoIndex"));
		args[1].SetBoolean(true);
		gfxMovie_.Invoke("_root.api.showInfoMsg", args, 2);
		return;
	}

	Scaleform::GFx::Value var[2];
	var[0].SetStringW(gLangMngr.getString("OneMomentPlease"));
	var[1].SetBoolean(false);
	gfxMovie_.Invoke("_root.api.showInfoMsg", var, 2);

	PKT_C2S_MarketBuyItemReq_s n;
	n.itemId = storeBuyItemID;
	n.buyIdx = (BYTE)buyIdx;
	p2pSendToHost(gClientLogic().localPlayer_, &n, sizeof(n));
}

void HUDGeneralStore::OnBuyItemAnswer(BYTE result)
{
	if (result != 0)
	{
		Scaleform::GFx::Value args[2];
		args[0].SetStringW(gLangMngr.getString("BuyItemFail"));
		args[1].SetBoolean(true);
		gfxMovie_.Invoke("_root.api.showInfoMsg", args, 2);
	}
	else
	{
		market_.setCurrency();
		gfxMovie_.Invoke("_root.api.buyItemSuccessful", "");
		gfxMovie_.Invoke("_root.api.hideInfoMsg", "");
	}
}

bool HUDGeneralStore::Init()
{
 	if(!gfxMovie_.Load("Data\\Menu\\WarZ_HUD_GeneralStore.swf", false)) 
	{
 		return false;
	}

#define MAKE_CALLBACK(FUNC) new r3dScaleformMovie::TGFxEICallback<HUDGeneralStore>(this, &HUDGeneralStore::FUNC)
 	gfxMovie_.RegisterEventHandler("eventReturnToGame", MAKE_CALLBACK(eventReturnToGame));
	gfxMovie_.RegisterEventHandler("eventBuyItem", MAKE_CALLBACK(eventBuyItem));

	gfxMovie_.SetCurentRTViewport(Scaleform::GFx::Movie::SM_ExactFit);

	market_.initialize(&gfxMovie_);

	isActive_ = false;
	isInit_ = true;
	return true;
}

bool HUDGeneralStore::Unload()
{
 	gfxMovie_.Unload();
	isActive_ = false;
	isInit_ = false;
	return true;
}

void HUDGeneralStore::Update()
{
	market_.update();
}

void HUDGeneralStore::Draw()
{
 	gfxMovie_.UpdateAndDraw();
}

void HUDGeneralStore::Deactivate()
{
	if (market_.processing())
	{
		return;
	}

	Scaleform::GFx::Value var[1];
	var[0].SetString("menu_close");
	gfxMovie_.OnCommandCallback("eventSoundPlay", var, 1);

	if(prevKeyboardCaptureMovie_)
	{
		prevKeyboardCaptureMovie_->SetKeyboardCapture();
		prevKeyboardCaptureMovie_ = NULL;
	}

	if(!g_cursor_mode->GetInt())
	{
		r3dMouse::Hide();
	}

	isActive_ = false;
}

void HUDGeneralStore::Activate()
{
	prevKeyboardCaptureMovie_ = gfxMovie_.SetKeyboardCapture(); // for mouse scroll events

	r3d_assert(!isActive_);
	r3dMouse::Show();
	isActive_ = true;

	gfxMovie_.Invoke("_root.api.showMarketplace", 0);

	Scaleform::GFx::Value var[1];
	var[0].SetString("menu_open");
	gfxMovie_.OnCommandCallback("eventSoundPlay", var, 1);
	
}