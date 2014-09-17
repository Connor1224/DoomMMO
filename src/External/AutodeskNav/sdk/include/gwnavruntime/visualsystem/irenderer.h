/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


#ifndef Navigation_IRenderer_H
#define Navigation_IRenderer_H

#include "gwnavruntime/visualsystem/visualcolor.h"
#include "gwnavruntime/basesystem/coordsystem.h"
#include "gwnavruntime/math/vec3f.h"
#include "gwnavruntime/base/types.h"
#include "gwnavruntime/kernel/SF_RefCount.h"
#include "gwnavruntime/containers/collection.h"

namespace Kaim
{


class RenderNodeBeginConfig
{
public:
	RenderNodeBeginConfig()
		: m_opaqueTriangleCount(0)
		, m_transparentTriangleCount(0)
		, m_lineCount(0)
		, m_textCount(0)
	{}

	void Reset()
	{
		m_opaqueTriangleCount = 0;
		m_transparentTriangleCount = 0;
		m_lineCount = 0;
		m_textCount = 0;
	}

	KyUInt32 m_opaqueTriangleCount;
	KyUInt32 m_transparentTriangleCount;
	KyUInt32 m_lineCount;
	KyUInt32 m_textCount;
};

class IRenderNode
{
public:

	IRenderNode() {}
	virtual ~IRenderNode() {}

	virtual void Begin(const RenderNodeBeginConfig& config) = 0;

	// Coordinates are given in client coordinates
	virtual void PushTriangle(const Vec3f& clientA, const Vec3f& clientB, const Vec3f& clientC, const VisualColor& color, const Vec3f& clientNormal) = 0;

	// Coordinates are given in client coordinates
	virtual void PushLine(const Vec3f& clientP, const Vec3f& clientQ, const VisualColor& color) = 0;

	// Coordinates are given in client coordinates
	virtual void PushText(const Vec3f& clientP, const VisualColor& color, const char* text, KyUInt32 length) = 0;

	virtual void End() = 0;

public:
	CoordSystem m_coordSystem;
};

typedef IRenderNode IRenderer;

}


#endif
