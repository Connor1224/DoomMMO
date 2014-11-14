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
*	Receive�� �������� ��������� �̿��Ͽ� � �۾��� ���ϴ��� Ȯ���� ��
*	������ �۾��� �����Ѵ�.

* Parameters: 
*   pIoData : Client�κ��� ���޹��� ������
*	pHandleData : Socket Handle�� ���õ� ������
* 
* Return: 
*	NONE
*******************************************************************************/

DWORD WINAPI JobProcessing (IN PIO_DATA pIoData, 	IN PHANDLE_DATA pHandleData)
{
	DWORD dwRet = ERROR_SUCCESS;

	switch ( pIoData->msgHeader.opCode )
	{
	// Ŭ���̾�Ʈ �α��� 
	case OP_LOGON:
		dwRet = Op_ClientLogon ( pIoData, pHandleData);
		break;
	// Ŭ���̾�Ʈ �α׿���
	case OP_LOGOFF:
		Op_ClientLogoff ( pIoData, pHandleData );
		break;	
	// Ŭ���̾�Ʈ ACK-GUID �޼���
	case OP_ACKGUID:
		// Ȯ�� ����������, GuidAck �� CrcAck �� ������ ����.
		dwRet = Op_CrcAck ( pIoData, pHandleData);			
		break;
	// Ŭ���̾�Ʈ ACK-CRC �޼��� 
	case OP_ACKCRC:
		dwRet = Op_CrcAck ( pIoData, pHandleData);
		break;

	// ������: Ŭ���̾�Ʈ�κ� �����ð��ȿ� ����޼����� �������� ���ϴ� ���,
	//		   �ش� Ŭ���̾�Ʈ ���� ������ �����ؾ��Ѵ�.

	default:
		break;
	}

	return dwRet;
}

// Ŭ���̾�Ʈ�� GUID�� ���;� �Ѵ�.
DWORD Op_ClientLogon (IN PIO_DATA pIoData, IN PHANDLE_DATA pHandleData)
{
	AHNHS_TRANS_BUFFER tbRequest;	// Ŭ���̾�Ʈ�� �����ϰ� �� ����
	DWORD dwRet = ERROR_SUCCESS;
	MSGHEADER header = { 0, 0 };
	

	// ------------------------------------------------------------------------
	// [_AhnHS_CreateClientObject ]
	// ------------------------------------------------------------------------
	//  - DESCRIPTION:
	//    ���� �ڵ��� �Է� �޾� Ŭ���̾�Ʈ �ڵ��� �����մϴ�. Ŭ���̾�Ʈ �ڵ��� 
	//    Ŭ���̾�Ʈ�� ������ �� ���� �����ϸ�, ������ �����Ǵ� ���� �ڵ��� ���� �ʰ� �����մϴ�.
	//
	//  - SYNTAX:
	//    AHNHS_CLIENT_HANDLE _AhnHS_CreateClientObject (IN AHNHS_SERVER_HANDLE hServer);
	//
	//  - PARAMETERS:
	//    hServer	  :AHNHS_SERVER_HANDLE	���� �ڵ�(Server Handle) 
	//
	//  - RETURN VALUE
		//    Ŭ���̾�Ʈ �ڵ�(Client Handle)

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
		//    ���� ���ǿ� �´� Ŭ���̾�Ʈ �ڵ��� �Է��Ͽ� ��û �޽����� �����մϴ�. 
		//    ��û �޽����� AHNHS_TRANS_BUFFER ����ü ���·� ��µǸ�, 
		//    ��� ���� ���� ������ �����ϴ�.
		//    typedef struct _AHNHS_TRANS_BUFFER
		//    {
		//      	unsigned short nLength;			// ��û �޽��� ������ ���� ���� ����
		//          unsigned char byBuffer[ANTICPX_TRANS_BUFFER_MAX]; // ��û �޽��� ������ ���� �� �ִ� �ִ� ����Ʈ ����
		//    } AHNHS_TRANS_BUFFER, *PAHNHS_TRANS_BUFFER;
		//
		//  ������:
		//    byBuffer�� ��û �޽��� ������ ���� �� �ִ� �ִ� ���� ũ���̹Ƿ� ��Ʈ��ũ�� ������ �� nLength 
		//    ��ŭ�� �����ؾ� �մϴ�.
		//
		// - SYNTAX:
		//   unsigned long __stdcall _AhnHS_MakeRequest (IN AHNHS_CLIENT_HANDLE hClient, 
		//												OUT PAHNHS_TRANS_BUFFER pRequestBuffer);
		// - PARAMETERS:
		//   .hClient			:AHNHS_CLIENT_HANDLE	Ŭ���̾�Ʈ �ڵ�
		//   .pRequestBuffer	:PAHNHS_TRANS_BUFFER	���� ������ ����/����
		// 
		// - RETURN VALUE:
		//   . ERROR_SUCCESS:
		//	   �Լ� ȣ���� �������� �� �����ϴ� ���Դϴ�. (Value = 0x00000000)
		//   . ��Ÿ				: ��Ÿ ���ϰ��� ���ؼ��� �ٽ��� ���α׷��� ���̵� ����ٶ�.


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
		// [Ȯ�� ��������]	tbRequest�� Ŭ���̾�Ʈ�� �۽���.	
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
	//   ������ Ŭ���̾�Ʈ �ڵ��� Ŭ���̾�Ʈ ������ ����� �� �����Ǿ�� �մϴ�. 
	//   �̶� Ŭ���̾�Ʈ �ڵ� ������ ���Ǿ��� �޸𸮳� �ý��� �ڿ��� �����ϰ� �˴ϴ�.
	//
	// - SYNTAX:
	//	 void __stdcall_AhnHS_CloseClientHandle (IN AHNHS_CLIENT_HANDLE hClient)
	//
	// - PARAMETERS:
	//   hClient	:AHNHS_CLIENT_HANDLE	Ŭ���̾�Ʈ �ڵ�(Client Handle)
	//
	// - RETURN VALUE:
	//   ����.
	//
	// - ������ : Ŭ���̾�Ʈ �ڵ��� ��� ������ ���� �����ڵ��� �����Ǿ�� �Ѵ� 

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
	//  _AhnHS_MakeRequest �Լ��� ���� ��û(�޽���)�� ���� Ŭ���̾�Ʈ�� 
	//   ������ �ùٸ��� �˻��ϴ� �Լ��Դϴ�.
	//
	// - SYNTAX:
	//  unsigned long __stdcall _AhnHS_VerifyResponse (IN AHNHS_CLIENT_HANDLE hClient,
	//													IN unsigned char *pbyResponse,
	//												IN unsigned long nResponseLength)
	// - PARAMETERS:
	//  .hClient		AHNHS_CLIENT_HANDLE 	:Ŭ���̾�Ʈ �ڵ�
	//	.pbyResponse	char *					:Ŭ���̾�Ʈ�� ���� ���� ������ ����
	//  .nResponseLength	unsigned long		:Ŭ���̾�Ʈ�� ���� ���� ������ ����
	//
	// - RETURN VALUE:
	//   . ERROR_SUCCESS    :
	//     �Լ� ȣ���� �������� �� �����ϴ� ���Դϴ�. (Value = 0x00000000) ���Ӱ� HackShield�� 
	//     ���������� ����ǰ� �ִٴ� �ǹ��Դϴ�.
	//   . ��Ÿ				: ��Ÿ ���ϰ��� ���ؼ��� �ٽ��� ���α׷��� ���̵� ����ٶ�.
	

	dwRet = _AhnHS_VerifyResponse ( pHandleData->hClient, pIoData->szBuf, pIoData->nBufLen );

	//   . ����             : �ϱ��� ���ϰ��� ���(bVerify = FALSE)�� �ش� Ŭ���̾�Ʈ ������ *�����ϸ�
	//                        ����(ERROR_SUCCESS)�� �ƴ� �� ���� ���ϰ��� ���� ������ �������� �ʰ� 
	//                        ���̻� AhnHS_MakeRequest �Լ��� ȣ������ �ʴ°��� �����մϴ�.
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
	
		// ������:
		// �ش� ������ Ŭ���̾�Ʈ���� �����κ��Ϳ� � ������ ���� ����Ǿ����� Ȯ���� ����
		// Ŭ���̾�Ʈ�� �ش� ����(dwRet)���� �����ϳ�
		
		// *���� ���������� �ش� Ŭ���̾�Ʈ�� ������ �ٷ� *�����ؾ� �մϴ�.
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


		// �Լ����� ���� ���� 
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
