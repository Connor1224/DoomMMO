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
#include "PsIntrinsics.h"
#include "CmStringTable.h"
#include "extensions/PxSerialization.h"
#include "SnSerialFramework.h"
#include "PxSerializer.h"
#include "../SnSerializationRegistry.h"

using namespace physx;
using namespace physx::Cm;

namespace 
{
	class PtrManager : public PxPtrManager
	{
	public:
		PtrManager()	{}

		virtual	void	registerPtr(void* ptr)
		{
			mPtrs.pushBack(ptr);
		}

		void	exportPtrsToStream(PxSerialStream& stream)
		{
			const PxU32 size = mPtrs.size();
			stream.storeBuffer(&size, sizeof(PxU32));

			for(PxU32 i=0;i<size;i++)
			{
				void* ptr = mPtrs[i];
				stream.storeBuffer(&ptr, sizeof(void*));
			}
		}

		Ps::Array<void*>	mPtrs;
	};
}

static void serializeCollectedObject(PxSerialStream& s, const Ps::Array<CollectedObject>& collectedObjects)
{
	PxU32 size = collectedObjects.size();
	s.storeBuffer(&size, sizeof(PxU32));
	if(size)
	{
		// PT: this works but the size depends a lot on padding now that our collected objects
		// contain a mix of 32bit and 64bit values. It makes it tedious to parse in the binary converter.
		//		s.storeBuffer(collectedObjects.begin(), size*sizeof(CollectedObject));

		// PT: so instead we will output the members one at a time, exactly as we want them
		const CollectedObject* objects = collectedObjects.begin();
		while(size--)
		{
			s.storeBuffer(&objects->mObject, sizeof(void*));
			s.storeBuffer(&objects->mInternalUserRef.mUserID, sizeof(PxSerialObjectRef));
			s.storeBuffer(&objects->mInternalUserRef.mFlags, sizeof(PxU32));
			objects++;
		}
	}
}

bool PxSerialization::serializeCollectionToBinary(PxOutputStream& outputStream, PxCollection& collection, PxSerializationRegistry& sr, const PxCollection* externalRefs, bool exportNames)
{
	if(!PxSerialization::isSerializable(collection, sr, externalRefs))
		return false; 
		
	InternalCollection& interCollection = static_cast<InternalCollection&>(collection);
	
	OutputStreamWriter writer(outputStream);
	LegacySerialStream s(writer);

	struct Header
	{
		PxU32					header;
		PxU32					version;
		PxU32					buildNumber;
		PxU32					platformTag;
		PxU32					markedPadding;
		PxU32					materialOffset;
		PxU32					nbExportNames;
		CollectedObject*		exportNames;
		PxU32					nbImportNames;
		CollectedObject*		importNames;
	};


	//serialized binary data.
	const PxU32 header = PX_MAKE_FOURCC('S','E','B','D');
	s.storeBuffer(&header, sizeof(PxU32));

	PxU32 version = PX_PHYSICS_VERSION;
	s.storeBuffer(&version, sizeof(PxU32));

	PxU32 buildNumber = 0;
#if defined(PX_BUILD_NUMBER)
	buildNumber =  PX_BUILD_NUMBER;
#endif
	s.storeBuffer(&buildNumber, sizeof(PxU32));
	
	PxU32 platformTag = getBinaryPlatformTag();
	s.storeBuffer(&platformTag, sizeof(PxU32));

	PxU32 markedPadding = 0;
#ifdef PX_CHECKED
	markedPadding = 1;
#endif
	s.storeBuffer(&markedPadding, sizeof(PxU32));
	
	//serializeCollectedObject should be changed to work on the original data

	// Write external export references 
	{
		PxU32 nbReferences = collection.getNbReferences();
		Ps::Array<CollectedObject> exportNames;
		exportNames.reserve(nbReferences);
		if(nbReferences)
		{
			Ps::Array<PxSerialObjectRef> refArray(nbReferences);
			collection.getReferences(&refArray[0], nbReferences);

			
			for(PxU32 i = 0; i < nbReferences; ++i)
			{
				PxSerialObjectRef ref = refArray[i];
				PxBase*	obj = collection.find(ref);
				const InternalUserRef internalRef(ref, 0x80000000);
				exportNames.pushBack(CollectedObject(obj, internalRef));
			}
		
		}
		serializeCollectedObject(s, exportNames);
	}


	// Write external import references
	{
		Ps::Array<CollectedObject> importNames;
		if (externalRefs)
		{
			PxU32 nbReferences = externalRefs->getNbReferences();
			//TODO: maybe we should select the objects that are actually referenced by our collection!
			//at least we should verify that we have all!
			importNames.reserve(nbReferences);
			//same iterator hack as above! Needs fixing!

			if(nbReferences)
			{
				Ps::Array<PxSerialObjectRef> refArray(nbReferences);
				externalRefs->getReferences(&refArray[0], nbReferences);

				
				for(PxU32 i = 0; i < nbReferences; ++i)
				{
					PxSerialObjectRef ref = refArray[i];
					PxBase*	obj = externalRefs->find(ref);
					const InternalUserRef internalRef(ref, 0x80000000);
					importNames.pushBack(CollectedObject(obj, internalRef));
				}
			}
		}
		serializeCollectedObject(s, importNames);
	}

		
	// PT: sort collection by "order" value. We need to do this prior to storing old addresses.
	const PxU32 nbObjects = interCollection.internalGetNbObjects();
	
	sortCollection(collection, sr);

	// Old addresses
	{
		s.storeBuffer(&nbObjects, sizeof(PxU32));

		// PT: TODO: WARNING: number of objects in the collection is now DIFFERENT from number of exported old addresses!
		PtrManager ptrManager;
		for(PxU32 i=0;i<nbObjects;i++)
		{
			PxBase* obj = interCollection.internalGetObject(i);          
			const PxSerializer* bs = sr.getSerializer(obj->getConcreteType());
			PX_ASSERT(bs);
			bs->registerPtrsForExport(*obj, ptrManager);
		}
		ptrManager.exportPtrsToStream(s);
	}
	
	Sn::serializeCollection(interCollection, s, static_cast<const InternalCollection*>(externalRefs), exportNames, sr);
	return true;
}
