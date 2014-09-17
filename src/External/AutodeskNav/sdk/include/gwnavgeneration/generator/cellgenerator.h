/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// primary contact: GUAL - secondary contact: LASI
#ifndef GwNavGen_CellGenerator_H
#define GwNavGen_CellGenerator_H


#include "gwnavruntime/navmesh/celldesc.h"
#include "gwnavgeneration/generator/cellgeneratorprofiling.h"
#include "gwnavgeneration/generator/generatorguidcompound.h"

#include "gwnavgeneration/generator/navcellgenerator.h"
#include "gwnavruntime/blob/blobhandler.h"
#include "gwnavruntime/navmesh/blobs/navcellblob.h"
#include "gwnavgeneration/generator/generatorsectorbuilder.h"

namespace Kaim
{

class PdgInput;
class GeneratorSystem;

class GeneratorNavDataElement;
class GeneratorSectorBuilder;

class CellGenerator
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:
	enum FilteringStatus
	{
		NAVCELLS_FILTERED,
		NAVCELLS_UNFILTERED
	};

	enum IsOverlapCell
	{
		IsOverlapCell_False = 0,
		IsOverlapCell_True = 1
	};

public:
	explicit CellGenerator(GeneratorSystem* sys) :
		m_sys(sys),
		m_navDataElement(KY_NULL),
		m_isOnOverlap(false)
	{
		m_navCellHandler = *KY_NEW BlobHandler<NavCellBlob>; // *KY_NEW to get RefCount=1
	}

	void Init(const CellPos& cellPos, GeneratorNavDataElement* navDataElement, IsOverlapCell isOverlapCell)
	{
		m_navDataElement = navDataElement;
		m_cellDesc.Init(m_sys->Partition(), cellPos, m_sys->Config().GetNormalizedRasterPrecision());
		m_isOnOverlap = (isOverlapCell == IsOverlapCell_True);
		m_cellProfiling.m_enabled = m_sys->RunOptions().m_doProfile;
	}

	void AddInputCell(const PdgInputCell* inputCell) { m_inputCells.PushBack(inputCell); }

	KyResult GenerateRawNavCell()
	{
		NavCellGenerator navCellGenerator(this, m_inputCells);
		KyResult res = navCellGenerator.GenerateRawNavCell(*m_navCellHandler.GetPtr());
		m_inputCells.Clear(); // Discard the input cells array, to clear some memory... we don't need them anymore
		return res;
	}

	void ClearNavCell() { if (m_navCellHandler) m_navCellHandler->Clear(); }

	const CellDesc& Desc() const { return m_cellDesc; }
	GeneratorSystem* Sys()             { return m_sys; }
	bool IsOnOverlap() const     { return m_isOnOverlap; }

	const NavCellBlob* GetNavCell()  { return m_navCellHandler->Blob(); }

	GeneratorNavDataElement* GetNavDataElement() { return m_navDataElement; }
	CellGeneratorProfiling& CellProfiling() { return m_cellProfiling; }

public:
	GeneratorSystem* m_sys;
	CellDesc m_cellDesc;
	GeneratorNavDataElement* m_navDataElement;
	KyArrayPOD<const PdgInputCell*> m_inputCells;
	Ptr<BlobHandler<NavCellBlob> > m_navCellHandler;
	bool m_isOnOverlap;
	CellGeneratorProfiling m_cellProfiling;
};


}


#endif
