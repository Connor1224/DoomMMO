/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_DynamicPdgInputCell_H
#define GwNavGen_DynamicPdgInputCell_H


#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavgeneration/input/taggedtriangle3i.h"
#include "gwnavruntime/world/tagvolume.h"
#include "gwnavruntime/base/kyguid.h"


namespace Kaim
{

class GeneratorSystem;
class IDynamicPdgInputCellVisitor;
class SpatializedPdgInput;


class DynamicPdgInputCell
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	DynamicPdgInputCell();

	DynamicPdgInputCell(CellCoord x, CellCoord y);

	~DynamicPdgInputCell();

	void Clear();
	void PushTriangle(const DynamicTaggedTriangle3i& triangle);
	void PushTagVolume(const TagVolume& volume);

public:
	KyGuid m_sectorGuid;
	CellPos m_cellPos;
	KyInt32 m_minAltitude;
	KyInt32 m_maxAltitude;

	KyArray<DynamicTaggedTriangle3i> m_triangles;
	KyArray< Ptr<TagVolume> > m_tagVolumes;
};


}


#endif
