#ifndef HUDDisplay_h
#define HUDDisplay_h

#include "r3d.h"
#include "APIScaleformGfx.h"
#include "../GameCode/UserProfile.h"
#include "../ObjectsCode/weapons/Weapon.h"

#define MAX_HUD_ACHIEVEMENT_QUEUE 8

class obj_Player;
class HUDDisplay
{
protected:
	bool Inited;

	bool chatVisible;
	bool isGroupLeaderActive;
	bool chatInputActive;
	float chatVisibleUntilTime;
	float lastChatMessageSent;
	int	currentChatChannel;

	int playersListVisible;

	float bloodAlpha;

	int writeNoteSavedSlotIDFrom;
	float timeoutForNotes; // stupid UI design :(
	float timeoutNoteReadAbuseReportedHideUI;

	bool RangeFinderUIVisible;

	int weaponInfoVisible;
	bool SafeZoneWarningVisible;
private:
	r3dScaleformMovie gfxMovie;
public:
	r3dScaleformMovie gfxHUD;
	r3dScaleformMovie gfxBloodStreak;
	r3dScaleformMovie gfxRangeFinder;

	void	eventChatMessage(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount);
	void	eventNoteWritePost(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount);
	void	eventNoteClosed(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount);
	void	eventNoteReportAbuse(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount);
	void	eventShowPlayerListContextMenu(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount);
	void	eventPlayerListAction(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount);

public:
	HUDDisplay();
	~HUDDisplay();

	bool 	Init();
	bool 	Unload();
	bool	currentinvite;

	int 	Update();
	int 	Draw();

	void	setBloodAlpha(float alpha);

	// HUD functions
	void	setVisibility(float percent); // [0,1]
	void	setHearing(float percent); // [0,1]
	void	setLifeParams(int food, int water, int health, int toxicity, int stamina);
	
	void	setWeaponInfo(int ammo, int clips, int firemode);
	void	showWeaponInfo(int state);
	void	setSlotInfo(int slotID, const char* name, int quantity, const char* icon);
	void	updateSlotInfo(int slotID, int quantity);
	void	showSlots(bool state);
	void	setActiveSlot(int slotID);
	void	setActivatedSlot(int slotID);
	void	showMessage(const wchar_t* text);

	void	showChat(bool showChat, bool force=false);
	void	showChatInput();
	void	addChatMessage(int tabIndex, const char* user, const char* text, uint32_t flags);
	bool	isChatInputActive() const { return chatInputActive || (r3dGetTime()-lastChatMessageSent)<0.25f || writeNoteSavedSlotIDFrom; }
	bool	isChatVisible() const { return chatVisible; }
	void	setChatTransparency(float alpha); //[0,1]
	void	setChatChannel(int index);
	void	setCarInfo(int var1 , int var2 , int var3 ,int var4, int var5 ,int var6, bool show);
	void	enableClanChannel();
	void	setFireMode(WeaponFiremodeEnum firemode); // 1, 3, 5
	void	showReloading(bool set);
	void	SetReloadingProgress(float progress); // progress - 0-100

	// player list fn
	void	clearPlayersList();
	void addPlayerToList(int num, const char* name, int Reputation, bool isLegend, bool isDev, bool isPunisher, bool isInvitePending, bool IsPremium, bool local);
	void	showPlayersList(int flag);
	int		isPlayersListVisible() const {return playersListVisible;}

	// notes
	bool	canShowWriteNote() const { return r3dGetTime() > timeoutForNotes; }
	void	showWriteNote(int slotIDFrom);
	void	StatusVehicle(const wchar_t* plr,const wchar_t* plr2);  // Server Vehicles
	void	VehicleWithoutGasoline();  // Server Vehicles
	void	VehicleDamaged();  // Server Vehicles
	void	showReadNote(const char* msg);
	void	showCarFull(const char* msg);

	void	showRangeFinderUI(bool set) { RangeFinderUIVisible = set; }

	void	showYouAreDead(const char* killedBy);

	void	showSafeZoneWarning(bool flag);

	void	addCharTag(const char* name, int Reputation, bool isSameClan, Scaleform::GFx::Value& result);

	void	moveUserIcon(Scaleform::GFx::Value& icon, const r3dPoint3D& pos, bool alwaysShow, bool force_invisible = false, bool pos_in_screen_space=false);
	void	setCharTagTextVisible1(Scaleform::GFx::Value& icon, bool isShowName, bool isSameGroup);
	void	setCharTagTextVisible(Scaleform::GFx::Value& icon, bool isShowName, bool isSameGroup);
	void	setCooldown(int slot,int CoolSecond,int value);
	void	removeUserIcon(Scaleform::GFx::Value& icon);
	void	removePlayerFromGroup(const char* gamertag,bool legend);
	void	aboutToLeavePlayerFromGroup(const char* gamertag);
	void	updateLeaderGroup(const char* gamertag);
	void	WantLeaveFromGroup();
	void	LeaveFromGroup();
	void	addPlayerToGroup(const char* gamertag,bool isLeader);
	void	enableGroupChannel();

	char GametagName[128];
	float TimeToWaitGroup;
	float TimeLeaveGroup;
};

#endif