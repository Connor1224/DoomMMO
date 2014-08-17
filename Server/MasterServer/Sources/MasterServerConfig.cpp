#include "r3dPCH.h"
#include "r3d.h"

#include "MasterServerConfig.h"
#include "MasterGameServer.h"
#include "MasterUserServer.h"
#include "../EclipseStudio/Sources/backend/WOBackendAPI.h"

CMasterServerConfig* gServerConfig = NULL;

extern	__int64 cfg_sessionId;

static const char* configFile = "MasterServer.cfg";

CMasterServerConfig::CMasterServerConfig()
{
  const char* group      = "MasterServer";

  if(_access(configFile, 0) != 0) {
    r3dError("can't open config file %s\n", configFile);
  }
  maxrent = r3dReadCFG_I(configFile, group, "MaxRent",0);
  masterPort_  = r3dReadCFG_I(configFile, group, "masterPort", SBNET_MASTER_PORT);
  clientPort_  = r3dReadCFG_I(configFile, group, "clientPort", GBNET_CLIENT_PORT);
  masterCCU_   = r3dReadCFG_I(configFile, group, "masterCCU",  3000);

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
  Temp_Load_WarZGames();
}

void CMasterServerConfig::Temp_Load_WarZGames()
{
  char group[128];
  sprintf(group, "WarZGames");

  int numGames   = r3dReadCFG_I(configFile, group, "numGames", 0);
  int maxPlayers = r3dReadCFG_I(configFile, group, "maxPlayers", 32);

  r3dOutToLog("WarZ %d games, %d players each\n", numGames, maxPlayers);

  for(int i=0; i<numGames; i++)
  {
    GBGameInfo ginfo;
    ginfo.mapId      = GBGameInfo::MAPID_WZ_Colorado;
    ginfo.maxPlayers = maxPlayers;

    sprintf(ginfo.name, "US Server %03d", i + 1);
    AddPermanentGame(10000 + i, ginfo, GBNET_REGION_US_West);

    sprintf(ginfo.name, "EU Server %03d", i + 1);
    AddPermanentGame(20000 + i, ginfo, GBNET_REGION_Europe);
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
    char name[512];
    char PasswordGame[512] ="";
	char MapSettings[512] ="";
	char Crosshair[512] ="";
	char enableSnipers[512] ="";

    r3dscpy(map,  r3dReadCFG_S(configFile, group, "map", ""));
    r3dscpy(data, r3dReadCFG_S(configFile, group, "data", ""));
    r3dscpy(name, r3dReadCFG_S(configFile, group, "name", ""));
	r3dscpy(Crosshair,  r3dReadCFG_S(configFile, group, "Crosshair", ""));
	r3dscpy(enableSnipers,  r3dReadCFG_S(configFile, group, "enableSnipers", ""));
    r3dscpy(PasswordGame, r3dReadCFG_S(configFile, group, "PasswordGame", ""));

	r3dscpy(MapSettings, r3dReadCFG_S(configFile, group, "MapSettings", ""));

	if (strcmp(Crosshair,"1") != 0 && strcmp(Crosshair,"0") != 0)
		strcpy(Crosshair,"1");

	if (strcmp(enableSnipers,"1") != 0 && strcmp(enableSnipers,"0") != 0)
		strcpy(enableSnipers,"1");

	if (strcmp(MapSettings,"6") == 0)
	{
		if (strcmp(PasswordGame,"") == 0)
				strcpy(MapSettings,"0");
	}
	else if (strcmp(MapSettings,"6") != 0)
	{
		if (strcmp(PasswordGame,"") != 0)
			strcpy(PasswordGame,"");
	}

    if(name[0] == 0)
      sprintf(name, "PermGame%d", i+1);

    if(*map == 0)
      continue;

    ParsePermamentGame(i, name, map, data, PasswordGame,MapSettings,Crosshair,enableSnipers);
  }

  return;
}

static int StringToGBMapID(char* str)
{
  if(stricmp(str, "MAPID_WZ_Colorado") == 0)
    return GBGameInfo::MAPID_WZ_Colorado;
  if(stricmp(str, "MAPID_WZ_PVE_Colorado") == 0)
    return GBGameInfo::MAPID_WZ_PVE_Colorado;
  if(stricmp(str, "MAPID_WZ_Cliffside") == 0)
    return GBGameInfo::MAPID_WZ_Cliffside;
  if(stricmp(str, "MAPID_CaliWood") == 0)
    return GBGameInfo::MAPID_CaliWood;
  if(stricmp(str, "MAPID_Devmap") == 0)
    return GBGameInfo::MAPID_Devmap;

  if(stricmp(str, "MAPID_Editor_Particles") == 0)
    return GBGameInfo::MAPID_Editor_Particles;
  if(stricmp(str, "MAPID_ServerTest") == 0)
    return GBGameInfo::MAPID_ServerTest;

  r3dError("bad GBMapID %s\n", str);
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

void CMasterServerConfig::ParsePermamentGame(int gameServerId, const char* name, const char* map, const char* data, const char* PasswordGame, const char* MapSettings, const char* Crosshair, const char* enableSnipers)
{
  char mapid[128];
  char maptype[128];
  char region[128];
  int minGames;
  int maxGames;
  if(5 != sscanf(map, "%s %s %s %d %d", mapid, maptype, region, &minGames, &maxGames)) {
    r3dError("bad map format: %s\n", map);
  }

  int maxPlayers;
  int minLevel = 0;
  int maxLevel = 0;
  bool EnCrosshair = (strcmp(Crosshair,"0") == 0)? false : true;
  bool SNP = (strcmp(enableSnipers,"0") == 0)? false : true;
  if(3 != sscanf(data, "%d %d %d", &maxPlayers, &minLevel, &maxLevel)) {
    r3dError("bad data format: %s\n", data);
  }

  GBGameInfo ginfo;
  ginfo.enableCrosshair = EnCrosshair;
  ginfo.enableSnipers = SNP;
  ginfo.mapId        = StringToGBMapID(mapid);
  ginfo.maxPlayers   = maxPlayers;
  ginfo.ownercustomerid = 0;
  ginfo.expirein = 0;

  r3dscpy(ginfo.PasswordGame,PasswordGame);
  r3dscpy(ginfo.MapSettings,MapSettings);
  r3dscpy(ginfo.name, name);

  r3dOutToLog("permgame: ID:%d, %s, %s - Password: %s\n",
    gameServerId, name, mapid,PasswordGame);

  EGBGameRegion eregion = StringToGBRegion(region);
  AddPermanentGame(gameServerId, ginfo, eregion);
}

void CMasterServerConfig::AddPermanentGame(int gameServerId, const GBGameInfo& ginfo, EGBGameRegion region)
{
  r3d_assert(numPermGames_ < R3D_ARRAYSIZE(permGames_));
  permGame_s& pg = permGames_[numPermGames_++];

  r3d_assert(gameServerId);
  pg.ginfo = ginfo;
  pg.ginfo.gameServerId = gameServerId;
  pg.ginfo.region       = region;

  return;
}

void CMasterServerConfig::DeleteRentServer(int gameServerId)
{
	if (!gameServerId)
		return;

	CWOBackendReq req("api_GetRentServers.aspx");
	req.AddParam("411", "1");

	if(!req.Issue())
	{
		r3dOutToLog("Read RentServers FAILED, code: %d\n", req.resultCode_);
		return;
	}

	pugi::xml_document xmlFile;
	req.ParseXML(xmlFile);
	pugi::xml_node xmlSafelock = xmlFile.child("Rent_Servers");

	while(!xmlSafelock.empty())
	{
		r3dPoint3D Position;
		char Password[512];
		char Name[512];
		char MapSettings[512];

        uint32_t gameServerId2 = xmlSafelock.attribute("gameServerId").as_uint();
	    r3dscpy(MapSettings,xmlSafelock.attribute("MapSettings").value());
	    uint32_t Region = xmlSafelock.attribute("Region").as_uint();
	    r3dscpy(Name, xmlSafelock.attribute("Name").value());
	    uint32_t MapID = xmlSafelock.attribute("MapID").as_uint();
	    r3dscpy(Password, xmlSafelock.attribute("Password").value());
	    uint32_t MaxPlayers = xmlSafelock.attribute("MaxPlayers").as_uint();
	    uint32_t Owner = xmlSafelock.attribute("Owner").as_uint();
	    uint32_t ExpireTime = xmlSafelock.attribute("ExpireTime").as_uint();
	    uint32_t RentalTime = xmlSafelock.attribute("RentalTime").as_uint();
		uint32_t CustomerID = xmlSafelock.attribute("CustomerID").as_uint();
		uint32_t SesionID = xmlSafelock.attribute("SesionID").as_uint();
			if (gameServerId == gameServerId2)
			{
				char* g_ServerApiKey = "bvx425698dg6GsnxwedszF";
				CWOBackendReq req("api_SrvRentGame.aspx");
				req.AddSessionInfo(CustomerID, SesionID);
				req.AddParam("skey1",  g_ServerApiKey);
				req.AddParam("StateSesion",2);
				req.AddParam("gameServerId",gameServerId);
				req.AddParam("MapSettings",0);
				req.AddParam("Region",0);
				req.AddParam("Name", "");
				req.AddParam("MapID",0);
				req.AddParam("Password", "");
				req.AddParam("MaxPlayers", 0);
				req.AddParam("Owner",0);
				req.AddParam("ExpireTime",0);
				req.AddParam("RentalTime", 0);
	
				// issue
				if(!req.Issue())
				{
					r3dOutToLog("!!!! RentServer failed, code: %d\n", req.resultCode_);
				}
				r3dOutToLog("Expired server ID %i\n",gameServerId);
				break;
			}
		xmlSafelock = xmlSafelock.next_sibling();
	}

}
void CMasterServerConfig::AddRentGame(int customerid , const char* name , const char* pwd , int mapid , int slot , int RentalTime, int Region, int SesionID, bool EnableCrosshair,bool enableSnipers)
{
	static std::vector<const CServerS*> supers;
	supers.clear();
	for(CMasterGameServer::TSupersList::const_iterator it = gMasterGameServer.supers_.begin(); it != gMasterGameServer.supers_.end(); ++it)
		supers.push_back(it->second);

	const CServerS* super = supers[0];

	__int64 secs1 = _time64(&secs1);
	GBGameInfo ginfo;
	ginfo.mapId = mapid;
	switch(Region)
	{
	case 1 : 
			ginfo.region = GBNET_REGION_US_West;
			break;
	case 10:
			ginfo.region = GBNET_REGION_Europe;
			break;
	case 2:
			ginfo.region = GBNET_REGION_US_East;
			break;
	}
	ginfo.enableCrosshair=EnableCrosshair;
	ginfo.enableSnipers=enableSnipers;
	ginfo.maxPlayers = slot;
	strcpy(ginfo.MapSettings,"6");
	
	
	ginfo.gameServerId = (int)u_GetRandom(300,999);

	if(gMasterGameServer.IsGameServerIdStarted(ginfo.gameServerId))
	{
		r3dOutToLog("RentServer id %d already started, reseting..\n");
		AddRentGame(customerid,name,pwd,mapid,slot,RentalTime,Region,SesionID,EnableCrosshair,enableSnipers);
		return;
	}
	ginfo.ownercustomerid = customerid; // !!!!!!! MUST SET CUSTOMERID FOR MY SERVER SCREEN !!!!!!!!!!
	r3dscpy(ginfo.name,name);
	r3dscpy(ginfo.PasswordGame,pwd);
	switch(RentalTime)
	{
	case 0: // 1 DAY
		    ginfo.expirein = 86400;
			break;
	case 1: // 1 MOUNT
		    ginfo.expirein = 2592000;
			break;
	case 2: // 2 MOUNTS
		    ginfo.expirein = 5184000;
			break;
	case 3: // 3 MOUNTS
		    ginfo.expirein = 7776000;
			break;
	case 4: // 6 MOUNTS
		    ginfo.expirein = 15552000;
			break;
	}
	ginfo.expirein = (int)secs1 + ginfo.expirein+120;

	char* g_ServerApiKey = "bvx425698dg6GsnxwedszF";
	CWOBackendReq req("api_SrvRentGame.aspx");
	req.AddSessionInfo(customerid, SesionID);
	req.AddParam("skey1",  g_ServerApiKey);
	req.AddParam("StateSesion",1);
	req.AddParam("gameServerId",ginfo.gameServerId);
	req.AddParam("MapSettings",6);
	req.AddParam("Region",ginfo.region);
	req.AddParam("Name", name);
	req.AddParam("MapID",ginfo.mapId);
	req.AddParam("Password", pwd);
	req.AddParam("MaxPlayers", slot);
	req.AddParam("Owner",customerid);
	req.AddParam("ExpireTime",ginfo.expirein);
	req.AddParam("RentalTime", RentalTime);
	int Cross = (ginfo.enableCrosshair)?true:false;
	req.AddParam("Crosshair", Cross);
	int SNP = (enableSnipers)?true:false;
	req.AddParam("SNP", SNP);
	
	// issue
	if(!req.Issue())
	{
		r3dOutToLog("!!!! RentServer failed, code: %d\n", req.resultCode_);
		//return req.resultCode_;
		return;
	}

	CMSNewGameData ngd(ginfo, "", 0);
	DWORD ip;
	DWORD port;
	__int64 sessionId;
	r3dOutToLog("RentServer : Starting..\n");
	gMasterGameServer.CreateNewGame(ngd, &ip, &port, &sessionId);
}

void CMasterServerConfig::expired(int gameServerId)
{
	//DeleteRentServer(gameServerId);
}
void CMasterServerConfig::SetRentPwd(int gameServerId, const char* pwd, int customerid, int SesionID, bool EnableCrosshair, bool enableSnipers)
{
	char* g_ServerApiKey = "bvx425698dg6GsnxwedszF";
	CWOBackendReq req("api_SrvRentGame.aspx");
	req.AddSessionInfo(customerid, SesionID);
	req.AddParam("skey1",  g_ServerApiKey);
	req.AddParam("StateSesion",0);
	req.AddParam("gameServerId",gameServerId);
	req.AddParam("MapSettings",6);
	req.AddParam("Region",0);
	req.AddParam("Name", "");
	req.AddParam("MapID",0);
	req.AddParam("Password", pwd);
	req.AddParam("MaxPlayers", 0);
	req.AddParam("Owner",0);
	req.AddParam("ExpireTime",0);
	req.AddParam("RentalTime", 0);
	int Cross = (EnableCrosshair)?true:false;
	req.AddParam("Crosshair", Cross);
	int SNP = (enableSnipers)?true:false;
	req.AddParam("SNP", SNP);
	 
	// issue
	if(!req.Issue())
	{
		r3dOutToLog("!!!! Rent Set password failed , code: %d\n", req.resultCode_);
		//return req.resultCode_;
		return;
	}

for(CMasterGameServer::TSupersList::const_iterator it = gMasterGameServer.supers_.begin();
		it != gMasterGameServer.supers_.end();
		++it)
	{
		const CServerS* super = it->second;
		for(int i=0; i<super->maxGames_; i++) 
		{
			CServerG* game = super->games_[i].game;
			if(!game) 
				continue;
			// find server and send req to server
			if (game->info_.ginfo.gameServerId == gameServerId)
			{
				strcpy(game->info_.ginfo.PasswordGame,pwd);
				game->info_.ginfo.enableCrosshair = EnableCrosshair;
				game->info_.ginfo.enableSnipers = enableSnipers;
				r3dOutToLog("RentServer : The New password of %i is saved.\nRestarting Server on 60 Seconds....",gameServerId);
				//gMasterGameServer.SendKillGame(game->info_.ginfo.gameServerId);
				return;
			}
		}
	}

}

void CMasterServerConfig::LoadRentConfig()
{

	CWOBackendReq req("api_GetRentServers.aspx");
	req.AddParam("411", "1");

	if(!req.Issue())
	{
		r3dOutToLog("Read RentServers FAILED, code: %d\n", req.resultCode_);
		return;
	}

	pugi::xml_document xmlFile;
	req.ParseXML(xmlFile);
	pugi::xml_node xmlSafelock = xmlFile.child("Rent_Servers");
	
	while(!xmlSafelock.empty())
	{
		r3dPoint3D Position;
		char Password[512];
		char Name[512];
		char MapSettings[512];
		
        uint32_t gameServerId = xmlSafelock.attribute("gameServerId").as_uint();
	    r3dscpy(MapSettings,xmlSafelock.attribute("MapSettings").value());
	    uint32_t Region = xmlSafelock.attribute("Region").as_uint();
	    r3dscpy(Name, xmlSafelock.attribute("Name").value());
	    uint32_t MapID = xmlSafelock.attribute("MapID").as_uint();
	    r3dscpy(Password, xmlSafelock.attribute("Password").value());
	    uint32_t MaxPlayers = xmlSafelock.attribute("MaxPlayers").as_uint();
	    uint32_t Owner = xmlSafelock.attribute("Owner").as_uint();
	    uint32_t ExpireTime = xmlSafelock.attribute("ExpireTime").as_uint();
	    uint32_t RentalTime = xmlSafelock.attribute("RentalTime").as_uint();
		uint32_t Crosshair = xmlSafelock.attribute("Crosshair").as_uint();
		uint32_t SNP = xmlSafelock.attribute("SNP").as_uint();

		if(!gMasterGameServer.IsGameServerIdStarted((int)gameServerId))
		{
			r3dOutToLog("Loading Rent Server %i.......\n",gameServerId);
			__int64 secs1 = _time64(&secs1);
			if (secs1<ExpireTime)
			{
				GBGameInfo ginfo;
				ginfo.expirein = ExpireTime;
				strcpy(ginfo.name,Name);
				strcpy(ginfo.PasswordGame,Password);
				ginfo.mapId = MapID;
				ginfo.region = Region;
				ginfo.maxPlayers = MaxPlayers;
				ginfo.ownercustomerid = Owner;
				strcpy(ginfo.MapSettings,MapSettings);
				ginfo.gameServerId = gameServerId;
				ginfo.enableCrosshair = ((int)Crosshair == 1)?true:false;
				ginfo.enableSnipers = ((int)SNP == 1)?true:false;

				CMSNewGameData ngd(ginfo, "", 0);
				DWORD ip;
				DWORD port;
				__int64 sessionId;
				gMasterGameServer.CreateNewGame(ngd, &ip, &port, &sessionId);
				r3dOutToLog("Rent Server ID %i is loaded\n",gameServerId);
			}
			/*else {
				DeleteRentServer(gameServerId);
			}*/
		}
		xmlSafelock = xmlSafelock.next_sibling();
	}
}