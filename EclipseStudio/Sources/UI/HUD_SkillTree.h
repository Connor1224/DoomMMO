#pragma once

#include "APIScaleformGfx.h"
#include "UIItemInventory.h"

#include "FrontEndShared.h"
#include "../GameCode/UserProfile.h"

#include "UIAsync.h"

class HUDSkillTree
{
	bool	isActive_;
	bool	isInit_;

public:
	HUDSkillTree();
	~HUDSkillTree();

	bool Init();
	bool Unload();

	bool IsInited() const { return isInit_; }

	void Update();
	void Draw();

	bool isActive() const { return isActive_; }
	void Activate();
	void Deactivate();

	void eventReturnToGame(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount);
	void eventLearnSkill(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount);
	void setSkillLearned(const char* learnedSkillEnumName);
	void setSkillLearned(const char* skillEnum, int level);

private:
	UIAsync<HUDSkillTree> async_;
	r3dScaleformMovie gfxMovie;
	r3dScaleformMovie* prevKeyboardCaptureMovie;

	void addSkillPoints();
	void showMessageBox();
	void showMessageBox(const char* message);
	void loadDefaultSkills(int maxskill);
	void refreshLearnedSkills();
	void sendData(char name[256], int y, bool isLearned);
};