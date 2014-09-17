/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_NavFloorBlobBuilder_H
#define Navigation_NavFloorBlobBuilder_H

#include "gwnavruntime/blob/baseblobbuilder.h"

namespace Kaim
{

class DynamicNavFloor;
class NavFloorBlob;
class NavFloorLinkInfo;
class NavHalfEdge;
class NavTag;
template<class T>
class KyArrayTLS_POD;

class NavFloorBlobBuilder : public BaseBlobBuilder<NavFloorBlob>
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_NavData)
public:
	NavFloorBlobBuilder(const DynamicNavFloor& dynamicNavFloor) :
		m_dynamicNavFloor(&dynamicNavFloor) {}

private:
	virtual void DoBuild();

	void WriteTriangles(NavHalfEdge* edges, KyUInt32 numberOfTriangles) const;
private:
	const DynamicNavFloor* m_dynamicNavFloor;

};



class NavFloorLinkInfoBlobBuilder : public BaseBlobBuilder<NavFloorLinkInfo>
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_NavData)
public:
	NavFloorLinkInfoBlobBuilder(const DynamicNavFloor& dynamicNavFloor) :
	m_dynamicNavFloor(&dynamicNavFloor) {}

private:
	virtual void DoBuild();
private:
	const DynamicNavFloor* m_dynamicNavFloor;
};

} // namespace Kaim

#endif // Navigation_NavFloorBlobBuilder_H
