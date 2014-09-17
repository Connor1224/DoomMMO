/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


//  Primary contact: GUAL - secondary contact: JUBA
#ifndef Navigation_DatabaseVisRep_H
#define Navigation_DatabaseVisRep_H


#include "gwnavruntime/visualsystem/visualrepresentation.h"
#include "gwnavruntime/visualsystem/idisplaylistbuilder.h"
#include "gwnavruntime/kernel/SF_Debug.h"


namespace Kaim
{

class Database;
class ITerrainTypeColorPalette;


// do we really need to use a displaylist for databases ?
class DatabaseDisplayListBuilder : public IDisplayListBuilder
{
public:
	DatabaseDisplayListBuilder(ITerrainTypeColorPalette* palette = KY_NULL)
		: m_palette(palette)
	{}

	virtual ~DatabaseDisplayListBuilder() {}

	virtual void DoBuild(ScopedDisplayList* /*displayList*/, char* /*blob*/, KyUInt32 /*deepBlobSize*/)
	{
		KY_DEBUG_ASSERTN(false, ("DatabaseDisplayListBuilder is only able to build from a database, not from a blob"));
	}

public:
	ITerrainTypeColorPalette* m_palette;
};

// TODO rename ActiveDataVisualRepresentation to DatabaseVisRep
// use Database* instead of ActiveData* as parameter
class DatabaseVisualRepresentation : public VisualRepresentation
{
public:
	/* Constructs a new instance of this class.
		\param navMeshElementManager			The NavMeshElementManager whose geometry will be built by this object. */
	DatabaseVisualRepresentation(Database* database= KY_NULL, ITerrainTypeColorPalette* palette = KY_NULL, KyFloat32 altOffset = 0.75f)
		: VisualRepresentation(database), m_palette(palette), m_altOffset(altOffset) {}

	virtual void BuildGeometry(VisualGeometryBuilder& geometryBuilder);

public:
	ITerrainTypeColorPalette* m_palette;
	KyFloat32 m_altOffset;
};


}


#endif
