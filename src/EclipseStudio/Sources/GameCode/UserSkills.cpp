#include "r3dPCH.h"
#include "r3d.h"
#include "shellapi.h"

#include "UserSkills.h"

const char* CUserSkills::SkillNames[CUserSkills::SKILL_ID_END] = {0};

static int setSkillNames()
{
	#define SET_NAME(xx) CUserSkills::SkillNames[CUserSkills::xx] = #xx
		SET_NAME(SkillName0);//		Increased health (1)
		SET_NAME(SkillName1);//		Increased Stamina (1)
		SET_NAME(SkillName2);//		Increases endurance, making items seem lighter (1)
		SET_NAME(SkillName3);//		Faster Stamina regeneration (1)
		SET_NAME(SkillName4);//		Increased Stamina (2)
		SET_NAME(SkillName5);//		Faster Stamina regeneration (2)
		SET_NAME(SkillName6);// 	Increases endurance, making items seem lighter (2)
		SET_NAME(SkillName7);//		Increased health (2)
		SET_NAME(SkillName8);//		Increased melee damage (1)
		SET_NAME(SkillName9);// 	Reduced weapon spread
		SET_NAME(SkillName10);//	Increased melee damage (2)
		SET_NAME(SkillName11);//	Lower weapon recoil
		SET_NAME(SkillName12);//	Hold breath longer / sniper (1)
		SET_NAME(SkillName13);//	Increased melee damage (3)
		SET_NAME(SkillName14);//	Hold breath longer / sniper	(2)
		SET_NAME(SkillName15);//	Coming Soon
		SET_NAME(SkillName16);//	Coming Soon
		SET_NAME(SkillName17);//	Coming Soon
		SET_NAME(SkillName18);//	Coming Soon
		SET_NAME(SkillName19);//	Hunger decreases slower (1)
		SET_NAME(SkillName20);//	Increased effectiveness of healing items (1)
		SET_NAME(SkillName21);//	Quieter footsteps
		SET_NAME(SkillName22);//	When out of food and water, health decreases slower (1)
		SET_NAME(SkillName23);//	Take less fall damage (1)
		SET_NAME(SkillName24);//	Hunger decreases slower (2)
		SET_NAME(SkillName25);//	Increased effectiveness of healing items (2)
		SET_NAME(SkillName26);//	When out of food and water, health decreases slower	(2)
		SET_NAME(SkillName27);//	Take less fall damage (2)
		SET_NAME(SkillName28);//	Increased effectiveness of healing items (3)
		SET_NAME(SkillName29);//	Take less damage overall
		SET_NAME(SkillName30);//	Lowers detection radius when near zombies
		SET_NAME(SkillName31);//	Coming Soon
		SET_NAME(SkillName32);//	Coming Soon
		SET_NAME(SkillName33);//	Coming Soon
	#undef SET_NAME
	return 1;
}

static int skillNamesInited = setSkillNames();
