/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/




#ifndef Navigation_VisualAxisFrame_H
#define Navigation_VisualAxisFrame_H


#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/visualsystem/visualrepresentation.h"


namespace Kaim
{

/// This class represents a set of three-dimensional axes. By default, each axis line is 20 meters in length.
/// The VisualAxisFrame class, in conjunction with the VisualAxisFrameRepresentation class that represents its geometry, 
/// can help you test your NavData rendering system.
class VisualAxisFrame
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_VisualRep)

public:
	VisualAxisFrame() : m_size(20.f) {}

public:
	KyFloat32 m_size; ///< The length of each axis line in meters. 
};


/// This implementation of VisualRepresentation pushes geometry that represents an VisualAxisFrame object.
/// You can use this class to test your NavData rendering system.
/// -	The right (X) axis is drawn using a red arrow.
/// -	The front (Y) axis is drawn using a green arrow.
/// -	The up (Z) axis is drawn using a blue arrow.
/// This is the same axis system visible in the standalone Gameware Navigation Lab tool. You can also use this class yourself 
/// to test your NavData rendering system. 
class VisualAxisFrameRepresentation : public VisualRepresentation
{
public:
	/// Constructs a new VisualAxisFrameRepresentation.
	/// \param axisFrame			The VisualAxisFrame instance whose geometry this object will construct. 
	VisualAxisFrameRepresentation(VisualAxisFrame* axisFrame) : VisualRepresentation(axisFrame) {}

	virtual ~VisualAxisFrameRepresentation() {}

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);
};


}

#endif
