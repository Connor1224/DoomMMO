/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LASI - secondary contact: NOBODY
#ifndef GwNavGen_PdgInputProducerFromClientInput_H
#define GwNavGen_PdgInputProducerFromClientInput_H


#include "gwnavruntime/kernel/SF_String.h"
#include "gwnavgeneration/input/generatorinputproducer.h"
#include "gwnavgeneration/input/clientinputconsumer.h"


namespace Kaim
{

/* Used through Generator::AddSectorFromClientInput() */
class ClientInputFeeder : public RefCountBase<ClientInputFeeder, MemStat_NavDataGen>
{
public:
	virtual ~ClientInputFeeder();

	ClientInput* AddClientInputFileName(const char* clientInputFileName, FileOpenerBase* fileOpener);

	KyResult Feed(ClientInputConsumer& inputConsumer);

public:
	struct Entry
	{
		Entry() : m_clientInput(KY_NULL) {}
		String m_fileName;
		ClientInput* m_clientInput;
	};
	KyArray<Entry> m_clientInputs;
};


}


#endif // GwNavGen_PdgInputProducerFromClientInput_H

