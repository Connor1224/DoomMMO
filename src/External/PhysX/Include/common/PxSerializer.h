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


#ifndef PX_SERIALIZER_H
#define PX_SERIALIZER_H
/** \addtogroup extensions
@{
*/

#include "PxSerialUtils.h"

#ifndef PX_DOXYGEN
namespace physx
{
#endif

/** 
 \brief Serialization interface class.

 PxSerializer is used to extend serializable PxBase classes with serialization functionality. The 
 interface is structured such that per-class adapter instances can be used as opposed to per-object 
 adapter instances, avoiding per object allocations. Hence the methods take a reference to PxBase as a parameter.

 The PxSerializer interface needs to be implemented for binary or RepX serialization to work on custom 
 types. If only RepX serialization is needed, some methods can be left empty, as they are only needed 
 for binary serialization.

 A default implementation is available as a template adapter (PxSerializerDefaultAdapter).

 @see PxSerializerDefaultAdapter, PX_NEW_SERIALIZER_ADAPTER, PxSerializationRegistry::registerSerializable
*/
class PxSerializer
{
public:

	/************************************************************************************************/

	/** @name Basics needed for Binary- and RepX-Serialization
	*/
	//@{

	/**
	\brief Returns string name of dynamic type.

	\return	Class name of most derived type of this object.
	*/
	virtual	 const	char*			getConcreteTypeName() const								= 0;   

	/**
	\brief Adds required objects to the collection.
	
	This method does not add the required objects recursively, e.g. objects required by required objects.

	@see PxCollection, PxSerialization::complete
	*/
	virtual			void			requires(PxBase&, PxCollection&) const					= 0;

	/**
	\brief Releases PxSerializer instance.
	*/
	virtual			void			release()												= 0;

	//@}
	/************************************************************************************************/

	/************************************************************************************************/

	/** @name Functionality needed for Binary Serialization only
	*/
	//@{

	/**
	\brief Register object's name in PxNameManager.

	@see PxNameManager
	*/
	virtual         void			registerNameForExport(PxBase&, PxNameManager&) const	= 0;

	/**
	\brief Register object's address in PxPtrManager.

	@see PxPtrManager
	*/
	virtual         void            registerPtrsForExport(PxBase&, PxPtrManager&) const		= 0;

	/**
	\brief Exports object's extra data to stream.
	*/
	virtual         void            exportExtraData(PxBase&, PxSerialStream&, void*) const	= 0;

	/**
	\brief Reads object's extra data from address

	\return New address after read data
	*/
	virtual         char*           importExtraData(PxBase&, char*, PxU32&) const			= 0;

	/**
	\brief Replace object's pointer with correct address.

	\return Whether the pointer was successfully resolved
	*/
	virtual         bool            resolvePointers(PxBase&, PxRefResolver&, void*) const	= 0;

	/**
	\brief Exports object's data to stream.
	*/
	virtual         void            exportData(PxBase&, PxSerialStream&) const				= 0;
		
	/**
	\brief Create object at the address.

	\return	PxBase object be created.
	*/
	virtual         PxBase*			createBinarySerializable(char*&, PxRefResolver&) const	= 0; 

	//@}
	/************************************************************************************************/

protected:
	
	virtual ~PxSerializer() {};
};


/** 
 \brief Default PxSerializer implementation.
*/
template<class T>
class PxSerializerDefaultAdapter : public PxSerializer
{
public:

	/************************************************************************************************/

	/** @name Basics needed for Binary- and RepX-Serialization
	*/
	//@{

	PxSerializerDefaultAdapter(const char* name) : mTypeName(name) {}

	virtual const char* getConcreteTypeName() const
	{ 
		return mTypeName; 
	}

	virtual	void requires(PxBase& obj, PxCollection& c) const
	{
		static_cast<T*>(&obj)->requires(c);
	}

	virtual void release(){ PxGetFoundation().getAllocatorCallback().deallocate(this); }

	//@}
	/************************************************************************************************/

	/** @name Functionality needed for Binary Serialization only
	*/
	//@{

	// object methods

	virtual void registerNameForExport(PxBase&, PxNameManager&)         const  {}

	virtual void registerPtrsForExport(PxBase& obj, PxPtrManager& pm)   const
	{ 
		pm.registerPtr((void*)&obj); 
	}

	virtual void exportExtraData(PxBase& obj, PxSerialStream& s, void* context) const
	{ 
		static_cast<T*>(&obj)->exportExtraData(s, context);
	}

	virtual char* importExtraData(PxBase& obj, char* address, PxU32& totalPadding)  const
	{ 
		return static_cast<T*>(&obj)->importExtraData(address, totalPadding);
	}

	virtual bool resolvePointers(PxBase& obj, PxRefResolver& v, void* context)  const
	{ 
		return static_cast<T*>(&obj)->resolvePointers(v, context);
	}

	virtual void exportData(PxBase& obj, PxSerialStream& s)  const
	{ 
		s.storeBuffer(&obj, sizeof(T));
	}

	// class methods

	virtual  PxBase*	createBinarySerializable(char*& address, PxRefResolver& v)  const
	{
		T* NewObject = new (address) T(v);									
		address += sizeof(T);																
		return NewObject;	
	}

	//@}
	/************************************************************************************************/

private:
	const char* mTypeName;	
};

/** 
 \brief Preprocessor Macro to simplify adapter creation.

 Note: that the allocator used for creation needs to match with the one used in PxSerializerDefaultAdapter<T>::release().
*/
#define PX_NEW_SERIALIZER_ADAPTER(x) \
	new( PxGetFoundation().getAllocatorCallback().allocate(sizeof(PxSerializerDefaultAdapter<x>), \
	"PxSerializerDefaultAdapter",  __FILE__, __LINE__ )) PxSerializerDefaultAdapter<x>(#x)

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif