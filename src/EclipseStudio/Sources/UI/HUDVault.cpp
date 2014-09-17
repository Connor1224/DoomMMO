#include "r3dPCH.h"
#include "r3d.h"

#include "HUDVault.h"

#include "FrontendShared.h"

HUDVault::HUDVault() :
isActive_(false),
isInit_(false),
prevKeyboardCaptureMovie_(NULL)
{
}

HUDVault::~HUDVault()
{
}

void HUDVault::eventReturnToGame(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{
	Deactivate();
}

void HUDVault::eventBackpackFromInventory(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{
	
}

void HUDVault::eventBackpackToInventory(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{

}

void HUDVault::eventBackpackGridSwap(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{

}

bool HUDVault::Init()
{
 	if(!gfxMovie_.Load("Data\\Menu\\WarZ_HUD_PostOffice.swf", false)) 
	{
 		return false;
	}

#define MAKE_CALLBACK(FUNC) new r3dScaleformMovie::TGFxEICallback<HUDVault>(this, &HUDVault::FUNC)
 	gfxMovie_.RegisterEventHandler("eventBackpackFromInventory", MAKE_CALLBACK(eventBackpackFromInventory));
	gfxMovie_.RegisterEventHandler("eventBackpackToInventory", MAKE_CALLBACK(eventBackpackToInventory));
	gfxMovie_.RegisterEventHandler("eventBackpackGridSwap", MAKE_CALLBACK(eventBackpackGridSwap));
	gfxMovie_.RegisterEventHandler("eventReturnToGame", MAKE_CALLBACK(eventReturnToGame));

	gfxMovie_.SetCurentRTViewport(Scaleform::GFx::Movie::SM_ExactFit);

	isActive_ = false;
	isInit_ = true;
	return true;
}

bool HUDVault::Unload()
{
 	gfxMovie_.Unload();
	isActive_ = false;
	isInit_ = false;
	return true;
}

void HUDVault::Update()
{

}

void HUDVault::Draw()
{
 	gfxMovie_.UpdateAndDraw();
}

void HUDVault::Deactivate()
{
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

void HUDVault::Activate()
{
	prevKeyboardCaptureMovie_ = gfxMovie_.SetKeyboardCapture(); // for mouse scroll events

	r3d_assert(!isActive_);
	r3dMouse::Show();
	isActive_ = true;

	gfxMovie_.Invoke("_root.api.showInventoryScreen", 0);

	Scaleform::GFx::Value var[1];
	var[0].SetString("menu_open");
	gfxMovie_.OnCommandCallback("eventSoundPlay", var, 1);
}