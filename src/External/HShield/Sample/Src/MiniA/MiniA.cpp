// MiniA.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"
#include "HShield.h"
#include "HSUpChk.h"
#include <assert.h>
#include <stdio.h>
#include <winsock2.h>
#include <process.h>
#include "NetSvMngr.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;											// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];						// The title bar text
DWORD g_dwMainThreadID;
TCHAR	g_szHShieldPath[MAX_PATH] = {0,};
TCHAR	g_szIniPath[MAX_PATH] = {0,};
int		g_nServerPort = 0;
char	g_szServerIP[20] = {0,};

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
int __stdcall AhnHS_Callback(long lCode, long lParamSize, void* pParam);

UINT
WINAPI
NetworkThreadProcClient ( LPVOID lParam );
void GetEnvInfoFromIni(void);

//
__inline void TRACE(LPCTSTR szFormat, ... )
{
	TCHAR szMsg[1024];
	ZeroMemory(szMsg, 1024);

	va_list vargs;
	va_start( vargs, szFormat );
	_vsntprintf( szMsg + _tcslen(szMsg), sizeof(szMsg), szFormat, vargs );
	va_end( vargs );
	OutputDebugString(szMsg);
}


int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	TCHAR	*pEnd = NULL;
	TCHAR	szFullFileName[MAX_PATH] = { 0, };
	TCHAR	szMsg[255];
	int		nRet;

	g_dwMainThreadID = GetCurrentThreadId();
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);

	// 현재 프로세스 실행 기준으로 FULL-PATH를 얻음
	GetModuleFileName(NULL, szFullFileName, MAX_PATH);
	pEnd = _tcsrchr( szFullFileName, _T('\\')) + 1;
	if (!pEnd)
	{
		return FALSE;	
	}
	*pEnd = _T('\0');

	_stprintf(g_szIniPath, _T("%s"), szFullFileName);				
	_stprintf(g_szHShieldPath, _T("%s\\HShield"), szFullFileName);	// HShield-Path 세팅
	_tcscat(szFullFileName, _T("HShield\\EhSvc.dll"));				// EHSVC.DLL =Path 세팅.
	_tcscat(g_szIniPath, _T("MiniAEnv.INI"));						// MiniAEnv.ini Path 세팅

	// INI파일로부터 환경정보를 읽음(확장서버연동 사용 유무, 서버 IP, 서버 PORT)
	GetEnvInfoFromIni();

	// HS-Monitoring 서비스 사용을 위한 구조체 선언
	AHNHS_EXT_ERRORINFO HsExtError;
	// 아이디나 모니터링 주소, 게임버전등은 암호화나 인코딩 하여 보관하였다가 실제 아래 함수에 입력시
	// 복호화나 디코딩 하여 입력하는 것이 좋다.
	sprintf( HsExtError.szServer, "%s", "210.121.169.209" );		// 모니터링 서버주소
	sprintf( HsExtError.szUserId, "%s", "GameUser" );				// 유저 아이디
	// _AhnHS_Initialize시점에 아이디를 얻을 수 없는 경우에는 널 스트링을 입력한다.
	// 아이디를 얻는 시점에 _AhnHS_SetUserId를 호출하여 아이디를 전송한다.
	// sprintf( HsExtError.szUserId, "%s", "" ); 
	sprintf( HsExtError.szGameVersion, "%s", "1.0.0.1" );			// 게임 버전

	// -------------------------------------------------------------------------
	// [_AhnHS_StartMonitor]
	// -------------------------------------------------------------------------
	// - DESCRIPTION: 
	//	  핵쉴드 모니터링 서비스를 시작합니다.
	// - SYNTAX:
	//	   EHSVC_API int __stdcall _AhnHS_StartMonitor(AHNHS_EXT_ERRORINFO HsExtError, LPCSTR szFullFileName)
	//
	//	  .  AHNHS_EXT_ERRORINFO HsExtErrorInfo		: AHNHS_EXT_ERRORINFO HsExtErrorInfo :  모니터링 관련 입력 정보들
    //						                          - 서버 URL주소. ( "123.456.789.123" or "www.monitor.com" )
	//												  - User ID		  ( "User1" )
	//												  - Game Version  ( "5.0.2.1" )
	//	  .  szFullFileName							: EHSVC.DLL 전체 경로
	//
	// - RETURN VALUE:
	//     . HS_ERR_OK          : 성공
	//     . 기타				: 기타 리턴값에 대해서는 핵쉴드 프로그래밍 가이드 참고바람.


	//
	nRet = _AhnHS_StartMonitor( HsExtError, szFullFileName );

	if (nRet != HS_ERR_OK)
	{
		_stprintf(szMsg, _T("모니터링 기능에 문제가 발생하였습니다.(Error Code = %x)\n프로그램을 종료합니다."), nRet);
		MessageBox(NULL, szMsg, szTitle, MB_OK);
		return FALSE;
	}

	// -------------------------------------------------------------------------
	// [_AhnHS_Initialize]
	// -------------------------------------------------------------------------
	// - DESCRIPTION: 
	//	  초기화 함수 호출 
	//    HackShield를 초기화하고, 옵션을 설정합니다. 
	//	  프로그램이 초기화될 때 한 번만 호출할 수 있습니다. 
	//    다른 게임 프로그램에서 HackShield를 사용하고 있거나 서비스가 비정상적으로 
	//    종료되었을 경우 오류가 발생할 수 있습니다. 
	//
	// - SYNTAX:
	//	   EHSVC_API int __stdcall _AhnHS_Initialize(const char* szFileName,	PFN_AhnHS_Callback pfn_Callback, 
	//												int nGameCode, const char* szLicenseKey, DWORD dwOption,
	//												UINT unAdditionalRatio)
	//	  . szFileName			: EHSvc.dll의 전체 경로
	//	  . Pfn_Callback		: Callback 함수의 포인터
	//    . nGameCode			: 4자리 숫자	각 게임에 해당하는 고유 ID 코드
	//    . szLicenseKe			: 24자리 문자열	각 게임에 해당하는 라이선스 키
	//    . dwOption			: 초기화 옵션 설정
	//    . unAdditionalRatio : 추가적인 비율 옵션 설정 (스피드 핵 감지율 레벨 , Self-Destruction 시간 레벨)
	//
	// - OPTIONS:
	//   핵쉴드 프로그래밍 가이드 참고바람.
	//
	// - RETURN VALUE:
	//     . HS_ERR_OK          : 성공
	//     . 기타				: 기타 리턴값에 대해서는 핵쉴드 프로그래밍 가이드 참고바람.

	
	nRet = _AhnHS_Initialize(szFullFileName, AhnHS_Callback, 
		1000, "B228F2916A48AC24", 
		AHNHS_CHKOPT_ALL,
		AHNHS_SPEEDHACK_SENSING_RATIO_NORMAL|AHNHS_SELFDESTRUCTION_RATIO_NORMAL);

	//아래 에러는 개발과정에서만 발생할 수 있으면 
	//이후 결코 발생해서는 안되는 에러이므로 assert처리를 했습니다.
	assert(nRet != HS_ERR_INVALID_PARAM);
	assert(nRet != HS_ERR_INVALID_LICENSE);
	assert(nRet != HS_ERR_ALREADY_INITIALIZED);

	if (nRet != HS_ERR_OK) 
	{
		//Error 처리 
		switch(nRet)
		{
			case HS_ERR_ANOTHER_SERVICE_RUNNING:
			{
				MessageBox(NULL, _T("다른 게임이 실행중입니다.\n프로그램을 종료합니다."), szTitle, MB_OK);
				break;
			}
			case HS_ERR_INVALID_FILES:
			{
				MessageBox(NULL, _T("잘못된 파일 설치되었습니다.\n프로그램을 재설치하시기 바랍니다."), szTitle, MB_OK);
				break;
			}
			case HS_ERR_DEBUGGER_DETECT:
			{
				MessageBox(NULL, _T("컴퓨터에서 디버거 실행이 감지되었습니다.\n디버거의 실행을 중지시킨 뒤에 다시 실행시켜주시기바랍니다."), szTitle, MB_OK);
				break;
			}
			case HS_ERR_NEED_ADMIN_RIGHTS:
			{
				MessageBox(NULL, _T("Admin 권한으로 실행되어야 합니다.\n프로그램을 종료합니다."), szTitle, MB_OK);
				break;
			}
			case HS_ERR_COMPATIBILITY_MODE_RUNNING:
			{
				MessageBox(NULL, _T("호환성 모드로 프로그램이 실행중입니다.\n프로그램을 종료합니다."), szTitle, MB_OK);
				break;				
			}
			default:
			{
				_stprintf(szMsg, _T("해킹방지 기능에 문제가 발생하였습니다.(Error Code = %x)\n프로그램을 종료합니다."), nRet);
				MessageBox(NULL, szMsg, szTitle, MB_OK);
				break;
			}
		}
		return FALSE;
	}

	// -------------------------------------------------------------------------
	// [_AhnHS_StartService]
	// -------------------------------------------------------------------------
	// - DESCRIPTION:
	//	   해킹 툴 탐지 기능과 해킹 차단 기능을 동작시킵니다. 
	//	   _AhnHS_Initialize 함수를 호출한 다음에 호출해야 하며, 중복 호출할 수 없습니다. 
	//	   _AhnHS_StopService 함수를 호출하여 서비스를 중지한 경우에는, 이 함수를 다시 
	//	   호출하여 서비스를 다시 시작시킬 수 있습니다.
	//
	// - SYNTAX:
	//	  EHSVC_API int __stdcall _AhnHS_StartService( );
	//
	// - RETURN VALUE:
	//    . HS_ERR_OK          : 성공
	//    . 기타				: 기타 리턴값에 대해서는 핵쉴드 프로그래밍 가이드 참고바람.

	nRet = _AhnHS_StartService();
	assert(nRet != HS_ERR_NOT_INITIALIZED);
	assert(nRet != HS_ERR_ALREADY_SERVICE_RUNNING);

	if (nRet != HS_ERR_OK)
	{
		_stprintf(szMsg, _T("해킹방지 기능에 문제가 발생하였습니다.(Error Code = %x)\n프로그램을 종료합니다."), nRet);
		MessageBox(NULL, szMsg, szTitle, MB_OK);
		return FALSE;
	}
	

	// 서버연동을 위한 Network 통신 Thread를 생성함.
	DWORD	nThreadID = 0;
	HANDLE m_hThread = ( HANDLE ) _beginthreadex ( NULL, 0, NetworkThreadProcClient, ( void * ) NULL, 0, (unsigned *) &nThreadID );
	
	MSG msg;
	HACCEL hAccelTable;

	LoadString(hInstance, IDC_MINIA, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MINIA);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	// -------------------------------------------------------------------------
	// [_AhnHS_StopService]
	// -------------------------------------------------------------------------
	// - DESCRIPTION:
	//	  해킹 차단 기능과 해킹 툴 탐지 기능을 정지시킵니다.
	//
	// - SYNTAX:
	// 	  EHSVC_API int __stdcall _AhnHS_StopService( );
	//
	// - RETURN VALUE:
	//     . HS_ERR_OK					 : 성공
	//	   . HS_ERR_NOT_INITIALIZED		 : _AhnHS_Initialize 함수를 호출하여 HackShield를 초기화하지 않은 상태에서 
	//										호출한 경우 발생하는 오류입니다. 
	//										이 오류는 개발 과정에서만 발생할 수 있는 오류로 별도의 처리는 하지 않아도 됩니다. (Value = 0x003)
	//     . HS_ERR_SERVICE_NOT_RUNNING  : _AhnHS_StartService 함수를 호출하여 HackShield를 시작하지 않은 상태에서 
	//                                     호출한 경우 발생한 오류입니다. 개발 과정에서만 발생할 수 있는 오류로 별도의
	//                                      처리는 하지 않아도 됩니다. (Value = 0x301)

	nRet = _AhnHS_StopService();
	
	if (nRet != HS_ERR_OK)
	{
		_stprintf(szMsg, _T("_AhnHS_StopService 오류 발생 (Error Code = %x)"), nRet);
		MessageBox(NULL, szMsg, szTitle, MB_OK);
		return FALSE;
	}

	
	// -------------------------------------------------------------------------
	// [_AhnHS_Uninitialize]
	// -------------------------------------------------------------------------
	// - DESCRIPTION:
	//	 시스템 내부적으로 사용되었던 메모리를 해제하고 변수를 초기화합니다.
	//
	// - SYNTAX:
	// 	 EHSVC_API int __stdcall _AhnHS_Uninitialize( );
	//
	// - RETURN VALUE:
	//   . HS_ERR_OK				    : 성공
	//	 . HS_ERR_SERVICE_STILL_RUNNING : _AhnHS_StopSerivice 함수를 호출하여 HackShield를 종료하지 않은 상태에서 
	//                                   호출하면 발생하는 오류입니다. 이 오류는 개발 과정에서만 발생할 수 있으므로 
	//                                   별도의 처리는 하지 않아도 됩니다. (Value = 0x302)
	//   . HS_ERR_NOT_INITIALIZED	     : _AhnHS_Initialize 함수 호출을 통하여 시스템을 초기화하지 않은 상태에서 
	//                                     호출하면 발생하는 오류입니다. 이 오류는 개발 과정에서만 발생할 수 있으므로 
	//                                     별도의 처리는 하지 않아도 됩니다. (Value = 0x003)


	nRet = _AhnHS_Uninitialize();

	if (nRet != HS_ERR_OK)
	{
		_stprintf(szMsg, _T("_AhnHS_Uninitialize 오류 발생 (Error Code = %x)"), nRet);
		MessageBox(NULL, szMsg, szTitle, MB_OK);
		return FALSE;
	}

	return msg.wParam;
}

//
void GetEnvInfoFromIni(void)
{
	// 서버 포트
	g_nServerPort = ::GetPrivateProfileInt(_T("ServerInfo"), _T("nPort"), 0, g_szIniPath);
	// 서버 IP
	::GetPrivateProfileString(_T("ServerInfo"), _T("IP"), _T(""), g_szServerIP, sizeof(g_szServerIP), g_szIniPath);
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_MINIA);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_MINIA;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

UINT
WINAPI
ShowMsgAndExitWithTimer_ThreadFunc( LPVOID lParam )
{
	
	Sleep(10000);  // 10초동안 대기..

	HANDLE hCurrentHandle = ::GetCurrentProcess();

	// 게임에서 사용중인 각종 메모리 버퍼 해제 및 Handle 종료 
	// ..
	// ..
	::TerminateProcess( hCurrentHandle, 0 );
	return 0;
}

//
void ShowMsgAndExitWithTimer(TCHAR *szMsg)
{
	UINT nThreadID = 0;
	
	HANDLE hThread = ( HANDLE ) _beginthreadex ( NULL, 0, ShowMsgAndExitWithTimer_ThreadFunc, NULL, 0, &nThreadID );

	::MessageBox(NULL, szMsg, szTitle, MB_OK);
	HANDLE hCurrentHandle = ::GetCurrentProcess();

	// 게임에서 사용중인 각종 메모리 버퍼 해제 및 Handle 종료 
	// ..
	// ..

	::TerminateProcess( hCurrentHandle, 0 );
}


int __stdcall AhnHS_Callback(long lCode, long lParamSize, void* pParam)
{
	switch(lCode)
	{
		//Engine Callback
		case AHNHS_ENGINE_DETECT_GAME_HACK:
		{
			TCHAR szMsg[255];
			_stprintf(szMsg, _T("다음 위치에서 해킹툴이 발견되어 프로그램을 종료시켰습니다.\n%s"), (char*)pParam);
			ShowMsgAndExitWithTimer(szMsg);

			break;
		}
		//창모드 해킹툴 감지
		case AHNHS_ENGINE_DETECT_WINDOWED_HACK:
		{
			ShowMsgAndExitWithTimer(_T("프로그램에 대하여 창모드 해킹툴이 감지되었습니다."));

			break;
		}
		//Speed 관련
		case AHNHS_ACTAPC_DETECT_SPEEDHACK:
		{
			ShowMsgAndExitWithTimer(_T("현재 이 PC에서 SpeedHack으로 의심되는 동작이 감지되었습니다."));
			break;
		}

		//디버깅 방지 
		case AHNHS_ACTAPC_DETECT_KDTRACE:	
		case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:
		{
			TCHAR szMsg[255];
			_stprintf(szMsg, _T("프로그램에 대하여 디버깅 시도가 발생하였습니다. (Code = %x)\n프로그램을 종료합니다."), lCode);
			ShowMsgAndExitWithTimer(szMsg);
			break;
		}
		
		case AHNHS_ACTAPC_DETECT_AUTOMACRO:
		{
			TCHAR szMsg[255];
			_stprintf(szMsg, _T("매크로 기능으로 의심되는 동작이 감지되었습니다.. (Code = %x)\n프로그램을 종료합니다."), lCode);
			ShowMsgAndExitWithTimer(szMsg);

			break;
		}

		// 코드 패치 감지
		case AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS:
		{
			TCHAR szMsg[255];
			_stprintf(szMsg, _T("다음 위치에서 메모리 접근이 감지되어 프로그램을 종료시켰습니다.\n%s"), (char*)pParam);
			ShowMsgAndExitWithTimer(szMsg);
			break;
		}

		//그외 해킹 방지 기능 이상 
		case AHNHS_ACTAPC_DETECT_AUTOMOUSE:
		case AHNHS_ACTAPC_DETECT_DRIVERFAILED:
		case AHNHS_ACTAPC_DETECT_HOOKFUNCTION:
		case AHNHS_ACTAPC_DETECT_MESSAGEHOOK:
		case AHNHS_ACTAPC_DETECT_MODULE_CHANGE:
		case AHNHS_ACTAPC_DETECT_ENGINEFAILED:
		case AHNHS_ACTAPC_DETECT_CODEMISMATCH:
		case AHNHS_ACTAPC_DETECT_PROTECTSCREENFAILED:
		{
			TCHAR szMsg[255];
			_stprintf(szMsg, _T("해킹 방어 기능에 이상이 발생하였습니다. (Code = %x)\n프로그램을 종료합니다."), lCode);
			ShowMsgAndExitWithTimer(szMsg);
			break;
		}
	}
	return 1;
}


#define		OP_LOGON				0x1		// Logon에 대한 Notify
#define		OP_LOGOFF				0x2		// Logoff에 대한 Notify
#define		OP_REQGUID				0x3		// GUID Request Message
#define		OP_ACKGUID				0x4		// GUID Ack Message
#define		OP_REQCRC				0x5		// CRC Request Message
#define		OP_ACKCRC				0x6		// CRC Ack Message
#define		OP_ERROR				0x7		// Error Message

// 서버와의 통신(서버연동)을 위한 Thread 프로시져.
UINT WINAPI NetworkThreadProcClient ( LPVOID lParam )
{
	int nErrCode = 0;	
	SOCKET socket = 0;
	MSGHEADER header = { 0, };
	DWORD dwRet = ERROR_SUCCESS;

	// 윈속 DLL을 로드한다.
	dwRet = BOANET_LoadWinsock2 ();
	if ( dwRet != ERROR_SUCCESS )
		goto _end;

	// Network관련 초기화 작업을 진행한다.
	dwRet = BOANET_InitializeCli ( &socket, &nErrCode );
	if ( dwRet != ERROR_SUCCESS )
	{
		BOANET_UnloadWinsock2 ();
		goto _end;
	}

	// 서버에 연결한다.
	dwRet = BOANET_ConnectCli ( socket, g_nServerPort, ( LPTSTR ) ( LPCTSTR ) g_szServerIP, &nErrCode );
	if ( dwRet != ERROR_SUCCESS )
	{
		BOANET_UnloadWinsock2 ();
		goto _end;
	}

	// Logon Msg를 보낸다.
	header.opCode = OP_LOGON;
	header.length = htonl ( 0 );

	dwRet = BOANET_SendCli ( socket,  ( PSTR ) &header, sizeof ( MSGHEADER ), &nErrCode );
	if ( dwRet != ERROR_SUCCESS )
	{
		BOANET_CloseCli ( &socket );
		BOANET_UnloadWinsock2 ();
		goto _end;
	}

	while ( TRUE )
	{
		BYTE byReqMsg[SIZEOF_REQMSG] = { 0, };
		BYTE byAckMsg[SIZEOF_GUIDACKMSG] = { 0, };
		PBYTE pbyResponse;
		AHNHS_TRANS_BUFFER TransBuffer;

		// 1. Header를 읽어들인다.
		dwRet = BOANET_ReceiveCli ( socket, ( PSTR ) &header, sizeof ( MSGHEADER ), &nErrCode );
		if ( dwRet != ERROR_SUCCESS )
		{
			BOANET_CloseCli ( &socket );
			BOANET_UnloadWinsock2 ();
			goto _end;
		}

		// 오류 패킷일 경우 종료
		if ( header.opCode == OP_ERROR )
		{
			dwRet = ntohl ( header.length );
			break;
		}

		// 2. Body를 받는다.		
		dwRet = BOANET_ReceiveCli ( socket, ( PSTR ) byReqMsg, ntohl ( header.length ), &nErrCode );
		if ( dwRet != ERROR_SUCCESS )
		{
			BOANET_CloseCli ( &socket );
			BOANET_UnloadWinsock2 ();
			goto _end;
		}

		// 3. Msg별 처리를 한다.

		// -------------------------------------------------------------------------
		// [_AhnHS_MakeResponse]
		// -------------------------------------------------------------------------
		// - DESCRIPTION: 
		//	  클라이언트에서 사용합니다. 서버에서 전달한 암호화된 버전 요구 메시지를 
		//	  복호화하고 현재 클라이언트 파일의 버전을 암호화하여 응답 메시지를 만듭니다.
		//
		// - SYNTAX:
		//		int __stdcall _AhnHS_MakeResponse (unsigned char *pbyRequest, unsigned long ulRequestLength, 
		//									PAHNHS_TRANS_BUFFER pResponseBuffer);
		// - PARAMETERS:
		//		.pbyRequest	unsigned char *				:[IN]	요청 메시지 버퍼
		//		.ulRequestLength	unsigned long		:[IN]	요청 메시지 길이
		//		.pResponseBuffer	PAHNHS_TRANS_BUFFER	:[OUT]  응답 메시지 버퍼
		//
		// - RETURN VALUE:
		//     . ERROR_SUCCESS      : 성공
		//     . 기타				: 기타 리턴값에 대해서는 핵쉴드 프로그래밍 가이드 참고바람.
		dwRet = _AhnHS_MakeResponse( byReqMsg, ntohl ( header.length ), &TransBuffer );
		if ( dwRet != ERROR_SUCCESS )
		{
			BOANET_CloseCli ( &socket );
			BOANET_UnloadWinsock2 ();
			goto _end;
		}

		TRACE("[%x] = _AhnHS_MakeResponse()", dwRet);

		// Header를 작성한다.
		header.opCode = OP_ACKCRC;
		header.length = htonl ( TransBuffer.nLength );



		// 4. Header를 보낸다.
		dwRet = BOANET_SendCli ( socket,  ( PSTR ) &header, sizeof ( MSGHEADER ), &nErrCode );
		if ( dwRet != ERROR_SUCCESS )
		{
			BOANET_CloseCli ( &socket );
			BOANET_UnloadWinsock2 ();
			goto _end;
		}

		// 5. Body를 보낸다.
		// [확장 서버연동] TransBuffer 의 바이트 배열을 사용한다.
		pbyResponse = TransBuffer.byBuffer;
		dwRet = BOANET_SendCli ( socket,  ( PSTR ) pbyResponse, ntohl ( header.length ), &nErrCode );
		if ( dwRet != ERROR_SUCCESS )
		{
			BOANET_CloseCli ( &socket );
			BOANET_UnloadWinsock2 ();
			goto _end;
		}
	}

	// Logoff Msg를 보낸다.
	header.opCode = OP_LOGOFF;
	header.length = htonl ( 0 );

	BOANET_SendCli ( socket,  ( PSTR ) &header, sizeof ( MSGHEADER ), &nErrCode );

	// 서버와 연결을 끊는다.
	BOANET_CloseCli ( &socket );

	// 윈속 DLL을 언로딩한다.	
	BOANET_UnloadWinsock2 ();

_end:

	if ( dwRet != ERROR_SUCCESS )
	{
		TRACE("[%x] = Network Thread Failed", dwRet);
	}
	else
	{
		TRACE("Network Thread Successfully Terminated");
	}

	_endthreadex ( 0 );

	return 0;
}

// 하기와 같은 형태로 함수를 구성하여 핵쉴드 업데이트 시점에 호출합니다.
void OnAhnHS_HSUpdate()
{
	// -------------------------------------------------------------------------
	// [_AhnHS_HSUpdate]
	// -------------------------------------------------------------------------
	// - DESCRIPTION: 
	//	 HackShield 엔진 파일들을 업데이트합니다.
	//
	// - SYNTAX:
	//	 DWORD __stdcall _AhnHS_HSUpdate( LPCTSTR szUpdateDir, DWORD dwTimeOut );
	//	
	// - PARAMETERS:
	//	 .szUpdateDir				업데이트 파일이 설치되어 있는 폴더
	//	 .dwTimeOut	milliseconds	업데이트 시 타임 아웃 시간. 0으로 설정 시 INFINITE으로 설정

	// - RETURN VALUE:
	//			. HACKSHIELD_ERROR_SUCESS	: 업데이트 성공 (Value = 0x00000000)
	//          . 기타						: 기타 리턴값에 대해서는 핵쉴드 프로그래밍 가이드 참고바람.

	DWORD dwResult = _AhnHS_HSUpdate(g_szHShieldPath, 1000 * 600);

	LPTSTR szErrorStr = _T("");
	switch(dwResult)
	{
		case 0x00000000:
			szErrorStr = _T("HS_ERR_OK");
			break;
		case 0x30000010:
			szErrorStr = _T("HS_ERR_ENVFILE_NOTREAD");
			break;
		case 0x30000020:
			szErrorStr = _T("HS_ERR_ENVFILE_NOTWRITE");
			break;
		case 0x30000030:
			szErrorStr = _T("HS_ERR_NETWORK_CONNECT_FAIL");
			break;
		case 0x30000050:
			szErrorStr = _T("HS_ERR_LIB_NOTEDIT_REG");
			break;
		case 0x30000060:
			szErrorStr = _T("HS_ERR_NOTFINDFILE");
			break;
		case 0x30000070:
			szErrorStr = _T("HS_ERR_PROTECT_LISTLOAD_FAIL");
			break;
		case 0x30000080:
			szErrorStr = _T("HS_ERR_PROTECT_VERITY_FAIL");
			break;
		case 0x30000090:
			szErrorStr = _T("HS_ERR_HSUPDATE_TIMEOUT");
			break;
		default :
			szErrorStr = _T("HS_ERR_UNKNOWN");
			break;
	}
	
	TRACE("%s[%x] = AhnHS_HSUpdate(%s, %d)", szErrorStr, dwResult, g_szHShieldPath, 0);
}
