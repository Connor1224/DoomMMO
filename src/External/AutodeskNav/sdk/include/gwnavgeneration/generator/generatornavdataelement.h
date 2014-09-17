/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_GeneratorNavDataElement_H
#define GwNavGen_GeneratorNavDataElement_H

#include "gwnavruntime/blob/blobhandler.h"
#include "gwnavruntime/kernel/SF_String.h"
#include "gwnavgeneration/input/dynamicgrid.h"
#include "gwnavruntime/navmesh/blobs/navmeshelementblob.h"
#include "gwnavgeneration/generator/generatorguidcompound.h"

namespace Kaim
{

class GeneratorSystem;
class PdgInput;
class CellGenerator;
class NavMeshElementBlob;
class GeneratorSectorBuilder;
struct PdgInputTileDescriptor;

class GeneratorNavDataElement
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:

	class CellGeneratorMapElement
	{
		KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	public: 
		CellGeneratorMapElement(KyInt32 x, KyInt32 y): m_x(x), m_y(y), m_generator(KY_NULL) {}
	public: 
		KyInt32 m_x;
		KyInt32 m_y;
		CellGenerator* m_generator;
	};

public:
	GeneratorNavDataElement(GeneratorSystem* sys)
		: m_sys(sys)
		, m_mainSectorGenerator(KY_NULL)
	{}
	
	// DeAllocate the Cells pointed in m_cellGenerators and clear the arrays.
	// NB. ClearCells is NOT called automatically when GeneratorNavDataElement is destroyed !
	// see note below
	void ClearCells();

	void ClearNavCells();

	NavMeshElementBlob* GetNavMeshElementBlob() { return m_navMeshElementHandler->Blob(); }

	KyResult GenerateRawNavCells();
	KyResult FilterNavCells();

	KyResult AggregateNavMeshElementFromNavCells();

	void ComputeCellBBox();

	KyResult SaveIndividualElements(Endianness::Type endianness);

	bool operator<(const GeneratorNavDataElement& other);

private:
	String GetIndividualElementFileName();

public:
	GeneratorSystem* m_sys;
	GeneratorGuidCompound m_guidCompound;
	GeneratorSectorBuilder* m_mainSectorGenerator;

	// Please note that cells allocated and stored in m_cellGeneratorsGrid
	// must be deallocated manually (not done in dtor).
	// DeAllocation is performed by ClearCells() method which is called from GeneratorSectorBuilder.
	// The reason for this is that a partitioner might create an aliasing
	// where 2 different navdata elements store pointers to the same cells
	// If we were to call ClearCells in the dtor, we would risk a 2x DeAlloc.
	DynamicGrid<CellGeneratorMapElement> m_cellGeneratorsGrid;
	CellBox m_cellBox;

	Ptr< BlobHandler<NavMeshElementBlob> > m_navMeshElementHandler;

	// when the PDE is an "overlap" pde, several tiles need to be loaded in order
	// to generate the proper navdata
	KyArrayPOD<const PdgInputTileDescriptor*> m_tileDependencies;
};


}


#endif

