/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: GUAL - secondary contact: LASI
#ifndef GwNavGen_GeneratorSector_H
#define GwNavGen_GeneratorSector_H

#include "gwnavruntime/base/kyguid.h"
#include "gwnavgeneration/common/generatorfilesystem.h"
#include "gwnavruntime/navdata/navdata.h"
#include "gwnavruntime/base/endianness.h"
#include "gwnavruntime/basesystem/coordsystem.h"
#include "gwnavgeneration/input/clientinputtagvolume.h"


namespace Kaim
{

class GeneratorSectorConfigBlob;
class GeneratorFileSystem;
class BlobFieldsMapping;

namespace GenFlags
{
	/// Describes where the Generator will get its input for each sector
	enum SectorInputSource
	{
		SECTOR_INPUT_FROM_PRODUCER,     ///< (default) Instruct the Generator to call the GeneratorInputProducer to get the input of a sector
		SECTOR_INPUT_FROM_CLIENT_INPUT  ///< Instruct the Generator to retrieve the input of a sector from its ClientInput file
	};

	/// Describes whether or not a particular should be rebuilt
	enum SectorBuildMode
	{
		SECTOR_BUILD,       ///< (default) The sector will be built even if a NavData was previously generated for it. 
		SECTOR_NO_REBUILD,  /// The generator will load the input for this sector but will not try to generate the NavData associated. Can be Used for Iterative build. 
	};
	
	/// Describes whether or not a ClientInput should be saved on a per-sector basis
	enum SectorClientInputSaveMode
	{
		SECTOR_CLIENT_INPUT_SAVE_ENABLED,   ///< (default) The client Input will be saved for this sector
		SECTOR_CLIENT_INPUT_SAVE_DISABLED,  ///< The client Input will not be saved for this sector
	};

	/// Filter controls what's saved and loaded from the serialized blobAggregate that
	/// stores the information of a GeneratorInputOutput.
	enum GenIoFilter
	{
		FILTER_INPUT_CONFIG_ONLY = 0, ///< Save/Load/Copy only the input config of the generation
		FILTER_ALL               = 1  ///< Save/Load/Copy only the input config, the options, the outputs
	};
}


/// The GeneratorSectorConfig class contains all the relevant information for a Generation sector.
/// It contains the GUID and name of the sector as well as some optional information such as the
/// OutputFilename for the associated NavData and the InputFilenames of the files that might be needed
/// by the producer to load the geometry of that sector.
class GeneratorSectorConfig
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	/// Constructs a new instance of this class.
	/// \param sectorGuid			A globally unique ID for this sector.
	/// \param sectorName			A descriptive name for this sector. If #KY_NULL, it will be generated automatically
	/// 							from the \c sectorGuid.
	GeneratorSectorConfig(KyGuid sectorGuid, const char* sectorName)
		: m_sectorGuid(sectorGuid) , m_sectorName(sectorName)
	{}

	/// Read a GeneratorSectorConfig from a GeneratorSectorConfigBlob
	void ReadFromBlob(const GeneratorSectorConfigBlob& blob);

	const char* GetName() const { return m_sectorName.ToCStr(); } ///< Accessor to m_sectorName as char*
	const String GetNameString() const { return m_sectorName; }   ///< Accessor to m_sectorName as String

	const KyGuid& GetGuid() { return m_sectorGuid; }          ///< Gets m_sectorGuid
	void SetGuid(const KyGuid& guid) { m_sectorGuid = guid; } ///< Sets m_sectorGuid

	/// Set the filename of the output data (Client Input / NavData) for this sector 
	/// - No file extension needed, it will be added automatically.
	/// 1. If filename path is absolute, NavData will be saved at the specified location, provided you have right access
	/// 2. If filename path is relative, the Generator RootDir will be added as a prefix
	/// Use GetAbsoluteOutputFilename with the Generator output dir as parameter to get the absolute path for this sector
	/// \param filename The filename for the output NavData file
	//void SetOutputFilename(const char* filename) { m_outputFilename = String(filename); }

	/// Returns a string that represent the filename, without extension, of all the output files 
	/// associated with the sectors. m_sectorName is used unless m_outputFilename was set. 
	//const char* GetOutputFileName() const { return m_outputFilename == "" ? m_sectorName.ToCStr() : m_outputFilename.ToCStr(); }

	/// 
	String GetAbsoluteOutputFileName(const GeneratorFileSystem& fileSystem, const String& extension) const;
	String GetRelativeOutputFileName(const GeneratorFileSystem& fileSystem, const String& extension) const;

	/// Add InputFilenames to help the GeneratorInputProducer locate the data it needs to describe this sector. 
	/// InputFiles are optionnal but are used for example with the OBJProducer to locate the OBJ file containing 
	/// the geometry of the sector. 
	/// \param filename   The filename of the input file to add. 	
	void AddInputFileName(const char* filename) { m_inputFilenames.PushBack(String(filename)); }

	/// Accessor to the list of m_inputFilenames
	const KyArray<String>& GetInputFileNames() const { return m_inputFilenames; }

	/// Add SeedPoint
	void AddSeedPoint(const Vec3f& position) { m_seedPoints.PushBack(position); }

	/// Add SeedPoint in client coordinates
	void AddSeedPointInClientCoordinates(const Vec3f& client_position, const CoordSystem& coordSystem) { AddSeedPoint(coordSystem.ClientToNavigation_Pos(client_position)); }

	/// Add AddTagVolume
	void AddTagVolume(const ClientInputTagVolume& inputTagVolume) { m_inputTagVolumes.PushBack(inputTagVolume); }

	/// Get SeedPoints
	const KyArray<Vec3f>& GetSeedPoints() const { return m_seedPoints; }

	/// Get TagVolumes
	const KyArray<ClientInputTagVolume>& GetTagVolumes() const { return m_inputTagVolumes; }

	/// Limits the generation to the inputCellBox
	void SetInputCellBox(const CellBox& inputCellBox) { m_inputCellBox = inputCellBox; }

	/// Get the InputCellBox
	const CellBox& GetInputCellBox() const { return m_inputCellBox; }

	/// Determines whether or not a cellBox was set for this sector
	bool IsInputCellBoxSet() { return m_inputCellBox.Min() < m_inputCellBox.Max(); }

public:
	/// The Guid for this sector.
	KyGuid m_sectorGuid; 

	/// The name of the sector. Can be used to retrieve the GeneratorSector from GeneratorInputOutput 
	/// or to ID a particular sector in your implementation of the GeneratorInputProducer. 
	String m_sectorName;

	/// Optional. Can be used to specify an outputFile for the .NavData and .ClientInput files associated with this sector. 
	/// By default, the outputFilename is build using the m_sectorName and accessed using GetOutputFilename()
	/// m_outputFilename should be provided with no file extension.
	//String m_outputFilename;

	/// Optional. A set of filename that the producer can use to access the relevant data during input consumption. 
	/// Typically, file-based producers such as OBJProducer will use this list. 
	KyArray<String> m_inputFilenames;

	/// in Navigation coordinates
	KyArray<Vec3f> m_seedPoints;

	/// in Navigation coordinates
	KyArray<ClientInputTagVolume> m_inputTagVolumes;

	/// Optional, The cellBox of this sector. See GeneratorSectorConfig::SetInputCellBox for more information.
	CellBox m_inputCellBox;

public: // internal
	void AddMapping(BlobFieldsMapping& mapping);
};

/// The GeneratorSector class provides a definition for a single sector to be treated by the Generator.
/// It contains the GeneratorSectorConfig that was used to build it along with sector-specific flags
/// and the navdata that has been generated once a Generation is done. (Accesible via GetNavData())
/// Typically a GeneratorSector can be obtained from a GeneratorInputOutput instance using AddSector() or 
/// one of the GetSector() method. 
class GeneratorSector : public RefCountBase<GeneratorSector, MemStat_NavDataGen>
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_CLASS_WITHOUT_COPY(GeneratorSector)

friend class Generator;
friend class GeneratorSectorBuilder;
friend class GeneratorSectorConfigBlobBuilder;

public:
	GeneratorSector() 
		: m_config(KyGuid::GetDefaultGuid(), "DefaultName")
		, m_inputSource(GenFlags::SECTOR_INPUT_FROM_PRODUCER)
		, m_buildMode(GenFlags::SECTOR_BUILD)
		, m_clientInputSaveMode(GenFlags::SECTOR_CLIENT_INPUT_SAVE_ENABLED)
		, m_userData(KY_NULL)
	{}
	
	GeneratorSector(const GeneratorSectorConfig& config)
		: m_config(config)
		, m_inputSource(GenFlags::SECTOR_INPUT_FROM_PRODUCER)
		, m_buildMode(GenFlags::SECTOR_BUILD)
		, m_clientInputSaveMode(GenFlags::SECTOR_CLIENT_INPUT_SAVE_ENABLED)
		, m_userData(KY_NULL)
	{}

	const char* GetName() const;       ///< Retrieves the name set for this sector as char*.
	const String& GetNameString() const; ///< Retrieves the name set for this sector as String.

	const KyGuid& GetGuid() const; ///< Retrieves the GUID set for this sector.
	String GetGuidString(); ///< Retrieves a string representing the GUID set for this sector.

	GeneratorSectorConfig& GetConfig(); ///< Accessor to the GeneratorSectorConfig for this sector
	const GeneratorSectorConfig& GetConfig() const; ///< Const Accessor to the GeneratorSectorConfig for this sector

	void* GetUserData() { return m_userData; }                  ///< Gets m_userData
	const void* GetUserData() const { return m_userData; }      ///< Gets m_userData
	void SetUserData(void* userData) { m_userData = userData; } ///< Sets m_userData

	String GetAbsoluteOutputFileName(const GeneratorFileSystem& fileSystem, const String& extension) const;
	String GetRelativeOutputFileName(const GeneratorFileSystem& fileSystem, const String& extension) const;

	void ToggleClientInputSave(bool enableSave); ///< Write accessor for #m_clientInputSaveMode.
	bool IsClientInputSaveEnabled() const;       ///< Read accessor for #m_clientInputSaveMode.

	void SetBuildMode(GenFlags::SectorBuildMode mode); ///< Write accessor for #m_buildMode.
	GenFlags::SectorBuildMode GetBuildMode() const; ///< Read accessor for #m_buildMode. 

	void SetInputSource(GenFlags::SectorInputSource source); ///< Write accessor for #m_inputSource.
	GenFlags::SectorInputSource GetInputSource() const; ///< Read accessor for #m_inputSource. 

	void SetClientInputSaveMode(GenFlags::SectorClientInputSaveMode clientinputMode); ///< Write accessor for #m_clientInputSaveMode. 
	GenFlags::SectorClientInputSaveMode GetClientInputSaveMode() const; /// Read accessor for #m_clientInputSaveMode. 

	bool CompareInputConfig(const GeneratorSector& other); ///< Compare m_config.m_sectorGuid, m_config.m_sectorName and m_config.m_inputCellBox
	void CopyFrom(const GeneratorSector& other, GenFlags::GenIoFilter filter); ///< Copy 

	/// Can be used once a sector has been generated and a NavData file saved to postProcess this file.
	const String& GetClientInputAbsoluteFileName() const;
	const String& GetNavDataAbsoluteFileName() const;

	/// Retrieve a pointer to the NavData that was generated for this sector, or #KY_NULL if the navdata is not available
	/// or has not been generated yet.
	/// The NavData provided is not bound to any Kaim::Database or Kaim::World and needs to be added manually in order to have access 
	/// to the stitched runtime data
	Ptr<NavData> GetNavData();

	/// If NavData was generated, save it to the specified location
	KyResult SaveNavData(const char* filename, FileOpenerBase* fileOpener = KY_NULL, Endianness::Type endianness = Endianness::BigEndian);

	/// Read a GeneratorSectorConfig from a GeneratorSectorConfigBlob
	void ReadFromBlob(const GeneratorSectorConfigBlob& blob, GenFlags::GenIoFilter filter);

	void AddMappingForOptionsAndOutputFields(BlobFieldsMapping& mapping);

private:
	GeneratorSectorConfig m_config;
	KyUInt32 m_inputSource;         /// takes values from GenerationFlags::SectorInputSource;
	KyUInt32 m_buildMode;           /// takes values from GenerationFlags::SectorBuildMode m_buildMode;
	KyUInt32 m_clientInputSaveMode; /// takes values from GenerationFlags::SectorClientInputSaveMode
	String m_clientInputAbsoluteFileName; ///< Set by the Generator when the ClientInput is saved
	String m_navDataAbsoluteFileName;     ///< Set by the Generator when the NavData is saved
	Ptr<NavData> m_navData;               ///< Will be accessible when generation is done through GetNavData(). #KY_NULL until then.
	void* m_userData;                     ///< Allows to point to your own data
};


inline const char* GeneratorSector::GetName() const { return m_config.m_sectorName.ToCStr(); }
inline const String& GeneratorSector::GetNameString() const { return m_config.m_sectorName; }

inline const String& GeneratorSector::GetClientInputAbsoluteFileName() const  { return m_clientInputAbsoluteFileName; }
inline const String& GeneratorSector::GetNavDataAbsoluteFileName() const      { return m_navDataAbsoluteFileName; }

inline const KyGuid& GeneratorSector::GetGuid() const { return m_config.m_sectorGuid; } 

inline String GeneratorSector::GetGuidString()
{
	char guidChars[37];
	m_config.m_sectorGuid.ToString(guidChars);
	return String(guidChars);
}

inline       GeneratorSectorConfig& GeneratorSector::GetConfig() { return m_config; }
inline const GeneratorSectorConfig& GeneratorSector::GetConfig() const { return m_config; }

inline Kaim::String GeneratorSector::GetAbsoluteOutputFileName(const GeneratorFileSystem& fileSystem, const String& extension) const
{
	return m_config.GetAbsoluteOutputFileName(fileSystem, extension);
}

inline Kaim::String GeneratorSector::GetRelativeOutputFileName(const GeneratorFileSystem& fileSystem, const String& extension) const
{
	return m_config.GetRelativeOutputFileName(fileSystem, extension);
}

inline void GeneratorSector::ToggleClientInputSave(bool enableSave)
{
	m_clientInputSaveMode = enableSave ? GenFlags::SECTOR_CLIENT_INPUT_SAVE_ENABLED : GenFlags::SECTOR_CLIENT_INPUT_SAVE_DISABLED;
}
inline bool GeneratorSector::IsClientInputSaveEnabled() const { return m_clientInputSaveMode == GenFlags::SECTOR_CLIENT_INPUT_SAVE_ENABLED; }

inline void GeneratorSector::SetBuildMode(GenFlags::SectorBuildMode mode) { m_buildMode = mode; }
inline GenFlags::SectorBuildMode GeneratorSector::GetBuildMode() const { return (GenFlags::SectorBuildMode)m_buildMode; }

inline void GeneratorSector::SetInputSource(GenFlags::SectorInputSource source) { m_inputSource = source; }
inline GenFlags::SectorInputSource GeneratorSector::GetInputSource() const { return (GenFlags::SectorInputSource)m_inputSource; }

inline void GeneratorSector::SetClientInputSaveMode(GenFlags::SectorClientInputSaveMode clientinputMode) { m_clientInputSaveMode = clientinputMode; }
inline GenFlags::SectorClientInputSaveMode GeneratorSector::GetClientInputSaveMode() const { return (GenFlags::SectorClientInputSaveMode)m_clientInputSaveMode; }

}


#endif

