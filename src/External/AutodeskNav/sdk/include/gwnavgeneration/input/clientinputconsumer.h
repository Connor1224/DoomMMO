/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// primary contact: GUAL - secondary contact: LASI
#ifndef GwNavGen_ClientInputConsumer_H
#define GwNavGen_ClientInputConsumer_H

#include "gwnavgeneration/input/taggedtriangle3i.h"
#include "gwnavruntime/math/box3f.h"
#include "gwnavgeneration/input/seedpointarray.h"
#include "gwnavgeneration/input/clientinput.h"
#include "gwnavgeneration/input/clientinputchunk.h"
#include "gwnavgeneration/input/clientinputtagvolume.h"

namespace Kaim
{

class GeneratorSystem;
class SpatializedPdgInput;
class PdgInput;
class GeneratorSectorConfig;
class GeneratorSector;
class GeneratorInputProducer;


/* interesting statistics, can be used to check if there was a OneMeter problem */
class ClientInputConsumerStatistics
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	ClientInputConsumerStatistics() { Clear(); }

	void Clear()
	{
		m_nbConsumedTriangles = 0;
		m_consumedSurface2d = 0;
		m_nbConsumedVolumes = 0;
	}

	KyUInt32 GetNbConsumedTriangles()               { return m_nbConsumedTriangles; }
	KyFloat32 GetConsumedSurface2d()                { return m_consumedSurface2d; }
	KyFloat32 GetAverageConsumedTriangleSurface2d() { return m_consumedSurface2d / (KyFloat32)m_nbConsumedTriangles; }
	void GetConsumedTrianglesBoundingBox(Box3f& box) { box = m_consumedTrianglesBoundingBox; }

public:
	KyInt32 m_nbConsumedTriangles;
	KyFloat32 m_consumedSurface2d;
	Box3f m_consumedTrianglesBoundingBox;
	KyUInt32 m_nbConsumedVolumes;
};

/// The ClientInputConsumer class is one of the primary components of the NavData generation system. Its role
/// is to accept data that describes your terrain from within Kaim::GeneratorInputProducer::Produce(const GeneratorSector& sector, ClientInputConsumer& consumer).
/// This data is then used as input for the NavData generation process. This data includes:
/// -	The triangles that make up the terrain mesh, along with an optional color for each that defines its
/// -	material: the type of terrain it represents.
/// -	Optional 3D Tag volumes that tag all triangles within their volumes with a particular NavTag.
/// -	Optional seed points that identify walkable areas.
class ClientInputConsumer
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_NavData)
	friend class GeneratorSectorBuilder;

public: 
	enum WriteMode { DO_NOT_WRITE_CLIENT_INPUT, DO_WRITE_CLIENT_INPUT };

public:
	ClientInputConsumer(GeneratorSystem* sys);

	~ClientInputConsumer() { Clear(); }

	/// Clears all information maintained by this object.
	void Clear();

	/// Sets up an instance of this class with any required components and configuration parameters. Called automatically by the Generator. For internal use. 
	void Init(Ptr<GeneratorSector> sector, WriteMode clientInputWriteMode);

	// ---------------------------------- Consumption ----------------------------------

	/// Provides multiple triangles to the ClientInputConsumer using a single array of floating-point numbers.
	/// All coordinates and altitudes must be expressed in Gameware Navigation coordinate system.
	/// All triangles are assigned the default NavTag. 
	/// Every three elements are interpreted as the X, Y and Z coordinates of a point in a triangle, and every three
	/// points are interpreted as a single triangle. In other words:
	/// -	<tt>vertices[0..2]</tt> should contain the X, Y and Z coordinates of the first point in the first triangle,
	/// -	<tt>vertices[3..5]</tt> should contain the X, Y and Z coordinates of the second point in the first triangle,
	/// -	<tt>vertices[6..8]</tt> should contain the X, Y and Z coordinates of the third point in the first triangle,
	/// -	<tt>vertices[9..11]</tt> should contain the X, Y and Z coordinates of the first point in the second triangle, etc.
	KyResult ConsumeTriangleFromFloatArray(KyFloat32* vertices)
	{
		return ConsumePlainTriangleFromPos(*(Vec3f*)&vertices[0], *(Vec3f*)&vertices[3], *(Vec3f*)&vertices[6]);
	}

	/// Identical to previous function but uses vertices in client coordinate system that must be specified in order to convert them in Navigation CoordSystem
	KyResult ConsumeTriangleFromFloatArrayInClientCoordinates(KyFloat32* client_vertices, const CoordSystem& clientCoordSystem)
	{
		return ConsumePlainTriangleFromPosInClientCoordinates(*(Vec3f*)&client_vertices[0], *(Vec3f*)&client_vertices[3], *(Vec3f*)&client_vertices[6], clientCoordSystem);
	}

	/// Provides a single triangle to the ClientInputConsumer. The normal of the face of the triangle is computed internally, 
	/// interpreting the points provided in clockwise order.
	/// All coordinates and altitudes must be expressed in Gameware Navigation coordinate system.
	/// They will be transformed automatically according to the CoordSystem set up for the Generator. 
	/// \param A                    The coordinates of the first point in the triangle.
	/// \param B                    The coordinates of the second point in the triangle.
	/// \param C                    The coordinates of the third point in the triangle.
	KyResult ConsumePlainTriangleFromPos(const Vec3f& A, const Vec3f& B, const Vec3f& C)
	{
		return ConsumeTriangleFromPos(A, B, C, m_defaultNavTag);
	}
	
	/// Identical to previous function but uses vertices in client coordinate system that must be specified in order to convert them in Navigation CoordSystem
	KyResult ConsumePlainTriangleFromPosInClientCoordinates(const Vec3f& client_A, const Vec3f& client_B, const Vec3f& client_C, const CoordSystem& clientCoordSystem)
	{
		return ConsumeTriangleFromPosInClientCoordinates(client_A, client_B, client_C, m_defaultNavTag, clientCoordSystem);
	}

	/// Provides a single triangle to the ClientInputConsumer. The normal of the face of the triangle is computed internally, 
	/// interpreting the points provided in clockwise order.
	/// All coordinates and altitudes must be expressed in Gameware Navigation coordinate system.
	/// They will be transformed automatically according to the CoordSystem set up for the Generator. 
	/// \param A                    The coordinates of the first point in the triangle.
	/// \param B                    The coordinates of the second point in the triangle.
	/// \param C                    The coordinates of the third point in the triangle.
	/// \param navTag				The NavTag that will be associated to this triangle
	KyResult ConsumeTriangleFromPos(const Vec3f& A, const Vec3f& B, const Vec3f& C, const DynamicNavTag& navTag);
	
	/// Identical to previous function but uses vertices in client coordinate system that must be specified in order to convert them in Navigation CoordSystem
	KyResult ConsumeTriangleFromPosInClientCoordinates(const Vec3f& client_A, const Vec3f& client_B, const Vec3f& client_C, const DynamicNavTag& navTag, const CoordSystem& clientCoordSystem)
	{
		return ConsumeTriangleFromPos( clientCoordSystem.ClientToNavigation_Pos(client_A),
		                               clientCoordSystem.ClientToNavigation_Pos(client_B),
		                               clientCoordSystem.ClientToNavigation_Pos(client_C),
		                               navTag);
	}

	/// Provides a single tag volume to the ClientInputConsumer.
	/// They will be transformed automatically according to the CoordSystem set up for the Generator. 
	KyResult ConsumeTagVolume(const ClientInputTagVolume& inputTagVolume);

	/// Provides a seed point that identifies a walkable area of the terrain. If you provide a seed point, any areas of NavData
	/// that are not reachable from that seed point are automatically discarded during a post-processing phase. You can provide as
	/// many seed points as necessary for each sector.
	/// All coordinates and altitudes must be expressed in Gameware Navigation coordinate system.
	/// \param position		The position of the seed point
	KyResult ConsumeSeedPoint(const Vec3f& position);

	/// Identical to previous function but uses vertices in client coordinate system that must be specified in order to convert them in Navigation CoordSystem
	KyResult ConsumeSeedPointInClientCoordinates(const Vec3f& client_position, const CoordSystem& clientCoordSystem) { return ConsumeSeedPoint(clientCoordSystem.ClientToNavigation_Pos(client_position)); }


	// ---------------------------------- Accessors ----------------------------------
	/// Defines whether or not to filter out back-facing triangles: i.e. faces whose normals point downwards (negative Z values).
	/// If filtering is enabled (the default), all downward facing triangles will be considered non-walkable.
	/// If you disable filtering, downward-facing triangles will be considered as stable, walkable terrain, and NavData will be generated for them. 
	void ToggleBackfaceTriangleFiltering(bool doTag) { m_tagBackfaceTriangleAsNonWalkable = doTag; }

	/// Retrieves information about the triangles consumed by this object. 
	ClientInputConsumerStatistics& GetStats() { return m_stats; }

	/// Provides a default NavTag for triangles passed to the consumer.
	/// \param tag					The default NavTag used when not explicitly passed to Consume functions.
	void SetDefaultNavTag(const DynamicNavTag& tag) { m_defaultNavTag = tag; }

private: 
	/// internal usage by GeneratorSectorBuilder
	void FinalizePdgInput(PdgInput& pdgInput);
	KyResult SaveClientInput(const String& fullFileName);
	KyResult FlushClientInput();

public:
	ClientInputConsumerStatistics m_stats; ///< Maintains statistics about the data added to this object. Do not modify. 
	DynamicNavTag m_defaultNavTag; ///< Stores the default NavTag set by SetDefaultNavTag().

	GeneratorSystem* m_sys; ///< Fast access to the Generator-related information
	
private: // ---------------------------------- Internal ----------------------------------
	Ptr<GeneratorSector> m_sector;  ///< Internal member: The sector being processed

	bool m_tagBackfaceTriangleAsNonWalkable; ///< Internal member: Accessed using ToggleBackfaceTriangleFiltering

	KyFloat32 m_cosSlopeMax;        ///< Internal member: cosinus of maximum slope
	bool m_coordSystemIsRightHanded;///< Internal member: true if the coordSystem is Right-Handed

	DynamicClientInputChunk m_dynamicClientInputChunk; ///< Internal member: Original input chunk
	KyUInt32 m_clientInputFlushCount;                  ///< Internal member: Count how many times the ClientInput was flushed
	WriteMode m_clientInputWriteMode;                  ///< Internal member: Defines whether or not we save the ClientInput

	KyUInt32 m_maxTriangleCountBeforeFlush; ///< Internal member: For limited memory mode. 1024*1024 by default
	KyUInt32 m_nbCellsByTile;               ///< Internal member: For limited memory mode. 25 by default

	SpatializedPdgInput* m_spatializedPdgInput; ///< Internal member: Spatialized input in Gameware Navigation coordinates
	KyArray<Vec3f> m_seedPoints;                ///< Internal member: Consumed seedpoints in Gameware Navigation coordinates

	KyArray<Vec3f> m_inputTriangles;    ///< Temporary buffer for triangles - in Gameware Navigation coordinates
	KyArray<Vec3f> m_childrenTriangles; ///< Temporary buffer for triangles - in Gameware Navigation coordinates
};


}


#endif

