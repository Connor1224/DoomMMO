/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_GeneratorSectorGenerator_H
#define GwNavGen_GeneratorSectorGenerator_H


#include "gwnavgeneration/generator/generatorsystem.h"
#include "gwnavgeneration/generator/generatorsector.h"
#include "gwnavgeneration/generator/generatorblobtyperegistry.h"
#include "gwnavruntime/navmesh/blobs/navmeshelementblob.h"
#include "gwnavruntime/blob/blobhandler.h"
#include "gwnavruntime/blob/blobaggregate.h"
#include "gwnavgeneration/input/pdginput.h"
#include "gwnavgeneration/input/clientinputconsumer.h"
#include "gwnavgeneration/generator/generatorguidcompound.h"
#include "gwnavgeneration/generator/navdataelementglobalmap.h"
#include "gwnavgeneration/generator/generationreport.h"
#include "gwnavruntime/navgraph/blobs/navgrapharray.h"
#include "gwnavruntime/navdata/navdataindex.h"
#include "gwnavruntime/navdata/navdata.h"
#include "gwnavruntime/kernel/SF_RefCount.h"

namespace Kaim
{

class NavCellGenerator;
class ClientInputConsumer;
class GeneratorSector;
class GeneratorNavDataElement;
class ClientInput;
class NavDataIndex;
struct PdgInputTile;
class ClientInputFeeder;

/* internal */
class GeneratorSectorBuilder
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public: 
	// determines whether or not the sector is overlapping another one
	enum SectorOverlapStatus { SECTOR_IS_OVERLAPPING_ANOTHER_SECTOR = 0, SECTOR_IS_ISOLATED };

public:
	GeneratorSectorBuilder(GeneratorSystem* sys, Ptr<GeneratorSector> sector);

	~GeneratorSectorBuilder();

	NavDataIndex* GetNavDataIndex() { return m_navDataIndexHandler.Blob(); }

public: // internal
	void ClearNavDataElements();

	void ClearCells();

	void ClearNavCells();

	KyResult ProducePdgInput(GeneratorInputProducer* producer);

	KyResult ResolveGlobalConfigFromClientInput(const ClientInput& input);

	GeneratorNavDataElement* GetOrCreateNavDataElement(const GeneratorGuidCompound& guidCompound);

	KyResult GenerateRawNavCells();
	KyResult GenerateRawNavCells_Tiled_AllMains();
	KyResult GenerateRawNavCells_Tiled_AllOverlaps();

	KyResult GenerateRawNavCells_Tiled_Main(const PdgInputTile* tile);

	KyResult FilterCells();

	KyResult AggregateNavMeshElement();

	KyResult MakeNavData();

	const GeneratorNavDataElementMap& GetNavDataElements() const { return m_navDataElements; }

	const KyGuid& GetGuid() const { return m_sector->GetGuid(); }

	const char* GetName() const { return m_sector->GetName(); }

	void ComputeSectorBBox(Box2f& box);
	void ComputeSectorCellBox(Box2i& cellBox);

	template<class OSTREAM> void LogConfiguration(OSTREAM& os);

	void ComputeStatistics();

private:
	void Init();
	void ComputeGenerationFlags();
	KyResult MakePlainSectorNavData();
	KyResult SaveNavData(const String& navDataName, BlobAggregate* navData);

public:
	GeneratorSystem* m_sys;
	Ptr<GeneratorSector> m_sector;
	ClientInputConsumer m_inputConsumer;

	Ptr<ClientInputFeeder> m_clientInputFeeder;

	PdgInput m_pdgInput; // aggregate file that contains globalConfig, sectorConfig, pdgInputCells
	GeneratorNavDataElementMap m_navDataElements;

	bool m_doGenerateClientInput;
	bool m_doGenerateNavData;

	SectorOverlapStatus m_overlapStatus;

	SectorStatistics m_sectorStatistics;
	
	BlobHandler<NavDataIndex> m_navDataIndexHandler;
};

template<class OSTREAM>
inline void GeneratorSectorBuilder::LogConfiguration(OSTREAM& os)
{
	os << "sectorName         : " << m_sector->GetName()             << Endl;
	os << "sectorGuidString   : " << m_sector->GetGuidString().ToCStr()       << Endl;
	os << "Overlap status     : " << (m_overlapStatus == SECTOR_IS_ISOLATED ? "SECTOR_IS_ISOLATED" : "SECTOR_IS_OVERLAPPING_ANOTHER_SECTOR") << Endl;
	os << "Build status       : " << (m_sector->GetBuildMode() == GenFlags::SECTOR_BUILD ? "REBUILD" : "NO REBUILD") << Endl;

	os << "doGenerateClientInput : " << m_doGenerateClientInput << Endl;
	os << "doGenerateNavData     : " << m_doGenerateNavData     << Endl;
}

} // namespace Kaim

#endif

