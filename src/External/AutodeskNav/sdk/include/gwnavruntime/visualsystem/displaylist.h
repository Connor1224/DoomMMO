/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// primary contact: BRGR - secondary contact: MAMU
#ifndef Navigation_DisplayList_H
#define Navigation_DisplayList_H

#include "gwnavruntime/visualsystem/displaylistblob.h"
#include "gwnavruntime/visualdebug/visualdebugblobcategory.h"
#include "gwnavruntime/visualsystem/irenderer.h"
#include "gwnavruntime/visualsystem/visualcolor.h"
#include "gwnavruntime/containers/kyarray.h"
#include "gwnavruntime/math/vec3f.h"
#include "gwnavruntime/blob/blobhandler.h"
#include "gwnavruntime/blob/blobarray.h"
#include "gwnavruntime/blob/userdatablobregistry.h"
#include "gwnavruntime/math/transform.h"
#include "gwnavruntime/math/box3f.h"
#include "gwnavruntime/math/triangle3f.h"


namespace Kaim
{

class VisualDebugServer;
class DisplayShapeBlobBuilder;
class World;
class DisplayListMemory;

/// Indicates where a ScopedDisplayList will end
enum DisplayListDestination
{
	DisplayListSentToVoid = 0,                  ///< ScopedDisplayLists will be unused
	DisplayListSentToInGameRender = 1 << 0,     ///< ScopedDisplayLists will be aggregated into the DisplayListManager in order to be rendered in game
	DisplayListSentToVisualDebugServer = 1 << 1 ///< ScopedDisplayLists will be sent through the VisualDebugServer, typically to be viewed in the NavigationLab (recommended)
};


/// Indicates where a ScopedDisplayList will end
enum DisplayListActivation 
{
	DisplayList_Disable, ///< Calls on the ScopedDisplayList will do nothing
	DisplayList_Enable,  ///< The display list is activated
};

const char* GetDisplayListDefaultGroupName();

}

// Define this to totally disable ScopedDisplayLists and DisplayListManager
//#define KY_DISABLE_DISPLAYLIST
#ifndef KY_DISABLE_DISPLAYLIST

namespace Kaim
{

class DisplayListManager;

/// ScopedDisplayList is used to push text, lines or shapes for rendering (locally or remotely)
/// e.g.
/// \code {
///   ScopedDisplayList displayList(GetWorld(), "MyDisplayList", "MygroupOfDisplayLisy", Kaim::DisplayList_Enable);
///   displayList->PushText(m_head, Kaim::VisualColor::Red, "Error");
///   displayList->PushPoint(m_position, Kaim::VisualColor(255, 0, 0));
/// } // Here the displayList is destroyed and committed to be rendered or sent. \endcode
///
class ScopedDisplayList
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_VisualSystem)
	KY_CLASS_WITHOUT_COPY(ScopedDisplayList)

public:
	ScopedDisplayList(DisplayListManager* manager, const char listName[64], const char groupName[64], DisplayListActivation activation);
	ScopedDisplayList(World* world, const char listName[64], const char groupName[64], DisplayListActivation activation); // World's DisplayListManager is implicitly used

	/// Indicates that the displayList must be hidden by default in the NavigationLab, the first time it is created
	/// User should consistently call this function over frame to prevent inconsistent behaviors depending on the connection frame!
	void HideByDefaultInNavigationLab();



	/// Pushes a text.
	void PushText(const Vec3f& pos, const VisualColor& color, const char* text, KyUInt32 textLength = 0);

	/// Pushes a line.
	void PushLine(const Vec3f& P, const Vec3f& Q, const VisualColor& color);

	/// Pushes a single triangle. 
	void PushTriangle(const Vec3f& A, const Vec3f& B, const Vec3f& C, const DisplayShapeColor& color);

	/// Pushes a single triangle. 
	void PushTriangle(const Triangle3f& triangle, const DisplayShapeColor& color);

	/// Pushes a cylinder whose base is centered on P. 
	void PushVerticalCylinder(const Vec3f& P, KyFloat32 radius, KyFloat32 height, KyUInt32 subdivisions, const DisplayShapeColor& color);

	/// Pushes a segment. P and Q are the centers of the two opposite ends, and halfWidth is half the width of the segment.
	void PushSegment(const Vec3f& A, const Vec3f& B, KyFloat32 radius, const DisplayShapeColor& color);

	/// Pushes a flat Quad
	void PushQuad(const Vec3f& A, const Vec3f& B, KyFloat32 radius, const DisplayShapeColor& color);

	/// Pushes a flat Quad
	void PushQuad(const Vec3f& A, const Vec3f& B, const Vec3f& C, const Vec3f& D, const DisplayShapeColor& color);

	/// Pushes an axis-aligned box centered on P. 
	void PushPoint(const Vec3f& P, const DisplayShapeColor& color) { PushPoint(P, 0.1f, color); }

	/// Pushes an axis-aligned box centered on P. 
	void PushPoint(const Vec3f& P, KyFloat32 radius, const DisplayShapeColor& color);

	/// Push a pyramid composed of a base and four sides. P the is center of the base, Q is the peak. 
	void PushPyramid(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth, const DisplayShapeColor& color);

	/// Pushes a hollow tube with a square cross-section. P and Q are at the center of two opposite faces, and the width of the
	/// tube is equal to twicehalfWidth. 
	void PushSquareTubeSegment( const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth, const DisplayShapeColor& color);

	/// Pushes an arrow from P to Q.
	/// \param P                 Starting point of the arrow.
	/// \param Q                 Ending point of the arrow.
	/// \param bodyHalfWidth     Half of the width of the body or trunk of the arrow.
	/// \param color             Color of the arrow.
	/// \param headWidthRatio    The ratio between the width of the head and the width of the body. In other words, the head or point
	///                           of the arrow will be this many times wider than the body or trunk of the arrow.
	///                           \defaultvalue   3.0f.
	/// \param headLengthRatio   The ratio between the length of the head and the length of the body. In other words, the head or point
	///                           of the arrow will be this many times longer than the body or trunk of the arrow.
	///                           \defaultvalue  0.33f.
	void PushArrow(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth, const DisplayShapeColor& color, KyFloat32 headWidthRatio = 3.0f, KyFloat32 headLengthRatio = 0.33f);

	/// Pushes a bezier arrow from P to Q.
	/// \param P                  Starting point of the arrow.
	/// \param Q                  Ending point of the arrow.
	/// \param bodyHalfWidth      Half of the width of the body or trunk of the arrow.
	/// \param color              Color of the arrow.
	/// \param headWidthRatio     The ratio between the width of the head and the width of the body. In other words, the head or point
	///                            of the arrow will be this many times wider than the body or trunk of the arrow.
	///                            \defaultvalue   3.0f
	/// \param directionnality    If 0, has no arrow. If 1, has an arrow at the end. If 2, has an arrow at start and end.
	///                            \defaultvalue   0 
	void PushBezierArrow(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth, const DisplayShapeColor& color, KyFloat32 headWidthRatio = 3.0f, KyUInt32 directionnality = 0);

	/// Pushes a Christmas tree: two orthogonal triangles with the same central axes. P is the center of the base of
	/// both triangles, and Q is at the top of both triangles. Both triangles extend a distance of bodyHalfWidth
	/// from the central axes.
	void PushChristmasTree(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth, const DisplayShapeColor& color);

	/// Pushes a silex from P to Q with the specified bodyHalfWidth. 
	void PushSilex(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth, const DisplayShapeColor& color);

	/// Pushes a double silex from P to Q with the specified bodyHalfWidth. Equivalent to a silex from P to the midpoint
	/// between P and Q, and a silex from Q to the midpoint between P and Q. 
	void PushDoubleSilex(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth, const DisplayShapeColor& color);

	/// Pushes two orthogonal rectangles with the same central axes. P is the center of one end of both rectangles, and Q 
	/// is at the center of the opposite end of both rectangles. Both rectangles extend a distance of bodyHalfWidth
	/// from the central axes. 
	void PushCrossedRectangles(const Vec3f& P, const Vec3f& Q, KyFloat32 bodyHalfWidth, const DisplayShapeColor& color);

	/// Pushes an axis-aligned box whose base is centered on P. 
	void PushColumn(const Vec3f& P, KyFloat32 halfWidth, KyFloat32 height, const DisplayShapeColor& color);

	void PushOrientedBox2d(const Transform& transform, const Box3f& extents, const DisplayShapeColor& color);

	/// Pushes an axis-aligned tetrahedron centered on P and pointing down. 
	void PushPoint_Tetrahedron(const Vec3f& P, KyFloat32 halfWidth, const DisplayShapeColor& color);

	/// Pushes a 2D flat disk centered on P. 
	void PushDisk(const Vec3f& P, KyFloat32 radius, KyUInt32 subdivisions, const DisplayShapeColor& color);

	void PushStadium(const Vec3f& P, const Vec3f& Q, KyFloat32 radius, KyUInt32 subdivisions, const DisplayShapeColor& color);

	/// Pushes an axis-aligned box. 
	void PushBox(const Box3f& box, const DisplayShapeColor& color);

	/// Pushes a hollow tube with a triangular cross-section. P and Q are at the center of two opposite faces, and the width of the
	/// tube is equal to twicehalfWidth. 
	void PushTriangleTubeSegment(const Vec3f& P, const Vec3f& Q, KyFloat32 halfWidth, const DisplayShapeColor& color);

	/// Pushes a three-dimensional oriented box.
	/// \param P             A point at one end of the box.
	/// \param Q             A point at the opposite end of the box.
	/// \param upHeight      The distance of the top of the box above P and Q.
	/// \param downHeight    The distance of the bottom of the box below P and Q.
	/// \param halfWidth     The distance of the sides of the box on either side ofP and Q. 
	/// \param color         Color of the wall.
	void PushWall(const Vec3f& P, const Vec3f& Q, KyFloat32 upHeight, KyFloat32 downHeight, KyFloat32 halfWidth, const DisplayShapeColor& color);

	void PushLadder(const Transform& transform, const Box3f& entrances, KyFloat32 ladderWidth, KyFloat32 rungsThickness, KyFloat32 rungsGapSize, const DisplayShapeColor& shapeColor);

	void PushParabola(const Kaim::Vec3f& P, const Kaim::Vec3f& Q, KyFloat32 heightBias, KyUInt32 subdivisions, const DisplayShapeColor& color);

	void PushFlag(const Vec3f& P, KyFloat32 height, KyFloat32 radius, const DisplayShapeColor& color);

	// ...


private:
	void PushSegmentBasedShape(const Vec3f& P, const Vec3f& Q, KyFloat32 size, const DisplayShapeColor& color, KyUInt32 displayShapeType);
	void PushCurveBasedShape(const Vec3f& P, const Vec3f& Q, KyFloat32 size, KyFloat32 subdivisions, const DisplayShapeColor& color, KyUInt32 displayShapeType);
	void PushText(DisplayText& text);
	void PushShape(DisplayShape& shape);
	

public: // internal
	~ScopedDisplayList();
	void Commit(); ///< Commit the content of the ScopedDisplayList.

	DisplayListManager* GetManager() { return m_manager; }

public: // internal
	const DisplayListActivation m_activation;

	DisplayListManager* m_manager;
	DisplayListMemory* m_memory; // memory assigned by the manager
};


// This class is used for ScopedDisplayLists to store them for future rendering or to send them for remote rendering.
// It also tracks the memory used by ScopedDisplayList and reuse it for next scopedDisplayList saving some allocations and deallocations.
class DisplayListManager
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_VisualSystem)

public:
	DisplayListManager();
	~DisplayListManager();

	void SetVisualDebugServer(VisualDebugServer* visualDebugServer) { m_visualDebugServer = visualDebugServer; }
	void SetInGameRenderNode(IRenderNode* renderNode)      { m_renderNode = renderNode; }	

	IRenderNode* GetRenderNode() { return m_renderNode; }

	bool IsRendererSupported() { return m_renderNode != KY_NULL; }
	bool IsVisualDebugSupported();

private:
	IRenderNode* m_renderNode;
	VisualDebugServer* m_visualDebugServer;

	KyArrayPOD<char> m_displayLists; // buffer containing the display list blobs
	BlobHandler<DisplayListBlob> m_displayListBlobHandler; // shared handler used to create all display list blobs
	KyArray<DisplayListMemory*> m_displayListMemories; // all memories used or free, thence if possible a display list does not allocate more memory than already used 

public: // internal
	DisplayListMemory* GetUnusedMemory(); // return the first unused memory in m_displayListMemories
	void PushMemory(DisplayListMemory*); // used by a display list to add the content of its assigned memory to m_displayLists
	void ReleaseMemory(DisplayListMemory*); // register this memory has free so that it can be reused by another display list

	void PushBlob(const char* displayListBlob, KyUInt32 blobsize);

	DisplayListBlob* GetNext(KyUInt32& sizeRead);
	void SubmitToRenderer(); // submit the content of m_displayLists
	void Clear();  // clear the content of m_displayLists
};

}

#else
// Here we use a separated file in order to easily maintain both versions by comparing the two files (displaylist.h and disableddisplaylist.inl)
#include "gwnavruntime/visualsystem/disableddisplaylist.inl"
#define ScopedDisplayList DisabledScopedDisplayList
#define DisplayListManager DisabledDisplayListManager
#endif // KY_DISABLE_DISPLAYLIST


#endif // Navigation_DisplayList_H
