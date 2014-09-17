/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// Primary contact: MAMU - secondary contact: NOBODY
#ifndef Navigation_RayCastQuery_DisplayListBuilder_H
#define Navigation_RayCastQuery_DisplayListBuilder_H

#include "gwnavruntime/visualsystem/displaylist.h"
#include "gwnavruntime/queries/blobs/raycastqueryblob.h"
#include "gwnavruntime/queries/raycastquery.h"
#include "gwnavruntime/queries/blobs/querydynamicoutputdisplaylistbuilder.h"

namespace Kaim
{

class RayCastQueryDisplayListBuilder : public IDisplayListBuilder
{
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 /*deepBlobSize*/ = 0)
	{
		const RayCastQueryBlob* queryBlob = (RayCastQueryBlob*) blob;
		
		RayCastQueryOutputBlob* queryOutputBlob = queryBlob->m_queryOutput.Ptr();
		if (queryOutputBlob != NULL)
		{
			Vec3f offsetVector = Vec3f::UnitZ();
			float arrowHalfWidth = 0.05f;

			DisplayShapeColor shapeColor;
			switch((RayCastQueryResult)queryOutputBlob->m_result)
			{
				case RAYCAST_NOT_INITIALIZED: break;
				case RAYCAST_NOT_PROCESSED:
					{
						shapeColor.m_triangleColor = VisualColor::Orange;
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, arrowHalfWidth, 
							shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, shapeColor.m_triangleColor);
						displayList->PushText(queryBlob->m_startPos3f + offsetVector, VisualColor::Red, "Not processed...");
					}
					break;
				case RAYCAST_DONE_START_OUTSIDE:
					{
						shapeColor.m_triangleColor = VisualColor::Red;
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, arrowHalfWidth, 
							shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, shapeColor.m_triangleColor);
						displayList->PushText(queryBlob->m_startPos3f + offsetVector, shapeColor.m_triangleColor, "Start outside!");
					}
					break;
				case RAYCAST_DONE_START_NAVTAG_FORBIDDEN:
					{
						shapeColor.m_triangleColor = VisualColor::Red;
						displayList->PushText(queryBlob->m_startPos3f + offsetVector, shapeColor.m_triangleColor, "Start NavTag forbidden!");
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, arrowHalfWidth, 
							shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, shapeColor.m_triangleColor);

						shapeColor.SetOnlyLineColor(shapeColor.m_triangleColor);
						displayList->PushTriangle(queryOutputBlob->m_startTriangle.m_triangle, shapeColor);
					}
					break;
				case RAYCAST_DONE_CANNOT_MOVE:
					{
						shapeColor.m_triangleColor = VisualColor::Red;
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, arrowHalfWidth, 
							shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, shapeColor.m_triangleColor);

						shapeColor.SetOnlyLineColor(VisualColor::Lime);
						displayList->PushTriangle(queryOutputBlob->m_startTriangle.m_triangle, shapeColor);
					}
					break;
				case RAYCAST_DONE_LACK_OF_WORKING_MEMORY:
					{
						shapeColor.m_triangleColor = VisualColor::Red;
						displayList->PushText(queryBlob->m_startPos3f + offsetVector, shapeColor.m_triangleColor, "Lack of working memory!");
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, arrowHalfWidth, 
							shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, shapeColor.m_triangleColor);

						shapeColor.SetOnlyLineColor(VisualColor::Lime);
						displayList->PushTriangle(queryOutputBlob->m_startTriangle.m_triangle, shapeColor);
						
					}
					break;
				case RAYCAST_DONE_QUERY_TOO_LONG:
					{
						shapeColor.m_triangleColor = VisualColor::Red;
						displayList->PushText(queryBlob->m_startPos3f + offsetVector, shapeColor.m_triangleColor, "Arrival in wrong floor!");
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, arrowHalfWidth, 
							shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, shapeColor.m_triangleColor);

						shapeColor.SetOnlyLineColor(VisualColor::Lime);
						displayList->PushTriangle(queryOutputBlob->m_startTriangle.m_triangle, shapeColor);
					}
					break;
				case RAYCAST_DONE_UNKNOWN_ERROR:
					{
						shapeColor.m_triangleColor = VisualColor::Red;
						displayList->PushText(queryBlob->m_startPos3f + offsetVector, shapeColor.m_triangleColor, "Unknown error!");
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, arrowHalfWidth, 
							shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, shapeColor.m_triangleColor);
					}
					break;
				case RAYCAST_DONE_ARRIVALPOS_FOUND_MAXDIST_REACHED:
					{
						shapeColor.m_triangleColor = VisualColor::Lime;
						displayList->PushArrow(queryBlob->m_startPos3f, queryOutputBlob->m_arrivalPos3f, arrowHalfWidth, shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryOutputBlob->m_arrivalPos3f, shapeColor.m_triangleColor);

						shapeColor.SetOnlyLineColor(VisualColor::Lime);
						displayList->PushTriangle(queryOutputBlob->m_startTriangle.m_triangle, shapeColor);
						displayList->PushTriangle(queryOutputBlob->m_arrivalTriangle.m_triangle, shapeColor);
						
					}
					break;
				case RAYCAST_DONE_COLLISION_FOUND_ARRIVAL_ERROR:
					{
						shapeColor.m_triangleColor = VisualColor::Red;
						displayList->PushArrow(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, arrowHalfWidth, 
							shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryBlob->m_startPos3f + queryBlob->m_normalizedDir2d * queryBlob->m_maxDist, shapeColor.m_triangleColor);

						displayList->PushText(queryOutputBlob->m_collisionPos3f + offsetVector, VisualColor::Red, "Collision point (arrival error)");
						displayList->PushLine(queryOutputBlob->m_collisionPos3f, queryOutputBlob->m_collisionPos3f + offsetVector, VisualColor::Red);

						shapeColor.SetOnlyLineColor(VisualColor::Lime);
						displayList->PushTriangle(queryOutputBlob->m_startTriangle.m_triangle, shapeColor);
					}
					break;
				case RAYCAST_DONE_ARRIVALPOS_FOUND_AFTER_COLLISION:
					{
						shapeColor.m_triangleColor = VisualColor::Orange;
						displayList->PushArrow(queryBlob->m_startPos3f, queryOutputBlob->m_arrivalPos3f, arrowHalfWidth, 
							shapeColor, 3.0f, 0.1f);
						displayList->PushLine(queryBlob->m_startPos3f, queryOutputBlob->m_arrivalPos3f, shapeColor.m_triangleColor);

						displayList->PushLine(queryOutputBlob->m_collisionPos3f, queryOutputBlob->m_collisionPos3f + offsetVector, VisualColor::Orange);
						displayList->PushText(queryOutputBlob->m_collisionPos3f + offsetVector, VisualColor::Orange, "Collision point");

						shapeColor.SetOnlyLineColor(VisualColor::Lime);
						displayList->PushTriangle(queryOutputBlob->m_startTriangle.m_triangle, shapeColor);
						displayList->PushTriangle(queryOutputBlob->m_arrivalTriangle.m_triangle, shapeColor);
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