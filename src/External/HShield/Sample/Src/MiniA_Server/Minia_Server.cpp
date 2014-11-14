#include <winsock2.h>
#include <Windows.h>
#include <stdio.h>
#include <Tchar.h>
#include <process.h>

#include "Minia_Server_Op.h"
#include "NetSvMngr.h"
#include "AntiCpXSvr.h"

HANDLE m_hStop = NULL;
TCHAR m_szHashFilePath[MAX_PATH * 2] = {0, };

//
unsigned int m_unPort;

//
extern DWORD m_dwCheckInterval;					// 서버연동 주기 

extern	AHNHS_SERVER_HANDLE	m_hServer;			// [확장 서버연동] 서버 핸들 개체


#define DEFAULT_PORT		8997

// Client 검사 주기
#define DEFAULT_CHECKINTERVAL	10000

//
void RunService();
BOOL LoadSetting ();
void StopService();

//
void RunService()
{
	UINT nThreadID;
	HANDLE hThread = NULL;
	BOOL bStop = FALSE;
	NETTHREAD_PARAM NetParam;

	if ( LoadSetting () == FALSE )
	{
		OutputDebugString ( TEXT ( "Minia_Server : LoadSetting Error!\n" ) );
		goto _END;
	}

	m_hStop = CreateEvent ( 0, FALSE, FALSE, 0 );

	if ( m_hStop == NULL )
	{
		goto _END;
	}

	// [확장 서버연동] 서버 개체를 생성
	// --------------------------------------------------------------------------
	// [_AhnHS_CreateServerObject]
	// --------------------------------------------------------------------------
	// - DESCRIPTION:
	//   :HSBGen.exe를 통해 생성한 .hsb 파일을 로드하여 서버 핸들(Server Handle)을 
	//    생성합니다. 보통 하나의 게임을 서비스하는 서버 프로세스에서 서버 핸들은 
	//	  한 개를 생성하여 게임 서버 프로세스가 종료할 때까지 유지합니다.
	//
	// - SYNTAX:
	//    AHNHS_SERVER_HANDLE __stdcall _AhnHS_CreateServerObject (IN const char *pszFilePath);
	//
	// - PARAMETERS:
	//    .pszFilePath	:const char *	HackShield Briefcase (.hsb) 파일 전체 경로
	//
	// - RETURN VALUE:
	//   서버 핸들을 올바르게 생성하지 못했을 경우 NULL(ANTICPX_INVALID_HANDLE_VALUE) 값을 리턴합니다. 
	//   보통 HackShield Briefcase (.hsb) 파일 경로가 올바르지 않을 경우나 시스템 리소스(메모리)가 
	//   부족할 경우에 발생합니다.

	m_hServer = _AhnHS_CreateServerObject ( m_szHashFilePath );

	if ( m_hServer == ANTICPX_INVALID_HANDLE_VALUE )
	{
		OutputDebugString (_TEXT("Minia_Server : _AhnHS_CreateServerObject Failed\n" ));
		goto _END;
	}

	// 서버 소켓통신 Thread에 필요한 정보를 전달
	NetParam.pbStop = &bStop;				// Thread 작동 유무
	NetParam.unPort = m_unPort;				// Server Open Port


	// [확장 서버연동] 송수신 BUFFER의 MAX SIZE를 설정함
	NetParam.unMaxBodyLen = ANTICPX_TRANS_BUFFER_MAX;

	// 서버 소켓통신 Thread에서 클라이언트로부터  DATA 수신시 JobProcessing함수를 실행한다.
	NetParam.pJobProcessingFunc = (PVOID) JobProcessing;

	// Network Thread를 생성한다.
	hThread = ( HANDLE ) _beginthreadex ( NULL, 0, NetworkThreadProc, ( void * ) &NetParam, 0, &nThreadID );
	
	if ( hThread == NULL )
	{
		OutputDebugString ( TEXT ( "Minia_Server : CreateThread Error!\n" ) );
		goto _END;
	}

	// 외부에서 종료 이벤트가 활성화(m_hStop)되기 전까지 무한대기
	WaitForSingleObject ( ( HANDLE ) m_hStop, INFINITE );
	bStop = TRUE;
	WaitForSingleObject ( hThread, 5000 );

_END:;

	// 서버 종료

	// [확장 서버연동] 핸들 종료
	if ( m_hServer != ANTICPX_INVALID_HANDLE_VALUE )
	{
		//--------------------------------------------------------------------
		//[_AhnHS_CloseServerHandle]
		//--------------------------------------------------------------------
		// - DESCRIPTION:
		//   서버 핸들(Server Handle)을 닫습니다.
		//
		// - SYNTAX:
		//   void __stdcall _AhnHS_CloseServerHandle (IN AHNHS_SERVER_HANDLE hServer);
		//
		// - PARAMETERS:
		//   .pszFilePath	:AHNHS_SERVER_HANDLE	_AhnHS_CreateServerObject함수를 통해 생성한 핸들
		//
		// - RETURN VALUE:
		//   없음.

		_AhnHS_CloseServerHandle ( m_hServer );
		m_hServer = ANTICPX_INVALID_HANDLE_VALUE;
	}


	if ( hThread != NULL )
		CloseHandle ( hThread );

	if ( m_hStop != NULL )
		CloseHandle ( m_hStop );
	
}

//
void StopService()
{
	if ( m_hStop != NULL )
		SetEvent ( m_hStop );
}

BOOL LoadSetting ()
{
	TCHAR szIniFile[MAX_PATH];
	TCHAR szCurFileName[MAX_PATH] = { 0, };
	TCHAR *pEnd = NULL;

	// Minia_Server.ini 경로를 구함.
	GetModuleFileName(NULL, szCurFileName, MAX_PATH);
	pEnd = _tcsrchr( szCurFileName, _T('\\')) + 1;
	if (!pEnd)
	{
		return FALSE;	
	}
	*pEnd = _T('\0');
	_stprintf(szIniFile, _T("%sMinia_Server.ini"), szCurFileName);
	
	// 서버 PORT
	m_unPort =  GetPrivateProfileInt ( _T("Setting"), _T("Port" ), DEFAULT_PORT, szIniFile );
	// 서버연동 주기
	m_dwCheckInterval =  GetPrivateProfileInt ( _T("Setting"), _T("CheckInterval" ), DEFAULT_CHECKINTERVAL, szIniFile );

	// [확장 서버연동] HSB 파일 PATH
	if ( GetPrivateProfileString ( _T("Setting"), _T("HsbFilePath"), NULL, m_szHashFilePath, MAX_PATH * 2, szIniFile ) == 0 )
	{
		return FALSE;
	}
	return TRUE;
}


int WINAPI WinMain ( HINSTANCE, HINSTANCE, LPSTR lpCmdLine, int ) 
{
	RunService();
	return 0;
}
