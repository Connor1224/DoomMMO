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
	GBNET_REGION_Unknown,
	GBNET_REGION_US_West,
	GBNET_REGION_US_East,
	GBNET_REGION_Europe,
	GBNET_REGION_Russia,
	GBNET_REGION_Brazil,
	GBNET_REGION_Custom,
};

// MAKE SURE to increase GBGAMEINFO_VERSION after changing following structs
#define GBGAMEINFO_VERSION 0x00000003

struct GBGameInfo
{

	char	name[16];
	char	mapName[16];
	char    pwdchar[512];     
	bool	ispass;     
	bool	isfarm;     
	bool	ispre; 
	BYTE	mapId;
	BYTE	maxPlayers;
	BYTE	flags;		// some game flags
	DWORD	gameServerId;	// unique server ID in our DB
	BYTE	region;		// game region
	char	workDir[24]; //work directory
	BYTE	gameType;    // 1 - GAMEWORLD, 2 - STRONGHOLD.
	
	GBGameInfo()
	{
	  sprintf(name, "g%08X", this);
	  sprintf(mapName, "NULL");
	  mapId = 0xFF;
	  maxPlayers = 0;
	  flags = 0;
	  gameServerId = 0;
	  region = GBNET_REGION_Unknown;
	  gameType = 0;
	  sprintf(workDir, "NULL");
	}
	
	bool IsValid() const
	{
	  if(mapId == 0xFF) return false;
	  if(maxPlayers == 0) return false;
	  if(gameServerId == 0) return false;
	  if(strcmp(workDir, "NULL") == 0) return false;
	  if(gameType == 0) return false;
	  if(strcmp(mapName, "NULL") == 0) return false;
	  return true;
	}
	
	bool FromString(const char* arg) 
	{
	  int v[64];
	  char mapname[24]; 
	  char workdir[24];
	  int args = sscanf(arg, "%d %d %d %d %d %d", 
	    &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &workdir[0], &mapname[0]);
	  if(args != 5) return false;

	  mapId         = (BYTE)v[0];
	  maxPlayers    = (BYTE)v[1];
	  flags         = (BYTE)v[2];
	  gameServerId  = (DWORD)v[3];
	  region        = (BYTE)v[4];
	  gameType		= (BYTE)v[5];
	  r3dscpy(workDir, &workdir[0]);
	  r3dscpy(mapName, &mapname[0]);
	  return true;
	}
	
	void ToString(char* arg) const
	{
	  sprintf(arg, "%d %d %d %d %d %s %s", 
	    mapId,
	    maxPlayers,
	    flags,
	    gameServerId,
	    region, gameType, workDir, mapName );
	}
};

#pragma pack(pop)