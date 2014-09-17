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


#ifndef PX_PHYSICS_PX_COLLECTION
#define PX_PHYSICS_PX_COLLECTION

/** \addtogroup common
@{
*/

#ifndef PX_DOXYGEN
namespace physx
{
#endif

/**
\brief Collection class for serialization.

A collection is a set of PxBase objects. PxBase objects can be added to the collection
regardless of other objects they depend on. Objects may be named using PxSerialObjectRef values in order 
to resolve dependencies between objects of different collections.

Serialization and deserialization only work through collections.

A scene is typically serialized using the following steps:

 -# create a serialization registry
 -# create a collection for scene objects
 -# complete the scene objects (adds all dependent objects, e.g. meshes)
 -# serialize collection
 -# release collection
 -# release serialization registry

For example the code may look like this:

\code
	PxScene* scene;		// The physics scene
	SerialStream s;		// The user-defined stream doing the actual write to disk
	
	PxSerializationRegistry* registry = PxSerialization::createSerializationRegistry();	// step 1)
	PxCollection* collection = PxSerialization::createCollection(*scene);				// step 2)
	PxSerialization::complete(*collection, *registry);									// step 3)
	PxSerialization::serializeCollectionToBinary(s, *collection, *registry);			// step 4)
	collection->release();																// step 5)
	registry->release();																// step 6)
\endcode

A scene is typically deserialized using the following steps:

 -# load a serialized collection into memory
 -# create a serialization registry
 -# create a collection by passing the serialized memory block
 -# add collected objects to scene
 -# release collection
 -# release serialization registry

For example the code may look like this:

\code
	PxScene* scene;		// The physics scene
	void* memory128;	// a 128-byte aligned buffer previously loaded from disk by the user	- step 1)
	
	PxSerializationRegistry* registry = PxSerialization::createSerializationRegistry();				// step 2)
	PxCollection* collection = PxSerialization::createCollectionFromBinary(memory128, *registry);	// step 3)
	scene->addCollection(*collection);																// step 4)
	collection->release();																			// step 5)
	registry->release();																			// step 6)
\endcode

@see PxBase, PxPhysics::createCollection()
*/
class PxCollection
{
public:
	/**
	\brief Adds a PxBase object to the collection.

	Adds a PxBase object to the collection. Optionally a PxSerialObjectRef name can be provided
	in order to resolve dependencies between collections. A PxSerialObjectRef value of PX_SERIAL_OBJECT_REF_INVALID 
	means the object remains unnamed. Objects can be added regardless of other objects they require.

	\param[in] object Object to be added to the collection
	\param[in] ref Optional PxSerialObjectRef name
	*/
	virtual void						add(PxBase& object, PxSerialObjectRef ref = PX_SERIAL_OBJECT_REF_INVALID) = 0;

	/**
	\brief Removes a PxBase member object from the collection.

	Object needs to be contained by the collection.

	\param[in] object PxBase object to be removed
	*/
	virtual	void						remove(PxBase& object) = 0;

	/**
	\brief Adds a PxBase object to the collection if its not already part of the collection.

	Does the same as #PxCollection::add with ref = PX_SERIAL_OBJECT_REF_INVALID except it additionally checks if the serializable 
	is already part of the collection, in which case no serializable is added.

	\param[in] object object to be added to the collection	
	*/
	virtual void						addRequired(PxBase& object) = 0;
			
	/**
	\brief Returns whether the collection contains a certain PxBase object.

	\param[in] object PxBase object
	\return Whether object is contained.
	*/
	virtual bool						contains(PxBase& object) const = 0;

	/**
	\brief Adds a reference to a member PxBase object.

	If the object is already associated with a reference within the collection, the reference is replaced.
	May only be called for objects that are members of the collection. The reference needs to be unique 
	within the collection.
	
	\param[in] object Member PxBase object
	\param[in] ref PxSerialObjectRef name to be given to the object
	*/
	virtual void						addReference(PxBase& object, PxSerialObjectRef ref) = 0;

	/**
	\brief Removes a reference from a contained PxBase object.

	May only be called for references that are associated with an object in the collection.
	
	\param[in] ref PxSerialObjectRef value
	*/
	virtual void						removeReference(PxSerialObjectRef ref) = 0;
	
	/**
	\brief Adds all PxBase objects and their references of collection to this collection.

	PxBase objects already in this collection are ignored. Object reference names 
	need to be conflict free, i.e. the same object may not have two different names within 
	the two collections.
	
	\param[in] collection Collection to be added
	*/
	virtual void						add(PxCollection& collection) = 0;

	/**
	\brief Removes all PxBase objects of collection from this collection.

	PxBase objects not present in this collection are ignored. Reference names of objects 
	which are removed are also removed.

	\param[in] collection Collection to be removed
	*/
	virtual void						remove(PxCollection& collection) = 0;

	/**
	\brief Gets number of PxBase objects in this collection.
	
	\return Number of objects in this collection
	*/
	virtual	PxU32						getNbObjects() const = 0;

	/**
	\brief Gets the PxBase object of this collection given its index.

	\param[in] index PxBase index in [0, getNbObjects())
	\return PxBase object at index index
	*/
	virtual PxBase&				getObject(PxU32 index) const = 0;

	/**
	\brief Copies member PxBase pointers to a user specified buffer.

	\param[out] userBuffer Array of PxBase pointers
	\param[in] bufferSize Capacity of userBuffer
	\param[in] startIndex Offset into list of member PxBase objects
	\return number of members PxBase objects that have been written to the userBuffer 
	*/
	virtual	PxU32						getObjects(PxBase** userBuffer, PxU32 bufferSize, PxU32 startIndex=0) const = 0;

	/**
	\brief Looks for a PxBase object given a PxSerialObjectRef value.

	If there is no PxBase object in the collection with the given reference name, NULL is returned.

	\param[in] ref PxSerialObjectRef value to look for
	\return PxBase object with the given reference value or NULL
	*/
	virtual PxBase*				find(PxSerialObjectRef ref) const = 0;
	
	/**
	\brief Gets number of PxSerialObjectRef names in this collection.
	
	\return Number of PxSerialObjectRef names in this collection
	*/
	virtual PxU32						getNbReferences() const = 0;

	/**
	\brief Copies member PxSerialObjectRef values to a user specified buffer.

	\param[out] userBuffer Array of PxSerialObjectRef values
	\param[in] bufferSize Capacity of userBuffer
	\param[in] startIndex Offset into list of member PxSerialObjectRef values
	\return number of members PxSerialObjectRef values that have been written to the userBuffer 
	*/
	virtual	PxU32						getReferences(PxSerialObjectRef* userBuffer, PxU32 bufferSize, PxU32 startIndex=0) const = 0;

	/**
	\brief Gets the PxSerialObjectRef name of a PxBase object within the collection.

	The PxBase object needs to be a member of the collection.

	\param[in] object PxBase object to get reference name for
	\return PxSerialObjectRef name of the object or PX_SERIAL_OBJECT_REF_INVALID if the object is unnamed
	*/
	virtual PxSerialObjectRef			getReference(const PxBase& object) const = 0;	

	/**
	\brief Deletes a collection object.

	This function only deletes the collection object, i.e. the container class. It doesn't delete objects
	that are part of the collection.

	@see PxPhysics::createCollection() 
	*/

	virtual void						release() = 0;

protected:
										PxCollection()	{}
	virtual								~PxCollection()	{}
};

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
