#pragma once

#include "APIScaleformGfx.h"

class HUDVault
{
public:
	HUDVault();
	~HUDVault();

	bool Init();
	bool Unload();

	bool IsInited() const { return isInit_; }

	void Update();
	void Draw();

	bool isActive() const { return isActive_; }
	void Activate();
	void Deactivate();

	void eventReturnToGame(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount);
	void eventBackpackFromInventory(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount);
	void eventBackpackToInventory(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount);
	void eventBackpackGridSwap(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount);

private:
	r3dScaleformMovie gfxMovie_;
	r3dScaleformMovie* prevKeyboardCaptureMovie_;

	bool isActive_;
	bool isInit_;
};