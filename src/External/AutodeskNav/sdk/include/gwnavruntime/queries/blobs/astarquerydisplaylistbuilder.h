/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// Primary contact: MAMU - secondary contact: NOBODY
#ifndef Navigation_AStarQuery_DisplayListBuilder_H
#define Navigation_AStarQuery_DisplayListBuilder_H

#include "gwnavruntime/visualsystem/displaylist.h"
#include "gwnavruntime/queries/blobs/astarqueryblob.h"
#include "gwnavruntime/path/pathvisrep.h"
#include "gwnavruntime/path/pathblob.h"
#include "gwnavruntime/queries/utils/baseastarquery.h"

namespace Kaim
{

class AStarQueryDisplayListBuilder : public IDisplayListBuilder
{
	virtual void DoBuild(ScopedDisplayList* displayList, char* blob, KyUInt32 /*deepBlobSize*/ = 0)
	{
		const AStarQueryBlob* aStarQueryBlob = (AStarQueryBlob*) blob;
		
		PathDisplayListBuilder pathDisplayListBuilder;
		AStarQueryOutputBlob* aStarQueryOutputBlob = aStarQueryBlob->m_queryOutput.Ptr();
		if (aStarQueryOutputBlob != NULL)
		{
			Vec3f offsetVector = Vec3f::UnitZ();
			float arrowHalfWidth = 0.05f;
			DisplayShapeColor shapeColor;
			shapeColor.m_triangleColor = VisualColor::Red;
			switch((AStarQueryResult)aStarQueryOutputBlob->m_result)
			{
				case ASTAR_NOT_INITIALIZED:  break;
				case ASTAR_NOT_PROCESSED:
				case ASTAR_PROCESSING_TRAVERSAL:
				case ASTAR_PROCESSING_TRAVERSAL_DONE:
				case ASTAR_PROCESSING_REFINING_INIT:
				case ASTAR_PROCESSING_REFINING:
				case ASTAR_PROCESSING_PATHCLAMPING_INIT:
				case ASTAR_PROCESSING_PATHCLAMPING:
				case ASTAR_PROCESSING_PATHBUILDING:
					{
						shapeColor.m_triangleColor = VisualColor::Orange;
						displayList->PushBezierArrow(aStarQueryBlob->m_startPos3f, aStarQueryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 1);
					}
					break;
				case ASTAR_DONE_START_OUTSIDE:
					{
						displayList->PushText(aStarQueryBlob->m_startPos3f + offsetVector, shapeColor.m_triangleColor, "Start outside!");
						displayList->PushBezierArrow(aStarQueryBlob->m_startPos3f, aStarQueryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 1);
					}
					break;
				case ASTAR_DONE_START_NAVTAG_FORBIDDEN:
					{
						displayList->PushText(aStarQueryBlob->m_startPos3f + offsetVector, shapeColor.m_triangleColor, "Start NavTag forbidden!");
						displayList->PushBezierArrow(aStarQueryBlob->m_startPos3f, aStarQueryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 1);
					}
					break;
				case ASTAR_DONE_END_OUTSIDE:
					{
						displayList->PushText(aStarQueryBlob->m_destPos3f + offsetVector, shapeColor.m_triangleColor, "Destination outside!");
						displayList->PushBezierArrow(aStarQueryBlob->m_startPos3f, aStarQueryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 1);
					}
					break;
				case ASTAR_DONE_END_NAVTAG_FORBIDDEN:
					{
						displayList->PushText(aStarQueryBlob->m_destPos3f + offsetVector, shapeColor.m_triangleColor, "Destination NavTag forbidden!");
						displayList->PushBezierArrow(aStarQueryBlob->m_startPos3f, aStarQueryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 1);
					}
					break;
				case ASTAR_DONE_PATH_MAX_COST_REACHED:
					{
						displayList->PushText(aStarQueryBlob->m_destPos3f + offsetVector, shapeColor.m_triangleColor, "path max cost reached !");
						displayList->PushBezierArrow(aStarQueryBlob->m_startPos3f, aStarQueryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 1);
					}
					break;
				case ASTAR_DONE_PATH_NOT_FOUND:
					{
						displayList->PushText(aStarQueryBlob->m_destPos3f + offsetVector, shapeColor.m_triangleColor, "Path not found !");
						displayList->PushBezierArrow(aStarQueryBlob->m_startPos3f, aStarQueryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 1);
					}
					break;
				case ASTAR_DONE_NAVDATA_CHANGED:
					{
						displayList->PushText(aStarQueryBlob->m_destPos3f + offsetVector, shapeColor.m_triangleColor, "NavData changed!");
						displayList->PushBezierArrow(aStarQueryBlob->m_startPos3f, aStarQueryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 1);
					}
					break;
				case ASTAR_DONE_ERROR_LACK_OF_WORKING_MEMORY:
					{
						displayList->PushText(aStarQueryBlob->m_destPos3f + offsetVector, shapeColor.m_triangleColor, "Lack of working memory!");
						displayList->PushBezierArrow(aStarQueryBlob->m_startPos3f, aStarQueryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 1);
					}
					break;
				case Kaim::ASTAR_DONE_COMPUTATION_ERROR :
					{
						displayList->PushText(aStarQueryBlob->m_destPos3f + offsetVector, shapeColor.m_triangleColor, "Computation Error");
						displayList->PushBezierArrow(aStarQueryBlob->m_startPos3f, aStarQueryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 1);
					}
					break;
				case ASTAR_DONE_PATH_FOUND:
					pathDisplayListBuilder.DoBuild(displayList, (char*)&aStarQueryOutputBlob->m_pathBlob);
					break;
				case ASTAR_DONE_COMPUTATION_CANCELED:
					{
						displayList->PushText(aStarQueryBlob->m_destPos3f + offsetVector, shapeColor.m_triangleColor, "Query Canceled");
						displayList->PushArrow(aStarQueryBlob->m_startPos3f, aStarQueryBlob->m_destPos3f, arrowHalfWidth, shapeColor, 3.0f, 0.1f);
					}
					break;
			}
		}
	}
};

}

#endif