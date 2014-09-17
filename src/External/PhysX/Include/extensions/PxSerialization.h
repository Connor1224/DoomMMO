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


#ifndef PX_SERIALIZATION_H
#define PX_SERIALIZATION_H
/** \addtogroup extensions
  @{
*/

#include "PxPhysXConfig.h"
#include "common/PxBase.h"
#include "cooking/PxCooking.h"
#include "common/PxIO.h"
#include "common/PxTolerancesScale.h"
#include "common/PxTypeInfo.h"
#include "common/PxStringTable.h"

#ifndef PX_DOXYGEN
namespace physx
{
#endif

class PxPhysics;
class PxCollection;

/**
\brief Utility functions for serialization

@see PxCollection, PxSerializationRegistry
*/
class PxSerialization
{
public:
	/**
	\brief Additional PxScene and PxPhysics options stored in serialized RepX data.

	The PxXmlParserOptions parameter can be serialized and deserialized along with PxCollection instances (Xml only).
	This is for application use only and has no impact on how objects are serialized or deserialized. 
	@see PxSerialization::createCollectionFromXml, PxSerialization::serializeCollectionToXml
	*/
	struct PxXmlParserOptions
	{
		/**
		\brief Up vector for the scene reference coordinate system.
		*/
		PxVec3				mUpVector;

		/**
		\brief Tolerances scale to be used for the scene.
		*/
		PxTolerancesScale	mScale;
		
		PxXmlParserOptions() : mUpVector(0) {}
		PxXmlParserOptions(PxVec3& upVector, PxTolerancesScale scale) : mUpVector(upVector), mScale(scale) {}
	};

	/**
	\brief Returns whether the collection is serializable with the externalReferences collection.

	Some definitions to explain whether a collection can be serialized or not:

	For definitions of <b>requires</b> and <b>complete</b> see #PxSerialization::complete

	A serializable object is a <b>root</b> if it can be serialized on it's own (with external references if necessary)
	The following objects are roots:
	- joints
	- shared shapes
	- actors
	- articulations
	- aggregates
	- triangle meshes
	- convex meshes
	- height fields
	- materials

	For example, an articulation link cannot be serialized without its articulation, so it's not a root.

	A collection C can be serialized with external references R iff
	- every object in C is either a root, or required by another object in C (no orphans)
	- C is complete relative to R (no dangling refs)
	- There is no object in R that requires an object in C (no circular dependencies)
	- There is no reference r such that R.find(r) and C.find(r) are valid and not equal (no clashes)

	\param[in] collection Collection to be checked
	\param[in] sr PxSerializationRegistry instance with information about registered classes.
	\param[in] externalReferences the external References collection
	\return  Whether the collection is serializable
	@see PxSerialization::complete, PxSerialization::serializeCollectionToBinary, PxSerialization::serializeCollectionToXml, PxSerializationRegistry
	*/
	static	bool			isSerializable(PxCollection& collection, PxSerializationRegistry& sr, const PxCollection* externalReferences = NULL);

	/**
	\brief Adds to a collection all objects such that it can be successfully serialized.
	
	A collection C is complete relative to an other collection D if every object required by C is either in C or D.
	This function adds objects to a collection, such that it becomes complete with respect to the exceptFor collection.
	Completeness is needed for serialization. See #PxSerialization::serializeCollectionToBinary, #PxSerialization::serializeCollectionToXml.

	Sdk objects require other sdk object according to the following rules: 
	 - joints require their actors and constraint
	 - rigid actors require their shapes
	 - shapes require their material(s) and mesh, if any
	 - articulations require their links and joints
	 - aggregates require their actors
	 - cloth actors require their cloth fabric

	If followJoints is specified another rule is added:
	 - actors require their joints
	
	Specifying followJoints will make whole jointed actor chains being added to the collection. Following chains 
	is interrupted whenever a object in exceptFor is encountered.

	\param[in,out] collection Collection which is completed
	\param[in] sr PxSerializationRegistry instance with information about registered classes.
	\param[in] exceptFor Optional exemption collection
	\param[in] followJoints Specifies whether joints should be added for jointed actors
	@see PxCollection, PxSerialization::serializeCollectionToBinary, PxSerialization::serializeCollectionToXml, PxSerializationRegistry
	*/
	static	void			complete(PxCollection& collection, PxSerializationRegistry& sr, const PxCollection* exceptFor = NULL, bool followJoints = false);
	
	/**
	\brief Creates PxSerialObjectRef values for unnamed objects in a collection.

	Creates PxSerialObjectRef names for unnamed objects in a collection starting at a base value and incrementing, 
	skipping values that are already assigned to objects in the collection.

	\param[in,out] collection Collection for which names are created
	\param[in] base Start address for PxSerialObjectRef names
	@see PxCollection
	*/
	static	void			createNames(PxCollection& collection, const PxSerialObjectRef base);

	/**
	\brief Removes objects of a given type from a collection, potentially adding them to another collection.

	\param[in,out] collection Collection from which objects are removed
	\param[in] serialType PxConcreteType of sdk objects that should be removed
	\param[in] sr PxSerializationRegistry instance with information about registered classes.
	\param[in,out] to Optional collection to which the removed objects are added

	@see PxCollection, PxConcreteType
	*/
	static  void			remove(PxCollection& collection, PxType serialType, PxSerializationRegistry& sr, PxCollection* to = NULL);

	/**
	\brief Collects all PxPhysics-level objects for serialization.

	This function adds all objects managed by the PxPhysics object to the input collection, which can
	then be serialized. Collected objects include materials, convex meshes, triangle meshes, height fields and shared shapes.

	This is a helper function to easily enumerate all objects managed by PxPhysics, but using it is not
	mandatory. Users can also manually add a subset of those objects to the collection.

	\param[in] physics Physics SDK to retrieve the objects from. See #PxPhysics
	\return Collection to which objects will be added. See #PxCollection

	@see PxCollection, PxPhysics
	*/
	static  PxCollection*	createCollection(PxPhysics& physics);
	
	/**
	\brief Collects all PxScene-level objects for serialization.

	This function adds all objects managed by the PxScene object to the input collection, which can
	then be serialized. Collected objects include actors, constraints, articulations
	and aggregates.

	This is a helper function to easily enumerate all objects managed by PxScene, but using it is not
	mandatory. Users can also manually add a subset of those objects to the collection.

	\param[in] scene Scene to retrieve the objects from. See #PxScene
	\return Collection to which objects will be added. See #PxCollection

	@see PxCollection, PxScene
	*/
	static	PxCollection*	createCollection(PxScene& scene);

	/**
	\brief Creates a PxCollection from RepX Xml data.

	\param inputData The input data containing the xml collection.
	\param cooking PxCooking instance used for sdk object instantiation.
	\param sr PxSerializationRegistry instance with information about registered classes.
	\param externalRefs PxCollection used to resolve external references.
	\param stringTable PxStringTable instance used for storing object names.
	\param outArgs Optional parameters of physics and scene deserialized from xml. See #PxSerialization::PxXmlParserOptions
	\return Returns a pointer to a PxCollection if successful or NULL if it failed.

	@see PxCollection, PxSerializationRegistry, PxInputData, PxStringTable, PxCooking, PxSerialization::PxXmlParserOptions
	*/
	static	PxCollection*	createCollectionFromXml(PxInputData& inputData, PxCooking& cooking, PxSerializationRegistry& sr, const PxCollection* externalRefs = NULL, PxStringTable* stringTable = NULL, PxXmlParserOptions* outArgs = NULL);
	
	/**
	\brief Deserializes a PxCollection from memory.

	Creates a collection from memory. If the collection has external dependencies another collection 
	can be provided to resolve these.

	The memory block provided has to be 128 bytes aligned and contain a contiguous serialized collection as written
	by PxSerialization::serializeCollectionToBinary.

	\param[in] memBlock Pointer to memory block containing the serialized collection
	\param[in] sr PxSerializationRegistry instance with information about registered classes.
	\param[in] externalRefs Collection to resolve external dependencies

	@see PxCollection, PxSerialization::complete, PxSerialization::serializeCollectionToBinary, PxSerializationRegistry
	*/
	static	PxCollection*	createCollectionFromBinary(void* memBlock, PxSerializationRegistry& sr, const PxCollection* externalRefs = NULL);

	/**
	\brief Serializes a physics collection to an RepX Xml output stream.

	The collection to be serialized needs to be complete @see PxSerialization.complete.
	Optionally Xml stores meshes in binary cooked format for fast loading. It does this when providing a valid non-null PxCooking pointer.

	\param outputStream Stream to save collection to.
	\param collection PxCollection instance which is serialized. The collection needs to be complete with respect to the externalRefs collection.
	\param sr PxSerializationRegistry instance with information about registered classes.
	\param cooking Optional pointer to cooking instance. If provided, cooked mesh data is cached for fast loading.
	\param externalRefs Collection containing external references.
	\param inArgs Optional parameters of physics and scene serialized to xml along with collection. See #PxSerialization::PxXmlParserOptions
	\return true if the collection is successfully serialized.

	@see PxCollection, PxOutputStream, PxSerializationRegistry, PxCooking, PxSerialization::PxXmlParserOptions
	*/
	static	bool			serializeCollectionToXml(PxOutputStream& outputStream, PxCollection& collection,  PxSerializationRegistry& sr, PxCooking* cooking = NULL, const PxCollection* externalRefs = NULL, PxXmlParserOptions* inArgs = NULL);
	
	/**
	\brief Serializes a collection to a binary stream.

	Serializes a collection to a stream. In order to resolve external dependencies the externalReferences collection has to be provided. 
	Optionally names of objects that where set for example with #PxActor::setName are serialized along with the objects.

	The collection can be successfully serialized if isSerializable(collection) returns true. See #isSerializable.

	The implementation of the output stream needs to fulfill the requirements on the memory block input taken by
	PxSerialization::createCollectionFromBinary.

	\param[out] outputStream into which the collection is serialized
	\param[in] collection Collection to be serialized
	\param[in] sr PxSerializationRegistry instance with information about registered classes.
	\param[in] externalRefs Collection used to resolve external dependencies
	\param[in] exportNames Specifies whether object names are serialized
	\return Whether serialization was successful

	@see PxCollection, PxOutputStream, PxSerialization::complete, PxSerialization::createCollectionFromBinary, PxSerializationRegistry
	*/
	static	bool			serializeCollectionToBinary(PxOutputStream& outputStream, PxCollection& collection, PxSerializationRegistry& sr, const PxCollection* externalRefs = NULL, bool exportNames = false );

	/** \brief Dumps the SDK's binary meta-data to a stream.

	A meta-data file contains information about the SDK's internal classes. Such a file is needed to convert
	binary-serialized SDK files from one platform to another. The converter needs meta-data files for the
	source and target platforms to perform conversions.
	
	\param[out] outputStream Stream to write meta data to
	\param[in] physics Physics SDK to generate meta data from
	\param[in] sr PxSerializationRegistry instance with information about registered classes used for conversion.
	
	@see PxOutputStream, PxSerializationRegistry
	*/
	static	void			dumpBinaryMetaData(PxOutputStream& outputStream, const PxPhysics& physics, PxSerializationRegistry& sr);

	/**
	\brief Creates binary converter.

	\param[in] sr		    PxSerializationRegistry instance with information about registered classes used for conversion.
	\param[in] error		User-defined error callback. May be NULL.
	
	\return Binary converter instance.

	@see PxSerializationRegistry, PxErrorCallback
	*/
	static PxBinaryConverter* createBinaryConverter(PxSerializationRegistry& sr, PxErrorCallback* error);

	/**
	\brief Creates an application managed registry for serialization.
	
	\param[in] physics Physics SDK to generate create serialization registry

	\return PxSerializationRegistry instance.

	@see PxSerializationRegistry
	*/
	static PxSerializationRegistry*    createSerializationRegistry(PxPhysics& physics);	
};

#ifndef PX_DOXYGEN
} // namespace physx
#endif

/** @} */
#endif
