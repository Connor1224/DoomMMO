/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_PointOfInterestDisplay_H
#define Navigation_PointOfInterestDisplay_H

#include "gwnavruntime/visualsystem/visualcolor.h"
#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"
#include "gwnavruntime/spatialization/spatializedpointdisplay.h"


namespace Kaim
{

class PointOfInterestBlob;


enum PointOfInterestDisplayListBuilderConfigEnum
{
	PointOfInterestDisplay_Spatialization
};

class PointOfInterestDisplayConfig
{
public:
	PointOfInterestDisplayConfig() { SetDefaults(); }

	void SetDefaults()
	{
		m_displaySpatialization = false;
		m_detailedDatabaseIndex = KyUInt32MAXVAL;
	}

	bool m_displaySpatialization;
	KyInt32 m_detailedDatabaseIndex;
};

class PointOfInterestDisplayParameters
{
public:
	PointOfInterestDisplayParameters() { SetDefaults(); }

	void SetDefaults()
	{
		m_flagColor_Undefined = VisualColor::Red;
		m_flagColor_Custom = VisualColor::Orange;
		m_mastHeight = 2.5f;
		m_flagHeight = 0.5f;
		m_circleRadius = 0.2f;
	}

	SpatializedPointDisplayParameters m_spatializedPointDisplayParameters;
	VisualColor m_flagColor_Undefined;
	VisualColor m_flagColor_Custom;
	KyFloat32 m_mastHeight;
	KyFloat32 m_flagHeight;
	KyFloat32 m_circleRadius;
};

class PointOfInterestDisplayListBuilder: public IDisplayListBuilder
{
public:
	PointOfInterestDisplayListBuilder() {}

	virtual void Init();
	virtual KyUInt32 GetVisualDebugElementType() { return DisplayListBuilderElementType_PointOfInterest; }

private:
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 deepBlobSize);
	static const VisualColor& GetColorFromPoiType(KyUInt32 pointOfInterestType, const PointOfInterestDisplayParameters& parameters);

public:
	static void Display(
		ScopedDisplayList* displayList, const PointOfInterestBlob* pointOfInterestBlob,
		const PointOfInterestDisplayConfig& displayConfig,
		const PointOfInterestDisplayParameters& parameters = PointOfInterestDisplayParameters());

	PointOfInterestDisplayParameters m_parameters;
};

} // namespace Kaim

#endif // Navigation_PointOfInterestDisplay_H
