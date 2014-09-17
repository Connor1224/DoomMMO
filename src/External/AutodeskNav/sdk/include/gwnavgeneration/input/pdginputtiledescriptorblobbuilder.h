/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LASI - secondary contact: GUAL
#ifndef GwNavGen_PdgInputTileDescriptorBlobBuilder_H
#define GwNavGen_PdgInputTileDescriptorBlobBuilder_H


#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavruntime/containers/kyarray.h"
#include "gwnavgeneration/input/pdginputtiledescriptor.h"
#include "gwnavgeneration/input/pdginputtileindex.h"


namespace Kaim
{

class PdgInputTileIndexBlobBuilder : public BaseBlobBuilder<PdgInputTileIndex>
{
public:
	PdgInputTileIndexBlobBuilder(const KyArray<PdgInputTileDescriptor>& tiles) : m_tiles(&tiles){}

private:
	virtual void DoBuild();

	const KyArray<PdgInputTileDescriptor>* m_tiles;
};


class PdgInputTileDescriptorBlobBuilder : public BaseBlobBuilder<PdgInputTileDescriptorBlob>
{
public:
	PdgInputTileDescriptorBlobBuilder(const PdgInputTileDescriptor& information) : m_info(&information) {}

private:
	virtual void DoBuild();

	const PdgInputTileDescriptor* m_info;
};


} //namespace Kaim

#endif
