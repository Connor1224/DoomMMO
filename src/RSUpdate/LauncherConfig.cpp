#include "r3dPCH.h"
#include "r3d.h"
#include "r3dNetwork.h"

#include "LauncherConfig.h"

CLauncherConfig gLauncherConfig;

CLauncherConfig::CLauncherConfig()
{
	const char* configFile = "Launcher.cfg";
	const char* group      = "Launcher";

	if(_access(configFile, 4) != 0) {
//	r3dError("can't open config file %s", configFile);
	}

	serialBuyURL = r3dReadCFG_S(configFile, group, "serialBuyURL", "http://localhost/buy?ref=WarL");
	serialExpiredBuyURL = r3dReadCFG_S(configFile, group, "serialExpiredBuyURL", "http://localhost/buy?ref=WarE");

	accountForgotPasswordURL = r3dReadCFG_S(configFile, group, "accountForgotPasswordURL",
		"https://localhost/account_check");
	accountRegisterURL = r3dReadCFG_S(configFile, group, "accountRegisterURL",
		"https://localhost/account_register");

	myAccountURL = r3dReadCFG_S(configFile, group, "myAccountURL", "https://localhost/account_check");
	forumsURL = r3dReadCFG_S(configFile, group, "forumsURL", "http://localhost/forum");
	supportURL = r3dReadCFG_S(configFile, group, "supportURL", "http://localhost/support");
	youtubeURL = r3dReadCFG_S(configFile, group, "youtubeURL", "http://www.youtube.com");
	facebookURL = r3dReadCFG_S(configFile, group, "facebookURL", "http://www.facebook.com");
	twitterURL = r3dReadCFG_S(configFile, group, "twitterURL", "http://twitter.com/");

	accountUnknownStatusMessage = r3dReadCFG_S(configFile, group, "accountUnknownStatusMessage",
		"Unknown account status, please contact support@localhost");
	accountDeletedMessage = r3dReadCFG_S(configFile, group, "accountDeletedMessage",
		"Your account was deleted because your payment was refunded or cancelled\n\nPlease contact your payment provider");
	accountBannedMessage = r3dReadCFG_S(configFile, group, "accountBannedMessage",
		"Your account has been permanently banned");
	accountHWBannedMessage = r3dReadCFG_S(configFile, group, "accountHWBannedMessage",
		"Your account has been banned by HardwareID");
	accountFrozenMessage = r3dReadCFG_S(configFile, group, "accountFrozenMessage",
		"Your account has been temporarily frozen because of violation of the Terms of Service ( Paragraph 2 )\n\nYou will be able to continue to use the service in %d hours");
  
	accountCreateFailedMessage = r3dReadCFG_S(configFile, group, "accountCreateFailedMessage",
		"Account creation failed, please try again later");
	accountCreateEmailTakenMessage = r3dReadCFG_S(configFile, group, "accountCreateEmailTakenMessage",
		"There is already registered account with that email!\nPlease note that you must use unique email per The War Z account");
	accountCreateInvalidSerialMessage = r3dReadCFG_S(configFile, group, "accountCreateInvalidSerialMessage",
		"Serial Key is not valid after Serial Key Check\ncontact support@localhost");

	webAPIDomainIP = r3dReadCFG_S(configFile, group, "webAPIDomainIP", "127.0.0.1");
	webAPIDomainBaseURL = r3dReadCFG_S(configFile, group, "webAPIDomainBaseURL", "/api/");
	webAPIDomainPort = r3dReadCFG_I(configFile, group, "webAPIDomainPort", 80);
	webAPIDomainUseSSL = r3dReadCFG_I(configFile, group, "webAPIDomainUseSSL", 0) ? true : false;

	ToSURL = r3dReadCFG_S(configFile, group, "tosURL", "http://127.0.0.1/EULA.rtf");
	EULAURL = r3dReadCFG_S(configFile, group, "eulaURL", "http://127.0.0.1/TOS.rtf");

	updateGameDataURL = r3dReadCFG_S(configFile, group, "updateGameDataURL",
		"http://127.0.0.1/wz/wz.xml");
	updateLauncherDataURL = r3dReadCFG_S(configFile, group, "updateLauncherDataURL",
		"http://127.0.0.1/wz/updater/woupd.xml");
	// updateLauncherDataHostURL Used by -generate cmdline arg to output a woupd.xml file.
	updateLauncherDataHostURL = r3dReadCFG_S(configFile, group, "updateLauncherDataHostURL",
		"http://127.0.0.1/wz/updater/");

	serverInfoURL = r3dReadCFG_S(configFile, group, "serverInfoURL",
		"http://127.0.0.1/api_getserverinfo.xml");

	//***************
	// Manual Config
	//***************
	serialBuyURL = "http://127.0.0.1/";
	serialExpiredBuyURL = "http://127.0.0.1/";

	accountForgotPasswordURL = "http://127.0.0.1/";
	accountRegisterURL = "http://127.0.0.1/reg";
	//accountRegisterURL = "DoCreateAcc()";//test

	myAccountURL = "https://127.0.0.1/account_check";
	forumsURL = "http://127.0.0.1/forum";
	supportURL = "http://127.0.0.1/support";
	youtubeURL = "http://youtube.com/";
	facebookURL = "http://facebook.com/";
	twitterURL = "http://twitter.com/";

	accountUnknownStatusMessage = "Unknown account status, please contact support@localhost";
	accountDeletedMessage = "Your account was deleted because your payment was refunded or cancelled\n\nPlease contact your payment provider";
	accountBannedMessage = "Your account has been permanently banned";
	accountHWBannedMessage = "Your account has been banned by HardwareID";
	accountFrozenMessage = "Your account has been temporarily frozen because of violation of the Terms of Service ( Paragraph 2 )\n\nYou will be able to continue to use the service in %d hours";
  
	accountCreateFailedMessage = "Account creation failed, please try again later";
	accountCreateEmailTakenMessage = "There is already registered account with that email!\nPlease note that you must use unique email per The War Z account";
	accountCreateInvalidSerialMessage = "Serial Key is not valid after Serial Key Check\ncontact support@localhost";

	webAPIDomainIP = "127.0.0.1";
	webAPIDomainBaseURL = "/api/";
	webAPIDomainPort = 80;
	webAPIDomainUseSSL = false;

	ToSURL = "http://127.0.0.1/EULA.rtf";
	EULAURL = "http://127.0.0.1/TOS.rtf";

	updateGameDataURL = "http://127.0.0.1/wz/wz.xml";
	updateLauncherDataURL = "http://127.0.0.1/wz/updater/woupd.xml";
	updateLauncherDataHostURL = "http://127.0.0.1/wz/updater/";
	serverInfoURL = "http://127.0.0.1/api_getserverinfo.xml";
   
	#define CHECK_I(xx) if(xx == 0)  r3dError("missing %s value", #xx);
	#define CHECK_S(xx) if(xx == "") r3dError("missing %s value", #xx);
	CHECK_I(webAPIDomainPort);
	CHECK_S(webAPIDomainIP);
	CHECK_S(webAPIDomainBaseURL);

	CHECK_S(updateGameDataURL);
	CHECK_S(updateLauncherDataURL);
	CHECK_S(updateLauncherDataHostURL);

	CHECK_S(serverInfoURL);
	#undef CHECK_I
	#undef CHECK_S
 
	return;
}