/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LASI - secondary contact: GUAL
#ifndef GwNavGen_GeneratorConfig_H
#define GwNavGen_GeneratorConfig_H

#include "gwnavgeneration/generator/generatorsectorlist.h"
#include "gwnavgeneration/generator/generatorparameters.h"
#include "gwnavgeneration/generator/generatorrunoptions.h"
#include "gwnavgeneration/generator/generatorguidcompound.h"
#include "gwnavruntime/base/endianness.h"
#include "gwnavruntime/base/types.h"
#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/kernel/SF_String.h"
#include "gwnavruntime/base/fileopener.h"
#include "gwnavruntime/navdata/navdataindex.h"
#include "gwnavruntime/basesystem/coordsystem.h"

namespace Kaim
{

/// The GeneratorInputOutput class is a central element of the generation. 
/// Its purpose is to setup a generation as well as store the data that have been produced.
/// It can be saved, loaded and modified to enable iterative rebuilds. 
class GeneratorInputOutput
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_CLASS_WITHOUT_COPY(GeneratorInputOutput)

public:
	GeneratorInputOutput() {}

	// ---------------------------------- Save, Load, Copy, Clear Functions ----------------------------------

	/// Save the GeneratorInputOutput in a file.
	/// \param fullFileName    The filename where to save the GeneratorInputOutput.
	/// \param mask            A mask formed with the values defined in #ConfigBlobMask to specify what is saved in the
	///                         BlobAggregate. Default mask is #GENERATOR_IO_CONFIG which suffice in most cases.
	/// \param fileOpener      A custom file opener to access files. By default, if no fileOpener is set, the DefaultFileOpener will be used.
	KyResult Save(const char* fullFileName, GenFlags::GenIoFilter filter = GenFlags::FILTER_INPUT_CONFIG_ONLY, FileOpenerBase* fileOpener = KY_NULL) const;
	
	/// Load the GeneratorInputOutput from a file.
	/// \param fullFileName    The filename from where to load the GeneratorInputOutput.
	/// \param mask            A mask formed with the values defined in #ConfigBlobMask to specify what has to be loaded
	///                         from the BlobAggregate. Default mask is #GENERATOR_IO_CONFIG which suffice in most cases.
	/// \param fileOpener      A custom file opener to access files. By default, if no fileOpener is set, the DefaultFileOpener will be used.
	KyResult Load(const char* fullFileName, GenFlags::GenIoFilter filter = GenFlags::FILTER_INPUT_CONFIG_ONLY, FileOpenerBase* fileOpener = KY_NULL);

	/// CopyFrom other with a filter
	void CopyFrom(const GeneratorInputOutput& other, GenFlags::GenIoFilter filter = GenFlags::FILTER_ALL);

	/// Clears everything (sectorList, runOptions, parameters, exclusive and overlap GUIDS)
	void Clear();


	// ---------------------------------- Add Functions ----------------------------------

	/// Use this function to Add new GeneratorSector to the list stored in the GeneratorInputOutput.
	/// This function checks if a sector with the same name or GUID was already added before
	/// and returns an empty Ptr in that case.
	/// When successful it returns a ref-counted Ptr to the sector that can be used, after a generation,
	/// to retrieve the NavData or change some build flags.
	/// \param config                 An instance of GeneratorSectorConfig that contains all the useful parameters related to the added sector.
	/// \param inputSource            Specify if the sector should be loaded using the producer or a ClientInput file.
	/// \param buildMode              Defines whether or not this particular sector should be rebuilt.
	/// \param clientInputSaveMode    Specify if the ClientInputFile should be saved for this sector.
	Ptr<GeneratorSector> AddSector(const GeneratorSectorConfig& config,
		GenFlags::SectorInputSource inputSource = GenFlags::SECTOR_INPUT_FROM_PRODUCER,
		GenFlags::SectorBuildMode buildMode = GenFlags::SECTOR_BUILD,
		GenFlags::SectorClientInputSaveMode clientInputSaveMode = GenFlags::SECTOR_CLIENT_INPUT_SAVE_ENABLED);

	/// Specifies the GUIDs of sectors that cover the same areas of terrain, but that will never be loaded into the same Database
	/// at the same time. This prevents the NavData generation system from generating alternative data in the area of overlap.
	/// This is primarily intended to support swappable sectors, which reflect multiple versions of the terrain in a single area --
	/// for example, one version of a sector that contains a building, and another version of a sector in which the building has
	/// been destroyed.
	/// You can still use swappable sectors without calling this method. However, as the alternative data generated in the area
	/// of overlap between the sectors will never be used, you can reduce the size of your NavData by preventing the NavData generation
	/// system from generating the data in the first place.
	/// \param guids         An array of GUIDs that identify the sectors that will never be loaded into the same Database at the
	///                      same time.
	/// \param guidsCount    The number of elements in the guids array. 
	void AddExclusiveGuids(const KyGuid* guids, KyUInt32 guidsCount);
	void AddExclusiveGuids(const GeneratorGuidCompound& generatorGuidCompound);
	
	/// Specifies the GUIDs of sectors that partially cover the same areas of terrain, and that will require overlap NavData to be generated
	/// even if an inputCellBox was associated for this sector. This effectively forces "normal" overlap generation and overrides the
	/// GeneratorSectorConfig::m_inputCellBox.
	/// \param guids         An array of GUIDs that identify the sectors that overlap even though they have a GeneratorSectorConfig::m_inputCellBox.
	/// \param guidsCount    The number of elements in the guids array.
	void AddExplicitOverlapSectors(const KyGuid* guids, KyUInt32 guidsCount);
	void AddExplicitOverlapSectors(const GeneratorGuidCompound& generatorGuidCompound);


	// ---------------------------------- Accessors ----------------------------------

	Ptr<GeneratorSector> GetSectorWithIndex(KyUInt32 index);     ///< \return A ref-counted Ptr on the matching GeneratorSector, if any, or an empty Ptr if none has been found.
	Ptr<GeneratorSector> GetSectorWithName(const char* name);    ///< \copydoc GetSectorWithIndex(KyUInt32 index)
	Ptr<GeneratorSector> GetSectorWithGuid(const KyGuid& guid);  ///< \copydoc GetSectorWithIndex(KyUInt32 index)

	const Ptr<GeneratorSector> GetSectorWithIndex(KyUInt32 index) const;     ///< \copydoc GetSectorWithIndex(KyUInt32 index)
	const Ptr<GeneratorSector> GetSectorWithName(const char* name) const;    ///< \copydoc GetSectorWithIndex(KyUInt32 index)
	const Ptr<GeneratorSector> GetSectorWithGuid(const KyGuid& guid) const;  ///< \copydoc GetSectorWithIndex(KyUInt32 index)


public:
	GeneratorParameters m_params; //In
	GeneratorRunOptions m_runOptions; //In
	KyArray<GeneratorGuidCompound> m_exclusiveGuids; //In
	KyArray<GeneratorGuidCompound> m_explicitOverlapSectors; //In
	GeneratorSectorList m_sectors; // In/Out
	
	// Out. The info contained in m_navDataIndexHandler redundant with m_sectors
	// but having a NavDataIndex Blob in GenIO files allows to load them in a pure runtime environment
	// that does link with gwnavgeneration.lib
	Ptr< BlobHandler<NavDataIndex> > m_navDataIndexHandler;
};


}


#endif
