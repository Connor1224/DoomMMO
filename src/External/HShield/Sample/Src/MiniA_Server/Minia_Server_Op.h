#ifndef _OP_H
#define _OP_H

#define		OP_LOGON				0x1		// Logon에 대한 Notify
#define		OP_LOGOFF				0x2		// Logoff에 대한 Notify
#define		OP_REQGUID				0x3		// GUID Request Message
#define		OP_ACKGUID				0x4		// GUID Ack Message
#define		OP_REQCRC				0x5		// CRC Request Message
#define		OP_ACKCRC				0x6		// CRC Ack Message
#define		OP_ERROR				0x7		// Error Message

#include "NetSvMngr.h"

DWORD WINAPI JobProcessing (IN PIO_DATA pIoData, 	IN PHANDLE_DATA pHandleData);
DWORD Op_ClientLogon ( IN PIO_DATA pIoData, IN PHANDLE_DATA pHandleData );
VOID Op_ClientLogoff ( IN PIO_DATA pIoData, IN PHANDLE_DATA pHandleData );
DWORD Op_GuidAck ( IN PIO_DATA pIoData, IN PHANDLE_DATA pHandleData );
DWORD Op_CrcAck ( IN PIO_DATA pIoData, IN PHANDLE_DATA pHandleData);

#endif
