/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


#ifndef GwNavGen_PdgInputCellVisualRepresentation_H
#define GwNavGen_PdgInputCellVisualRepresentation_H


#include "gwnavruntime/visualsystem/visualrepresentation.h"
#include "gwnavgeneration/input/pdginputcell.h"

namespace Kaim
{

class PdgInputCell;
class ITerrainTypeColorPalette;

/* Implements the visual representation of an input cell */
class PdgInputCellVisualRepresentation : public VisualRepresentation
{
public:
	PdgInputCellVisualRepresentation(PdgInputCell* inputCell, const ITerrainTypeColorPalette* palette = KY_NULL) :
		VisualRepresentation(inputCell), m_palette(palette), m_rasterPrecision(0.0f)
	{}

	virtual ~PdgInputCellVisualRepresentation() {}

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);

public:
	const ITerrainTypeColorPalette* m_palette;
	KyFloat32 m_rasterPrecision;
};


}

#endif // Navigation_PdgInputCellVisualRepresentation_H
