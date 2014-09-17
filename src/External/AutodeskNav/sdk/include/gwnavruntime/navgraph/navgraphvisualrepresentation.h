/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// Primary contact: JUBE - secondary contact: NOBODY
#ifndef Navigation_NavGraphVisualRepresentation_H
#define Navigation_NavGraphVisualRepresentation_H


#include "gwnavruntime/visualsystem/visualrepresentation.h"


namespace Kaim
{

class NavGraph;
class NavGraphBlob;


/* This class of VisualRepresentation defines the way the data in a NavGraph
	should be rendered using triangles. */
class NavGraphVisualRepresentation : public VisualRepresentation
{

public:
	class Config
	{
	public:
		Config() : m_altitudeOffset(0.0f) {}

		void SetAltitudeOffSet(KyFloat32 altitudeOffset) { m_altitudeOffset = altitudeOffset;}

	public:
		KyFloat32 m_altitudeOffset;
	};

	explicit NavGraphVisualRepresentation(const NavGraph* navGraph) : VisualRepresentation(navGraph) {}

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);

	Config m_config;
	const NavGraphBlob* m_navGraphBlob;
};

/* This class of VisualRepresentation defines the way the data in a NavGraphBlob
	should be rendered using triangles. */
class NavGraphBlobVisualRepresentation : public VisualRepresentation
{

public:
	explicit NavGraphBlobVisualRepresentation(const NavGraphBlob* navGraphBlob) : VisualRepresentation(navGraphBlob) { m_detailLevel = KY_VISUAL_DETAIL_HIGH; }

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);

	NavGraphVisualRepresentation::Config m_config;

private:
	void DrawEdge(VisualGeometryBuilder& geometryBuilder, bool bidirectional, const Vec3f& edgeStart, const Vec3f& edgeEnd, KyFloat32 arrowHalfWidth);
	void DrawVertex(VisualGeometryBuilder& geometryBuilder, const Vec3f& pos, KyFloat32 halfWidth);
};


}

#endif

