/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


	


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_BoundaryTypes_H
#define GwNavGen_BoundaryTypes_H


#include "gwnavruntime/navmesh/navmeshtypes.h"


namespace Kaim
{
	// world coordinates
	typedef KyInt32 BoundaryCoord;
	typedef Vec2i BoundaryPos;
	typedef Box2i BoundaryBox;

	// relative to NavPixelBox
	typedef KyInt32 NavBoundaryCoord;
	typedef Vec2i NavBoundaryPos;

	// relative to NavPixelBox
	typedef KyInt32 NavPixelCoord;
	typedef Vec2i NavPixelPos;
	typedef Vec3i NavPixelPos3d;

	// relative to exclusivePixelBox
	typedef KyInt32 ExclBoundaryCoord;
	typedef Vec2i ExclBoundaryPos;
	typedef Vec3i ExclBoundaryVertexPos; // x and y are relative to exclusivePixelBox

	class Boundary
	{
		KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	public:
		typedef KyUInt32 VertexStaticStatus;
		static const VertexStaticStatus Vertex_Dynamic; // = 0
		static const VertexStaticStatus Vertex_Static; //= 1
		static const VertexStaticStatus Vertex_Narrow; // = 2
		static const VertexStaticStatus Vertex_FeatureDiscontinuity; // = 3

		typedef KyUInt32 CardinalDir;
		static const CardinalDir CardinalDir_East; //  = 0; //   2
		static const CardinalDir CardinalDir_North; // = 2; // 4-+-0
		static const CardinalDir CardinalDir_West; //  = 4; //   6
		static const CardinalDir CardinalDir_South; // = 6; 
		static const CardinalDir CardinalDir_NorthEast; // = 1;//   5  4
		static const CardinalDir CardinalDir_NorthWest; // = 3;//    \/
		static const CardinalDir CardinalDir_SouthWest; // = 5;//    /\ .
		static const CardinalDir CardinalDir_SouthEast; // = 7;//   6  7
		static const CardinalDir CardinalDir_None; //  = 8;

		typedef KyUInt32 EdgeType;
		static const EdgeType EdgeType_Wall; //           = 0;
		static const EdgeType EdgeType_Hole; //           = 1;
		static const EdgeType EdgeType_CellLink_East; //  = 2;
		static const EdgeType EdgeType_CellLink_North; // = 3;
		static const EdgeType EdgeType_CellLink_West; //  = 4;
		static const EdgeType EdgeType_CellLink_South; // = 5;
		static const EdgeType EdgeType_External_FloorLink; // = 6; Represent the outer edges of a 2d layer
		static const EdgeType EdgeType_Internal_FloorLink; // = 7; Represent and edge that separate 2 navTag in the same layer
		static const EdgeType EdgeType_Unset; // = KyUInt32MAXVAL;

		typedef KyUInt32 IsVisited;
		static const IsVisited Unvisited; //         = 0;
		static const IsVisited Visited; //           = 1;
		static const IsVisited NoNeedToBeVisited; // = 2;

		typedef KyUInt32 PolylineCycle;
		static const PolylineCycle Polyline_NotCycle; // = 0;
		static const PolylineCycle Polyline_Cycle; //    = 1;
		static const PolylineCycle PolylineCycle_Unset; // = KyUInt32MAXVAL;

		typedef KyUInt32 Color;
		static const Color Color_Unset; //         = KyUInt32MAXVAL;

		typedef KyUInt32 Side;
		static const Side Left; //  = 0;
		static const Side Right; // = 1;
		static const Side Side_Unset; // = KyUInt32MAXVAL;

		typedef KyUInt32 ContourWinding;
		static const ContourWinding ContourWinding_CCW; //   = 0;
		static const ContourWinding ContourWinding_CW; //    = 1;
		static const ContourWinding ContourWinding_Unset; // = KyUInt32MAXVAL;

		typedef KyUInt32 Order;
		static const Order StraightOrder; // = 0;
		static const Order ReverseOrder; //  = 1;
		static const Order Order_Unset; //   = KyUInt32MAXVAL;
	};
}


#endif
