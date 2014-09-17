/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: LAPA - secondary contact: NOBODY

#ifndef Navigation_FloatStat_H
#define Navigation_FloatStat_H

#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/kernel/SF_String.h"


namespace Kaim
{

class FloatStat
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_Statistics)

public:
	FloatStat() { ResetValues(); }
	FloatStat(const String& name, KyUInt32 dataSize = 300) { Init(name, dataSize); }

	// Default data size is 300 to get statistics on a cyclic window of 5 seconds at 60 FPS.
	void Init(const String& name, KyUInt32 dataSize = 300)
	{
		m_name = name;
		SetDataSize(dataSize);
	}

	void SetDataSize(KyUInt32 dataSize)
	{
		m_values.Resize(dataSize);
		ResetValues();
	}

	void ResetValues();
	void Update();

	const String& GetName()    const { return m_name; }
	KyFloat32     GetCurrent() const { return m_current; }
	KyFloat32     GetAverage() const { return m_average; }
	KyFloat32     GetMin()     const { return m_min; }
	KyFloat32     GetMax()     const { return m_max; }

	KyFloat32& GetWritableCurrent() { return m_current; }
	void SetCurrent(KyFloat32 current) { m_current = current; }

private:
	void AddValue();
	void ReplaceValue();
	void UpdateMin(KyUInt32 index);
	void UpdateMax(KyUInt32 index);

	String m_name;
	Array<KyFloat32> m_values;
	KyFloat32 m_current;
	KyFloat32 m_average;
	KyFloat32 m_min;
	KyFloat32 m_max;
	KyUInt32 m_currentIndex;
	KyUInt32 m_minIndex;
	KyUInt32 m_maxIndex;
	bool m_cycled;
};

} // namespace Kaim

#endif // Navigation_FloatStat_H
