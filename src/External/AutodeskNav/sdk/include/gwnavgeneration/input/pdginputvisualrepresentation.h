/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


#ifndef GwNavGen_PdgInputVisualRepresentation_H
#define GwNavGen_PdgInputVisualRepresentation_H


#include "gwnavruntime/visualsystem/visualrepresentation.h"


namespace Kaim
{


class PdgInput;
class ITerrainTypeColorPalette;


class PdgInputVisualRepresentation : public VisualRepresentation
{
public:
	PdgInputVisualRepresentation(PdgInput* pdgInput, const ITerrainTypeColorPalette* palette = KY_NULL)
		: VisualRepresentation(pdgInput), m_palette(palette) {}

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);

public:
	const ITerrainTypeColorPalette* m_palette;
};


}

#endif // Navigation_PdgInputVisualRepresentation_H
