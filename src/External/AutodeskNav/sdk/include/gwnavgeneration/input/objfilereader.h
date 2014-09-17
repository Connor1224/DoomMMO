/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: JAPA - secondary contact: GUAL
#ifndef GwNavGen_ObjFileReader_H
#define GwNavGen_ObjFileReader_H


#include "gwnavruntime/math/vec3f.h"
#include "gwnavruntime/containers/kyarray.h"
#include "gwnavruntime/kernel/SF_String.h"
#include "gwnavruntime/math/box3f.h"


namespace Kaim
{

class File;
class GeneratorSystem;
class FileOpenerBase;

/// A basic parser of .obj files in text format able to read vertices, triangles and materials
class ObjFileReader
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
	KY_CLASS_WITHOUT_COPY(ObjFileReader)
public:
	ObjFileReader();

	~ObjFileReader();

	/// Read Obj file directly from specified filename. If sys != KY_NULL, read Obj file using GeneratorSystem root path.
	KyResult ReadObjFile(const char* objFileName, FileOpenerBase* fileOpener, GeneratorSystem* sys = KY_NULL);

	// ---------------------------------- Parsed data ----------------------------------
	KyUInt32 VerticesCount() const { return m_vertices.GetCount();   }
	const Vec3f* Vertices()  const { return m_vertices.GetDataPtr(); }

	KyUInt32 IndicesCount()   const { return m_indices.GetCount();   }
	const KyUInt32* Indices() const { return m_indices.GetDataPtr(); }

	KyUInt32 MaterialIndicesCount()   const { return m_materialIndices.GetCount();   }
	const KyUInt32* MaterialIndices() const { return m_materialIndices.GetDataPtr(); }

	KyUInt32 GetFacesCount() const { return m_facesCount; }
	KyUInt32 GetFaceMinVertexCount() const { return m_faceMinVertexCount; }
	KyUInt32 GetFaceMaxVertexCount() const { return m_faceMaxVertexCount; }

	// ---------------------------------- Misc ----------------------------------
	void Clear();

	/// Will browse all the vertices extracted from file and compute a bounding box from them
	Box3f ComputeBoundingBox() const;

	/// Cache the filename associated with is reader in case we need to read again
	const char* GetFileName() { return m_currentFileName; }

public:
	KyArray<String> m_materialNames;
	KyUInt32 m_primitiveWithoutMaterialCount; ///< The number of primitives (eg: triangles) that are not associated to any material.

private:
	KyResult ReadFileContent(const char* objFileName, FileOpenerBase* fileOpener, GeneratorSystem* sys);
	KyResult ReadPosition(char* str, Vec3f& pos);
	KyResult ReadFace(char* str);
	KyResult ParseMaterialNames(File* file);

	/* Return the materialName index in the array.*/
	KyUInt32 RecordMaterialName(const String& materialName);
	void RemoveMaterialName(const String& materialName);
	void ClearMaterialNames() { m_materialNames.Clear(); }

	KyResult ReadContent(File* file);

	char* ReadLine();

private:
	char* m_line;
	char* m_fileIOBuffer;
	const char* m_waterMark;
	const char* m_currentFileName;
	char* m_currentPosInBuffer;

	const KyUInt32 m_maxLineSize;
	const KyUInt32 m_maxBufferSize;

	KyUInt32 m_currentMaterialIndex;
	KyUInt32 m_facesCount;
	KyUInt32 m_faceMinVertexCount;
	KyUInt32 m_faceMaxVertexCount;

	// current file data
	KyArray<Vec3f> m_vertices;
	KyArrayPOD<KyUInt32> m_indices;
	KyArrayPOD<KyUInt32> m_materialIndices;
	KyArrayPOD<KyUInt32> m_faceIndices;
};


}


#endif

