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

	// ���� ���μ��� ���� �������� FULL-PATH�� ����
	GetModuleFileName(NULL, szFullFileName, MAX_PATH);
	pEnd = _tcsrchr( szFullFileName, _T('\\')) + 1;
	if (!pEnd)
	{
		return FALSE;	
	}
	*pEnd = _T('\0');

	_stprintf(g_szIniPath, _T("%s"), szFullFileName);				
	_stprintf(g_szHShieldPath, _T("%s\\HShield"), szFullFileName);	// HShield-Path ����
	_tcscat(szFullFileName, _T("HShield\\EhSvc.dll"));				// EHSVC.DLL =Path ����.
	_tcscat(g_szIniPath, _T("MiniAEnv.INI"));						// MiniAEnv.ini Path ����

	// INI���Ϸκ��� ȯ�������� ����(Ȯ�弭������ ��� ����, ���� IP, ���� PORT)
	GetEnvInfoFromIni();

	// HS-Monitoring ���� ����� ���� ����ü ����
	AHNHS_EXT_ERRORINFO HsExtError;
	// ���̵� ����͸� �ּ�, ���ӹ������� ��ȣȭ�� ���ڵ� �Ͽ� �����Ͽ��ٰ� ���� �Ʒ� �Լ��� �Է½�
	// ��ȣȭ�� ���ڵ� �Ͽ� �Է��ϴ� ���� ����.
	sprintf( HsExtError.szServer, "%s", "210.121.169.209" );		// ����͸� �����ּ�
	sprintf( HsExtError.szUserId, "%s", "GameUser" );				// ���� ���̵�
	// _AhnHS_Initialize������ ���̵� ���� �� ���� ��쿡�� �� ��Ʈ���� �Է��Ѵ�.
	// ���̵� ��� ������ _AhnHS_SetUserId�� ȣ���Ͽ� ���̵� �����Ѵ�.
	// sprintf( HsExtError.szUserId, "%s", "" ); 
	sprintf( HsExtError.szGameVersion, "%s", "1.0.0.1" );			// ���� ����

	// -------------------------------------------------------------------------
	// [_AhnHS_StartMonitor]
	// -------------------------------------------------------------------------
	// - DESCRIPTION: 
	//	  �ٽ��� ����͸� ���񽺸� �����մϴ�.
	// - SYNTAX:
	//	   EHSVC_API int __stdcall _AhnHS_StartMonitor(AHNHS_EXT_ERRORINFO HsExtError, LPCSTR szFullFileName)
	//
	//	  .  AHNHS_EXT_ERRORINFO HsExtErrorInfo		: AHNHS_EXT_ERRORINFO HsExtErrorInfo :  ����͸� ���� �Է� ������
    //						                          - ���� URL�ּ�. ( "123.456.789.123" or "www.monitor.com" )
	//												  - User ID		  ( "User1" )
	//												  - Game Version  ( "5.0.2.1" )
	//	  .  szFullFileName							: EHSVC.DLL ��ü ���
	//
	// - RETURN VALUE:
	//     . HS_ERR_OK          : ����
	//     . ��Ÿ				: ��Ÿ ���ϰ��� ���ؼ��� �ٽ��� ���α׷��� ���̵� ����ٶ�.


	//
	nRet = _AhnHS_StartMonitor( HsExtError, szFullFileName );

	if (nRet != HS_ERR_OK)
	{
		_stprintf(szMsg, _T("����͸� ��ɿ� ������ �߻��Ͽ����ϴ�.(Error Code = %x)\n���α׷��� �����մϴ�."), nRet);
		MessageBox(NULL, szMsg, szTitle, MB_OK);
		return FALSE;
	}

	// -------------------------------------------------------------------------
	// [_AhnHS_Initialize]
	// -------------------------------------------------------------------------
	// - DESCRIPTION: 
	//	  �ʱ�ȭ �Լ� ȣ�� 
	//    HackShield�� �ʱ�ȭ�ϰ�, �ɼ��� �����մϴ�. 
	//	  ���α׷��� �ʱ�ȭ�� �� �� ���� ȣ���� �� �ֽ��ϴ�. 
	//    �ٸ� ���� ���α׷����� HackShield�� ����ϰ� �ְų� ���񽺰� ������������ 
	//    ����Ǿ��� ��� ������ �߻��� �� �ֽ��ϴ�. 
	//
	// - SYNTAX:
	//	   EHSVC_API int __stdcall _AhnHS_Initialize(const char* szFileName,	PFN_AhnHS_Callback pfn_Callback, 
	//												int nGameCode, const char* szLicenseKey, DWORD dwOption,
	//												UINT unAdditionalRatio)
	//	  . szFileName			: EHSvc.dll�� ��ü ���
	//	  . Pfn_Callback		: Callback �Լ��� ������
	//    . nGameCode			: 4�ڸ� ����	�� ���ӿ� �ش��ϴ� ���� ID �ڵ�
	//    . szLicenseKe			: 24�ڸ� ���ڿ�	�� ���ӿ� �ش��ϴ� ���̼��� Ű
	//    . dwOption			: �ʱ�ȭ �ɼ� ����
	//    . unAdditionalRatio : �߰����� ���� �ɼ� ���� (���ǵ� �� ������ ���� , Self-Destruction �ð� ����)
	//
	// - OPTIONS:
	//   �ٽ��� ���α׷��� ���̵� ����ٶ�.
	//
	// - RETURN VALUE:
	//     . HS_ERR_OK          : ����
	//     . ��Ÿ				: ��Ÿ ���ϰ��� ���ؼ��� �ٽ��� ���α׷��� ���̵� ����ٶ�.

	
	nRet = _AhnHS_Initialize(szFullFileName, AhnHS_Callback, 
		1000, "B228F2916A48AC24", 
		AHNHS_CHKOPT_ALL,
		AHNHS_SPEEDHACK_SENSING_RATIO_NORMAL|AHNHS_SELFDESTRUCTION_RATIO_NORMAL);

	//�Ʒ� ������ ���߰��������� �߻��� �� ������ 
	//���� ���� �߻��ؼ��� �ȵǴ� �����̹Ƿ� assertó���� �߽��ϴ�.
	assert(nRet != HS_ERR_INVALID_PARAM);
	assert(nRet != HS_ERR_INVALID_LICENSE);
	assert(nRet != HS_ERR_ALREADY_INITIALIZED);

	if (nRet != HS_ERR_OK) 
	{
		//Error ó�� 
		switch(nRet)
		{
			case HS_ERR_ANOTHER_SERVICE_RUNNING:
			{
				MessageBox(NULL, _T("�ٸ� ������ �������Դϴ�.\n���α׷��� �����մϴ�."), szTitle, MB_OK);
				break;
			}
			case HS_ERR_INVALID_FILES:
			{
				MessageBox(NULL, _T("�߸��� ���� ��ġ�Ǿ����ϴ�.\n���α׷��� �缳ġ�Ͻñ� �ٶ��ϴ�."), szTitle, MB_OK);
				break;
			}
			case HS_ERR_DEBUGGER_DETECT:
			{
				MessageBox(NULL, _T("��ǻ�Ϳ��� ����� ������ �����Ǿ����ϴ�.\n������� ������ ������Ų �ڿ� �ٽ� ��������ֽñ�ٶ��ϴ�."), szTitle, MB_OK);
				break;
			}
			case HS_ERR_NEED_ADMIN_RIGHTS:
			{
				MessageBox(NULL, _T("Admin �������� ����Ǿ�� �մϴ�.\n���α׷��� �����մϴ�."), szTitle, MB_OK);
				break;
			}
			case HS_ERR_COMPATIBILITY_MODE_RUNNING:
			{
				MessageBox(NULL, _T("ȣȯ�� ���� ���α׷��� �������Դϴ�.\n���α׷��� �����մϴ�."), szTitle, MB_OK);
				break;				
			}
			default:
			{
				_stprintf(szMsg, _T("��ŷ���� ��ɿ� ������ �߻��Ͽ����ϴ�.(Error Code = %x)\n���α׷��� �����մϴ�."), nRet);
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
	//	   ��ŷ �� Ž�� ��ɰ� ��ŷ ���� ����� ���۽�ŵ�ϴ�. 
	//	   _AhnHS_Initialize �Լ��� ȣ���� ������ ȣ���ؾ� �ϸ�, �ߺ� ȣ���� �� �����ϴ�. 
	//	   _AhnHS_StopService �Լ��� ȣ���Ͽ� ���񽺸� ������ ��쿡��, �� �Լ��� �ٽ� 
	//	   ȣ���Ͽ� ���񽺸� �ٽ� ���۽�ų �� �ֽ��ϴ�.
	//
	// - SYNTAX:
	//	  EHSVC_API int __stdcall _AhnHS_StartService( );
	//
	// - RETURN VALUE:
	//    . HS_ERR_OK          : ����
	//    . ��Ÿ				: ��Ÿ ���ϰ��� ���ؼ��� �ٽ��� ���α׷��� ���̵� ����ٶ�.

	nRet = _AhnHS_StartService();
	assert(nRet != HS_ERR_NOT_INITIALIZED);
	assert(nRet != HS_ERR_ALREADY_SERVICE_RUNNING);

	if (nRet != HS_ERR_OK)
	{
		_stprintf(szMsg, _T("��ŷ���� ��ɿ� ������ �߻��Ͽ����ϴ�.(Error Code = %x)\n���α׷��� �����մϴ�."), nRet);
		MessageBox(NULL, szMsg, szTitle, MB_OK);
		return FALSE;
	}
	

	// ���������� ���� Network ��� Thread�� ������.
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
	//	  ��ŷ ���� ��ɰ� ��ŷ �� Ž�� ����� ������ŵ�ϴ�.
	//
	// - SYNTAX:
	// 	  EHSVC_API int __stdcall _AhnHS_StopService( );
	//
	// - RETURN VALUE:
	//     . HS_ERR_OK					 : ����
	//	   . HS_ERR_NOT_INITIALIZED		 : _AhnHS_Initialize �Լ��� ȣ���Ͽ� HackShield�� �ʱ�ȭ���� ���� ���¿��� 
	//										ȣ���� ��� �߻��ϴ� �����Դϴ�. 
	//										�� ������ ���� ���������� �߻��� �� �ִ� ������ ������ ó���� ���� �ʾƵ� �˴ϴ�. (Value = 0x003)
	//     . HS_ERR_SERVICE_NOT_RUNNING  : _AhnHS_StartService �Լ��� ȣ���Ͽ� HackShield�� �������� ���� ���¿��� 
	//                                     ȣ���� ��� �߻��� �����Դϴ�. ���� ���������� �߻��� �� �ִ� ������ ������
	//                                      ó���� ���� �ʾƵ� �˴ϴ�. (Value = 0x301)

	nRet = _AhnHS_StopService();
	
	if (nRet != HS_ERR_OK)
	{
		_stprintf(szMsg, _T("_AhnHS_StopService ���� �߻� (Error Code = %x)"), nRet);
		MessageBox(NULL, szMsg, szTitle, MB_OK);
		return FALSE;
	}

	
	// -------------------------------------------------------------------------
	// [_AhnHS_Uninitialize]
	// -------------------------------------------------------------------------
	// - DESCRIPTION:
	//	 �ý��� ���������� ���Ǿ��� �޸𸮸� �����ϰ� ������ �ʱ�ȭ�մϴ�.
	//
	// - SYNTAX:
	// 	 EHSVC_API int __stdcall _AhnHS_Uninitialize( );
	//
	// - RETURN VALUE:
	//   . HS_ERR_OK				    : ����
	//	 . HS_ERR_SERVICE_STILL_RUNNING : _AhnHS_StopSerivice �Լ��� ȣ���Ͽ� HackShield�� �������� ���� ���¿��� 
	//                                   ȣ���ϸ� �߻��ϴ� �����Դϴ�. �� ������ ���� ���������� �߻��� �� �����Ƿ� 
	//                                   ������ ó���� ���� �ʾƵ� �˴ϴ�. (Value = 0x302)
	//   . HS_ERR_NOT_INITIALIZED	     : _AhnHS_Initialize �Լ� ȣ���� ���Ͽ� �ý����� �ʱ�ȭ���� ���� ���¿��� 
	//                                     ȣ���ϸ� �߻��ϴ� �����Դϴ�. �� ������ ���� ���������� �߻��� �� �����Ƿ� 
	//                                     ������ ó���� ���� �ʾƵ� �˴ϴ�. (Value = 0x003)


	nRet = _AhnHS_Uninitialize();

	if (nRet != HS_ERR_OK)
	{
		_stprintf(szMsg, _T("_AhnHS_Uninitialize ���� �߻� (Error Code = %x)"), nRet);
		MessageBox(NULL, szMsg, szTitle, MB_OK);
		return FALSE;
	}

	return msg.wParam;
}

//
void GetEnvInfoFromIni(void)
{
	// ���� ��Ʈ
	g_nServerPort = ::GetPrivateProfileInt(_T("ServerInfo"), _T("nPort"), 0, g_szIniPath);
	// ���� IP
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
	
	Sleep(10000);  // 10�ʵ��� ���..

	HANDLE hCurrentHandle = ::GetCurrentProcess();

	// ���ӿ��� ������� ���� �޸� ���� ���� �� Handle ���� 
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

	// ���ӿ��� ������� ���� �޸� ���� ���� �� Handle ���� 
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
			_stprintf(szMsg, _T("���� ��ġ���� ��ŷ���� �߰ߵǾ� ���α׷��� ������׽��ϴ�.\n%s"), (char*)pParam);
			ShowMsgAndExitWithTimer(szMsg);

			break;
		}
		//â��� ��ŷ�� ����
		case AHNHS_ENGINE_DETECT_WINDOWED_HACK:
		{
			ShowMsgAndExitWithTimer(_T("���α׷��� ���Ͽ� â��� ��ŷ���� �����Ǿ����ϴ�."));

			break;
		}
		//Speed ����
		case AHNHS_ACTAPC_DETECT_SPEEDHACK:
		{
			ShowMsgAndExitWithTimer(_T("���� �� PC���� SpeedHack���� �ǽɵǴ� ������ �����Ǿ����ϴ�."));
			break;
		}

		//����� ���� 
		case AHNHS_ACTAPC_DETECT_KDTRACE:	
		case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:
		{
			TCHAR szMsg[255];
			_stprintf(szMsg, _T("���α׷��� ���Ͽ� ����� �õ��� �߻��Ͽ����ϴ�. (Code = %x)\n���α׷��� �����մϴ�."), lCode);
			ShowMsgAndExitWithTimer(szMsg);
			break;
		}
		
		case AHNHS_ACTAPC_DETECT_AUTOMACRO:
		{
			TCHAR szMsg[255];
			_stprintf(szMsg, _T("��ũ�� ������� �ǽɵǴ� ������ �����Ǿ����ϴ�.. (Code = %x)\n���α׷��� �����մϴ�."), lCode);
			ShowMsgAndExitWithTimer(szMsg);

			break;
		}

		// �ڵ� ��ġ ����
		case AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS:
		{
			TCHAR szMsg[255];
			_stprintf(szMsg, _T("���� ��ġ���� �޸� ������ �����Ǿ� ���α׷��� ������׽��ϴ�.\n%s"), (char*)pParam);
			ShowMsgAndExitWithTimer(szMsg);
			break;
		}

		//�׿� ��ŷ ���� ��� �̻� 
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
			_stprintf(szMsg, _T("��ŷ ��� ��ɿ� �̻��� �߻��Ͽ����ϴ�. (Code = %x)\n���α׷��� �����մϴ�."), lCode);
			ShowMsgAndExitWithTimer(szMsg);
			break;
		}
	}
	return 1;
}


#define		OP_LOGON				0x1		// Logon�� ���� Notify
#define		OP_LOGOFF				0x2		// Logoff�� ���� Notify
#define		OP_REQGUID				0x3		// GUID Request Message
#define		OP_ACKGUID				0x4		// GUID Ack Message
#define		OP_REQCRC				0x5		// CRC Request Message
#define		OP_ACKCRC				0x6		// CRC Ack Message
#define		OP_ERROR				0x7		// Error Message

// �������� ���(��������)�� ���� Thread ���ν���.
UINT WINAPI NetworkThreadProcClient ( LPVOID lParam )
{
	int nErrCode = 0;	
	SOCKET socket = 0;
	MSGHEADER header = { 0, };
	DWORD dwRet = ERROR_SUCCESS;

	// ���� DLL�� �ε��Ѵ�.
	dwRet = BOANET_LoadWinsock2 ();
	if ( dwRet != ERROR_SUCCESS )
		goto _end;

	// Network���� �ʱ�ȭ �۾��� �����Ѵ�.
	dwRet = BOANET_InitializeCli ( &socket, &nErrCode );
	if ( dwRet != ERROR_SUCCESS )
	{
		BOANET_UnloadWinsock2 ();
		goto _end;
	}

	// ������ �����Ѵ�.
	dwRet = BOANET_ConnectCli ( socket, g_nServerPort, ( LPTSTR ) ( LPCTSTR ) g_szServerIP, &nErrCode );
	if ( dwRet != ERROR_SUCCESS )
	{
		BOANET_UnloadWinsock2 ();
		goto _end;
	}

	// Logon Msg�� ������.
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

		// 1. Header�� �о���δ�.
		dwRet = BOANET_ReceiveCli ( socket, ( PSTR ) &header, sizeof ( MSGHEADER ), &nErrCode );
		if ( dwRet != ERROR_SUCCESS )
		{
			BOANET_CloseCli ( &socket );
			BOANET_UnloadWinsock2 ();
			goto _end;
		}

		// ���� ��Ŷ�� ��� ����
		if ( header.opCode == OP_ERROR )
		{
			dwRet = ntohl ( header.length );
			break;
		}

		// 2. Body�� �޴´�.		
		dwRet = BOANET_ReceiveCli ( socket, ( PSTR ) byReqMsg, ntohl ( header.length ), &nErrCode );
		if ( dwRet != ERROR_SUCCESS )
		{
			BOANET_CloseCli ( &socket );
			BOANET_UnloadWinsock2 ();
			goto _end;
		}

		// 3. Msg�� ó���� �Ѵ�.

		// -------------------------------------------------------------------------
		// [_AhnHS_MakeResponse]
		// -------------------------------------------------------------------------
		// - DESCRIPTION: 
		//	  Ŭ���̾�Ʈ���� ����մϴ�. �������� ������ ��ȣȭ�� ���� �䱸 �޽����� 
		//	  ��ȣȭ�ϰ� ���� Ŭ���̾�Ʈ ������ ������ ��ȣȭ�Ͽ� ���� �޽����� ����ϴ�.
		//
		// - SYNTAX:
		//		int __stdcall _AhnHS_MakeResponse (unsigned char *pbyRequest, unsigned long ulRequestLength, 
		//									PAHNHS_TRANS_BUFFER pResponseBuffer);
		// - PARAMETERS:
		//		.pbyRequest	unsigned char *				:[IN]	��û �޽��� ����
		//		.ulRequestLength	unsigned long		:[IN]	��û �޽��� ����
		//		.pResponseBuffer	PAHNHS_TRANS_BUFFER	:[OUT]  ���� �޽��� ����
		//
		// - RETURN VALUE:
		//     . ERROR_SUCCESS      : ����
		//     . ��Ÿ				: ��Ÿ ���ϰ��� ���ؼ��� �ٽ��� ���α׷��� ���̵� ����ٶ�.
		dwRet = _AhnHS_MakeResponse( byReqMsg, ntohl ( header.length ), &TransBuffer );
		if ( dwRet != ERROR_SUCCESS )
		{
			BOANET_CloseCli ( &socket );
			BOANET_UnloadWinsock2 ();
			goto _end;
		}

		TRACE("[%x] = _AhnHS_MakeResponse()", dwRet);

		// Header�� �ۼ��Ѵ�.
		header.opCode = OP_ACKCRC;
		header.length = htonl ( TransBuffer.nLength );



		// 4. Header�� ������.
		dwRet = BOANET_SendCli ( socket,  ( PSTR ) &header, sizeof ( MSGHEADER ), &nErrCode );
		if ( dwRet != ERROR_SUCCESS )
		{
			BOANET_CloseCli ( &socket );
			BOANET_UnloadWinsock2 ();
			goto _end;
		}

		// 5. Body�� ������.
		// [Ȯ�� ��������] TransBuffer �� ����Ʈ �迭�� ����Ѵ�.
		pbyResponse = TransBuffer.byBuffer;
		dwRet = BOANET_SendCli ( socket,  ( PSTR ) pbyResponse, ntohl ( header.length ), &nErrCode );
		if ( dwRet != ERROR_SUCCESS )
		{
			BOANET_CloseCli ( &socket );
			BOANET_UnloadWinsock2 ();
			goto _end;
		}
	}

	// Logoff Msg�� ������.
	header.opCode = OP_LOGOFF;
	header.length = htonl ( 0 );

	BOANET_SendCli ( socket,  ( PSTR ) &header, sizeof ( MSGHEADER ), &nErrCode );

	// ������ ������ ���´�.
	BOANET_CloseCli ( &socket );

	// ���� DLL�� ��ε��Ѵ�.	
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

// �ϱ�� ���� ���·� �Լ��� �����Ͽ� �ٽ��� ������Ʈ ������ ȣ���մϴ�.
void OnAhnHS_HSUpdate()
{
	// -------------------------------------------------------------------------
	// [_AhnHS_HSUpdate]
	// -------------------------------------------------------------------------
	// - DESCRIPTION: 
	//	 HackShield ���� ���ϵ��� ������Ʈ�մϴ�.
	//
	// - SYNTAX:
	//	 DWORD __stdcall _AhnHS_HSUpdate( LPCTSTR szUpdateDir, DWORD dwTimeOut );
	//	
	// - PARAMETERS:
	//	 .szUpdateDir				������Ʈ ������ ��ġ�Ǿ� �ִ� ����
	//	 .dwTimeOut	milliseconds	������Ʈ �� Ÿ�� �ƿ� �ð�. 0���� ���� �� INFINITE���� ����

	// - RETURN VALUE:
	//			. HACKSHIELD_ERROR_SUCESS	: ������Ʈ ���� (Value = 0x00000000)
	//          . ��Ÿ						: ��Ÿ ���ϰ��� ���ؼ��� �ٽ��� ���α׷��� ���̵� ����ٶ�.

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
