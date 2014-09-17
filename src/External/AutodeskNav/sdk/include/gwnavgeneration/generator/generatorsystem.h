/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: LASI
#ifndef GwNavGen_GeneratorSystem_H
#define GwNavGen_GeneratorSystem_H


#include "gwnavruntime/basesystem/basesystem.h"
#include "gwnavruntime/basesystem/coordsystem.h"
#include "gwnavruntime/basesystem/intcoordsystem.h"
#include "gwnavruntime/blob/blobhandler.h"
#include "gwnavruntime/kernel/SF_File.h"
#include "gwnavruntime/navmesh/squarepartition.h"
#include "gwnavgeneration/common/pathmap.h"
#include "gwnavgeneration/common/generatorfile.h"
#include "gwnavgeneration/common/generatorfilesystem.h"
#include "gwnavgeneration/common/generatormemory.h"
#include "gwnavgeneration/generator/iparallelforinterface.h"
#include "gwnavgeneration/generator/navdataelementglobalmap.h"
#include "gwnavgeneration/generator/generatorparameters.h"
#include "gwnavgeneration/generator/generatorrunoptions.h"




namespace Kaim
{

class GeneratorSectorConfig;
class BlobCategory;
class BlobAggregate;
class GeneratorProfiling;
class BaseVisualDebugServer;
class BaseVisualDebugManager;
class VisualDebugServer;
class FileOpenerBase;

/* GeneratorSystem gathers everything that is global across a generation: configuration, options... */
class GeneratorSystem
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	enum GlobalConfigSource { GLOBAL_CONFIG_DEFAULT, GLOBAL_CONFIG_FROM_USER, GLOBAL_CONFIG_FROM_DATA, GlobalConfigSource_Force32 = 0xFFFFFFFF };
	
public:
	GeneratorSystem();
	~GeneratorSystem();

	KyResult SetGeneratorParameters(const GeneratorParameters& globalConfig, GlobalConfigSource globalConfigSource);

	const GeneratorParameters& Config() const { return m_genParams; }
	GeneratorParameters& Config()             { return m_genParams; }

	const GeneratorRunOptions& RunOptions() const { return m_runOptions; }
	GeneratorRunOptions& RunOptions()             { return m_runOptions; }

	const GeneratorFileSystem& GetFileSystem() const { return m_fileSystem; }
	GeneratorFileSystem& GetFileSystem()             { return m_fileSystem; }

	String GetAbsoluteFileName(const String& relativeFileName) const { return GetFileSystem().GetAbsoluteFileName(relativeFileName); }

	const SquarePartition& Partition() const { return m_partition; }
	SquarePartition& Partition()             { return m_partition; }

	KyFloat32 GetActualCellSizeInMeter() const { return m_partition.m_nbPixelsOnCellSide * m_genParams.GetNormalizedRasterPrecision(); }

	/* Load Blob from fileName */
	void* LoadBlobBase(const char* fileName, BaseBlobHandler& blobHandler);

	template<class T>
	T* LoadBlob(const char* fileName, BlobHandler<T>& blobHandler) { return (T*)LoadBlobBase(fileName, blobHandler); }

	/* Save Blob to fileName */
	KyResult SaveBlob(const char* fileName, const BaseBlobHandler& blobHandler, Endianness::Type endianness = Endianness::BigEndian);

	/* Load BlobAggregate from fileName */
	KyResult LoadAggregate(const char* fileName, BlobAggregate& aggregate);

	/* Save Blob to fileName */
	KyResult SaveAggregate(const char* fileName, BlobAggregate& aggregate, Endianness::Type endianness = Endianness::BigEndian);


	// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
	// BEGIN REMOVE THIS FROM GeneratorSystem

	// previously in CoordSystem
	KyFloat32 GetMaxEdgeLengthOfTriangle(const Vec3f& A, const Vec3f& B, const Vec3f& C) const
	{
		const Vec3f AB = B - A;
		const Vec3f BC = C - B;
		const Vec3f CA = A - C;
		return Kaim::Max(AB.GetLength(), Kaim::Max(BC.GetLength(), CA.GetLength()));
	}

	bool IsTriangleTooBig(const Vec3f& A, const Vec3f& B, const Vec3f& C) const
	{
		const KyFloat32 maxLength = IntCoordSystem::IntToNavigation_Dist(KyInt32(sqrtf(KyFloat32(KyInt32MAXVAL))) - 1, m_genParams.GetNormalizedRasterPrecision());
		return GetMaxEdgeLengthOfTriangle(A, B, C) >= maxLength;
	}

	bool IsCoordinateValid(const Vec3f& pos)
	{
		KyFloat32 navigationCoordMax = IntToNavigation_Dist(KyInt32MAXVAL - 1); // inferior to 2**31;
		if (pos.x < navigationCoordMax && pos.x > -navigationCoordMax &&
			pos.y < navigationCoordMax && pos.y > -navigationCoordMax &&
			pos.z < navigationCoordMax && pos.z > -navigationCoordMax)
			return true;
		return false;
	}

	inline KyFloat32 IntToNavigation_Dist(KyUInt32 intValue)     { return IntCoordSystem::IntToNavigation_Dist(intValue, m_genParams.GetNormalizedRasterPrecision());}

	inline KyUInt32 NavigationToInt_Dist(KyFloat32 coordInMeter) { return IntCoordSystem::NavigationToInt_Dist(coordInMeter, m_genParams.GetNormalizedRasterPrecision());}

	inline void IntToNavigation_Pos(const Vec2i& intPos, Vec2f& pos)
	{
		pos.x = IntToNavigation_Dist(intPos.x);
		pos.y = IntToNavigation_Dist(intPos.y);
	}

	inline void IntToNavigation_Pos(const Vec3i& intPos, Vec3f& pos)
	{
		pos.x = IntToNavigation_Dist(intPos.x);
		pos.y = IntToNavigation_Dist(intPos.y);
		pos.z = IntToNavigation_Dist(intPos.z);
	}

	inline void NavigationToInt_Pos(const Vec2f& pos, Vec2i& intPos)
	{
		intPos.x = NavigationToInt_Dist(pos.x);
		intPos.y = NavigationToInt_Dist(pos.y);
	}

	inline  void NavigationToInt_Pos(const Vec3f& pos, Vec3i& intPos)
	{
		intPos.x = NavigationToInt_Dist(pos.x);
		intPos.y = NavigationToInt_Dist(pos.y);
		intPos.z = NavigationToInt_Dist(pos.z);
	}

	// TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO TODO
	// END REMOVE THIS FROM GeneratorSystem

	void StartVisualDebug();
	void StopVisualDebug();
	bool IsVisualDebugConnected();

	/// Indicates whether or not the input geometry will be flushed in file periodically to avoid exceeding memory limits. 
	bool IsLowMemoryModeEnabled() const { return RunOptions().m_doEnableLimitedMemoryMode; }

public:
	String m_generatorInputOuputSaveFileName;
	GeneratorParameters m_genParams;
	GlobalConfigSource m_globalConfigSource;
	GeneratorRunOptions m_runOptions;
	GeneratorFileSystem m_fileSystem;
	SquarePartition m_partition;
	GeneratorNavDataElementMap m_navDataElementGlobalMap;
	BlobCategory* m_blobRegistry;
	Ptr<IParallelForInterface> m_parallelForInterface;
	Ptr<ITlsAlloc> m_tlsAlloc;
	GeneratorProfiling* m_generatorProfiling;
	FileOpenerBase* m_fileOpener;
	VisualDebugServer* m_visualDebugServer;
};


template <class OSTREAM>
inline OSTREAM& operator<<(OSTREAM& os, GeneratorSystem& sys)
{
	os << KY_LOG_SMALL_TITLE_BEGIN("", "GeneratorSystem Info");
	os << "ParallelForInterface status:       "  << (sys.m_parallelForInterface       != KY_NULL ? "SET" : "UNSET") << Endl;
	os << "VisualDebugServer status:           " << (sys.IsVisualDebugConnected()  ? "CONNECTED" : "DECONNECTED")   << Endl;
	os << "GamewareKey : "                       << (Kaim::GamewareKey::GetInstance().GetKey())                     << Endl;
	os << KY_LOG_SMALL_TITLE_END("", "GeneratorSystem Info");
	return os;
}

}


#endif

