/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_DynamicGrid_H
#define GwNavGen_DynamicGrid_H


#include "gwnavruntime/math/box2i.h"
#include "gwnavruntime/containers/kyarray.h"


namespace Kaim
{
class DynamicGridColumn;

/*
cells are indexed this way
  |     |     |     |     |
--+-----+-----+-----+-----+--
  |-2, 1|-1, 1| 0, 1| 1, 1|
  |     |     |     |     |
--+-----+-----+-----+-----+--
  |-2, 0|-1, 0| 0, 0| 1, 0|
  |     |     |     |     |
--+-----+---(0,0)---+-----+--
  |-2,-1|-1,-1| 0,-1| 1,-1|
  |     |     |     |     |
--+-----+-----+-----+---- +--
  |-2,-2|-1,-2| 0,-2| 1,-2|
  |     |     |     |     |
--+-----+-----+-----+-----+--
  |     |     |     |     |
*/

class BaseElementCreator : public NewOverrideBase<MemStat_NavDataGen>
{
public:
	virtual void* NewElement(KyInt32 x, KyInt32 y) = 0;
	virtual void DeleteElement(void* element) = 0;
	virtual ~BaseElementCreator() {}
};


class BaseDynamicGrid : public NewOverrideBase<MemStat_NavDataGen>
{
public:
	BaseDynamicGrid(BaseElementCreator* baseElementCreator) :
		m_baseElementCreator(baseElementCreator)
	{ Clear(); }

	virtual ~BaseDynamicGrid() {}

	void Clear();

protected:
	void* GetVoidElement(KyInt32 x, KyInt32 y);
	void* GetOrCreateVoidElement(KyInt32 x, KyInt32 y);
	DynamicGridColumn* GetColumn(KyInt32 x);
	void* AddNewElement(KyInt32 x, KyInt32 y);
	void DeleteElement(void* element) { m_baseElementCreator->DeleteElement(element); }

protected:
	BaseElementCreator* m_baseElementCreator;

	KyArrayPOD<void*> m_elements;
	Box2i m_box;

	// columns
	KyInt32 m_root_x;
	KyArrayPOD<DynamicGridColumn*> m_plus_x;  //          [root_x..]
	KyArrayPOD<DynamicGridColumn*> m_minus_x; // ]..root_x[

	friend class DynamicGridColumn;
};


template<class T>
class DynamicGrid : public BaseDynamicGrid
{
public:
	class ElementCreator : public BaseElementCreator
	{
	public:
		virtual void* NewElement(KyInt32 x, KyInt32 y) { return KY_NEW T(x, y); }
		virtual void DeleteElement(void* element) { delete (T*)element; }
	};

public:
	DynamicGrid() : BaseDynamicGrid(&m_elementCreator) {}
	DynamicGrid(BaseElementCreator* baseElementCreator) : BaseDynamicGrid(baseElementCreator) {}

	virtual ~DynamicGrid() { Clear(); }

	T* GetElement(KyInt32 x, KyInt32 y) { return (T*)GetVoidElement(x, y); }

	T* GetOrCreateElement(KyInt32 x, KyInt32 y) { return (T*)GetOrCreateVoidElement(x, y); }

	T** GetElements()
	{
		if (m_elements.GetCount() == 0)
			return KY_NULL;
		return (T**)&m_elements[0];
	}

	KyUInt32 GetElementsCount() const { return m_elements.GetCount(); }

private:
	ElementCreator m_elementCreator;
};


}


#endif
