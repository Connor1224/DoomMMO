/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

#ifndef Navigation_PolygonBuilder_H
#define Navigation_PolygonBuilder_H

#include "gwnavruntime/kernel/SF_Alg.h"
#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/dynamicnavmesh/tagvolumemergertypes.h"
#include "gwnavruntime/containers/bitfield.h"
#include "gwnavruntime/math/geometryfunctions.h"

namespace Kaim
{

class ScopedDisplayList;
class WorkingMemory;

class PolygonBuilder
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public: 
	typedef KyArray<const MergerRawInputEdgePiece*> BuilderInput;
	typedef KyArray<MergedPolygonWithHoles> BuilderOutput;

public: 
	PolygonBuilder(const BuilderInput& inputEdges) : m_inputEdges(&inputEdges) {}
	KyResult Build(WorkingMemory* workingMemory, BuilderOutput& result);

public: 
	void RenderInput(KyFloat32 rasterPrecision, const Vec2i& cellOffset, ScopedDisplayList& displayList);
	void RenderOutput(const BuilderOutput& output, KyFloat32 rasterPrecision, const Vec2i& cellOffset, ScopedDisplayList& displayList);

private: 
	KyResult ExtractRawPolygons(WorkingMemory* workingMemory);
	KyResult BindHoles();
	KyResult BuildOutput(BuilderOutput& result);

private: 
	const BuilderInput* m_inputEdges;
	KyArray<MergedPolygon> m_extractedPolygons;
	KyArray<Vec2i> m_rightMostPoints;
	KyArray<PolygonWinding> m_extractedPolygonWinding;
	KyArray<KyUInt32> m_holeTable;
};

} // namespace Kaim

#endif // Navigation_PolygonBuilder_H
