/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LASI - secondary contact: GUAL
#ifndef GwNavGen_DynamicNavRasterCell_H
#define GwNavGen_DynamicNavRasterCell_H


#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavgeneration/common/growinglistpool.h"
#include "gwnavgeneration/common/growingsmallbuffer.h"
#include "gwnavgeneration/raster/dynamicrastercell.h"
#include "gwnavgeneration/navraster/navrastercell.h"
#include "gwnavruntime/navmesh/celldesc.h"
#include "gwnavruntime/base/timeprofiler.h"
#include "gwnavgeneration/common/stlvector.h"
#include "gwnavgeneration/navraster/navrastercellscanlinepainter.h"

namespace Kaim
{

//forward decl
class GeneratorSystem;
class BlobChunkBuilder;

class NavRasterWorkingColumnElement
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	inline KyUInt32 GetEastNeighborIndex()  const { return m_connections[0].neighborIndex;}
	inline KyUInt32 GetNorthNeighborIndex() const { return m_connections[1].neighborIndex;}
	inline KyUInt32 GetWestNeighborIndex()  const { return m_connections[2].neighborIndex;}
	inline KyUInt32 GetSouthNeighborIndex() const { return m_connections[3].neighborIndex;}

	enum ConnectionTag 
	{
		NO_TAG,
		HOLE, 
		WALL,
		STEP,
		BORDER,
		Dummy_FORCE32 = 0xFFFFFFFF
	};

	class Connection 
	{
	public: 
		Connection() {tag = NO_TAG; neighborPtr = KY_NULL; }
		KyUInt32 neighborIndex; 
		KyUInt32 tag; 
		NavRasterWorkingColumnElement* neighborPtr;
	};

public:

	NavRasterWorkingColumnElement& operator=(const NavRasterWorkingColumnElement& other) 
	{
		this->m_altitude = other.m_altitude;
		this->m_distanceFromHole = other.m_distanceFromHole;
		this->m_distanceFromWall = other.m_distanceFromWall;
		this->m_floor = other.m_floor;
		this->m_navTagIdx = other.m_navTagIdx;
		for (KyUInt32 i = 0 ; i < 4; ++i)
		{
			this->m_connections[i].neighborPtr = other.m_connections[i].neighborPtr;
			this->m_connections[i].tag = other.m_connections[i].tag;
		}
		this->m_featureBitField = other.m_featureBitField;
		return *this;
	}

	NavRasterWorkingColumnElement()
	{
		Clear();
	}
	
	void Clear()
	{
		for (KyInt8 i = 0 ; i < 4 ; ++i)
		{
			m_connections[i].neighborPtr = KY_NULL;
			m_connections[i].tag = NO_TAG;
		}
		m_altitude = -KyFloat32MAXVAL;
		m_distanceFromWall = KyFloat32MAXVAL;
		m_distanceFromHole = KyFloat32MAXVAL;
		m_floor = -1;
		m_navTagIdx = KyUInt32MAXVAL;
		m_featureBitField = 0;
	}

	// remove the pointer to the neighbor pixel and make sure we update
	// the symmetric neighbor connections as well. 
	void DisconnectFromNeighbors()
	{
		for(KyInt32 n = 0 ; n < 4; ++n)
		{
			if (m_connections[n].neighborPtr)
			{
				m_connections[n].neighborPtr->m_connections[(n+2)%4].neighborPtr = KY_NULL;
				m_connections[n].neighborPtr = KY_NULL;
			}
		}
	}

	void TagNeighbors(KyUInt32 tag)
	{
		for(KyInt32 n = 0 ; n < 4; ++n)
		{
			if (m_connections[n].neighborPtr)
			{
				// get the part of the bit field that correspond to the symmetric connection
				KyUInt32 bitShift = ((n+2)%4)*8; 
				KyUInt32 shiftedTag = tag << bitShift;
				m_connections[n].neighborPtr->m_featureBitField |= shiftedTag;
			}
		}
	}

	KyUInt32 GetFeatureMask(KyUInt32 dir)
	{
		return (0xFF << dir) && m_featureBitField;
	}

	KyFloat32 m_altitude;
	KyFloat32 m_distanceFromWall;
	KyFloat32 m_distanceFromHole;
	KyInt32 m_floor;
	KyUInt32 m_navTagIdx;
	KyUInt32 m_featureBitField;

	// connections are indexed as follow:
	//  +-----------+
	//  |     1     |
	//  |           |
	//  |2    +    0|
	//  |           |
	//  |     3     |
	//  +-----------+

	//0 corresponds to East (+1;0) and index increases CCW
	//Stores level index of neighbor position if reachable, 0xFFFFFFFF otherwise
	Connection m_connections[4];
};

class DynamicNavRasterFinalColumn
{
public:
	DynamicNavRasterFinalColumn() : m_count(0), m_elements(KY_NULL) {}
	KyUInt32 m_count;
	NavRasterColumnElement* m_elements;
};

class DynamicNavRasterFinalCornerColumn
{
public:
	DynamicNavRasterFinalCornerColumn() : m_count(0), m_elements(KY_NULL) {}
	KyUInt32 m_count;
	NavRasterCornerColumnElement* m_elements;
};

class DynamicNavRasterWorkingColumn
{
public:
	DynamicNavRasterWorkingColumn() : m_count(0), m_elements(KY_NULL) {}
	KyUInt32 m_count;
	NavRasterWorkingColumnElement* m_elements;
};

class DynamicNavRasterNavTagSubstitution
{
public:
	DynamicNavRasterNavTagSubstitution() : m_element(KY_NULL), m_navTagIdx(KY_NULL) {}
	DynamicNavRasterNavTagSubstitution(NavRasterWorkingColumnElement* _element, KyUInt32 _navTagIdx)
		: m_element(_element), m_navTagIdx(_navTagIdx) {}

	NavRasterWorkingColumnElement* m_element; 
	KyUInt32 m_navTagIdx;
};

class DynamicNavRasterCell
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
private: 
	enum NavRasterCardinalDir
	{
		EAST  = 0,
		NORTH = 1,
		WEST  = 2,
		SOUTH = 3,
		NORTH_EAST = 4,
		NORTH_WEST = 5,
		SOUTH_WEST = 6,
		SOUTH_EAST = 7
	};

public:
	enum CornerColumnIndexes
	{
		NORTH_WEST_CORNER = 0,
		NORTH_EAST_CORNER = 1,
		SOUTH_EAST_CORNER = 2,
		SOUTH_WEST_CORNER = 3
	};

	DynamicNavRasterCell(GeneratorSystem* sys, const DynamicRasterCell* inputRasterCell, const CellDesc& cellDesc);
	~DynamicNavRasterCell();

	void CreateNavRaster();

	DynamicNavRasterFinalColumn GetFinalColumnFromLocal(const PixelPos& localPixelPos) const;
	DynamicNavRasterFinalColumn GetFinalColumn(const PixelPos& pixelPos) const;

	/* Write NavRaster file file if specified in the Generator RunOptions, for debug purpose. */
	KyResult WriteIntermediateFile();

	bool IsEmpty() const { return m_isEmpty; }
	bool HasMultipleNavTags() const { return m_hasMultipleNavTags; }

	inline KyInt32 GetWorkingNeighborColumnIndex(const PixelPos& pixelPos, KyInt32 dir);
	inline KyInt32 GetFinalNeighborColumnIndex(const PixelPos& pixelPos, KyInt32 dir);

	inline NavRasterColumnElement* GetFinalNeighbor(const PixelPos& pos, const NavRasterColumnElement& pixel, KyInt32 dir);

	inline DynamicNavRasterWorkingColumn* GetWorkingNeighborColumn(const PixelPos& pos, KyInt32 dir);
	inline DynamicNavRasterFinalColumn* GetFinalNeighborColumn(const PixelPos& pos, KyInt32 dir);

	inline NavRasterWorkingColumnElement* GetWorkingNeighbor(const NavRasterWorkingColumnElement& pixel, KyInt32 dir);
	inline const NavRasterWorkingColumnElement* GetWorkingNeighbor(const NavRasterWorkingColumnElement& pixel, KyInt32 dir) const;


private:
	void CreateUnpaintNavRaster();
	void BuildRaster();
	void FilterIsolatedSteps();
	void TagConnections();
	void ErodePixels();

	void MakeFinalColumns();

	void EnforceSymetry();
	void ComputeManhattanDistanceMap();
	void ComputeChessboardDistanceMap();

	void ReleaseWorkingColumns()
	{
		//swap trick (see: http://stackoverflow.com/questions/253157/how-to-downsize-stdvector)
		TlsStlVector<DynamicNavRasterWorkingColumn>().swap(m_workingColumns);
		m_workingColumnsPool.Release();
	}

	const NavRasterWorkingColumnElement* GetWorkingDiagonalNeighbor(const NavRasterWorkingColumnElement& pos, KyInt32 dir)  const;
	NavRasterWorkingColumnElement* GetWorkingDiagonalNeighbor(const NavRasterWorkingColumnElement& pos, KyInt32 dir);

public:
	GeneratorSystem* m_sys;
	CellDesc m_cellDesc;
	KyInt32 m_intEntityRadius;
	KyUInt32 m_highestFloor;

public:
	const DynamicRasterCell* m_inputRasterCell;

	PixelBox m_enlargedPixelBox;
	PixelBox m_navPixelBox;

	//coloring
	NavRasterCellScanlinePainter m_painter;

	// final columns
	TlsStlVector<DynamicNavRasterFinalColumn> m_finalColumns;
	DynamicNavRasterFinalCornerColumn m_finalCornerColumns[4];
	GrowingSmallBufferPool m_finalColumnsPool;
	GrowingSmallBufferPool m_finalCornerColumnsPool;

	TlsStlVector<DynamicNavRasterWorkingColumn> m_workingColumns;
	GrowingSmallBufferPool m_workingColumnsPool;

	bool m_isEmpty;
	bool m_hasMultipleNavTags; //extracted from the rasterCell

	KyFloat32* m_navRasterMs;
	KyFloat32* m_paintingMs;

	friend class DynamicNavRasterCellBlobBuilder;
};

inline DynamicNavRasterFinalColumn DynamicNavRasterCell::GetFinalColumnFromLocal(const PixelPos& localPixelPos) const
{
	KyInt32 idx = m_navPixelBox.GetRowMajorIndexFromLocalPos(localPixelPos);
	return m_finalColumns[idx];
}

inline DynamicNavRasterFinalColumn DynamicNavRasterCell::GetFinalColumn(const PixelPos& pixelPos) const
{
	KyInt32 idx = m_navPixelBox.GetRowMajorIndex(pixelPos);
	return m_finalColumns[idx];
}

inline KyInt32 DynamicNavRasterCell::GetWorkingNeighborColumnIndex(const PixelPos& pixelPos, KyInt32 dir)
{
	PixelPos neighborPos;
	switch(dir)
	{
	case 0: neighborPos = pixelPos.NeighborEast();
		break;
	case 1: neighborPos = pixelPos.NeighborNorth();
		break;
	case 2: neighborPos = pixelPos.NeighborWest();
		break;
	case 3: neighborPos = pixelPos.NeighborSouth();
		break;
	default: return - 1;
	}

	if (m_enlargedPixelBox.IsInside(neighborPos))
	{
		return m_enlargedPixelBox.GetRowMajorIndex(neighborPos);
	}
	return -1;
}

inline KyInt32 DynamicNavRasterCell::GetFinalNeighborColumnIndex(const PixelPos& pixelPos, KyInt32 dir)
{
	PixelPos neighborPos;
	switch(dir)
	{
	case 0: neighborPos = pixelPos.NeighborEast();
		break;
	case 1: neighborPos = pixelPos.NeighborNorth();
		break;
	case 2: neighborPos = pixelPos.NeighborWest();
		break;
	case 3: neighborPos = pixelPos.NeighborSouth();
		break;
	default: return - 1;
	}

	if (m_navPixelBox.IsInside(neighborPos))
	{
		return m_navPixelBox.GetRowMajorIndex(neighborPos);
	}
	return -1;
}

inline NavRasterColumnElement* DynamicNavRasterCell::GetFinalNeighbor(const PixelPos& pos, const NavRasterColumnElement& pixel, KyInt32 dir)
{
	PixelPos neighborPos;
	KyUInt32 neighborIndexInColumn;
	switch(dir)
	{
	case 0: 
		neighborPos = pos.NeighborEast();
		neighborIndexInColumn = pixel.GetEastNeighborIndex();
		break;
	case 1: 
		neighborPos = pos.NeighborNorth();
		neighborIndexInColumn = pixel.GetNorthNeighborIndex();
		break;
	case 2: 
		neighborPos = pos.NeighborWest();
		neighborIndexInColumn = pixel.GetWestNeighborIndex();
		break;
	case 3:
		neighborPos = pos.NeighborSouth();
		neighborIndexInColumn = pixel.GetSouthNeighborIndex();
		break;
	default: return KY_NULL;
	}

	if (m_navPixelBox.IsInside(neighborPos))
	{
		KyInt32 neighborColumnIndex = GetFinalNeighborColumnIndex(pos,dir);
		if (neighborColumnIndex < 0)
			return KY_NULL;
		DynamicNavRasterFinalColumn& neighborColumn = m_finalColumns[neighborColumnIndex];
		if (neighborColumn.m_count > neighborIndexInColumn)
		{
			return &m_finalColumns[neighborColumnIndex].m_elements[neighborIndexInColumn];
		}
	}
	return KY_NULL;
}

inline NavRasterWorkingColumnElement* DynamicNavRasterCell::GetWorkingNeighbor(const NavRasterWorkingColumnElement& pixel, KyInt32 dir)
{
	return dir < 4 ? pixel.m_connections[dir].neighborPtr : GetWorkingDiagonalNeighbor(pixel, dir);
}

inline const NavRasterWorkingColumnElement* DynamicNavRasterCell::GetWorkingNeighbor(const NavRasterWorkingColumnElement& pixel, KyInt32 dir) const
{
	return dir < 4 ? pixel.m_connections[dir].neighborPtr : GetWorkingDiagonalNeighbor(pixel, dir);
}

inline DynamicNavRasterWorkingColumn* DynamicNavRasterCell::GetWorkingNeighborColumn(const PixelPos& pos, KyInt32 dir)
{
	PixelPos neighborPos;
	switch(dir)
	{
	case 0: 
		neighborPos = pos.NeighborEast();
		break;
	case 1: 
		neighborPos = pos.NeighborNorth();
		break;
	case 2: 
		neighborPos = pos.NeighborWest();
		break;
	case 3:
		neighborPos = pos.NeighborSouth();
		break;
	default: return KY_NULL;
	}

	if (m_enlargedPixelBox.IsInside(neighborPos))
	{
		KyInt32 neighborColumnIndex = GetWorkingNeighborColumnIndex(pos,dir);
		if (neighborColumnIndex < 0)
			return KY_NULL;
		DynamicNavRasterWorkingColumn& neighborColumn = m_workingColumns[neighborColumnIndex];
		return &neighborColumn;
	}
	return KY_NULL;

}

inline DynamicNavRasterFinalColumn* DynamicNavRasterCell::GetFinalNeighborColumn(const PixelPos& pos, KyInt32 dir)
{
	PixelPos neighborPos;
	switch(dir)
	{
	case 0: 
		neighborPos = pos.NeighborEast();
		break;
	case 1: 
		neighborPos = pos.NeighborNorth();
		break;
	case 2: 
		neighborPos = pos.NeighborWest();
		break;
	case 3:
		neighborPos = pos.NeighborSouth();
		break;
	default: return KY_NULL;
	}

	if (m_navPixelBox.IsInside(neighborPos))
	{
		KyInt32 neighborColumnIndex = GetFinalNeighborColumnIndex(pos,dir);
		if (neighborColumnIndex < 0)
			return KY_NULL;
		DynamicNavRasterFinalColumn& neighborColumn = m_finalColumns[neighborColumnIndex];
		return &neighborColumn;
	}
	return KY_NULL;
}

}

#endif
