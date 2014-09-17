/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_SpatializedPointDisplay_H
#define Navigation_SpatializedPointDisplay_H

#include "gwnavruntime/spatialization/spatializedpoint.h"
#include "gwnavruntime/visualsystem/visualcolor.h"
#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"


namespace Kaim
{

class PointDbSpatializationBlob;
class SpatializedPointBlob;

class SpatializedPointDisplayConfig
{
public:
	SpatializedPointDisplayConfig() { SetDefaults(); }

	void SetDefaults()
	{
		m_detailedDatabaseIndex = KyUInt32MAXVAL;
	}

	KyInt32 m_detailedDatabaseIndex;
};

class SpatializedPointDisplayParameters
{
public:
	SpatializedPointDisplayParameters() { SetDefaults(); }

	void SetDefaults()
	{
		m_linkColor = VisualColor::LightYellow;
		m_spatializedPointColor_Projected = VisualColor::Lime;
		m_spatializedPointColor_NotProjected = VisualColor::Red;
		m_spatializationDetailsColor_Projected = VisualColor::Cyan;
		m_spatializationDetailsColor_NotProjected = VisualColor::Red;
		m_trianglePointColor = VisualColor::LightGreen;
		m_spatializedPointWidth = 0.1f;
		m_rangePointsWidth = 0.1f;
	}

	VisualColor m_linkColor;
	VisualColor m_spatializedPointColor_Projected;
	VisualColor m_spatializedPointColor_NotProjected;
	VisualColor m_spatializationDetailsColor_Projected;
	VisualColor m_spatializationDetailsColor_NotProjected;
	VisualColor m_trianglePointColor;
	KyFloat32 m_spatializedPointWidth;
	KyFloat32 m_rangePointsWidth;
};

class SpatializedPointDisplayListBuilder: public IDisplayListBuilder
{
public:
	SpatializedPointDisplayListBuilder() {}

	virtual void Init();

private:
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize);

	static void DisplayTriangleProjection(ScopedDisplayList* displayList, const Vec3f& spatializedPoint,
		const PointDbSpatializationBlob& pointDbSpatializationBlob, const SpatializedPointDisplayParameters& parameters);

	static void DisplayProjectionDetails(ScopedDisplayList* displayList, const Vec3f& spatializedPoint,
		const PointDbSpatializationBlob& pointDbSpatializationBlob, const SpatializedPointDisplayParameters& parameters,
		const VisualColor& lineColor);

public:
	static void Display(
		ScopedDisplayList* displayList, const SpatializedPointBlob* spatializedPointBlob,
		const SpatializedPointDisplayConfig& displayConfig,
		const SpatializedPointDisplayParameters& parameters = SpatializedPointDisplayParameters());

	SpatializedPointDisplayParameters m_parameters;
};

} // namespace Kaim

#endif // Navigation_SpatializedPointDisplay_H
