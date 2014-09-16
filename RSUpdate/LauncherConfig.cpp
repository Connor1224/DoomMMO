#include "r3dPCH.h"
#include "r3d.h"
#include "r3dNetwork.h"

#include "LauncherConfig.h"

CLauncherConfig gLauncherConfig;

CLauncherConfig::CLauncherConfig()
{
	const char* configFile = "Launcher.cfg";
	const char* group      = "Launcher";

	serialBuyURL = "http://serialbuyurl/";
	serialExpiredBuyURL = "http://serialexpiredurl/";

	accountForgotPasswordURL = "http://accountforgotpassword/";

	myAccountURL = "http://myaccounturl/";
	forumsURL = "http://forums/";
	supportURL = "http://support/";
	youtubeURL = "http://youtube.com/";
	facebookURL = "http://facebook.com/";
	twitterURL = "http://twitter.com/";

	accountUnknownStatusMessage = "Unknown account status, please contact support @ The Wicked Z";
	accountDeletedMessage = "Your account was deleted because your payment was refunded or cancelled\n\nPlease contact your payment provider";
	accountBannedMessage = "Your account has been permanently banned";
	accountFrozenMessage = "Your account has been temporarily frozen because of violation of the Terms of Service ( Paragraph 2 )\n\nYou will be able to continue to use the service in %d hours";
  
	accountCreateFailedMessage = "Account creation failed, please try again later";
	accountCreateEmailTakenMessage = "There is already registered account with that email!\nPlease note that you must use unique email per The Wicked Z account";
	accountCreateInvalidSerialMessage = "Serial Key is not valid after Serial Key Check\ncontact support@localhost";

	webAPIDomainIP = "70.42.74.46";
	webAPIDomainBaseURL = "/api/";
	webAPIDomainPort = 80;
	webAPIDomainUseSSL = false;

	ToSURL = "http://70.42.74.46/other/EULA.rtf";
	EULAURL = "http://70.42.74.46/other/TOS.rtf";

	updateGameDataURL = "http://70.42.74.46/wz/wz.xml";
	updateLauncherDataURL = "http://70.42.74.46/wz/updater/woupd.xml";
	updateLauncherDataHostURL = "http://70.42.74.46/wz/updater/";
	serverInfoURL = "http://70.42.74.46/api_getserverinfo.xml";
   
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