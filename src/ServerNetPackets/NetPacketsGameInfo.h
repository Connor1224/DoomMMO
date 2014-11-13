#pragma once

#pragma pack(push)
#pragma pack(1)

#define SBNET_MASTER_PORT	34000	// default port for master server
#define GBNET_CLIENT_PORT	34001	// default port for game browser master server (client requests)
#define SBNET_MASTER_WATCH_PORT	34005	// watch port for master server
#define SBNET_SUPER_WATCH_PORT	34006	// watch port for supervisor server
#define SBNET_GAME_PORT		34010

enum EGBGameRegion
{
	GBNET_REGION_Unknown   = 0,
	GBNET_REGION_US_West   = 1,
	GBNET_REGION_US_East   = 2,
	GBNET_REGION_Europe    = 10,
	GBNET_REGION_Europe1 = 11,
    GBNET_REGION_Europe2 = 12,
	GBNET_REGION_Russia    = 20,
	GBNET_REGION_184 = 21
};

// MAKE SURE to increase GBGAMEINFO_VERSION after changing following structs
#define GBGAMEINFO_VERSION 0x00000003

struct GBGameInfo
{
	enum EMapId
	{
	  MAPID_Editor_Particles = 0,
	  MAPID_ServerTest,
	  MAPID_WZ_Colorado,
	  MAPID_Laskar,
	   MAPID_WZ_Cliffside_Farm,
	   MAPID_WZ_Cliffside_PVP,
	    MAPID_WZ_RaccoonCity,
		  MAPID_WZ_RaccoonCity_Premium,
		MAPID_WZ_CARCANFLY,
		MAPID_wo_wasteland,
		MAPID_wo_inferno,
		MAPID_wo_eastern_bunker_tdm,
		MAPID_WO_Grozny,
		MAPID_wo_shippingyard,
	  // NOTE: do *NOT* add maps inside current IDs, add ONLY at the end
	  // otherwise current map statistics in DB will be broken
	  MAPID_MAX_ID,
	};

	char	name[16];
	char    pwdchar[512];
	bool ispass;
	bool isfarm;
	bool ispre;
	int ownercustomerid;
	int expirein;
	bool isexited;
	BYTE	mapId;
	BYTE	maxPlayers;
	BYTE	flags;		// some game flags
	DWORD	gameServerId;	// unique server ID in our DB
	BYTE	region;		// game region
	//char   IsPassword[16];
	
	GBGameInfo()
	{
	  sprintf(name, "g%08X", this);
	  mapId = 0xFF;
	  maxPlayers = 0;
	  flags = 0;
	  gameServerId = 1;
	  region = GBNET_REGION_US_West;
	}
	
	bool IsValid() const
	{
	  if(mapId == 0xFF) return false;
	  if(maxPlayers == 0) return false;
	  if(gameServerId == 0) return false;
	   //if(ispassword == 0) return false;
	  return true;
	}
	
	bool FromString(const char* arg) 
	{
	  int v[14];
	  int args = sscanf(arg, "%d %d %d %d %d %d %d", 
	    &v[0], &v[1], &v[2], &v[3], &v[4], &v[5] , &v[6]);
	  if(args != 7) return false;
	  
	  bool pre = false;
	  bool pass = false;
	  if ((DWORD)v[0] == 1)
      pre = true;

	  if ((DWORD)v[6] == 1)
	  pass = true;

	  ispre = pre;
	  ispass = pass;
	  mapId         = (BYTE)v[1];
	  maxPlayers    = (BYTE)v[2];
	  flags         = (BYTE)v[3];
	  gameServerId  = (DWORD)v[4];
	  region        = (BYTE)v[5];
	  return true;
	}
	
	void ToString(char* arg) const
	{
	  sprintf(arg, "%d %d %d %d %d", 
	    mapId,
	    maxPlayers,
	    flags,
	    gameServerId,
	    region);
	}
};

#pragma pack(pop)