/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// Authors: GUAL
#ifndef Navigation_VisualDebugRegistry_H
#define Navigation_VisualDebugRegistry_H


#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/world/worldelementtype.h"
#include "gwnavruntime/kernel/SF_Hash.h"


namespace Kaim
{

class World;
class WorldElement;
class VisualDebugServer;

typedef Hash<KyUInt32, WorldElement*, FixedSizeHash<KyUInt32>, AllocatorGH<KyUInt32, MemStat_VisualDebugRegistry> > WorldElementHash;

class VisualDebugRegistry
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_VisualDebugRegistry)
	KY_CLASS_WITHOUT_COPY(VisualDebugRegistry)

public:
	VisualDebugRegistry(World* world) : m_world(world), m_nextVisualDebugId(0) {}
	
	~VisualDebugRegistry();

	void Register(WorldElement* worldElement);

	void UnRegister(WorldElement* worldElement);

	WorldElement* Get(KyUInt32 visualDebugId);

	WorldElement* GetAs(KyUInt32 visualDebugId, WorldElementType worldElementType);

	template<class T>
	T* GetAs(KyUInt32 visualDebugId) { return (T*)GetAs(visualDebugId, T::GetStaticType()); }

	//const WorldElementHash& GetElements() const { return m_hash; }

	// Called every frame, this is where we send visual debug data associated to WorldElements
	void SendVisualDebug(VisualDebugServer& visualDebugServer);

	void OnConnection();

private:
	World* m_world; // allows to browse directly the WorldElement collections in SendVisualDebug() instead if browsing m_hash
	KyUInt32 m_nextVisualDebugId;
	WorldElementHash m_hash; // visualDebugId => WorldElement
};

}

#endif
