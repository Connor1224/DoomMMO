// This code contains NVIDIA Confidential Information and is disclosed to you
// under a form of NVIDIA software license agreement provided separately to you.
//
// Notice
// NVIDIA Corporation and its licensors retain all intellectual property and
// proprietary rights in and to this software and related documentation and
// any modifications thereto. Any use, reproduction, disclosure, or
// distribution of this software and related documentation without an express
// license agreement from NVIDIA Corporation is strictly prohibited.
//
// ALL NVIDIA DESIGN SPECIFICATIONS, CODE ARE PROVIDED "AS IS.". NVIDIA MAKES
// NO WARRANTIES, EXPRESSED, IMPLIED, STATUTORY, OR OTHERWISE WITH RESPECT TO
// THE MATERIALS, AND EXPRESSLY DISCLAIMS ALL IMPLIED WARRANTIES OF NONINFRINGEMENT,
// MERCHANTABILITY, AND FITNESS FOR A PARTICULAR PURPOSE.
//
// Information and code furnished is believed to be accurate and reliable.
// However, NVIDIA Corporation assumes no responsibility for the consequences of use of such
// information or for any infringement of patents or other rights of third parties that may
// result from its use. No license is granted by implication or otherwise under any patent
// or patent rights of NVIDIA Corporation. Details are subject to change without notice.
// This code supersedes and replaces all information previously supplied.
// NVIDIA Corporation products are not authorized for use as critical
// components in life support devices or systems without express written approval of
// NVIDIA Corporation.
//
// Copyright (c) 2008-2013 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  

#include "PsArray.h"
#include "PxString.h"
#include "pvd/PxVisualDebugger.h"
#include "PxDefaultPvdErrorCallback.h"
#include "CmPhysXCommon.h"
#include "PvdConnection.h"

using namespace physx;

namespace __internal
{
	struct ErrorMsg
	{
		PxAllocatorCallback&	mAllocator;

		PxErrorCode::Enum		mErrorCode;
		char*					mErrorMsg;
		char*					mFileName;
		PxI32					mLineNum;

		ErrorMsg( PxAllocatorCallback& inAllocator, PxErrorCode::Enum inErrorCode, const char* inErrorMsg,
			const char* inFileName, PxI32 inLineNum );
		~ErrorMsg();
	};

	struct MsgAllocator
	{
		PxAllocatorCallback& mAllocator;

		MsgAllocator( PxAllocatorCallback& alloc )
			: mAllocator( alloc )
		{
		}
		MsgAllocator( PxAllocatorCallback* alloc = NULL )
			: mAllocator( *alloc )
		{
			PX_ASSERT(false);
		}
		MsgAllocator( const MsgAllocator& other ) 
			: mAllocator( other.mAllocator )
		{
		}
		MsgAllocator& operator=( const MsgAllocator& )
		{
			return *this;
		}
		PxAllocatorCallback& getAllocator() { return mAllocator; }
		void* allocate(size_t size, const char* filename, int line)
		{
			return getAllocator().allocate(size, "PxDefaultPvdErrorCallback", filename, line);
		}
		void deallocate(void* ptr)
		{
			getAllocator().deallocate(ptr);
		}
	};

	PX_INLINE char* CloneStr( PxAllocatorCallback& inAllocator, const char* inStr )
	{
		PxU32 len = (PxU32) strlen( inStr );
		char* newStr = (char*)inAllocator.allocate( len + 1, "CloneStr", __FILE__, __LINE__ );
		PxStrcpy( newStr, len + 1, inStr );
		newStr[len] = '\0';
		return newStr;
	}

	ErrorMsg::ErrorMsg( PxAllocatorCallback& inAllocator, PxErrorCode::Enum inErrorCode, const char* inErrorMsg,
	const char* inFileName, PxI32 inLineNum )
	: mAllocator( inAllocator )
	, mErrorCode( inErrorCode )
	, mLineNum( inLineNum )
	{

		mErrorMsg = CloneStr(inAllocator, inErrorMsg);
		mFileName = CloneStr(inAllocator, inFileName);
	}

	ErrorMsg::~ErrorMsg()
	{
		if( mErrorMsg )
			mAllocator.deallocate( mErrorMsg );

		if( mFileName )
			mAllocator.deallocate( mFileName );
	}
};

using namespace __internal;

namespace physx
{
	class MessageArray : public shdfnd::Array<ErrorMsg*, MsgAllocator>
	{
	public:
		MessageArray( MsgAllocator inAllocator )
			:shdfnd::Array<ErrorMsg*, MsgAllocator>(inAllocator)
		{}
	};
};


PxDefaultPvdErrorCallback::PxDefaultPvdErrorCallback( PxAllocatorCallback& inAllocator, PxVisualDebugger* inDebugger )
	: mAllocator( inAllocator), mDebugger( inDebugger )
{
	mErrorMsgs = createMessageArray();
}

PxDefaultPvdErrorCallback::~PxDefaultPvdErrorCallback()
{
	clearMessageHistory();
}

MessageArray* PxDefaultPvdErrorCallback::createMessageArray()
{
	void* buffer = mAllocator.allocate( sizeof(MessageArray), "MesssageArray", __FILE__, __LINE__ );
	return new (buffer) MessageArray(MsgAllocator(mAllocator));
}

void PxDefaultPvdErrorCallback::releaseMessageArray(MessageArray* inArray)
{
	if( inArray != NULL )
	{
		inArray->~MessageArray();
		mAllocator.deallocate(inArray);
	}
}

void PxDefaultPvdErrorCallback::setVisualDebugger( PxVisualDebugger* inDebugger )
{
	mDebugger = inDebugger;
}

void PxDefaultPvdErrorCallback::clearMessageHistory()
{
	if( mErrorMsgs != NULL )
	{
		MessageArray& errorMesgs = *mErrorMsgs;
		for( PxU32 i = 0; i < errorMesgs.size(); i++ )
		{
			errorMesgs[i]->~ErrorMsg();
			mAllocator.deallocate(errorMesgs[i]);
			errorMesgs[i] = NULL;
		}

		errorMesgs.clear();
	
		releaseMessageArray(mErrorMsgs);
		mErrorMsgs = NULL;
	}
}

PxDefaultPvdErrorCallback&	PxDefaultPvdErrorCallback::operator=( const PxDefaultPvdErrorCallback& )
{
	PX_ASSERT(false);
	return *this;
}

PxDefaultPvdErrorCallback::PxDefaultPvdErrorCallback( const PxDefaultPvdErrorCallback& another )
:mAllocator( another.mAllocator )
{
	PX_ASSERT(false);
}

#if PX_SUPPORT_VISUAL_DEBUGGER
void PxDefaultPvdErrorCallback::reportError(PxErrorCode::Enum inErrorCode, 
	const char* inErrorMsg, const char* inFileName, int inLineNum)
{
	if( mDebugger == NULL 
		|| mDebugger->getPvdConnectionFactory() == NULL 
		|| !mDebugger->getPvdConnectionFactory()->isConnected() )
	{
		if( mErrorMsgs == NULL )
		{
			mErrorMsgs = createMessageArray();
		}

		void* buffer = mAllocator.allocate( sizeof(ErrorMsg), "PvdErrorStream::ErrorMsg", __FILE__, __LINE__);
		ErrorMsg* msg = new (buffer) ErrorMsg( mAllocator, inErrorCode, inErrorMsg, inFileName, inLineNum );
		mErrorMsgs->pushBack(msg);
	}
	else
	{
		if( mErrorMsgs != NULL )
		{
			MessageArray& errorMesgs = *mErrorMsgs;

			for( PxU32 i = 0; i < errorMesgs.size(); i++ )
			{
				mDebugger->sendErrorMessage( errorMesgs[i]->mErrorCode
					, errorMesgs[i]->mErrorMsg
					, errorMesgs[i]->mFileName
					, errorMesgs[i]->mLineNum);
			}
		}

		clearMessageHistory();

		mDebugger->sendErrorMessage( inErrorCode, inErrorMsg, inFileName, inLineNum );
		// sending msg to pvd
	}
}
#else
void PxDefaultPvdErrorCallback::reportError(PxErrorCode::Enum, const char* , const char* , int )
{}
#endif
