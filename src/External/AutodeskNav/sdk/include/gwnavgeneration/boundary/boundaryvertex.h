/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


	


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_BoundaryVertex_H
#define GwNavGen_BoundaryVertex_H

#include "gwnavgeneration/boundary/boundarytypes.h"

namespace Kaim
{

class BoundaryEdge;

/*
Internal class to BoundaryGraph.
This class has no constructor for performance purpose. Be careful.
*/
class BoundaryVertex
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	void Init(ExclBoundaryCoord x, ExclBoundaryCoord y, KyFloat32 altitude, KyUInt32 index);

	void ComputeStaticStatus();

	bool IsStatic() const 
	{
		return m_staticStatus == Boundary::Vertex_Static 
			|| m_staticStatus == Boundary::Vertex_FeatureDiscontinuity;
	}
	bool IsDiscontinuity() const 
	{
		return m_staticStatus == Boundary::Vertex_FeatureDiscontinuity;
	}

	bool IsNextToDiscontinuity() const;

	const ExclBoundaryPos& Pos() { return m_exclBoundaryPos; }

	//needed in NavFloorBuilder (find())
	bool operator == (const BoundaryVertex& other) const
	{
		bool result = other.m_exclBoundaryPos == m_exclBoundaryPos;
		result &= other.m_altitude == m_altitude;
		result &= other.m_index == m_index;
		return result;
	}

public:
	ExclBoundaryPos m_exclBoundaryPos; // relative to exclusivePixelBox
	KyFloat32 m_altitude;

	//       |              \     /
	//       6               7   5
	//       \/               \ /
	// --0-> V <-4--           V
	//       /\               / \ .
	//       2               1   3
	//       |              /     \ .

	BoundaryEdge* m_ins[8];

	//       /\	             \     /
	//       2                3   1
	//       |                 \ /
	// <-4---V---0->            V
	//       |                 / \ .
	//       6                5   7
	//       \/	             /     \ .

	BoundaryEdge* m_outs[8];

	Boundary::VertexStaticStatus m_staticStatus;
	KyUInt32 m_index;
};


} // namespace Kaim


#endif
