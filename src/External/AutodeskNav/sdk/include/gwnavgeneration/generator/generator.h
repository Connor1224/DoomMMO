/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: LASI
#ifndef GwNavGen_Generator_H
#define GwNavGen_Generator_H

#include "gwnavruntime/base/fileopener.h"
#include "gwnavruntime/kernel/SF_String.h"
#include "gwnavruntime/kernel/SF_RefCount.h"

#include "gwnavgeneration/common/generatormemory.h"
#include "gwnavgeneration/common/generatormemoryheap.h"
#include "gwnavgeneration/generator/generatorprofiling.h"
#include "gwnavgeneration/generator/globalcellgrid.h"
#include "gwnavgeneration/generator/igeneratorglue.h"
#include "gwnavgeneration/generator/generatorblobtyperegistry.h"
#include "gwnavgeneration/generator/generatorguidcompound.h"
#include "gwnavgeneration/generator/generatorsector.h"
#include "gwnavgeneration/generator/generatorsystem.h"
#include "gwnavgeneration/input/clientinputconsumer.h"
#include "gwnavgeneration/input/generatorinputproducer.h"
#include "gwnavgeneration/generator/generatorsectorlist.h"
#include "gwnavgeneration/generator/generatorconfigblob.h"
#include "gwnavgeneration/generator/generatorinputoutput.h"

namespace Kaim
{

class PdgInput;
class CellGenerator;
class IParallelForInterface;
class CollisionInterfaceFactory;
class NavMeshElementBlob;
class ClientInputConsumer;
class GeneratorSectorBuilder;
class GeneratorNavDataElement;


/// The Generator is the principal class in the NavData generation system.
/// \section generatoroutput Output files
/// The Generator is capable of generating multiple kinds of data files:
/// -  .ClientInput files, which are records of the input triangles, NavTags and TagVolumes that describe
///     your terrain, passed as inputs to the NavData generation system.
/// -  .GenConfig files, which record the list of sectors and the generation parameters that where used when the Generator::Generate()
///     function was called. See Kaim::GeneratorInputOutput class
/// -  Intermediate data files, which record temporary data calculated during the generation process. Mostly for debug purposes.
/// -  Final .NavData files, which contain the final data that you will load into your game at runtime.
/// You can control which types of data files you want to generate each time you use the Generator.
/// \section generatorsectors Sectors and multi-sectors
/// The Generator can be set up with any number of sectors, each of which is responsible for a block of terrain geometry
/// within a defined area of your game world. Separate input, intermediate and output data are created for each sector,
/// so that the sectors can be streamed into memory independently at runtime. Adjacent sectors whose triangles lie close to
/// each other will be linked automatically at runtime.
/// You can also configure the Generator to re-partition each sector that you provide into smaller sectors automatically.
class Generator : public RefCountBaseNTS<Generator, MemStat_NavDataGen>
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

friend class NavDataIndexBlobBuilder_FromGenerator;

public:
	/// Constructs a new Generator initialized with the provided information.
	/// \param producer                A pointer to an object that the Generator will use to retrieve the input data for each
	///                                  sector.This object is ref-counted : It will be destroyed automatically when appropriate.
	/// \param guidGeneratorInterface  A pointer to an object that the Generator will use to create GUIDs for the NavData it generates.
	///                                 For Windows platforms, you can use the DefaultGuidGeneratorInterface class. For Linux platforms,
	///                                 you will need to supply your own implementation. This object is ref-counted: It will be destroyed
	///                                 automatically when appropriate.
	/// \param parallelForInterface    A pointer to an object that the Generator will use to parallelize computations over multiple
	///                                 processors, or KY_NULL in order to perform all computations sequentially on a single processor.
	///	                                This object is ref-counted; It will be destroyed automatically when appropriate.
	/// \param tlsAlloc                A pointer to an object that the Generator will use to allocate and free memory in secondary threads,
	///                                  when configured to use parallel generation.  This object is ref-counted; It will be destroyed
	///                                  automatically when appropriate.
	Generator(Ptr<GeneratorInputProducer> producer, 
			  Ptr<IParallelForInterface> parallelForInterface = KY_NULL, 
			  Ptr<ITlsAlloc> tlsAlloc = KY_NULL);

	/// Constructs a new Generator initialized with the provided GeneratorInputProducer and IGeneratorGlue.
	/// Requires an instance of a class that derives from IGeneratorGlue, which the Generator invokes to retrieve objects that implement
	/// IParallelForInterface and ITlsAlloc when needed.
	Generator(Ptr<GeneratorInputProducer> producer, Ptr<IGeneratorGlue> generatorGlue);

	~Generator();

public:
	// ------------------------------ Configuration ------------------------------
	
	/// Sets the Kaim::FileOpenerBase derived class that will be used to open any files. 
	/// This is not mandatory to call this function, an instance of \c DefaultFileOpener will be used automatically.
	void SetFileOpener(FileOpenerBase* fileOpener);

	/// Sets the directory used for the generation.
	/// It is passed as an absolute directory and a relative path.
	/// The relative path will be embedded in the generated data, 
	/// so when visual debugging all data can be found by providing absoluteBaseDirectory to the NavigationLab.
	/// \param absoluteOutputBaseDir An absolute path used as a base path for relativeOutputDir
	/// \param relativeOutputDir A relative path to absoluteOutputBaseDir. The absolute path to this directory is : absoluteOutputBaseDir/relativeOutputDir
	void SetOutputDirectory(const char* absoluteOutputBaseDir, const char* relativeOutputDir);

	// ---------------------------------- Generation ----------------------------------

	/// Launch the generation of the sectors stored in \c config, using the config parameters and runOptions
	/// also defined in this structure. See #GeneratorInputOutput for more information
	/// \return A #KyResult that indicates the success or failure of the request.
	KyResult Generate(GeneratorInputOutput& config);

	// ---------------------------------- Reporting ----------------------------------

	/// Creates a detailed record of all configuration parameters and sector information, and writes the record to the specified stream.
	template <class OSTREAM> void LogConfiguration(OSTREAM& os);

	/// Retrieves a detailed record of the results of the last call to Generate(). 
	const GenerationReport& GetGenerationReport() const { return m_report; }

	/// Get some profiling information about the time taken by the various steps of the generation
	const GeneratorProfiling& GetGeneratorProfiling() const { return m_generatorProfiling; }


	// ---------------------------------- Internal Access Only ----------------------------------

	GeneratorSystem& Sys() { return m_sys; }
	const KyArrayPOD<GeneratorSectorBuilder*>& GetSectorGenerators() const { return m_sectorGenerators; }

private:
	void ClearGeneratedData();
	void ClearSectors();

	KyResult AddSector(Ptr<GeneratorSector> sector);
	KyResult Generate();

	KyResult CreateSectorGenerators();
	GeneratorSectorBuilder* CreateSectorGenerator(Ptr<GeneratorSector> sector);
	GeneratorSectorBuilder* CreateSectorGeneratorFromClientInput(Ptr<GeneratorSector> sector);

	void Init(Ptr<IParallelForInterface> parallelFor, Ptr<ITlsAlloc> tlsAlloc);
	KyResult GenerateInit();
	bool CheckConfig() const;
	bool AreGuidsAndNamesUnique(bool pdgInputLoaded) const;
	bool IsGlobalConfigConsistent() const;
	KyResult LoadOrProduceInputs();
	KyResult MakeGlobalGrid();
	KyResult MakeNavDataElements();
	KyResult GenerateSectors();
	enum FindInputCellMode { KEEP_EXPLICIT_OVERLAP_CELLS = 0, IGNORE_EXPLICIT_OVERLAP_CELLS, };
	void FindMainContributingInputCells(KyUInt32 mainIdx, const PdgInputCellsAtPos* cellsAtPos, KyArray<PdgInputCellInfo>& inputCells, FindInputCellMode mode);
	KyResult FeedInputCells(const KyArray<PdgInputCellInfo>& inputCells, const CellPos& cellPos, GeneratorNavDataElement* navDataElement, bool isOverlapCell);
	bool IsGuidCompoundExclusive(const GeneratorGuidCompound& guidCompound) const;
	bool IsGuidCompoundAnExplicitOverlap(const GeneratorGuidCompound& guidCompound) const;
	KyResult SaveGeneratorInputOutput(const GeneratorInputOutput& generatorInputOutput);
	void UpdateSectorOverlapStatus();

private:
	Ptr<GeneratorInputProducer> m_producer;
	GeneratorSystem m_sys;
	DefaultFileOpener m_defaultFileOpener;
	KyArrayPOD<GeneratorSectorBuilder*> m_sectorGenerators;
	GeneratorSectorList m_sectorList;
	GlobalCellGrid m_globalGrid;
	KyArray<GeneratorGuidCompound> m_exclusiveGuids;
	KyArray<GeneratorGuidCompound> m_explicitOverlapSectors;
	KyUInt32 m_timeStamp;
	GenerationReport m_report;
	GeneratorProfiling m_generatorProfiling;
};


template <class OSTREAM>
inline void Generator::LogConfiguration(OSTREAM& os)
{
	os << KY_LOG_BIG_TITLE_BEGIN("", "Generator Configuration Info");

	os << m_sys;
	os << m_sys.m_genParams;
	os << m_sys.m_runOptions;

	for (KyUInt32 sectorIdx = 0; sectorIdx < m_sectorGenerators.GetCount(); ++sectorIdx)
	{
		os << KY_LOG_SMALL_TITLE_BEGIN("", "Sector " << sectorIdx <<" Info");
		m_sectorGenerators[sectorIdx]->LogConfiguration(os);
		os << KY_LOG_SMALL_TITLE_END("", "Sector " << sectorIdx <<" Info");
	}

	os << KY_LOG_BIG_TITLE_END("", "Generator Configuration Info");
}


}


#endif
