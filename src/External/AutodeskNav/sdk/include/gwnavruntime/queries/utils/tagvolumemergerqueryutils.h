/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: LASI - secondary contact: NOBODY
#ifndef Navigation_TagVolumeQueryUtils_H
#define Navigation_TagVolumeQueryUtils_H

#include "gwnavruntime/kernel/SF_Types.h"
#include "gwnavruntime/math/vec3f.h"
#include "gwnavruntime/math/vec2i.h"
#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/dynamicnavmesh/tagvolumemergertypes.h"
#include "gwnavruntime/containers/kyarray.h"


namespace Kaim
{

class NavFloorBlob;
class DatabaseGenMetrics;
class ScopedDisplayList;
class MergedPolygonWithHoles;
class EarClippingTriangulatorInputPolygonWithHoles;
class TriangulatorOutput;
class VisualColor;
class ScopedDisplayList;

namespace TagVolumeMergerUtils
{
	Vec2i GetSnappedNavVertexPixelPos(const DatabaseGenMetrics& activeCoord, const Vec3f& pos3f);

	// RENDER
	void PolylineToDisplayList(KyFloat32 integerPrecision, Vec2i* polyline, KyUInt32 polylinePointCount, ScopedDisplayList& displayList, const VisualColor& color, KyFloat32 zOffset);

	PolygonWinding ComputeContourWinding(const KyArray<KyUInt32>& contour, const NavFloorBlob* staticNavfloorBlob);

	KyUInt32 FindOrAddPointToEarPolygon(const Kaim::Vec2i point, Kaim::EarClippingTriangulatorInputPolygonWithHoles& polygon);
	
	void ConvertTriangulatorInput(const MergedPolygonWithHoles& input, EarClippingTriangulatorInputPolygonWithHoles& convertedInput);

	void RenderTriangulatorOutputToDisplayList(const TriangulatorOutput& output, ScopedDisplayList& displayList, const Kaim::VisualColor& color, KyFloat32 rasterPrecision, const Vec2i& offset);
}

}

#endif //Navigation_TagVolumeQueryUtils_H

