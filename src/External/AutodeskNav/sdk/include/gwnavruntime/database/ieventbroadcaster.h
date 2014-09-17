/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: JUBA - secondary contact: LAPA

#ifndef Navigation_IEventBroadCaster_H
#define Navigation_IEventBroadCaster_H

#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/kernel/SF_RefCount.h"

namespace Kaim
{

class Database;
class NavData;

class IEventBroadCaster : public RefCountBase<IEventBroadCaster, Stat_Default_Mem>
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_WorldFwk)
	KY_CLASS_WITHOUT_COPY(IEventBroadCaster)

public:
	IEventBroadCaster(Database* database) : m_database(database) {}

	void SetDatabase(Database* database) { m_database = database; }

	virtual void BroadCastOnAddNavData(NavData* navData);
	virtual void BroadCastOnRemoveNavData(NavData* navData);

public:
	Database* m_database;
};


} // namespace Kaim

#endif // Navigation_Database_H
