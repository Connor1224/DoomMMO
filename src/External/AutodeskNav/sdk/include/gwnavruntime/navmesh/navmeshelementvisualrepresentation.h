/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// ---------- Primary contact: JUBA - secondary contact: MAMU
#ifndef Navigation_NavMeshElementVisualRepresentation_H
#define Navigation_NavMeshElementVisualRepresentation_H


#include "gwnavruntime/visualsystem/visualrepresentation.h"


namespace Kaim
{

class NavMeshElementBlob;
class ITerrainTypeColorPalette;

/* This class of VisualRepresentation defines the way the data in a NavMeshElementBlob
	should be rendered using triangles.
	A NavCellVisualRepresentation is used to build the geometry for each NavCellBlob
	in the NavMeshElementBlob, and the geometries of all NavCells are combined to build the geometry for the
	NavMeshElementBlob. */
class NavMeshElementVisualRepresentation : public VisualRepresentation
{
public:
	NavMeshElementVisualRepresentation(const NavMeshElementBlob* navMeshElementBlob, KyFloat32 altOffset, const ITerrainTypeColorPalette* palette = KY_NULL) :
	  VisualRepresentation(navMeshElementBlob), m_palette(palette), m_altOffset(altOffset) {}

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);

public:
	const ITerrainTypeColorPalette* m_palette;
	KyFloat32 m_altOffset;
};


}


#endif

