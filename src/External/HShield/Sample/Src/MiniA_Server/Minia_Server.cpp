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
extern DWORD m_dwCheckInterval;					// �������� �ֱ� 

extern	AHNHS_SERVER_HANDLE	m_hServer;			// [Ȯ�� ��������] ���� �ڵ� ��ü


#define DEFAULT_PORT		8997

// Client �˻� �ֱ�
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

	// [Ȯ�� ��������] ���� ��ü�� ����
	// --------------------------------------------------------------------------
	// [_AhnHS_CreateServerObject]
	// --------------------------------------------------------------------------
	// - DESCRIPTION:
	//   :HSBGen.exe�� ���� ������ .hsb ������ �ε��Ͽ� ���� �ڵ�(Server Handle)�� 
	//    �����մϴ�. ���� �ϳ��� ������ �����ϴ� ���� ���μ������� ���� �ڵ��� 
	//	  �� ���� �����Ͽ� ���� ���� ���μ����� ������ ������ �����մϴ�.
	//
	// - SYNTAX:
	//    AHNHS_SERVER_HANDLE __stdcall _AhnHS_CreateServerObject (IN const char *pszFilePath);
	//
	// - PARAMETERS:
	//    .pszFilePath	:const char *	HackShield Briefcase (.hsb) ���� ��ü ���
	//
	// - RETURN VALUE:
	//   ���� �ڵ��� �ùٸ��� �������� ������ ��� NULL(ANTICPX_INVALID_HANDLE_VALUE) ���� �����մϴ�. 
	//   ���� HackShield Briefcase (.hsb) ���� ��ΰ� �ùٸ��� ���� ��쳪 �ý��� ���ҽ�(�޸�)�� 
	//   ������ ��쿡 �߻��մϴ�.

	m_hServer = _AhnHS_CreateServerObject ( m_szHashFilePath );

	if ( m_hServer == ANTICPX_INVALID_HANDLE_VALUE )
	{
		OutputDebugString (_TEXT("Minia_Server : _AhnHS_CreateServerObject Failed\n" ));
		goto _END;
	}

	// ���� ������� Thread�� �ʿ��� ������ ����
	NetParam.pbStop = &bStop;				// Thread �۵� ����
	NetParam.unPort = m_unPort;				// Server Open Port


	// [Ȯ�� ��������] �ۼ��� BUFFER�� MAX SIZE�� ������
	NetParam.unMaxBodyLen = ANTICPX_TRANS_BUFFER_MAX;

	// ���� ������� Thread���� Ŭ���̾�Ʈ�κ���  DATA ���Ž� JobProcessing�Լ��� �����Ѵ�.
	NetParam.pJobProcessingFunc = (PVOID) JobProcessing;

	// Network Thread�� �����Ѵ�.
	hThread = ( HANDLE ) _beginthreadex ( NULL, 0, NetworkThreadProc, ( void * ) &NetParam, 0, &nThreadID );
	
	if ( hThread == NULL )
	{
		OutputDebugString ( TEXT ( "Minia_Server : CreateThread Error!\n" ) );
		goto _END;
	}

	// �ܺο��� ���� �̺�Ʈ�� Ȱ��ȭ(m_hStop)�Ǳ� ������ ���Ѵ��
	WaitForSingleObject ( ( HANDLE ) m_hStop, INFINITE );
	bStop = TRUE;
	WaitForSingleObject ( hThread, 5000 );

_END:;

	// ���� ����

	// [Ȯ�� ��������] �ڵ� ����
	if ( m_hServer != ANTICPX_INVALID_HANDLE_VALUE )
	{
		//--------------------------------------------------------------------
		//[_AhnHS_CloseServerHandle]
		//--------------------------------------------------------------------
		// - DESCRIPTION:
		//   ���� �ڵ�(Server Handle)�� �ݽ��ϴ�.
		//
		// - SYNTAX:
		//   void __stdcall _AhnHS_CloseServerHandle (IN AHNHS_SERVER_HANDLE hServer);
		//
		// - PARAMETERS:
		//   .pszFilePath	:AHNHS_SERVER_HANDLE	_AhnHS_CreateServerObject�Լ��� ���� ������ �ڵ�
		//
		// - RETURN VALUE:
		//   ����.

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

	// Minia_Server.ini ��θ� ����.
	GetModuleFileName(NULL, szCurFileName, MAX_PATH);
	pEnd = _tcsrchr( szCurFileName, _T('\\')) + 1;
	if (!pEnd)
	{
		return FALSE;	
	}
	*pEnd = _T('\0');
	_stprintf(szIniFile, _T("%sMinia_Server.ini"), szCurFileName);
	
	// ���� PORT
	m_unPort =  GetPrivateProfileInt ( _T("Setting"), _T("Port" ), DEFAULT_PORT, szIniFile );
	// �������� �ֱ�
	m_dwCheckInterval =  GetPrivateProfileInt ( _T("Setting"), _T("CheckInterval" ), DEFAULT_CHECKINTERVAL, szIniFile );

	// [Ȯ�� ��������] HSB ���� PATH
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
