/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: BRGR - secondary contact: NOBODY
#ifndef Navigation_VisualDebugServer_H
#define Navigation_VisualDebugServer_H

#include "gwnavruntime/kernel/SF_Types.h"

#if defined(KY_ENABLE_SOCKETS) && defined(KY_ENABLE_THREADS)

#include "gwnavruntime/visualdebug/internal/socketdispatcher.h"
#include "gwnavruntime/visualdebug/internal/visualdebugstat.h"
#include "gwnavruntime/visualdebug/visualdebugclientregistry.h"
#include "gwnavruntime/visualdebug/amp/Amp_ThreadMgr.h"
#include "gwnavruntime/blob/blobfieldarray.h"
#include "gwnavruntime/blob/blobhandler.h"
#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavruntime/base/memory.h"

namespace Kaim
{

class FileOpenerBase;
class File;

class ServerStatusChangedCallback;
class MessageBlob;
class MessageAggregatedBlob;
class MessageBlobHandler;
class BlobFieldsStore;

class FloatStat;


/// Configure the VisualDebugServer.
/// Call UseNavigationLab() or UseLocalFile() to determine how to run the VisualDebugServer.
/// Note that Mode_UseLocalFile mode logs _all_ sent data in to a file, it is mainly intended for testing.
class VisualDebugServerConfig
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_WorldFwk)

public:
	/// Enumerates the visual debug information destination.
	enum Mode {
		Mode_UseNavigationLab, ///< Sends visual debug information to the NavigationLab over the Network
		Mode_UseLocalFile      ///< Writes _all_ visual debug information to a file on the platform
	};

	/// Define the behavior for VisualDebugServer::Start.
	enum WaitOnStart {
		DoNotWaitOnStart, ///< Start won't wait for a connection
		DoWaitOnStart     ///< Start will block until a connection occurs
	};

	/// Define how to detect disconnections.
	enum DisconnectionMode {
		UseSocketErrors, ///< use socket errors to detect disconnections (recommended)
		UseHeartbeat     ///< use heartbeat messages to detect disconnections (disconnection can occur when breakpointing)
	};

	/// Determine how the data are sent.
	enum RunMode {
		RunSynchronously, ///< run in current thread (recommended)
		RunAsynchronously ///< use secondary threads for sending blobs
	};

public:
	VisualDebugServerConfig()
		: m_mode(Mode_UseNavigationLab)
		, m_serverPort(0)
		, m_broadcastPort(0)
		, m_waitOnStart(DoNotWaitOnStart)
		, m_navigationLabCommandHandlers(KY_NULL)
		, m_localFileName(KY_NULL)
		, m_localFileOpener(KY_NULL)
		, m_maximumMegaBytes(1)
		, m_expectedKiloBytesPerFrame(256)
		, m_disconnectionMode(UseSocketErrors)
		, m_runMode(RunSynchronously)
	{}

	/// Reduce chances of selecting conflicting ports by looking at https://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.txt
	/// \param serverPort Indicates the port to use for connecting to the server
	/// \param broadcastPort Indicates the port to use for broadcasting the availability of this server
	/// \param navigationLabCommandHandlers Allows advanced use of the server by specifying handlers. 
	///        If NULL or empty, received messages are discarded when received. 
	///        Otherwise, the user has to call HandleNextReceivedMessage() or GetNextReceivedMessage(), if not received messages won't be freed over frame.
	void UseNavigationLab(KyUInt16 serverPort = 48888, KyUInt16 broadcastPort = 48887, MessageReceiverRegistry* navigationLabCommandHandlers = KY_NULL)
	{
		
		m_mode                         = Mode_UseNavigationLab;
		m_navigationLabCommandHandlers = navigationLabCommandHandlers;
		m_serverPort                   = serverPort;
		m_broadcastPort                = broadcastPort;
		m_localFileName                = KY_NULL;
		m_localFileOpener              = KY_NULL;
	}

	void UseLocalFile(const char* localFileName, FileOpenerBase* localFileOpener = KY_NULL)
	{
		m_mode                         = Mode_UseLocalFile;
		m_navigationLabCommandHandlers = KY_NULL;
		m_serverPort                   = 0;
		m_localFileName                = localFileName;
		m_localFileOpener              = localFileOpener;
	}

	void SetWaitOnStart(WaitOnStart waitOnStart) { m_waitOnStart = waitOnStart; }
	
	/// Define memory limitations for the VisualDebugServer
	void SetMemoryConsumption(KyUInt32 expectedKiloBytesPerFrame, KyUInt32 maximumMegaBytesToUse)
	{
		m_expectedKiloBytesPerFrame = expectedKiloBytesPerFrame;
		m_maximumMegaBytes = maximumMegaBytesToUse;
	}

	void SetDisconnectionMode(DisconnectionMode disconnectionMode) { m_disconnectionMode = disconnectionMode; }
	
	void SetRunMode(RunMode runMode) { m_runMode = runMode; }

	void SetServerName(const char* serverName) { m_serverName = String(serverName); }

public:
	/// The visual debug information destination mode.
	Mode m_mode;

	/// The port used to listen to NavigationLab incoming connections
	KyUInt16 m_serverPort;
	
	/// The port used to broadcast the existence of this server
	KyUInt16 m_broadcastPort;

	WaitOnStart m_waitOnStart;


	/// Optional, handles the commands that come from the Gameware Navigation from the Network.
	/// This is typically set in our examples lightweight game engine, the LabGame and its LabEngine
	/// to control the examples from the NavigationLab. Works only if running in Mode_UseNavigationLab mode.
	MessageReceiverRegistry* m_navigationLabCommandHandlers;

	/// mandatory if #m_mode = Mode_UseLocalFile.
	const char* m_localFileName;

	/// relevant if #m_mode = Mode_UseLocalFile.
	/// if #m_localFileOpener is NULL, DefaultFileOpener will be used
	FileOpenerBase* m_localFileOpener;

	/// Specifies the maximum amount of memory this VisualDebugServer is allowed to allocate. 
	KyUInt32 m_maximumMegaBytes;
	/// Specifies the expected amount of memory to allocate per frame. Used to define size of a message.
	KyUInt32 m_expectedKiloBytesPerFrame;

	/// Define how to detect disconnections
	DisconnectionMode m_disconnectionMode;

	/// Define how to run the server, either directly in the current thread (recommended), or using secondary threads.
	RunMode m_runMode;

	/// Associate a name to the server to ease identification of this server when connecting from the NavigationLab
	String m_serverName;
};


/// The VisualDebugServer manages the sending of data to clients.
/// Each World maintains an instance of this class, accessible through its World::GetVisualDebugServer() and created on World::StartVisualDebug() call.
class VisualDebugServer
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_VisualDebug)

public:

	// --------------- Server Status ----------------

	/// Indicates whether the VisualDebugServer is connected to a network socket or if the localFile is ready for write.
	/// In other words, tells if it's worth calling several times the Send() function.
	bool IsConnected() const;

	/// \return true between Start and Stop calls, false otherwise.
	bool IsStarted() const { return m_isStarted; }


	// ------------------ Send Statistics ------------------ 
	/// Send the key-value with min, max, average values that will be displayed in the NavigationLab under the given group name with other key-values.

	void SendStats(const char* statName, const KyFloat32 value, const KyFloat32 average, const KyFloat32 max, const KyFloat32 min, const char* groupName);
	void SendStats(const FloatStat& timeStat, const char* groupName);
	void SendStats(const char* statName, const FloatStat& timeStat, const char* groupName);


	// ------------------ Send Single Value ------------------ 
	/// Send a key-value pair that will be displayed in the NavigationLab under the given group name with other key-values.

	void SendUInt32(const char* statName, const KyUInt32 value, const char* groupName);
	void SendInt32(const char* statName, const KyInt32 value, const char* groupName);
	void SendFloat32(const char* statName, const KyFloat32 value, const char* groupName);
	void SendString(const char* statName, const char* value, const char* groupName);


	// --------------- Advanced usage : Handling Received Blob Messages ----------------
	/// Handle received messages based on the MessageReceiverRegistry parameter provided to Start()
	/// HandleNextReceivedMessage() can be used alone to retrieve the next message and handle it immediately.
	/// \code
	/// while (server->HandleNextReceivedMessage() == KY_SUCCESS) {}
	/// \endcode
	///
	/// Alternatively, for more advanced use, for instance to first store all messages and then to handle them later,
	/// you can use GetNextReceivedMessage() and HandleMessage().
	/// \code
	/// Ptr<Kaim::Net::AMP::Message> msgPtr = server->GetNextReceivedMessage();
	/// while (msgPtr != KY_NULL)
	/// {
	///     HandleMessage(msgPtr); 
	///     msgPtr = server->GetNextReceivedMessage();
	/// }
	/// \endcode

	/// \return KY_SUCCESS if a message was handled, KY_ERROR if there's no message to handle.
	KyResult HandleNextReceivedMessage();
	/// Return next received message
	Ptr<Net::AMP::Message> GetNextReceivedMessage();
	/// Handle the given message
	KyResult HandleMessage(Ptr<Net::AMP::Message> msg);


private:
	KyResult OpenNavigationLabConnection();
	KyResult OpenFileConnection();

	KyResult AggregateMessage(Ptr<BaseBlobHandler> handlerPtr);
	KyResult CreateAggregateMessage(KyUInt32 deepBlobSize);
	KyResult SendAggregateMessage();

	KyResult SetupMessageRegistry(MessageReceiverRegistry* receiverRegistry);
	KyResult OpenConnection(SocketDispatchMode mode, KyUInt32 port, KyUInt32 broadcastPort, bool initSocketConnection);
	
	void AddToStats(KyUInt32 blobtypeId, KyUInt32 deepBlobSize);
	void ClearStatistics();
	void SendInternalStatistics();

	void SendStatGroups();
	VisualDebugStatGroup* GetStatGroup(const char* groupName);
	VisualDebugStatGroup* GetOrCreateStatGroup(const char* groupName);

	KyResult OnConnection();    // called from the socketThread when connection is established
	KyResult OnDisconnection(); // called from the socketThread when disconnected

	// Checks if there is enough memory before creating a new message
	void CheckMemory();

public: // internal

	// --------------- For internal use only ----------------
	/// These functions are used by internal components.  They are mainly used by World and WorldElement instances.
	/// So there's no need for Navigation's users to call them directly

	VisualDebugServer();
	~VisualDebugServer();

	KyResult Start(const VisualDebugServerConfig& config);
	KyResult ReStart() { return Start(m_config); }
	void Stop();

	/// Defines a special value that can be passed to VisualDebugServer::WaitForConnection()
	/// in the calling thread until the connection is established.
	static KyUInt32 GetInfiniteWaitDelay() { return KY_WAIT_INFINITE; }

	/// Wait for the connection to be established (pass GetInfiniteWaitDelay() to wait infinitely if no connection).
	bool WaitForConnection(KyUInt32 maxDelayMilliseconds);

	/// Starts a new frame with the specified frame index. typically called by the World::Update().
	void Update(KyUInt32 frameIndex);

	/// Starts a new frame and increments the frame index. Called typically when there is no World (ex: in NavMeshGeneration).
	void Update() { Update(m_frameIndex); ++m_frameIndex; }

	/// Sends a blob, the blob will be copied into a secondary buffer.
	KyResult Send(BaseBlobHandler& blobHandler);

	/// Sends a blob from a BlobBuilder, the blob will be built into the message, no copy occurs.
	template <class T>
	KyResult Send(BaseBlobBuilder<T>& blobBuilder)
	{
		const KyUInt32 blobType = T::GetBlobTypeId();
		Ptr<BaseBlobHandler> blobHandler = m_handlerRegistry.Get(blobType);
		if (blobHandler == KY_NULL)
		{
			blobHandler = *KY_NEW BlobHandler<T>;
			m_handlerRegistry.SetVal(blobType, blobHandler);
		}

		blobBuilder.Build((BlobHandler<T>&)*blobHandler);
		return this->AggregateMessage(blobHandler);
	}

	/// Enable additional statistics to be sent. These statistics are mainly useful for internal development/debugging.
	void SetAdvancedInternalStatistics(bool enable) { m_enableAdvancedInternalStatistics = enable; }

	/// Set the ThreadId that Send and Update are supposed to be called from. By default, assigned when calling Start.
	void SetThreadId(ThreadId threadId) { m_operatingThreadId = threadId; }

	/// Get the ThreadId that Send and Update are supposed to be called from.
	ThreadId GetThreadId() const  { return m_operatingThreadId; }

	/// Check if it is safe to call Update and Send functions.
	bool CanSafelyOperate() const { return (m_operatingThreadId == GetCurrentThreadId()); }

	// Wait until all messages are sent (or if connection is lost or ended client-side).
	void WaitForEmptiedSendQueue();

	/// Retrieves the number of the current frame. 
	KyUInt32 GetFrameIndex() const { return m_frameIndex; }

	Net::AMP::ThreadMgr* GetThreadMgr()             { return m_socketThreadMgr.GetPtr(); } ///< Accessor to the internal ThreadMgr
	const Net::AMP::ThreadMgr* GetThreadMgr() const { return m_socketThreadMgr.GetPtr(); } ///< Accessor to the internal ThreadMgr

	/// \return The MemoryHeap used for sending messages.
	MemoryHeap* GetHeap() { return Memory::GetHeapByAddress(m_socketThreadMgr.GetPtr()); }

	
private:
	// Config
	VisualDebugServerConfig m_config; // The config passed to Start()
	KyUInt32 m_frameIndex; // The current frame index

	// Status
	bool m_isStarted;  // Set to true between calls to Start and Stop
	bool m_isConnected;
	bool m_wasDisconnected; // Used to cancel previous m_aggregatedBlobMsg
	ThreadId m_operatingThreadId; // ThreadId of the thread that Send functions and Update are supposed to be called from

	// Socket/Thread management
	Ptr<Net::AMP::ThreadMgr> m_socketThreadMgr;
	Ptr<Net::AMP::DiscardMessageHandler> m_msgDiscarder; // used to discard messages we are not interested in
	Ptr<MessageBlobHandler> m_msgBlobHandler;            // used to handle received MessageBlob
	SocketDispatcherFactory* m_socketFactory;            // in charge of creating the socket depending on VisualDebugServerConfig::Mode
	Ptr<File> m_file;                                    // used when mode is Mode_UseLocalFile 
	
	friend class ServerStatusChangedCallback;
	Ptr<ServerStatusChangedCallback> m_statusCallback; // Callback for connection status change
	Event m_connectedEvent;

	// Message construction
	typedef UserPtrBlobRegistry<BaseBlobHandler> BlobHandlerRegistry;
	BlobHandlerRegistry m_handlerRegistry;
	MessageAggregatedBlob* m_aggregatedBlobMsg;


	// Stats
	KyUInt32 m_messagesNotSentOnWait;
	KyUInt32 m_memoryLimitReached; // Records if the memory limit was reached during this frame
	KyFloat32 m_blockTimeMs; // Record the time spend to wait (approximated due to msleep)
	KyUInt32 m_framePerSecond;
	KyUInt64 m_lastTick;
	KyUInt64 m_frameCounter;

	KyUInt32 m_sentBlobCountInFrame;
	KyUInt32 m_sentBlobSizeInFrame;
	KyUInt32 m_sentMessageCountInFrame;
	KyUInt32 m_receivedMessageCountInFrame;

	UserBlobRegistry<SentBlobTypeStats> m_sentBlobTypeStatsRegistry;

	KyArray<VisualDebugStatGroup> m_statToSend;
	bool m_enableAdvancedInternalStatistics;
};

} // namespace Kaim

#endif

#endif // Navigation_VisualDebugServer_H
