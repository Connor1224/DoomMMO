#include "r3dPCH.h"
#include "r3d.h"
#include "r3dDebug.h"
#include "CCRC32.h"
static unsigned antihackNotFound = 1;
static unsigned antihackWarNotFound = 1;
#include <ShellAPI.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <DbgHelp.h>
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib, "version.lib")  // for "VerQueryValue"
#include <string>
#include <iostream>   
#include "StackWalker.h"
//#include "ProcViewer/ProcessViewer.h"
#include "../SF/Console/EngineConsole.h"
#ifdef FINAL_BUILD
#include "resource.h"
#include "HShield.h"
#include "HSUpChk.h"
#pragma comment(lib,"HShield.lib")
#pragma comment(lib,"HSUpChk.lib")
#include <assert.h>
#include <stdio.h>
#include <winsock2.h>
#include <process.h>
#endif
typedef void (*HackCheck)();
typedef LONG    NTSTATUS;
//extern __declspec(dllimport) int __stdcall WG_StopInject1();
//extern __declspec(dllimport) int __stdcall WG_StartD3D();
//extern __declspec(dllimport) int __stdcall WG_StopD3D();
//extern __declspec(dllimport) int __stdcall WG_StartVirtualMem(int power);
typedef NTSTATUS (WINAPI *pNtQIT)(HANDLE, LONG, PVOID, ULONG, PULONG);

#define STATUS_SUCCESS    ((NTSTATUS)0x000 00000L)
// Global Variables:
HINSTANCE hInst;											// current instance
TCHAR szTitle[500];								// The title bar text
TCHAR szWindowClass[500];						// The title bar text
DWORD g_dwMainThreadID;
TCHAR	g_szHShieldPath[MAX_PATH] = {0,};
TCHAR	g_szIniPath[MAX_PATH] = {0,};
int		g_nServerPort = 0;
char	g_szServerIP[20] = {0,};
#define ThreadQuerySetWin32StartAddress 9
char __r3dCmdLine[1024];
int __stdcall AhnHS_Callback(long lCode, long lParamSize, void* pParam);
typedef bool (*Win32MsgProc_fn)(UINT uMsg, WPARAM wParam, LPARAM lParam);

//hack: add temp handler for external message processing

namespace
{
	const int		NUM_MSG_PROC_MAX = 8;
	Win32MsgProc_fn	r3dApp_MsgProc3[NUM_MSG_PROC_MAX] = {0};
}

static	int		StartWinHeight = 300;
static	int		StartWinWidth  = 300;
static	int		curDispWidth   = 0;
static	int		curDispHeight  = 0;

namespace win
{
	HINSTANCE	hInstance  = NULL;
	HWND		hWnd       = NULL;
	const char 	*szWinName = "$o, yeah!$";
	HICON		hWinIcon   = 0;
	int		bSuspended	= 0;
	int		bNeedRender	= 1;

	void		HandleActivate();
	void		HandleDeactivate();
	void		HandleMinimize();
	void		HandleRestore();

	int		Init();
}

void RegisterMsgProc (Win32MsgProc_fn proc)
{
	for (int i = 0; i<NUM_MSG_PROC_MAX; i++ )
	{
		if (!r3dApp_MsgProc3[i])
		{
			r3dApp_MsgProc3[i] = proc;
			return;
		}
	}
	r3d_assert ( 0 && "RegisterMsgProc error; register more than NUM_MSG_PROC_MAX MsgProc." );
}

void UnregisterMsgProc (Win32MsgProc_fn proc)
{
	for (int i = 0; i<NUM_MSG_PROC_MAX; i++ )
	{
		if (r3dApp_MsgProc3[i] == proc)
		{
			r3dApp_MsgProc3[i] = NULL;
			return;
		}
	}
	r3d_assert ( 0 && "UnregisterMsgProc error." );
}

#define INPUT_KBD_STACK	32
volatile static  int   input_ScanStack[INPUT_KBD_STACK + 2];
volatile static  int   * volatile input_StackHead = input_ScanStack;
volatile static  int   * volatile input_StackTail = input_ScanStack;


bool g_bExit = false;

bool IsNeedExit()
{
	return g_bExit;
}

int win::input_Flush()
{
	input_StackHead = input_ScanStack;
	input_StackTail = input_ScanStack;

	return 1;
}

int win::kbhit()
{
	if(input_StackHead == input_StackTail)
		return 0;
	else
		return 1;
}

int win::getch( bool bPop )
{
	int	ch;

	if(!win::kbhit())
		return 0;

	ch = *input_StackTail;
	if ( bPop )
		input_StackTail++;

	if(input_StackTail >= input_ScanStack + INPUT_KBD_STACK)
		input_StackTail = input_ScanStack;

	return ch;
}


static 
void wnd__DrawLogo()
{
#if TSG_BITMAP
	HDC		hDC;
	HANDLE		hBmp;

	// load background image and center our window
	hBmp = LoadImage(
		GetModuleHandle(NULL), 
		MAKEINTRESOURCE(TSG_BITMAP),
		IMAGE_BITMAP,
		win__Width,
		win__Height,
		LR_DEFAULTCOLOR
		);
	if(hBmp == NULL) return;

	if(hDC = GetDC(win::hWnd))
	{
		HDC	dc;

		dc = CreateCompatibleDC(hDC);
		SelectObject(dc, hBmp);
		BitBlt(hDC, 0, 0, win__Width, win__Height, dc, 0, 0, SRCCOPY);
		DeleteDC(dc);

		ReleaseDC(win::hWnd, hDC);
	}

	DeleteObject(hBmp);

#endif

	return;
}

bool winMouseWasVisible = true;
extern bool r3dMouse_visible;
void win::HandleActivate()
{
	//  if(!win::bSuspended)
	//		return;

	win::bSuspended = 0;
	win::bNeedRender = 1;

	if(Mouse)    Mouse->SetCapture();
	if(Keyboard) Keyboard->ClearPressed();

	if( winMouseWasVisible || g_cursor_mode->GetInt() )
		r3dMouse::Show(true);
	else 
		r3dMouse::Hide(true);
}

void win::HandleDeactivate()
{
	//  if(win::bSuspended)
	//    return;
	win::bSuspended = 1;

	if( !(r_render_on_deactivation && r_render_on_deactivation->GetInt()) )
		win::bNeedRender = 0;
	else
		win::bNeedRender = 1;

	winMouseWasVisible = r3dMouse_visible;

	if(Mouse)    Mouse->ReleaseCapture();
	if(Keyboard) Keyboard->ClearPressed();
}

void win::HandleMinimize()
{
	win::bNeedRender = 0;
}

void win::HandleRestore()
{
	win::bNeedRender = 1;
}


//
//
//
// main Callback function
//
//

/*
namespace 
{
//
// some vars to simulate move moving by ourselves.
//

static bool bOnCaption = false;	// true if we're on caption
static bool bDragging  = true;	// true if we're dragging our window
static int  dragX      = 0;		// offset of drag point, relative to window origin
static int  dragY      = 0;		// offset of drag point, relative to window origin

static void dragDisable()
{
if(!bDragging) return;

ReleaseCapture();
bDragging = false;

return;
}  
};
*/

void (*OnDblClick)() = 0;

static 
LRESULT CALLBACK win__WndFunc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int userProcActive = 0;

	if(userProcActive) {
		// in very rare occasions we can reenter this function
		// one of cases is that when error is thrown from processing user or UI functions
		// so, in this case we'll enter to never ending loop
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	userProcActive = true;
	bool bAnyCompleted = false;
	for(int i = 0; i < NUM_MSG_PROC_MAX; i++ )
	{
		if ( r3dApp_MsgProc3 [i] )
			bAnyCompleted |= r3dApp_MsgProc3[i](uMsg, wParam, lParam);
	}
	userProcActive = false;
	if ( bAnyCompleted ) return 0;


	//r3dOutToLog("uMsg %x\n", uMsg);
	switch(uMsg) 
	{
	case WM_CLOSE:
		{
			r3dOutToLog("alt-f4 pressed\n");
			r3dOutToLog("...terminating application\n");

			ClipCursor(NULL);

			//HRESULT res = TerminateProcess(r3d_CurrentProcess, 0);

			g_bExit = true;
			return 0;
		}

	case WM_CONTEXTMENU:
		// disable context menu
		return 0;

	case WM_ENTERMENULOOP:
		r3dOutToLog("!!!warning!!! r3dApp entered to modal menu loop\n");
		break;

	case WM_ENTERSIZEMOVE:
		//r3d_assert(0 && "WM_ENTERSIZEMOVE");
		win::HandleDeactivate();
		return 0;

	case WM_EXITSIZEMOVE:
		r3dOutToLog("WM_EXITSIZEMOVE: %d\n", win::bSuspended);
		win::HandleActivate();
		break;

		/*
		case WM_CAPTURECHANGED:
		// if we lose capture from our window - disable dragging
		if(bDragging && (HWND)lParam != hWnd) {
		// note: do not call dragDisable(), because it will call ReleaseCapture()
		bDragging = false;
		break;
		}
		break;

		case WM_LBUTTONUP:
		{
		if(bDragging) {
		dragDisable();
		}
		break;
		}

		case WM_LBUTTONDOWN:
		{
		if(bOnCaption) {
		// start dragging our window, calc drag anchor point, relative to window corner
		POINT pp;        
		GetCursorPos(&pp);
		RECT rr;
		GetWindowRect(hWnd, &rr);

		dragX = pp.x - rr.left;
		dragY = pp.y - rr.top;

		::SetCapture(hWnd);
		bDragging = true;
		}

		break;
		}

		case WM_MOUSEMOVE:
		{
		if(bDragging) {
		POINT pp;        
		GetCursorPos(&pp);
		SetWindowPos(hWnd, 0, pp.x - dragX, pp.y - dragY, 0, 0, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOZORDER);
		}

		break;
		}

		case WM_NCHITTEST:
		{
		if(!r3dRenderer || !r3dRenderer->d3dpp.Windowed) 
		break;
		if(win::bSuspended)
		break;

		// if cursor is clipped somewhere, don't allow this window dragging at all.
		RECT clipRect;
		::GetClipCursor(&clipRect);
		if(clipRect.left != 0 || clipRect.top != 0 || clipRect.right != curDispWidth || clipRect.bottom != curDispHeight) {
		return HTCLIENT;
		}

		LONG hitTest = DefWindowProc(hWnd, uMsg, wParam, lParam);
		if(hitTest == HTCLIENT && bOnCaption) {
		if(Mouse) Mouse->Hide(); 
		if(Mouse) Mouse->SetCapture();

		bOnCaption = false;
		return hitTest;
		}

		if(hitTest == HTCAPTION && bOnCaption) {
		// return that we still in client area - so windows will not start windows moving by itself
		return HTCLIENT;
		}

		if(hitTest == HTCAPTION && !bOnCaption) {
		if(Mouse) Mouse->Show(); 
		if(Mouse) Mouse->ReleaseCapture();

		// we will simuate dragging by ourselves, otherwise game will be in modal dragging loop!
		bOnCaption = true;
		return HTCLIENT;
		}

		return hitTest;
		}
		*/    

		// disable menu calling when ALT pressed
	case WM_SYSCOMMAND:
		{
			// disable those system commands
			switch(wParam & 0xFFFF) {
	case 0xF093:		// system menu on caption bar
	case SC_KEYMENU:
	case SC_MOVE:
		return 0;
	case SC_MINIMIZE:
		win::HandleMinimize();
		break;
	case SC_RESTORE:
		win::HandleRestore();
		break;

			}

			r3dOutToLog("r3dapp: SysCmd: %x\n", wParam & 0xFFFF);
			break;
		}

	case WM_NCACTIVATE: 
		//dragDisable();

		if((wParam & 0xFFFF) == TRUE) 
			win::HandleActivate();
		break;

	case WM_ACTIVATE:
		//dragDisable();

		if((wParam & 0xFFFF) == WA_INACTIVE) {
			win::HandleDeactivate();
		} else {
			win::HandleActivate();
		}
		break;

	case WM_KEYDOWN:
		{
			EngineConsole::ProcessKey( wParam );
			break;
		}

	case WM_LBUTTONDBLCLK:
		if( OnDblClick )
			OnDblClick() ;
		break;

		// store char to input stream
	case WM_CHAR:
		{
			EngineConsole::ProcessChar( wParam );

			int	ch;

			ch              = (TCHAR)wParam;
			*(input_StackHead++) = ch;
			if(input_StackHead >= input_ScanStack + INPUT_KBD_STACK)
				input_StackHead = input_ScanStack;
			break;
		}

	case WM_PAINT:
		{
			HDC         hDC;
			PAINTSTRUCT ps;

			hDC = BeginPaint(hWnd,&ps);
			wnd__DrawLogo();
			EndPaint(hWnd,&ps);
			break;
		}

	case WM_DESTROY:
		//PostQuitMessage (0);
		g_bExit = true;
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void r3dWinStyleModify(HWND hWnd, int add, DWORD style)
{
	if(add)
		SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(win::hWnd, GWL_STYLE) | style);
	else
		SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(win::hWnd, GWL_STYLE) & ~style);
}
void Checkpro()
{
	Sleep(100);
	if (FindWindow("#32770",NULL) != NULL)
		TerminateProcess(GetCurrentProcess(),-1);
}
#ifdef FINAL_BUILD
int getRand()
{
	int getal;
	srand(time(NULL));

	getal = rand() % 100;

	return getal;
}
#endif
BOOL ListProcessModules( DWORD dwPID ) 
{ 
	HANDLE hModuleSnap = INVALID_HANDLE_VALUE; 
	MODULEENTRY32 me32; 

	//  Take a snapshot of all modules in the specified process. 
	hModuleSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, dwPID ); 
	if( hModuleSnap == INVALID_HANDLE_VALUE ) 
	{ 
		//    printError( TEXT("CreateToolhelp32Snapshot (of modules)") ); 
		return( FALSE ); 
	} 

	//  Set the size of the structure before using it. 
	me32.dwSize = sizeof( MODULEENTRY32 ); 

	//  Retrieve information about the first module, 
	//  and exit if unsuccessful 
	if( !Module32First( hModuleSnap, &me32 ) ) 
	{ 
		// printError( TEXT("Module32First") );  // Show cause of failure 
		CloseHandle( hModuleSnap );     // Must clean up the snapshot object! 
		return( FALSE ); 
	} 

	//  Now walk the module list of the process, 
	//  and display information about each module 
	do 
	{ 
		/*r3dOutToLog("MODULE NAME:     %s\n",             me32.szModule ); 
		r3dOutToLog("\n     executable     = %s",             me32.szExePath ); 
		r3dOutToLog("\n     process ID     = 0x%08X",         me32.th32ProcessID ); 
		r3dOutToLog("\n     ref count (g)  =     0x%04X",     me32.GlblcntUsage ); 
		r3dOutToLog("\n     ref count (p)  =     0x%04X",     me32.ProccntUsage ); 
		r3dOutToLog("\n     base address   = 0x%08X", (DWORD) me32.modBaseAddr ); 
		r3dOutToLog("\n     base size      = %d\n",             me32.modBaseSize ); */
		std::string str (me32.szModule);
		std::string str2 (".tmp");
		std::size_t found = str.find(str2);
		if (found != -1)
		{
			r3dOutToLog("MODULE NAME:     %s\n",             me32.szModule);
			TerminateProcess(GetCurrentProcess(),-1);
		}

	} while( Module32Next( hModuleSnap, &me32 ) ); 

	//  Do not forget to clean up the snapshot object. 
	CloseHandle( hModuleSnap ); 
	return( TRUE ); 
} 
void CheckUndifinedThread()
{
	::Sleep(10000);
	ListProcessModules(GetCurrentProcessId());
}
BOOL win::Init()
{
	static const char* szWinClassName = "r3dWin";

	r3d_assert(hInstance != NULL);

	WNDCLASS  wndclass;
	wndclass.style         = CS_DBLCLKS | CS_GLOBALCLASS;
	wndclass.lpfnWndProc   = win__WndFunc;		// window function
	wndclass.cbClsExtra    = 0;				// no extra count of bytes
	wndclass.cbWndExtra    = 0;				// no extra count of bytes
	wndclass.hInstance     = GetModuleHandle(NULL);	// this instance
	wndclass.hIcon         = (hWinIcon) ? hWinIcon : LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = szWinClassName;
	RegisterClass(&wndclass);

	HDC		disp_dc;

	disp_dc  = CreateIC("DISPLAY", NULL, NULL, NULL);
	curDispWidth  = GetDeviceCaps(disp_dc, HORZRES);
	curDispHeight = GetDeviceCaps(disp_dc, VERTRES);
	//disp_bpp = GetDeviceCaps(disp_dc, BITSPIXEL);
	DeleteDC(disp_dc);

	hWnd = CreateWindow(
		/* window class name */       szWinClassName,            			
		/* window caption*/           szWinName,					
		/* window style */            WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		/* initial x position */      (curDispWidth - StartWinWidth) / 2,		
		/* initial y position */      (curDispHeight - StartWinHeight) / 2,		
		/* initial x size */          StartWinWidth,				
		/* initial y size */          StartWinHeight,				
		/* parent window handle */    NULL,                 			
		/* window menu handle*/       NULL,                 			
		/* program instance handle */ GetModuleHandle(NULL), 			
		/* creation parameters */     NULL);                 			

	if(!hWnd) {
		MessageBox(GetActiveWindow(), "Window Creation Failed", "Error", MB_OK);
		return FALSE;
	}

	/*
	//WinStyleModify(0, WS_SYSMENU);
	WinStyleModify(0, WS_BORDER);
	WinStyleModify(0, WS_CAPTION);
	WinStyleModify(1, WS_DLGFRAME);
	*/

	// set icon
	::SendMessage(hWnd, WM_SETICON, TRUE,  (LPARAM)wndclass.hIcon);
	::SendMessage(hWnd, WM_SETICON, FALSE, (LPARAM)wndclass.hIcon);

	r3dWinStyleModify(hWnd, 0, WS_THICKFRAME);	// prevent resize
	r3dWinStyleModify(hWnd, 0, WS_MAXIMIZEBOX);	// prevent maximize
	//r3dWinStyleModify(hWnd, 0, WS_MINIMIZEBOX);	// prevent minimize
#ifdef FINAL_BUILD
	if (antihackNotFound) {
		MessageBox(GetActiveWindow(), "Can't Start GayGuard System", "AntiHack Error", MB_OK);
		ExitProcess(0);
	}

	if (antihackWarNotFound) {
		MessageBox(GetActiveWindow(), "Can't Start WarGuard System", "AntiHack Error", MB_OK);
		ExitProcess(0);
	}

	 //CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(CheckUndifinedThread),NULL,0,0);
#endif

	ShowWindow(win::hWnd, FALSE);
	wnd__DrawLogo();
	InvalidateRect(win::hWnd, NULL, FALSE);
	UpdateWindow(win::hWnd);
	SetFocus(win::hWnd);
	/*HMODULE hMod;
	hMod = LoadLibrary(TEXT("WarGuard.dll"));
	HackCheck _HackCheck;
	_HackCheck = (HackCheck)GetProcAddress(hMod,"WG_UnHookD3D");
	_HackCheck();
	_HackCheck = (HackCheck)GetProcAddress(hMod,"WG_StartWarZTH");
	_HackCheck();*/

	return TRUE;
}



int win::ProcessSuspended()
{
	if(!bSuspended)
		return FALSE;

	MSG msg;
	while(PeekMessage(&msg, NULL,0,0,PM_NOREMOVE))
	{
		if(!GetMessage (&msg, NULL, 0, 0)) 
			return 1;

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return TRUE;
}
//---------------------------------------------------------------
typedef void( WINAPI * PGNSI)(void);
void LoadDllAntHack()
{
HINSTANCE MyDll = LoadLibraryA("WZGLib.dll");
PGNSI pGNSI;
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG00");//0
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG01");//1
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG02");//2
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG03");//3
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG04");//4
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG05");//5
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG06");//6
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG07");//7
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG08");//8
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG09");//9
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG10");//10
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG11");//11
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG12");//12
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG13");//13
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG14");//14
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG15");//15
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG16");//16
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG17");//17
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG18");//18
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG19");//19
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG20");//20
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG21");//21
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG22");//22
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG23");//23
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG24");//24
pGNSI();
pGNSI = (PGNSI)GetProcAddress(MyDll,"WZG25");//25
pGNSI();
}

//---------------------------------------------------------------
void Hide_Scanner()
{
	HWND hWnd;
	hWnd = FindWindow(0,0);
	if ( hWnd > 0 && GetParent(hWnd) == 0)
	{    
		Sleep (5000);
		ExitProcess(0);
	}
}


void Hide_Scan(){
again:
	Hide_Scanner();
	Sleep(1000);
	goto again;
}

void DetectHide(){
	//CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(Hide_Scan),NULL,0,0);
	//Hide_Scanner();
}  

void ClasseWindow(LPCSTR WindowClasse){
	HWND WinClasse = FindWindowExA(NULL,NULL,WindowClasse,NULL);
	if( WinClasse > 0)
	{
		Sleep(5000); 
		ExitProcess(0);     
	}
}

void ClasseCheckWindow(){    
	//ClasseWindow("ConsoleWindowClass");   
	//ClasseWindow("ThunderRT6FormDC");   
	ClasseWindow("PROCEXPL");            
	ClasseWindow("ProcessHacker");      
	ClasseWindow("PhTreeNew");           
	ClasseWindow("SysListView32");       
	ClasseWindow("TformSettings");
	ClasseWindow("TWildProxyMain");
	ClasseWindow("TUserdefinedform");
	ClasseWindow("TformAddressChange");
	ClasseWindow("TMemoryBrowser");
	ClasseWindow("TFoundCodeDialog"); 
}

void classescan(){
again:
	ClasseCheckWindow();
	Sleep(30000);
	goto again;
}

void protegerclasse(){
	//CreateThread(NULL,NULL,LPTHREAD_START_ROUTINE(classescan),NULL,0,0);
	//ClasseCheckWindow();
}  
static DWORD checkThread()
{
	for (;;) {
		for(int i=0; i<25000; i++)
		{
			//r3dOutToLog("%d\n",GetCurrentThreadId());
			HANDLE threadHandle = OpenThread( THREAD_ALL_ACCESS, FALSE, i );
			if( threadHandle != INVALID_HANDLE_VALUE ) {

			}
		}
	}
	// return 1;
}

#define qtdKey 6
static DWORD checkForForbiddenKeys(LPVOID args) // Allright Forbidden Keys System
{
	const WORD fbKeys[qtdKey] = {
		VK_INSERT,
		VK_DELETE,
		VK_HOME,
		VK_END,
		VK_PRIOR,
		VK_NEXT,
	};

	for (;;) {
		for (unsigned i = 0; i < qtdKey; i++) {
			if (GetAsyncKeyState(fbKeys[i]) & 0x8000)
				ExitProcess(0);
			//MessageBoxA(0,"Forbidden Key Detect!","Allright Forbidden Key Detect!",MB_OK | MB_ICONSTOP);
		}
		Sleep(10);
	}
}

HINSTANCE LoadME;

unsigned long* pInterface;
DWORD XORKEY1 = 0xF95BC105;
DWORD XORKEY2 = 0xC2FC0091;

DWORD EndSceneAddress = 0x0;
DWORD DrawIndexedPrimitiveAddress = 0x0;
DWORD PresentAddress = 0x0;

HANDLE  hThreadCRC = NULL;
HANDLE  hThreadDBG = NULL;

typedef unsigned int  uint;
typedef unsigned char BYTE;
struct Process_Data
{
	HANDLE hProc;
	uint StartofRegion;
	uint EndofRegion;
	uint Base;
};

#include <iostream>
#include <fstream>
using namespace std;

char buf1;
char buf2;
byte buf3;
byte buf4;

DWORD genrand(){
	DWORD buffer;
	char lowbyte;

	__asm
	{
		rdtsc
			mov dword ptr buffer, eax
			mov byte ptr lowbyte, al
	};

	buffer*=(DWORD)GetTickCount()*(DWORD)lowbyte;

	buffer|=0xf0000000;

	return buffer;
}

int crc()
{
	Process_Data pd;
	ofstream file;
	ofstream Offsets;
	ifstream read;
	uint offset=0;

	DWORD fileXOR = genrand();

	uint tempSize;
	MEMORY_BASIC_INFORMATION mbi;	

	tempSize= (DWORD)GetModuleHandle(NULL) + 0x1000; //pe header

	pd.StartofRegion=(DWORD)GetModuleHandle(NULL);

	VirtualQuery((LPVOID)tempSize,&mbi,sizeof(mbi));

	pd.EndofRegion=tempSize+(uint)mbi.RegionSize;

	file.open("WZ_05.bin",ios::out|ios::binary);

	for(uint i=pd.StartofRegion; i<pd.EndofRegion; i++)
	{
		char buffer;

		buffer = *(BYTE*)(i);
		buffer ^= fileXOR;
		file.put(buffer);
	}

	file.close();

	while(true){
		read.open("WZ_05.bin",ios::in|ios::binary);

		Sleep(10000);

		for(uint i=pd.StartofRegion; i<pd.EndofRegion; i++,offset++)
		{
			IsDebuggerPresent();

			read.get(buf1);

			buf2 = *(BYTE*)(i);
			buf2 ^= fileXOR;

			if((buf1)!= buf2)
			{
				DWORD addressB = (DWORD)GetProcAddress(GetModuleHandle("ntdll"),"NtRaiseException");
				__asm{
					mov ESP, 0
						jmp dword ptr addressB
				};
			}
		}
		read.close();
	}
	return 0;
}

bool handler_called;

ULONG excphandler()
{
	handler_called=true;

	return EXCEPTION_CONTINUE_EXECUTION;
}

void DoDebugLoop()
{
	/*AddVectoredExceptionHandler(1,(PVECTORED_EXCEPTION_HANDLER)excphandler);


	while(true)
	{
	Sleep(1000);

	handler_called=false;

	RaiseException(0x40010007,0,NULL,NULL);

	if(!handler_called)
	{
	DWORD addressB = (DWORD)GetProcAddress(GetModuleHandle("ntdll"),"NtRaiseException");
	__asm{
	mov ESP, 0
	jmp dword ptr addressB
	};
	}
	}*/

	while(true)
	{
		BOOL bExceptionHit = FALSE;
		__try {
			_asm
			{
				pushfd
					or dword ptr [esp], 0x100
					popfd
					// Set the Trap Flag
					// Load value into EFLAGS register
					nop
			}
		}
		__except(EXCEPTION_EXECUTE_HANDLER) {
			bExceptionHit = TRUE;

			// An exception has been raised ?
			// there is no debugger.
		}

		if (bExceptionHit == FALSE)
		{
			DWORD addressB = (DWORD)GetProcAddress(GetModuleHandle("ntdll"),"NtRaiseException");
			__asm{
				mov ESP, 0
					jmp dword ptr addressB
			};
		}

		Sleep(1000);
	}
}

HANDLE  hThreadAC = NULL;
#define DLL_CRC	0xD4882F2C	// CRC
static void startupFunc(DWORD in)
{
	CCRC32 crc;
	//  in = in;
#ifdef FINAL_BUILD
	//DetectHide();
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ClasseWindow, NULL, 0, NULL); // Start Thread Allright Forbidden Keys
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) ClasseCheckWindow, NULL, 0, NULL); // Start Thread Allright Forbidden Keys
	//r3dOutToLog("AllrightSystem : HideProcess Start\n");
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) checkForForbiddenKeys, NULL, 0, NULL); // Start Thread Allright Forbidden Keys
	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) checkProcess, NULL, 0, NULL); // Start Thread Allright Forbidden Keys
	// CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) checkThreadCount, NULL, 0, NULL); // Start Thread Allright Forbidden Keys
	//	CProcessViewerApp::InitInstance();
	//r3dOutToLog("%d",(int)g_BreakPointThreads.Count());
	//r3dOutToLog("AllrightSystem : checkForForbiddenKeys Start\n");
	//protegerclasse();
	//ClasseCheckWindow();  
	//r3dOutToLog("AllrightSystem : CheckWindowsClass Start\n");
#endif

	game::PreInit();
//---------------------------------------WARZHACK--------------------------------------
  LoadLibrary("WZGLib.dll");

//-----------------------------------------------------------------------------
	/*#ifdef FINAL_BUILD
	if (LoadLibrary(TEXT("DirectX.dll")) != NULL) {
	unsigned long myCrc = 0;
	if (!crc.FileCRC("DirectX.dll", &myCrc))
	antihackNotFound = 1;
	else {
	if (myCrc == DLL_CRC) {
	antihackNotFound = 0;
	} else {
	antihackNotFound = 1;
	}
	} 

	} else
	{
	antihackNotFound = 1;
	}
	#endif*/
	win::Init();

	game::Init();

	game::MainLoop();

	game::Shutdown();
}

//
//
// the One And Only - WinMain!
//
//
/*#ifdef FINAL_BUILD
void ShowMsgAndExitWithTimer(TCHAR *szMsg)
{
UINT nThreadID = 0;

//HANDLE hThread = ( HANDLE ) _beginthreadex ( NULL, 0, ShowMsgAndExitWithTimer_ThreadFunc, NULL, 0, &nThreadID );
//_AhnHS_StopService();
r3dError(szMsg);
TerminateProcess(r3d_CurrentProcess,0);
}
int __stdcall AhnHS_Callback(long lCode, long lParamSize, void* pParam)
{
//win::HsCallBack = lCode;
hs_callback->SetInt((int)lCode);
switch(lCode)
{
//Engine Callback
case AHNHS_ENGINE_DETECT_GAME_HACK:
{
TCHAR szMsg[255];
_stprintf(szMsg, _T("Game Hack found\n%s"), (char*)pParam);
ShowMsgAndExitWithTimer(szMsg);

break;
}
case AHNHS_ENGINE_DETECT_WINDOWED_HACK:
{
//ShowMsgAndExitWithTimer(_T("Windowed Hack found."));

break;
}
case AHNHS_ACTAPC_DETECT_SPEEDHACK:
{
ShowMsgAndExitWithTimer(_T("Speed Hack found."));
break;
}


case AHNHS_ACTAPC_DETECT_KDTRACE:	
case AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED:
{
TCHAR szMsg[255];
_stprintf(szMsg, _T("AHNHS_ACTAPC_DETECT_KDTRACE_CHANGED"), lCode);
ShowMsgAndExitWithTimer(szMsg);
break;
}

case AHNHS_ACTAPC_DETECT_AUTOMACRO:
{
TCHAR szMsg[255];
_stprintf(szMsg, _T("AHNHS_ACTAPC_DETECT_AUTOMACRO"), lCode);
ShowMsgAndExitWithTimer(szMsg);

break;
}

case AHNHS_ACTAPC_DETECT_ABNORMAL_FUNCTION_CALL:
{
TCHAR szMsg[255];
_stprintf(szMsg, _T("Detect Abnormal Memory Access\n%s"), (char*)pParam);
ShowMsgAndExitWithTimer(szMsg);
break;
}
case AHNHS_ACTAPC_DETECT_ABNORMAL_MEMORY_ACCESS:
{
TCHAR szMsg[255];
_stprintf(szMsg, _T("Detect Memory Access\n%s"), (char*)pParam);
ShowMsgAndExitWithTimer(szMsg);
break;
}


case AHNHS_ACTAPC_DETECT_AUTOMOUSE:
{
TCHAR szMsg[255];
_stprintf(szMsg, _T("HackShield DETECT_AUTOMOUSE."), lCode);
ShowMsgAndExitWithTimer(szMsg);
break;
}
case AHNHS_ACTAPC_DETECT_DRIVERFAILED:
{
TCHAR szMsg[255];
_stprintf(szMsg, _T("HackShield DETECT_DRIVERFAILED."), lCode);
ShowMsgAndExitWithTimer(szMsg);
break;
}
case AHNHS_ACTAPC_DETECT_ALREADYHOOKED:
{
TCHAR szMsg[255];
_stprintf(szMsg, _T("HackShield Detect D3D Hack2."));
ShowMsgAndExitWithTimer(szMsg);
break;
}
case AHNHS_ACTAPC_DETECT_HOOKFUNCTION:
{
TCHAR szMsg[255];
_stprintf(szMsg, _T("HackShield Detect D3D Hack.\n Module Name:%s"), ACTAPCPARAM_DETECT_HOOKFUNCTION().szModuleName);
ShowMsgAndExitWithTimer(szMsg);
break;
}
case AHNHS_ACTAPC_DETECT_MESSAGEHOOK:
{
TCHAR szMsg[255];
_stprintf(szMsg, _T("HackShield DETECT_MESSAGEHOOK."), lCode);
ShowMsgAndExitWithTimer(szMsg);
break;
}
case AHNHS_ACTAPC_DETECT_MODULE_CHANGE:
{
TCHAR szMsg[255];
_stprintf(szMsg, _T("HackShield DETECT_MODULE_CHANGE."), lCode);
ShowMsgAndExitWithTimer(szMsg);
break;
}
case AHNHS_ACTAPC_DETECT_ENGINEFAILED:
{
TCHAR szMsg[255];
_stprintf(szMsg, _T("HackShield DETECT_ENGINEFAILED."), lCode);
ShowMsgAndExitWithTimer(szMsg);
break;
}
case AHNHS_ACTAPC_DETECT_CODEMISMATCH:
{
TCHAR szMsg[255];
_stprintf(szMsg, _T("HackShield CODEMISMATCH."), lCode);
ShowMsgAndExitWithTimer(szMsg);
break;
}
case AHNHS_ACTAPC_DETECT_MEM_MODIFY_FROM_LMP:
case AHNHS_ACTAPC_DETECT_LMP_FAILED:
{
TCHAR szMsg[255];
_stprintf(szMsg, _T("HackShield Detect memory modify."), lCode);
ShowMsgAndExitWithTimer(szMsg);
break;
}
case AHNHS_ACTAPC_DETECT_ABNORMAL_HACKSHIELD_STATUS:
{
TCHAR szMsg[255];
if (lCode != HS_ERR_ALREADY_GAME_STARTED)
{
_stprintf(szMsg, _T("HackShield Service already started by other game"), lCode);
//ShowMsgAndExitWithTimer(szMsg);
break;
}
else
{
_stprintf(szMsg, _T("HackShield Service Error"), lCode);
//ShowMsgAndExitWithTimer(szMsg);
break;
}
}
case AHNHS_ACTAPC_DETECT_PROTECTSCREENFAILED:
{
TCHAR szMsg[255];
_stprintf(szMsg, _T("HackShield PROTECTSCREENFAILED."), lCode);
//ShowMsgAndExitWithTimer(szMsg);
break;
}
}
return 1;
}
#endif*/

BOOLEAN BlockAPI (HANDLE,CHAR *,CHAR *);
//void AntiInject ();

/****************/
#ifdef FINAL_BUILD

DWORD g_dwLoadLibraryAJMP;

/* HOOK FUNCTION */

DWORD WINAPI jumphook( DWORD AddressToPerformJump, DWORD AddressOfMyFunction, DWORD LenghOfTheAreaToPerformTheJump	)
{
	if( LenghOfTheAreaToPerformTheJump < 5 )
		return 0;

	DWORD RelativeJump, 
		NextInstructionAddress,
		Flag;

	if ( ! VirtualProtect((LPVOID)AddressToPerformJump, LenghOfTheAreaToPerformTheJump, PAGE_EXECUTE_READWRITE, &Flag) )
		return 0;

	NextInstructionAddress = AddressToPerformJump + LenghOfTheAreaToPerformTheJump;

	*(BYTE*)AddressToPerformJump = 0xE9;

	for( DWORD i = 5; i < LenghOfTheAreaToPerformTheJump; i++)
		*(BYTE*)(AddressToPerformJump+i) = 0x90;

	RelativeJump = AddressOfMyFunction - AddressToPerformJump - 0x5;

	*(DWORD*)(AddressToPerformJump + 0x1) = RelativeJump;

	VirtualProtect((LPVOID)AddressToPerformJump, LenghOfTheAreaToPerformTheJump, Flag, &Flag);

	return NextInstructionAddress; 
}

/* END HOOK FUNCTION */

HMODULE WINAPI hLoadLibraryA( LPCSTR lpLibFileName )
{	
	__asm
	{
		mov eax, dword ptr ss:[esp + 0x18]
		cmp dword ptr ds:[eax-0x12], 0x8B55FF8B
			je erro
	}


	if( lpLibFileName )
	{
		if( !strcmp( lpLibFileName, "twain_32.dll" ) )
			__asm jmp g_dwLoadLibraryAJMP
	}			

	return LoadLibraryExA( lpLibFileName, 0, 0 );

erro:

	/* dll injetada */


	ExitProcess( 0 );

	return 0;
}

void ZPerformHooks()
{
	g_dwLoadLibraryAJMP = (DWORD)GetModuleHandle( "kernel32" ) + 0x6E2A1;

	jumphook( (DWORD)LoadLibraryA, (DWORD)&hLoadLibraryA, 57 );
}
#endif
static float LastVirtualMemoryTime;
static bool WGVMOn;
#ifdef FINAL_BUILD
void WGVirtualMemoryThread()
{
	::Sleep(30000);
	if (WGVMOn)
	{
		game::WarGuardWorkers(2);
		WGVMOn = false;
		r3dOutToLog("WarGuard : Turn Off VirtualMemory.\n");	
	}
	/*::Sleep(30000);

	if (WGVMOn)
	{
	game::WarGuardWorkers(2);
	WGVMOn = false;
	r3dOutToLog("WarGuard : Turn Off VirtualMemory.\n");	
	}
	return;*/
	if ((r3dGetTime() - LastVirtualMemoryTime) > 15.0f)
	{
	WGVMOn = !WGVMOn;
	LastVirtualMemoryTime = r3dGetTime();
	if (WGVMOn)
	game::WarGuardWorkers(1);
	else
	game::WarGuardWorkers(2);
	}
}

BOOLEAN BlockAPI (HANDLE hProcess, CHAR *libName, CHAR *apiName) 
{ 
	CHAR pRet[]={0xC3}; 
	HINSTANCE hLib = NULL; 
	VOID *pAddr = NULL; 
	BOOL bRet = FALSE; 
	DWORD dwRet = 0; 

	hLib = LoadLibrary (libName); 
	if (hLib) { 
		pAddr = (VOID*)GetProcAddress (hLib, apiName); 
		if (pAddr) { 
			if (WriteProcessMemory (hProcess, 
				(LPVOID)pAddr, 
				(LPVOID)pRet, 
				sizeof (pRet), 
				&dwRet )) { 
					if (dwRet) { 
						bRet = TRUE; 
					} 
			} 
		} 
		FreeLibrary (hLib); 
	} 
	return bRet; 
}
#endif

int GetModule()
{
    HMODULE hMods[1024];
    HANDLE hProcess;
    DWORD cbNeeded;
    unsigned int i;

    // Print the process identifier.

   // r3dOutToLog( "\nProcess ID: %u\n", processID );

    // Get a handle to the process.

    hProcess = GetCurrentProcess()/*OpenProcess( PROCESS_QUERY_INFORMATION |
                            PROCESS_VM_READ,
                            FALSE, processID )*/;
    if (NULL == hProcess)
        return 1;

   // Get a list of all the modules in this process.

    if( EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
    {
        for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
        {
            TCHAR szModName[MAX_PATH];

            // Get the full path to the module's file.

            if ( GetModuleFileNameEx( hProcess, hMods[i], szModName,
                                      sizeof(szModName) / sizeof(TCHAR)))
            {
                // Print the module name and handle value.

                r3dOutToLog( TEXT("\t%s (0x%08X)\n"), szModName, hMods[i] );
            }
        }
    }
    
    // Release the handle to the process.

    CloseHandle( hProcess );

    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	/*VOID *pAddr = NULL; 
	VOID *hooks = NULL; 
	HINSTANCE hLib = NULL; 
	hLib = LoadLibrary ("Kernel32.dll"); 
	DWORD isc;
	pAddr = (VOID*)GetProcAddress (hLib, "LoadLibraryA"); 

	if (!ReadProcessMemory(GetCurrentProcess(), (LPVOID)pAddr,(LPVOID)&hooks,6,&isc))
	MessageBox(NULL, "ReadProcessMemory Failed", "", MB_OK);

	r3dOutToLog("ReadProcessMemory size %d\n",(int)isc);


	if (BlockAPI(GetCurrentProcess(),"Kernel32.dll","LoadLibraryA")/* && BlockAPI(GetCurrentProcess(),"Kernel32.dll","LoadLibraryW"))
	MessageBox(NULL, "BlockAPI Success", "", MB_OK);
	else
	MessageBox(NULL, "BlockAPI Failed", "", MB_OK);

	pAddr = GetProcAddress(GetModuleHandle("Kernel32.dll"), "LoadLibraryA");

	if (!WriteProcessMemory(GetCurrentProcess(),(LPVOID)pAddr,(LPVOID)hooks,sizeof(hooks),NULL));
	MessageBox(NULL, "WriteProcessMemory Failed", "", MB_OK);*/

	 
	//if (BlockAPI(GetCurrentProcess(),"Kernel32.dll","CreateRemoteThread")/* && BlockAPI(GetCurrentProcess(),"Kernel32.dll","LoadLibraryW")*/)
	//MessageBox(NULL, "BlockAPI Success", "", MB_OK);
#ifdef FINAL_BUILD
	//extern HMODULE WGPs;
	//HackCheck WGWorkers;

	//extern HMODULE WGMem;
	/*FILE* f = fopen("WarGuard.dll", "rb");
	if(f == NULL) {
	//MessageBox(NULL, "WarGuard not found", "", MB_OK);
	return false;
	}

	fseek(f, 0, SEEK_END);
	long size = ftell(f);
	fseek(f, 0, SEEK_SET);

	BYTE* data = new BYTE[size + 1];
	if(fread(data, 1, size, f) != size) {
	fclose(f);
	delete [] data;
	//MessageBox(NULL, "WarGuard fread", "", MB_OK);
	TerminateProcess(GetCurrentProcess(),-1);
	return 0;
	}

	DWORD crc32 = r3dCRC32(data, size);

	//r3dOutToLog("%x\n",crc32);
	if (crc32 != 0x5B5C2CE6)
	{
	//MessageBox(NULL, "WarGuard crc32", "", MB_OK);
	TerminateProcess(GetCurrentProcess(),-1);
	return false;
	}*/

	//WGMem = LoadLibrary(TEXT("WarGuard\\WGMemory.dll"));

	int value1 = getRand();
	int value2 = getRand();
	int value3 = getRand();

	//int cb = x578941xlskwijdsk(value1,value2,value3);
	//r3dOutToLog("%d %d\n",cb,value1*value2/value3*10);
	//if ( cb != value1*value2/value3*10)
	//TerminateProcess(GetCurrentProcess(),-1);



	TCHAR	*pEnd = NULL;
	TCHAR	szFullFileName[MAX_PATH] = { 0, };
	TCHAR	szMsg[255];
	int		nRet;

	g_dwMainThreadID = GetCurrentThreadId();
	LoadString(hInstance, 103, szTitle, 500);
	GetModuleFileName(NULL, szFullFileName, MAX_PATH);
	pEnd = _tcsrchr( szFullFileName, _T('\\')) + 1;
	if (!pEnd)
	{
		return FALSE;	
	}
	*pEnd = _T('\0');

	_stprintf(g_szIniPath, _T("%s"), szFullFileName);				
	_stprintf(g_szHShieldPath, _T("%s\\HShield"), szFullFileName);
	_tcscat(szFullFileName, _T("HShield\\EhSvc.dll"));			
	_tcscat(g_szIniPath, _T("MiniAEnv.INI"));		

	AHNHS_EXT_ERRORINFO HsExtError;
	sprintf( HsExtError.szServer, "%s", "127.0.0.1" );	
	sprintf( HsExtError.szUserId, "%s", "GameUser" );			
	sprintf( HsExtError.szGameVersion, "%s", "1.0.0.1" );

	DWORD dwResult ;
	/*dwResult = _AhnHS_HSUpdate(g_szHShieldPath, 1000 * 600);
	if (dwResult != 0)
	{
		_stprintf(szMsg, _T("HackShield Update Failed. Folder HShield or HSUpdate.exe is missing from your computer(Error Code = %d)."), dwResult);
		MessageBox(NULL, szMsg, szTitle, MB_OK);
		return FALSE;
	}*/

	/*if (WG_StopInject1() != 487587)
		TerminateProcess(GetCurrentProcess(),-1);*/

	dwResult = _AhnHS_HSUpdateEx(g_szHShieldPath, 1000 * 600,1000 ,AHNHSUPDATE_CHKOPT_HOSTFILE| AHNHSUPDATE_CHKOPT_GAMECODE,  HsExtError, 1000* 20 );

	nRet = _AhnHS_StartMonitor(HsExtError,g_szHShieldPath);
	//r3dOutToLog("_AhnHS_StartMonitor\n");

	if (nRet != HS_ERR_OK && nRet != 257 && nRet != 12159)
	{
		_stprintf(szMsg, _T("HackShield Load Error. Folder HShield is missing from your computer(Error Code = %d)."), nRet);
		MessageBox(NULL, szMsg, szTitle, MB_OK);
		return FALSE;
	}

	//WGWorkers = (HackCheck)GetProcAddress(WGPs,"WG_UnHookInject");
	// WGWorkers();
	extern int __stdcall AhnHS_Callback(long lCode, long lParamSize, void* pParam);
	nRet = _AhnHS_Initialize(szFullFileName, AhnHS_Callback, 
		1000, "B228F2916A48AC24", 
		AHNHS_CHKOPT_ALL|AHNHS_DISPLAY_HACKSHIELD_TRAYICON|AHNHS_CHKOPT_LOADLIBRARY|AHNHS_CHKOPT_PROTECT_D3DX|AHNHS_CHKOPT_LOCAL_MEMORY_PROTECTION,
		AHNHS_SPEEDHACK_SENSING_RATIO_HIGHEST);

	//r3dOutToLog("_AhnHS_Initialize\n");

	if (nRet != HS_ERR_OK && nRet != 257 && nRet != 12159)
	{
		_stprintf(szMsg, _T("HackShield Initialize Error. Folder HShield is missing from your computer(Error Code = %d)."), nRet);
		MessageBox(NULL, szMsg, szTitle, MB_OK);
		return FALSE;
	}

	//WGPs = LoadLibrary(TEXT("WarGuard\\WGProc.dll"));
	//WGWorkers = (HackCheck)GetProcAddress(WGPs,"FAPFAPFAP5");
	//WGWorkers();

	DWORD version;
	nRet = _AhnHS_GetSDKVersion(&version);

	//WGWorkers = (HackCheck)GetProcAddress(WGMem,"WG_StartInject");
	// WGWorkers();
	if (nRet != HS_ERR_OK)
	{
		_stprintf(szMsg, _T("HackShield Api Error. (Error Code = %d)."), nRet);
		MessageBox(NULL, szMsg, szTitle, MB_OK);
	}
	else
	{
		//_stprintf(szMsg, _T("HackShield SDK Version %d."), version);
		if (version != 1446617492)
		{
			_stprintf(szMsg, _T("HackShield Load Failed. Wrong Version."));
			MessageBox(NULL, szMsg, szTitle, MB_OK);
			return FALSE;
		}
	}
	//r3dOutToLog("HackShield SDK Version %x\n",version);

	//WGWorkers = (HackCheck)GetProcAddress(WGPs,"WG_UnHookInject");
	//WGWorkers();
	//FreeLibrary(WGPs);

	nRet = _AhnHS_StartService();
	assert(nRet != HS_ERR_NOT_INITIALIZED);
	assert(nRet != HS_ERR_ALREADY_SERVICE_RUNNING);
	//r3dOutToLog("_AhnHS_StartService\n");

	if (nRet != HS_ERR_OK && nRet != 257 && nRet != 12159)
	{
		_stprintf(szMsg, _T("HackShield Start Error. Folder HShield is missing from your computer(Error Code = %d)."), nRet);
		MessageBox(NULL, szMsg, szTitle, MB_OK);
		return FALSE;
	}

	//ListProcessModules(GetCurrentProcessId());
	//if (WG_StartVirtualMem() != 235784)
	//TerminateProcess(GetCurrentProcess(),-1);
	//GetModule();
	//r3dOutToLog("WarGuard initialized\n");


	antihackNotFound = 0;



	antihackWarNotFound = 0;
#endif
#ifdef _DEBUG
#ifdef ENABLE_MEMORY_DEBUG
	//_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
	//_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );

	_CrtMemState _ms;
	_CrtMemCheckpoint(&_ms);
#endif
#endif

#if 0
	char* __internal_gnrt_lkey(const char* name, int exp_year, int exp_month, int exp_day);
	void r3dLibraryInit(char* license);
	void checkLicenseKey();

	char* lic = __internal_gnrt_lkey("testing", 2010, 7, 10);
	r3dLibraryInit(lic);
	checkLicenseKey();
#endif

	// set our game to run only one processor (we're not using multithreading)
	// that will help with timer
	//DWORD_PTR oldmask = ::SetThreadAffinityMask(::GetCurrentThread(), 0);


#ifdef FINAL_BUILD
	//WG_StopD3D();
#endif
	r3dscpy(__r3dCmdLine, lpCmdLine);

#ifdef _DEBUG
	//DWORD NewMask = ~(_EM_ZERODIVIDE | _EM_OVERFLOW | _EM_INVALID);
	//_controlfp(NewMask, _MCW_EM);
#endif 

	win::hInstance = hInstance;

	r3dThreadEntryHelper(startupFunc, 0);

	PostQuitMessage(0);

#ifdef _DEBUG
#ifdef ENABLE_MEMORY_DEBUG
	_CrtMemDumpAllObjectsSince(&_ms);
#endif
#endif
	return 0;
}
