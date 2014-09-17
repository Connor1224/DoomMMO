/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LASI - secondary contact: GUAL
#ifndef GwNavGen_PdgInputTileIndex_H
#define GwNavGen_PdgInputTileIndex_H

#include "gwnavgeneration/input/pdginputtiledescriptor.h"
#include "gwnavgeneration/generator/generatorblobtyperegistry.h"


namespace Kaim
{

class PdgInputTileIndex
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_ROOT_BLOB_CLASS(Generator, PdgInputTileIndex, 0)
	KY_CLASS_WITHOUT_COPY(PdgInputTileIndex)

public: 
	PdgInputTileIndex() {}
	BlobArray<PdgInputTileDescriptorBlob> m_tileDescriptors;
};
inline void SwapEndianness(Endianness::Target e, PdgInputTileIndex& self)
{
	SwapEndianness(e, self.m_tileDescriptors);
}

} // namespace Kaim

#endif
