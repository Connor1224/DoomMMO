/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


	


// primary contact: LASI - secondary contact: GUAL
#ifndef GwNavGen_NavRasterVisualRepresentation_H
#define GwNavGen_NavRasterVisualRepresentation_H


#include "gwnavruntime/visualsystem/visualrepresentation.h"
#include "navrastercell.h"


namespace Kaim
{

class VisualGeometryBuilder;
class NavRasterCell;

class NavRasterCellVisualRepresentation : public VisualRepresentation
{
public:
	NavRasterCellVisualRepresentation(NavRasterCell* navRasterCell) : VisualRepresentation(navRasterCell) {}

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);

private:
	const NavRasterColumnElement* GetNeighbor(const NavRasterColumnElement& pixel, const PixelPos& pos, KyInt32 dir);
};


}


#endif
