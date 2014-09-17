/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_SpatializedPdgInput_H
#define GwNavGen_SpatializedPdgInput_H

#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/base/kyguid.h"
#include "gwnavruntime/kernel/SF_String.h"
#include "gwnavgeneration/input/dynamicgrid.h"
#include "gwnavgeneration/input/dynamicpdginputcell.h"


namespace Kaim
{

class GeneratorSystem;
class Triangle3i;
class TaggedTriangle3i;
class GeneratorSectorConfig;
class PdgInput;

/*
cells are indexed this way
  |     |     |     |     |
--+-----+-----+-----+-----+--
  |-2, 1|-1, 1| 0, 1| 1, 1|
  |     |     |     |     |
--+-----+-----+-----+-----+--
  |-2, 0|-1, 0| 0, 0| 1, 0|
  |     |     |     |     |
--+-----+-----#-----+-----+--
  |-2,-1|-1,-1| 0,-1| 1,-1|
  |     |     |     |     |
--+-----+-----+-----+---- +--
  |-2,-2|-1,-2| 0,-2| 1,-2|
  |     |     |     |     |
--+-----+-----+-----+-----+--
  |     |     |     |     |

a cell contains at least all triangles that intersect with
[cell_min - enlargement, cell_max + enlargement]
  |     |     |     |
  |   +---------+   |
--+---|-+-----+-|---+--
  |   | |     | |   |
  |   | |     | |   |
--+---|-+-----+-|---+--
  |   +---------+   |
  |     |     |     |
              >-<
               enlargement
*/
class SpatializedPdgInput
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:
	struct TileElement
	{
		KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	public:
		TileElement(KyInt32 x, KyInt32 y) : m_x(x), m_y(y) {}
		KyArrayPOD<DynamicPdgInputCell*> m_cells;
		KyInt32 m_x;
		KyInt32 m_y;
	};

	struct TileFilenamesElement
	{
		KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	public:
		TileFilenamesElement(KyInt32 x, KyInt32 y) : m_x(x), m_y(y) {}
		KyArray<String> m_filenames;
		KyInt32 m_x;
		KyInt32 m_y;
	};

public:
	SpatializedPdgInput(GeneratorSystem* sys);

	~SpatializedPdgInput() { ClearAll(); }

	void SetName(const String& name)
	{
		m_sectorName = name;
	}

	void SetMaxTriangleCountBeforeFlush(KyUInt32 nb) { m_maxTriangleCountBeforeFlush = nb; }
	void SetNbCellsByTile(KyUInt32 nb) { m_nbCellsByTile = nb; }

	void ClearAll()
	{
		ClearCellAndTileGrids();
		ClearFlushedTiledFileNames();
	}

	void ClearCellAndTileGrids();

	void ClearFlushedTiledFileNames();

	void PushTriangle(const DynamicTaggedTriangle3i& inputTriangle);

	void PushTagVolume(const TagVolume& volume);

	DynamicPdgInputCell* GetCell(CellCoord x, CellCoord y) { return m_cellGrid.GetElement(x, y); }

	DynamicPdgInputCell** GetCells() { return m_cellGrid.GetElements(); }

	KyUInt32 GetCellsCount() const { return m_cellGrid.GetElementsCount(); }

	KyResult FlushTiles();

public:
	GeneratorSystem* m_sys;

	KyUInt32 m_nbTrianglesPushed;
	KyUInt32 m_nbVolumesPushed;
	KyUInt32 m_maxTriangleCountBeforeFlush;
	KyUInt32 m_nbCellsByTile;

	DynamicGrid<DynamicPdgInputCell> m_cellGrid; //grid of cell size

	DynamicGrid<TileElement> m_tileGrid; //grid of m_nbCellsByInputTile * cellSize
	DynamicGrid<TileFilenamesElement> m_tileFilenames; //grid of m_nbCellsByInputTile * cellSize. Logs the parts saved so far

	KyGuid m_sectorGuid;
	String m_sectorName;
	String m_tilePartsTempDir;
};

}

#endif
