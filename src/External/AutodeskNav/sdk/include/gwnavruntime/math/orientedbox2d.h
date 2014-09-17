/*
* Copyright 2012 Autodesk, Inc. All rights reserved.
* Use of this software is subject to the terms of the Autodesk license agreement and any attachments or Appendices thereto provided at the time of installation or download,
* or which otherwise accompanies this software in either electronic or hard copy form, or which is signed by you and accepted by Autodesk.
*/


// Primary contact: JUBA - secondary contact: NOBODY
#ifndef Navigation_OrientedBox2d_H
#define Navigation_OrientedBox2d_H


#include "gwnavruntime/math/vec3f.h"
#include "gwnavruntime/basesystem/logger.h"

namespace Kaim
{

/*
     ^       +--------------------+
     |       |                    |
     |width  |                    |
     |       |                    |
     |       |                    |        orientation
     v      A+--------------------+------------->

             <-------------------->
                   length
*/

/// This class represents a three-dimensional bounding box with a free orientation on the (X,Y) plane.
/// It is defined by:
/// -	The position of a single reference corner (#m_a).
/// -	An orientation vector that determines the facing direction of the box from the reference corner (#m_normalizedOrientation).
/// -	A length value, which determines the extents of the box along its orientation vector (#m_length).
/// -	A width value, which determines the extents of the box perpendicular to the orientation vector (#m_width).
/// 	The width of the box always extends to the left when facing the orientation.
/// -	A thickness value, which determines the extents of the box along the vertical Z axis (#m_thickness).
/// For example:
/// \code
///  ^       +--------------------+
///  |       |                    |
///  |width  |                    |
///  |       |                    |
///  |       |                    |        orientation
///  v      A+--------------------+------------->
/// 
///          <-------------------->
///                 length
/// \endcode
class OrientedBox2d
{
	KY_DEFINE_NEW_DELETE_OPERATORS(Stat_Default_Mem)
public:
	// ---------------------------------- Constructors ----------------------------------

	/// Creates a new OrientedBox2d with 0.0f length, width and thickness.
	/// If you use this constructor, you must call Set() to set the dimensions of the rectangle
	/// before it can be used.
	OrientedBox2d() : m_length(0.f), m_width(0.f), m_thickness(0.f) {}

	/// Creates a new OrientedBox2d and initialize it accordingly to provided information.
	/// \param a				The position of the reference corner.
	/// \param orientation		The facing orientation of the box.
	/// \param length			The extents of the box along its orientation vector.
	/// \param width			The extents of the box perpendicular to its orientation vector on the (X,Y) plane.
	/// \param thickness		The extents of the box along the vertical Z axis. 
	OrientedBox2d(const Vec3f& a, const Vec2f& orientation, KyFloat32 length, KyFloat32 width, KyFloat32 thickness) :
	 m_a(a), m_length(length), m_width(width), m_thickness(thickness)
	{
		orientation.GetNormalized(m_normalizedOrientation);
	}


	// ---------------------------------- Main API Functions ----------------------------------

	/// Sets the dimensions of the box.
	/// \param a				The position of the reference corner.
	/// \param orientation		The facing orientation of the box.
	/// \param length			The extents of the box along its orientation vector.
	/// \param width			The extents of the box perpendicular to its orientation vector on the (X,Y) plane.
	/// \param thickness		The extents of the box along the vertical Z axis. 
	void Set(const Vec3f& a, const Vec2f& orientation, KyFloat32 length, KyFloat32 width, KyFloat32 thickness)
	{
		KY_DEBUG_ASSERTN(orientation.GetLength() == Vec2f(orientation).Normalize(), ("Wrong BoxObtacle parameter, the orientation should be normalized"));

		m_a.Set(a.x, a.y, a.z);
		m_normalizedOrientation.Set(orientation.x, orientation.y);
		m_length = length;
		m_width = width;
		m_thickness = thickness;
	}


	// ---------------------------------- Public Data Members ----------------------------------

	Vec3f m_a; ///< The position of the reference corner. 
	Vec2f m_normalizedOrientation; ///< The facing orientation of the box (unit vector). 
	KyFloat32 m_length; ///< The extents of the box along its orientation vector. 
	KyFloat32 m_width; ///< The extents of the box perpendicular to its orientation vector on the (X,Y) plane. 
	KyFloat32 m_thickness; ///< The extents of the box along the vertical Z axis. 
};

}

#endif

