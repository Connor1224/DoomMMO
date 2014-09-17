/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/



// primary contact: BRGR - secondary contact: MAMU
#ifndef Navigation_DisabledDisplayList_H
#define Navigation_DisabledDisplayList_H

// We use a separated file from displaylist.h in order to easily maintain both versions by comparing the two files (displaylist.h and disableddisplaylist.inl)
#ifdef KY_DISABLE_DISPLAYLIST

namespace Kaim
{

class DisabledDisplayListManager;

// ScopedDisplayList is used to push text, lines or shapes for rendering (locally or remotely)
// e.g.
// {
//   ScopedDisplayList displayList(GetWorld(), "MyDisplayList", Kaim::DisplayList_Enable);
//   displayList->PushText(m_head, Kaim::VisualColor::Red, "Error");
//   displayList->PushPoint(m_position, Kaim::VisualColor(255, 0, 0));
// } // Here the displayList is destroyed and committed to be rendered or sent.
//
class DisabledScopedDisplayList
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_VisualSystem)
	KY_CLASS_WITHOUT_COPY(DisabledScopedDisplayList)

public:
	DisabledScopedDisplayList(DisabledDisplayListManager*, const char*, const char*, DisplayListActivation) {}
	DisabledScopedDisplayList(World*, const char*, const char*, DisplayListActivation) {}

	// Indicates that the displayList must be hidden by default in the NavigationLab, the first time it is created
	// User should consistently call this function over frame to prevent inconsistent behaviors depending on the connection frame!
	KY_INLINE void HideByDefaultInNavigationLab() {}



	// Text
	KY_INLINE void PushText(const Vec3f&, const VisualColor&, const char*, KyUInt32 = 0) {}

	// Line
	KY_INLINE void PushLine(const Vec3f&, const Vec3f&, const VisualColor&) {}

	/// Pushes a single triangle. 
	KY_INLINE void PushTriangle(const Vec3f&, const Vec3f&, const Vec3f&, const DisplayShapeColor&) {}
	KY_INLINE void PushTriangle(const Triangle3f&, const DisplayShapeColor&) {}

	/// Pushes a cylinder whose base is centered on P. 
	KY_INLINE void PushVerticalCylinder(const Vec3f&, KyFloat32, KyFloat32, KyUInt32, const DisplayShapeColor&) {}

	/// Pushes a line segment.P and Q are the centers of the two opposite ends, and halfWidth is half the width of the segment.
	KY_INLINE void PushSegment(const Vec3f&, const Vec3f&, KyFloat32, const DisplayShapeColor&) {}

	/// Pushes two triangles that form a quad. 
	KY_INLINE void PushQuad(const Vec3f&, const Vec3f&, const Vec3f&, const Vec3f&, const DisplayShapeColor&) {}

	/// Pushes an axis-aligned box centered on P. 
	KY_INLINE void PushPoint(const Vec3f&, const DisplayShapeColor&) {}

	/// Pushes an axis-aligned box centered on P. 
	KY_INLINE void PushPoint(const Vec3f&, KyFloat32, const DisplayShapeColor&) {}

	/// Push a pyramid composed of a base and four sides.P the is center of the base,Q is the peak. 
	KY_INLINE void PushPyramid(const Vec3f&, const Vec3f&, KyFloat32, const DisplayShapeColor&) {}

	/// Pushes a hollow tube with a square cross-section.P and Q are at the center of two opposite faces, and the width of the
	/// tube is equal to twicehalfWidth. 
	KY_INLINE void PushSquareTubeSegment( const Vec3f&, const Vec3f&, KyFloat32, const DisplayShapeColor&) {}

	/// Pushes an arrow fromP toQ.
	/// \param bodyHalfWidth    Half of the width of the body or trunk of the arrow. 
	/// The ratio between the width of the head and the width of the body. In other words, the head or point
	/// of the arrow will be this many times wider than the body or trunk of the arrow.
	/// \defaultvalue    3.0f
	/// The ratio between the length of the head and the length of the body. In other words, the head or point
	/// of the arrow will be this many times longer than the body or trunk of the arrow.
	/// \defaultvalue    0.33f 
	KY_INLINE void PushArrow(const Vec3f&, const Vec3f&, KyFloat32, const DisplayShapeColor&, KyFloat32 = 3.0f, KyFloat32 = 0.33f) {}

	/// Pushes a bezier arrow fromP toQ.
	/// \param bodyHalfWidth      Half of the width of the body or trunk of the arrow. 
	/// \param headWidthRatio     The ratio between the width of the head and the width of the body. In other words, the head or point
	///                            of the arrow will be this many times wider than the body or trunk of the arrow.
	///                            \defaultvalue    3.0f
	/// \param directionnality    If 0, has no arrow. If 1, has an arrow at the end. If 2, has an arrow at start and end.
	///                            \defaultvalue    0 
	KY_INLINE void PushBezierArrow(const Vec3f&, const Vec3f&, KyFloat32, const DisplayShapeColor&, KyFloat32 = 3.0f, KyUInt32 = 0) {}

	/// Pushes a Christmas tree: two orthogonal triangles with the same central axes.P is the center of the base of
	/// both triangles, and Q is at the top of both triangles. Both triangles extend a distance of bodyHalfWidth
	/// from the central axes.
	KY_INLINE void PushChristmasTree(const Vec3f&, const Vec3f&, KyFloat32, const DisplayShapeColor&) {}

	/// Pushes a silex fromP toQ with the specified bodyHalfWidth. 
	KY_INLINE void PushSilex(const Vec3f&, const Vec3f&, KyFloat32, const DisplayShapeColor&) {}

	/// Pushes a double silex fromP toQ with the specified bodyHalfWidth. Equivalent to a silex fromP to the midpoint
	/// between P and Q, and a silex fromQ to the midpoint between P and Q. 
	KY_INLINE void PushDoubleSilex(const Vec3f&, const Vec3f&, KyFloat32, const DisplayShapeColor&) {}

	/// Pushes two orthogonal rectangles with the same central axes.P is the center of one end of both rectangles, and Q 
	/// is at the center of the opposite end of both rectangles. Both rectangles extend a distance of bodyHalfWidth
	/// from the central axes. 
	KY_INLINE void PushCrossedRectangles(const Vec3f&, const Vec3f&, KyFloat32, const DisplayShapeColor&) {}

	/// Pushes an axis-aligned box whose base is centered on P. 
	KY_INLINE void PushColumn(const Vec3f&, KyFloat32, KyFloat32, const DisplayShapeColor&) {}

	KY_INLINE void PushOrientedBox2d(const Transform&, const Box3f&, const DisplayShapeColor&) {}

	/// Pushes an axis-aligned tetrahedron centered on P and pointing down. 
	KY_INLINE void PushPoint_Tetrahedron(const Vec3f&, KyFloat32, const DisplayShapeColor&) {}

	/// Pushes a 2D flat disk centered on P. 
	KY_INLINE void PushDisk(const Vec3f&, KyFloat32, KyUInt32, const DisplayShapeColor&) {}

	KY_INLINE void PushStadium(const Vec3f&, const Vec3f&, KyFloat32, KyUInt32, const DisplayShapeColor&) {}

	/// Pushes an axis-aligned box. 
	KY_INLINE void PushBox(const Box3f&, const DisplayShapeColor&) {}

	/// Pushes a hollow tube with a triangular cross-section.P and Q are at the center of two opposite faces, and the width of the
	/// tube is equal to twicehalfWidth. 
	KY_INLINE void PushTriangleTubeSegment(const Vec3f&, const Vec3f&, KyFloat32, const DisplayShapeColor&) {}

	/// Pushes a three-dimensional oriented box.
	/// \param P			A point at one end of the box.
	/// \param Q			A point at the opposite end of the box.
	/// \param upHeight		The distance of the top of the box above P and Q.
	/// \param downHeight	The distance of the bottom of the box below P and Q
	/// \param halfWidth	The distance of the sides of the box on either side ofP and Q. 
	KY_INLINE void PushWall(const Vec3f&, const Vec3f&, KyFloat32, KyFloat32, KyFloat32, const DisplayShapeColor&) {}

	KY_INLINE void PushLadder(const Transform&, const Box3f&, KyFloat32, KyFloat32, KyFloat32, const DisplayShapeColor&) {}

	KY_INLINE void PushParabola(const Kaim::Vec3f&, const Kaim::Vec3f&, KyFloat32, KyUInt32, const DisplayShapeColor&) {}

	KY_INLINE void PushFlag(const Vec3f&, KyFloat32, KyFloat32, const DisplayShapeColor&) {}

	// ...


private:

public: // internal
	~DisabledScopedDisplayList() {}
	void Commit() {}

	KY_INLINE DisabledDisplayListManager* GetManager() { return KY_NULL; }

public: // internal
};


// This class is used for ScopedDisplayLists to store them for future rendering or to send them for remote rendering.
// It also tracks the memory used by ScopedDisplayList and reuse it for next scopedDisplayList saving some allocations and deallocations.
class DisabledDisplayListManager
{
	KY_DEFINE_NEW_DELETE_OPERATORS(MemStat_VisualSystem)

public:
	DisabledDisplayListManager() {}
	~DisabledDisplayListManager() {}

	KY_INLINE void SetInGameRenderNode(IRenderNode*) {}
	KY_INLINE void SetVisualDebugServer(VisualDebugServer*) {}

	IRenderNode* GetRenderNode() { return KY_NULL; }

	KY_INLINE bool IsRendererSupported() { return false; }
	KY_INLINE bool IsVisualDebugSupported() { return false; }

private:

public: // internal
	KY_INLINE DisplayListMemory* GetUnusedMemory() { return KY_NULL; }
	KY_INLINE void PushMemory(DisplayListMemory*) {}
	KY_INLINE void ReleaseMemory(DisplayListMemory*) {}

	KY_INLINE void PushBlob(const char*, KyUInt32) {}

	KY_INLINE DisplayListBlob* GetNext(KyUInt32&) { return KY_NULL; }
	KY_INLINE void SubmitToRenderer() {}
	KY_INLINE void Clear() {}
};

}

#else
KY_COMPILE_TIME_MESSAGE("Please include displaylist.h instead of disableddisplaylist.inl")
//KY_COMPILER_ASSERT(0) // make sure this doesn't compile
#endif // KY_DISABLE_DISPLAYLIST

#endif // Navigation_DisabledDisplayList_H
