/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

#ifndef KAIM_INPUT_EXTRACTOR_H
#define KAIM_INPUT_EXTRACTOR_H

// primary contact: LASI - secondary contact: NONE

#include "gwnavruntime/containers/kyarray.h"
#include "gwnavruntime/database/navtag.h"
#include "gwnavruntime/dynamicnavmesh/tagvolumemergertypes.h"
#include "gwnavruntime/kernel/SF_RefCount.h"

namespace Kaim
{

class WorkingMemory;
class NavFloor;
class Database;
class TagVolumeMergerQuery;
class TagVolume;

class TagVolumeMergerQueryInputExtractor
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public: 
	TagVolumeMergerQueryInputExtractor(TagVolumeMergerQuery* query) : 
	m_query(query) {}

public:
	KyResult AssignTagVolumesToFloors();
	KyResult ExtractInputData(WorkingMemory* workingMemory);

private: 
	KyUInt32 FindOrAddNavTag_Sorted(const DynamicNavTag& navTag, KyUInt32 firstConsideredIndex); // returns the index of corresponding navtag in m_navTags
	KyResult FilterTagVolumes();
	KyResult ExtractNavtags();
	KyResult ExtractNavFloorContours(WorkingMemory* workingMemory);
	KyResult ExtractTagVolumeContours(WorkingMemory* workingMemory);
	void ComputeNavFloorDependancies(const NavFloor* navfloor, const CoordBox64& tagVolumeBox, KyArray<KyUInt32>& navfloorDependancies);

private: 
	// input
	TagVolumeMergerQuery* m_query;

	// internal
	KyArray<KyArray<Ptr<TagVolume> > >  m_tagVolumesImpactingNavFloors;
};

}

#endif //KAIM_INPUT_EXTRACTOR_H
