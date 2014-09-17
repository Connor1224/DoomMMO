/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: MAMU
#ifndef GwNavGen_ObjVisualRepresentation_H
#define GwNavGen_ObjVisualRepresentation_H


#include "gwnavruntime/visualsystem/visualrepresentation.h"
#include "gwnavruntime/basesystem/coordsystem.h"


namespace Kaim
{

class ObjFileReader;
class ColorPalette;


/*  VisualRepresentation of all triangles of an obj file.
	The color of the triangles is set depending on the associated material according to the given color palette.
	if materialIndex != KyUInt32MAXVAL, only the passed materialIndex is represented. */
class ObjVisualRepresentation : public VisualRepresentation
{
public:
	enum TriangleWinding { CCW, CW };

	ObjVisualRepresentation(
			const ObjFileReader* obj,
			const CoordSystem& coordSystem,
			const ColorPalette* palette,
			TriangleWinding triangleWinding = CCW,
			KyUInt32 representedMaterialIndex = KyUInt32MAXVAL);

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);

private:
	CoordSystem m_coordSystem;
	TriangleWinding m_triangleWinding;
	const ColorPalette* m_palette;
	KyUInt32 m_representedMaterialIndex;
};


}


#endif

