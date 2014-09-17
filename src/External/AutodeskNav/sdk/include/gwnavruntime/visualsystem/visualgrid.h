/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



#ifndef Navigation_VisualGrid_H
#define Navigation_VisualGrid_H


#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/visualsystem/visualrepresentation.h"


namespace Kaim
{

/// This class represents an axis-aligned horizontal grid. By default, the grid has 10 squares on each side,
/// and each square is 10 meters in length and width.
/// The VisualGrid class, in conjunction with the VisualGridRepresentation class that represents its geometry, 
/// can help you test your NavData rendering system.
class VisualGrid
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_VisualRep)

public:
	/// Constructs a new VisualGrid. 
	VisualGrid() : m_nbCellsX(10), m_nbCellsY(10), m_cellSizeX(10.f), m_cellSizeY(10.f) {}

public:
	KyInt32 m_nbCellsX;    ///< The number of grid cells along the X axis. 
	KyInt32 m_nbCellsY;    ///< The number of grid cells along the Y axis. 
	KyFloat32 m_cellSizeX; ///< The length of each grid cell along the X axis. 
	KyFloat32 m_cellSizeY; ///< The length of each grid cell along the Y axis. 
};


/// This implementation of VisualRepresentation pushes geometry that represents a VisualGrid object.
/// The grid is drawn using gray and dark gray segments.
/// This is the same grid visible in the standalone Gameware Navigation Lab tool. You can also use this class yourself 
/// to test your NavData rendering system. 
class VisualGridRepresentation : public VisualRepresentation
{
public:
	/// Constructs a new VisualGridRepresentation.
	/// \param grid				The VisualGrid instance whose geometry this object will construct. 
	VisualGridRepresentation(VisualGrid* grid) : VisualRepresentation(grid) {}

	virtual ~VisualGridRepresentation() {}

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);
};


}

#endif
