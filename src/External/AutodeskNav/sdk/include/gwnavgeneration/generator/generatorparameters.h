/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// primary contact: GUAL - secondary contact: LASI
#ifndef GwNavGen_GeneratorGlobalConfig_H
#define GwNavGen_GeneratorGlobalConfig_H


#include "gwnavruntime/basesystem/coordsystem.h"
#include "gwnavgeneration/generator/generatoradvancedparameters.h"
#include "gwnavgeneration/input/taggedtriangle3i.h"
#include "gwnavgeneration/generator/generatorblobtyperegistry.h"

namespace Kaim
{

class BlobAggregate;


class GeneratorParametersBlob
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_ROOT_BLOB_CLASS(Generator, GeneratorParametersBlob, 0)
	KY_CLASS_WITHOUT_COPY(GeneratorParametersBlob)
public:
	GeneratorParametersBlob() {}
	BlobFieldArray m_fields;
};
inline void SwapEndianness(Endianness::Target e, GeneratorParametersBlob& self)
{
	SwapEndianness(e, self.m_fields);
}


/// The GeneratorParameters class contains configuration parameters that control the characteristics of the NavData 
/// created by the Generator. You can create an instance of this class, set up its data members as desired, and 
/// pass it in a call to Generator::SetGeneratorParameters().
class GeneratorParameters
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	// The binary backward compatibility of the GeneratorParameters blob is ensured by its nature (BlobFieldMapping) 
	// However, when a parameter is added or renamed its value will be set to the default value (from Ctor) 
	// when loading older versions of configs (from older clientInput). 
	// This behavior might have some random effects. 
	enum { Version = 0 };

	/// Enumerates the possibilities for considering back-facing triangles to be walkable. 
	enum BackFaceTrianglesWalkable { BACKFACE_TRIANGLES_ARE_NON_WALKABLE = 0, ///< Indicates that back-facing triangles should not be considered walkable. 
									 BACKFACE_TRIANGLES_ARE_WALKABLE = 1 ///< Indicates that back-facing triangles should be considered walkable. 
									 };

	/// Enumerates the possibilities for generating a NavMesh that accurately reflects the altitude of the terrain mesh. For details,
	/// see #m_doUseAltitudeTolerance. 
	enum GenerateVerticallyAccurateNavMesh
	{
		VERTICAL_ACCURACY_DISABLED = 0, ///< Instructs the Generator not to adjust the altitude of the NavData. 
		VERTICAL_ACCURACY_ENABLED = 1 ///< Instructs the Generator to perform a post-processing step to adjust the altitude of the NavData to match the altitude of the terrain mesh more accurately. 
	};

public:
	GeneratorParameters() { SetDefaultValues(); }

	/// Clears all information maintained by this object. 
	void SetDefaultValues()
	{
		m_entityHeight = 2.0f;
		m_entityRadius = 0.40f;
		m_stepMax = 0.60f;
		m_slopeMax = 50.0f;

		m_backFaceTrianglesWalkable = BACKFACE_TRIANGLES_ARE_WALKABLE;
		m_rasterPrecision = 0.20f;
		m_cellSize = 20.0f;

		m_advancedParameters.SetDefaultValues();
		m_defaultNavTag.m_blindDataArray.Clear();

		m_altitudeTolerance = 0.5f;
		m_doUseAltitudeTolerance = VERTICAL_ACCURACY_ENABLED;
	}

	/// Returns how many "pixels" EntityRadius represent in terms of Normalized RasterPrecision.
	/// Minimum is 1, no matter the raster precision.
	KyUInt32 GetEntityRadiusInPixels() const;

	/// Returns a "Normalized" RasterPrecision depending on the value of m_rasterPrecision and m_entityRadius.
	/// This value will be the closest submultiple of m_entityRadius such as NormalizedRasterPrecision = 1/n * m_entityRadius
	/// where n is an unsigned integer >= 1. 
	KyFloat32 GetNormalizedRasterPrecision() const;

	/// Write accessor for #m_backFaceTrianglesWalkable. 
	void SetBackFaceTrianglesWalkable(BackFaceTrianglesWalkable backFaceTrianglesWalkable) { m_backFaceTrianglesWalkable = backFaceTrianglesWalkable; }

	/// Read accessor for #m_backFaceTrianglesWalkable. 
	BackFaceTrianglesWalkable GetBackFaceTrianglesWalkable() const { return (BackFaceTrianglesWalkable)m_backFaceTrianglesWalkable; }

	/// Build a GeneratorParametersBlob from these parameters.
	void BuildBlob(BlobHandler<GeneratorParametersBlob>& handler) const;

	/// \copydoc ReadFromBlob()
	KyResult ReadFromBlobHandler(BlobHandler<GeneratorParametersBlob>& handler) { return ReadFromBlob(*handler.Blob()); }

	/// Read parameters values from a BlobAggregate.
	/// \returns #KY_SUCCESS if one and only one GeneratorParametersBlob was found in the aggregate, #KY_ERROR otherwise.
	KyResult ReadFromAggregate(BlobAggregate& aggregate);

	/// \return true if the specified GeneratorParameters contains the same values as this object. 
	bool operator==(const GeneratorParameters& other) const;

	/// \return true if the specified GeneratorParameters contains at least one value that is different from this object. 
	bool operator!=(const GeneratorParameters& other) const { return !operator==(other); }

	void AddMapping(BlobFieldsMapping& mapping, KyUInt32& version);
	void AddMapping(BlobFieldsMapping& mapping, KyUInt32& version) const;

	/// Read parameters values from a GeneratorParametersBlob.
	/// \returns #KY_SUCCESS if GeneratorParametersBlob has been successfully read, #KY_ERROR otherwise (namely in case of bad Blob version).
	KyResult ReadFromBlob(const GeneratorParametersBlob& blob);

public:
	/// The height of the character that will use the NavData at runtime.
	/// \units			meters
	/// \defaultvalue    2.0f 
	KyFloat32 m_entityHeight; // in meters

	/// The radius (or half-width) of the character that will use the NavData at runtime.
	/// \units			meters
	/// \defaultvalue    0.4f 
	KyFloat32 m_entityRadius; // in meters

	/// The maximum difference in altitude that the character that will use the NavData at runtime can traverse in its normal movement. 
	/// \units			meters
	/// \defaultvalue    0.6f 
	KyFloat32 m_stepMax;      // in meters

	/// The maximum slope that the character that will use the NavData at runtime can traverse in its normal movement. Any input
	///  triangle with a slope greater than this value is automatically tagged with the exclusive NavTag, and no  NavData will be
	///  generated for that triangle.
	/// \units 			degrees
	/// \defaultvalue    50.0f 
	KyFloat32 m_slopeMax;     // in degrees

	/// Specifies the default NavTag that will be applied to all input triangles that do not otherwise have a NavTag
	/// set explicitly.
	DynamicNavTag m_defaultNavTag;

	/// Determines whether or not back-face triangles (i.e. triangles whose points go in counter-clockwise order when seen
	/// from above) will be considered walkable and have NavData generated for them.
	/// \acceptedvalues    Any value from the #BackFaceTrianglesWalkable enumeration.
	/// \defaultvalue      #BACKFACE_TRIANGLES_ARE_WALKABLE 
	KyUInt32 m_backFaceTrianglesWalkable;

	/// Determines the approximate width and length of each pixel used to rasterize the input triangles.
	/// This value is rounded to a "Normalized" value so that a pixel is exactly 1/n * m_entityRadius
	/// where n is an integer value of at least 1. 
	/// The "Normalized" value is accessible using GetNormalizedRasterPrecision and is computed using the following formula:
	/// NormalizeRasterPrecision = m_entityRadius/GetEntityRadiusInPixels() 
	/// where GetEntityRadiusInPixels() returns Max(1, NearestInt(m_entityRadius/m_rasterPrecision))
	/// For example if m_entityRadius = 0.4 and m_rasterPrecision = 0.25, entityRadiusInPixel = NearestInt(1.6) = 2 
	/// and NormalizedPixelSize = 0.4/ 2 = 0.2
	/// \units			meters
	/// \defaultvalue    0.2f 
	KyFloat32 m_rasterPrecision; // aka pixelSize

	/// Determines the approximate width and length of each cell in the grid used to partition the NavMesh internally.
	/// This value will be rounded so that a cell contains exactly "n" pixels where n is an integer
	/// \units			meters
	/// \defaultvalue    20.0f 
	KyFloat32 m_cellSize;        // cell side size in meter of the NavMesh grid partition

	/// Determines the maximum difference in altitude that may exist between the NavMesh and the original terrain mesh.
	/// The way this parameter is interpreted depends on whether or not the Generator is configured to
	/// generate vertically accurate NavData: i.e. on the value of #m_doUseAltitudeTolerance.
	/// -	When vertically accurate NavMesh generation is enabled (the default), this parameter determines the maximum vertical
	/// 	distance that the NavMesh triangles may be from the original raster at any point.
	/// -	When vertically accurate NavMesh generation is disabled, this parameter is only taken into account at the edges
	/// 	of NavMesh triangles that compose the borders between NavCells and NavFloors. It has no effect on the NavData
	/// 	generated in the interior of the cells themselves.
	/// Lower values typically result in a NavMesh with smaller triangles that is more faithful to the original 
	/// rasterization. Larger values simplify the NavMesh more aggressively, and typically result in a NavMesh with fewer,
	/// larger triangles. In most cases, the default value should be sufficient unless you need an accurate NavMesh for your
	/// own purposes outside of Gameware Navigation.
	/// \units 			meters
	/// \defaultvalue    0.5f 
	KyFloat32 m_altitudeTolerance; //in m

	/// Determines whether or not the Generator performs a post-processing step to ensure that altitude of the NavData accurately
	/// reflects the altitude of the terrain mesh.
	/// -	Without this post-processing step, the altitude of the NavData is guaranteed to match the altitude of the terrain mesh
	/// 	only at the boundaries between adjacent cells. Within each cell, the altitude of the NavData is not guaranteed to match
	/// 	the terrain mesh with any degree of precision. 
	/// -	Carrying out the post-processing step increases the vertical accuracy of the final NavMesh, but may increase the size of 
	/// 	the final NavData and the time required to generate it.
	/// \acceptedvalues	Any value from the GenerateVerticallyAccurateNavMesh enumeration. 
	/// \defaultvalue	#VERTICAL_ACCURACY_ENABLED 
	KyUInt32 m_doUseAltitudeTolerance;

	/// Contains more advanced configuration parameters. The default values of the parameters offered by GeneratorAdvancedParameters
	/// should suffice for most projects. 
	GeneratorAdvancedParameters m_advancedParameters;
};

template <class OSTREAM>
inline OSTREAM& operator<<(OSTREAM& os, GeneratorParameters& config)
{
	BlobFieldsMapping configMapping;
	KyUInt32 version;
	config.AddMapping(configMapping, version);

	os << KY_LOG_SMALL_TITLE_BEGIN("", "GeneratorParameters Info");
	os << configMapping;
	os << KY_LOG_SMALL_TITLE_END("", "GeneratorParameters Info");
	return os;
}


class GeneratorParametersBlobBuilder : public BaseBlobBuilder<GeneratorParametersBlob>
{
public:
	GeneratorParametersBlobBuilder(const GeneratorParameters* globalConfig) : m_globalConfig(globalConfig) {}

private:
	virtual void DoBuild()
	{
		BlobFieldsMapping mapping;
		KyUInt32 version = GeneratorParameters::Version;
		m_globalConfig->AddMapping(mapping, version);

		BlobFieldArrayBuilder blobFieldArrayBuilder("GlobalConfig", mapping);

		BLOB_BUILD(m_blob->m_fields, blobFieldArrayBuilder);
	}

	const GeneratorParameters* m_globalConfig;
};


} // namespace Kaim

#endif
