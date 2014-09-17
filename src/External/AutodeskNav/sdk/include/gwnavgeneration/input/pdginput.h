/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_PdgInput_H
#define GwNavGen_PdgInput_H


#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/blob/blobaggregate.h"
#include "gwnavgeneration/generator/generatorparameters.h"
#include "gwnavgeneration/input/pdginputcell.h"
#include "gwnavruntime/basesystem/coordsystem.h"
#include "gwnavgeneration/input/seedpointarray.h"
#include "gwnavgeneration/input/pdginputtiledescriptor.h"
#include "gwnavruntime/containers/sharedpoollist.h"
#include "gwnavgeneration/input/pdginputtile.h"
#include "gwnavgeneration/generator/generatorsector.h"


namespace Kaim
{

class GeneratorSystem;
class SpatializedPdgInput;
class PdgInputLoader;

class PdgInput
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_CLASS_WITHOUT_COPY(PdgInput)
private: 
	struct LoadedTile
	{
		KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	public:
		LoadedTile() : m_info(KY_NULL) {} // for KyArray<LoadedTile>
		LoadedTile(const PdgInputTileDescriptor* info) : m_info(info) {}
	public:
		const PdgInputTileDescriptor* m_info;
		BlobHandler<PdgInputTile> m_handler;
	};
public:
	/*	Constructor to only read a PdgInput.*/
	PdgInput() : m_sys(KY_NULL) {}

	/*	---Internal--- Constructor used during NavData generation.*/
	PdgInput(GeneratorSystem* sys) : m_sys(sys) {}
	
	~PdgInput() { 	UnloadTiles(); }

	/*	Load uses GeneratorFile if a GeneratorSystem has been specified in constructor.
		Otherwise it loads from specified filename.*/
	//KyResult Load(const char* fullFileName, FileOpenerBase* fileOpener = KY_NULL);

	//KyResult Load(File* file);

	/* ! Will automatically clear the previously loaded tile*/
	PdgInputTile* LoadSingleTile(KyUInt32 tileIdx); 

	/* ! Will load a new Tile (if not already loaded) and add it to the list */
	PdgInputTile* GetOrLoadTile(const PdgInputTileDescriptor& info); 

	/* ! Will unload all the previoulsy loaded tiles */
	void UnloadTiles(); 

	/* Load from rootDirectory/sectorName.PdgInput */
	// KyResult LoadFromSectorName(const char* sectorName);

	KyResult InitFromFlatSpatializedPdgInput(Ptr<GeneratorSector> sectorConfig, SpatializedPdgInput& spatializedPdgInput);
	
	KyResult InitFromTiledSpatializedPdgInput(Ptr<GeneratorSector> sectorConfig, SpatializedPdgInput& spatializedPdgInput);

	void SetSeedPoints(KyArray<Vec3f>& seedPoints) { m_seedPoints = seedPoints; }

public:
	GeneratorSystem* m_sys;

	GeneratorParameters                            m_globalConfig;
	Ptr<GeneratorSector>	                           m_sector;
	KyArray< Ptr<BlobHandler<PdgInputCell> > > m_inputCells;
	KyArray<Vec3f>                             m_seedPoints;
	KyArray<PdgInputTileDescriptor>            m_tileDescriptors;
	KyArray<LoadedTile*>                       m_loadedTiles;
};



}


#endif

