#pragma once

#include "../../ServerNetPackets/NetPacketsGameInfo.h"

class CMasterServerConfig
{
  public:
	int		masterPort_;
	int		clientPort_;
	int		serverId_;
	int		masterCCU_;	// max number of connected peers
	
	int maxrent;
	float supervisorCoolDownSeconds_;

	//
	// permanent games groups
	//
	struct permGame_s
	{
	  GBGameInfo	ginfo;

	  permGame_s()
	  {
	  }
	};
	permGame_s	permGames_[4096];
	int		numPermGames_;
	int		numRentGames_;
	void		LoadConfig();

	void		Temp_Load_WarZGames();
	void		LoadRentConfig();
	void		expired(int gameServerId);
	void		SetRentPwd(int gameServerId, const char* pwd, int customerid, int SesionID, bool EnableCrosshair, bool enableSnipers);
	void		DeleteRentServer(int gameServerId);
	void		AddRentGame(int customerid , const char* name , const char* pwd , int mapid , int slot , int RentalTime, int Region, int SesionID, bool EnableCrosshair,bool enableSnipers);
	void		LoadPermGamesConfig();
	void		ParsePermamentGame(int gameServerId, const char* name, const char* map, const char* data, const char* PasswordGame, const char* MapSettings, const char* Crosshair, const char* enableSnipers);
	void		 AddPermanentGame(int gameServerId, const GBGameInfo& ginfo, EGBGameRegion region);

  public:
	CMasterServerConfig();
};
extern CMasterServerConfig* gServerConfig;
