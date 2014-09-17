/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// ---------- Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_NavCellStitcher_H
#define Navigation_NavCellStitcher_H

#include "gwnavruntime/containers/kyarray.h"
#include "gwnavruntime/navmesh/blobs/navvertex.h"
#include "gwnavruntime/kernel/SF_RefCount.h"


namespace Kaim
{

class NavCell;
class NavCellGrid;
class NavCellPosInfo;
class NavFloor;
class ActiveCell;
class Database;
class NavHalfEdgeRawPtr;

struct CellInfo
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_NavData)
public:
	ActiveCell* m_activeCell;
	CellPos m_cellPos;
};

struct EdgeVertices
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_NavData)
public:
	NavVertex m_startVertex;
	NavVertex m_endVertex;
	KyFloat32 m_startAltitude;
	KyFloat32 m_endAltitude;
};

class NavFloorStitcher
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_NavData)
public:
	NavFloorStitcher(Database* database) : m_altitudeTolerance(0.5f), m_database(database) {}

	void StitchAllNavFloorsOfNavCell(const CellPos& cellPos, NavCell& navCell);
	void UnStitchAllNavFloorsOfNavCell(NavCell& navCell);
	void UnStitchNavFloor(NavFloor* currentNavFloor);

private :
	void RetrieveNavFloorCandidates(NavFloor* navFloor, CellInfo& currentCellInfo, CellInfo* neihbourCellsInfo);
	void FindNavFloorsCandidatesInCell(NavFloor* navFloor, CellInfo& currentCellInfo, FloorBoundariesType floorBoundariesType, KyArrayPOD<NavFloor*, MemStat_NavData>& candidates);
	void LinkWithCandidates(Ptr<NavFloor>& navFloor, CellInfo* neihbourCellsInfo);

	template <class VertexComparator>
	void FindEdgeLinkInCandidates(const NavHalfEdgeRawPtr& edgePtr, KyUInt32 boundaryEdgeIdx, const EdgeVertices& edgeVertices, NavHalfEdgeRawPtr& link, NavHalfEdgeType wantedEdgeType, KyArrayPOD<NavFloor*, MemStat_NavData>& candidates);
	void ComputeNeihbourCellsInfo(const CellPos& cellPos, CellInfo& currentCellInfo, CellInfo* neihbourCellsInfo);
public:
	KyFloat32 m_altitudeTolerance;
	Database* m_database;

private:
	KyArrayPOD<NavFloor*, MemStat_NavData> m_candidateInCell;
	KyArrayPOD<NavFloor*, MemStat_NavData> m_candidateInNeihgboursCell[4];
};


}

#endif //Navigation_NavCellStitcher_H

