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



#ifndef PX_REPX_SIMPLE_TYPE_H
#define PX_REPX_SIMPLE_TYPE_H

/** \addtogroup extensions
  @{
*/

#include "foundation/PxSimpleTypes.h"


#ifndef PX_DOXYGEN
namespace physx
{
#endif
	
	class PxPhysics;
	class PxCooking;
	class PxStringTable;

	typedef PxU64 PxRepXId;
	
	/**
	\brief Helper class containing the mapping of id to object, and type name.
	*/
	struct PxRepXObject
	{
		/**
		\brief Identifies the extension meant to handle this object.
		@see PxTypeInfo, PX_DEFINE_TYPEINFO, PxRepXSerializer
		*/
		const char*			mTypeName;

		/**
		\brief Pointer to the live object this was created from
		*/
		const void*			mLiveObject;

		/**
		\brief Id given to this object at some point
		*/
		PxRepXId			mId;
		PxRepXObject( const char* inTypeName = "", const void* inLiveObject = NULL, const PxRepXId inId = 0 )
			: mTypeName( inTypeName )
			, mLiveObject( inLiveObject )
			, mId( inId )
		{
		}
		bool isValid() const { return mLiveObject != NULL; }
	};

	/**
	\brief Arguments required to instantiate a repx collection.

	Extra arguments can be added to the object map under special ids.

	@see PxRepXSerializer::objectToFile, PxRepXSerializer::fileToObject
	*/
	struct PxRepXInstantiationArgs
	{
		PxPhysics&			mPhysics;
		PxCooking*			mCooker;
		PxStringTable*		mStringTable;
		PxRepXInstantiationArgs( PxPhysics& inPhysics, PxCooking* inCooking = NULL , PxStringTable* inStringTable = NULL ) 
			: mPhysics( inPhysics )
			, mCooker( inCooking )
			, mStringTable( inStringTable )
		{
		}

		PxRepXInstantiationArgs& operator=(const PxRepXInstantiationArgs&);
	};


#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif 