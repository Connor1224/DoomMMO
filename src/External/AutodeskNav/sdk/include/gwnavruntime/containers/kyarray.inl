/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


namespace Kaim
{

// Only one template implementation for all the KyArray classes
namespace ArrayAlg
{
	template <typename ArrayT, typename ValueT>
	bool DoesContain(const ArrayT& array, const ValueT& value)
	{
		for (UPInt i = 0; i < array.GetSize(); ++i)
		{
			if (array[i] == value)
				return true;
		}
		return false;
	}


	// simpler
	template <typename ArrayT>
	void RemoveConsecutiveDuplicates(ArrayT& array)
	{
		if (array.GetCount() == 0)
			return;

		UPInt write = 0;
		for (UPInt read = 1; read < array.GetSize(); ++read)
		{
			if (array[write] != array[read])
			{
				++write;
				array[write] = array[read];
			}
		}
		array.Resize(write + 1);
	}
}


template<class T, int SID, class SizePolicy>
KY_INLINE bool KyArray<T, SID, SizePolicy>::DoesContain(const ValueType& value) const { return ArrayAlg::DoesContain<SelfType,ValueType>(*this, value); }
template<class T, int SID, class SizePolicy>
KY_INLINE void KyArray<T, SID, SizePolicy>::RemoveConsecutiveDuplicates() { return ArrayAlg::RemoveConsecutiveDuplicates<SelfType>(*this); }

template<class T, int SID, class SizePolicy>
KY_INLINE bool KyArrayPOD<T, SID, SizePolicy>::DoesContain(const ValueType& value) const { return ArrayAlg::DoesContain<SelfType,ValueType>(*this, value); }
template<class T, int SID, class SizePolicy>
KY_INLINE void KyArrayPOD<T, SID, SizePolicy>::RemoveConsecutiveDuplicates() { return ArrayAlg::RemoveConsecutiveDuplicates<SelfType>(*this); }

template<class T, int SID, class SizePolicy>
KY_INLINE bool KyArrayLH<T, SID, SizePolicy>::DoesContain(const ValueType& value) const { return ArrayAlg::DoesContain<SelfType,ValueType>(*this, value); }
template<class T, int SID, class SizePolicy>
KY_INLINE void KyArrayLH<T, SID, SizePolicy>::RemoveConsecutiveDuplicates() { return ArrayAlg::RemoveConsecutiveDuplicates<SelfType>(*this); }

template<class T, int SID, class SizePolicy>
KY_INLINE bool KyArrayLH_POD<T, SID, SizePolicy>::DoesContain(const ValueType& value) const { return ArrayAlg::DoesContain<SelfType,ValueType>(*this, value); }
template<class T, int SID, class SizePolicy>
KY_INLINE void KyArrayLH_POD<T, SID, SizePolicy>::RemoveConsecutiveDuplicates() { return ArrayAlg::RemoveConsecutiveDuplicates<SelfType>(*this); }

template<class T, int SID, class SizePolicy>
KY_INLINE bool KyArrayDH<T, SID, SizePolicy>::DoesContain(const ValueType& value) const { return ArrayAlg::DoesContain<SelfType,ValueType>(*this, value); }
template<class T, int SID, class SizePolicy>
KY_INLINE void KyArrayDH<T, SID, SizePolicy>::RemoveConsecutiveDuplicates() { return ArrayAlg::RemoveConsecutiveDuplicates<SelfType>(*this); }

template<class T, int SID, class SizePolicy>
KY_INLINE bool KyArrayDH_POD<T, SID, SizePolicy>::DoesContain(const ValueType& value) const { return ArrayAlg::DoesContain<SelfType,ValueType>(*this, value); }
template<class T, int SID, class SizePolicy>
KY_INLINE void KyArrayDH_POD<T, SID, SizePolicy>::RemoveConsecutiveDuplicates() { return ArrayAlg::RemoveConsecutiveDuplicates<SelfType>(*this); }


} // namespace Kaim

