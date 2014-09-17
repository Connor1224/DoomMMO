/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


	


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_RasterCellVisualRepresentation_H
#define GwNavGen_RasterCellVisualRepresentation_H


#include "gwnavruntime/visualsystem/visualrepresentation.h"


namespace Kaim
{

class VisualGeometryBuilder;
class RasterCell;


class RasterCellVisualRepresentation : public VisualRepresentation
{
public:
	RasterCellVisualRepresentation(RasterCell* rasterCell) : VisualRepresentation(rasterCell) {}

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);
};


}


#endif
