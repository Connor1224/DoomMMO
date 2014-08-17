#include "r3dPCH.h"
#include "r3d.h"

#include "r3dDebug.h"

#include "HUD_SkillTree.h"
#include "FrontendShared.h"

#include "../GameCode/SkillSystem.h"

#include "../ObjectsCode/ai/AI_Player.H"
#include "../ObjectsCode/weapons/Weapon.h"
#include "../ObjectsCode/weapons/WeaponArmory.h"

#include "../multiplayer/MasterServerLogic.h"
#include "../multiplayer/ClientGameLogic.h"

extern 	SkillSystem* gSkillSystem = new SkillSystem;

HUDSkillTree::HUDSkillTree() :
isActive_(false),
isInit_(false),
prevKeyboardCaptureMovie(NULL)
{
}

HUDSkillTree::~HUDSkillTree()
{
}

void HUDSkillTree::addSkillPoints()
{
	Scaleform::GFx::Value var[1];

	const wiCharDataFull& slot = gUserProfile.ProfileData.ArmorySlots[gUserProfile.SelectedCharID];
	r3dOutToLog("XP: %i\n", slot.Stats.XP);

	var[0].SetInt(slot.Stats.XP);
	gfxMovie.Invoke("_root.api.setSkillPoints", var, 1);
}

void HUDSkillTree::showMessageBox()
{
	Scaleform::GFx::Value var[2];
	var[0].SetStringW(gLangMngr.getString("OneMomentPlease"));
	var[1].SetBoolean(false);
	gfxMovie.Invoke("_root.api.showInfoMsg", var, 2);
}

void HUDSkillTree::showMessageBox(const char* message)
{
	Scaleform::GFx::Value var[2];
	var[0].SetString(message);
	var[1].SetBoolean(true);
	gfxMovie.Invoke("_root.api.showInfoMsg", var, 2);
}

void HUDSkillTree::setSkillLearned(const char* learnedSkillEnumName)
{
	if(!isInit_) return;

	Scaleform::GFx::Value var[1];
	var[0].SetString(learnedSkillEnumName);
	gfxMovie.Invoke("_root.api.setSkillLearned", var, 1);
}

void HUDSkillTree::setSkillLearned(const char* skillEnum, int level)
{
	if(!isInit_) return;
	
	char enumnev[256];
	sprintf(enumnev,"%s_%d", skillEnum, level);

	Scaleform::GFx::Value var[1];
	var[0].SetString(enumnev);
	gfxMovie.Invoke("_root.api.setSkillLearned", var, 1);

	r3dOutToLog("setSkillLearnedCheck: %s\n", enumnev);
}

void HUDSkillTree::eventReturnToGame(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{
	Deactivate();
}

void HUDSkillTree::eventLearnSkill(r3dScaleformMovie* pMovie, const Scaleform::GFx::Value* args, unsigned argCount)
{
	const char* learnedSkillEnumName = args[0].GetString();

	r3dOutToLog("LearnedSkillEnum: %s\n", learnedSkillEnumName);
	
	if (gSkillSystem->LearnSkillFromFlash(learnedSkillEnumName) == 0)
	{
		/*Scaleform::GFx::Value var[1];
		var[0].SetInt(Jelenlegi_SkillPontok - SkillPrice);
		gfxMovie.Invoke("_root.api.setSkillPoints", var, 1);*/

		setSkillLearned(learnedSkillEnumName);
	}
	else
	{
		showMessageBox("Something Wrong");
	}
}

void HUDSkillTree::sendData(char name[256], int y, bool isLearned)
{
	Scaleform::GFx::Value var[7];
	char skill[256];
	char skillbw[256];
	char skillname[256];
	char skilldesc[256];
	char enumnev[256];
	sprintf(skill, "$Data/Menu/skillIcons/%s_%d.png",name, y);
	sprintf(skillbw, "$Data/Menu/skillIcons/%s_%dBW.png",name, y);
	sprintf(skillname, "%s", name);
	sprintf(skilldesc, "%s", name);
	sprintf(enumnev,"%s_%d", name, y);
	var[0].SetString(enumnev);
	var[1].SetStringW(gLangMngr.getString(skillname));
	var[2].SetStringW(gLangMngr.getString(skilldesc));
	var[3].SetString(skill);
	var[4].SetString(skillbw);
	var[5].SetInt(100 + y * 10);
	int learned = isLearned ? 1:0;
	var[6].SetInt(learned);
	gfxMovie.Invoke("_root.api.addSkillInfo", var, 7);
}

void HUDSkillTree::refreshLearnedSkills()
{
	Scaleform::GFx::Value var[8];
	const wiCharDataFull& slot = gUserProfile.ProfileData.ArmorySlots[gUserProfile.SelectedCharID];

	int learnedSkilNum = sizeof(slot.Skills) / sizeof(int);

	for(int i = 0; i < learnedSkilNum; i++)
	{
		switch(slot.Skills[i].SkillID)
		{
		case SKILL_PLUS_HP: //+ 1 hp
			{
				for(int y = 1; y <= slot.Skills[i].Level; y++)
				{
					setSkillLearned("SKILL_PLUS_HP", y);
				}
				break;
			}
		case SKILL_PLUS_DMG: // +1 dmg
			{
				for(int y = 1; y <= slot.Skills[i].Level; y++)
				{
					setSkillLearned("SKILL_PLUS_DMG", y);
				}
				break;
			}
		case SKILL_PLUS_HEALING: // +1 healing effect
			{
				for(int y = 1; y <= slot.Skills[i].Level; y++)
				{
					setSkillLearned("SKILL_PLUS_HEALING", y);
				}			
				break;
			}
		case SKILL_MINUS_POISON: // -1 poison chance
			{
				for(int y = 1; y <= slot.Skills[i].Level; y++)
				{
					setSkillLearned("SKILL_MINUS_POISON", y);
				}
				break;
			}
		case SKILL_MINUS_BLEEDING: //-2 bleeding chance
			{
				for(int y = 1; y <= slot.Skills[i].Level; y++)
				{
					setSkillLearned("SKILL_MINUS_BLEEDING", y);
				}
				break;
			}
		case SKILL_PLUS_LOOTCHANCE: // +5 loot chance
			{
				for(int y = 1; y <= slot.Skills[i].Level; y++)
				{
					setSkillLearned("SKILL_PLUS_LOOTCHANCE", y);
				}
				break;
			}
		case SKILL_PLUS_DEF: // +2 def
			{
				for(int y = 1; y <= slot.Skills[i].Level; y++)
				{
					setSkillLearned("SKILL_PLUS_DEF", y);
				}
				break;
			}
		case SKILL_SURVIVOR: //+2 def, + 15 hp
			{
				setSkillLearned("SKILL_SURVIVOR", 1);
				break;
			}
		case SKILL_MINUS_BLEEDING_HIGH: // -6 bleeding chance
			{
				setSkillLearned("SKILL_MINUS_BLEEDING_HIGH", 1);
				break;
			}
		case SKILL_BERSERKING: // nem tudom meg
			{
				setSkillLearned("SKILL_BERSERKING", 1);
				break;
			}
		case SKILL_PLUS_STAMINA: // +5 stamina
			{
				for(int y = 1; y <= slot.Skills[i].Level; y++)
				{
					setSkillLearned("SKILL_PLUS_STAMINA", y);
				}
				break;
			}
		case SKILL_WHO_NEED_FOOD: // nem tudom meg
			{
				setSkillLearned("SKILL_WHO_NEED_FOOD", 1);
				break;
			}
		case SKILL_MINUS_MDMG: // -5 melee dmg
			{
				for(int y = 1; y <= slot.Skills[i].Level; y++)
				{
					setSkillLearned("SKILL_MINUS_MDMG", y);
				}
				break;
			}
		case SKILL_PLUS_HEALING_HIGH: // +4 healing 
			{
				setSkillLearned("SKILL_PLUS_HEALING_HIGH", 1);
				break;
			}
		case SKILL_PLUS_VACCINE: // +2 vaccine
			{
				for(int y = 1; y <= slot.Skills[i].Level; y++)
				{
					setSkillLearned("SKILL_PLUS_VACCINE", y);
				}
				break;
			}
		case SKILL_PLUS_HEALING_BIGGER: // +2 healing
			{
				for(int y = 1; y <= slot.Skills[i].Level; y++)
				{
					setSkillLearned("SKILL_PLUS_HEALING_BIGGER", y);
				}
				break;
			}
		case SKILL_MINUS_NOISE: // -20 noise
			{
				for(int y = 1; y <= slot.Skills[i].Level; y++)
				{
					setSkillLearned("SKILL_MINUS_NOISE", y);
				}
				break;
			}
		case SKILL_PLUS_MDMG: // +3 melee dmg
			{
				for(int y = 1; y <= slot.Skills[i].Level; y++)
				{
					setSkillLearned("SKILL_PLUS_MDMG", y);
				}
				break;
			}
		case SKILL_CALL_ME_DOCTOR: // ???
			{
				setSkillLearned("SKILL_CALL_ME_DOCTOR", 1);
				break;
			}
		case SKILL_POISON_WHAT: // ???
			{
				setSkillLearned("SKILL_POISON_WHAT", 1);
				break;
			}
		case SKILL_HARDER_HIT: // ???
			{
				setSkillLearned("SKILL_HARDER_HIT", 1);
				break;
			}
		}
	}
}

void HUDSkillTree::loadDefaultSkills(int maxskill)
{
	Scaleform::GFx::Value var[8];
	const wiCharDataFull& slot = gUserProfile.ProfileData.ArmorySlots[gUserProfile.SelectedCharID];

	for(int i = 0; i < maxskill; i++)
	{
		switch(slot.AllSkills[i].SkillID)
		{
			case SKILL_PLUS_HP: //+ 1 hp
				{
					for(int y = 1; y <= slot.AllSkills[i].Level; y++)
					{
						sendData("SKILL_PLUS_HP", y, false);
					}
					break;
				}
			case SKILL_PLUS_DMG: // +1 dmg
				{
					for(int y = 1; y <= slot.AllSkills[i].Level; y++)
					{
						sendData("SKILL_PLUS_DMG", y, false);
					}
					break;
				}
			case SKILL_PLUS_HEALING: // +1 healing effect
				{
					for(int y = 1; y <= slot.AllSkills[i].Level; y++)
					{
						sendData("SKILL_PLUS_HEALING", y, false);
					}			
					break;
				}
			case SKILL_MINUS_POISON: // -1 poison chance
				{
					for(int y = 1; y <= slot.AllSkills[i].Level; y++)
					{
						sendData("SKILL_MINUS_POISON", y, false);
					}
					break;
				}
			case SKILL_MINUS_BLEEDING: //-2 bleeding chance
				{
					for(int y = 1; y <= slot.AllSkills[i].Level; y++)
					{
						sendData("SKILL_MINUS_BLEEDING", y, false);
					}
					break;
				}
			case SKILL_PLUS_LOOTCHANCE: // +5 loot chance
				{
					for(int y = 1; y <= slot.AllSkills[i].Level; y++)
					{
						sendData("SKILL_PLUS_LOOTCHANCE", y, false);
					}
					break;
				}
			case SKILL_PLUS_DEF: // +2 def
				{
					for(int y = 1; y <= slot.AllSkills[i].Level; y++)
					{
						sendData("SKILL_PLUS_DEF", y, false);
					}
					break;
				}
			case SKILL_SURVIVOR: //+2 def, + 15 hp
				{
					sendData("SKILL_SURVIVOR", 1, false);
					break;
				}
			case SKILL_MINUS_BLEEDING_HIGH: // -6 bleeding chance
				{
					sendData("SKILL_MINUS_BLEEDING_HIGH", 1, false);
					break;
				}
			case SKILL_BERSERKING: // nem tudom meg
				{
					sendData("SKILL_BERSERKING", 1, false);
					break;
				}
			case SKILL_PLUS_STAMINA: // +5 stamina
				{
					for(int y = 1; y <= slot.AllSkills[i].Level; y++)
					{
						sendData("SKILL_PLUS_STAMINA", y, false);
					}
					break;
				}
			case SKILL_WHO_NEED_FOOD: // nem tudom meg
				{
					sendData("SKILL_WHO_NEED_FOOD", 1, false);
					break;
				}
			case SKILL_MINUS_MDMG: // -5 melee dmg
				{
					for(int y = 1; y <= slot.AllSkills[i].Level; y++)
					{
						sendData("SKILL_MINUS_MDMG", y, false);
					}
					break;
				}
			case SKILL_PLUS_HEALING_HIGH: // +4 healing 
				{
					sendData("SKILL_PLUS_HEALING_HIGH", 1, false);
					break;
				}
			case SKILL_PLUS_VACCINE: // +2 vaccine
				{
					for(int y = 1; y <= slot.AllSkills[i].Level; y++)
					{
						sendData("SKILL_PLUS_VACCINE", y, false);
					}
					break;
				}
			case SKILL_PLUS_HEALING_BIGGER: // +2 healing
				{
					for(int y = 1; y <= slot.AllSkills[i].Level; y++)
					{
						sendData("SKILL_PLUS_HEALING_BIGGER", y, false);
					}
					break;
				}
			case SKILL_MINUS_NOISE: // -20 noise
				{
					for(int y = 1; y <= slot.AllSkills[i].Level; y++)
					{
						sendData("SKILL_MINUS_NOISE", y, false);
					}
					break;
				}
			case SKILL_PLUS_MDMG: // +3 melee dmg
				{
					for(int y = 1; y <= slot.AllSkills[i].Level; y++)
					{
						sendData("SKILL_PLUS_MDMG", y, false);
					}
					break;
				}
			case SKILL_CALL_ME_DOCTOR: // ???
				{
					sendData("SKILL_CALL_ME_DOCTOR", 1, false);
					break;
				}
			case SKILL_POISON_WHAT: // ???
				{
					sendData("SKILL_POISON_WHAT", 1, false);
					break;
				}
			case SKILL_HARDER_HIT: // ???
				{
					sendData("SKILL_HARDER_HIT", 1, false);
					break;
				}
		}
	}
}

bool HUDSkillTree::Init()
{
	if(!gfxMovie.Load("Data\\Menu\\HUD_SkillTree.swf", false)) 
	{
		return false;
	}
/*************************************************EVENTEK HELYE!!!!!!!!!*************************************************/
#define MAKE_CALLBACK(FUNC) new r3dScaleformMovie::TGFxEICallback<HUDSkillTree>(this, &HUDSkillTree::FUNC)
	/*gfxMovie.RegisterEventHandler("eventBackpackFromInventory", MAKE_CALLBACK(eventBackpackFromInventory));
	gfxMovie.RegisterEventHandler("eventBackpackToInventory", MAKE_CALLBACK(eventBackpackToInventory));
	gfxMovie.RegisterEventHandler("eventBackpackGridSwap", MAKE_CALLBACK(eventBackpackGridSwap));*/
	gfxMovie.RegisterEventHandler("eventReturnToGame", MAKE_CALLBACK(eventReturnToGame));
	gfxMovie.RegisterEventHandler("eventLearnSkill", MAKE_CALLBACK(eventLearnSkill));

	gfxMovie.SetCurentRTViewport(Scaleform::GFx::Movie::SM_ExactFit);
	wiCharDataFull& slot = gUserProfile.ProfileData.ArmorySlots[gUserProfile.SelectedCharID];
	int asd = gSkillSystem->LoadAllSkills(slot);
	r3dOutToLog("max skill: %d\n", asd);

	loadDefaultSkills(asd);
	/*Scaleform::GFx::Value var[7];
	for(int i=0; i<42; ++i)
	{
		char skill[256];
		char skillbw[256];
		char skillname[256];
		char skilldesc[256];
		char enumNev[256];
		sprintf(skill, "$Data/Menu/skillIcons/Skill%d.dds", i);
		sprintf(skillbw, "$Data/Menu/skillIcons/Skill%dBW.dds", i);
		sprintf(skillname, "SkillName%d", i);
		sprintf(skilldesc, "SkillDesc%d", i);
		sprintf(enumNev, "enumNev%d", i);
		var[0].SetString(enumNev);
		var[1].SetStringW(gLangMngr.getString(skillname));
		var[2].SetStringW(gLangMngr.getString(skilldesc));
		var[3].SetString(skill);
		var[4].SetString(skillbw);
		var[5].SetInt(100 + i * 10);
		var[6].SetInt(0);	
		gfxMovie.Invoke("_root.api.addSkillInfo", var, 7);
	}*/

	isActive_ = false;
	isInit_ = true;
	return true;
}

void HUDSkillTree::Update()
{

}

bool HUDSkillTree::Unload()
{
	gfxMovie.Unload();
	isActive_ = false;
	isInit_ = false;
	return true;
}

void HUDSkillTree::Draw()
{
	gfxMovie.UpdateAndDraw();
}

void HUDSkillTree::Deactivate()
{
	Scaleform::GFx::Value var[1];
	var[0].SetString("menu_close");
	gfxMovie.OnCommandCallback("eventSoundPlay", var, 1);

	if(prevKeyboardCaptureMovie)
	{
		prevKeyboardCaptureMovie->SetKeyboardCapture();
		prevKeyboardCaptureMovie = NULL;
	}

	if(!g_cursor_mode->GetInt())
	{
		r3dMouse::Hide();
	}

	isActive_ = false;
}

void HUDSkillTree::Activate()
{
	prevKeyboardCaptureMovie = gfxMovie.SetKeyboardCapture(); // for mouse scroll events

	r3d_assert(!isActive_);
	r3dMouse::Show();
	isActive_ = true;
	
	addSkillPoints();
	refreshLearnedSkills();

	gfxMovie.Invoke("_root.api.showSkillTree", 0);

	Scaleform::GFx::Value var[1];
	var[0].SetString("menu_open");
	gfxMovie.OnCommandCallback("eventSoundPlay", var, 1);
}