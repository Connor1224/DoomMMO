/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



#ifndef KyGlue_DefaultGeneratorGlue_H
#define KyGlue_DefaultGeneratorGlue_H


#include "gwnavgeneration/generator/igeneratorglue.h"
#include "gwnavgenerationglue/defaultguidgeneratorinterface/defaultguidgeneratorinterface.h"
#include "gwnavgenerationglue/tbbparallelforinterface/tbbparallelforinterface.h"
#include "gwnavgenerationglue/tbbtlsalloc/tbbtlsalloc.h"


namespace KyGlue
{

/// TODO 
class DefaultGeneratorGlue : public Kaim::IGeneratorGlue
{
public:
	virtual Kaim::IParallelForInterface*   GetParallelFor()   { return &m_tbbparallelFor;       }
	virtual Kaim::ITlsAlloc*               GetTlsAlloc()      { return &m_tbbtlsAlloc;          }

public:
	KyGlue::DefaultGuidGeneratorInterface   m_defaultGuidGenerator;
	KyGlue::TbbParallelForInterface         m_tbbparallelFor;
	KyGlue::TbbTlsAlloc                     m_tbbtlsAlloc;
};


}

#endif

