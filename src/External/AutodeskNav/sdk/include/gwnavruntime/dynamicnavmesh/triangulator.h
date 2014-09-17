/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


/*
* Copyright 2011 Autodesk, Inc.  All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form.
*/

#ifndef KAIM_TRIANGULATOR_H
#define KAIM_TRIANGULATOR_H

// primary contact: LASI - secondary contact: NONE

#include "gwnavruntime/math/vec2i.h"
#include "gwnavruntime/containers/kyarray.h"
#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/dynamicnavmesh/tagvolumemergertypes.h"

namespace Kaim
{
class ScopedDisplayList;
class VisualColor;
class DynamicTriangulation;
class NavFloor;

class PolygonTriangulator
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

	public:
	void TriangulatorOuputToDynamicTriangulation(const TriangulatorOutput& triangulatorOutput, DynamicTriangulation& dynamicTriangulation);
	void TriangulatorOuputFromDynamicTriangulation(const  DynamicTriangulation& dynamicTriangulation, const KyArray<MergerRawInputEdgePiece>& inputEdges, TriangulatorOutput& triangulatordynamicTriangulationOutput);

	void RenderTriangulatorOutputToDisplayList(const TriangulatorOutput& output, ScopedDisplayList& displayList, const VisualColor& color, KyFloat32 rasterPrecision);
	void RenderTriangulatorInputToDisplayList(const TriangulatorInputPolygon& input, ScopedDisplayList& displayList, const VisualColor& color, KyFloat32 rasterPrecision);

public:
	Vec2i m_posOffset;
};

} // namespace Kaim

#endif //KAIM_TRIANGULATOR_H
