#pragma once

#include "../ObjectsCode/ai/AI_Player.H"

enum SkillIds //HunZ - az enumokat szinkronban kell tartani a kliens oldali skillsystemmel!
{
	SKILL_PLUS_HP,
	SKILL_PLUS_DMG,
	SKILL_PLUS_HEALING, // +2 healing
	SKILL_MINUS_POISON,
	SKILL_MINUS_BLEEDING,
	SKILL_PLUS_LOOTCHANCE,
	SKILL_PLUS_DEF,
	SKILL_SURVIVOR,
	SKILL_MINUS_BLEEDING_HIGH,
	SKILL_BERSERKING,
	SKILL_PLUS_STAMINA,
	SKILL_WHO_NEED_FOOD,
	SKILL_MINUS_MDMG,
	SKILL_PLUS_HEALING_HIGH,
	SKILL_PLUS_VACCINE,
	SKILL_PLUS_HEALING_BIGGER,
	SKILL_MINUS_NOISE,
	SKILL_PLUS_MDMG,
	SKILL_CALL_ME_DOCTOR,
	SKILL_POISON_WHAT,
	SKILL_HARDER_HIT,
	SKILL_ID_END,
};

class SkillSystem
{
public:
	SkillSystem();
	~SkillSystem();

	int GetMaxLevelBySkillID(int skillID);
	int LoadAllSkills(wiCharDataFull& slot);
	int GetLevel(const char* enumnev);
	int GetSkillID(const char* enumnev);
	int LearnSkillFromFlash(const char* enumnev);

private:
	int MAX_SKILL_LEVEL;
};