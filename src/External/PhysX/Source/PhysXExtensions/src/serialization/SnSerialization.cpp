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

#include "PxPhysicsAPI.h"
#include "PxConstraintExt.h"
#include "PsFoundation.h"
#include "PxMetaData.h"
#include "SnSerialFramework.h"
#include "CmStringTable.h"
#include "SnConvX.h"
#include "SnSerializationRegistry.h"
#include "ExtSerialization.h"
#include "PxSerializer.h"

using namespace physx;

namespace
{
	bool isRoot(PxBase& object, const PxCollection&, PxSerializationRegistry&)
	{
		bool bRet = true;

		PxType type = object.getConcreteType();
		switch(type)
		{
		case PxConcreteType::eSHAPE:
			{
				PxShape& shape = reinterpret_cast<PxShape&>(object);
				if(shape.isExclusive())
					bRet = false;
				break;
			}
		case PxConcreteType::eARTICULATION_LINK:
		case PxConcreteType::eCONSTRAINT:
		case PxConcreteType::eCLOTH_FABRIC:
			bRet = false;
			break;
		default:
			break;
		}

		return bRet;
	}

	void getRequiresCollection(PxCollection& collection, PxSerializationRegistry& sr, PxCollection& requiresCollection, bool followJoints)
	{
		for (PxU32 i = 0; i < collection.getNbObjects(); ++i)
		{
			PxBase& s = collection.getObject(i);
			//s.requires(requiresCollection);	
			const PxSerializer* bs = sr.getSerializer(s.getConcreteType());
			bs->requires(s, requiresCollection);

			if(followJoints)
			{
				PxRigidActor* actor = s.is<PxRigidActor>();
				if(actor)
				{
					Ps::Array<PxConstraint*> objects(actor->getNbConstraints());
					actor->getConstraints(objects.begin(), objects.size());

					for(PxU32 j=0;j<objects.size();j++)
					{
						PxU32 typeId;
						PxJoint* joint = reinterpret_cast<PxJoint*>(objects[j]->getExternalReference(typeId));				
						if(typeId == PxConstraintExtIDs::eJOINT)
						{
							//joint->requires(requiresCollection);
							const PxSerializer* bs = sr.getSerializer(joint->getConcreteType());
							bs->requires(*joint, requiresCollection);
							if(!requiresCollection.contains(*joint))
								requiresCollection.add(*joint);							
						}
					}
				}
			}
		}
	}	
}

bool PxSerialization::isSerializable(PxCollection& collection, PxSerializationRegistry& sr, const PxCollection* externalReferences) 
{		
	bool bRet = true;
	PxPhysics& physics = sr.getPhysics();
	PxCollection* curCollection = physics.createCollection();
	PxCollection* nonRootCollection = physics.createCollection();

	for(PxU32 i = 0; i < collection.getNbObjects(); ++i)
	{
		PxBase& s = collection.getObject(i);
		if(isRoot(s, collection, sr))
			curCollection->add(s);							
		else
			nonRootCollection->add(s);	

		if(externalReferences)
		{
			PxSerialObjectRef ref = collection.getReference(s);
			if(ref != PX_SERIAL_OBJECT_REF_INVALID)
			{
				PxBase* object = externalReferences->find(ref);
				if(object && (object != &s))
				{
					curCollection->release();
					nonRootCollection->release();
					Ps::getFoundation().error(physx::PxErrorCode::eINVALID_PARAMETER, __FILE__, __LINE__, 
						"NpCollection::isSerializable: object shares reference name with other object in externalReferences (reference clash).");
					return false;
				}
			}
		}		
	}

	PxCollection* requiresCollection = physics.createCollection();

	do
	{
		PxCollection* tmpCollection = physics.createCollection();
		for (PxU32 i = 0; i < curCollection->getNbObjects(); ++i)
		{
			PxBase& s = curCollection->getObject(i);
			const PxSerializer* bs = sr.getSerializer(s.getConcreteType());
			bs->requires(s, *tmpCollection);
		}
		requiresCollection->add(*tmpCollection);			
		curCollection->release();
		curCollection = tmpCollection;			
	}while(curCollection->getNbObjects() > 0);

	curCollection->release();

	for(PxU32 j = 0; j < nonRootCollection->getNbObjects(); ++j)
	{
		PxBase& nonroot = nonRootCollection->getObject(j);
		bRet = requiresCollection->contains(nonroot);
		if(!bRet)
		{
			requiresCollection->release();
			nonRootCollection->release();
			Ps::getFoundation().error(physx::PxErrorCode::eINVALID_PARAMETER, __FILE__, __LINE__, 
				"NpCollection::isSerializable: object is neither a root nor required by other objects in the collection (orphan).");
			return false;		
		}
	}
	nonRootCollection->release();

	for(PxU32 j = 0; j < requiresCollection->getNbObjects(); ++j)
	{
		PxBase& s0 = requiresCollection->getObject(j);
		bRet = collection.contains(s0);
		if(!bRet && externalReferences)
		{
			bRet = externalReferences->contains(s0) &&  externalReferences->getReference(s0) != PX_SERIAL_OBJECT_REF_INVALID;
		}
		if(!bRet)
		{
			requiresCollection->release();
			Ps::getFoundation().error(physx::PxErrorCode::eINVALID_PARAMETER, __FILE__, __LINE__, 
				"NpCollection::isSerializable: cannot find a required serial object (collection is not complete).");
			return false;
		}
	}
	requiresCollection->release();

	if(externalReferences)
	{
		PxCollection* oppsiteRequiresCollection = physics.createCollection();
		for (PxU32 i = 0; i < externalReferences->getNbObjects(); ++i)
		{
			PxBase& s = externalReferences->getObject(i);			
			const PxSerializer* bs = sr.getSerializer(s.getConcreteType());
			bs->requires(s, *oppsiteRequiresCollection);
		}

		for(PxU32 j = 0; j < oppsiteRequiresCollection->getNbObjects(); ++j)
		{
			PxBase& s0 = oppsiteRequiresCollection->getObject(j);
			if(!externalReferences->contains(s0))			
			{
				if(collection.contains(s0))
				{
					oppsiteRequiresCollection->release();
					Ps::getFoundation().error(physx::PxErrorCode::eINVALID_PARAMETER, __FILE__, __LINE__, 
						"NpCollection::isSerializable: object in externalReferences requires an object in collection (circular dependency).");
					return false;
				}
			}
		}
		oppsiteRequiresCollection->release();
	}

	return true;
}

void PxSerialization::complete(PxCollection& collection, PxSerializationRegistry& sr, const PxCollection* exceptFor, bool followJoints)
{
	PxPhysics& physics = sr.getPhysics();
	PxCollection* curCollection = physics.createCollection();

	curCollection->add(collection);
	do
	{
		PxCollection* requiresCollection = physics.createCollection();

		getRequiresCollection(*curCollection, sr, *requiresCollection, followJoints);

		//remove object from requireCollection if it in collection or exceptFor
		PxCollection* removeCollection = physics.createCollection();
		for (PxU32 i = 0; i < requiresCollection->getNbObjects(); ++i)
		{
			PxBase& s = requiresCollection->getObject(i);
			if(collection.contains(s) || (exceptFor && exceptFor->contains(s)))
				removeCollection->add(s);				    
		}
		requiresCollection->remove(*removeCollection);
		removeCollection->release();

		collection.add(*requiresCollection);
		curCollection->release();
		curCollection = requiresCollection;

	}while(curCollection->getNbObjects() > 0);

	curCollection->release();
}

void PxSerialization::createNames(PxCollection& collection, const PxSerialObjectRef base)
{
	PxSerialObjectRef localBase = base;
	PxU32 nbObjects = collection.getNbObjects();

	for (PxU32 i = 0; i < nbObjects; ++i)
	{
		while(collection.find(localBase))
		{
			localBase++;
		}

		PxBase& s = collection.getObject(i);		
		if(PX_SERIAL_OBJECT_REF_INVALID == collection.getReference(s))
		{
			collection.addReference(s, localBase);
			localBase++;
		}
	}
}

void PxSerialization::remove(PxCollection& collection, PxType serialType, PxSerializationRegistry& sr, PxCollection* to)
{
	PxCollection* tmpCollection = sr.getPhysics().createCollection();
	for (PxU32 i = 0; i < collection.getNbObjects(); i++)
	{
		PxBase& object = collection.getObject(i);
		if(serialType == object.getConcreteType())
			tmpCollection->add(object);
	}

	collection.remove(*tmpCollection);
	if (to)
		to->add(*tmpCollection);

	tmpCollection->release();
}

PxCollection* PxSerialization::createCollection(PxPhysics& physics)
{
	PxCollection* collection = physics.createCollection();
	if (!collection)
		return NULL;

	// Collect convexes
	{
		shdfnd::Array<PxConvexMesh*> objects(physics.getNbConvexMeshes());
		const PxU32 nb = physics.getConvexMeshes(objects.begin(), objects.size());
		PX_ASSERT(nb == objects.size());
		PX_UNUSED(nb);

		for(PxU32 i=0;i<objects.size();i++)
			collection->add(*objects[i]);
	}

	// Collect triangle meshes
	{
		Ps::Array<PxTriangleMesh*> objects(physics.getNbTriangleMeshes());
		const PxU32 nb = physics.getTriangleMeshes(objects.begin(), objects.size());

		PX_ASSERT(nb == objects.size());
		PX_UNUSED(nb);

		for(PxU32 i=0;i<objects.size();i++)
			collection->add(*objects[i]);
	}

	// Collect heightfields
	{
		Ps::Array<PxHeightField*> objects(physics.getNbHeightFields());
		const PxU32 nb = physics.getHeightFields(objects.begin(), objects.size());

		PX_ASSERT(nb == objects.size());
		PX_UNUSED(nb);

		for(PxU32 i=0;i<objects.size();i++)
			collection->add(*objects[i]);
	}

	// Collect materials
	{
		Ps::Array<PxMaterial*> objects(physics.getNbMaterials());
		const PxU32 nb = physics.getMaterials(objects.begin(), objects.size());

		PX_ASSERT(nb == objects.size());
		PX_UNUSED(nb);

		for(PxU32 i=0;i<objects.size();i++)
			collection->add(*objects[i]);
	}

#if PX_USE_CLOTH_API
	// Collect cloth fabrics
	{
		Ps::Array<PxClothFabric*> objects(physics.getNbClothFabrics());
		const PxU32 nb = physics.getClothFabrics(objects.begin(), objects.size());

		PX_ASSERT(nb == objects.size());
		PX_UNUSED(nb);

		for(PxU32 i=0;i<objects.size();i++)
			collection->add(*objects[i]);
	}
#endif

	// Collect shared shapes
	{
		Ps::Array<PxShape*> objects(physics.getNbShapes());
		const PxU32 nb = physics.getShapes(objects.begin(), objects.size());

		PX_ASSERT(nb == objects.size());
		PX_UNUSED(nb);

		for(PxU32 i=0;i<objects.size();i++)
			collection->add(*objects[i]);
	}
	return collection;
}

PxCollection* PxSerialization::createCollection(PxScene& scene)
{
	PxCollection* collection = scene.getPhysics().createCollection();
	if (!collection)
		return NULL;

	// Collect actors
	{
		PxActorTypeFlags selectionFlags = PxActorTypeFlag::eRIGID_STATIC | PxActorTypeFlag::eRIGID_DYNAMIC;

#if PX_USE_PARTICLE_SYSTEM_API
		selectionFlags |= PxActorTypeFlag::ePARTICLE_SYSTEM | PxActorTypeFlag::ePARTICLE_FLUID;
#endif
#if PX_USE_CLOTH_API
		selectionFlags |= PxActorTypeFlag::eCLOTH;
#endif

		Ps::Array<PxActor*> objects(scene.getNbActors(selectionFlags));
		const PxU32 nb = scene.getActors(selectionFlags, objects.begin(), objects.size());

		PX_ASSERT(nb==objects.size());
		PX_UNUSED(nb);

		for(PxU32 i=0;i<objects.size();i++)
			collection->add(*objects[i]);
	}


	// Collect constraints
	{
		Ps::Array<PxConstraint*> objects(scene.getNbConstraints());
		const PxU32 nb = scene.getConstraints(objects.begin(), objects.size());

		PX_ASSERT(nb==objects.size());
		PX_UNUSED(nb);

		for(PxU32 i=0;i<objects.size();i++)
		{
			PxU32 typeId;
			PxJoint* joint = reinterpret_cast<PxJoint*>(objects[i]->getExternalReference(typeId));
			if(typeId == PxConstraintExtIDs::eJOINT)
				collection->add(*joint);
		}
	}

	// Collect articulations
	{
		Ps::Array<PxArticulation*> objects(scene.getNbArticulations());
		const PxU32 nb = scene.getArticulations(objects.begin(), objects.size());

		PX_ASSERT(nb==objects.size());
		PX_UNUSED(nb);

		for(PxU32 i=0;i<objects.size();i++)
			collection->add(*objects[i]);
	}

	// Collect aggregates
	{
		Ps::Array<PxAggregate*> objects(scene.getNbAggregates());
		const PxU32 nb = scene.getAggregates(objects.begin(), objects.size());

		PX_ASSERT(nb==objects.size());
		PX_UNUSED(nb);

		for(PxU32 i=0;i<objects.size();i++)
			collection->add(*objects[i]);
	}

	return collection;
}

void PxSerialization::dumpBinaryMetaData(PxOutputStream& outputStream, const PxPhysics& sdk, PxSerializationRegistry& sr)
{
	class MetaDataStream : public PxOutputStream
	{
	public:
		virtual	PxU32 write(const void* src, PxU32 count)
		{
			PX_ASSERT(count==sizeof(PxMetaDataEntry));
			const PxMetaDataEntry* entry = (const PxMetaDataEntry*)src;
			metaData.pushBack(*entry);
			return count;
		}		
		shdfnd::Array<PxMetaDataEntry> metaData;
	}s;

	PxGetPhysicsBinaryMetaData(sdk, s);
	Ext::RegisterExtMetaData(s, sr);

	shdfnd::Array<char>	stringTable;

	PxU32 nb = s.metaData.size();
	PxMetaDataEntry* entries = s.metaData.begin();
	for(PxU32 i=0;i<nb;i++)
	{
		entries[i].mType = (const char*)size_t(Cm::addToStringTable(stringTable, entries[i].mType));
		entries[i].mName = (const char*)size_t(Cm::addToStringTable(stringTable, entries[i].mName));
	}

	PxU32 platformTag = Cm::getBinaryPlatformTag();

#ifdef PX_X64
	const PxU32 gaussMapLimit = PxGetGaussMapVertexLimitForPlatform(PxPlatform::ePC);
	const PxU32 tiledHeightFieldSamples = 0;
#endif
#if defined(PX_X86) || defined(__CYGWIN__)
	const PxU32 gaussMapLimit = PxGetGaussMapVertexLimitForPlatform(PxPlatform::ePC);
	const PxU32 tiledHeightFieldSamples = 0;
#endif
#ifdef PX_X360
	const PxU32 gaussMapLimit = PxGetGaussMapVertexLimitForPlatform(PxPlatform::eXENON);
	const PxU32 tiledHeightFieldSamples = 0;
#endif
#ifdef PX_PS3
	const PxU32 gaussMapLimit = PxGetGaussMapVertexLimitForPlatform(PxPlatform::ePLAYSTATION3);
	const PxU32 tiledHeightFieldSamples = 1;
#endif
#ifdef PX_ARM
	const PxU32 gaussMapLimit = PxGetGaussMapVertexLimitForPlatform(PxPlatform::eARM);
	const PxU32 tiledHeightFieldSamples = 0;
#endif
#ifdef PX_WIIU
	const PxU32 gaussMapLimit = PxGetGaussMapVertexLimitForPlatform(PxPlatform::eWIIU);
	const PxU32 tiledHeightFieldSamples = 0;
#endif

	const PxU32 header = PX_MAKE_FOURCC('M','E','T','A');
	const PxU32 version = PX_PHYSICS_VERSION;
	const PxU32 ptrSize = sizeof(void*);
	PxU32 buildNumber = 0;
#if defined(PX_BUILD_NUMBER)
	buildNumber =  PX_BUILD_NUMBER;
#endif
	outputStream.write(&header, 4);
	outputStream.write(&version, 4);
	outputStream.write(&buildNumber, 4);
	outputStream.write(&ptrSize, 4);
	outputStream.write(&platformTag, 4);
	outputStream.write(&gaussMapLimit, 4);
	outputStream.write(&tiledHeightFieldSamples, 4);

	outputStream.write(&nb, 4);
	outputStream.write(entries, nb*sizeof(PxMetaDataEntry));

	PxU32 length = stringTable.size();
	const char* table = stringTable.begin();
	outputStream.write(&length, 4);
	outputStream.write(table, length);
}

PxBinaryConverter* PxSerialization::createBinaryConverter(PxSerializationRegistry& sr, PxErrorCallback* error)
{
	return PX_NEW(Sn::ConvX)(error, sr);
}
