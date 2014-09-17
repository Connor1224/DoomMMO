/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// Primary contact: MAMU - secondary contact: NOBODY
#ifndef Navigation_RayCanGoQuery_DisplayListBuilder_H
#define Navigation_RayCanGoQuery_DisplayListBuilder_H

#include "gwnavruntime/visualsystem/displaylist.h"
#include "gwnavruntime/queries/blobs/raycangoqueryblob.h"
#include "gwnavruntime/queries/raycangoquery.h"
#include "gwnavruntime/queries/blobs/querydynamicoutputdisplaylistbuilder.h"

namespace Kaim
{

class RayCanGoQueryDisplayListBuilder : public IDisplayListBuilder
{
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 /*deepBlobSize*/ = 0)
	{
		const RayCanGoQueryBlob* queryBlob = (RayCanGoQueryBlob*) blob;
		
		RayCanGoQueryOutputBlob* queryOutputBlob = queryBlob->m_queryOutput.Ptr();
		if (queryOutputBlob != NULL)
		{
			Vec3f offsetVector = Vec3f::UnitZ();
			float arrowHalfWidth = 0.05f;
			DisplayShapeColor shapeColor;
			switch((RayCanGoQueryResult)queryOutputBlob->m_result)
			{
				case RAYCANGO_NOT_INITIALIZED: break;
				case RAYCANGO_NOT_PROCESSED:
					{
						shapeColor.m_triangleColor = VisualColor::Orange;
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_destPos3f, shapeColor.m_triangleColor);
					}
					break;
				case RAYCANGO_DONE_START_OUTSIDE:
					{
						shapeColor.m_triangleColor = VisualColor::Red;
						displayList->PushText(queryBlob->m_startPos3f + offsetVector, shapeColor.m_triangleColor, "Start outside!");
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_destPos3f, shapeColor.m_triangleColor);
					}
					break;
				case RAYCANGO_DONE_START_NAVTAG_FORBIDDEN:
					{
						shapeColor.m_triangleColor = VisualColor::Red;
						displayList->PushText(queryBlob->m_startPos3f + offsetVector, shapeColor.m_triangleColor, "Start NavTag forbidden!");
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_destPos3f, shapeColor.m_triangleColor);
						
						shapeColor.SetOnlyLineColor(shapeColor.m_triangleColor);
						displayList->PushTriangle(queryOutputBlob->m_startTriangle.m_triangle, shapeColor);
					}
					break;
				case RAYCANGO_DONE_COLLISION_DETECTED:
					{
						shapeColor.m_triangleColor = VisualColor::Red;
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_destPos3f, shapeColor.m_triangleColor);

						shapeColor.SetOnlyLineColor(VisualColor::Lime);
						displayList->PushTriangle(queryOutputBlob->m_startTriangle.m_triangle, shapeColor);
					}
					break;
				case RAYCANGO_DONE_ARRIVAL_IN_WRONG_FLOOR:
					{
						shapeColor.m_triangleColor = VisualColor::Red;
						displayList->PushText(queryBlob->m_destPos3f + offsetVector, shapeColor.m_triangleColor, "Arrival in wrong floor!");
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_destPos3f, shapeColor.m_triangleColor);

						shapeColor.SetOnlyLineColor(VisualColor::Lime);
						displayList->PushTriangle(queryOutputBlob->m_startTriangle.m_triangle, shapeColor);
					}
					break;
				case RAYCANGO_DONE_LACK_OF_WORKING_MEMORY:
					{
						shapeColor.m_triangleColor = VisualColor::Red;
						displayList->PushText(queryBlob->m_destPos3f + offsetVector, shapeColor.m_triangleColor, "Lack of working memory!");
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_destPos3f, shapeColor.m_triangleColor);

						shapeColor.SetOnlyLineColor(VisualColor::Lime);
						displayList->PushTriangle(queryOutputBlob->m_startTriangle.m_triangle, shapeColor);
					}
					break;
				case RAYCANGO_DONE_UNKNOWN_ERROR:
					{
						shapeColor.m_triangleColor = VisualColor::Red;
						displayList->PushText(queryBlob->m_destPos3f + offsetVector, shapeColor.m_triangleColor, "Unknown error!");
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_destPos3f, shapeColor.m_triangleColor);
					}
					break;
				case RAYCANGO_DONE_SUCCESS:
					{
						shapeColor.m_triangleColor = VisualColor::Lime;
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_destPos3f, shapeColor.m_triangleColor);
						
						shapeColor.SetOnlyLineColor(VisualColor::Lime);
						displayList->PushTriangle(queryOutputBlob->m_startTriangle.m_triangle, shapeColor);
						displayList->PushTriangle(queryOutputBlob->m_destTriangle.m_triangle, shapeColor);
						
					}
					break;
			}

			QueryDynamicOutputBlob* queryDynamicOutputBlob = queryOutputBlob->m_queryDynamicOutputBlobRef.Ptr();
			if (queryDynamicOutputBlob != NULL)
			{
				QueryDynamicOutputDisplayListBuilder dynamicOutputDLBuilder;
				dynamicOutputDLBuilder.DoBuild(displayList, (char*)queryDynamicOutputBlob, 0);
			}
		}
	}
};

}

#endif