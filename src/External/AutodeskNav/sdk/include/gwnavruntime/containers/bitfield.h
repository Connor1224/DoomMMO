/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/

#ifndef Navigation_BitField_H
#define Navigation_BitField_H

#include "gwnavruntime/base/memory.h"

namespace Kaim
{


class BitFieldBase
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)

public:
	static KY_INLINE KyUInt32 GetWordIndex(KyUInt32 bitIndex)            { return bitIndex >> 5;        } // / 32
	static KY_INLINE KyUInt32 GetBitIndexInWord(KyUInt32 bitIndex)       { return bitIndex & 0x1F;      } // % 32
	static KY_INLINE KyUInt32 GetWordsCount(KyUInt32 bitCount)           { return (bitCount + 31) >> 5; } // / 32
	static KY_INLINE KyUInt32 GetWordMaskFromBitIndex(KyUInt32 bitIndex) { return GetWordMask(GetBitIndexInWord(bitIndex)); }

	static KY_INLINE void SetBit(KyUInt32& word, KyUInt32 bitIndex)   { word |= GetWordMaskFromBitIndex(bitIndex);              }
	static KY_INLINE void UnsetBit(KyUInt32& word, KyUInt32 bitIndex) { word &= ~GetWordMaskFromBitIndex(bitIndex);             }
	static KY_INLINE bool IsBitSet(KyUInt32 word, KyUInt32 bitIndex)  { return (word & GetWordMaskFromBitIndex(bitIndex)) != 0; }

	static KY_INLINE void SetBit(KyUInt32* words, KyUInt32 bitIndex)         { SetBit(words[GetWordIndex(bitIndex)], bitIndex);          }
	static KY_INLINE void UnsetBit(KyUInt32* words, KyUInt32 bitIndex)       { UnsetBit(words[GetWordIndex(bitIndex)], bitIndex);        }
	static KY_INLINE bool IsBitSet(const KyUInt32* words, KyUInt32 bitIndex) { return IsBitSet(words[GetWordIndex(bitIndex)], bitIndex); }

	static KY_INLINE KyUInt32 GetWordMask(KyUInt32 bitIndexInWord)
	{
		static const KyUInt32 s_maskToIsolateBit[32] =
		{
			       1, 1U <<  1, 1U <<  2, 1U <<  3, 1U <<  4, 1U <<  5, 1U <<  6, 1U <<  7,
			1U <<  8, 1U <<  9, 1U << 10, 1U << 11, 1U << 12, 1U << 13, 1U << 14, 1U << 15,
			1U << 16, 1U << 17, 1U << 18, 1U << 19, 1U << 20, 1U << 21, 1U << 22, 1U << 23,
			1U << 24, 1U << 25, 1U << 26, 1U << 27, 1U << 28, 1U << 29, 1U << 30, 1U << 31
		};

		return s_maskToIsolateBit[bitIndexInWord];
	}

public:

	BitFieldBase() : m_words(KY_NULL), m_wordsCapacity(0), m_bitsCount(0) {}

	// Create a bitfield of bitsCount bits (which may be round up) with all bits unset 
	explicit BitFieldBase(KyUInt32 bitsCount, int memStat) : m_words(KY_NULL), m_wordsCapacity(0), m_bitsCount(0) { Resize_(bitsCount, memStat); }

	~BitFieldBase() { KY_FREE(m_words); }

	KyUInt32 GetCount() const;

	void SetBit(KyUInt32 bitIndex);
	void UnsetBit(KyUInt32 bitIndex);
	bool IsBitSet(KyUInt32 bitIndex) const;

	void UnsetAllBits();
	void SetAllBits();

	// bitValue must be 0 or 1
	void SetBitValue(KyUInt32 bitIndex, KyUInt32 bitValue);
	// return 0 or 1
	KyUInt32 GetBitValue(KyUInt32 bitIndex) const;

	void Clear(); // frees memory !

protected:
	void Resize_(KyUInt32 newBitsCount, int memStat);
	void Reserve_(KyUInt32 newBitsCount, int memStat);

private:
	typedef KyUInt32 Word32;

	Word32  Word(KyUInt32 bitIndex) const;
	Word32& Word(KyUInt32 bitIndex)      ;

private:
	Word32* m_words;
	KyUInt32 m_wordsCapacity;
	KyUInt32 m_bitsCount;
};

KY_INLINE void     BitFieldBase::SetBitValue(KyUInt32 bitIndex, KyUInt32 bitValue) { bitValue == 0 ? UnsetBit(bitIndex) : SetBit(bitIndex); }
KY_INLINE KyUInt32 BitFieldBase::GetBitValue(KyUInt32 bitIndex)              const { return (Word(bitIndex) >> GetBitIndexInWord(bitIndex)) & 0x00000001; }

KY_INLINE void     BitFieldBase::SetBit(KyUInt32 bitIndex)            { SetBit(m_words, bitIndex); }
KY_INLINE void     BitFieldBase::UnsetBit(KyUInt32 bitIndex)          { UnsetBit(m_words, bitIndex); }
KY_INLINE bool     BitFieldBase::IsBitSet(KyUInt32 bitIndex)    const { return IsBitSet(m_words , bitIndex); }
KY_INLINE KyUInt32 BitFieldBase::GetCount()                     const { return m_bitsCount; }

KY_INLINE BitFieldBase::Word32  BitFieldBase::Word(KyUInt32 bitIndex) const { return m_words[GetWordIndex(bitIndex)]; }
KY_INLINE BitFieldBase::Word32& BitFieldBase::Word(KyUInt32 bitIndex)       { return m_words[GetWordIndex(bitIndex)]; }

template<int SID>
class BitFieldMemStat : public BitFieldBase
{
	KY_DEFINE_NEW_DELETE_OPERATORS(SID)
public:
	explicit BitFieldMemStat() : BitFieldBase() {}
	explicit BitFieldMemStat(KyUInt32 bitsCount) : BitFieldBase(bitsCount, SID) {}
	void Resize(KyUInt32 newBitsCount)  { Resize_(newBitsCount, SID);  }
	void Reserve(KyUInt32 newBitsCount) { Reserve_(newBitsCount, SID); }
};

typedef BitFieldMemStat<Stat_Default_Mem> BitField;

} // Kaim

#endif // Navigation_BitField_H
