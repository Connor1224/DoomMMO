/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY
#ifndef Navigation_BlobField32_H
#define Navigation_BlobField32_H


#include "gwnavruntime/base/types.h"
#include "gwnavruntime/blob/blobarray.h"
#include "gwnavruntime/blob/baseblobbuilder.h"
#include "gwnavruntime/kernel/SF_String.h"
#include "gwnavruntime/containers/kyarray.h"


namespace Kaim
{


class BlobField32
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	static const KyUInt32 Type_Unset     = 0;
	static const KyUInt32 Type_KyUInt32  = 1;
	static const KyUInt32 Type_KyInt32   = 2;
	static const KyUInt32 Type_KyFloat32 = 3;

public:
	BlobField32() : m_type(Type_Unset), m_value(0) {}

	KyUInt32&  ValueAsUInt32()  { return m_value; }
	KyInt32&   ValueAsInt32()   { return *(KyInt32*)&m_value;  }
	KyFloat32& ValueAsFloat32() { char* value = (char*)&m_value; return *(KyFloat32*)value;  }

public:
	BlobArray<char> m_name;
	KyUInt32 m_type;
	KyUInt32 m_value;
};
inline void SwapEndianness(Endianness::Target e, BlobField32& self)
{
	SwapEndianness(e, self.m_name);
	SwapEndianness(e, self.m_type);
	SwapEndianness(e, self.m_value);
}


class BlobField32Mapping
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	BlobField32Mapping() : m_type(BlobField32::Type_Unset), m_name(KY_NULL), m_value(KY_NULL) {}

	BlobField32Mapping(KyUInt32 type, const char* name, bool* value)      : m_type(type), m_name(name), m_value((char*)value) {}
	BlobField32Mapping(KyUInt32 type, const char* name, KyUInt32* value)  : m_type(type), m_name(name), m_value((char*)value) {}
	BlobField32Mapping(KyUInt32 type, const char* name, KyInt32* value)   : m_type(type), m_name(name), m_value((char*)value) {}
	BlobField32Mapping(KyUInt32 type, const char* name, KyFloat32* value) : m_type(type), m_name(name), m_value((char*)value) {}

	bool&      ValueAsBool()    { return *((bool*)m_value);      }
	KyUInt32&  ValueAsUInt32()  { return *((KyUInt32*)m_value);  }
	KyInt32&   ValueAsInt32()   { return *((KyInt32*)m_value);   }
	KyFloat32& ValueAsFloat32() { return *((KyFloat32*)m_value); }

public:
	KyUInt32 m_type;
	const char* m_name;
	char* m_value; // char* to avoid strict aliasing optimizations
};


template <class OSTREAM>
inline OSTREAM& operator<<(OSTREAM& os, BlobField32Mapping& mapping)
{
	os << mapping.m_name << " : "; 

	switch(mapping.m_type)
	{
		case BlobField32::Type_KyUInt32  : os <<(*(KyUInt32*) (mapping.m_value)); break;
		case BlobField32::Type_KyInt32   : os <<(*(KyInt32*)  (mapping.m_value)); break;
		case BlobField32::Type_KyFloat32 : os <<(*(KyFloat32*)(mapping.m_value)); break;
	}

	return os;
}


class BlobField32Builder : public BaseBlobBuilder<BlobField32>
{
public:
	BlobField32Builder(BlobField32Mapping& mapping) : m_mapping(&mapping) {}

private:
	virtual void DoBuild()
	{
		BLOB_SET(m_blob->m_type, m_mapping->m_type);
		BLOB_STRING(m_blob->m_name, m_mapping->m_name);
		BLOB_SET(m_blob->m_value, m_mapping->ValueAsUInt32());
	}

	BlobField32Mapping* m_mapping;
};




class BlobMultiField32
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	BlobMultiField32() : m_valueType(BlobField32::Type_Unset) {}
	
	KyUInt32 GetCount() const { return m_values.GetCount(); }

	const KyUInt32&  ValueAsUInt32(KyUInt32 idx) const  { return m_values.GetValues()[idx]; }
	const KyInt32&   ValueAsInt32(KyUInt32 idx)  const  { return *(KyInt32*)(&m_values.GetValues()[idx]);  }
	const KyFloat32& ValueAsFloat32(KyUInt32 idx) const { char* value = (char*)(&m_values.GetValues()[idx]); return *(KyFloat32*)value;  }

public:
	BlobArray<char> m_name;
	BlobArray<char> m_category;
	KyUInt32 m_valueType;
	BlobArray<KyUInt32> m_values;
};
inline void SwapEndianness(Endianness::Target e, BlobMultiField32& self)
{
	SwapEndianness(e, self.m_name);
	SwapEndianness(e, self.m_category);
	SwapEndianness(e, self.m_valueType);
	SwapEndianness(e, self.m_values);
}


class BlobMultiField32Mapping
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	BlobMultiField32Mapping() 
		: m_valueType(BlobField32::Type_Unset), m_name(KY_NULL), m_category (KY_NULL), m_values(KY_NULL) {}

	BlobMultiField32Mapping(KyUInt32 type, const char* name, const char* category, KyUInt32 size)
		: m_valueType(type), m_name(name), m_category(category), m_values(size) {}


	KyUInt32 GetCount() const { return m_values.GetCount(); }

	void MapUInt32(KyUInt32 index, const KyUInt32& value)   { m_values[index] = (char*)&value; }
	void MapInt32(KyUInt32 index, const KyInt32& value)     { m_values[index] = (char*)&value; }
	void MapFloat32(KyUInt32 index, const KyFloat32& value) { m_values[index] = (char*)&value; }

	KyUInt32&  ValueAsUInt32(KyUInt32 index)  { return *((KyUInt32*)m_values[index]);  }
	KyInt32&   ValueAsInt32(KyUInt32 index)   { return *((KyInt32*)m_values[index]);   }
	KyFloat32& ValueAsFloat32(KyUInt32 index) { return *((KyFloat32*)m_values[index]); }

	
public:
	KyUInt32 m_valueType;
	const char* m_name;
	const char* m_category;
	KyArrayPOD<char*> m_values;
};


template <class OSTREAM>
inline OSTREAM& operator<<(OSTREAM& os, BlobMultiField32Mapping& mapping)
{
	os << mapping.m_name << "(" << mapping.m_category << ") : "; 

	for (KyUInt32 idx = 0; idx < mapping.GetCount();  ++idx)
	{
		switch(mapping.m_valueType)
		{
		case BlobField32::Type_KyUInt32  : os <<((KyUInt32) (mapping.ValueAsUInt32(idx)));	break;
		case BlobField32::Type_KyInt32   : os <<((KyInt32)  (mapping.ValueAsInt32(idx)));	break;
		case BlobField32::Type_KyFloat32 : os <<((KyFloat32)(mapping.ValueAsFloat32(idx)));	break;
		}

		os << ", ";
	}
	

	return os;
}



class BlobMultiField32Builder : public BaseBlobBuilder<BlobMultiField32>
{
public:
	BlobMultiField32Builder(BlobMultiField32Mapping& mapping) : m_mapping(&mapping) {}

private:
	virtual void DoBuild()
	{
		BLOB_STRING(m_blob->m_name, m_mapping->m_name);
		BLOB_STRING(m_blob->m_category, m_mapping->m_category);
		BLOB_SET(m_blob->m_valueType, m_mapping->m_valueType);
		
		KyUInt32 count = m_mapping->GetCount();
		KyUInt32* blobValues = BLOB_ARRAY(m_blob->m_values, count);
		if (IsWriteMode() == true)
		{
			for (KyUInt32 idx = 0; idx < count; ++idx)
			{
				blobValues[idx] = m_mapping->ValueAsUInt32(idx); 
			}
		}
	}

	BlobMultiField32Mapping* m_mapping;
};


}


#endif

