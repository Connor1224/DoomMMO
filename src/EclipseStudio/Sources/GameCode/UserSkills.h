#pragma once

class CUserSkills
{
  public:
	//
	// hardcoded vars used to generate skill Ids.
	// note: adjust setSkillNames when changing skill ids
	enum ESkillIDs
	{
		SkillName0 = 0,
		SkillName1,
		SkillName2,
		SkillName3,
		SkillName4,
		SkillName5,
		SkillName6,
		SkillName7,
		SkillName8,
	    SkillName9,
		SkillName10,
		SkillName11,
		SkillName12,
		SkillName13,
		SkillName14,
		SkillName15,
        SkillName16,
        SkillName17,
        SkillName18,
        SkillName19,
        SkillName20,
        SkillName21,
        SkillName22,
        SkillName23,
        SkillName24,
        SkillName25,
        SkillName26,
        SkillName27,
        SkillName28,
        SkillName29,
        SkillName30,
        SkillName31,
        SkillName32,
        SkillName33,
		SKILL_ID_END           = 34,
	};  
	const static char* SkillNames[SKILL_ID_END];
};      