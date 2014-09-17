/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LASI - secondary contact: GUAL
#ifndef GwNavGen_GeneratorSectorsConfigBlob_H
#define GwNavGen_GeneratorSectorsConfigBlob_H

#include "gwnavgeneration/generator/generatorsectorconfigblob.h"
#include "gwnavgeneration/generator/generatorparameters.h"
#include "gwnavruntime/base/guidcompound.h"
#include "gwnavgeneration/generator/generatorguidcompound.h"

namespace Kaim
{

class GeneratorParameters;
class GeneratorSectorList;
class GeneratorSectorConfig;

class GeneratorSectorsConfigBlob
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_ROOT_BLOB_CLASS(Generator, GeneratorSectorsConfigBlob, 0)

public:
	void GetSectorList(GeneratorSectorList& sectorList, GenFlags::GenIoFilter filter) const;
	void GetExclusiveGuids(KyArray<GeneratorGuidCompound>& guids) const;
	void GetExplicitOverlaps(KyArray<GeneratorGuidCompound>& guids) const;

public:
	BlobArray<GeneratorSectorConfigBlob> m_sectors;
	BlobArray<GuidCompound> m_exclusiveGuids;
	BlobArray<GuidCompound> m_explicitOverlapSectors;

};
inline void SwapEndianness(Endianness::Target e, GeneratorSectorsConfigBlob& self)
{
	SwapEndianness(e, self.m_sectors);
	SwapEndianness(e, self.m_exclusiveGuids);
	SwapEndianness(e, self.m_explicitOverlapSectors);
}

}


#endif

