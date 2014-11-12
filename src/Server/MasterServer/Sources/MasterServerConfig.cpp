#include "r3dPCH.h"
#include "r3d.h"

#include "MasterServerConfig.h"
#include "MasterGameServer.h"
#include "MasterUserServer.h"
#include "../EclipseStudio/Sources/backend/WOBackendAPI.h"

	CMasterServerConfig* gServerConfig = NULL;

static const char* configFile = "ServerProperties.cfg";

CMasterServerConfig::CMasterServerConfig()
{
  const char* group      = "MasterServer";

  if(_access(configFile, 0) != 0) {
    r3dError("can't open config file %s\n", configFile);
  }

  masterPort_  = r3dReadCFG_I(configFile, group, "masterPort", SBNET_MASTER_PORT);
  clientPort_  = r3dReadCFG_I(configFile, group, "clientPort", GBNET_CLIENT_PORT);
  masterCCU_   = r3dReadCFG_I(configFile, group, "masterCCU",  3000);
  idCount = 1;
  supervisorCoolDownSeconds_ = r3dReadCFG_F(configFile, group, "supervisorCoolDownSeconds",  15.0f);

  #define CHECK_I(xx) if(xx == 0)  r3dError("missing %s value in %s", #xx, configFile);
  #define CHECK_S(xx) if(xx == "") r3dError("missing %s value in %s", #xx, configFile);
  CHECK_I(masterPort_);
  CHECK_I(clientPort_);
  #undef CHECK_I
  #undef CHECK_S

  serverId_    = r3dReadCFG_I(configFile, group, "serverId", 0);
  if(serverId_ == 0)
  {
	MessageBox(NULL, "you must define serverId in MasterServer.cfg", "", MB_OK);
	r3dError("no serverId");
  }
  if(serverId_ > 255 || serverId_ < 1)
  {
	MessageBox(NULL, "bad serverId", "", MB_OK);
	r3dError("bad serverId");
  }
  
  LoadConfig();
  
  return;
}

void CMasterServerConfig::LoadConfig()
{
  r3dCloseCFG_Cur();
  
  numPermGames_ = 0;

  LoadPermGamesConfig();
  //Temp_Load_WarZGames();
}

void CMasterServerConfig::Temp_Load_WarZGames()
{
  char group[128];
  sprintf(group, "Doom-MMO Games");

  int numGames   = r3dReadCFG_I(configFile, group, "numGames", 0);
  int maxPlayers = r3dReadCFG_I(configFile, group, "maxPlayers", 32);
  int mapid		 = r3dReadCFG_I(configFile, group, "mapID", 1);
  int maptype    = r3dReadCFG_I(configFile, group, "gameType", 1);
  const char *workdir    = r3dReadCFG_S(configFile, group, "workDir", "WZ_COLORADO");
  std::string workstr = workdir;
  const char *mapname    = r3dReadCFG_S(configFile, group, "mapName", "Colorado");	
  
  r3dOutToLog("Doom-MMO %d games, %d players each\n", numGames, maxPlayers);
  
  for(int i=0; i<numGames; i++)
  {
    GBGameInfo ginfo;
    ginfo.mapId        = mapid;
	ginfo.gameType	 = maptype;
    ginfo.maxPlayers = maxPlayers;
	sprintf(ginfo.name, "US Server %03d", i + 1);
	r3dscpy(ginfo.workDir, workstr.c_str());
	r3dscpy(ginfo.mapName, mapname);
    sprintf(ginfo.name, "US Server %03d", i + 1);
    AddPermanentGame(10000 + i, ginfo, GBNET_REGION_US_West);

    /*sprintf(ginfo.name, "EU Server %03d", i + 1);
    AddPermanentGame(20000 + i, ginfo, GBNET_REGION_Europe);*/
  }
}

void CMasterServerConfig::LoadPermGamesConfig()
{
  numPermGames_ = 0;

//#ifdef _DEBUG
//  r3dOutToLog("Permanet games disabled in DEBUG");
//  return;
//#endif
  
  for(int i=0; i<250; i++)
  {
    char group[128];
    sprintf(group, "PermGame%d", i+1);

    char map[512] = "";    
    char data[512] = "";
    char pwdchar[512];
    char name[512];
    int ispwd;
    int ispre2;
    char ispwd1[512];
    char ispre1[512];
    r3dscpy(ispwd1,  r3dReadCFG_S(configFile, group, "ispwd", ""));
    r3dscpy(ispre1,  r3dReadCFG_S(configFile, group, "ispte", ""));
    r3dscpy(map,  r3dReadCFG_S(configFile, group, "map", ""));
    r3dscpy(data, r3dReadCFG_S(configFile, group, "data", ""));
    r3dscpy(name, r3dReadCFG_S(configFile, group, "name", ""));
    r3dscpy(pwdchar, r3dReadCFG_S(configFile, group, "pwd", ""));
    if(name[0] == 0)
      sprintf(name, "PermGame%d", i+1);

    if(*map == 0)
      continue;
    
    sscanf(ispwd1, "%d", &ispwd);
    sscanf(ispre1, "%d", &ispre2);
    bool ispre = false;
    if (ispre2 == 1)
    {
		ispre = true;
    }
    sscanf(ispwd1, "%d", &ispwd);
    bool ispass = false;
    if (ispwd == 1)
    {
		ispass = true;
    }
    bool isfarm = false;
    if (ispass && ispre)
    {
        ispass = false;
		ispre = false;
        isfarm = true;
    }

    ParsePermamentGame(i, name, map, data,pwdchar,ispass,ispre,isfarm);
  }

  return;  
}

static int StringToGBMapID(char* str)
{
 /* if(stricmp(str, "MAPID_WZ_Colorado") == 0)
    return GBGameInfo::MAPID_WZ_Colorado;

  if(stricmp(str, "MAPID_Editor_Particles") == 0)
    return GBGameInfo::MAPID_Editor_Particles;
  if(stricmp(str, "MAPID_ServerTest") == 0)
    return GBGameInfo::MAPID_ServerTest;
  if(stricmp(str, "MAPID_WZ_TestTown") == 0)
    return GBGameInfo::MAPID_WZ_TestTown;
  if(stricmp(str, "MAPID_WZ_Stronghold") == 0)
    return GBGameInfo::MAPID_WZ_StrongHold;
    
  r3dError("bad GBMapID %s\n", str);*/
  return 0;
}

static EGBGameRegion StringToGBRegion(const char* str)
{
  if(stricmp(str, "GBNET_REGION_US_West") == 0)
    return GBNET_REGION_US_West;
  if(stricmp(str, "GBNET_REGION_US_East") == 0)
    return GBNET_REGION_US_East;
  if(stricmp(str, "GBNET_REGION_Europe") == 0)
    return GBNET_REGION_Europe;
  if(stricmp(str, "GBNET_REGION_Russia") == 0)
    return GBNET_REGION_Russia;

  r3dError("bad GBGameRegion %s\n", str);
  return GBNET_REGION_Unknown;
}

void CMasterServerConfig::ParsePermamentGame(int gameServerId, const char* name, const char* map, const char* data, const char* pwdchar, bool ispass, bool ispre, bool isfarm)
{
	int mapid;
	int maptype;
	char workDir[128];
	char mapName[128];
	int minGames;
	int maxGames;

	if (6 != sscanf(map, "%d %d %s %s %d %d", &mapid, &maptype, workDir, mapName, &minGames, &maxGames)) {
		r3dError("bad map format: %s\n", map);
	}

	int maxPlayers;
	int minLevel = 0;
	int maxLevel = 0;

	if (3 != sscanf(data, "%d %d %d", &maxPlayers, &minLevel, &maxLevel)) {
		r3dError("bad data format: %s\n", data);
	}

	for (int i = 0; i < maxGames; i++)
	{
		GBGameInfo ginfo;
		ginfo.mapId = mapid;
		ginfo.gameType = maptype;
		ginfo.maxPlayers = maxPlayers;
		ginfo.ispass = ispass;
		ginfo.ispre = ispre;
		ginfo.isfarm = isfarm;

		char nameMsg[16];
		r3dscpy(nameMsg, name);
		char id[2];				//WARNING DO NOT MAKE MORE THAN 2 DIGITS WORTH OF SERVERS
		strcat(nameMsg, " ");
		strcat(nameMsg, itoa(i + 1, id, 10));

		r3dscpy(ginfo.name, nameMsg);
		r3dscpy(ginfo.workDir, workDir);
		r3dscpy(ginfo.mapName, mapName);
		r3dscpy(ginfo.pwdchar, pwdchar);

		r3dOutToLog("permgame: ID:%d, %s, %d, %s\n",
			idCount, name, mapid, pwdchar);

		EGBGameRegion eregion = GBNET_REGION_US_West;
		AddPermanentGame(idCount++, ginfo, eregion);
	}
}

void CMasterServerConfig::AddPermanentGame(int gameServerId, const GBGameInfo& ginfo, EGBGameRegion region)
{
	r3d_assert(numPermGames_ < R3D_ARRAYSIZE(permGames_));
	permGame_s& pg = permGames_[numPermGames_++];

	r3d_assert(gameServerId);
	pg.ginfo = ginfo;
	pg.ginfo.gameServerId = gameServerId;
	pg.ginfo.region = region;

	return;
}
