/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


#ifndef Navigation_ArrayTLS_H
#define Navigation_ArrayTLS_H

#include "gwnavruntime/containers/kyarray.h"
#include "gwnavgeneration/common/generatormemory.h"

namespace Kaim
{

// ArrayTLS is a ArrayDH that always uses GeneratorMemory::TlsHeap()
// This avoid to pass GeneratorMemory::TlsHeap() to ArrayDH constructors
template<class T>
class KyArrayTLS : public KyArrayDH<T>
{
public:
	typedef T             ValueType;
	typedef KyArrayTLS    SelfType;
	typedef KyArrayDH<T>  BaseType;

	KyArrayTLS()                                 : BaseType(GeneratorMemory::TlsHeap())       {}
	explicit KyArrayTLS(int size)                : BaseType(GeneratorMemory::TlsHeap(), size) {}
	KyArrayTLS(const SelfType& a) : BaseType(a) {}
	const SelfType& operator=(const SelfType& a) { BaseType::operator=(a); return *this; }

	KyUInt32 GetCount() const { return (KyUInt32)BaseType::GetSize(); }
	bool DoesContain(const ValueType& value) const { return ArrayAlg::DoesContain<SelfType,ValueType>(*this, value); }
	void RemoveConsecutiveDuplicates() { return ArrayAlg::RemoveConsecutiveDuplicates<SelfType>(*this); }
};


// ArrayTLS_POD is a ArrayDH_POD that always uses GeneratorMemory::TlsHeap()
// This avoid to pass GeneratorMemory::TlsHeap() to ArrayDH_POD constructors
template<class T>
class KyArrayTLS_POD : public ArrayDH_POD<T>
{
public:
	typedef T               ValueType;
	typedef KyArrayTLS_POD  SelfType;
	typedef ArrayDH_POD<T>  BaseType;

	KyArrayTLS_POD()                        : BaseType(GeneratorMemory::TlsHeap())       {}
	explicit KyArrayTLS_POD(int size)                : BaseType(GeneratorMemory::TlsHeap(), size) {}
	KyArrayTLS_POD(const SelfType& a) : BaseType(a) {}
	const SelfType& operator=(const SelfType& a) { BaseType::operator=(a); return *this; }

	KyUInt32 GetCount() const { return (KyUInt32)BaseType::GetSize(); }
	bool DoesContain(const ValueType& value) const { return ArrayAlg::DoesContain<SelfType,ValueType>(*this, value); }
	void RemoveConsecutiveDuplicates() { return ArrayAlg::RemoveConsecutiveDuplicates<SelfType>(*this); }
};


} // namespace Kaim


#endif // Navigation_ArrayTLS_H
