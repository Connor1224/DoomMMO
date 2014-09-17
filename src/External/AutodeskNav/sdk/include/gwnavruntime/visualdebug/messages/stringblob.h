/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

#ifndef LabEngine_StringBlob_H
#define LabEngine_StringBlob_H

#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavruntime/blob/blobtypes.h"
#include "gwnavruntime/kernel/SF_String.h"

namespace Kaim
{

class StringBlob
{
public:
	StringBlob() {}

	Kaim::BlobArray<char> m_string;
};


inline void SwapEndianness(Kaim::Endianness::Target e, StringBlob& self)
{
	SwapEndianness(e, self.m_string);
}


class StringBlobBuilder : public Kaim::BaseBlobBuilder<StringBlob>
{
	KY_CLASS_WITHOUT_COPY(StringBlobBuilder)
public:
	StringBlobBuilder(const String& string) : m_string(string) {}
	~StringBlobBuilder() {}

	virtual void DoBuild()
	{
		BLOB_ARRAY_COPY(m_blob->m_string, m_string.ToCStr(), m_string.GetLength());
	}

	String m_string;
};

}

#endif