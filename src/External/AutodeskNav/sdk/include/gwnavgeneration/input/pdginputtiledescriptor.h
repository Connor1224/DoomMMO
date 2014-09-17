/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LASI - secondary contact: NOBODY
#ifndef GwNavGen_PdgInputTileDescriptorBlob_H
#define GwNavGen_PdgInputTileDescriptorBlob_H

#include "gwnavruntime/blob/blobarray.h"
#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/containers/kyarray.h"
#include "gwnavruntime/kernel/SF_String.h"


namespace Kaim
{


class PdgInputTileDescriptorBlob
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_CLASS_WITHOUT_COPY(PdgInputTileDescriptorBlob)

public:
	PdgInputTileDescriptorBlob() {}

	BlobArray<char> m_tileFilename;
	BlobArray<CellPos> m_cellPositions;
	TilePos m_tilePos;
};
inline void SwapEndianness(Endianness::Target e, PdgInputTileDescriptorBlob& self)
{
	SwapEndianness(e, self.m_tileFilename);
	SwapEndianness(e, self.m_cellPositions);
	SwapEndianness(e, self.m_tilePos);
}


struct PdgInputTileDescriptor
{
public:
	PdgInputTileDescriptor() {} // To make KyArray happy

	PdgInputTileDescriptor(const TilePos& tilepos, const KyArray<CellPos>& cellPositions, const String& tileFilename)
		: m_tilePos(tilepos), m_cellPositions(cellPositions), m_tileFilename(tileFilename) {}

	void Init(const PdgInputTileDescriptorBlob& blob)
	{
		m_tileFilename = blob.m_tileFilename.GetValues();

		m_cellPositions.Resize(blob.m_cellPositions.GetCount());
		const CellPos* blobCellPositions = blob.m_cellPositions.GetValues();
		for (UPInt i = 0; i < blob.m_cellPositions.GetCount(); ++i)
			m_cellPositions[i] = blobCellPositions[i];

		m_tilePos = blob.m_tilePos;
	}

public:
	TilePos m_tilePos;
	KyArray<CellPos> m_cellPositions;
	String m_tileFilename;
};



} // namespace Kaim

#endif
