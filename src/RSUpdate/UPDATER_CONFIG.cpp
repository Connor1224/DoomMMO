#include "r3dPCH.h"
#include "r3d.h"

bool	UPDATER_UPDATER_ENABLED  = 1;
char	UPDATER_VERSION[512]     = "1.0.0";
char	UPDATER_VERSION_SUFFIX[512] = "OpenBeta";
char	UPDATER_BUILD[512]	 = __DATE__ " " __TIME__;

char	BASE_RESOURSE_NAME[512]  = "WZ";
char	GAME_EXE_NAME[512]       = "MiniA.exe";
char	GAME_TITLE[512]          = "RaGEZONE Community Edition";

// updater (xml and exe) and game info on our server.
//char	UPDATE_DATA_URL[512]     = "https://127.0.0.1/wz/wz.xml";	// url for data update
//char	UPDATE_UPDATER_URL[512]  = "https://127.0.0.1/wz/updater/woupd.xml";

// HIGHWIND CDN
//char	UPDATE_UPDATER_HOST[512] = "http://127.0.0.1/wz/updater/";

// LOCAL TESTING
//http://127.0.0.1/support

bool	UPDATER_STEAM_ENABLED	 = false;
