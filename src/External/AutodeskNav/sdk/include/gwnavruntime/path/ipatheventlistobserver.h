/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LAPA - secondary contact: NOBODY
#ifndef Navigation_IPathEventObserver_H
#define Navigation_IPathEventObserver_H

#include "gwnavruntime/kernel/SF_RefCount.h"
#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/path/patheventlist.h"


namespace Kaim
{
class Bot;
class NavTag;

/// IPathEventListObservers are called when related PathEventList has just been created
/// or is about to be destroyed.
/// It is useful to namely maintain a decoration on a PathEventList, such as parsing the
/// PathEvent array to add check points.
class IPathEventListObserver : public RefCountBase<IPathEventListObserver, MemStat_PathFollowing>
{

public:
	/// Explains the reason the PathEventList is about to be destroyed.
	enum DestructionPurpose
	{
		PathIsBeeingCleared,              ///< The Path is being destroyed itself.
		ReplacePathWithNewPath,           ///< A new Path has been computed, the former Path and the related PathEventList are being destroyed.
		RecomputePathEventListForSamePath ///< A new PathEventList is about to be computed for the same Path.
	};

	IPathEventListObserver() {}
	virtual ~IPathEventListObserver() {}

	/// This function is called each time a stage of Path validation/PathEventList build is done.
	/// In this function you can:
	/// - set some PathEvent as CheckPointStatus_EventIsACheckPoint,
	/// - modify the PathEvent::m_userSpecific member,
	/// - retrieve the NavTag on intervals between events to do your own stuff (such
	///   as registering the bot in the corresponding game object).
	/// The newly created PathEvents have been pushed back in pathEventList. The first new
	/// one is at firstIndexOfNewEvent. If this is the first stage of the validation
	/// (say after a new Path has been computed or NavData changed...), firstIndexOfNewEvent
	/// is 0 (the first PathEvent of the PathEventList, aka lowerBound). Note that in
	/// this case, the first PathEvent is likely to be before bot target on path.
	virtual void OnPathEventListBuldingStageDone(Bot* bot, PathEventList& pathEventList, KyUInt32 firstIndexOfNewEvent) = 0;

	/// This function is called each time a non-empty PathEventList is going to be destroyed.
	/// This happens when we need to recompute the PathValidityInterval from scratch: A new
	/// Path has been computed, NavData has changed...
	virtual void OnPathEventListDestroy(Bot* bot,  PathEventList& pathEventList, DestructionPurpose destructionPurpose) = 0;
};


/// This default IPathEventListObserver set all the PathEvent located on NavGraphVertex as CheckPoint
class DefaultPathEventListObserver : public IPathEventListObserver
{
public:
	DefaultPathEventListObserver() {}
	virtual ~DefaultPathEventListObserver() {}
	virtual void OnPathEventListDestroy(Kaim::Bot* /*bot*/, Kaim::PathEventList& /*pathEventList*/, DestructionPurpose /*destructionPurpose*/) {}
	virtual void OnPathEventListBuldingStageDone(Kaim::Bot* /*bot*/, Kaim::PathEventList& pathEventList, KyUInt32 firstIndexOfNewEvent)
	{
		for(KyUInt32 i = firstIndexOfNewEvent; i < pathEventList.GetPathEventCount(); ++i)
		{
			Kaim::PathEvent& pathEvent = pathEventList.GetPathEvent(i);
			switch (pathEvent.m_eventType)
			{
				case Kaim::PathEventType_OutsideConnection      :
				case Kaim::PathEventType_NavGraphVertex         :
				case Kaim::PathEventType_OutsideNavMeshPathNode :
				{
					pathEvent.SetCheckPointStatus(Kaim::CheckPointStatus_EventIsACheckPoint);
					break;
				}
			default :
				break;
			}
		}
	}
};

} // namespace Kaim

#endif // Navigation_IPathEventObserver_H
