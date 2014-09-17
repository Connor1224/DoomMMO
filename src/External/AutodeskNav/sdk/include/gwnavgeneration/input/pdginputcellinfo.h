/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_PdgInputCellInfo_H
#define GwNavGen_PdgInputCellInfo_H


#include "gwnavruntime/base/memory.h"
#include "gwnavgeneration/generator/generatorsectorbuilder.h"


namespace Kaim
{

class PdgInputCell;
class GeneratorSectorBuilder;


class PdgInputCellInfo
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:
	PdgInputCellInfo() : m_sectorGenerator(KY_NULL), m_inputCell(KY_NULL) {} // default Ctor for KyArray<PdgInputCellInfo>
	PdgInputCellInfo(GeneratorSectorBuilder* sectorGenerator, const CellPos& cellpos, PdgInputCell* inputCell = KY_NULL) :
		m_sectorGenerator(sectorGenerator), m_cellPos(cellpos), m_inputCell(inputCell)
	{}

public:
	GeneratorSectorBuilder* m_sectorGenerator;
	CellPos m_cellPos;
	PdgInputCell* m_inputCell;
};

} // namespace Kaim

#endif
