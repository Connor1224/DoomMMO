/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_IPathFinderQuery_H
#define Navigation_IPathFinderQuery_H

#include "gwnavruntime/querysystem/iquery.h"
#include "gwnavruntime/math/vec3f.h"
#include "gwnavruntime/queries/blobs/queryblobcategory.h"
#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavruntime/kernel/SF_String.h"
#include "gwnavruntime/path/path.h"

namespace Kaim
{

class PathClamperContext;
class QueryUtils;

/// Base class for all pathfinder queries.
/// Pathfinder queries takes all at least a start position as input and returns a
/// Path as an output.
class IPathFinderQuery : public ITimeSlicedQuery
{
public:
	/// Enumerates the possible status of a IPathFinderQuery.
	enum PathFinderResult
	{
		PathFinderNotInitialized, ///< The IPathFinderQuery is not yet initialized.
		PathFinderNotComputed,    ///< The IPathFinderQuery is initialized but has not been processed.
		PathFinderUnderProcess,   ///< The IPathFinderQuery is currently under process.
		PathFinderSuccess,        ///< The IPathFinderQuery has been completed and a Path has been found.
		PathFinderFailure         ///< The IPathFinderQuery has been completed but no Path has been found.
	};

	// ---------------------------------- Public Member Functions ----------------------------------

	IPathFinderQuery();
	virtual ~IPathFinderQuery() {}

	Path* GetPath() const;
	const Vec3f& GetStartPos() const;

	// ---------------------------------- Virtual Member Functions ----------------------------------

	/// This function is virtual because not all the PathFinders have a preset destination, it may return KY_NULL.
	virtual const Vec3f* GetDestination() const = 0;

	/// Fills the String with an explanation of the result. Use this to debug your pathfinder query.
	virtual void GetPathFinderTextResult(String&) const {}

	virtual PathFinderResult GetPathFinderResult() const = 0;

protected :
	// Returns a ClampResult as KyUInt32.
	KyUInt32 ClampOneEdge(PathClamperContext* clamperContext, QueryUtils& queryUtils, KyUInt32& intersectionTestCount);
	// pass startPos and endPos since they may be different from the real computed because of outside navMesh
	// If different the start/end pos of the computed path, they will be added to the output path
	void ComputePathFromPathClamperContext(QueryUtils& queryUtils, const Vec3f& startPos, const Vec3f& endPos);

public:
	// ---------------------------------- Public Data Members ----------------------------------

	Vec3f m_startPos3f;           ///< The starting position for the Path request.
	Ptr<Path> m_path;             ///< The Path found by the query.
	KyUInt32 m_dataBaseChangeIdx; ///< Stores the revision of the Database when the Path has been computed.
};

KY_INLINE IPathFinderQuery::IPathFinderQuery() :
	m_startPos3f(KyFloat32MAXVAL, KyFloat32MAXVAL, KyFloat32MAXVAL),
	m_path(KY_NULL),
	m_dataBaseChangeIdx(0)
{}

KY_INLINE Path*        IPathFinderQuery::GetPath()     const { return m_path;       }
KY_INLINE const Vec3f& IPathFinderQuery::GetStartPos() const { return m_startPos3f; }

}

#endif //Navigation_IPathFinderQuery_H

