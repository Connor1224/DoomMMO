/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LASI - secondary contact: GUAL
#ifndef GwNavGen_PdgInputTile_H
#define GwNavGen_PdgInputTile_H

#include "gwnavruntime/blob/blobarray.h"
#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavgeneration/input/pdginputcell.h"

namespace Kaim
{

struct PdgInputTile
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_ROOT_BLOB_CLASS(Generator, PdgInputTile, 0)

public:
	PdgInputTile() {}

public:
	TilePos m_tilePos;
	BlobArray<PdgInputCell> m_inputCells;

private:
	PdgInputTile& operator=(const PdgInputTile& other);
	PdgInputTile(const PdgInputTile& other);
};

inline void SwapEndianness(Endianness::Target e, PdgInputTile& self)
{
	SwapEndianness(e, self.m_tilePos);
	SwapEndianness(e, self.m_inputCells);
}

}


#endif

