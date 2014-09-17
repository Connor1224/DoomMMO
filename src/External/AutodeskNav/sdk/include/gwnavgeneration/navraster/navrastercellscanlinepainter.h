/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LASI - secondary contact: NONE
#ifndef GwNavGen_NavRasterCellScanlinePainter_H
#define GwNavGen_NavRasterCellScanlinePainter_H


#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavgeneration/containers/tlsarray.h"
#include "gwnavgeneration/common/generatormemory.h"

namespace Kaim
{

class DynamicNavRasterCell;
class NavRasterColumnElement;
class NavRasterWorkingColumnElement;
class DynamicNavRasterFinalColumn;

class NavRasterCellScanlinePainter
{
public:
	typedef KyUInt32 Color; //needed by inline utility functions
private:

	enum ForbiddenMergeDetectionMode
	{
		DO_CHECK_BEVELS, 
		DO_NOT_CHECK_BEVELS
	};

	enum RasterDir
	{
		EAST = 0,
		NORTH = 1,
		WEST = 2,
		SOUTH = 3
	};

	enum LineRelativePos
	{
		LINE_ABOVE,
		LINE_BELOW
	};

	enum SearchResult
	{
		SEARCH_RESULT_FOUND,
		NO_SEARCH_RESULT
	};

	enum ScanLineFillMode
	{
		USE_TERRAIN_TYPE,
		DONT_USE_TERRAIN_TYPE
	};

	struct ScanPixel{
		ScanPixel() // default Ctor
		{
			m_ptr = KY_NULL;
			m_pos = PixelPos(KyInt32MAXVAL,KyInt32MAXVAL);
			m_floor = KyUInt32MAXVAL;
		}
		ScanPixel(NavRasterColumnElement* ptr, const PixelPos& pos, KyUInt32 floor)
		{
			m_ptr = ptr;
			m_pos = pos;
			m_floor = floor;
		}
		NavRasterColumnElement* m_ptr;
		PixelPos m_pos;
		KyUInt32 m_floor;
	};

	enum ScanLineStatus
	{
		SCANLINE_NOT_COMPUTED,
		SCANLINE_COMPLETE,
		SCANLINE_INCOMPLETE
	};

	struct ScanLine
	{
		ScanLine()
		{
			m_seed = ScanPixel();
			m_status = SCANLINE_NOT_COMPUTED;
		}
		ScanLine(ScanPixel& seed)
		{
			m_seed = seed;
			m_status = SCANLINE_NOT_COMPUTED;
		}
		const PixelPos& GetPlusBoundaryPixelPos() const
		{
			if (m_plusElems.GetCount() == 0)
			{
				return m_seed.m_pos;
			}
			return m_plusElems.Back().m_pos;
		}

		const PixelPos& GetMinusBoundaryPixelPos() const
		{
			if (m_minusElems.GetCount() == 0)
			{
				return m_seed.m_pos;
			}
			return m_minusElems.Back().m_pos;
		}
		bool operator<(const ScanLine& other) const
		{
			return GetPlusBoundaryPixelPos() < other.GetMinusBoundaryPixelPos();
		}
		ScanLineStatus m_status;
		ScanPixel m_seed;
		KyArrayTLS<ScanPixel> m_plusElems;
		KyArrayTLS<ScanPixel> m_minusElems;
	};


	typedef KyArrayTLS<KyArrayTLS_POD<NavRasterColumnElement*> > PendingColorMergeTable;
	typedef KyArrayTLS<KyArrayTLS_POD<Color> > ColorMergeTable;
	
public: //keep it simple

	NavRasterCellScanlinePainter()
		: m_nbColorUsed(0), m_navRaster(KY_NULL), m_fillMode(USE_TERRAIN_TYPE), m_paintingMs(KY_NULL)
	{}

	void Clear() 
	{	
		m_nbColorUsed = 0;
		m_navTagIdxTable.Clear();
		m_colorsBledFromBorder.Clear();
		m_coloredPixelCounter.Clear();
	}

	bool IsColorPossible(PixelPos& pixelPos, KyUInt32 floor, NavRasterCellScanlinePainter::Color newColor);
	bool IsColorPossible_singleColumn(DynamicNavRasterFinalColumn& column, KyUInt32 floor, NavRasterCellScanlinePainter::Color newColor, bool ignoreCurrentColor);

	void SetNavRasterCell(DynamicNavRasterCell* cell) { m_navRaster = cell; }
	void SetFillMode(ScanLineFillMode mode) { m_fillMode = mode; }
	void SetPaintingMs(KyFloat32* paintingMs) { m_paintingMs = paintingMs; }

	KyResult Paint();
	
	const KyArrayTLS_POD<KyUInt32>& GetNavTagIdxTable() const { return m_navTagIdxTable; }
	KyUInt32 GetNbColorsUsed() const { return m_nbColorUsed; }

private:
	void ColorBorder();
	void MergeCornerColors();
	void FloodFillFromBorders();
	KyResult PaintInterior();
	void DetectForbiddenBorderMerges(bool *forbiddenPairsTable, ForbiddenMergeDetectionMode mode = DO_CHECK_BEVELS);

	void MergeBevelsCW();
	void MergeBevelsCCW(bool* forbiddenPairs);

	inline bool IsMergeLegal(Color src, Color dest, const bool* forbiddenMerges)
	{
		return !forbiddenMerges[src*m_nbColorUsed + dest] && !forbiddenMerges[dest*m_nbColorUsed + src];
	}
	bool IsMergeLegal(Color src, Color dest, const ColorMergeTable& table, const bool* forbiddenMerges);
	void BleedOnBorders();

	bool ScanLineFloodFill(const PixelPos& pos, KyUInt32 floor, Color color, const PixelBox& bbox);
	void ExpandScanLine(ScanLine& scanLine, KyUInt32 color, const PixelBox& bbox);
	SearchResult FindAdjacentLineScanPixel(const ScanPixel& pixel, LineRelativePos dir, ScanPixel& result);
	void PaintScanLine(ScanLine& scanLine, KyUInt32 color);

	void SetLayerIdFromColor();

	void FilterSmallRegions(KyUInt32 minPixelCount);

private:
	Color m_nbColorUsed;
	DynamicNavRasterCell* m_navRaster;
	ScanLineFillMode m_fillMode;
	KyFloat32* m_paintingMs;
	KyArrayTLS_POD<Color> m_colorsBledFromBorder;
	KyArrayTLS_POD<KyUInt32> m_navTagIdxTable;
	KyArrayTLS<ScanLine> m_scanLineQueue; // cache the array between floodFills to avoid multiple resizes
	KyArrayTLS_POD<KyUInt32> m_coloredPixelCounter;
};
}

#endif
