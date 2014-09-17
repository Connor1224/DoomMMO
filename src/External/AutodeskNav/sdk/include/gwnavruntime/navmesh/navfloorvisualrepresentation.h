/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_NavFloorVisualRepresentation_H
#define Navigation_NavFloorVisualRepresentation_H

#include "gwnavruntime/visualsystem/visualrepresentation.h"
#include "gwnavruntime/basesystem/intcoordsystem.h"
#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"
#include "gwnavruntime/database/databasegenmetrics.h"
#include "gwnavruntime/navmesh/navfloor.h"

namespace Kaim
{

class NavFloor;
class ITerrainTypeColorPalette;


class NavFloorDisplayListBuilder : public IDisplayListBuilder
{
public:
	void Setup(const DatabaseGenMetrics& databaseGenMetrics, const NavFloor* navFloor, const ITerrainTypeColorPalette* palette = KY_NULL)
	{
		const CellPos& cellPos = navFloor->GetCellPos();
		KyInt32 cellSizeInCoord = IntCoordSystem::PixelCoordToInt(databaseGenMetrics.m_cellSizeInPixel);
		CoordPos64 cellOrigin(cellPos.x * cellSizeInCoord, cellPos.y * cellSizeInCoord);

		m_palette = palette;
		m_cellOrigin = cellOrigin;
		m_integerPrecision = databaseGenMetrics.m_integerPrecision;
	}

	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize = 0);

public: // internal
	const ITerrainTypeColorPalette* m_palette;
	CoordPos64 m_cellOrigin;
	KyFloat32 m_integerPrecision;
};


/* This class of VisualRepresentation defines the way a NavFloor should be rendered using triangles. */
class NavFloorVisualRepresentation : public VisualRepresentation
{
public :
	class Config
	{
	public:
		Config() : m_altitudeOffset(0.0f) {}

		void SetAltitudeOffSet(KyFloat32 altitudeOffset) { m_altitudeOffset = altitudeOffset;}

	public:
		KyFloat32 m_altitudeOffset;
	};

public:
	NavFloorVisualRepresentation(const NavFloor* navFloor, KyInt32 cellSizeInPixel, KyFloat32 integerPrecision, const ITerrainTypeColorPalette* palette = KY_NULL)
		: VisualRepresentation(navFloor), m_palette(palette), m_integerPrecision(integerPrecision), m_cellSizeInPixel(cellSizeInPixel), m_cellSizeInCoord(IntCoordSystem::PixelCoordToInt(cellSizeInPixel)) {}

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);

	void RenderInvalidLink(const NavHalfEdgeIdx edgeIdx, const NavFloorBlob& currentFloorBlob, const CoordPos64 cellOrigin, VisualGeometryBuilder &geometryBuilder);

public:
	const ITerrainTypeColorPalette* m_palette;
	KyFloat32 m_integerPrecision;
	KyInt32 m_cellSizeInPixel;
	KyInt32 m_cellSizeInCoord;
	Config m_config;
};

class NavFloorBlobVisualRepresentation : public VisualRepresentation
{
public :
	class Config
	{
	public:
		Config() : m_altitudeOffset(0.0f) {}

		void SetAltitudeOffSet(KyFloat32 altitudeOffset) { m_altitudeOffset = altitudeOffset;}

	public:
		KyFloat32 m_altitudeOffset;
	};

public:
	NavFloorBlobVisualRepresentation(const NavFloorBlob* navFloorBlob, KyFloat32 integerPrecision, KyInt32 cellSizeInCoord, const CellPos& cellPos, const ITerrainTypeColorPalette* palette = KY_NULL)
		: VisualRepresentation(navFloorBlob), m_palette(palette), m_integerPrecision(integerPrecision), m_cellSizeInCoord(cellSizeInCoord), m_cellPos(cellPos) {}

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);

public:
	const ITerrainTypeColorPalette* m_palette;
	KyFloat32 m_integerPrecision;
	KyInt32 m_cellSizeInCoord;
	CellPos m_cellPos;
	Config m_config;
};
}

#endif // #Navigation_NavFloorVisualRepresentation_H
