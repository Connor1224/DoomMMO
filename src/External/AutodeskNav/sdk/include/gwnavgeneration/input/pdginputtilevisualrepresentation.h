/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


#ifndef Navigation_PdgInputTileVisualRepresentation_H
#define Navigation_PdgInputTileVisualRepresentation_H


#include "gwnavruntime/visualsystem/visualrepresentation.h"

namespace Kaim
{

struct PdgInputTile;
class ITerrainTypeColorPalette;

/* Implements the visual representation of an input cell */
class PdgInputTileVisualRepresentation : public VisualRepresentation
{
public:
	PdgInputTileVisualRepresentation(PdgInputTile* inputTile, ITerrainTypeColorPalette* palette = KY_NULL) :
		VisualRepresentation(inputTile), m_palette(palette)
	{}

	virtual ~PdgInputTileVisualRepresentation() {}

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);

public:
	ITerrainTypeColorPalette* m_palette;
};


}

#endif // Navigation_PdgInputCellVisualRepresentation_H
