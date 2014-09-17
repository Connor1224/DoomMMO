/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LASI - secondary contact: GUAL
#ifndef GwNavGen_NavRasterCell_H
#define GwNavGen_NavRasterCell_H


#include "gwnavruntime/base/endianness.h"
#include "gwnavruntime/basesystem/coordsystem.h"
#include "gwnavruntime/navmesh/squarepartition.h"
#include "gwnavruntime/blob/blobarray.h"
#include "gwnavruntime/math/triangle3i.h"
#include "gwnavruntime/navmesh/celldesc.h"
#include "gwnavruntime/containers/stringstream.h"
#include "gwnavgeneration/generator/generatorblobtyperegistry.h"

namespace Kaim
{

// After the erosion pass, we tag the pixels that are directly adjacent to
// certain features. 
	enum NavRasterFeatureMask
	{
		UNDEFINED_NAVRASTER_FEATURE = 0,
		NAVRASTER_HOLE_PIXEL = 1,
		NAVRASTER_WALL_PIXEL = 2
		// next = 4/8/16 etc... 
	};

	enum NavRasterCardinalDir
	{
		NAVRASTER_EAST_DIR = 0, 
		NAVRASTER_NORTH_DIR = 1, 
		NAVRASTER_WEST_DIR = 2, 
		NAVRASTER_SOUTH_DIR = 3
	};

class NavRasterColumnElement
{
public:
	typedef KyUInt32 ElementColor;
	typedef KyUInt32 LayerId;
	static const KyUInt32 s_navRaster_noConnection; /*= KyUInt32MAXVAL*/
	static const ElementColor s_navRaster_noColor; /*= KyUInt32MAXVAL*/
	static const ElementColor s_navRaster_irrelevantColor; /*= KyUInt32MAXVAL - 7*/ //to be compliant with boundary colors
	static const ElementColor s_navRaster_noLayer; /*= KyUInt32MAXVAL*/

public:
	
	inline KyUInt32 GetNeighborIndex(KyUInt32 connectionIdx) const { return m_connections[connectionIdx].neighborIndex;}
	inline KyUInt32 GetEastNeighborIndex() const { return m_connections[0].neighborIndex;}
	inline KyUInt32 GetNorthNeighborIndex() const { return m_connections[1].neighborIndex;}
	inline KyUInt32 GetWestNeighborIndex() const { return m_connections[2].neighborIndex;}
	inline KyUInt32 GetSouthNeighborIndex() const { return m_connections[3].neighborIndex;}

	class Connection 
	{
	public: 
		Connection() { neighborIndex = s_navRaster_noConnection; }
		KyUInt32 neighborIndex; 
	};
	

public:

	NavRasterColumnElement()
	{
		for (KyInt8 i = 0 ; i < 4 ; ++i)
		{
			m_connections[i].neighborIndex = s_navRaster_noConnection;
		}
		m_altitude = 0;
		m_color = s_navRaster_noColor;
		m_navTagIdx = KyUInt32MAXVAL;
		m_layerId = s_navRaster_noLayer;
	}

	inline KyFloat32 GetAltitude() const { return m_altitude;}

	KyUInt32 GetFeatureMask(KyUInt32 dir) const
	{
		KyUInt32 bitshift = (dir*8);
		return ((0xFF << bitshift) & m_featureBitField) >> bitshift;
	}

	KyFloat32 m_altitude;
	ElementColor m_color;
	KyUInt32 m_navTagIdx;
	LayerId m_layerId;
	KyUInt32 m_featureBitField; // among NavRasterFeatureMask enum values

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

inline void SwapEndianness(Endianness::Target e, NavRasterColumnElement& self)
{
	SwapEndianness(e, self.m_altitude);
	SwapEndianness(e, self.m_color);
	SwapEndianness(e, self.m_navTagIdx);
	SwapEndianness(e, self.m_layerId);
	SwapEndianness(e, self.m_connections[0].neighborIndex);
	SwapEndianness(e, self.m_connections[1].neighborIndex);
	SwapEndianness(e, self.m_connections[2].neighborIndex);
	SwapEndianness(e, self.m_connections[3].neighborIndex);
}

//In the corners of the exclusive pixel box, a single color might not
//be enough to describe the pixel and ensure consistency across borders
//For these pixels, we can require bevel edges
//We therefore use a particular element with an extra "half" color
//we arbitrarily decide that m_color correspond the left part of outgoing
//bevel half edge whereas m_secondColor correspond to the right part
//
//  ___NW__   _NE__
//  | \ 2 |  | 1 / |
//	|  \  |  |  /  |   1: m_color
//	|_1_\_|  |_/_2_|   2: m_secondColor
//  __SW__    __SE_
//	| 2 / |  | \ 1 |
//	|  /  |  |  \  |
//	|_/_1_|  |_2_\_|

class NavRasterCornerColumnElement
{
public: 
	typedef KyUInt32 ElementColor;
	typedef KyUInt32 LayerId;
	typedef NavRasterColumnElement Element;

public:
	NavRasterCornerColumnElement() :
		m_altitude(0),
		m_color(Element::s_navRaster_noColor),
		m_secondColor(Element::s_navRaster_noColor),
		m_layerId(Element::s_navRaster_noLayer),
		m_navTagIdx(KyUInt32MAXVAL)
	{
		for (KyInt8 i = 0 ; i < 4 ; ++i)
		{
			m_connections[i].neighborIndex = Element::s_navRaster_noConnection;
		}
	}

	bool AreColorsConsistent() { return m_secondColor == m_color; }

public: 
	KyFloat32 m_altitude;
	ElementColor m_color;
	ElementColor m_secondColor;
	LayerId m_layerId;
	KyUInt32 m_navTagIdx;

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
	Element::Connection m_connections[4];
	
};

inline void SwapEndianness(Endianness::Target e, NavRasterCornerColumnElement& self)
{
	SwapEndianness(e, self.m_altitude);
	SwapEndianness(e, self.m_color);
	SwapEndianness(e, self.m_navTagIdx);
	SwapEndianness(e, self.m_layerId);
	SwapEndianness(e, self.m_secondColor);
	SwapEndianness(e, self.m_connections[0].neighborIndex);
	SwapEndianness(e, self.m_connections[1].neighborIndex);
	SwapEndianness(e, self.m_connections[2].neighborIndex);
	SwapEndianness(e, self.m_connections[3].neighborIndex);
}


class NavRasterColumn
{
public:
	NavRasterColumn() {}

public:
	BlobArray<NavRasterColumnElement> m_navRasterPoints;

private:
	NavRasterColumn& operator=(const NavRasterColumn& navRasterColumn);
	NavRasterColumn(const NavRasterColumn& navRasterColumn);
};
inline void SwapEndianness(Endianness::Target e, NavRasterColumn& self)
{
	SwapEndianness(e, self.m_navRasterPoints);
}

class NavRasterCornerColumn
{
public:
	NavRasterCornerColumn() {}

public:
	BlobArray<NavRasterCornerColumnElement> m_navRasterCornerPoints;

private:
	NavRasterCornerColumn& operator=(const NavRasterCornerColumn& navRasterColumn);
	NavRasterCornerColumn(const NavRasterCornerColumn& navRasterColumn);
};
inline void SwapEndianness(Endianness::Target e, NavRasterCornerColumn& self)
{
	SwapEndianness(e, self.m_navRasterCornerPoints);
}


class NavRasterCell
{
	KY_ROOT_BLOB_CLASS(Generator, NavRasterCell, 0)

public:
	enum CornerColumnIndexes
	{
		NORTH_WEST_CORNER = 0,
		NORTH_EAST_CORNER = 1,
		SOUTH_EAST_CORNER = 2,
		SOUTH_WEST_CORNER = 3
	};

	KyFloat32 m_rasterPrecision;
	SquarePartition m_partition;
	CellDesc m_cellDesc;
	BlobArray<NavRasterColumn> m_columns; // row major
	NavRasterCornerColumn m_cornerColumns[4];
};
inline void SwapEndianness(Endianness::Target e, NavRasterCell& self)
{
	SwapEndianness(e, self.m_rasterPrecision);
	SwapEndianness(e, self.m_partition);
	SwapEndianness(e, self.m_cellDesc);
	SwapEndianness(e, self.m_columns);
	SwapEndianness(e, self.m_cornerColumns[NavRasterCell::NORTH_WEST_CORNER]);
	SwapEndianness(e, self.m_cornerColumns[NavRasterCell::NORTH_EAST_CORNER]);
	SwapEndianness(e, self.m_cornerColumns[NavRasterCell::SOUTH_EAST_CORNER]);
	SwapEndianness(e, self.m_cornerColumns[NavRasterCell::SOUTH_WEST_CORNER]);
}

}


#endif

