/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LASI - secondary contact: GUAL

#ifndef GwNavGen_PdgInputTileBlobBuilder_H
#define GwNavGen_PdgInputTileBlobBuilder_H


#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavgeneration/input/pdginputtile.h"

namespace Kaim
{

class GeneratorSystem;
class DynamicPdgInputCell;

class PdgInputTileBlobBuilder : public BaseBlobBuilder<PdgInputTile>
{
public:
	PdgInputTileBlobBuilder(GeneratorSystem* sys, const TilePos& tilePos, const KyArrayPOD<DynamicPdgInputCell*>& cells) : m_sys(sys), m_tilePos(tilePos), m_cells(&cells) {}

private:
	virtual void DoBuild();

	GeneratorSystem* m_sys;
	TilePos m_tilePos;
	const KyArrayPOD<DynamicPdgInputCell*>* m_cells;
};

class PdgInputTileBlobBuilder_FromCellBlobs : public BaseBlobBuilder<PdgInputTile>
{
public:
	PdgInputTileBlobBuilder_FromCellBlobs(const TilePos& tilePos, const KyArray<Ptr<BlobHandler<PdgInputCell> > >& cells) : m_tilePos(tilePos), m_cells(&cells) {}

private:
	virtual void DoBuild();

	TilePos m_tilePos;
	const KyArray<Ptr<BlobHandler<PdgInputCell> > >* m_cells;
};

}

#endif
