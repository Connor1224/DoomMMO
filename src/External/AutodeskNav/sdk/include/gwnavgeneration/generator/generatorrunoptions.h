/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: LASI
#ifndef GwNavGen_GeneratorRunOptions_H
#define GwNavGen_GeneratorRunOptions_H


#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/containers/kyarray.h"
#include "gwnavgeneration/generator/generatorvisualdebugoptions.h"
#include "gwnavruntime/kernel/SF_String.h"
#include "gwnavruntime/blob/blobfieldarray.h"
#include "gwnavgeneration/generator/generatorblobtyperegistry.h"


namespace Kaim
{

class BlobAggregate;
/// Enumerates the different places that an intermediate data file can be placed.  
enum InCellSubDir
{
	IN_CELL_SUB_DIR, ///< Indicates that the data file should be placed in a sub-directory for data within that cell. 
	NOT_IN_CELL_SUB_DIR ///< Indicates that the data file should be placed outside of the cell sub-directory. 
};

class GeneratorRunOptionsBlob
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_ROOT_BLOB_CLASS(Generator, GeneratorRunOptionsBlob, 0)
	KY_CLASS_WITHOUT_COPY(GeneratorRunOptionsBlob)
public:
	GeneratorRunOptionsBlob() {}
	BlobFieldArray m_fields;
};
inline void SwapEndianness(Endianness::Target e, GeneratorRunOptionsBlob& self)
{
	SwapEndianness(e, self.m_fields);
}

/// The IntermediateFileConfig class is used by IntermediateFilesConfig to store configuration parameters
/// that control whether or not the Generator writes a single specific type of intermediate data to files on disk,
/// where, and how.
class IntermediateFileConfig
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	/// Constructs a new instance of this class.
	/// \param directory			Sets the value of #m_directory.
	/// \param inCellSubDir			Sets the value of #m_inCellSubDir.
	/// \param extension			Sets the value of #m_extension. 
	IntermediateFileConfig(const String& directory, InCellSubDir inCellSubDir, const String& extension);

	/// Write accessor for #m_doWrite. 
	bool DoWrite() const { return m_doWrite; }

	/// Returns the actual file name that should be used for the file that corresponds to the \c cellPos cell. 
	String FileName(const CellPos& cellPos) const;

private:
	static String GetCellFileName(const char* containingDirectory, const CellPos& cellPos, const char* extension, bool inCellSubDir);

public:
	/// Determines whether or not this type of intermediate data should be written to files on disk.
	/// \defaultvalue		\c false 
	bool m_doWrite;

	/// The directory in which the files for this type of intermediate data will be written, relative to the
	/// root directory set for the Generator. 
	String m_directory;

	/// Determines whether or not the files for this type of intermediate data will be divided into 
	/// different subdirectories for different cells.
	/// \defaultvalue	#NOT_IN_CELL_SUB_DIR 
	InCellSubDir m_inCellSubDir;

	/// Determines the extension to use for the files generated for this type of intermediate data. 
	String m_extension;
};

class IntermediateCellsConfig
{
public:
	typedef KyArray<CellPos>::ConstIterator CellPosConstIterator;
public:
	IntermediateCellsConfig() : m_writeAll(false) {}
	void ToggleWriteAll(bool doWriteAll) { m_writeAll = doWriteAll; }
	bool HasCellsToWrite() const { return m_writeAll || !IsEmpty(); }
	bool IsEmpty() const;
	bool ShouldBuild(const CellPos& pos) const;
	void AddCellToBuild(const CellPos& pos);
	void Clear();
	CellPosConstIterator CellPosBegin() const { return m_cellsToBuild.Begin(); }
	CellPosConstIterator CellPosEnd() const { return m_cellsToBuild.End(); }
	KyUInt32 GetCellsToBuildCount() const { return m_cellsToBuild.GetCount(); }
	const CellPos& GetCell(KyUInt32 index) const { return m_cellsToBuild[index]; }
private:
	bool Contain(const CellPos& pos) const;
private:
	/* Generate intermediate data for these cells. */
	KyArray<CellPos> m_cellsToBuild;

	/* Overrides m_cellsToBuild when set to true and make ShouldBuild() always return true */
	bool m_writeAll;
};

/// The IntermediateFilesConfig class is used by GeneratorRunOptions to store configuration parameters that control
/// whether or not the Generator writes different types of intermediate data to files on disk, where, and how.
class IntermediateFilesConfig
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	/// Constructs a new instance of this class. 
	IntermediateFilesConfig();

	/// Const read accessor for #m_rasters. 
	const IntermediateFileConfig& Rasters() const { return m_rasters; }

	/// Const read accessor for #m_navRasters. 
	const IntermediateFileConfig& NavRasters() const { return m_navRasters; }

	/// Const read accessor for #m_boundaryGraphs. 
	const IntermediateFileConfig& BoundaryGraphs() const { return m_boundaryGraphs; }

	/// Const read accessor for #m_navCells. 
	const IntermediateFileConfig& NavCells() const { return m_navCells; }

	/// Read accessor for #m_triangulation. 
	const IntermediateFileConfig& Triangulation() const { return m_triangulation; }

	/// Const read accessor for #m_navGraphs. 
	const IntermediateFileConfig& NavGraphs() const { return m_navGraphs; }

	/// Read accessor for #m_rasters. 
	IntermediateFileConfig& Rasters() { return m_rasters; }

	/// Read accessor for #m_navRasters. 
	IntermediateFileConfig& NavRasters() { return m_navRasters; }

	/// Read accessor for #m_boundaryGraphs. 
	IntermediateFileConfig& BoundaryGraphs() { return m_boundaryGraphs; }

	/// Read accessor for #m_navCells. 
	IntermediateFileConfig& NavCells() { return m_navCells; }

	/// Read accessor for #m_triangulation. 
	IntermediateFileConfig& Triangulation() { return m_triangulation; }

	/// Read accessor for #m_navGraphs. 
	IntermediateFileConfig& NavGraphs() { return m_navGraphs; }

	/// Sets the Generator to write or not write all possible types of intermediate data files.
	/// \param doWrite		\c true makes the Generator write all intermediate data files, \c false disables
	/// 					the Generator from writing any intermediate files. 
	void SetAllDoWrites(bool doWrite);

	/// Returns true if at least one intermediate file will be created. 
	bool DoWriteSomeIntermediateFile() const;
public:
	/// Contains the configuration data that controls the way the Generator writes raster data to files on disk. 
	IntermediateFileConfig m_rasters;

	/// Contains the configuration data that controls the way the Generator writes navRaster data to files on disk. 
	IntermediateFileConfig m_navRasters;

	/// Contains the configuration data that controls the way the Generator writes boundary graph data to files on disk. 
	IntermediateFileConfig m_boundaryGraphs;

	/// Contains the configuration data that controls the way the Generator writes NavMesh cell data to files on disk. 
	IntermediateFileConfig m_navCells;

	/// Contains the configuration data that controls the way the Generator writes triangulation data to files on disk. 
	IntermediateFileConfig m_triangulation;

	/// Contains the configuration data that controls the way the Generator writes point graph data to files on disk. 
	IntermediateFileConfig m_navGraphs;

	IntermediateCellsConfig m_cellsConfig;
};

/// The GeneratorRunOptions class contains configuration parameters that control the way in which the Generator
/// carries out the process of generating NavData for its sectors. You can create an instance of this class,
/// set up its data members as desired, and pass it in a call to Generator::SetRunOptions().
class GeneratorRunOptions
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:
	enum { Version = 0 };

public:
	/// Constructs a new instance of this class. 
	GeneratorRunOptions() { SetDefaultValues(); }

	/// Clears all information maintained by this object. 
	void SetDefaultValues();

	/// Read accessor for #m_intermediateFiles. 
	const IntermediateFilesConfig& Intermediates() const { return m_intermediateFiles; }

	/// Const read accessor for #m_intermediateFiles. 
	IntermediateFilesConfig& Intermediates() { return m_intermediateFiles; }

	/// Restricts NavData generation to the specified cell, and generates intermediate data for that cell. 
	/// Used to troubleshoot localized problems with NavData generation. 
	void AddCellToBuild(const CellPos& pos) { m_intermediateFiles.m_cellsConfig.AddCellToBuild(pos); }

	/// Indicates whether or not the Generator will actually use parallel processing. When the Generator is configured 
	/// to write intermediate data, multi-core generation is deactivated. 
	bool DoReallyUseMultiCore() const { return m_doMultiCore && !m_intermediateFiles.DoWriteSomeIntermediateFile(); }

	/// For internal use. 
	bool DoReallyUseVisualDebug() const { return !DoReallyUseMultiCore() && m_visualDebugOptions.IsActive(); }

	void BuildBlob(BlobHandler<GeneratorRunOptionsBlob>& handler) const;
	KyResult ReadBlob(const GeneratorRunOptionsBlob& blob);
	KyResult ReadAggregate(BlobAggregate& aggregate);

	void AddMapping(BlobFieldsMapping& mapping);
	void AddMapping(BlobFieldsMapping& mapping) const;

public:
	/// Determines whether or not the Generator saves the GeneratorInputOutput content.
	/// Check the class GeneratorInputOutput.
	/// \defaultvalue \c true
	bool m_doSaveGeneratorInputOutput;

	/// FileName of the saved GeneratorInputOutput
	/// \defaultvalue \c "Generator"
	String m_generatorInputOuputSaveFileName;

	/// Determines whether or not the NavData generation system spreads computations across all available CPUs on your computer.
	/// \defaultvalue \c true
	bool m_doMultiCore;

	/// Determines whether or not areas of overlap between adjacent sectors are treated separately.
	/// Consider two sectors, A and B, that overlap significantly. When the geometry of A or B is loaded into the game by 
	/// itself, the corresponding NavData needs to reflect only the terrain of that sector. However, when both sectors are loaded
	/// into the game at the same time, the NavData in the overlapping area needs to reflect the combination of triangles in
	/// the two sectors. 
	/// When this parameter is set to \c true, the NavData generation system generates an alternate chunk of
	/// NavData for the area of the overlap that represents the combination of the two geometries. This alternate chunk is stored
	/// in either one of the two sectors, and identified by a GUID that is the combination of the GUIDs for both overlapping sectors.
	/// The runtime stitching system automatically detects when both sectors are loaded at the same time, and uses that alternate chunk
	/// for the area of overlap instead of the NavData in each sector that reflects only the geometry of that individual sector.
	/// \defaultvalue \c true 
	bool m_doGenerateOverlaps;

	/// Determines whether or not .NavData files are written.
	/// \defaultvalue \c true 
	bool m_doWriteNavDataFiles;

	/// Determines whether or not the input geometry consumption is buffered to avoid exceeding the memory limitations on 32 bit.
	/// Activating this mode might slightly increase the generation time.
	/// \defaultvalue	\c false 
	bool m_doEnableLimitedMemoryMode;

	/// Determines the endianness of the final output files. This allows you to avoid swapping the byte order of the data when loading
	/// NavData on a machine with a different endianness.
	/// \acceptedvalues Any element from the Endianness::Type enumeration.
	/// \defaultvalue \c Endianness::BigEndian 
	KyUInt32 m_outputFilesEndianness;

	/// Determines whether or not the Generator uses thread local storage when an ITlsAlloc object is provided in its constructor.
	/// \defaultvalue \c true
	bool m_doUseTlsIfAvailable;

	/// Determines whether or not the Generator will profile its CPU performance.
	/// \defaultvalue \c false
	bool m_doProfile;

	/// Determines whether or not the Generator will log its configuration parameters in its report for each
	/// run. See also #m_doLogReport.
	/// \defaultvalue \c false
	bool m_doLogConfiguration;

	/// Determines whether or not the Generator will log a report the results of each run. When this parameter is set to \c true,
	/// you can access the report by calling Generator::GetGenerationReport() after the generation has completed.
	/// \defaultvalue \c true
	bool m_doLogReport;

	/// Determines whether or not the Generator will log verbose details in its report for each run. See also #m_doLogReport.
	/// \defaultvalue \c false
	bool m_doLogReportDetails;

	/// For internal use. Do not modify.
	GeneratorVisualDebugOptions m_visualDebugOptions;

	/// Contains configuration parameters that relate to the generation of intermediate data files. 
	IntermediateFilesConfig m_intermediateFiles;
};

template <class OSTREAM>
inline OSTREAM& operator<<(OSTREAM& os, GeneratorRunOptions& runOptions)
{
	os << KY_LOG_SMALL_TITLE_BEGIN("", "GeneratorRunOptions");

	os << "doSaveGeneratorInputOutput      : " << runOptions.m_doSaveGeneratorInputOutput      << Endl;
	os << "generatorInputOuputSaveFileName : " << runOptions.m_generatorInputOuputSaveFileName << Endl;
	os << "doMultiCore                     : " << runOptions.m_doMultiCore                     << Endl;
	os << "doGenerateOverlaps              : " << runOptions.m_doGenerateOverlaps              << Endl;
	os << "doWriteNavDataFiles             : " << runOptions.m_doWriteNavDataFiles             << Endl;
	os << "doEnableLimitedMemoryMode       : " << runOptions.m_doEnableLimitedMemoryMode       << Endl;
	os << "outputFilesEndianness           : " << runOptions.m_outputFilesEndianness           << Endl;
	os << "doUseTlsIfAvailable             : " << runOptions.m_doUseTlsIfAvailable             << Endl;
	os << "doProfile                       : " << runOptions.m_doProfile                       << Endl;
	os << "doLogConfiguration              : " << runOptions.m_doLogConfiguration              << Endl;
	os << "doLogReport                     : " << runOptions.m_doLogReport                     << Endl;
	os << "doLogReportDetails              : " << runOptions.m_doLogReportDetails              << Endl;

	os << KY_LOG_SMALL_TITLE_END("", "GeneratorRunOptions");

	return os;
}


class GeneratorRunOptionsBlobBuilder : public BaseBlobBuilder<GeneratorRunOptionsBlob>
{
public:
	GeneratorRunOptionsBlobBuilder(const GeneratorRunOptions* runOptions) : m_runOptions(runOptions) {}

private:
	virtual void DoBuild();

	const GeneratorRunOptions* m_runOptions;
};


} // namespace Kaim

#endif // GwNavGen_GeneratorRunOptions_H

