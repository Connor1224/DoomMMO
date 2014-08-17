#include "r3dPCH.h"
#include "r3d.h"
#include "HWInfo.h"
#include "WOLoginHelper.h"
#include "WOBackendAPI.h"
#include "SteamHelper.h"
#include <stdio.h>
#include <Windows.h>
#include <Iphlpapi.h>
#include <Assert.h>
#pragma comment(lib, "iphlpapi.lib")  

char* getMAC(){// By Yuri-BR
    PIP_ADAPTER_INFO AdapterInfo;
    DWORD dwBufLen = sizeof(AdapterInfo);
    char *mac_addr = (char*)malloc(17);

    AdapterInfo = (IP_ADAPTER_INFO *) malloc(sizeof(IP_ADAPTER_INFO));
    if (AdapterInfo == NULL) 
    {
        printf("Error allocating memory needed to call GetAdaptersinfo\n");
    }

    // Make an initial call to GetAdaptersInfo to get the necessary size into the dwBufLen     variable
    if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) 
    {

        AdapterInfo = (IP_ADAPTER_INFO *) malloc(dwBufLen);
        if (AdapterInfo == NULL) 
        {
             printf("Error allocating memory needed to call GetAdaptersinfo\n");
        }
     }

    if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) 
    {
        PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;// Contains pointer to current adapter info
        do 
        {
            sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X", // By Yuri-BR
            pAdapterInfo->Address[0], pAdapterInfo->Address[1],
            pAdapterInfo->Address[2], pAdapterInfo->Address[3],
            pAdapterInfo->Address[4], pAdapterInfo->Address[5]);

            return mac_addr;

            printf("\n");
            pAdapterInfo = pAdapterInfo->Next;        
        }
        while(pAdapterInfo);                        
        }
        free(AdapterInfo);
} 

void CLoginHelper::DoLogin()
{
	SaveUserName();
	LoadComputerToken(); 

	CWOBackendReq req("api_Login.aspx");
	req.AddParam("username", username);
	req.AddParam("password", passwd);
	req.AddParam("serverkey", "rJFhzL01360bPQZqU11FxZ589o5Eb6dr");
	req.AddParam("computerid", hardwareid);
	req.AddParam("mac", getMAC());

	if(!req.Issue())
	{
		r3dOutToLog("Login FAILED, code: %d\n", req.resultCode_);
		loginAnswerCode = ANS_Error;
		return;
	}

	int n = sscanf(req.bodyStr_, "%d %d %d", 
		&CustomerID, 
		&SessionID,
		&AccountStatus);
	if(n != 3) {
		r3dError("Login: bad answer\n");
		return;
	}

	if(AccountStatus == 999) {
		loginAnswerCode = ANS_Deleted;
		return;
	}

	if(CustomerID == 0) {
		loginAnswerCode = ANS_BadPassword;
		return;
	}
	
	if(AccountStatus == 100)
		loginAnswerCode = ANS_Logged;
	else if(AccountStatus == 70)
		loginAnswerCode = ANS_GameActive;
	else if(AccountStatus == 200)
		loginAnswerCode = ANS_Banned;
	else if(AccountStatus == 201)  
		loginAnswerCode = ANS_Frozen;
	else if(AccountStatus == 202)
		loginAnswerCode = ANS_HWBanned;
	else if(AccountStatus == 300)
		loginAnswerCode = ANS_TimeExpired;
	else
		loginAnswerCode = ANS_Unknown;
		
	return;
}

bool CLoginHelper::CheckSteamLogin()
{
	r3d_assert(gSteam.steamID);
	r3d_assert(gSteam.authToken.getSize() > 0);

	CkString ticket;
	gSteam.authToken.encode("hex", ticket);

	CWOBackendReq req("api_SteamLogin.aspx");
	req.AddParam("ticket", ticket.getAnsi());
	if(!req.Issue()) {
		r3dOutToLog("CheckSteamLogin: failed %d\n", req.resultCode_);
		return false;
	}

	int n = sscanf(req.bodyStr_, "%d %d %d", 
		&CustomerID, 
		&SessionID,
		&AccountStatus);
	if(n != 3) {
		r3dOutToLog("CheckSteamLogin: bad answer %s\n", req.bodyStr_);
		return false;
	}
	
	return true;
}

void CLoginHelper::SaveUserName()
{
	HKEY hKey;
	int hr;
	hr = RegCreateKeyEx(HKEY_CURRENT_USER, 
		"Software\\Tilcon Network\\DoomMMO", 
		0, 
		NULL,
		REG_OPTION_NON_VOLATILE, 
		KEY_ALL_ACCESS,
		NULL,
		&hKey,
		NULL);
	if(hr == ERROR_SUCCESS)
	{
		DWORD size = strlen(username) + 1;
		
		hr = RegSetValueEx(hKey, "username", NULL, REG_SZ, (BYTE*)username, size);
		RegCloseKey(hKey);
	}
}

bool CLoginHelper::LoadUserName()
{
	// query for game registry node
	HKEY hKey;
	int hr;
	hr = RegOpenKeyEx(HKEY_CURRENT_USER, 
		"Software\\Tilcon Network\\DoomMMO", 
		0, 
		KEY_ALL_ACCESS, 
		&hKey);
	if(hr != ERROR_SUCCESS)
		return true;

	DWORD size = sizeof(username);
	hr = RegQueryValueEx(hKey, "username", NULL, NULL, (BYTE*)username, &size);
	RegCloseKey(hKey);
	
	return true;
}
bool CLoginHelper::LoadComputerToken(){
    HKEY hKey;
    int hr;
    hr = RegOpenKeyEx(HKEY_CURRENT_USER, 
        "Software\\House of Warrior Interactive\\WarZ", 
        0, 
        KEY_ALL_ACCESS, 
        &hKey);
    if(hr != ERROR_SUCCESS)
    {
        CreateComputerToken();
        return true;
    }

    DWORD size = sizeof(hardwareid);
    hr = RegQueryValueEx(hKey, "hardwareid", NULL, NULL, (BYTE*)hardwareid, &size);
    RegCloseKey(hKey);

    return true;
}

void CLoginHelper::CreateComputerToken()
{
    HKEY hKey;
    int hr;
    hr = RegCreateKeyEx(HKEY_CURRENT_USER, 
        "Software\\Tilcon Network\\DoomMMO", 
        0, 
        NULL,
        REG_OPTION_NON_VOLATILE, 
        KEY_ALL_ACCESS,
        NULL,
        &hKey,
        NULL);
    if(hr == ERROR_SUCCESS)
    {
        CHWInfo g_HardwareInfo;
        g_HardwareInfo.Grab();
        sprintf(hardwareid, "0x%I64x", g_HardwareInfo.uniqueId); // Hardware ID By Yuri-BR

        DWORD size = strlen(hardwareid) + 1;

        hr = RegSetValueEx(hKey, "hardwareid", NULL, REG_SZ, (BYTE*)hardwareid, size);
        RegCloseKey(hKey);
    }
}  

void CLoginHelper::CreateAuthToken(char* token) const
{
	char sessionInfo[512];
	sprintf(sessionInfo, "%d:%d:%d", CustomerID, SessionID, AccountStatus);

	for(size_t i=0; i<strlen(sessionInfo); ++i)
		sessionInfo[i] = sessionInfo[i] ^ 0x64;
        
	CkString encoded;
	encoded = sessionInfo;
	encoded.base64Encode("utf-8");
      
	strcpy(token, encoded.getAnsi());
	return;
}

void CLoginHelper::CreateLoginToken(char* token) const
{
	char crlogin[128];
	strcpy(crlogin, username);
	
	CkString s1;
	s1 = username;
	s1.base64Encode("utf-8");

	CkString s2;
	s2 = passwd;
	s2.base64Encode("utf-8");
      
	sprintf(token, "-login \"@%s\" -pwd \"@%s\"", s1.getAnsi(), s2.getAnsi());
	return;
}