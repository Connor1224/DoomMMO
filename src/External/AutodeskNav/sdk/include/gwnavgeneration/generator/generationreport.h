/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LASI - secondary contact: NOBODY
#ifndef GwNavGen_GenerationReport_H
#define GwNavGen_GenerationReport_H

#include "gwnavruntime/containers/stringstream.h"
#include "gwnavruntime/kernel/SF_String.h"
#include "gwnavruntime/basesystem/logger.h"

namespace Kaim
{


/// The SectorStatistics class encapsulates a detailed set of statistics about a single sector treated by
/// the NavData generation framework. Instances of this class can be retrieved from a GenerationReport 
/// through its GenerationReport::m_sectorStatistics array.
class SectorStatistics
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	/// For internal use: constructs a new SectorStatistic.
	SectorStatistics():
		m_doLogDetails(false),
		m_inputCellsCount(0),
		m_consumedTriangleCount(0),
		m_generatedTotalNavCellsCount(0),
		m_generatedMainNavCellsCount(0),
		m_generatedOverlapNavCellsCount(0),
		m_navdataElementsCount(0),
		m_sectorInputConsumptionDuration(0.0f),
		m_sectorRawNavMeshTime(0.0f),
		m_navdataSizeInBytes(0),
		m_totalNavmeshSizeInBytes(0),
		m_overlapDataSizeInBytes(0),
		m_navDataGenerated(false)
	{
		m_logPrefix = "    ";
	}

public:
	mutable bool m_doLogDetails;
	String m_logPrefix;

	String m_clientInputAbsoluteFileName;
	String m_navDataAbsoluteFileName;

	String m_sectorName; ///< Maintains the name assigned to the sector. 
	KyGuid m_guid; ///< Maintains the unique GUID assigned to the sector. 

	KyUInt32 m_inputCellsCount; ///< Maintains the number of input cells created internally for the input geometry of the sector. 
	KyUInt32 m_consumedTriangleCount; ///< Maintains the number of input triangles consumed in the input geometry of the sector. 
	Box3f m_inputBoundingBox; ///< Maintains the bounding box around the input geometry of the sector. 

	KyUInt32 m_generatedTotalNavCellsCount; ///< Maintains the total number of cells in the NavMesh generated for the sector. 
	KyUInt32 m_generatedMainNavCellsCount; ///< Maintains the total number of unique cells in the NavMesh generated for the sector: i.e. cells that do not overlap the NavData of other sectors. 
	KyUInt32 m_generatedOverlapNavCellsCount; ///< Maintains the total number of overlap cells in the NavMesh generated for the sector: i.e. cells that overlap the NavData of one or more other sectors. 
	KyUInt32 m_navdataElementsCount; ///< Maintains the total number of NavData elements in the sector: i.e. the number of data chunks that are identified by different GUID compounds. 

	KyFloat32 m_sectorInputConsumptionDuration; ///< Maintains the time taken to consume the input geometry for the sector. 
	KyFloat32 m_sectorRawNavMeshTime; ///< Maintains the time taken to generate the NavData for the sector. 
	
	KyUInt32 m_navdataSizeInBytes; ///< Maintains the total size of the NavData generated for the sector. 
	KyUInt32 m_totalNavmeshSizeInBytes; ///< Maintains the size of the NavMeshElement generated for the sector. 
	KyUInt32 m_overlapDataSizeInBytes; ///< Maintains the size of the overlap data generated for the sector: the NavData that overlaps the NavData of one or more other sectors. 

	bool m_navDataGenerated;
};


template <class OSTREAM>
inline OSTREAM& operator<<(OSTREAM& os, const SectorStatistics& stats)
{
	FloatFormatter fmt2f("%.2f");
	const char* prefix = stats.m_logPrefix.ToCStr();

	os << KY_LOG_SMALL_TITLE_BEGIN(prefix, "stat for sector " << stats.m_sectorName);

	os << prefix << "GUID:                        " << stats.m_guid << Endl;
	os << prefix << "Input Consumption time:      " << fmt2f(stats.m_sectorInputConsumptionDuration) << " seconds" << Endl;
	os << prefix << "RawNavMesh Generation Time:  " << fmt2f(stats.m_sectorRawNavMeshTime)           << " seconds" << Endl;
	os << prefix << "InputTriangle Count:         " << stats.m_consumedTriangleCount << Endl;
	os << prefix << "NavData Size:                " << stats.m_navdataSizeInBytes << " bytes" << Endl;

	// ClientInput File
	os << prefix << "ClientInput File: ";
	if (stats.m_clientInputAbsoluteFileName == "")
		os << "--NONE--" << Endl;
	else
		os << stats.m_clientInputAbsoluteFileName << Endl;

	// NavData File
	os << prefix << "NavData File:     ";
	if (stats.m_navDataAbsoluteFileName == "")
		os << "--NONE--" << Endl;
	else
		os << stats.m_navDataAbsoluteFileName << Endl;

	// ---------------------------------------- Details ----------------------------------------
	if (stats.m_doLogDetails)
	{
		const Box3f& box = stats.m_inputBoundingBox;

		KyFloat32 extentX = box.m_max.x - box.m_min.x;
		KyFloat32 extentY = box.m_max.y - box.m_min.y;
		if (extentX > 0.0f && extentY > 0.0f)
		{
			os << prefix << "Input geometry BBOX Min: ("   << fmt2f(box.m_min.x) << "," << fmt2f(box.m_min.y) << "," << fmt2f(box.m_min.z) << ")" << Endl;
			os << prefix << "Input geometry BBOX Max: ("   << fmt2f(box.m_max.x) << "," << fmt2f(box.m_max.y) << "," << fmt2f(box.m_max.z) << ")" << Endl;
			os << prefix << "Input geometry BBOX Extent: " << fmt2f(extentX * 0.0001f) << " Km by " << fmt2f(extentY * 0.0001f) << " Km" << Endl;
		}

		os << prefix << "Nb Input cells:       " << stats.m_inputCellsCount                     << Endl; 
		os << prefix << "Nb NavData cells:     " << stats.m_generatedTotalNavCellsCount         << Endl;
		os << prefix << "Nb Main cells:        " << stats.m_generatedMainNavCellsCount          << Endl;
		os << prefix << "Nb Overlap cells:     " << stats.m_generatedOverlapNavCellsCount       << Endl;
		os << prefix << "Nb NavData Element:   " << stats.m_navdataElementsCount                << Endl;
		os << prefix << "NavMesh size:         " << stats.m_totalNavmeshSizeInBytes << " bytes" << Endl;
		os << prefix << "Overlap size:         " << stats.m_overlapDataSizeInBytes  << " bytes" << Endl;
	}

	os << KY_LOG_SMALL_TITLE_END(prefix, "stat for sector " << stats.m_sectorName);
	
	return os;
}


/// The GenerationReport class encapsulates a detailed report of all activities carried out during a
/// call to the Generator::Generate(GeneratorInputOutput&) method. This report may be useful to you for evaluating the data
/// generated in your level design tool; it may also be useful to help Autodesk middleware Support
/// diagnose problems that occur in your NavData generation.
/// You can retrieve an instance of this class by calling Generator::GetGenerationReport() at any time
/// after a successful NavData generation. You can interact with the retrieved instance programmatically
/// in your application, or write the report to a text string by calling ToString().
class GenerationReport
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:
	/// For internal use: constructs a new GenerationReport.
	GenerationReport() :
		m_doLogTitle(true),
		m_doLogDetails(true),
		m_doLogSectorStatistics(true),
		m_totalGenerationSeconds(0.0f),
		m_inputConsumptionSeconds(0.0f),
		m_rawNavMeshGenerationSeconds(0.0f),
		m_navMeshFilteringSeconds(0.0f),
		m_aggregateNavDataSeconds(0.0f),
		m_allSectorsCount(0),
		m_generatedSectorsCount(0)
	{
		m_logPrefix = "  ";
	}

	void AddSectorStats(const SectorStatistics& sectorStats)
	{
		m_sectorStatistics.PushBack(sectorStats);
		++m_allSectorsCount;
		if (sectorStats.m_navDataGenerated)
			++m_generatedSectorsCount;
	}

public:
	bool m_doLogTitle;
	bool m_doLogDetails;
	bool m_doLogSectorStatistics;
	String m_logPrefix;
	KyFloat32 m_totalGenerationSeconds; ///< Maintains the total time taken by the NavData generation process, in seconds. 
	KyFloat32 m_inputConsumptionSeconds; ///< Maintains the time taken to consume the input geometry, in seconds. 
	KyFloat32 m_rawNavMeshGenerationSeconds; ///< Maintains the time taken to generate the RawNavMesh, in seconds. 
	KyFloat32 m_navMeshFilteringSeconds; ///< Maintains the time taken to smooth and simplify the NavMesh, in seconds. 
	KyFloat32 m_aggregateNavDataSeconds;

	KyUInt32 m_allSectorsCount;       ///< Maintains the total number of sectors set up for the Generator. 
	KyUInt32 m_generatedSectorsCount; ///< Maintains the total number of sectors for which NavData was generated. 

	KyArray<SectorStatistics> m_sectorStatistics; ///< Maintains an array of objects that contain detailed statistics about the sectors for which NavData was generated. 

	String m_absoluteGeneratorInputOuputSaveFileName;
};


template <class OSTREAM>
inline OSTREAM& operator<<(OSTREAM& os, const GenerationReport& report)
{
	FloatFormatter fmt2f("%.2f");
	const char* prefix = report.m_logPrefix.ToCStr();

	if (report.m_doLogTitle)
		os << KY_LOG_BIG_TITLE_BEGIN(prefix, "GENERATION REPORT");

	os << prefix << "Input Consumption time:     " << fmt2f(report.m_inputConsumptionSeconds)     << " seconds" << Endl;
	os << prefix << "RawNavMesh Generation time: " << fmt2f(report.m_rawNavMeshGenerationSeconds) << " seconds" << Endl;
	os << prefix << "NavMesh Filtering time:     " << fmt2f(report.m_navMeshFilteringSeconds)     << " seconds" << Endl;
	os << prefix << "Aggregate NavData time:     " << fmt2f(report.m_aggregateNavDataSeconds)     << " seconds" << Endl;
	os << prefix << "Total Generation time:      " << fmt2f(report.m_totalGenerationSeconds)      << " seconds" << Endl;
	os << prefix << "Generated Sector count:     "    <<       report.m_generatedSectorsCount << Endl;
	os << Endl;

	if (report.m_doLogSectorStatistics)
	{
		for (KyUInt32 i = 0; i < report.m_sectorStatistics.GetCount(); ++i)
		{
			report.m_sectorStatistics[i].m_doLogDetails = report.m_doLogDetails;
			os << report.m_sectorStatistics[i];
		}
	}

	if (report.m_absoluteGeneratorInputOuputSaveFileName != "")
	{
		os << "Generated GenIO File" << Endl;
		os << report.m_absoluteGeneratorInputOuputSaveFileName << Endl;
	}

	if (report.m_doLogTitle)
		os << KY_LOG_BIG_TITLE_END(prefix, "GENERATION REPORT");

	return os;
}

}


#endif

