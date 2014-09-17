/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_OBJProducer_H
#define GwNavGen_OBJProducer_H


#include "gwnavruntime/base/types.h"
#include "gwnavruntime/base/memory.h"
#include "gwnavgeneration/input/taggedtriangle3i.h"
#include "gwnavgeneration/input/generatorinputproducer.h"
#include "gwnavgeneration/input/objfilereader.h"

namespace Kaim
{

class ObjFileReader;
class ClientInputConsumer;
class NavTagByString;

/// Used internally by the OBJProducer class to represent each input .obj file.
class ObjGeometryForInputProducer
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:
	/// For internal use: constructs a new ObjGeometryForInputProducer.
	ObjGeometryForInputProducer(bool generateNavTagsPerMaterial, const DynamicNavTag& navTag);

	~ObjGeometryForInputProducer();

	/// Sets the NavTag that will be tagged to all faces that have no material, and to faces that have
	/// a material that has not been mapped to any other NavTag. Default value is a non-exclusive NavTag
	/// with empty blind data. 
	void SetDefaultNavTag(const DynamicNavTag& navTag) { m_defaultNavTag = navTag; }

	/// Creates a mapping between the specified material and the specified NavTag. Any faces that have the
	/// specified material will be tagged with that NavTag.
	/// Faces with materials that are not associated to any NavTag will be automatically given 
	/// a NavTag if that mechanism is enabled (see GenerateNavTagsPerMaterial), or else they
	/// will be given the default NavTag (see SetDefaultNavTag()). 
	void AddNavTagMaterial(const char* materialName, const DynamicNavTag& navTag);

	/// Enables or disables the automatic mapping of materials to NavTags (enabled by default).
	/// When enabled, each registered material in the .obj gets a NavTag with 1 word according to the following rule:
	/// the first word of the NavTag will be a mask with 1 bit set at position i+1 % 32, where i is the index in
	/// the NavTag list.
	/// For example: m_navTags[0].GetWord(0) -> 0..010, m_terrainTypes[1].GetWord(0) -> 0..100  etc.
	/// 0xFFFFFFFF is reserved for plain triangles without any materials.
	/// NavTag assignations are circular. If there are more then 30 materials, multiple materials will get
	/// mapped to the same NavTag. 
	void GenerateNavTagsForMaterials(bool enable) { m_generateNavTagsPerMaterial = enable; }

	/// Initializes the association between materials (material index) and NavTags. 
	/// If GenerateNavTagsForMaterials() is enabled, a NavTag is automatically generated
	/// for each material. Otherwise, the default NavTag is associated to each material. 
	void InitAssociationBetweenMaterialIndexAndNavTag();

	/// Initializes the association between material indices and NavTags, and fills it from the map of 
	/// associations between NavTags and material names created by the customer (using 
	/// AddNavTagMaterial()). 
	void CreateAssociationBetweenMaterialIndexAndNavTag();

	/// Returns the NavTag associated to a material index. 
	DynamicNavTag& GetNavTagFromMaterialIndex(KyUInt32 materialIndex);

public:
	ObjFileReader m_objFileReader;
	bool m_trianglesAreCounterClockwise;    ///< Indicates whether or not the vertices of the triangles in the input .obj file are specified in counter-clockwise order (true) or in clockwise order (false). 
	NavTagByString* m_navTagByMaterialName; ///< Stores the mapping (material names => NavTags). Do not modify. 
	DynamicNavTag m_defaultNavTag;          ///< The default NavTag used to tag each face that has a material for which no NavTag has been specified. Do not modify. 
	bool m_generateNavTagsPerMaterial;      ///< If true, NavTags are automatically generated for each material. Do not modify directly; see GenerateNavTagsForMaterials(). 

private:
	/// Temporary data created when the obj has been loaded: 
	/// Association of a NavTag for each material index from the obj.
	KyArray<DynamicNavTag> m_navTagByMaterialIndex;
};

/// The OBJProducer class is a concrete implementation of GeneratorInputProducer that can read the triangle and material
/// data contained in a .obj file, and pass it to the ClientInputConsumer.
/// This class is used internally by the standalone NavData generation tools supplied with Gameware Navigation. If you save your
/// terrain geometry in .obj files, you can use this class yourself to create NavData from the triangles in your files. 
class OBJProducer : public GeneratorInputProducer
{
public:
	/// Constructs a new OBJProducer. 
	OBJProducer(bool generateNavTagsPerMaterial = false, const DynamicNavTag& defaultNavTag = DynamicNavTag());

	~OBJProducer();

	/// Sets the NavTag that will be tagged to all faces that have the specified material in ALL input .obj files.
	/// The materials which will be not associated to any NavTag will be automatically affected 
	/// a procedurally generated NavTag if the mechanism is enabled (see EnableAutomaticGenerationOfNavTagsForMaterials)
	/// else they will be affected the default NavTag (see SetDefaultNavTag).
	void AddNavTagMaterial(const char* materialName, const DynamicNavTag& navTag);

	virtual KyResult Produce(const GeneratorSector& sector, ClientInputConsumer& inputConsumer);

	void SetupCoordSystem(const Kaim::CoordSystemConfig& objCoordSystemConfig) { m_objCoordSystem.Setup(objCoordSystemConfig); }

private:
	void SetupGlobalNavTagMaterial(ObjGeometryForInputProducer* geometry);

public:
	bool m_generateNavTagsPerMaterial;       ///< If true, each material is automatically mapped to a NavTag. Do not modify. 
	DynamicNavTag m_defaultNavTag;           ///< The default NavTag used to tag each face that has a material for which no NavTag was specified. Do not modify. 
	NavTagByString* m_navTagByMaterialName;  ///< Stores the mapping between materials and NavTags. Do not modify. 
	CoordSystem m_objCoordSystem;            ///< defines the CoordSystem of the obj files
};


}


#endif

