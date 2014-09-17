/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// primary contact: GUAL - secondary contact: NOBODY

#ifndef Navigation_Path_H
#define Navigation_Path_H

#include "gwnavruntime/containers/kyarray.h"
#include "gwnavruntime/navmesh/identifiers/navtriangleptr.h"
#include "gwnavruntime/navgraph/identifiers/navgraphvertexptr.h"
#include "gwnavruntime/navgraph/identifiers/navgraphedgeptr.h"

namespace Kaim
{

/// Defines the different kind of PathEdge within a Path.
enum PathEdgeType
{
	PathEdgeType_Undefined  , ///< The PathEdgeType has not yet been set for the corresponding PathEdge.
	PathEdgeType_OnNavMesh  , ///< The PathEdge has been computed on the NavMesh.
	PathEdgeType_OnNavGraph , ///< The PathEdge corresponds to a NavGraph edge.
	PathEdgeType_FromOutside, ///< This PathEdge is coming from outside the NavMesh.
	PathEdgeType_ToOutside     ///< The PathEdge is going to outside the NavMesh.
};


/// The class representing a path.
class Path
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_Path)
	KY_CLASS_WITHOUT_COPY(Path)
	Path();

public:
	/// A class that configures the creation of a Path.
	class CreateConfig
	{
	public:
		// ---------------------------------- Public Members ----------------------------------
		CreateConfig(KyUInt32 nodesCount = 0) : m_nodesCount(nodesCount) {}

		KyUInt32 m_nodesCount;
	};


	// ---------------------------------- Path Creation ----------------------------------

	/// Creates a Path instance and allocates necessary memory accordingly to
	/// the information provided through createConfig.
	static Ptr<Path> CreatePath(const CreateConfig& createConfig);


	// ---------------------------------- Get Counts ----------------------------------

	KyUInt32 GetNodeCount() const;
	KyUInt32 GetEdgeCount() const;


	// ---------------------------------- Get Node Data ----------------------------------

	const Vec3f& GetPathStartPosition() const;
	const Vec3f& GetPathEndPosition() const;

	const Vec3f* GetNodePositionBuffer() const;
	const Vec3f& GetNodePosition(KyUInt32 nodeIdx) const;
	      Vec3f& GetNodePosition(KyUInt32 nodeIdx);

	const NavTrianglePtr& GetNodeNavTrianglePtr(KyUInt32 nodeIdx) const;
	      NavTrianglePtr& GetNodeNavTrianglePtr(KyUInt32 nodeIdx);

	// ---------------------------------- Get Edge Data ----------------------------------

	const Vec3f& GetPathEdgeStartPosition(KyUInt32 edgeIdx) const;
	const Vec3f& GetPathEdgeEndPosition  (KyUInt32 edgeIdx) const;

	const NavGraphEdgePtr& GetEdgeNavGraphEdgePtr(KyUInt32 edgeIdx) const;
	      NavGraphEdgePtr& GetEdgeNavGraphEdgePtr(KyUInt32 edgeIdx);

	PathEdgeType GetPathEdgeType(KyUInt32 edgeIdx) const;

	// ---------------------------------- Set Node Data ----------------------------------

	void SetNodePosition       (KyUInt32 nodeIdx, const Vec3f& position               );
	void SetNodeNavTrianglePtr (KyUInt32 nodeIdx, const NavTrianglePtr& navTrianglePtr);

	// ---------------------------------- Set Edge Data ----------------------------------

	void SetEdgeNavGraphEdgePtr(KyUInt32 edgeIdx, const NavGraphEdgePtr& navGraphEdgePtr);
	void SetPathEdgeType       (KyUInt32 edgeIdx, PathEdgeType edgeType                 );

private :
	static KyUInt32 ComputeByteSize(const CreateConfig& createConfig);

	void InitBuffers();
	void ClearBuffers();

	template <class T>
	void ClearBufferCPP(T*& buffer, KyUInt32 count);
	template <class T>
	void ClearBufferPOD(T*& buffer, KyUInt32 count);
	template <class T>
	void InitBufferCPP(T*& buffer, KyUInt32 count, char*& memory);
	template <class T>
	void InitBufferPOD(T*& buffer, KyUInt32 count, char*& memory);


public:
	// ---------------------------------- Public Data Members ----------------------------------

	/// The Database on which the Path has been computed. If you created the Path
	/// by yourself and want a Bot follow it you must set #m_database to the Database
	/// on which the Bot will move.
	Database* m_database;


private:
	KyUInt32 m_byteSize;
	KyUInt32 m_flags;

	KyUInt32 m_nodesCount;
	KyUInt32 m_edgesCount;

	Vec3f*          m_nodePositions;
	NavTrianglePtr* m_nodetrianglePtrs;

	NavGraphEdgePtr* m_edgeNavGraphEdgePtr;
	KyUInt8*         m_edgeTypes;

	KyUInt32 RefCount; // consider thread-safetyness here with mutable AtomicInt<int> RefCount;

public: // internal
	// Lifetime - separate implementation due to Create function
	void AddRef();
	void Release();
	// Debug method only.
	int GetRefCount() const { return RefCount; }
};


}

#include "gwnavruntime/path/path.inl"



#endif
