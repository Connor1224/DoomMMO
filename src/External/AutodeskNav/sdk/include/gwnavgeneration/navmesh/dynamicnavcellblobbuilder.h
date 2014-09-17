/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: LASI (NavCellCleaner_BlobBuilder)
#ifndef GwNavGen_DynamicNavCellBlobBuilder_H
#define GwNavGen_DynamicNavCellBlobBuilder_H

#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavgeneration/generator/generatornavdatafilter.h"

namespace Kaim
{

class NavCellBlob;

// The purpose of this "special" BlobBuilder is to build a new NavCellBlob using an original NavCellBlob
// and a list of NavFloors to be removed. All the navFloorBlob-related indexes have to be rebuilt.
class NavCellCleaner_BlobBuilder : public BaseBlobBuilder<NavCellBlob>
{

private:
	typedef GeneratorNavDataFilter::NavFloorFilteringStatus FilteringStatus;

public:
	NavCellCleaner_BlobBuilder(const NavCellBlob& originalNavCell, const KyArrayPOD<FilteringStatus>& floorsToKeep)
	{
		m_originalNavCell = &originalNavCell; 
		m_floorsToKeep = &floorsToKeep; 
	}

private:
	virtual void DoBuild();

	const NavCellBlob* m_originalNavCell;
	const KyArrayPOD<FilteringStatus>* m_floorsToKeep;
};

} // namespace Kaim

#endif // GwNavGen_DynamicNavCellBlobBuilder_H
