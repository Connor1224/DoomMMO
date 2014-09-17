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


//#ifdef REMOVED

// PX_SERIALIZATION

//#define DEBUG_STREAM_SIZES

//#pragma warning( disable : 4512 ) //  assignment operator could not be generated

#include "PxErrorCallback.h"
#include "SnSerialFramework.h"
#include "PsIntrinsics.h"
#include "CmStringTable.h"
#include "stdio.h"
#include "serialization/SnSerializationRegistry.h"
#include "common/PxSerialUtils.h"
#include "PxSerializer.h"

#ifdef PX_DEBUG
	#define SERIAL_STATS
#endif

namespace physx {

#ifdef SERIAL_STATS
#include <stdio.h>
#endif

namespace Sn
{
	struct SerialStats
	{
		PX_INLINE SerialStats() : mClassName(NULL), mCount(0), mTotalSize(0)	{}
		PX_INLINE SerialStats(const char* className, PxU32 count, PxU32 totalSize) 
			: mClassName(className), mCount(count), mTotalSize(totalSize)	{}
		const char*	mClassName;
		PxU32		mCount;
		PxU32		mTotalSize;
	};

	static const bool gIncludeSentinels = false;
	
	typedef bool (*ProcessSerializableCallback)	(PxBase* s, void* userData);
	static bool processCollection(Cm::InternalCollection& c, ProcessSerializableCallback cb, void* userData)
	{
		const PxU32 nb = c.internalGetNbObjects();
		for(PxU32 i=0;i<nb;i++)
		{
			PxBase* s = c.internalGetObject(i);
			//		if(s->isSerializationDisabled())
			//			continue;
			if(!(cb)(s, userData))
				return false;
		}
		return true;
	}

	#define MAX_REGISTERED_NAMES	64	// PT: "64 names should be enough for everybody"... or something.
	struct RegisteredName
	{
		const char*		mName;
		const char**	mNameAddress;
	};
	class InternalNameManager : public PxNameManager
	{
	public:
		InternalNameManager() : mNbRegisteredNames(0)
		{
		}
		virtual	void registerName(const char** name)
		{
			if(*name==NULL)
				return;

			PX_ASSERT(mNbRegisteredNames<MAX_REGISTERED_NAMES);
			mRegisteredNames[mNbRegisteredNames].mNameAddress = name;
			mRegisteredNames[mNbRegisteredNames].mName = *name;
			mNbRegisteredNames++;
		}
		PxU32			mNbRegisteredNames;
		RegisteredName	mRegisteredNames[MAX_REGISTERED_NAMES];
	};

	struct ExportParams
	{
		PxCollection*					mCollection;
		PxSerialStream*					mStream;
		physx::shdfnd::Array<char>*		mStringTable;
		PxSerializationRegistry*		mSr;
	};

	void serializeCollection(Cm::InternalCollection& collection, PxSerialStream& stream, const Cm::InternalCollection* externalReferences, bool exportNames, PxSerializationRegistry& sr)
	{
		PX_UNUSED(externalReferences);

		struct Local
		{
			static bool ExportSerializableToStream(PxBase* s, void* userData)
			{

				PX_ASSERT(s->getConcreteType());
				//			PxSerialStream* stream = reinterpret_cast<PxSerialStream*>(userData);

				ExportParams* params = (ExportParams*)userData;
				PxSerialStream* stream = params->mStream;
				physx::shdfnd::Array<char>* stringTable = params->mStringTable;
				PxSerializationRegistry* sr = params->mSr;

				stream->align();

				#ifdef DEBUG_STREAM_SIZES
					printf("%d\n", stream->getTotalStoredSize());
				#endif
				

				if(gIncludeSentinels)
				{
					int dead = 0xdead;
					stream->storeBuffer(&dead, 4);
				}

				// PT: gather names to export
				InternalNameManager nameManager;				
				const PxSerializer* bs = sr->getSerializer(s->getConcreteType());
				PX_ASSERT(bs);
				bs->registerNameForExport(*s, nameManager);

				// PT: convert pointer to indices for exported names
				for(PxU32 i=0;i<nameManager.mNbRegisteredNames;i++)
				{
					const RegisteredName& name = nameManager.mRegisteredNames[i];
					size_t* target = reinterpret_cast<size_t*>(name.mNameAddress);
					if(stringTable)
						*target = 1 + Cm::addToStringTable(*stringTable, name.mName);	// PT: +1 because we reserve 0 for NULL pointer (no name)
					else
						*target = 0;
				}

				// PT: export object to file with converted names				
				bs->exportData(*s, *stream);

				// PT: restore original name pointers (undo pointer-to-indice conversion)
				for(PxU32 i=0;i<nameManager.mNbRegisteredNames;i++)
				{
					const RegisteredName& name = nameManager.mRegisteredNames[i];
					*name.mNameAddress = name.mName;
				}

				if(gIncludeSentinels)
				{
					int dead = 0xdead;
					stream->storeBuffer(&dead, 4);
				}

				return true;
			}

			static bool ExportExtraData(PxBase* s, void* userData)
			{
				ExportParams* params = (ExportParams*)userData;
				PxSerialStream* stream = params->mStream;
				stream->align();

				if(gIncludeSentinels)
				{
					int dead = 0xdead;
					stream->storeBuffer(&dead, 4);
				}

				//s->exportExtraData(*stream, params->mCollection);
				PxSerializationRegistry* sr = params->mSr;
				const PxSerializer* bs = sr->getSerializer(s->getConcreteType());
				PX_ASSERT(bs);
				bs->exportExtraData(*s, *stream, params->mCollection);

				if(gIncludeSentinels)
				{
					int dead = 0xdead;
					stream->storeBuffer(&dead, 4);
				}

				return true;
			}
		};		

		physx::shdfnd::Array<char>	stringTable;

		ExportParams params;
		params.mCollection  = &collection;
		params.mStream		= &stream;
		params.mStringTable	= exportNames ? &stringTable : NULL;
		params.mSr = &sr;
		processCollection(collection, Local::ExportSerializableToStream, &params);

		//	PxAlignStream(stream);
		//processCollection(collection, Local::ExportFields, &params);
		processCollection(collection, Local::ExportExtraData, &params);

		// PT: export string table, if there is one
		const PxU32 length = stringTable.size();
		const char* table = stringTable.begin();
		stream.storeBuffer(&length, sizeof(PxU32));
		if(length)
			stream.storeBuffer(table, length);
	}

	static char* read32(char* address, PxU32& data)
	{
		PxU32* p = reinterpret_cast<PxU32*>(address);
		data = *p;
		address += sizeof(PxU32);
		return address;
	}

	class PtrResolver : public PxPtrManager
	{
	public:
		PtrResolver(Cm::RefResolver& resolver, PxU32 maxPtrs, void** ptrs) : mResolver(resolver), mOldAddresses(ptrs), mMaxPtrs(maxPtrs), mIndex(0)	{}

		virtual	void registerPtr(void* ptr)
		{
			PX_ASSERT(mIndex<mMaxPtrs);
			mResolver.setNewAddress(mOldAddresses[mIndex++], ptr);
		}

		Cm::RefResolver&	mResolver;
		void**					mOldAddresses;
		PxU32					mMaxPtrs;
		PxU32					mIndex;
	private:
		PtrResolver& operator=(const PtrResolver&);
	};

	bool deserializeCollection(Cm::InternalCollection& collection, Cm::RefResolver& Ref, void* buffer, PxU32 nbObjectsInCollection, PxU32 nbOldAddresses, void** oldAddresses, PxSerializationRegistry& sr)
	{
#ifdef SERIAL_STATS
		Ps::HashMap<PxType, SerialStats> stats;
#endif
		PxU32 totalPadding = 0;

		char* Address = reinterpret_cast<char*>(buffer);

		PxU32 nbObjects = nbObjectsInCollection;

	#ifdef PX_DEBUG
		PxU32 nbClasses = 0;
	#endif

		while(nbObjects--)
		{
			Address = PxAlignStream(Address, totalPadding);

#ifdef SERIAL_STATS
			char* BaseAddress = Address;
#endif

			PxBase* H = reinterpret_cast<PxBase*>(Address);
			const PxType classType = H->getConcreteType();

			const PxSerializer* bs = sr.getSerializer(classType);
			PX_ASSERT(bs);
			PxBase* Serial = bs->createBinarySerializable(Address, Ref);
			PX_ASSERT(Serial);

#ifdef SERIAL_STATS
			if(Serial)
			{
				
				const Ps::HashMap<PxType, SerialStats>::Entry* e = stats.find(classType);
				if( e )
				{
					SerialStats& stat( const_cast<SerialStats&>( e->second ) );

					stat.mCount++;
					stat.mTotalSize += PxU32(Address - BaseAddress);
				}
				else
				{
					const char* name = Serial->getConcreteTypeName();
					PX_ASSERT(name);
					stats.insert(classType, SerialStats(name, 1, PxU32(Address - BaseAddress)));
				}
#ifdef DEBUG_STREAM_SIZES
				static PxU32 tsize = 8;
				tsize += PxU32(Address - BaseAddress);
				printf("%d\n", tsize);
				//			printf("%d\n", PxU32(Address - BaseAddress));
#endif
			}
#endif

			if(Serial)
			{
				collection.internalAdd(Serial);
#ifdef PX_DEBUG
				nbClasses++;
#endif
			}
		}
		PX_ASSERT(nbObjectsInCollection == collection.internalGetNbObjects());

		// Update ref-resolver
		{
			PtrResolver ptrResolver(Ref, nbOldAddresses, oldAddresses);

			struct LocalParams
			{
				PtrResolver* resolver;
				PxSerializationRegistry* mSr;
			};

			struct Local
			{
				static bool ImportOldAddress(PxBase* s, void* userData)
				{
					LocalParams* params = reinterpret_cast<LocalParams*>(userData);
					PtrResolver* ptrResolver = params->resolver;
					
					PxSerializationRegistry* reg = params->mSr;
					const PxSerializer* bs = reg->getSerializer(s->getConcreteType());
					PX_ASSERT(bs);
					bs->registerPtrsForExport(*s, *ptrResolver);
					
					return true;
				}
			};

			LocalParams params;
			params.resolver = &ptrResolver;
			params.mSr = &sr;
			if(!processCollection(collection, Local::ImportOldAddress, &params))
				return false;
		}
		
		{
			//sschirm: extracted from Local because of osx internal compile error
			struct LocalImportParams
			{
				char**								mAddress;
				PxU32								mTotalPadding;
				PxSerializationRegistry*			mSr;
#ifdef SERIAL_STATS
				Ps::HashMap<PxType, SerialStats>*	mStats;
#endif
			};

			struct Local
			{
				static bool ImportExtraData(PxBase* s, void* userData)
				{
					LocalImportParams* params = reinterpret_cast<LocalImportParams*>(userData);
					char* Address = *params->mAddress;

					Address = PxAlignStream(Address, params->mTotalPadding);
#ifdef SERIAL_STATS
					char* previousAddress = Address;
#endif					
					PxSerializationRegistry* reg = params->mSr;
					const PxSerializer* bs = reg->getSerializer(s->getConcreteType());
					PX_ASSERT(bs);
					Address = bs->importExtraData(*s, Address, params->mTotalPadding);
#ifdef SERIAL_STATS
					const Ps::HashMap<PxType, SerialStats>::Entry* e = params->mStats->find(s->getConcreteType());
					PX_ASSERT(e);
					SerialStats& stat( const_cast<SerialStats&>( e->second ) );
					stat.mTotalSize += PxU32(Address - previousAddress);
#endif
					*params->mAddress = Address;
					return true;
				}			
			};

			LocalImportParams importParams;
			importParams.mAddress		= &Address;
			importParams.mTotalPadding	= 0;
			importParams.mSr = &sr;
#ifdef SERIAL_STATS
			importParams.mStats			= &stats;
#endif
			//processCollection(collection, Local::ImportFields, &importParams);
			processCollection(collection, Local::ImportExtraData, &importParams);
			totalPadding += importParams.mTotalPadding;
		}

		// PT: import string table
		PxU32 stringTableSize;
		Address = read32(Address, stringTableSize);

		const char* stringTable = NULL;
		if(stringTableSize)
		{
			stringTable = Address;
			Address += stringTableSize;
		}
		Ref.setStringTable(stringTable);
		
#ifdef SERIAL_STATS
		Ps::HashMap<PxType, SerialStats>::Iterator it = stats.getIterator();
		while (!it.done())
		{
			SerialStats stat = it->second;
			if(stat.mCount)
			{
				PX_ASSERT(stat.mClassName);
				printf("%s | %d | %d\n", stat.mClassName, stat.mCount, stat.mTotalSize);
			}
			++it;
		}
		printf("Padding: %d bytes\n", totalPadding);
#endif

		return true;
	}

}
}
