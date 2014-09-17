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

#include "SnSerializationRegistry.h"
#include "PxPhysics.h"
#include "ExtSerialization.h"
#include "PxSerializer.h"
#include "foundation/PxString.h"

using namespace physx;

SnSerializationRegistry::~SnSerializationRegistry()
{
	for( MapSerializer::Iterator iter = mSerializers.getIterator(); !iter.done(); ++iter)
		iter->second->release();

	for( MapRepXSerializer::Iterator iter = mRepXSerializers.getIterator(); !iter.done(); ++iter)
		iter->second->destroy();

}

bool SnSerializationRegistry::registerSerializable(PxType type, PxSerializer* adapter)
{
	if(mSerializers.find(type))
		return false;

	mSerializers.insert(type, adapter);
	return true;
}

const PxSerializer* SnSerializationRegistry::getSerializer(PxType type) const 
{
	const MapSerializer::Entry* e = mSerializers.find(type);
	return e ? e->second : NULL;
}

void SnSerializationRegistry::registerBinaryMetaData(PxBinaryMetaDataCallback callback)
{
	mCustomMetaData.pushBack(callback);
}

void SnSerializationRegistry::getBinaryMetaData(PxOutputStream& stream)
{
	for(PxU32 i = 0; i < mCustomMetaData.size(); i++)
	{
		mCustomMetaData[i](stream);
	}
}

bool SnSerializationRegistry::registerRepXSerializer(PxType type, PxRepXSerializer* serializer)
{
	if(mRepXSerializers.find(type))
	{
		physx::shdfnd::getFoundation().error(physx::PxErrorCode::eDEBUG_WARNING, __FILE__, __LINE__, 
			"Type %d has already been inserted", type);
		return false;
	}

	mRepXSerializers.insert(type, serializer);
	return true;
}

PxRepXSerializer* SnSerializationRegistry::getRepXSerializer(const char* typeName) const
{
	SnSerializationRegistry* sr = const_cast<SnSerializationRegistry*>(this);
	for( MapRepXSerializer::Iterator iter = sr->mRepXSerializers.getIterator(); !iter.done(); ++iter)
	{
		if ( physx::PxStricmp( iter->second->getTypeName(), typeName ) == 0 )
			return iter->second;
	}
	return NULL;
}

PxSerializationRegistry* PxSerialization::createSerializationRegistry(PxPhysics& physics)
{
	PxSerializationRegistry* sr = PX_NEW(SnSerializationRegistry)(physics);	

	PxPhysicsRegisterForSerialization(*sr);
	Ext::ExtensionsRegisterForSerialization(*sr);

	return sr;
}

class CollectionSorter
{
	struct Element
	{
		PxBase*                 mObj;
		Ps::Array<PxU32>				mChildren;
		bool							mFinished;

		Element(PxBase* obj = NULL) : mObj(obj), mFinished(false)	{}
	};

public:
	CollectionSorter(PxCollection& collection, PxSerializationRegistry& sr) : mCollection(collection), mSr(sr) {}

	void sort()
	{
		PxCollection* children = mSr.getPhysics().createCollection();
		Element element;

		Ps::HashMap<PxBase*, PxU32>	map;
		PxU32 i;

		PxU32 nbObject = mCollection.getNbObjects();
		for( i = 0; i < nbObject; ++i )
		{
			PxBase* object = &mCollection.getObject(i);
			element.mObj = object;
			map.insert(object, mElements.size());
			mElements.pushBack(element);
		}

		for( i = 0; i < nbObject; ++i )
		{
			Element& element = mElements[i];
			const PxSerializer* bs = mSr.getSerializer(element.mObj->getConcreteType());
			bs->requires(*element.mObj, *children);

			PxU32 count = children->getNbObjects();
			for(PxU32 j = 0; j < count; ++j)
			{
				PxBase* child = &children->getObject(j);
				if( mCollection.contains(*child) )
				{
					element.mChildren.pushBack(map.find(child)->second);
				}
			}
			(static_cast<Cm::InternalCollection*>(children))->mArray.clear();
		}   
		children->release();

		for( i = 0; i < nbObject; ++i )
		{
			if( mElements[i].mFinished )
				continue;

			AddElement(mElements[i]);
		}

		(static_cast<Cm::InternalCollection&>(mCollection)).mArray.clear();
		for(Ps::Array<PxBase*>::Iterator o = mSorted.begin(); o != mSorted.end(); ++o )
		{
			mCollection.add(**o);
		}
	}

	void AddElement(Element& e)
	{
		if( !e.mFinished )
		{
			for( Ps::Array<PxU32>::Iterator child = e.mChildren.begin(); child != e.mChildren.end(); ++child )
			{
				AddElement(mElements[*child]);
			}

			mSorted.pushBack(e.mObj);
			e.mFinished = true;
		}
	}

private:
	CollectionSorter& operator=(const CollectionSorter&);
	Ps::Array<Element>			mElements;
	PxCollection&				mCollection;
	PxSerializationRegistry&    mSr;
	Ps::Array<PxBase*>			mSorted;
};

void sortCollection(PxCollection& collection, PxSerializationRegistry& sr)
{
	CollectionSorter sorter(collection, sr);	
	sorter.sort();	
}
