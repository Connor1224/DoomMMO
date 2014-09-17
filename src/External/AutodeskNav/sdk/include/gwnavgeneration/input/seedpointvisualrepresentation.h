/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


#ifndef GwNavGen_SeedPointVisualRepresentation_H
#define GwNavGen_SeedPointVisualRepresentation_H


#include "gwnavruntime/visualsystem/visualrepresentation.h"


namespace Kaim
{


class SeedPointVisualRepresentation : public VisualRepresentation
{
public:
	SeedPointVisualRepresentation(const Vec3f* position, VisualColor color = VisualColor(0, 255, 0, 255))
		: VisualRepresentation(position), m_color(color) {}

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);

public:
	VisualColor m_color;
};


}

#endif // Navigation_SeedPointVisualRepresentation_H
