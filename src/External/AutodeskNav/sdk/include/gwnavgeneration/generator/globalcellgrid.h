/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


	


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_GlobalCellGrid_H
#define GwNavGen_GlobalCellGrid_H


#include "gwnavgeneration/input/dynamicgrid.h"
#include "gwnavgeneration/input/pdginputcell.h"
#include "gwnavgeneration/input/pdginputcellinfo.h"

namespace Kaim
{

class GeneratorSystem;


class PdgInputCellsAtPos
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

private:
	static bool CompareCellInfo(const PdgInputCellInfo& a, const PdgInputCellInfo& b)
	{
		if (a.m_sectorGenerator->m_doGenerateNavData == b.m_sectorGenerator->m_doGenerateNavData)
			return a.m_sectorGenerator->m_sector->GetGuid() < b.m_sectorGenerator->m_sector->GetGuid();
		else
			return a.m_sectorGenerator->m_doGenerateNavData;
	};

public:
	PdgInputCellsAtPos(KyInt32 x, KyInt32 y) : m_cellPos(x, y) {}

	void AddInputCellInfo(const PdgInputCellInfo& inputCellInfo) { m_inputCellInfos.PushBack(inputCellInfo); }

	// m_inputCells from sectors with m_doGenerateOverlapNavData==true are at the beginning of the m_inputCellInfos
	void SortInputCellInfos()
	{
		Alg::QuickSort(m_inputCellInfos, CompareCellInfo);
	}

public:
	CellPos m_cellPos;
	KyArray<PdgInputCellInfo> m_inputCellInfos;
};


class GlobalCellGrid
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:
	GlobalCellGrid(GeneratorSystem* sys) : m_sys(sys), m_grid() {}

	void AddInputCellInfo(const PdgInputCellInfo& inputCellInfo)
	{
		PdgInputCellsAtPos* cellsAtPos = m_grid.GetOrCreateElement(inputCellInfo.m_cellPos.x, inputCellInfo.m_cellPos.y);
		cellsAtPos->AddInputCellInfo(inputCellInfo);
	}

	PdgInputCellsAtPos* GetCellsAtPos(CellCoord x, CellCoord y) { return m_grid.GetElement(x, y); }

	PdgInputCellsAtPos** GetAllCellsAtPos() { return m_grid.GetElements(); }

	KyUInt32 GetCellsAtPosCount() { return m_grid.GetElementsCount(); }

public:
	GeneratorSystem* m_sys;
	DynamicGrid<PdgInputCellsAtPos> m_grid;
};


}


#endif

