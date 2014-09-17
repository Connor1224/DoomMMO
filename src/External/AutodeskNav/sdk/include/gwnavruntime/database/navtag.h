/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// primary contact: GUAL - secondary contact: LASI
#ifndef Navigation_NavTag_H
#define Navigation_NavTag_H

#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavruntime/navmesh/navmeshtypes.h"
#include "gwnavruntime/containers/kyarray.h"
#include "gwnavruntime/navdata/navdatablobcategory.h"

namespace Kaim
{

typedef KyUInt32 NavTagIdx;
static const NavTagIdx NavTagIdx_Invalid = KyUInt32MAXVAL;
typedef KyUInt32 BlindDataIdx;

class NavTag;

enum NavTagExclusionFlag
{
	NAVTAG_FLAG_DEFAULT,
	NAVTAG_FLAG_EXCLUSIVE
};

class DynamicNavTag
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	DynamicNavTag() : m_exclusionFlag(NAVTAG_FLAG_DEFAULT) {}
	bool operator!=(const DynamicNavTag& other) const { return !operator==(other); }
	bool operator==(const DynamicNavTag& other) const
	{
		if (m_blindDataArray.GetCount() != other.m_blindDataArray.GetCount())
			return false;

		for (KyUInt32 i = 0; i < m_blindDataArray.GetCount(); ++i)
		{
			if (m_blindDataArray[i] != other.m_blindDataArray[i])
				return false;
		}

		if (m_exclusionFlag != other.m_exclusionFlag)
			return false;

		return true;
	}
	void SetAsExclusive() { m_exclusionFlag = NAVTAG_FLAG_EXCLUSIVE; }
	bool IsExclusive() const { return m_exclusionFlag == NAVTAG_FLAG_EXCLUSIVE; }

	// NavTags are compared using the values in the blind array
	KY_INLINE bool operator<(const DynamicNavTag& other) const
	{
		if(m_exclusionFlag != other.m_exclusionFlag)
		{
			return IsExclusive();
		}
		KyUInt32 minCount = Kaim::Min(m_blindDataArray.GetCount(), other.m_blindDataArray.GetCount());

		for(KyUInt32 wordIdx = 0; wordIdx < minCount; ++wordIdx)
		{
			if(m_blindDataArray[wordIdx] < other.m_blindDataArray[wordIdx])
				return true;
		}

		if(m_blindDataArray.GetCount() != other.m_blindDataArray.GetCount())
			return m_blindDataArray.GetCount() < other.m_blindDataArray.GetCount();

		return false;
	}

	void InitFromBlob(const NavTag& navTag);
	void Init(const KyUInt32* blindDataBuffer, KyUInt32 blindDataCount)
	{
		m_blindDataArray.Resize(blindDataCount);
		for(KyUInt32 idx = 0; idx < blindDataCount; ++idx)
			m_blindDataArray[idx] = blindDataBuffer[idx];
	}

	void Clear()
	{
		m_blindDataArray.Clear();
	}

public:
	KyArrayPOD<KyUInt32> m_blindDataArray;
	NavTagExclusionFlag m_exclusionFlag;
};


class NavTag
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_ROOT_BLOB_CLASS(NavData, NavTag, 0)
	KY_CLASS_WITHOUT_COPY(NavTag)
public:
	NavTag() {}
	bool operator!=(const NavTag& other) const { return !operator==(other); }
	bool operator==(const NavTag& other) const
	{
		if(GetWordCount() != other.GetWordCount())
			return false;

		const KyUInt32* words = GetWords();
		const KyUInt32* otherWords = other.GetWords();
		for(KyUInt32 wordIdx = 0; wordIdx < GetWordCount(); ++wordIdx)
		{
			if(words[wordIdx] != otherWords[wordIdx])
				return false;
		}
		return m_exclusionFlag == other.m_exclusionFlag;
	}

	bool operator<(const NavTag& other) const 
	{	
		if(m_exclusionFlag != other.m_exclusionFlag)
		{
			return IsExclusive();
		}
		KyUInt32 minCount = Kaim::Min(GetWordCount(), other.GetWordCount());

		const KyUInt32* words = GetWords();
		const KyUInt32* otherWords = other.GetWords();
		for(KyUInt32 wordIdx = 0; wordIdx < minCount; ++wordIdx)
		{
			if(words[wordIdx] >= otherWords[wordIdx])
				return false;
		}

		if(GetWordCount() != other.GetWordCount())
			return GetWordCount() < other.GetWordCount();

		return false;
	}

	KyUInt32  GetWordCount()                const;
	const KyUInt32* GetWords()                    const;
	KyUInt32  GetWord(BlindDataIdx wordIdx) const;
	KyUInt32 GetByteSize() const { return KyUInt32((char*)(m_blindData.GetValues() + m_blindData.GetCount()) - (char*)this); }

	bool IsExclusive() const { return m_exclusionFlag == NAVTAG_FLAG_EXCLUSIVE; }

public:
	BlobArray<KyUInt32> m_blindData;
	KyUInt32 m_exclusionFlag;
};

inline void SwapEndianness(Endianness::Target e, NavTag& self)
{
	SwapEndianness(e, self.m_blindData);
	SwapEndianness(e, self.m_exclusionFlag);
}

class NavTagBlobBuilder : public BaseBlobBuilder<NavTag>
{
public:
	NavTagBlobBuilder(const DynamicNavTag& navTag) : m_navTag(&navTag) {}
private:
	void DoBuild()
	{
		if(IsWriteMode())
		{
			m_blob->m_exclusionFlag = m_navTag->m_exclusionFlag;
		}
		KyUInt32* blobArray = BLOB_ARRAY(m_blob->m_blindData, m_navTag->m_blindDataArray.GetCount());
		if (this->IsWriteMode())
		{
			for (KyUInt32 i = 0; i < m_navTag->m_blindDataArray.GetCount(); ++i)
				blobArray[i] = m_navTag->m_blindDataArray[i];
		}
	}

	const DynamicNavTag* m_navTag;
};

class NavTagCopier : public BaseBlobBuilder<NavTag>
{
public:
	NavTagCopier(const NavTag& navTag) : m_navTag(&navTag) {}
private:
	void DoBuild()
	{
		if(IsWriteMode())
		{
			m_blob->m_exclusionFlag = m_navTag->m_exclusionFlag;
		}
		KyUInt32* blobArray = BLOB_ARRAY(m_blob->m_blindData, m_navTag->GetWordCount());
		if (this->IsWriteMode())
		{
			for (KyUInt32 i = 0; i < m_navTag->GetWordCount(); ++i)
				blobArray[i] = m_navTag->GetWord(i);
		}
	}

	const NavTag* m_navTag;
};

KY_INLINE void DynamicNavTag::InitFromBlob(const NavTag& navTag)
{
	Init(navTag.GetWords(), navTag.GetWordCount());
	m_exclusionFlag = (NavTagExclusionFlag)navTag.m_exclusionFlag;
}

KY_INLINE       KyUInt32  NavTag::GetWordCount()                const { return m_blindData.GetCount();  }
KY_INLINE const KyUInt32* NavTag::GetWords()                    const { return m_blindData.GetValues(); }
KY_INLINE       KyUInt32  NavTag::GetWord(BlindDataIdx wordIdx) const
{
	KY_DEBUG_ASSERTN(wordIdx < GetWordCount(), ("Wrong index pass to this function. input is %u, it must be lower than %u", wordIdx, GetWordCount()));
	return GetWords()[wordIdx];
}
}


#endif

