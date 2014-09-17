/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: LAPA - secondary contact: NOBODY


namespace Kaim
{

template<class NavTagTraversePredicate>
KY_INLINE FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate>::FindFirstVisiblePositionOnPathQuery()
	: BaseFindFirstVisiblePositionOnPathQuery()
	, m_predicate()
	, m_displayList(KY_NULL)
{}

template<class NavTagTraversePredicate>
KY_INLINE FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate>::FindFirstVisiblePositionOnPathQuery(const NavTagTraversePredicate& predicate)
	: BaseFindFirstVisiblePositionOnPathQuery()
	, m_predicate(predicate)
	, m_displayList(KY_NULL)
{}

template<class NavTagTraversePredicate>
KY_INLINE void FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate>::Initialize(Database* database, const PositionOnPath& startPositionOnPath, const PositionOnPath& endPositionOnPath, const Vec3f& visibilityStartPoint)
{
	BaseFindFirstVisiblePositionOnPathQuery::Initialize(database, startPositionOnPath, endPositionOnPath, visibilityStartPoint);
}

template<class NavTagTraversePredicate>
KY_INLINE void FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate>::SetVisibilityStartTrianglePtr(const NavTrianglePtr& startTrianglePtr) { BaseFindFirstVisiblePositionOnPathQuery::SetVisibilityStartTrianglePtr(startTrianglePtr); }
template<class NavTagTraversePredicate>
KY_INLINE void FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate>::SetSamplingDistance(KyFloat32 samplingDistance)                       { BaseFindFirstVisiblePositionOnPathQuery::SetSamplingDistance(samplingDistance);           }

template<class NavTagTraversePredicate>
KY_INLINE FindFirstVisiblePositionOnPathQueryResult   FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate>::GetResult()             const { return BaseFindFirstVisiblePositionOnPathQuery::GetResult();            }

template<class NavTagTraversePredicate> KY_INLINE const PositionOnPath& FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate>::GetStartPositionOnPath()        const { return BaseFindFirstVisiblePositionOnPathQuery::GetStartPositionOnPath();        }
template<class NavTagTraversePredicate> KY_INLINE const PositionOnPath& FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate>::GetEndPositionOnPath()          const { return BaseFindFirstVisiblePositionOnPathQuery::GetEndPositionOnPath();          }
template<class NavTagTraversePredicate> KY_INLINE const Vec3f&          FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate>::GetVisibilityStartPoint()       const { return BaseFindFirstVisiblePositionOnPathQuery::GetVisibilityStartPoint();       }
template<class NavTagTraversePredicate> KY_INLINE const NavTrianglePtr& FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate>::GetVisibilityStartTrianglePtr() const { return BaseFindFirstVisiblePositionOnPathQuery::GetVisibilityStartTrianglePtr(); }
template<class NavTagTraversePredicate> KY_INLINE KyFloat32             FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate>::GetSamplingDistance()           const { return BaseFindFirstVisiblePositionOnPathQuery::GetSamplingDistance();           }
template<class NavTagTraversePredicate> KY_INLINE const PositionOnPath& FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate>::GetFirstVisiblePositionOnPath() const { return BaseFindFirstVisiblePositionOnPathQuery::GetFirstVisiblePositionOnPath(); }

template<class NavTagTraversePredicate>
KY_INLINE void FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate>::Advance(WorkingMemory* workingMemory)
{
	PerformQuery(workingMemory);
	m_processStatus = QueryDone;
}

template<class NavTagTraversePredicate>
inline void FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate>::PerformQuery(WorkingMemory* workingMemory)
{
	if (GetResult() != FINDFIRSTVISIBLEPOSITIONONPATH_NOT_PROCESSED)
		return;

	KY_LOG_ERROR_IF(m_database == KY_NULL, ("the query must have been initialized with a valid Database before calling PerformQuery"));

	// Set default result value
	m_result = FINDFIRSTVISIBLEPOSITIONONPATH_DONE_NO_VISIBLE_POSITION_FOUND;

	if (m_displayList)
	{
		const KyFloat32 pointDisplaySize = Min(0.1f, 0.25f * m_samplingDistance);
		Kaim::DisplayShapeColor shapeColor;
		shapeColor.m_lineColor = VisualColor::Cyan;
		m_displayList->PushPoint(m_startPositionOnPath.GetPosition(), pointDisplaySize, shapeColor);
		shapeColor.m_lineColor = VisualColor::Magenta;
		m_displayList->PushPoint(m_endPositionOnPath.GetPosition(), pointDisplaySize, shapeColor);
	}

	// Instantiate a rayCanGoQuery to be used in all subsequent ValidateCandidate calls
	RayCanGoQuery<NavTagTraversePredicate> rayCanGoQuery(m_predicate);

	m_firstVisiblePositionOnPath = m_startPositionOnPath;
	while (m_endPositionOnPath.IsStrictlyBefore(m_firstVisiblePositionOnPath))
	{
		m_firstVisiblePositionOnPath.MoveBackward_StopAtPathNode(m_samplingDistance);

		if (ValidateCandidate(rayCanGoQuery, m_firstVisiblePositionOnPath, workingMemory) == StopSampling)
			return;
	}
}

template<class NavTagTraversePredicate>
KY_INLINE BaseFindFirstVisiblePositionOnPathQuery::CandidateValidationResult FindFirstVisiblePositionOnPathQuery<NavTagTraversePredicate>::ValidateCandidate(
	RayCanGoQuery<NavTagTraversePredicate>& rayCanGoQuery, const PositionOnPath& candidate, WorkingMemory* workingMemory)
{
	rayCanGoQuery.Initialize(m_database, m_visibilityStartPos3f, candidate.GetPosition());
	rayCanGoQuery.SetStartTrianglePtr(m_visibilityStartTrianglePtr);
	rayCanGoQuery.PerformQuery(workingMemory);

	CandidateValidationResult result = StopSampling;
	Kaim::DisplayShapeColor shapeColor;
	shapeColor.m_lineColor = VisualColor::White;

	switch (rayCanGoQuery.GetResult())
	{
	case RAYCANGO_DONE_START_OUTSIDE:
		{
			m_result = FINDFIRSTVISIBLEPOSITIONONPATH_DONE_VISIBILITYSTARTPOINT_OUTSIDE;
			shapeColor.m_lineColor = VisualColor::Magenta;
			result = StopSampling;
			break;
		}

	case RAYCANGO_DONE_START_NAVTAG_FORBIDDEN:
		{
			m_result = FINDFIRSTVISIBLEPOSITIONONPATH_DONE_VISIBILITYSTARTPOINT_NAVTAG_FORBIDDEN;
			shapeColor.m_lineColor = VisualColor::Magenta;
			result = StopSampling;
			break;
		}

	case RAYCANGO_DONE_LACK_OF_WORKING_MEMORY:
		{
			m_result = FINDFIRSTVISIBLEPOSITIONONPATH_DONE_LACK_OF_WORKING_MEMORY;
			shapeColor.m_lineColor = VisualColor::DarkRed;
			result = StopSampling;
			break;
		}

	case RAYCANGO_DONE_UNKNOWN_ERROR:
		{
			m_result = FINDFIRSTVISIBLEPOSITIONONPATH_DONE_UNKNOWN_ERROR;
			shapeColor.m_lineColor = VisualColor::DarkRed;
			result = StopSampling;
			break;
		}

	case RAYCANGO_DONE_COLLISION_DETECTED:
	case RAYCANGO_DONE_ARRIVAL_IN_WRONG_FLOOR:
		{
			shapeColor.m_lineColor = VisualColor::Red;
			result = ContinueSampling;
			break;
		}

	case RAYCANGO_DONE_SUCCESS:
		{
			m_result = FINDFIRSTVISIBLEPOSITIONONPATH_DONE_SUCCESS;
			shapeColor.m_lineColor = VisualColor::Lime;
			result = StopSampling;
			break;
		}
	default:
		{
			KY_LOG_ERROR(("This must not happen !"));
			m_result = FINDFIRSTVISIBLEPOSITIONONPATH_DONE_UNKNOWN_ERROR;
			shapeColor.m_lineColor = VisualColor::DarkRed;
			result = StopSampling;
			break;
		}
	}

	if (m_displayList)
	{
		const KyFloat32 pointDisplaySize = Min(0.1f, 0.25f * m_samplingDistance);
		m_displayList->PushPoint(candidate.GetPosition(), pointDisplaySize, shapeColor);
		m_displayList->PushLine(rayCanGoQuery.GetStartPos(), rayCanGoQuery.GetDestPos(), shapeColor.m_lineColor);
	}

	return result;
}

} // namespace Kaim
