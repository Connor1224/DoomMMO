/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


#ifndef GwNavGen_TagVolumesVisualRepresentation_H
#define GwNavGen_TagVolumesVisualRepresentation_H


#include "gwnavruntime/visualsystem/visualrepresentation.h"
#include "gwnavruntime/visualsystem/visualcolor.h"


namespace Kaim
{

class PdgInput;


class TagVolumesVisualRepresentation : public VisualRepresentation
{
public:
	TagVolumesVisualRepresentation(PdgInput* pdgInput) : VisualRepresentation(pdgInput) {}
	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);
};


}

#endif
