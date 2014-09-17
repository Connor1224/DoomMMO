/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



#ifndef GwNavGen_ClientInputVisualRepresentation_H
#define GwNavGen_ClientInputVisualRepresentation_H


#include "gwnavruntime/visualsystem/visualrepresentation.h"
#include "gwnavruntime/basesystem/coordsystem.h"


namespace Kaim
{


class ClientInput;
class ITerrainTypeColorPalette;


class ClientInputVisualRepresentation : public VisualRepresentation
{

public:
	enum DrawSeedPointMode { DO_NOT_DRAW_SEEDPOINTS, DRAW_SEEDPOINTS };

public:
	ClientInputVisualRepresentation(
			ClientInput* clientInput,
			const ITerrainTypeColorPalette* terrainTypePalette,
			DrawSeedPointMode drawSeedPoints = DO_NOT_DRAW_SEEDPOINTS);

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);

public:
	const ITerrainTypeColorPalette* m_terrainTypePalette;
	DrawSeedPointMode m_drawSeedPoints;
};


}

#endif // Navigation_ClientInputVisualRepresentation_H
