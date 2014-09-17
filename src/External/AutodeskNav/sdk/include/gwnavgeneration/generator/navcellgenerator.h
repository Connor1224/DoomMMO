/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: LASI
#ifndef GwNavGen_NavCellGenerator_H
#define GwNavGen_NavCellGenerator_H


#include "gwnavruntime/navmesh/celldesc.h"
#include "gwnavgeneration/generator/cellgeneratorprofiling.h"
#include "gwnavruntime/blob/blobhandler.h"
#include "gwnavgeneration/containers/tlsarray.h"

namespace Kaim
{

class GeneratorSystem;
class DynamicRasterCell;
class DynamicNavRasterCell;
class DynamicNavCellHeightField;
class BoundaryGraph;
class PdgInputCell;
class GeneratorSectorBuilder;
class GeneratorSector;
class NavCellBlob;
class CellGenerator;
class NavTag;

/* NavCellGenerator is where most of the effective generation algorithms are run. */
class NavCellGenerator
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	NavCellGenerator(CellGenerator* cellGenerator, const KyArrayPOD<const PdgInputCell*>& inputCells);
	~NavCellGenerator();

	void ClearAll();

	// clears all the m_dynamic temporary data ...
	void ClearGenerationData();

	KyResult GenerateRawNavCell(BlobHandler<NavCellBlob>& handler);

	bool IsEmpty() const { return m_isEmpty; }

private:
	void ComputeUniqueAndIndexNavTag();
	void FindOrAddNavTag(const NavTag& tag);
	
	// quick exit to avoid unnecessary overlaps
	bool AreInputCellsAltitudesOverlapping(); 

	KyResult GenerateRaster();
	KyResult GenerateNavRaster();
	KyResult GenerateHeightField();
	KyResult GenerateBoundaries();
	KyResult GenerateNavCell(BlobHandler<NavCellBlob>& handler);

	// must be called AFTER BuildNavDataCell
	KyResult DumpIntermediateNavCell(BlobHandler<NavCellBlob>& handler);

	GeneratorSector* GetParentSector();

private:
	// Allocated and maintained by the cellGenerator
	CellGenerator* m_cellGenerator;
	const KyArrayPOD<const PdgInputCell*>* m_inputCells;
	KyArrayTLS_POD<const NavTag*> m_navTagArray;

	// dynamic (navgen) data. Allocated and maintained by the navcellGenerator
	DynamicRasterCell* m_dynamicRaster;
	DynamicNavRasterCell* m_dynamicNavRaster;
	DynamicNavCellHeightField* m_navcellHeightField;
	BoundaryGraph* m_boundaryGraph;
	KyArrayTLS_POD<KyUInt32> m_navTagIdxTable;

	bool m_isEmpty;
};


}


#endif
