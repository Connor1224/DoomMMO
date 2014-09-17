/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

// primary contact: LASI - secondary contact: GUAL
#ifndef GwNavGen_GeneratorSectorsConfigBlobBuilder_H
#define GwNavGen_GeneratorSectorsConfigBlobBuilder_H

#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavgeneration/generator/generatorconfigblob.h"
#include "gwnavgeneration/generator/generatorguidcompound.h"
#include "gwnavruntime/containers/kyarray.h"

namespace Kaim
{

class GeneratorInputOutput;

class GeneratorSectorsConfigBlobBuilder : public BaseBlobBuilder<GeneratorSectorsConfigBlob>
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:
	GeneratorSectorsConfigBlobBuilder(const GeneratorInputOutput& config, GenFlags::GenIoFilter filter) : m_generatorInputOutput(&config), m_filter(filter) {}

private:
	virtual void DoBuild();
private: 
	const GeneratorInputOutput* m_generatorInputOutput;
	GenFlags::GenIoFilter m_filter;
};

}


#endif

