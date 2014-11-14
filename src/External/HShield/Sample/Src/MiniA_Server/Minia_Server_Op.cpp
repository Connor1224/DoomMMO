#include <WinSock2.h>
#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

#include "Minia_Server_Op.h"
#include "NetSvMngr.h"

DWORD m_dwCheckInterval;
AHNHS_SERVER_HANDLE	m_hServer;

//
/*******************************************************************************
*	Receive한 데이터의 헤더정보를 이용하여 어떤 작업을 원하는지 확인한 후
*	적절한 작업을 수행한다.

* Parameters: 
*   pIoData : Client로부터 전달받은 데이터
*	pHandleData : Socket Handle과 관련된 데이터
* 
* Return: 
*	NONE
*******************************************************************************/

DWORD WINAPI JobProcessing (IN PIO_DATA pIoData, 	IN PHANDLE_DATA pHandleData)
{
	DWORD dwRet = ERROR_SUCCESS;

	switch ( pIoData->msgHeader.opCode )
	{
	// 클라이언트 로그인 
	case OP_LOGON:
		dwRet = Op_ClientLogon ( pIoData, pHandleData);
		break;
	// 클라이언트 로그오프
	case OP_LOGOFF:
		Op_ClientLogoff ( pIoData, pHandleData );
		break;	
	// 클라이언트 ACK-GUID 메세지
	case OP_ACKGUID:
		// 확장 서버연동은, GuidAck 와 CrcAck 의 구별이 없음.
		dwRet = Op_CrcAck ( pIoData, pHandleData);			
		break;
	// 클라이언트 ACK-CRC 메세지 
	case OP_ACKCRC:
		dwRet = Op_CrcAck ( pIoData, pHandleData);
		break;

	// ※주의: 클라이언트로부 일정시간안에 응답메세지를 수신하지 못하는 경우,
	//		   해당 클라이언트 접속 세션을 종료해야한다.

	default:
		break;
	}

	return dwRet;
}

// 클라이언트의 GUID를 얻어와야 한다.
DWORD Op_ClientLogon (IN PIO_DATA pIoData, IN PHANDLE_DATA pHandleData)
{
	AHNHS_TRANS_BUFFER tbRequest;	// 클라이언트에 전달하게 될 변수
	DWORD dwRet = ERROR_SUCCESS;
	MSGHEADER header = { 0, 0 };
	

	// ------------------------------------------------------------------------
	// [_AhnHS_CreateClientObject ]
	// ------------------------------------------------------------------------
	//  - DESCRIPTION:
	//    서버 핸들을 입력 받아 클라이언트 핸들을 생성합니다. 클라이언트 핸들은 
	//    클라이언트가 접속할 때 마다 생성하며, 세션이 유지되는 동안 핸들을 닫지 않고 재사용합니다.
	//
	//  - SYNTAX:
	//    AHNHS_CLIENT_HANDLE _AhnHS_CreateClientObject (IN AHNHS_SERVER_HANDLE hServer);
	//
	//  - PARAMETERS:
	//    hServer	  :AHNHS_SERVER_HANDLE	서버 핸들(Server Handle) 
	//
	//  - RETURN VALUE
		//    클라이언트 핸들(Client Handle)

		pHandleData->hClient = _AhnHS_CreateClientObject ( m_hServer );

		if ( pHandleData->hClient == ANTICPX_INVALID_HANDLE_VALUE )
		{
			dwRet = ERROR_INVALID_HANDLE;
		}
		else 
		{
		// -----------------------------------------------------------
		// [_AhnHS_MakeRequest ]
		// -----------------------------------------------------------
		// - DESCRIPTION:
		//    현재 세션에 맞는 클라이언트 핸들을 입력하여 요청 메시지를 생성합니다. 
		//    요청 메시지는 AHNHS_TRANS_BUFFER 구조체 형태로 출력되며, 
		//    멤버 변수 값은 다음과 같습니다.
		//    typedef struct _AHNHS_TRANS_BUFFER
		//    {
		//      	unsigned short nLength;			// 요청 메시지 생성에 사용된 버퍼 길이
		//          unsigned char byBuffer[ANTICPX_TRANS_BUFFER_MAX]; // 요청 메시지 생성에 사용될 수 있는 최대 바이트 버퍼
		//    } AHNHS_TRANS_BUFFER, *PAHNHS_TRANS_BUFFER;
		//
		//  ※주의:
		//    byBuffer는 요청 메시지 생성에 사용될 수 있는 최대 버퍼 크기이므로 네트워크로 전송할 때 nLength 
		//    만큼만 전송해야 합니다.
		//
		// - SYNTAX:
		//   unsigned long __stdcall _AhnHS_MakeRequest (IN AHNHS_CLIENT_HANDLE hClient, 
		//												OUT PAHNHS_TRANS_BUFFER pRequestBuffer);
		// - PARAMETERS:
		//   .hClient			:AHNHS_CLIENT_HANDLE	클라이언트 핸들
		//   .pRequestBuffer	:PAHNHS_TRANS_BUFFER	보낼 데이터 버퍼/길이
		// 
		// - RETURN VALUE:
		//   . ERROR_SUCCESS:
		//	   함수 호출을 성공했을 때 리턴하는 값입니다. (Value = 0x00000000)
		//   . 기타				: 기타 리턴값에 대해서는 핵쉴드 프로그래밍 가이드 참고바람.


		dwRet = _AhnHS_MakeRequest( pHandleData->hClient, &tbRequest );

		TCHAR szMsg[256]={0,};
		_stprintf(szMsg, "##_AhnHS_MakeRequest #1 result:%x\r\n", dwRet);
		OutputDebugString(szMsg);
	}


	if ( dwRet != ERROR_SUCCESS )
	{
		TCHAR szMsg[MAX_PATH] = { 0, };

		_stprintf ( szMsg, TEXT ( "Minia_Server : _AhnHS_MakeRequest Fail ( 0x%X ) IP : %d.%d.%d.%d\t%d\n" ),
					dwRet,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
					pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
					ntohs ( pHandleData->sockaddr.sin_port ) );

		OutputDebugString ( szMsg );

		header.opCode = OP_ERROR;
		header.length = htonl ( dwRet );

		IoWrite ( pHandleData, &header, NULL, 0 );
	}
	else
	{
		// [확장 서버연동]	tbRequest를 클라이언트에 송신함.	
		header.opCode = OP_REQGUID;
		header.length = htonl ( tbRequest.nLength );

		IoWrite ( pHandleData, &header, tbRequest.byBuffer, tbRequest.nLength );
	
	}

	return dwRet;
}


VOID
Op_ClientLogoff (
	IN PIO_DATA pIoData,
	IN PHANDLE_DATA pHandleData
	)
{

	pIoData;

	// ----------------------------------------------------------
	// [_AhnHS_CloseClientHandle]
	// ----------------------------------------------------------
	// - DESCRIPTION:
	//   생성한 클라이언트 핸들은 클라이언트 세션이 종료될 때 해제되어야 합니다. 
	//   이때 클라이언트 핸들 생성에 사용되었던 메모리나 시스템 자원을 해제하게 됩니다.
	//
	// - SYNTAX:
	//	 void __stdcall_AhnHS_CloseClientHandle (IN AHNHS_CLIENT_HANDLE hClient)
	//
	// - PARAMETERS:
	//   hClient	:AHNHS_CLIENT_HANDLE	클라이언트 핸들(Client Handle)
	//
	// - RETURN VALUE:
	//   없음.
	//
	// - ※주의 : 클라이언트 핸들이 모두 해제된 이후 서버핸들이 해제되어야 한다 

	_AhnHS_CloseClientHandle ( pHandleData->hClient );
	pHandleData->hClient = ANTICPX_INVALID_HANDLE_VALUE;

	ShutDownSocket ( pIoData, pHandleData );
}



DWORD
Op_CrcAck (
	IN PIO_DATA pIoData,
	IN PHANDLE_DATA pHandleData
	)
{

	DWORD dwRet = ERROR_SUCCESS;
	MSGHEADER header = { 0, 0 };
	TCHAR szMsg[MAX_PATH] = { 0, };
	AHNHS_TRANS_BUFFER ptsRequestBuf;
	BOOL bVerify = FALSE;


	// -----------------------------------------------------------------
	// [_AhnHS_VerifyResponse ]
	// -----------------------------------------------------------------
	// - DESCRIPTION:
	//  _AhnHS_MakeRequest 함수를 통한 요청(메시지)에 대한 클라이언트의 
	//   응답이 올바른지 검사하는 함수입니다.
	//
	// - SYNTAX:
	//  unsigned long __stdcall _AhnHS_VerifyResponse (IN AHNHS_CLIENT_HANDLE hClient,
	//													IN unsigned char *pbyResponse,
	//												IN unsigned long nResponseLength)
	// - PARAMETERS:
	//  .hClient		AHNHS_CLIENT_HANDLE 	:클라이언트 핸들
	//	.pbyResponse	char *					:클라이언트로 부터 받은 데이터 버퍼
	//  .nResponseLength	unsigned long		:클라이언트로 부터 받은 데이터 길이
	//
	// - RETURN VALUE:
	//   . ERROR_SUCCESS    :
	//     함수 호출을 성공했을 때 리턴하는 값입니다. (Value = 0x00000000) 게임과 HackShield가 
	//     정상적으로 실행되고 있다는 의미입니다.
	//   . 기타				: 기타 리턴값에 대해서는 핵쉴드 프로그래밍 가이드 참고바람.
	

	dwRet = _AhnHS_VerifyResponse ( pHandleData->hClient, pIoData->szBuf, pIoData->nBufLen );

	//   . 주의             : 하기의 리턴값일 경우(bVerify = FALSE)만 해당 클라이언트 접속을 *종료하며
	//                        성공(ERROR_SUCCESS)이 아닌 그 외의 리턴값인 경우는 접속을 종료하지 않고 
	//                        더이상 AhnHS_MakeRequest 함수도 호출하지 않는것을 권장합니다.
	if (  dwRet == ERROR_ANTICPXSVR_BAD_MESSAGE || 
			dwRet == ERROR_ANTICPXSVR_REPLY_ATTACK ||
			dwRet == ERROR_ANTICPXSVR_HSHIELD_FILE_ATTACK ||
			dwRet == ERROR_ANTICPXSVR_CLIENT_FILE_ATTACK ||
			dwRet == ERROR_ANTICPXSVR_MEMORY_ATTACK || 
			dwRet == ERROR_ANTICPXSVR_NANOENGINE_FILE_ATTACK ||
			dwRet == ERROR_ANTICPXSVR_UNKNOWN_CLIENT || 
			dwRet == ERROR_ANTICPXSVR_INVALID_HACKSHIELD_VERSION ||
			dwRet == ERROR_ANTICPXSVR_INVALID_ENGINE_VERSION )
	{
		bVerify = FALSE;
	}
	else
	{
		bVerify = TRUE;
	}



	if ( bVerify != TRUE ) 
	{
		_stprintf ( szMsg, TEXT ( "Minia_Server : _AhnHS_VerifyResponse Fail ( 0x%X ) IP : %d.%d.%d.%d\t%d\n" ),
			dwRet,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
			ntohs ( pHandleData->sockaddr.sin_port ) );
		
		
		OutputDebugString ( szMsg );
		
		header.opCode = OP_ERROR;
		header.length = htonl ( dwRet );
	
		// ※주의:
		// 해당 샘플은 클라이언트에서 서버로부터에 어떤 유형에 의해 종료되었는지 확인을 위해
		// 클라이언트에 해당 리턴(dwRet)값을 전달하나
		
		// *게임 서버에서는 해당 클라이언트의 접속을 바로 *종료해야 합니다.
		// Close(..)

		IoWrite ( pHandleData, &header, NULL, 0 );
	}
	else
	{
		_stprintf ( szMsg, TEXT ( "Minia_Server : CRC Check OK, IP : %d.%d.%d.%d\t%d\n" ),
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
			pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
			ntohs ( pHandleData->sockaddr.sin_port ) );
		
		Sleep ( m_dwCheckInterval );
		
		//ZeroMemory ( pHandleData->byClientInfo, SIZEOF_REQINFO );


		// 함수설명 위와 동일 
		dwRet = _AhnHS_MakeRequest ( pHandleData->hClient, &ptsRequestBuf );

		TCHAR szMsg[256]={0,};
		_stprintf(szMsg, "##_AhnHS_MakeRequest #2 result:%x\r\n", dwRet);
		OutputDebugString(szMsg);
		
		
		if ( dwRet != ERROR_SUCCESS )
		{
			_stprintf ( szMsg, TEXT ( "Minia_Server : _AhnHS_MakeRequest Fail ( 0x%X ) IP : %d.%d.%d.%d\t%d\n" ),
				dwRet,
				pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b1,
				pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b2,
				pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b3,
				pHandleData->sockaddr.sin_addr.S_un.S_un_b.s_b4,
				ntohs ( pHandleData->sockaddr.sin_port ) );
			
			
			OutputDebugString ( szMsg );
			
			header.opCode = OP_ERROR;
			header.length = htonl ( dwRet );
			
			IoWrite ( pHandleData, &header, NULL, 0 );
		}
		else
		{		
			header.opCode = OP_REQCRC;
			header.length = htonl ( ptsRequestBuf.nLength );

			IoWrite ( pHandleData, &header, ptsRequestBuf.byBuffer, ptsRequestBuf.nLength );
		}
	}

	return dwRet;
}
