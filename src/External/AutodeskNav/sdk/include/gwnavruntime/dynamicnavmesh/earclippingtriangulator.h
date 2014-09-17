/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


/*
* Copyright 2011 Autodesk, Inc.  All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form.
*/

#ifndef Navigation_EarClippingTriangulator_H
#define Navigation_EarClippingTriangulator_H

#include "gwnavruntime/math/vec2i.h"
#include "gwnavruntime/containers/kyarray.h"


namespace Kaim
{

class DynamicTriangulation;

class EarClippingTriangulatorInputPolygon
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	EarClippingTriangulatorInputPolygon() {}
public:
	KyArray<KyUInt32> m_pointIndices;
};

class EarClippingTriangulatorInputPolygonWithHoles
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	EarClippingTriangulatorInputPolygonWithHoles() {}
public:
	KyArray<Vec2i> m_points;
	EarClippingTriangulatorInputPolygon m_exterior;
	KyArray<EarClippingTriangulatorInputPolygon> m_holes;
};

class EarClippingTriangulator
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	EarClippingTriangulator(const EarClippingTriangulatorInputPolygonWithHoles& input, DynamicTriangulation& output) :
	m_input(&input), m_output(&output) {}

	KyResult Triangulate();

public:
	typedef KyUInt16 PolygonVertexIdx;
	static const PolygonVertexIdx PolygonVertexIdx_Invalid = KyUInt16MAXVAL;

	class PolygonVertex
	{
	public:
		enum Type
		{
			TYPE_CONVEX = 0,
			TYPE_REFLEX = 1,
			TYPE_EAR = 2,
			TYPE_UNKNOWN, };

		PolygonVertex() :
			m_posIdx(KyUInt16MAXVAL),
			m_vertexType((KyUInt16)TYPE_UNKNOWN),
			m_nextPolygonVertexIdx(PolygonVertexIdx_Invalid),
			m_prevPolygonVertexIdx(PolygonVertexIdx_Invalid),
			m_duplicateVertexIdx(PolygonVertexIdx_Invalid)
		{}

		Type GetVertexType() const { return (Type)m_vertexType; }
		void SetVertexType(Type vertexType)  { m_vertexType = KyUInt16(vertexType); }
		KyUInt16 m_posIdx;
		KyUInt16 m_vertexType;
		PolygonVertexIdx m_nextPolygonVertexIdx;
		PolygonVertexIdx m_prevPolygonVertexIdx;
		PolygonVertexIdx m_duplicateVertexIdx;
	};

	class HoleIdxWithMaxXVertexIdx
	{
	public:
		HoleIdxWithMaxXVertexIdx() : m_toConnectIdx(PolygonVertexIdx_Invalid) {}
		Vec2i m_posToConnect;
		PolygonVertexIdx m_toConnectIdx;
	};

	class FindVertexIdxResult
	{
	public:
		enum ResultType { NOTFOUND, INTERSECTION_ON_VERTEX, INTERSECTION_ON_EDGE };

		FindVertexIdxResult () : m_resultType(NOTFOUND), m_intersectionPolygonVertexIdx(PolygonVertexIdx_Invalid), m_xIntersection(KyInt32MAXVAL) {}

		void SetVertexIntersection(KyInt32 xIntersection, PolygonVertexIdx intersectionPolygonVertexIdx)
		{
			m_resultType = INTERSECTION_ON_VERTEX;
			m_xIntersection = xIntersection;
			m_intersectionPolygonVertexIdx = intersectionPolygonVertexIdx;
		}

		void SetEdgeIntersection(const Vec2i& edgeStart, const Vec2i& edgeEnd, PolygonVertexIdx intersectionPolygonVertexIdx)
		{
			m_resultType = INTERSECTION_ON_EDGE;
			m_edgeStart = edgeStart;
			m_edgeEnd = edgeEnd;
			m_intersectionPolygonVertexIdx = intersectionPolygonVertexIdx;
		}

		ResultType m_resultType;
		PolygonVertexIdx m_intersectionPolygonVertexIdx;
		KyInt32 m_xIntersection;
		Vec2i m_edgeStart;
		Vec2i m_edgeEnd;
	};

	KY_INLINE PolygonVertex::Type GetPolygonVertexType(const Vec2i& prevPos, const Vec2i& currentPos, const Vec2i& nextPos) const
	{
		const KyInt32 crossProduct = (nextPos - currentPos) ^ (prevPos - currentPos);
		return crossProduct > 0 ? PolygonVertex::TYPE_CONVEX : PolygonVertex::TYPE_REFLEX;
	}

	KY_INLINE bool HasDuplicate(PolygonVertexIdx polygonVertexIdx) const { return m_polygonVertices[polygonVertexIdx].m_duplicateVertexIdx != polygonVertexIdx; }


	void FillPolygonVerticesForExterior(PolygonVertexIdx& currentNewPolygonVertexIdx); 
	void FillPolygonVerticesForHole(const EarClippingTriangulatorInputPolygon& holePolygon, HoleIdxWithMaxXVertexIdx& currentHoleMinX, PolygonVertexIdx& currentNewPolygonVertexIdx);

	KyResult LinkHoleToExterior(HoleIdxWithMaxXVertexIdx& currentHoleMinX, PolygonVertexIdx& currentNewPolygonVertexIdx);
	KyUInt32 FindAndClipEar(PolygonVertexIdx& firstValidVertexIdx); // return the number of removed PolygonVertices

	void FindPolygonVertexIdxToLinkTo(KyUInt32 posToConnectIdx, FindVertexIdxResult& result);
	void FindPolygonVertexIdxToLinkToForEdgeIntersection(KyUInt32 posToConnectIdx, FindVertexIdxResult& result);
	PolygonVertexIdx FindBestCandidateForHoleConnection(KyUInt32 posToConnectIdx);
	bool IsInAngularSectorOfVertex(const Vec2i& posToConnect, const PolygonVertex* polygonVertex) const;


	bool IsPolygonVertexAnEar(const PolygonVertex* polygonVertex) const;
	void UpdateVertexConvexStatus(PolygonVertex* polygonVertex);
	void UpdateVertexEarStatus(PolygonVertex* polygonVertex);

	void PrintInputPolygonForDebug();
public:
	const EarClippingTriangulatorInputPolygonWithHoles* m_input;
	DynamicTriangulation* m_output;
	KyArray<PolygonVertex> m_polygonVertices; // the "linked" polygon structure
};

} // namespace Kaim

#endif //Navigation_EarClippingTriangulator_H
