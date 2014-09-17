/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef GwNavGen_DynamicGridColumn_H
#define GwNavGen_DynamicGridColumn_H

#include "gwnavruntime/base/memory.h"
#include "gwnavruntime/containers/kyarray.h"


namespace Kaim
{


class BaseDynamicGrid;


class DynamicGridColumn
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	DynamicGridColumn() :
		m_grid(KY_NULL), m_x(KyInt32MAXVAL), m_root_y(KyInt32MAXVAL)
	{}

	DynamicGridColumn(BaseDynamicGrid* grid, KyInt32 x) :
		m_grid(grid), m_x(x), m_root_y(KyInt32MAXVAL)
	{}

	void* GetOrCreateVoidElement(KyInt32 y);

	void* GetVoidElement(KyInt32 y);

public:
	BaseDynamicGrid* m_grid;
	KyInt32 m_x;
	KyInt32 m_root_y;
	KyArrayPOD<void*> m_plus_y;  //          [root_y..[
	KyArrayPOD<void*> m_minus_y; // ]..root_y[
};


}


#endif
