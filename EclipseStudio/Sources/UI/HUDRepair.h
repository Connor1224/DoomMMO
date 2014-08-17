#pragma once

#include "..\GameCode\UserProfile.h"
#include "APIScaleformGfx.h"
#include "UIItemInventory.h"

class HUDRepair
{
private:
	r3dScaleformMovie* prevKeyboardCaptureMovie_;
	bool	isActive_;
	bool	isInit;
	r3dScaleformMovie gfxMovie;

public:
	HUDRepair();
	~HUDRepair();

	bool 	Init();
	bool 	Unload();

	bool	IsInited() const { return isInit; }

	void 	Update();
	void 	Draw();

	bool	isActive() const { return isActive_; }
	void	Activate();
	void	Deactivate();

	void	addClientSurvivor(const wiCharDataFull& slot);

	void	eventReturnToGame(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount);
	void	eventRepairAll(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount);
	void	eventRepairItemInSlot(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount);

	void	updateSurvivorTotalWeight();
	void	reloadBackpackInfo();

	UIItemInventory itemInventory_;
};
