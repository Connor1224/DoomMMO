/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: BRGR - secondary contact: NOBODY

#ifndef Navigation_SocketDispatcher_H
#define Navigation_SocketDispatcher_H

#include "gwnavruntime/kernel/SF_Types.h"

#ifdef KY_ENABLE_SOCKETS
#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/visualdebug/amp/Amp_Socket.h"

namespace Kaim
{

class File;

enum SocketDispatchMode 
{
	SocketDispatch_NetworkOnly = 0, //< data are sent to and received from the network exclusively
	SocketDispatch_SendToNetworkAndFile = 1, //< data are sent to network and file, and received from network
	SocketDispatch_SendToFile = 2, //< data are sent to file and receive calls always give a Heartbeat message
	SocketDispatch_ReceiveFromNetworkToFile = 3, //< data are received from network and written to file, and sent to network
	SocketDispatch_ReceiveFromFile = 4, //< data are received from file exclusively, and sent to nowhere.
};

class SocketDispatcherFactory : public Net::AMP::SocketImplFactory
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:
	// @param file must be ready for write and read
	SocketDispatcherFactory(SocketDispatchMode mode, File* file) : m_mode(mode), m_file(file)
	{
		// check that a file is passed if the given mode requires one!
		KY_ASSERT( m_mode == SocketDispatch_NetworkOnly || m_file != KY_NULL );
	}
	virtual ~SocketDispatcherFactory() {}

	virtual Net::AMP::SocketInterface* Create();
	virtual void Destroy(Net::AMP::SocketInterface* socketImpl);

private:
	SocketDispatchMode m_mode;
	File* m_file;
};

}

#endif

#endif // Navigation_SocketDispatcher_H
