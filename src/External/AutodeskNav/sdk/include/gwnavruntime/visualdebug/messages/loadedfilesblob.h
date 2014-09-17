/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

#ifndef LabEngine_LoadedFilesBlob_H
#define LabEngine_LoadedFilesBlob_H


#include "gwnavruntime/visualdebug/visualdebugblobcategory.h"
#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavruntime/blob/blobtypes.h"
#include "gwnavruntime/kernel/SF_String.h"
#include "gwnavruntime/visualdebug/messages/stringblob.h"

namespace Kaim
{

class LoadedFilesBlob
{
	KY_ROOT_BLOB_CLASS(VisualDebugMessage, LoadedFilesBlob, 0)

public:
	LoadedFilesBlob() {}

	BlobArray<StringBlob> m_fileNames;
};

inline void SwapEndianness(Kaim::Endianness::Target e, LoadedFilesBlob& self)
{
	SwapEndianness(e, self.m_fileNames);
}

class LoadedFilesBlobBuilder : public Kaim::BaseBlobBuilder<LoadedFilesBlob>
{
	KY_CLASS_WITHOUT_COPY(LoadedFilesBlobBuilder)
public:
	LoadedFilesBlobBuilder(const KyArray<Kaim::String>& fileNames) : m_fileNames(fileNames)
	{}

	~LoadedFilesBlobBuilder() {}

	virtual void DoBuild()
	{
		StringBlob* fileNames = BLOB_ARRAY(m_blob->m_fileNames, m_fileNames.GetCount());
		for (KyUInt32 i = 0; i < m_fileNames.GetCount(); ++i)
		{
			BLOB_BUILD(fileNames[i], StringBlobBuilder(m_fileNames[i]));
		}
	}

	const KyArray<String> m_fileNames;
};

}

#endif