#include "r3dPCH.h"
#include "r3d.h"
#include "shellapi.h"

#include "SkillSystem.h"

#include "multiplayer/ClientGameLogic.h"

SkillSystem::SkillSystem()
{
	MAX_SKILL_LEVEL = 9;
}

SkillSystem::~SkillSystem()
{

}
int SkillSystem::GetLevel(const char* enumnev)
{
	int level = 0;

	if(strstr(enumnev, "1"))
	{
		level = 1;
		return level;
	}
	if(strstr(enumnev, "2"))
	{
		level = 2;
		return level;
	}
	if(strstr(enumnev, "3"))
	{
		level = 3;
		return level;
	}
	if(strstr(enumnev, "4"))
	{
		level = 4;
		return level;
	}
	if(strstr(enumnev, "5"))
	{
		level = 5;
		return level;
	}
	if(strstr(enumnev, "6"))
	{
		level = 6;
		return level;
	}
	if(strstr(enumnev, "7"))
	{
		level = 7;
		return level;
	}	
	if(strstr(enumnev, "8"))
	{
		level = 8;
		return level;
	}
	if(strstr(enumnev, "9"))
	{
		level = 9;
		return level;
	}

	return level;
}

int SkillSystem::GetSkillID(const char* enumnev)
{
	int skillID = -1;
	for (int i = 0; i < MAX_SKILL_LEVEL; i++)
	{
		char pattern[10] = "_";
		sprintf_s(pattern, "%d", i);
		if (strstr(enumnev, pattern))
		{
			char enumCpy[128];
			int patternLength = (int) strlen(pattern);
			int enumLength = (int) strlen(enumnev);
			int copyLength = enumLength - (patternLength + 1);

			strncpy ( enumCpy, enumnev, copyLength);
			enumCpy[copyLength] = '\0';

			r3dOutToLog("Namiez: %s\n", enumCpy);

			if(strstr(enumCpy, "SKILL_PLUS_HP"))
			{
				skillID = 0;
			}
			else if(strstr(enumCpy, "SKILL_PLUS_DMG"))
			{
				skillID = 1;
			}
			else if(strstr(enumCpy, "SKILL_PLUS_HEALING"))
			{
				skillID = 2;
			}
			else if(strstr(enumCpy, "SKILL_MINUS_POISON"))
			{
				skillID = 3;
			}
			else if(strstr(enumCpy, "SKILL_MINUS_BLEEDING"))
			{
				skillID = 4;
			}
			else if(strstr(enumCpy, "SKILL_PLUS_LOOTCHANCE"))
			{
				skillID = 5;
			}
			else if(strstr(enumCpy, "SKILL_PLUS_DEF"))
			{
				skillID = 6;
			}
			else if(strstr(enumCpy, "SKILL_SURVIVOR"))
			{
				skillID = 7;
			}
			else if(strstr(enumCpy, "SKILL_MINUS_BLEEDING_HIGH"))
			{
				skillID = 8;
			}
			else if(strstr(enumCpy, "SKILL_BERSERKING"))
			{
				skillID = 9;
			}
			else if(strstr(enumCpy, "SKILL_PLUS_STAMINA"))
			{
				skillID = 10;
			}
			else if(strstr(enumCpy, "SKILL_WHO_NEED_FOOD"))
			{
				skillID = 11;
			}
			else if(strstr(enumCpy, "SKILL_MINUS_MDMG"))
			{
				skillID = 12;
			}
			else if(strstr(enumCpy, "SKILL_PLUS_HEALING_HIGH"))
			{
				skillID = 13;
			}
			else if(strstr(enumCpy, "SKILL_PLUS_VACCINE"))
			{
				skillID = 14;
			}
			else if(strstr(enumCpy, "SKILL_PLUS_HEALING_BIGGER"))
			{
				skillID = 15;
			}
			else if(strstr(enumCpy, "SKILL_MINUS_NOISE"))
			{
				skillID = 16;
			}
			else if(strstr(enumCpy, "SKILL_PLUS_MDMG"))
			{
				skillID = 17;
			}
			else if(strstr(enumCpy, "SKILL_CALL_ME_DOCTOR"))
			{
				skillID = 18;
			}
			else if(strstr(enumCpy, "SKILL_POISON_WHAT"))
			{
				skillID = 19;
			}
			else if(strstr(enumCpy, "SKILL_HARDER_HIT"))
			{
				skillID = 20;
			}
			else
			{
				skillID = -1;
			}
		}
	}

	return skillID;
}


int SkillSystem::LearnSkillFromFlash(const char* enumnev)
{
	obj_Player* plr = gClientLogic().localPlayer_;
	
	if (GetSkillID(enumnev) != -1)
	{
		PKT_S2C_SendSkillToClient_s n;
		n.SkillToSend.SkillID = GetSkillID(enumnev);
		n.SkillToSend.Level = GetLevel(enumnev);
		p2pSendToHost(plr, &n, sizeof(n));
		r3dOutToLog("Send SkillID: %d, SkillLevel: %d \n", GetSkillID(enumnev), GetLevel(enumnev));
		return 0;
	}
	else
	{
		return 1;
	}
	r3dOutToLog("Na itt a majom == SkillID: %d, SkillLevel: %d \n", GetSkillID(enumnev), GetLevel(enumnev));

	return 1;
}

int SkillSystem::LoadAllSkills(wiCharDataFull& slot)
{
	int fuckyoubazdmegtitov = 0;
	for (int i = 0; i < SKILL_ID_END; i++)
	{
		slot.AllSkills[i].SkillID = i;
		slot.AllSkills[i].Level = GetMaxLevelBySkillID(i);
		r3dOutToLog("allskill: id:%d , maxlevel: %d\n", slot.AllSkills[i].SkillID, slot.AllSkills[i].Level);
		fuckyoubazdmegtitov = i;
	}
	return fuckyoubazdmegtitov;
}

int SkillSystem::GetMaxLevelBySkillID(int skillID)
{
	int ret = 0;

	switch(skillID)
	{
		case SKILL_PLUS_HP: //+ 1 hp
			{
				ret = 8;
				return ret;
			}

		case SKILL_PLUS_DMG: // +1 dmg
			{
				ret = 4;
				return ret;
			}

		case SKILL_PLUS_HEALING: // +1 healing effect
			{
				ret = 3;
				return ret;
			}

		case SKILL_MINUS_POISON: // -1 poison chance
			{
				ret = 9;
				return ret;
			}

		case SKILL_MINUS_BLEEDING: //-2 bleeding chance
			{
				ret =  2;
				return ret;
			}

		case SKILL_PLUS_LOOTCHANCE: // +5 loot chance
			{
				ret = 3;
				return ret;
			}
		case SKILL_PLUS_DEF: // +2 def
			{
				ret = 4;
				return ret;
			}

		case SKILL_SURVIVOR: //+2 def, + 15 hp
			{
				ret = 1;
				return ret;
			}

		case SKILL_MINUS_BLEEDING_HIGH: // -6 bleeding chance
			{
				ret = 1;
				return ret;
			}

		case SKILL_BERSERKING: // nem tudom meg
			{
				ret = 1;
				return ret;
			}

		case SKILL_PLUS_STAMINA: // +5 stamina
			{
				ret = 3;
				return ret;
			}

		case SKILL_WHO_NEED_FOOD: // nem tudom meg
			{
				ret = 1;
				return ret;
			}

		case SKILL_MINUS_MDMG: // -5 melee dmg
			{
				ret = 3;
				return ret;
			}

		case SKILL_PLUS_HEALING_HIGH: // +4 healing 
			{
				ret = 1;
				return ret;
			}

		case SKILL_PLUS_VACCINE: // +2 vaccine
			{
				ret = 6;
				return ret;
			}

		case SKILL_PLUS_HEALING_BIGGER: // +2 healing
			{
				ret = 6; // ez amugy het, csak elirtam a kepen.. :O 
				return ret;
			}

		case SKILL_MINUS_NOISE: // -20 noise //itt tartok
			{
				ret = 1;
				return ret;
			}

		case SKILL_PLUS_MDMG: // +3 melee dmg
			{
				ret = 3;
				return ret;
			}

		case SKILL_CALL_ME_DOCTOR: // ???
			{
				ret = 1;
				return ret;
			}

		case SKILL_POISON_WHAT: // ???
			{
				ret = 1;
				return ret;
			}

		case SKILL_HARDER_HIT: // ???
			{
				ret = 1;
				return ret;
			}
	}
	return ret;
}


