/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_PdgInputCellBlobBuilder_H
#define GwNavGen_PdgInputCellBlobBuilder_H


#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavgeneration/input/pdginputcell.h"
#include "gwnavruntime/containers/kyarray.h"


namespace Kaim
{

class GeneratorSystem;
class DynamicPdgInputCell;


class PdgInputCellBlobBuilder : public BaseBlobBuilder<PdgInputCell>
{
public:
	PdgInputCellBlobBuilder(GeneratorSystem* sys, const DynamicPdgInputCell& dynamicPdgInputCell) { m_sys = sys; m_dyn = &dynamicPdgInputCell; }

private:
	virtual void DoBuild();
	GeneratorSystem* m_sys;
	const DynamicPdgInputCell* m_dyn;
};

class PdgInputCellBlobMerger : public BaseBlobBuilder<PdgInputCell>
{
public:
	PdgInputCellBlobMerger(const KyArrayPOD<const PdgInputCell*>& cells) { m_cells = &cells; }

private:
	virtual void DoBuild();

	const KyArrayPOD<const PdgInputCell*>* m_cells;
};


class PdgInputCellCopier : public BaseBlobBuilder<PdgInputCell>
{
public:
	PdgInputCellCopier(const PdgInputCell* otherCell) { m_cell = otherCell; }

private:
	virtual void DoBuild()
	{
		KyArrayPOD<const PdgInputCell*> dummyArray;
		dummyArray.PushBack(m_cell);
		BLOB_BUILD(*m_blob, PdgInputCellBlobMerger(dummyArray));
	}
	const PdgInputCell* m_cell;
};

}


#endif
