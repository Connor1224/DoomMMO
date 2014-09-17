/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_NavMeshElementBlob_H
#define Navigation_NavMeshElementBlob_H

#include "gwnavruntime/navmesh/blobs/navcellblob.h"
#include "gwnavruntime/basesystem/intcoordsystem.h"
#include "gwnavruntime/basesystem/coordsystem.h"
#include "gwnavruntime/base/guidcompound.h"

namespace Kaim
{

/// The NavMeshElementBlob contains the NavMesh static data (a set of NavCellBlob) for an element (a GuidCompound) within a sector.
/// It is produced by Gameware Navigation generation.
class NavMeshElementBlob
{
	KY_CLASS_WITHOUT_COPY(NavMeshElementBlob)
	KY_ROOT_BLOB_CLASS(NavData, NavMeshElementBlob, 0)
public:
	/// This object indicates the settings used to generate the NavMeshElementBlob, and thereby the physical characteristics and
	/// movement model of the characters expected to use the NavMesh at runtime.
	class GenParameters
	{
	public:
		GenParameters();
		bool operator==(const GenParameters& other) const;
		bool operator!=(const GenParameters& other) const { return !operator==(other); }

		KyInt32 GetCellSizeInCoord() const; ///< Retrieves the number of raster pixels that make up the length and width of each cell. 

		KyFloat32 m_entityRadius;      ///< The radius in meters of the characters that can use this NavMesh, in meters.
		KyFloat32 m_entityHeight;      ///< The height in meter of the characters that can use this NavMesh, in meters.
		KyFloat32 m_stepMax;           ///< The maximum difference in altitude, in meters, that is considered navigable between neighboring pixels in the raster created from the terrain mesh.
		KyFloat32 m_slopeMax;          ///< The maximum slope, in degrees, for a triangle in the terrain mesh to be considered navigable.
		KyFloat32 m_altitudeTolerance; ///< The maximum difference in altitude that may exist between the NavMesh and the original terrain mesh.
		KyFloat32 m_integerPrecision;  ///< For internal use. Do not modify.
		KyInt32 m_cellSizeInPixel;     ///< For internal use. Do not modify.
	};

	enum NavMeshFlags
	{
		NAVMESH_NO_FLAG          = 0,
		NAVMESH_FROM_PARTITIONER = 1,
		// NAVMESH_ADDITIONAL_FLAG = 1<<1; etc
		NavMeshFlags_FORCE32 = 0xFFFFFFFF
	};

public:
	NavMeshElementBlob() {}

	KyUInt32           GetNavCellBlobount()         const;
	const NavCellBlob& GetNavCellBlob(KyUInt32 idx) const;
	const CellBox&     GetCellBox()                 const;

	/// Indicates whether or not the specified NavMeshElementBlob was created with the same generation parameters as this object. 
	bool IsCompatibleWith(const NavMeshElementBlob& navMeshElementBlob) const;

public: // Internal
	bool IsValid() const; ///< Performs some basic tests on static data. For internal debug purpose.
public:
	CellBox m_cellBox;             ///< The box of NavCells maintained by this NavMeshElementBlob. Do not modify. 
	GuidCompound m_guidCompound;   ///< The GuidCompound that uniquely identifies this NavMeshElementBlob. For internal use. Do not modify. 
	GenParameters m_genParameters; ///< The physical characteristics and movemement model of the character for which the NavMeshElementBlob was generated. 
	KyUInt32 m_flags;              ///< Maintains extra information about this NavMeshElementBlob, set at generation time. For internal use. Do not modify. 

	BlobArray<BlobRef<NavCellBlob> > m_cells; ///< The list of NavCellBlob maintained by the NavMeshElementBlob. For internal use. Do not modify. 
};

/// Swaps the endianness of the data in the specified object. This function allows this type of object to be serialized through the blob framework.
/// The first parameter specifies the target endianness, and the second parameter provides the object whose data is to be swapped.
inline void SwapEndianness(Endianness::Target e, NavMeshElementBlob::GenParameters& self)
{
	SwapEndianness(e, self.m_entityRadius);
	SwapEndianness(e, self.m_entityHeight);
	SwapEndianness(e, self.m_stepMax);
	SwapEndianness(e, self.m_slopeMax);
	SwapEndianness(e, self.m_altitudeTolerance);
	SwapEndianness(e, self.m_integerPrecision);
	SwapEndianness(e, self.m_cellSizeInPixel);
}

/// Swaps the endianness of the data in the specified object. This function allows this type of object to be serialized through the blob framework.
/// The first parameter specifies the target endianness, and the second parameter provides the object whose data is to be swapped.
inline void SwapEndianness(Endianness::Target e, NavMeshElementBlob& self)
{
	SwapEndianness(e, self.m_cellBox);
	SwapEndianness(e, self.m_guidCompound);
	SwapEndianness(e, self.m_genParameters);
	SwapEndianness(e, self.m_flags);
	SwapEndianness(e, self.m_cells);
}

KY_INLINE KyInt32 NavMeshElementBlob::GenParameters::GetCellSizeInCoord() const { return IntCoordSystem::PixelCoordToInt(m_cellSizeInPixel); }

KY_INLINE KyUInt32           NavMeshElementBlob::GetNavCellBlobount()         const { return m_cells.GetCount();  }
KY_INLINE const NavCellBlob& NavMeshElementBlob::GetNavCellBlob(KyUInt32 idx) const { return *m_cells.GetValues()[idx].Ptr(); }
KY_INLINE const CellBox&     NavMeshElementBlob::GetCellBox()                 const { return m_cellBox;           }

KY_INLINE bool NavMeshElementBlob::IsCompatibleWith(const NavMeshElementBlob& navMeshElementBlob) const
{
	return m_genParameters == navMeshElementBlob.m_genParameters;
}



}


#endif //Navigation_NavMeshElementBlob_H

