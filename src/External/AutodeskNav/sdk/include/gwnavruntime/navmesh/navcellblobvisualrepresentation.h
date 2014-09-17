/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_NavCellVisualRepresentation_H
#define Navigation_NavCellVisualRepresentation_H

#include "gwnavruntime/visualsystem/visualrepresentation.h"
#include "gwnavruntime/basesystem/intcoordsystem.h"


namespace Kaim
{

class NavCellBlob;
class ITerrainTypeColorPalette;

/* This class of VisualRepresentation defines the way a NavCellBlob should be rendered using triangles. */
class NavCellVisualRepresentation : public VisualRepresentation
{
public :
	class Config
	{
	public:
		Config() : m_altitudeOffset(0.05f) {}

		void SetAltitudeOffSet(KyFloat32 altitudeOffset) { m_altitudeOffset = altitudeOffset;}

	public:
		KyFloat32 m_altitudeOffset;
	};

public:
	NavCellVisualRepresentation(const NavCellBlob* navCellBlob, KyInt32 cellSizeInPixel, KyFloat32 integerPrecision, const ITerrainTypeColorPalette* palette = KY_NULL)
		: VisualRepresentation(navCellBlob), m_palette(palette), m_integerPrecision(integerPrecision), m_cellSizeInPixel(cellSizeInPixel), m_cellSizeInCoord(IntCoordSystem::PixelCoordToInt(cellSizeInPixel)) {}

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);

public:
	const ITerrainTypeColorPalette* m_palette;
	KyFloat32 m_integerPrecision;
	KyInt32 m_cellSizeInPixel;
	KyInt32 m_cellSizeInCoord;
	Config m_config;
};


}

#endif // #Navigation_NavCellVisualRepresentation_H
