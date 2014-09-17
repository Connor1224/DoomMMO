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

#include "PsHash.h"
#include "PsHashMap.h"
#include "CmIO.h"
#include "PxFileBuffer.h"
#include "PsFile.h"
#include "PsString.h"
#include "extensions/PxSerialization.h"
#include "PxPhysics.h"
#include "SnSerialFramework.h"
#include "PxSerializer.h"

using namespace physx;

namespace
{
	PX_FORCE_INLINE PxU32 read32(void*& buffer)
	{
		char* address = (char*)buffer;
		const PxU32 value = *(PxU32*)address;
		address += sizeof(PxU32);
		buffer = address;
		return value;
	}

	PX_FORCE_INLINE PxU64 read64(void*& buffer)
	{
		char* address = (char*)buffer;
		const PxU64 value = *(PxU64*)address;
		address += sizeof(PxU64);
		buffer = address;
		return value;
	}

	PX_FORCE_INLINE void* readPtr(void*& buffer)
	{
		char* address = (char*)buffer;
		void* value = *(void**)address;
		address += sizeof(void*);
		buffer = address;
		return value;
	}

}

PxCollection* PxSerialization::createCollectionFromBinary(void* memBlock, PxSerializationRegistry& sr, const PxCollection* externalRefs)
{

#ifdef PX_CHECKED
		if(size_t(memBlock) & (PX_SERIAL_FILE_ALIGN-1))
		{
			Ps::getFoundation().error(PxErrorCode::eINVALID_PARAMETER, __FILE__, __LINE__, "Buffer must be 128-bytes aligned.");
			return NULL;
		}
#endif
//
		const PxU32 header = read32(memBlock);
		PX_UNUSED(header);
		const PxU32 version = read32(memBlock);
		PX_UNUSED(version);
		const PxU32 buildNumber = read32(memBlock);
		PX_UNUSED(buildNumber);
		const PxU32 platformTag = read32(memBlock);
		PX_UNUSED(platformTag);
		const PxU32 markedPadding = read32(memBlock);
		PX_UNUSED(markedPadding);

#if PX_CHECKED
		if (header != PX_MAKE_FOURCC('S','E','B','D'))
		{
			Ps::getFoundation().error(physx::PxErrorCode::eINVALID_PARAMETER, __FILE__, __LINE__, 
				"Buffer contains data with wrong header indicating invalid binary data.");
			return NULL;
		}

		if (version != PX_PHYSICS_VERSION)
		{
			Ps::getFoundation().error(physx::PxErrorCode::eINVALID_PARAMETER, __FILE__, __LINE__, 
				"Buffer contains data with wrong serialization format version:\nExpected: 0x%x \nActual: 0x%x\n",
				PX_PHYSICS_VERSION,
				version);
			return NULL;
		}

		if (platformTag != Cm::getBinaryPlatformTag())
		{
			Ps::getFoundation().error(physx::PxErrorCode::eINVALID_PARAMETER, __FILE__, __LINE__, 
				"Buffer contains data with platform mismatch:\nExpected: %s \nActual: %s\n",
				Cm::getBinaryPlatformName(Cm::getBinaryPlatformTag()),
				Cm::getBinaryPlatformName(platformTag));
			return NULL;
		}
#endif

		Cm::RefResolver resolver;

		// Export names
		Cm::CollectedObject* collectedObjects = NULL;
		PxU32 nbCollectedObjects = 0;
		{
			nbCollectedObjects = read32(memBlock);

			if(nbCollectedObjects)
			{
				// PT: TODO: get rid of this alloc
				collectedObjects = (Cm::CollectedObject*)PX_ALLOC_TEMP(sizeof(Cm::CollectedObject)*nbCollectedObjects, PX_DEBUG_EXP("CollectedObject"));

				for(PxU32 i=0;i<nbCollectedObjects;i++)
				{
					collectedObjects[i].mObject						= readPtr(memBlock);
					collectedObjects[i].mInternalUserRef.mUserID	= read64(memBlock);
					collectedObjects[i].mInternalUserRef.mFlags		= read32(memBlock);
				}
			}
		}

		// Import names
		{
			PxU32 nbNames = read32(memBlock);

			//todo, nbNames > 0 seems to indicate that we need to have an externalRefs
			if (nbNames > 0 && !externalRefs)
			{
				Ps::getFoundation().error(physx::PxErrorCode::eINVALID_PARAMETER, __FILE__, __LINE__, 
					"Serialized collection references external objects but parameter externalRefs is NULL.");

				if(collectedObjects)
					PX_FREE(collectedObjects);
			
				return NULL;
			}

			for(PxU32 i=0;i<nbNames;i++)
			{
				void* coObject				= readPtr(memBlock);
				PxSerialObjectRef coUserID	= read64(memBlock);
				PxU32 coFlags				= read32(memBlock);
				PX_UNUSED(coFlags);
				void* newObject = externalRefs->find(coUserID);
				PX_ASSERT(newObject);
				resolver.setNewAddress(coObject, newObject);
			}
		}

		// Old addresses - we can't update the ref resolver immediately since the new addresses are not known yet.
		void** oldAddresses;
		PxU32 nbObjectsInCollection;
		PxU32 nbOldAddresses;
		{
			char* address = (char*)memBlock;
			nbObjectsInCollection = *(PxU32*)address;
			address += sizeof(PxU32);

			nbOldAddresses = *(PxU32*)address;
			address += sizeof(PxU32);

			oldAddresses = reinterpret_cast<void**>(address);
			address += nbOldAddresses * sizeof(void*);
			memBlock = address;
		}

		PxCollection* collection = sr.getPhysics().createCollection();
		Cm::InternalCollection* internalCollection = static_cast<Cm::InternalCollection*>(collection);

		if(!Sn::deserializeCollection(*internalCollection, static_cast<Cm::RefResolver&>(resolver), memBlock, nbObjectsInCollection, nbOldAddresses, oldAddresses, sr))
		{
			if(collectedObjects)
				PX_FREE(collectedObjects);
			collection->release();
			return NULL;
		}

		Ps::HashMap<PxU16, PxU16>  materialIndexTranslator;		
		
		const PxU32 nb = internalCollection->internalGetNbObjects();
		if (nb)
		{
			PxBase** objects = internalCollection->internalGetObjects();
			PxBase** last = objects + nb;
			while(objects!=last)
			{
				PxBase* s = *objects++;
				
				const PxSerializer* bs = sr.getSerializer(s->getConcreteType());
				PX_ASSERT(bs);
				if(!bs->resolvePointers(*s, resolver, &materialIndexTranslator))
				{
					if(collectedObjects)
						PX_FREE(collectedObjects);
					collection->release();
					return NULL;
				}
			}
		}

		for(PxU32 i=0;i<nbCollectedObjects;i++)
		{
			const Cm::CollectedObject& co = collectedObjects[i];

			void* newAddress = resolver.newAddress(co.mObject);
			PX_ASSERT(newAddress);
			if(newAddress)
			{
				//todo use internal function
				collection->addReference(*static_cast<PxBase*>(newAddress), co.mInternalUserRef.mUserID);
			}
		}

		if(collectedObjects)
			PX_FREE(collectedObjects);

		internalCollection->internalAddObjectsToFactory();
		return collection;

}
